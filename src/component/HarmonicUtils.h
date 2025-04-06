#ifndef HARMONICUTILS_H
#define HARMONICUTILS_H

#ifndef PI
    #define PI 3.14159265358979323846
#endif

#include <vector>

class HarmonicUtils {
public:

    static std::vector<double> generateHarmonic(
        int numPoints, double amplitude, 
        double frequency, double phase, double deltaT) {

        std::vector<double> harmonic(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            double t = i * deltaT;
            harmonic[i] = amplitude * sin(2 * PI * frequency * t + phase);
        }

        return harmonic;
    }

};

#endif  // HARMONICUTILS_H
