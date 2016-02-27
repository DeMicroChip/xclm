/*
 * The MIT License (MIT)
 *
 * Copyright (c)2015-2016, does not matter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ABSTRACTPATCHER_H
#define ABSTRACTPATCHER_H

#include <cerrno>                       // for errno
#include <algorithm>                    // for search
#include <array>                        // for array
#include <boost/filesystem/path.hpp>    // for path
#include <fstream>                      // for ifstream, ios, etc
#include <iterator>                     // for distance
#include <string>                       // for streamsize, string
#include <system_error>                 // for error_code, etc
#include "hashes/sha0.h"                // for SHA0, SHA0::digest_t
#include "xclm_error.h"                 // for make_error_code, NOERROR, etc

/**
 * @brief base class for all file base operation
 */
class AbstractRunner
{
    public:
        explicit AbstractRunner() = default;
        virtual ~AbstractRunner() = default;

        /**
         * @brief run
         * @return
         */
        virtual std::error_code run() = 0;

        /**
         * @brief MAX_READ_SIZE  64kb
         */
        static constexpr std::streamsize  MAX_READ_SIZE = 1024 * 64;

    protected:
        /**
         * @brief checkNeededFilesExists
         * @param compilerBinDir
         * @return
         */
        virtual std::error_code checkNeededFilesExists(const boost::filesystem::path &compilerBinDir) const;

        /**
         * @brief calc digest in MICROCHIP mode about given file
         * @param progName
         * @param digest
         * @return
         */
        virtual std::error_code hashing(const boost::filesystem::path &progName, Hash::SHA0::digest_t &digest) const;

        /**
         * @brief findOffset
         * @param progName
         * @param offset
         * @return
         */
        template<typename _FileSystem, typename _Offset, typename _Container>
        std::error_code findOffset(const _FileSystem &progName, _Offset &offset, const _Container& toSearch) const {
            std::ifstream  readFile(progName.string(), std::ios::binary | std::ios::in);
            if (readFile.bad())
                return std::error_code(errno, std::generic_category());

            const auto &fileLength = streamSize(readFile);

            std::array<char, MAX_READ_SIZE>  buf;
            for (std::streamsize i = fileLength; i > 0;) {
                auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;

                buf.fill(0x00);
                readFile.read(buf.data(), readSize);

                const auto &it = std::search(buf.cbegin(), buf.cend(), std::begin(toSearch), std::end(toSearch));
                if (it != buf.cend()) { // found
                    offset = _Offset((fileLength - i) + std::distance(buf.cbegin(), it));
                    return NOERROR;
                }

                i -= readSize;
                if (i > toSearch.size()) { // jump hash size back for a full search
                    i += toSearch.size();
                    readFile.seekg(-toSearch.size(), std::ios::cur);
                }
            }
            return xclm::make_error_code(xclm::errc::digest_not_found);
        }

        /**
         * @brief get size of file
         * @param stream
         */
        template<typename _Stream>
        std::streamsize streamSize(_Stream& stream) const {
            auto cur = stream.tellg();
            stream.seekg(0, std::ios::end);
            auto pos = stream.tellg();
            stream.seekg(cur, std::ios::beg);
            return pos;
        }

    protected:
        static const std::array<std::string, 4>  filesToPatch;
};

#endif // ABSTRACTPATCHER_H
