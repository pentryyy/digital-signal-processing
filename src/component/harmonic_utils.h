#ifndef HARMONICUTILS_H
#define HARMONICUTILS_H

class HarmonicUtils {
public:
    static std::vector<double> generateHarmonic(
        int numPoints, double amplitude, double frequency, double phase, double deltaT) {

        std::vector<double> harmonic(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            double t = i * deltaT;
            harmonic[i] = amplitude * sin(2 * M_PI * frequency * t + phase);
        }

        return harmonic;
    }

    static void exportData(const SignalData& data, const std::string& filename) {

        std::ofstream outFile("../data/" + filename);
        if (!outFile) {
            throw std::runtime_error("Error opening output file.");
        }

        outFile << std::fixed << std::setprecision(6);

        const std::vector<double>& timeValues   = data.getTime();
        const std::vector<double>& signalValues = data.getSignal();
        
        if (timeValues.size() != signalValues.size()) {
            throw std::runtime_error("Data size mismatch.");
        }

        for (size_t i = 0; i < timeValues.size(); ++i) {
            outFile << timeValues[i] << "\t" << signalValues[i] << "\n";
        }

        outFile.close();
        std::cout << "Data successfully written to " + filename << "\n";
    }

    static SignalData importData(const std::string& filename) {

        std::ifstream inFile("../data/" + filename);
        if (!inFile) {
            throw std::runtime_error("Error opening input file.");
        }

        SignalData  data;
        std::string line;

        while (std::getline(inFile, line)) {
            double time, value;
            std::istringstream iss(line);

            if (iss >> time >> value) {
                data.addData(time, value);
            } else {
                continue;
            }
        }

        if (data.getTime().empty()) {
            throw std::runtime_error("No valid data found in file.");
        }

        std::cout << "Successfully imported " << data.getTime().size() 
                  << " data points from " << filename << "\n";
        
        return data;
    }
};

#endif // HARMONICUTILS_H
