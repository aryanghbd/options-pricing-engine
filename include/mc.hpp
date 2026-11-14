#pragma once

#include <cstdint>

namespace bs {

struct McResult {
    double price;
    double std_error;
};

// Monte Carlo price of a European option under risk-neutral GBM.
// N paths, reproducible for a given seed.
McResult mc_call(double S, double K, double r, double sigma, double T,
                 long N, std::uint64_t seed = 42);
McResult mc_put(double S, double K, double r, double sigma, double T,
                long N, std::uint64_t seed = 42);

}  // namespace bs
