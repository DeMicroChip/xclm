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
#ifndef __SHA0_H__
#define __SHA0_H__

#include <array>
#include <cstddef>
#include <cstdint>

namespace Hash
{
    class SHA0
    {
        public:
            // default SHA0 mode
            explicit SHA0();
            virtual ~SHA0() = default;

            /* Size of hash in bytes */
            static constexpr auto digest_size = 20U;
            typedef uint8_t  value_type;
            typedef std::array<value_type, SHA0::digest_size> digest_t;

            /* Re-initialize the class. */
            void reset();

            /* Returns the message digest. */
            bool digest(digest_t &digest);

            /* Provide input. */
            void update(const void* message, int len);

            /* Provide input. */
            template<typename T>
            void update(const T& data) {
                update(data.data(), data.size());
            }

        protected:
            virtual void expandW(std::array<uint32_t, 80> &W) const {
                for(uint32_t t = 16; t < 80; t++) {
                    W[t] = W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16];
                }
            }

            template<typename _UINT32 = uint32_t>
            void _sbox(_UINT32 &A, _UINT32 &B, _UINT32 &C, _UINT32 &D, _UINT32 &E, _UINT32 temp) const {
                temp &= 0xFFFFFFFF;
                E = D;
                D = C;
                C = leftrotate(30,B);
                B = A;
                A = temp;

            }

            virtual void sbox1(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 0; t < 20; t++) {
                    uint32_t temp  = leftrotate(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + 0x5A827999;
                    _sbox(A, B, C, D, E, temp);
                }
            }

            virtual void sbox2(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 20; t < 40; t++) {
                    uint32_t temp  = leftrotate(5,A) + (B ^ C ^ D) + E + W[t] + 0x6ED9EBA1;
                    _sbox(A, B, C, D, E, temp);
                }
            }

            virtual void sbox3(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 40; t < 60; t++) {
                    uint32_t temp  = leftrotate(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + 0x8F1BBCDC;
                    _sbox(A, B, C, D, E, temp);
                }
            }

            virtual void sbox4(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t &E, const std::array<uint32_t, 80> &W) const {
                for (uint32_t t = 60; t < 80; t++) {
                    uint32_t temp  = leftrotate(5,A) + (B ^ C ^ D) + E + W[t] + 0xCA62C1D6;
                    _sbox(A, B, C, D, E, temp);
                }
            }

            /* Process the next 512 bits of the message */
            void process_message_block();

            /* Pads the current message block to 512 bits */
            void pad_message();

            /* Performs a circular left shift operation */
            template<typename _INT32 = int32_t, typename _UINT32 = uint32_t>
            _UINT32 leftrotate(_INT32 bits, _UINT32 word) const {
                return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32 - bits));
            }

        private:
            /* Message digest buffers */
            std::array<uint32_t, 5> H;

            /* Message length in bits */
            uint32_t length_low;

            /* Message length in bits */
            uint32_t length_high;

            /* 512-bit message blocks */
            std::array<uint8_t, 64> message_block;

            /* Index into message block array */
            int message_block_index;

            /* Is the digest computed? */
            bool computed;

            /* Is the message digest corrupted? */
            bool corrupted;
    };
}

#endif /*  __SHA0_H__ */
