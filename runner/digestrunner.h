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
#ifndef DIGESTRUNNER_H
#define DIGESTRUNNER_H

#include "abstractrunner.h"
#include <iostream>
#include <boost/filesystem/operations.hpp>

/**
 * @brief The DigestRunner class
 */
class DigestRunner : public AbstractRunner
{
    public:
        explicit DigestRunner(const boost::filesystem::path &file)
        : AbstractRunner(),
          fileToHash(file) {
        }
        virtual ~DigestRunner() = default;

        /**
         * @brief run
         * @return
         */
        virtual std::error_code run() {
            xclm::boost::system::error_code  bec;
            if (!boost::filesystem::exists(fileToHash, bec)) {
                std::cerr << u8"[ERROR](" << fileToHash.string() << ") " << bec.message() << ". Code: " << bec.value() << std::endl;
                return bec;
            }

            Hash::SHA0::digest_t digest;
            auto ec = hashing(fileToHash, digest);
            if (ec) {
                std::cerr << u8"[ERROR] Unable to calculate digest about " << fileToHash.string() << ". " << ec.message() << ". Code: " << ec.value() << std::endl;
                return ec;
            }
            std::cout << "Digest about '" << fileToHash.string() << "' : " << xclm::toHexString(digest) << std::endl;

            return NOERROR;
        }

    private:
        const boost::filesystem::path  fileToHash;
};

#endif // DIGESTRUNNER_H
