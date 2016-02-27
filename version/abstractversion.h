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
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <ostream>
#include <iterator>
#include <cstdint>
#include <cstdio>
#include "logging.h"
#include "util.h"

/**
 * @brief abstract base class of known version of xclm
 */
class AbstractVersion
{
    public:
        explicit AbstractVersion() = default;
        virtual ~AbstractVersion() = default;

        /**
         * @brief print count of diggest at offset to stream
         * @param stream
         * @param offset
         * @param count
         */
        template<typename _Stream, typename _Offset, typename _Count>
        void printDigest(_Stream &stream, const _Offset &offset, const _Count &count) const {
            _Offset index = offset;
            LOG << u8"Dump hash at offset " << offset << std::endl;
            for (_Count i = 0; i <= count; i++) {
                index %= hiddenHashTable().size();
                stream << hiddenHashTable()[index];
                index++;
            }
            stream << std::endl;
        }

        virtual const std::string&  hiddenHashTable() const = 0;
        virtual const std::string&  xclmDigest() const = 0;
        virtual const std::string&  name() const = 0;
};

#endif // HASHTABLE_H
