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

#include <stdint.h>
#include <stdio.h>

/*  X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10
         + X^8  + X^7  + X^5  + X^4  + X^2  + X^1  + X^0  */
#define CRC32_POLYNOM 0xedb88320

// ---------------------------------------------------------------------------------------------- //

static uint32_t table[256];

// ---------------------------------------------------------------------------------------------- //

int main()
{
    int i, j;

    /* Compute table */
    for (i = 0; i < 256; ++i)
    {
        uint32_t crc = (uint32_t)i;

        for (j = 0; j < 8; ++j)
        {
            if (crc & 0x00000001)
                crc = (crc >> 1) ^ CRC32_POLYNOM;
            else
                crc = (crc >> 1);
        }

        table[i] = crc;
    }

    /* Print table */
    printf("static uint32_t table[256] =\n");
    printf("{\n");

    for (i = 0; i < 256; ++i)
    {
        /* Indent rows */
        if ((i%8) == 0)
            printf("    ");

        printf("0x%08x", table[i]);

        if (i < 255)
            printf(",");

        /* 8 values per row */
        if ((i%8) == 7)
            printf("\n");
        else
            printf(" ");
    }

    printf("};\n");

    return 0;
}

// ---------------------------------------------------------------------------------------------- //
