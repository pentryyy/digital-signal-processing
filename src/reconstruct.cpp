#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <fftw3.h>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

class SignalProcessor {
public:
    static std::vector<double> generateHarmonic(int numPoints, double amplitude, 
                                              double frequency, double phase, double deltaT) {
        std::vector<double> harmonic(numPoints);
        for(int i = 0; i < numPoints; ++i) {
            double t = i * deltaT;
            harmonic[i] = amplitude * sin(2 * M_PI * frequency * t + phase);
        }
        return harmonic;
    }

    static std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> 
    processSignal(const std::vector<double>& signal, double deltaT, double targetDistortion) {
        int N = signal.size();
        std::vector<std::complex<double>> fftResult(N);
        
        // Выполняем FFT
        fftw_plan plan = fftw_plan_dft_r2c_1d(N, const_cast<double*>(signal.data()), 
                                            reinterpret_cast<fftw_complex*>(fftResult.data()), FFTW_ESTIMATE);
        fftw_execute(plan);
        fftw_destroy_plan(plan);

        // Рассчитываем амплитуды и частоты
        std::vector<double> amplitudes(N);
        std::vector<double> freq(N);
        for(int i = 0; i < N; ++i) {
            amplitudes[i] = std::abs(fftResult[i]) / N;
            freq[i] = i / (deltaT * N);
        }

        // Алгоритм выбора компонент
        int half = N/2;
        double totalEnergy = 0.0;
        for(double amp : amplitudes) totalEnergy += amp*amp;
        
        double requiredEnergy = (1 - pow(targetDistortion/100, 2)) * totalEnergy;
        std::vector<int> indices(N);
        for(int i = 0; i < N; ++i) indices[i] = i;
        
        std::sort(indices.begin(), indices.end(), [&amplitudes](int a, int b) {
            return amplitudes[a] > amplitudes[b];
        });

        std::vector<std::complex<double>> filteredFFT(N, 0.0);
        double currentEnergy = 0.0;
        for(int idx : indices) {
            if(idx > half) continue;
            
            double componentEnergy = (idx != 0) ? 2*pow(amplitudes[idx], 2) : pow(amplitudes[idx], 2);
            if(currentEnergy + componentEnergy > requiredEnergy) {
                double remaining = requiredEnergy - currentEnergy;
                double scale = sqrt(remaining / componentEnergy);
                filteredFFT[idx] = fftResult[idx] * scale;
                if(idx != 0) filteredFFT[N-idx] = fftResult[N-idx] * scale;
                break;
            }
            
            filteredFFT[idx] = fftResult[idx];
            if(idx != 0) filteredFFT[N-idx] = fftResult[N-idx];
            currentEnergy += componentEnergy;
        }

        // Обратное FFT
        std::vector<double> reconstructed(N);
        fftw_plan inv_plan = fftw_plan_dft_c2r_1d(N, 
            reinterpret_cast<fftw_complex*>(filteredFFT.data()), reconstructed.data(), FFTW_ESTIMATE);
        fftw_execute(inv_plan);
        fftw_destroy_plan(inv_plan);
        
        // Нормализация
        for(auto& val : reconstructed) val /= N;

        return {freq, amplitudes, reconstructed};
    }
};

int main() {
    
    const int    numPoints        = 10000;
    const double samplingFreq     = 2000.0;
    const double deltaT           = 1.0 / samplingFreq;
    const double targetDistortion = 5.0;

    // Генерация тестового сигнала
    std::vector<std::tuple<double, double, double>> harmonicParams = {
        {2.0, 50.0, 0.0}, {1.5, 100.0, M_PI/4}, {1.0, 200.0, M_PI/2},
        {0.8, 300.0, 3*M_PI/4}, {0.6, 400.0, M_PI}, {0.5, 500.0, M_PI/6},
        {0.4, 600.0, M_PI/3}, {0.3, 700.0, 2*M_PI/3}, {0.2, 800.0, 5*M_PI/6},
        {0.1, 900.0, M_PI/8}
    };

    std::vector<double> signal(numPoints, 0.0);
    for(const auto& [amp, freq, phase] : harmonicParams) {
        auto harmonic = SignalProcessor::generateHarmonic(numPoints, amp, freq, phase, deltaT);
        for(int i = 0; i < numPoints; ++i) signal[i] += harmonic[i];
    }

    // Обработка сигнала
    auto [freq, amplitudes, reconstructed] = SignalProcessor::processSignal(signal, deltaT, targetDistortion);

    // Расчет искажений
    double mse = 0.0, power = 0.0;
    for(int i = 0; i < numPoints; ++i) {
        mse += pow(signal[i] - reconstructed[i], 2);
        power += pow(signal[i], 2);
    }
    double distortion = sqrt(mse/power) * 100;

    // Визуализация
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
