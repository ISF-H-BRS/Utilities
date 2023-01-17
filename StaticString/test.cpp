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

#include <array>
#include <cassert>
#include <iostream>

#define ASSERT(x) assert(x)
#include "staticstring.h"

// ---------------------------------------------------------------------------------------------- //

using String = StaticString<64>;

// ---------------------------------------------------------------------------------------------- //

void testTokensKeep()
{
    static constexpr auto TokenBehavior = String::TokenBehavior::KeepEmptyTokens;
    static constexpr char TokenSeparator = ';';

    static constexpr size_t ExpectedCount = 10;

    const String string = ";One;Two;Three;;Five;;;Eight;";

    size_t count = string.getTokenCount(TokenSeparator, TokenBehavior);
    assert(count == ExpectedCount);

    const String token0 = string.getToken(TokenSeparator, 0, TokenBehavior);
    const String token1 = string.getToken(TokenSeparator, 1, TokenBehavior);
    const String token2 = string.getToken(TokenSeparator, 2, TokenBehavior);
    const String token3 = string.getToken(TokenSeparator, 3, TokenBehavior);
    const String token4 = string.getToken(TokenSeparator, 4, TokenBehavior);
    const String token5 = string.getToken(TokenSeparator, 5, TokenBehavior);
    const String token6 = string.getToken(TokenSeparator, 6, TokenBehavior);
    const String token7 = string.getToken(TokenSeparator, 7, TokenBehavior);
    const String token8 = string.getToken(TokenSeparator, 8, TokenBehavior);
    const String token9 = string.getToken(TokenSeparator, 9, TokenBehavior);

    assert(token0 == "");
    assert(token1 == "One");
    assert(token2 == "Two");
    assert(token3 == "Three");
    assert(token4 == "");
    assert(token5 == "Five");
    assert(token6 == "");
    assert(token7 == "");
    assert(token8 == "Eight");
    assert(token9 == "");

    const String sub = string.getTokens(TokenSeparator, 3, 3, TokenBehavior);
    assert(sub == "Three;;Five");

    std::array<String, ExpectedCount> tokens;
    count = string.getAllTokens(TokenSeparator, tokens, TokenBehavior);
    assert(count == ExpectedCount);

    assert(tokens[0] == "");
    assert(tokens[1] == "One");
    assert(tokens[2] == "Two");
    assert(tokens[3] == "Three");
    assert(tokens[4] == "");
    assert(tokens[5] == "Five");
    assert(tokens[6] == "");
    assert(tokens[7] == "");
    assert(tokens[8] == "Eight");
    assert(tokens[9] == "");

    //for (const String& token : tokens)
    //    std::cout << token << std::endl;
}

// ---------------------------------------------------------------------------------------------- //

void testTokensSkip()
{
    static constexpr auto TokenBehavior = String::TokenBehavior::SkipEmptyTokens;
    static constexpr char TokenSeparator = ';';

    static constexpr size_t ExpectedCount = 5;

    const String string = ";One;Two;Three;;Five;;;Eight;";

    size_t count = string.getTokenCount(TokenSeparator, TokenBehavior);
    assert(count == ExpectedCount);

    const String token0 = string.getToken(TokenSeparator, 0, TokenBehavior);
    const String token1 = string.getToken(TokenSeparator, 1, TokenBehavior);
    const String token2 = string.getToken(TokenSeparator, 2, TokenBehavior);
    const String token3 = string.getToken(TokenSeparator, 3, TokenBehavior);
    const String token4 = string.getToken(TokenSeparator, 4, TokenBehavior);

    assert(token0 == "One");
    assert(token1 == "Two");
    assert(token2 == "Three");
    assert(token3 == "Five");
    assert(token4 == "Eight");

    const String sub = string.getTokens(TokenSeparator, 1, 3, TokenBehavior);
    assert(sub == "Two;Three;Five");

    std::array<String, ExpectedCount> tokens;
    count = string.getAllTokens(TokenSeparator, tokens, TokenBehavior);
    assert(count == ExpectedCount);

    assert(tokens[0] == "One");
    assert(tokens[1] == "Two");
    assert(tokens[2] == "Three");
    assert(tokens[3] == "Five");
    assert(tokens[4] == "Eight");

    //for (const String& token : tokens)
    //    std::cout << token << std::endl;
}

// ---------------------------------------------------------------------------------------------- //

auto main() -> int
{
    testTokensKeep();
    testTokensSkip();

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// ---------------------------------------------------------------------------------------------- //
