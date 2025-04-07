#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <fftw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "matplotlibcpp.h"

#include "include/signal_data.h"

#include "component/harmonic_utils.h"
#include "component/harmonic_params.h"

namespace plt = matplotlibcpp;

int main() {
    
    const int    numPoints        = 10000;
    const double deltaT           = 1e-4;
    const double targetDistortion = 5.0;

    std::vector<double> signal(numPoints, 0.0);
    for(const auto& [amp, freq, phase] : HarmonicParams::harmonicParams) {
        auto harmonic = HarmonicUtils::generateHarmonic(numPoints, amp, freq, phase, deltaT);
        for(int i = 0; i < numPoints; ++i) signal[i] += harmonic[i];
    }

    // Обработка сигнала
    auto [freq, amplitudes, reconstructed] = HarmonicUtils::processSignal(signal, deltaT, targetDistortion);

    // Расчет искажений
    double mse = 0.0, power = 0.0;
    for(int i = 0; i < numPoints; ++i) {
        mse += pow(signal[i] - reconstructed[i], 2);
        power += pow(signal[i], 2);
    }
    double distortion = sqrt(mse/power) * 100;

    std::vector<double> time(numPoints);
    for(int i = 0; i < numPoints; ++i) time[i] = i * deltaT;
    
    // Первый график - Временная область
    plt::figure_size(1200, 600);
    plt::plot(time, signal, {{"label", "Original"}, {"alpha", "0.7"}});
    plt::plot(time, reconstructed, {{"label", "Reconstructed"}, {"linestyle", "--"}, {"color", "r"}});
    plt::title("Signal Comparison (Distortion: " + std::to_string(distortion) + "%)");
    plt::xlabel("Time, s");
    plt::ylabel("Amplitude");
    plt::legend();
    plt::grid(true);
    plt::show();
    
    // Второй график - Частотная область
    plt::figure_size(1200, 600);
    int half = numPoints/2;
    std::vector<double> freqHalf(freq.begin(), freq.begin() + half);
    std::vector<double> ampsHalf(amplitudes.begin(), amplitudes.begin() + half);
    for(auto& amp : ampsHalf) amp *= 2;
    
    plt::stem(freqHalf, ampsHalf, "b");
    plt::xlabel("Frequency, Hz");
    plt::ylabel("Amplitude");
    plt::title("Amplitude Spectrum");
    plt::grid(true);
    plt::show();

    return 0;
}
