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

#include "elapsedtimer.h"

// ---------------------------------------------------------------------------------------------- //

ElapsedTimer::ElapsedTimer()
    : m_start(std::chrono::steady_clock::now())
{
}

// ---------------------------------------------------------------------------------------------- //

void ElapsedTimer::restart()
{
    m_start = std::chrono::steady_clock::now();
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getElapsed() const -> std::chrono::nanoseconds
{
    auto diff = std::chrono::steady_clock::now() - m_start;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getNanoseconds() const -> long long
{
    return getElapsed().count();
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getMicroseconds() const -> long long
{
    return getNanoseconds() / 1000;
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getMilliseconds() const -> long long
{
    return getNanoseconds() / 1'000'000;
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getNanosecondsString() const -> std::string
{
    return std::to_string(getNanoseconds()) + " ns";
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getMicrosecondsString() const -> std::string
{
    return std::to_string(getMicroseconds()) + " µs";
}

// ---------------------------------------------------------------------------------------------- //

auto ElapsedTimer::getMillisecondsString() const -> std::string
{
    return std::to_string(getMilliseconds()) + " ms";
}

// ---------------------------------------------------------------------------------------------- //
