#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/matplotlibcpp.h"

#include "component/harmonic/harmonic_utils.h"

namespace plt = matplotlibcpp;

int main() {
    SignalData data = HarmonicUtils::importData("merged_signal_data.txt");

    /*
    Количество точек
    Шаг времени
    Целевой коэффициент гармонических искажений (в %) 
    */
   const int    numOfPoints      = 10000;
   const double deltaT           = 1e-4;
   const double targetDistortion = 5.0;

    // Векторы для хранения исходного сигнала и времени для него
    std::vector<double> resultSignal = data.getSignal();
    std::vector<double> time         = data.getTime();

    // Обработка сигнала с заданным порогом
    auto [freq, amplitudes, reconstructed] = HarmonicUtils::processSignal(
        resultSignal, deltaT, targetDistortion
    );

    // Расчет искажений
    double mse = 0.0, power = 0.0;
    for(int i = 0; i < numOfPoints; ++i) {
        mse   += pow(resultSignal[i] - reconstructed[i], 2);
        power += pow(resultSignal[i], 2);
    }
    
    double distortion = sqrt(mse/power) * 100;

    for(int i = 0; i < numOfPoints; ++i) {
        double t = i * deltaT;
        data.addData(t, resultSignal[i]);
    }

    // Выполняем корректировку амплитуд частотных компонентов после преобразования сигнала
    std::vector<double> freqHalf(freq.begin(), freq.begin() + numOfPoints / 2);
    std::vector<double> ampsHalf(amplitudes.begin(), amplitudes.begin() + numOfPoints / 2);
    
    for(double& amp : ampsHalf) 
        amp *= 2;

    // Первый график - Временная область
    plt::figure_size(1200, 600);
    plt::plot(time, resultSignal, {{"label", "Original"}, {"alpha", "0.7"}});
    plt::plot(time, reconstructed, {{"label", "Reconstructed"}, {"linestyle", "--"}, {"color", "r"}});
    plt::title("Signal Comparison (Distortion: " + std::to_string(distortion) + "%)");
    plt::xlabel("Time, s");
    plt::ylabel("Amplitude");
    plt::legend();
    plt::grid(true);
    
    plt::show();
    
    // Второй график - Частотная область
    plt::figure_size(1200, 600);    

    std::map<std::string, std::string> stemParams = {
        {"basefmt", " "},
        {"linefmt", "b-"},
        {"markerfmt", "bo"}
    };

    plt::stem(freqHalf, ampsHalf, stemParams);
    plt::xlabel("Frequency, Hz");
    plt::ylabel("Amplitude");
    plt::title("Amplitude Spectrum");
    plt::grid(true);
    
    plt::show();

    return 0;
}
