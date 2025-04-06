#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <tuple>

#include "include/SignalData.h"
#include "include/matplotlibcpp.h"

#include "component/HarmonicUtils.h"
#include "component/HarmonicParams.h"

#define plt matplotlibcpp

int main() {
    SignalData data;

    int numHarmonics = HarmonicParams::harmonicParams.size(); // Количество гармоник
    int numPoints    = 10000;                                 // Количество точек

    double samplingFrequency = 1000.0;                  // Частота дискретизации (Гц)
    double deltaT            = 1.0 / samplingFrequency; // Шаг времени

    // Векторы для хранения данных
    std::vector<std::vector<double>> harmonics(numHarmonics, std::vector<double>(numPoints));
    std::vector<double> resultSignal(numPoints, 0.0);

    // Генерация гармоник
    for (int i = 0; i < numHarmonics; ++i) {

        double amplitude = std::get<0>(HarmonicParams::harmonicParams[i]);
        double frequency = std::get<1>(HarmonicParams::harmonicParams[i]);
        double phase     = std::get<2>(HarmonicParams::harmonicParams[i]);

        harmonics[i] = HarmonicUtils::generateHarmonic(
            numPoints, amplitude, frequency, phase, deltaT
        );

        // Суммирование гармоник
        for (int j = 0; j < numPoints; ++j) {
            resultSignal[j] += harmonics[i][j];
        }
    }

    for (int i = 0; i < numPoints; ++i) {
        double t = i * deltaT;
        data.addData(t, resultSignal[i]);
    }

    // Построение графика
    plt::figure_size(800, 400);
    plt::plot(data.getTime(), data.getSignal(), {{"label", "Result Signal"}, {"color", "b"}});
    plt::xlabel("Time, s");
    plt::ylabel("Signal Amplitude");
    plt::title("Signal Plot");
    plt::legend();
    plt::grid(true);
    
    plt::show();

    return 0;
}
