#include "component/necessary_headers.h"
#include "component/signal_data.h"
#include "component/matplotlibcpp.h"

#include "component/harmonic/harmonic_utils.h"

namespace plt = matplotlibcpp;

int main() {
    SignalData data = HarmonicUtils::importData("merged_signal_data.txt");

    plt::figure_size(1200, 600);
    plt::plot(SignalTime, SignalFrequency, {{"label", "Result Signal"}, {"color", "b"}});
    plt::xlabel("Time, s");
    plt::ylabel("Signal Amplitude");
    plt::title("Signal Plot");
    plt::legend();
    plt::grid(true);
    
    plt::show();

    return 0;
}
