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
#include <iostream>
#include <fstream>
#include "abstractrunner.h"

/**
 * @brief files to patch
 */
const std::array<std::string, 4>  AbstractRunner::filesToPatch = {{ "picc", "picc18", "cgpic", "cgpic18" }};

/**
 * @brief AbstractPatcher::checkNeededFilesExists
 * @param compilerBinDir
 * @return
 */
std::error_code AbstractRunner::checkNeededFilesExists(const boost::filesystem::path &compilerBinDir) const
{
    xclm::boost::system::error_code  ec;
    for(const auto& f : filesToPatch)
    {
        auto fileToCheck(compilerBinDir / f);
        if (!boost::filesystem::exists(fileToCheck, ec))
        {
            std::cerr << u8"[ERROR](" << fileToCheck.string() << ") " << ec.message() << ". Code: " << ec.value() << std::endl;
            return ec;
        }
    }
    return NOERROR;
}

/**
 * @brief calc digest in MICROCHIP mode about given file
 * @param progName
 * @return
 */
std::error_code AbstractRunner::hashing(const boost::filesystem::path &progName, Hash::SHA0::digest_t &digest) const
{
    std::ifstream  readFile(progName.string(), std::ios::binary | std::ios::in);
    if (readFile.bad())
        return std::error_code(errno, std::generic_category());

    const auto fileLength = streamSize(readFile);

    Hash::SHA0 md(Hash::SHA0_MODE::MICROCHIP);

    std::array<char, MAX_READ_SIZE>  buf;
    for (auto i = fileLength; i > 0;)
    {
        auto readSize = i > MAX_READ_SIZE ? MAX_READ_SIZE : i;
        readFile.read(buf.data(), readSize);
        md.update(buf.data(), readSize);
        i -= readSize;
    }

    md.digest(digest);

    return NOERROR;
}
