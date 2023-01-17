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

#include "freestorearray.h"

#include <iostream>
#include <span>

// ---------------------------------------------------------------------------------------------- //

static_assert(std::ranges::contiguous_range<FreeStoreArray<double, 10>>);
static_assert(std::ranges::contiguous_range<const FreeStoreArray<double, 10>>);

static_assert(std::ranges::sized_range<FreeStoreArray<double, 10>>);
static_assert(std::ranges::sized_range<const FreeStoreArray<double, 10>>);

// ---------------------------------------------------------------------------------------------- //

class Test
{
public:
    Test() : m_id(s_nextId++)
    {
        std::cout << m_id << ": Test()" << std::endl;
    };

    Test(int foo) : m_id(s_nextId++)
    {
        std::cout << m_id << ": Test(int foo)" << std::endl;
    };

    Test(const Test& other) : m_id(s_nextId++)
    {
        std::cout << m_id << ": Test(const Test& other)" << std::endl;
    };

    Test(Test&& other) : m_id(s_nextId++)
    {
        std::cout << m_id << ": Test(Test&& other)" << std::endl;
    };

    ~Test()
    {
        std::cout << m_id << ": ~Test()" << std::endl;
    }

    auto operator=(const Test& other) -> Test&
    {
        std::cout << m_id << ": operator=(const Test& other)" << std::endl;
        return *this;
    }

    auto operator=(Test&& other) -> Test&
    {
        std::cout << m_id << ": operator=(Test&& other)" << std::endl;
        return *this;
    }

private:
    size_t m_id;
    static size_t s_nextId;
};

// ---------------------------------------------------------------------------------------------- //

void testSpan(std::span<const double> data)
{
    double sum = 0.0;

    for (double d : data)
        sum += d;

    std::cout << "Span test, sum: " << sum << std::endl;
}

// ---------------------------------------------------------------------------------------------- //

size_t Test::s_nextId = 0;

// ---------------------------------------------------------------------------------------------- //

auto main() -> int
{
    std::cout << "Test span:" << std::endl;
    const FreeStoreArray<double, 10> spanArray = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    testSpan(spanArray);

    std::cout << "Test 1:" << std::endl;
    FreeStoreArray<Test, 10> testArray1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::cout << std::endl;

    std::cout << "Test 2:" << std::endl;
    FreeStoreArray<Test, 10> testArray2 = testArray1;
    std::cout << std::endl;

    std::cout << "Test 3:" << std::endl;
    FreeStoreArray<Test, 10> testArray3;
    std::cout << std::endl;

    std::cout << "Test 4:" << std::endl;
    std::copy(testArray2.begin(), testArray2.end(), testArray3.begin());
    std::cout << std::endl;

    std::cout << "Test 5:" << std::endl;
    FreeStoreArray<Test, 10> testArray4 = std::move(testArray1);
    std::cout << std::endl;

    std::cout << "Test 6:" << std::endl;
    for (Test& test : testArray1)
    {
        Test* ptr = &test;
        std::cout << "Found an object at " << ptr << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Test 7:" << std::endl;
    try {
        Test* ptr = &testArray1.at(0);
        std::cout << "Accessed object at " << ptr << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Test 8:" << std::endl;
    try {
        for (size_t i = 0; i < testArray4.size() + 1; ++i)
        {
            Test* ptr = &testArray4.at(i);
            std::cout << "Accessed object at " << ptr << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "End of test." << std::endl;

    return 0;
}

// ---------------------------------------------------------------------------------------------- //
