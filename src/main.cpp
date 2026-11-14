#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bs.hpp"
#include "mc.hpp"

int main(int argc, char** argv) {
    const bool mc = argc == 7 && std::strcmp(argv[6], "mc") == 0;
    if (argc != 6 && !mc) {
        std::fprintf(stderr, "usage: %s S K r sigma T [mc]\n", argv[0]);
        return 1;
    }
    const double S = std::atof(argv[1]);
    const double K = std::atof(argv[2]);
    const double r = std::atof(argv[3]);
    const double sigma = std::atof(argv[4]);
    const double T = std::atof(argv[5]);

    const double c = bs::call(S, K, r, sigma, T);
    const double p = bs::put(S, K, r, sigma, T);
    std::printf("call: %.4f\nput:  %.4f\n", c, p);

    if (mc) {
        std::printf("\nMonte Carlo convergence (call, exact = %.4f)\n", c);
        std::printf("%10s %10s %10s %10s\n", "N", "estimate", "std err", "|error|");
        for (long N = 1000; N <= 10000000; N *= 10) {
            const bs::McResult m = bs::mc_call(S, K, r, sigma, T, N);
            std::printf("%10ld %10.4f %10.4f %10.4f\n", N, m.price, m.std_error,
                        std::fabs(m.price - c));
        }
    }
    return 0;
}
