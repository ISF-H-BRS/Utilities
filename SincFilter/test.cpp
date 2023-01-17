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

#include "sincfilter.h"

#include <cmath>
#include <iostream>
#include <vector>

// ---------------------------------------------------------------------------------------------- //

auto main() -> int
{
    static constexpr double TwoPi = 2.0 * 3.14159265358979323846;

    static constexpr size_t SignalSize = 1000;
    static constexpr size_t FilterSize = 121;
    static constexpr size_t OutputSize = SignalSize + FilterSize - 1;

    static constexpr double BandLow  = 45.0 / SignalSize;
    static constexpr double BandHigh = 55.0 / SignalSize;

    static constexpr double Cutoff = 400.0 / SignalSize;

    static constexpr double SignalOffset = 0.5;

    static constexpr double FirstSignalAmplitude = 0.2;
    static constexpr double FirstSignalFrequency = 50.0;

    static constexpr double SecondSignalAmplitude = 0.05;
    static constexpr double SecondSignalFrequency = 450.0;

    static constexpr auto WindowType = SincFilter::WindowType::None;

    auto lowPass = SincFilter::lowPass(FilterSize, Cutoff, WindowType);
    auto highPass = SincFilter::highPass(FilterSize, Cutoff, WindowType);
    auto stopBand = SincFilter::stopBand(FilterSize, BandLow, BandHigh, WindowType);
    auto bandPass = SincFilter::bandPass(FilterSize, BandLow, BandHigh, WindowType);

    auto combined = stopBand - highPass; // equiv. to stopBand * lowPass

    std::vector<double> signal(SignalSize);

    for (size_t i = 0; i < SignalSize; ++i)
    {
        const auto time = static_cast<double>(i) / SignalSize;

        signal[i] = SignalOffset +
                    FirstSignalAmplitude * std::sin(TwoPi * FirstSignalFrequency * time) +
                    SecondSignalAmplitude * std::sin(TwoPi * SecondSignalFrequency * time);
    }

    std::vector<double> lowPassFiltered(OutputSize);
    lowPass.apply(signal, lowPassFiltered);

    std::vector<double> highPassFiltered(OutputSize);
    highPass.apply(signal, highPassFiltered);

    std::vector<double> stopBandFiltered(OutputSize);
    stopBand.apply(signal, stopBandFiltered);

    std::vector<double> bandPassFiltered(OutputSize);
    bandPass.apply(signal, bandPassFiltered);

    std::vector<double> combinedFiltered(OutputSize);
    combined.apply(signal, combinedFiltered);

    std::cout << "Signal;"
                 "Low-pass Coeffs;"
                 "High-pass Coeffs;"
                 "Stop-band Coeffs;"
                 "Band-pass Coeffs;"
                 "Combined Coeffs;"
                 "Low-pass Output;"
                 "High-pass Output;"
                 "Stop-band Output;"
                 "Band-pass Output;"
                 "Combined Output\n";

    for (size_t i = 0; i < OutputSize; ++i)
    {
        if (i < SignalSize)
            std::cout << signal[i];

        std::cout << ";";

        if (i < FilterSize)
        {
            std::cout << lowPass.coefficients()[i] << ";";
            std::cout << highPass.coefficients()[i] << ";";
            std::cout << stopBand.coefficients()[i] << ";";
            std::cout << bandPass.coefficients()[i] << ";";
            std::cout << combined.coefficients()[i] << ";";
        }
        else
            std::cout << ";;;;;";

        std::cout << lowPassFiltered[i] << ";";
        std::cout << highPassFiltered[i] << ";";
        std::cout << stopBandFiltered[i] << ";";
        std::cout << bandPassFiltered[i] << ";";
        std::cout << combinedFiltered[i] << "\n";
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------- //
