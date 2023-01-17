// ============================================================================================== //
//                                                                                                //
//   This file is part of the ISF Utilities collection.                                           //
//                                                                                                //
//   Author:                                                                                      //
//   Marcel Hasler <mahasler@gmail.com>                                                           //
//                                                                                                //
//   Copyright (c) 2022 - 2023                                                                    //
//   Bonn-Rhein-Sieg University of Applied Sciences                                               //
//                                                                                                //
//   Redistribution and use in source and binary forms, with or without modification,             //
//   are permitted provided that the following conditions are met:                                //
//                                                                                                //
//   1. Redistributions of source code must retain the above copyright notice,                    //
//      this list of conditions and the following disclaimer.                                     //
//                                                                                                //
//   2. Redistributions in binary form must reproduce the above copyright notice,                 //
//      this list of conditions and the following disclaimer in the documentation                 //
//      and/or other materials provided with the distribution.                                    //
//                                                                                                //
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"                  //
//   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED            //
//   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.           //
//   IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,             //
//   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT           //
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR           //
//   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,            //
//   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)           //
//   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE                   //
//   POSSIBILITY OF SUCH DAMAGE.                                                                  //
//                                                                                                //
// ============================================================================================== //

#include "sha256.h"

#include <string.h>

// ---------------------------------------------------------------------------------------------- //

#define _SHA256_ROTATE_LEFT(a,b)  (((a) << (b)) | ((a) >> (32-(b))))
#define _SHA256_ROTATE_RIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

// ---------------------------------------------------------------------------------------------- //

static const uint32_t _sha256_round_constants[64] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// ---------------------------------------------------------------------------------------------- //

struct _sha256_ctx
{
    uint32_t hash[8];
};

// ---------------------------------------------------------------------------------------------- //

void _sha256_init(struct _sha256_ctx *ctx)
{
    ctx->hash[0] = 0x6a09e667;
    ctx->hash[1] = 0xbb67ae85;
    ctx->hash[2] = 0x3c6ef372;
    ctx->hash[3] = 0xa54ff53a;
    ctx->hash[4] = 0x510e527f;
    ctx->hash[5] = 0x9b05688c;
    ctx->hash[6] = 0x1f83d9ab;
    ctx->hash[7] = 0x5be0cd19;
}

// ---------------------------------------------------------------------------------------------- //

void _sha256_transform(struct _sha256_ctx *ctx, uint8_t *chunk)
{
    /* Message schedule array */
    uint32_t w[64];

    uint32_t s0, s1;
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t ch, maj;
    uint32_t temp1, temp2;

    uint32_t i;

    /* Copy chunk into first 16 words of message schedule array */
    for (i = 0; i < 16; ++i)
        w[i] = (chunk[i*4] << 24) | (chunk[i*4 + 1] << 16) | (chunk[i*4 + 2] << 8) | (chunk[i*4 + 3]);

    /* Extend first 16 words into remaining 48 words of message schedule array */
    for (i = 16 ; i < 64; ++i)
    {
        s0 = _SHA256_ROTATE_RIGHT(w[i-15], 7) ^ _SHA256_ROTATE_RIGHT(w[i-15], 18) ^ (w[i-15] >> 3);
        s1 = _SHA256_ROTATE_RIGHT(w[i-2], 17) ^ _SHA256_ROTATE_RIGHT(w[i-2],  19) ^ (w[i-2] >> 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }

    /* Initialize working variables to current hash value */
    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];
    f = ctx->hash[5];
    g = ctx->hash[6];
    h = ctx->hash[7];

    /* Run compression loop */
    for (i = 0; i < 64; ++i)
    {
        s1 = _SHA256_ROTATE_RIGHT(e, 6) ^ _SHA256_ROTATE_RIGHT(e, 11) ^ _SHA256_ROTATE_RIGHT(e, 25);
        ch = (e & f) ^ (~e & g);
        temp1 = h + s1 + ch + _sha256_round_constants[i] + w[i];

        s0 = _SHA256_ROTATE_RIGHT(a, 2) ^ _SHA256_ROTATE_RIGHT(a, 13) ^ _SHA256_ROTATE_RIGHT(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        temp2 = s0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    /* Add compressed chunk to current hash value */
    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
    ctx->hash[5] += f;
    ctx->hash[6] += g;
    ctx->hash[7] += h;
}

// ---------------------------------------------------------------------------------------------- //

void sha256_compute(const uint8_t *buffer, uint32_t length, struct sha256_result *result)
{
    uint32_t i;

    uint8_t chunk[64];
    uint32_t chunk_position = 0;

    uint64_t message_length = length * 8;

    struct _sha256_ctx ctx;
    _sha256_init(&ctx);

    /* Break message into 512-bit chunks */
    for (i = 0; i < length; ++i)
    {
        chunk[chunk_position++] = buffer[i];
        if (chunk_position == 64)
        {
            _sha256_transform(&ctx, chunk);
            chunk_position = 0;
        }
    }

    /* Pad remaining buffer */
    if (chunk_position < 56)
    {
      chunk[chunk_position++] = 0x80;
      while (chunk_position < 56)
         chunk[chunk_position++] = 0x00;
    }
    else
    {
        chunk[chunk_position++] = 0x80;
        while (chunk_position < 64)
            chunk[chunk_position++] = 0x00;

        _sha256_transform(&ctx, chunk);

        memset(chunk, 0, 56);
        chunk_position = 56;
    }

    /* Append length of message */
    chunk[56] = message_length >> 56;
    chunk[57] = message_length >> 48;
    chunk[58] = message_length >> 40;
    chunk[59] = message_length >> 32;
    chunk[60] = message_length >> 24;
    chunk[61] = message_length >> 16;
    chunk[62] = message_length >> 8;
    chunk[63] = message_length;

    _sha256_transform(&ctx, chunk);

    /* Produce final hash value */
    for (i = 0; i < 8; ++i)
    {
        result->hash[i*4 + 0] = ctx.hash[i] >> 24;
        result->hash[i*4 + 1] = ctx.hash[i] >> 16;
        result->hash[i*4 + 2] = ctx.hash[i] >> 8;
        result->hash[i*4 + 3] = ctx.hash[i];
    }
}

// ---------------------------------------------------------------------------------------------- //
