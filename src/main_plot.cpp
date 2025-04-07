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

#include "component/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    SignalData data = HarmonicUtils::importData("merged_signal_data.txt");

    plt::figure_size(1200, 600);
    plt::plot(data.getTime(), data.getSignal(), {{"label", "Result Signal"}, {"color", "b"}});
    plt::xlabel("Time, s");
    plt::ylabel("Signal Amplitude");
    plt::title("Signal Plot");
    plt::legend();
    plt::grid(true);
    
    plt::show();
}