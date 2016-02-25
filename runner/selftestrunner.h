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
#ifndef SELFTESTRUNNER_H
#define SELFTESTRUNNER_H

#include <iostream>
#include "abstractrunner.h"
#include "hashes/sha0.h"

/**
 * @brief The SelftestRunner class
 */
class SelftestRunner : public AbstractRunner
{
    public:
        explicit SelftestRunner() = default;
        virtual ~SelftestRunner() = default;

        /**
         * @brief run
         * @return
         */
        virtual std::error_code run() {
            std::array<unsigned char, 3> buf = {{ 'a', 'b', 'c' }};

            Hash::SHA0 hash;
            hash.update(buf);

            Hash::SHA0::digest_t digest;
            hash.digest(digest);

            auto result = xclm::toHexString(digest);
            auto valid = DIGEST0 == result;
            std::cout << "Calculated: " << result << std::endl;
            std::cout << "Should be : " << DIGEST0 << std::endl;
            std::cout << "Valid     : " << std::boolalpha << valid << std::endl;

            return valid ? NOERROR : xclm::make_error_code(xclm::errc::selftest_failed);
        }

    private:
        const std::string DIGEST0 = "0164b8a914cd2a5e74c4f7ff082c4d97f1edf880";
};

#endif // SELFTESTRUNNER_H
