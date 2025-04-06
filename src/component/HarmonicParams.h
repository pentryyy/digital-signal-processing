#ifndef HARMONICPARAMS_H
#define HARMONICPARAMS_H

#include <vector>

class HarmonicParams {
public:
    static const std::vector<std::tuple<double, double, double>> harmonicParams;
};

#endif // HARMONICPARAMS_H

#ifndef PI
    #define PI 3.14159265358979323846
#endif

/*
Параметры гармоник
амплитуда, частота, начальная фаза
*/
const std::vector<std::tuple<double, double, double>> HarmonicParams::harmonicParams = {
    {2.0, 50.0,  PI * 0},
    {1.5, 100.0, PI / 4},
    {1.0, 200.0, PI / 2},
    {0.8, 300.0, PI * 3 / 4},
    {0.6, 400.0, PI},
    {0.5, 500.0, PI / 6},
    {0.4, 600.0, PI / 3},
    {0.3, 700.0, PI * 2 / 3},
    {0.2, 800.0, PI * 5 / 6}, 
    {0.1, 900.0, PI / 8}
};
