#include "mc.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace bs {

namespace {
McResult simulate(double S, double K, double r, double sigma, double T, long N,
                  std::uint64_t seed, bool is_call) {
    std::mt19937_64 rng(seed);
    std::normal_distribution<double> norm(0.0, 1.0);

    const double drift = (r - 0.5 * sigma * sigma) * T;
    const double vol = sigma * std::sqrt(T);

    double sum = 0.0, sum_sq = 0.0;
    for (long i = 0; i < N; ++i) {
        const double S_T = S * std::exp(drift + vol * norm(rng));
        const double payoff = is_call ? std::max(S_T - K, 0.0) : std::max(K - S_T, 0.0);
        sum += payoff;
        sum_sq += payoff * payoff;
    }

    const double mean = sum / N;
    const double var = (sum_sq / N - mean * mean) * N / (N - 1);
    const double disc = std::exp(-r * T);
    return {disc * mean, disc * std::sqrt(var / N)};
}
}  // namespace

McResult mc_call(double S, double K, double r, double sigma, double T, long N,
                 std::uint64_t seed) {
    return simulate(S, K, r, sigma, T, N, seed, true);
}

McResult mc_put(double S, double K, double r, double sigma, double T, long N,
                std::uint64_t seed) {
    return simulate(S, K, r, sigma, T, N, seed, false);
}

}  // namespace bs
