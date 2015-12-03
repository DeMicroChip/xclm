#ifndef __SHA0_H__
#define __SHA0_H__

#include <stdint.h>
#include <string>

namespace Hash
{
    typedef enum
    {
        NORMAL,
        MICROCHIP  // modified mode, like xclm is using
    } SHA0_MODE;

    class SHA0
    {
        public:
            // default SHA0 mode
            explicit SHA0(SHA0_MODE mode = SHA0_MODE::NORMAL);
            ~SHA0() = default;

            /* Size of hash in bytes */
            static const uint8_t hash_size = 20;
            typedef std::array<uint8_t, SHA0::hash_size> digest_t;

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

            template<typename T, std::size_t N>
            void update(const std::array<T, N> &arr) {
                update(arr.data(), arr.size());
            }

        private:
            /* Process the next 512 bits of the message */
            void process_message_block();

            /* Pads the current message block to 512 bits */
            void pad_message();

            /* Performs a circular left shift operation */
            uint32_t circular_shift(int bits, unsigned word) {
                return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
            }

            /* Message digest buffers */
            uint32_t H[5];

            /* Message length in bits */
            uint32_t length_low;

            /* Message length in bits */
            uint32_t length_high;

            /* 512-bit message blocks */
            uint8_t message_block[64];

            /* Index into message block array */
            int message_block_index;

            /* Is the digest computed? */
            bool computed;

            /* Is the message digest corrupted? */
            bool corrupted;

            const SHA0_MODE sha0Mode;
    };
}

#endif /*  __SHA0_H__ */