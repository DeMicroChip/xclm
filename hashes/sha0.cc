/*
 * The MIT License (MIT)
 *
 * Copyright (c)2015, does not matter
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

#include <arpa/inet.h>
#include <array>
#include "sha0.h"

namespace Hash {

SHA0::SHA0(SHA0_MODE mode)
    : sha0Mode(mode)
{
    reset();
}

void  SHA0::reset()
{
    length_low = 0;
    length_high = 0;
    message_block_index = 0;

    H[0] = 0x67452301;
    H[1] = 0xEFCDAB89;
    H[2] = 0x98BADCFE;
    H[3] = 0x10325476;
    H[4] = 0xC3D2E1F0;

    computed = false;
    corrupted = false;
}

bool SHA0::digest(digest_t &digest)
{
    if (corrupted)
        return false;
    if (!computed)
    {
        pad_message();
        computed = true;
    }

    uint32_t* array = (uint32_t*)digest.data();
    for(int i = 0; i < 5; i++)
        array[i] = htonl(H[i]);

    return true;
}

void SHA0::update(const void *message_array, int length)
{
    if (!length) return;

    if (computed || corrupted) {
        corrupted = true;
        return;
    }

    const uint8_t* array = (const uint8_t*)message_array;

    while(length-- && !corrupted) {
        message_block[message_block_index++] = *array & 0xFF;

        length_low += 8;
        length_low &= 0xFFFFFFFF;

        if (length_low == 0) {
            length_high++;
            length_high &= 0xFFFFFFFF;
            if (length_high == 0) {
                /* Message is too long */
                corrupted = true;
            }
        }

        if (message_block_index == 64) {
            process_message_block();
        }

        array++;
    }
}

void SHA0::process_message_block()
{
    /* Constants defined for SHA-0/1 */
    constexpr std::array<uint32_t, 6> K = {{
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6,
        0x70E44324,   // modifed values by microchip
        0x359D3E2A    // modifed values by microchip
    }};

    std::array<uint32_t, 80> W;
    int t;
    uint32_t temp;
    uint32_t A, B, C, D, E;

    /* Initialize the first 16 words in the array W */
    for(t = 0; t < 16; t++) {
        W[t] = ((uint32_t)message_block[t * 4]) << 24;
        W[t] |= ((uint32_t)message_block[t * 4 + 1]) << 16;
        W[t] |= ((uint32_t)message_block[t * 4 + 2]) << 8;
        W[t] |= ((uint32_t)message_block[t * 4 + 3]);
    }

    for(t = 16; t < 80; t++) {
//SHA1         W[t] = circular_shift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
        W[t] = W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16];
    }

    A = H[0];
    B = H[1];
    C = H[2];
    D = H[3];
    E = H[4];

    for(t = 0; t < 20; t++) {
        temp = circular_shift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = circular_shift(30,B);
        B = A;
        A = temp;
    }

    for(t = 20; t < 40; t++) {
        temp = circular_shift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = circular_shift(30,B);
        B = A;
        A = temp;
    }

    for(t = 40; t < 60; t++) {
        if (sha0Mode == SHA0_MODE::MICROCHIP)
            temp = circular_shift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] - K[4]; // sub and other value
        else
            temp = circular_shift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];

        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = circular_shift(30,B);
        B = A;
        A = temp;
    }

    for(t = 60; t < 80; t++) {
        if (sha0Mode == SHA0_MODE::MICROCHIP)
            temp = circular_shift(5,A) + (B ^ C ^ D) + E + W[t] - K[5]; // sub and other value
        else
            temp = circular_shift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];

        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = circular_shift(30,B);
        B = A;
        A = temp;
    }

    H[0] = (H[0] + A) & 0xFFFFFFFF;
    H[1] = (H[1] + B) & 0xFFFFFFFF;
    H[2] = (H[2] + C) & 0xFFFFFFFF;
    H[3] = (H[3] + D) & 0xFFFFFFFF;
    H[4] = (H[4] + E) & 0xFFFFFFFF;

    message_block_index = 0;
}

/* Check to see if the current message block is too small to hold
 * the initial padding bits and length.  If so, we will pad the
 * block, process it, and then continue padding into a second
 * block.
 */
void SHA0::pad_message()
{
    if (message_block_index > 55) {
        message_block[message_block_index++] = 0x80;
        while(message_block_index < 64) {
            message_block[message_block_index++] = 0;
        }

        process_message_block();

        while(message_block_index < 56) {
            message_block[message_block_index++] = 0;
        }
    } else {
        message_block[message_block_index++] = 0x80;
        while(message_block_index < 56) {
            message_block[message_block_index++] = 0;
        }
    }

    /* Store the message length as the last 8 octets */
    message_block[56] = (length_high >> 24) & 0xFF;
    message_block[57] = (length_high >> 16) & 0xFF;
    message_block[58] = (length_high >> 8) & 0xFF;
    message_block[59] = (length_high) & 0xFF;
    message_block[60] = (length_low >> 24) & 0xFF;
    message_block[61] = (length_low >> 16) & 0xFF;
    message_block[62] = (length_low >> 8) & 0xFF;
    message_block[63] = (length_low) & 0xFF;

    process_message_block();
}

} // namespace Hash
