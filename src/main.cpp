#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <tuple>

#include "include/SignalData.h"
#include "include/matplotlibcpp.h"
#include "component/HarmonicUtils.h"

#define plt matplotlibcpp

int main() {
    // Объект для хранения данных графика гармоник
    SignalData data;

    int    numHarmonics      = 10;                      // Количество гармоник
    int    numPoints         = 10000;                   // Количество точек
    double samplingFrequency = 1000.0;                  // Частота дискретизации (Гц)
    double iMax              = 5.0;                     // Максимальная амплитуда
    double deltaT            = 1.0 / samplingFrequency; // Шаг времени

    // Параметры гармоник (амплитуда, частота, начальная фаза)
    std::vector<std::tuple<double, double, double>> harmonicParams = {
        {2.0, 50.0,  0.0},
        {1.5, 100.0, PI / 4},
        {1.0, 200.0, PI / 2},
        {0.8, 300.0, 3 * PI / 4},
        {0.6, 400.0, PI},
        {0.5, 500.0, PI / 6},
        {0.4, 600.0, PI / 3},
        {0.3, 700.0, 2 * PI / 3},
        {0.2, 800.0, 5 * PI / 6}, 
        {0.1, 900.0, PI / 8}
    };

    // Векторы для хранения данных
    std::vector<std::vector<double>> harmonics(numHarmonics, std::vector<double>(numPoints));
    std::vector<double> resultSignal(numPoints, 0.0);

    // Генерация гармоник
    for (int i = 0; i < numHarmonics; ++i) {
        double amplitude = std::get<0>(harmonicParams[i]);
        double frequency = std::get<1>(harmonicParams[i]);
        double phase = std::get<2>(harmonicParams[i]);

        harmonics[i] = HarmonicUtils::generateHarmonic(numPoints, amplitude, frequency, phase, deltaT);

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
