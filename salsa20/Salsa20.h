// Copyright (c) 2015 Nezametdinov E. Ildus
// See LICENSE.TXT for licensing details

#ifndef _SALSA20_H_
#define _SALSA20_H_

#include <cassert>
#include <stdint.h>
#include <cstring>

/**
 * Represents Salsa20 cypher. Supports only 256-bit keys.
 */
class Salsa20
{
public:
    /// Helper constants
    enum
    {
        VECTOR_SIZE = 16, //DWORD, the vector size
        BLOCK_SIZE = 64, //bytes, deal 64 block once
        KEY_SIZE = 32, //bytes
        IV_SIZE = 8 //bytes
    };

    /**
     * \brief Constructs cypher with given key.
     * \param[in] key 256-bit key
     */
    Salsa20(const uint8_t* key = NULL)
    {
        std::memset(vector_, 0, sizeof(vector_));
        setKey(key);
    }

    /**
     * \brief Sets key.
     * \param[in] key 256-bit key
     */
    void setKey(const uint8_t* key)
    {
        static const char constants[] = "expand 32-byte k";

        if(key == NULL)
            return;

        vector_[0] = convert(reinterpret_cast<const uint8_t*>(&constants[0]));
        vector_[1] = convert(&key[0]);
        vector_[2] = convert(&key[4]);
        vector_[3] = convert(&key[8]);
        vector_[4] = convert(&key[12]);
        vector_[5] = convert(reinterpret_cast<const uint8_t*>(&constants[4]));

        std::memset(&vector_[6], 0, 4 * sizeof(uint32_t));

        vector_[10] = convert(reinterpret_cast<const uint8_t*>(&constants[8]));
        vector_[11] = convert(&key[16]);
        vector_[12] = convert(&key[20]);
        vector_[13] = convert(&key[24]);
        vector_[14] = convert(&key[28]);
        vector_[15] = convert(reinterpret_cast<const uint8_t*>(&constants[12]));
    }

    /**
     * \brief Sets IV.
     * \param[in] iv 64-bit IV 4 bytes
     */
    void setIv(const uint8_t* iv)
    {
        if(iv == NULL)
            return;

        vector_[6] = convert(&iv[0]);
        vector_[7] = convert(&iv[4]);
        vector_[8] = vector_[9] = 0;
    }

    /**
     * \brief Generates key stream.
     * \param[out] output generated key stream
     */
    void generateKeyStream(uint8_t output[BLOCK_SIZE])
    {
        uint32_t x[VECTOR_SIZE];
        std::memcpy(x, vector_, sizeof(vector_));

        for(int32_t i = 20; i > 0; i -= 2)
        {
            x[4 ] ^= rotate(static_cast<uint32_t>(x[0 ] + x[12]),  7);
            x[8 ] ^= rotate(static_cast<uint32_t>(x[4 ] + x[0 ]),  9);
            x[12] ^= rotate(static_cast<uint32_t>(x[8 ] + x[4 ]), 13);
            x[0 ] ^= rotate(static_cast<uint32_t>(x[12] + x[8 ]), 18);
            x[9 ] ^= rotate(static_cast<uint32_t>(x[5 ] + x[1 ]),  7);
            x[13] ^= rotate(static_cast<uint32_t>(x[9 ] + x[5 ]),  9);
            x[1 ] ^= rotate(static_cast<uint32_t>(x[13] + x[9 ]), 13);
            x[5 ] ^= rotate(static_cast<uint32_t>(x[1 ] + x[13]), 18);
            x[14] ^= rotate(static_cast<uint32_t>(x[10] + x[6 ]),  7);
            x[2 ] ^= rotate(static_cast<uint32_t>(x[14] + x[10]),  9);
            x[6 ] ^= rotate(static_cast<uint32_t>(x[2 ] + x[14]), 13);
            x[10] ^= rotate(static_cast<uint32_t>(x[6 ] + x[2 ]), 18);
            x[3 ] ^= rotate(static_cast<uint32_t>(x[15] + x[11]),  7);
            x[7 ] ^= rotate(static_cast<uint32_t>(x[3 ] + x[15]),  9);
            x[11] ^= rotate(static_cast<uint32_t>(x[7 ] + x[3 ]), 13);
            x[15] ^= rotate(static_cast<uint32_t>(x[11] + x[7 ]), 18);
            x[1 ] ^= rotate(static_cast<uint32_t>(x[0 ] + x[3 ]),  7);
            x[2 ] ^= rotate(static_cast<uint32_t>(x[1 ] + x[0 ]),  9);
            x[3 ] ^= rotate(static_cast<uint32_t>(x[2 ] + x[1 ]), 13);
            x[0 ] ^= rotate(static_cast<uint32_t>(x[3 ] + x[2 ]), 18);
            x[6 ] ^= rotate(static_cast<uint32_t>(x[5 ] + x[4 ]),  7);
            x[7 ] ^= rotate(static_cast<uint32_t>(x[6 ] + x[5 ]),  9);
            x[4 ] ^= rotate(static_cast<uint32_t>(x[7 ] + x[6 ]), 13);
            x[5 ] ^= rotate(static_cast<uint32_t>(x[4 ] + x[7 ]), 18);
            x[11] ^= rotate(static_cast<uint32_t>(x[10] + x[9 ]),  7);
            x[8 ] ^= rotate(static_cast<uint32_t>(x[11] + x[10]),  9);
            x[9 ] ^= rotate(static_cast<uint32_t>(x[8 ] + x[11]), 13);
            x[10] ^= rotate(static_cast<uint32_t>(x[9 ] + x[8 ]), 18);
            x[12] ^= rotate(static_cast<uint32_t>(x[15] + x[14]),  7);
            x[13] ^= rotate(static_cast<uint32_t>(x[12] + x[15]),  9);
            x[14] ^= rotate(static_cast<uint32_t>(x[13] + x[12]), 13);
            x[15] ^= rotate(static_cast<uint32_t>(x[14] + x[13]), 18);
        }

        for(size_t i = 0; i < VECTOR_SIZE; ++i)
        {
            x[i] += vector_[i];
            convert(x[i], &output[4 * i]);
        }

        ++vector_[8];
        vector_[9] += vector_[8] == 0 ? 1 : 0;
    }

    /**
     * \brief Processes blocks.
     * \param[in] input input
     * \param[out] output output
     * \param[in] numBlocks number of blocks
     */
    void processBlocks(const uint8_t* input, uint8_t* output, size_t numBlocks)
    {
        assert(input != NULL && output != NULL);

        uint8_t keyStream[BLOCK_SIZE];

        for(size_t i = 0; i < numBlocks; ++i)
        {
            generateKeyStream(keyStream);

            for(size_t j = 0; j < BLOCK_SIZE; ++j)
                *(output++) = keyStream[j] ^ *(input++);
        }
    }

    /**
     * \brief Processes bytes.
     *
     * This function should be used carefully. If number of bytes is not multiple of
     * block size, then next call to the processBlocks function will be invalid.
     * Normally this function should be used once at the end of encryption or
     * decryption.
     * \param[in] input input
     * \param[out] output output
     * \param[in] numBytes number of bytes
     */
    void processBytes(const uint8_t* input, uint8_t* output, size_t numBytes)
    {
        assert(input != NULL && output != NULL);

        uint8_t keyStream[BLOCK_SIZE];
        size_t numBytesToProcess;

        while(numBytes != 0)
        {
            generateKeyStream(keyStream);
            numBytesToProcess = numBytes >= BLOCK_SIZE ? BLOCK_SIZE : numBytes;

            for(size_t i = 0; i < numBytesToProcess; ++i, --numBytes)
                *(output++) = keyStream[i] ^ *(input++);
        }
    }

private:
    /**
     * \brief Rotates value.
     * \param[in] value value
     * \param[in] numBits number of bits to rotate
     * \return result of the rotation
     */
    uint32_t rotate(uint32_t value, uint32_t numBits)
    {
        return (value << numBits) | (value >> (32 - numBits));
    }

    /**
     * \brief Converts 32-bit unsigned integer value to the array of bytes.
     * \param[in] value 32-bit unsigned integer value
     * \param[out] array array of bytes
     */
    void convert(uint32_t value, uint8_t* array)
    {
        array[0] = static_cast<uint8_t>(value >> 0);
        array[1] = static_cast<uint8_t>(value >> 8);
        array[2] = static_cast<uint8_t>(value >> 16);
        array[3] = static_cast<uint8_t>(value >> 24);
    }

    /**
     * \brief Converts array of bytes to the 32-bit unsigned integer value.
     * \param[in] array array of bytes
     * \return 32-bit unsigned integer value
     */
    uint32_t convert(const uint8_t* array)
    {
        return ((static_cast<uint32_t>(array[0]) << 0)  |
            (static_cast<uint32_t>(array[1]) << 8)  |
            (static_cast<uint32_t>(array[2]) << 16) |
            (static_cast<uint32_t>(array[3]) << 24));
    }

    // Data members
    uint32_t vector_[VECTOR_SIZE];
};

#endif
