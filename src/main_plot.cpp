#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

#include "include/signal_data.h"

#include "component/harmonic_utils.h"

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    SignalData data = HarmonicUtils::importData("merged_signal_data.txt");

    plt::figure_size(800, 400);
    plt::plot(data.getTime(), data.getSignal(), {{"label", "Result Signal"}, {"color", "b"}});
    plt::xlabel("Time, s");
    plt::ylabel("Signal Amplitude");
    plt::title("Signal Plot");
    plt::legend();
    plt::grid(true);
    
    plt::show();
}