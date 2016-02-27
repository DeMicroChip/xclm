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
#ifndef SHA0MICROCHIP_H
#define SHA0MICROCHIP_H

#include "sha0.h"
#include <array>
#include <cstdint>

namespace Hash
{
    class SHA0MicroChip : public SHA0
    {
        public:
            explicit SHA0MicroChip() = default;
            virtual ~SHA0MicroChip() = default;

        protected:
            virtual void sbox3(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 40; t < 60; t++) {
                    uint32_t temp  = leftrotate(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] - 0x70E44324;  // modified value and sub op
                    _sbox(A, B, C, D, E, temp);
                }
            }

            virtual void sbox4(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 60; t < 80; t++) {
                    uint32_t temp  = leftrotate(5,A) + (B ^ C ^ D) + E + W[t] - 0x359D3E2A; // modified value and sub op
                    _sbox(A, B, C, D, E, temp);
                }
            }
    };
}

#endif // SHA0MICROCHIP_H
