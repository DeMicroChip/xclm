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
#ifndef VUNKNOWN_H
#define VUNKNOWN_H

#include <string>
#include "v202.h"

class XclmUnknown : public XclmV202
{
    public:
        explicit XclmUnknown(const std::string& digest)
        : XclmV202(),
          XCLM_UNKNOWN_DIGEST(digest) {
        }
        virtual ~XclmUnknown() = default;

        virtual const std::string& xclmDigest() const override {
            return XCLM_UNKNOWN_DIGEST;
        }

        virtual const std::string&  name() const override {
            return VERSION;
        }

    private:
        const std::string XCLM_UNKNOWN_DIGEST;
        const std::string VERSION = "xclm (unknown)";
};

#endif // VUNKNOWN_H
