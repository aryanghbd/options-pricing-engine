#include <cstdio>
#include <cstdlib>

#include "bs.hpp"

int main(int argc, char** argv) {
    if (argc != 6) {
        std::fprintf(stderr, "usage: %s S K r sigma T\n", argv[0]);
        return 1;
    }
    const double S = std::atof(argv[1]);
    const double K = std::atof(argv[2]);
    const double r = std::atof(argv[3]);
    const double sigma = std::atof(argv[4]);
    const double T = std::atof(argv[5]);

    std::printf("call: %.4f\nput:  %.4f\n", bs::call(S, K, r, sigma, T),
                bs::put(S, K, r, sigma, T));
    return 0;
}
