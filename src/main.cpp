#include "component/necessary_headers.h"
#include "component/signal_data.h"

#include "component/harmonic/harmonic_utils.h"
#include "component/harmonic/harmonic_params.h"

int main() {
    SignalData data;

    /*
    Количество точек
    Шаг времени
    */
    const int    numOfPoints = 10000;
    const double deltaT      = 1e-4;

    // Векторы для хранения данных
    std::vector<double> resultSignal(numOfPoints, 0.0);

    // Генерация гармоник и их суммирование
    for (const std::tuple<double, double, double> item : HarmonicParams::harmonicParams) {

        double amplitude = std::get<0>(item);
        double frequency = std::get<1>(item);
        double phase     = std::get<2>(item);

        std::vector<double> harmonic = HarmonicUtils::generateHarmonic(
            numOfPoints, amplitude, frequency, phase, deltaT
        );

        for (int i = 0; i < numOfPoints; ++i) {
            resultSignal[i] += harmonic[i];
        }
    }

    for (int i = 0; i < numOfPoints; ++i) {
        double t = i * deltaT;
        data.addData(t, resultSignal[i]);
    }

    HarmonicUtils::exportData(data, "merged_signal_data.txt");

    return 0;
}
