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

#include "component/signal_data.h"

#include "component/harmonic/harmonic_utils.h"
#include "component/harmonic/harmonic_params.h"

int main() {
    SignalData data;

    /*
    Количество гармоник
    Количество точек
    Шаг времени
    */
    const int    numOfHarmonics    = HarmonicParams::harmonicParams.size();
    const int    numOfPoints       = 10000;
    const double deltaT            = 1e-4;

    // Векторы для хранения данных
    std::vector<std::vector<double>> harmonics(numOfHarmonics, std::vector<double>(numOfPoints));
    std::vector<double> resultSignal(numOfPoints, 0.0);

    // Генерация гармоник и их суммирование
    for (int i = 0; i < numOfHarmonics; ++i) {

        double amplitude = std::get<0>(HarmonicParams::harmonicParams[i]);
        double frequency = std::get<1>(HarmonicParams::harmonicParams[i]);
        double phase     = std::get<2>(HarmonicParams::harmonicParams[i]);

        harmonics[i] = HarmonicUtils::generateHarmonic(
            numOfPoints, amplitude, frequency, phase, deltaT
        );

        for (int j = 0; j < numOfPoints; ++j) {
            resultSignal[j] += harmonics[i][j];
        }
    }

    for (int i = 0; i < numOfPoints; ++i) {
        double t = i * deltaT;
        data.addData(t, resultSignal[i]);
    }

    HarmonicUtils::exportData(data, "merged_signal_data.txt");

    return 0;
}
