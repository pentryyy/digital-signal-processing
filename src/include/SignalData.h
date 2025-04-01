#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <vector>

struct SignalData {
    std::vector<double> time;
    std::vector<double> signal;

    SignalData() = default;

    const std::vector<double>& getTime() const { return time; }
    const std::vector<double>& getSignal() const { return signal; }

    void addData(double t, double s) {
        time.push_back(t);
        signal.push_back(s);
    }
};

#endif  // SIGNALDATA_H