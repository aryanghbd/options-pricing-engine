#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "bs.hpp"

static void check(bool ok, const char* what) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) std::exit(1);
}

int main() {
    const double S = 100, K = 100, r = 0.05, sigma = 0.2, T = 1.0;

    const double c = bs::call(S, K, r, sigma, T);
    const double p = bs::put(S, K, r, sigma, T);

    check(std::fabs(c - 10.4506) < 1e-3, "call matches hand calc (~10.45)");
    check(std::fabs((c - p) - (S - K * std::exp(-r * T))) < 1e-9, "put-call parity");
    check(std::fabs(bs::norm_cdf(0.0) - 0.5) < 1e-12, "N(0) = 0.5");

    // Delta vs. a central finite difference of the price.
    const double h = 1e-4;
    const double fd_call = (bs::call(S + h, K, r, sigma, T) - bs::call(S - h, K, r, sigma, T)) / (2 * h);
    const double fd_put = (bs::put(S + h, K, r, sigma, T) - bs::put(S - h, K, r, sigma, T)) / (2 * h);
    check(std::fabs(bs::delta_call(S, K, r, sigma, T) - fd_call) < 1e-6, "call delta matches finite difference");
    check(std::fabs(bs::delta_put(S, K, r, sigma, T) - fd_put) < 1e-6, "put delta matches finite difference");
    check(std::fabs(bs::delta_call(S, K, r, sigma, T) - bs::delta_put(S, K, r, sigma, T) - 1.0) < 1e-12,
          "call delta - put delta = 1");

    // Gamma vs. a central finite difference of delta.
    const double fd_gamma = (bs::delta_call(S + h, K, r, sigma, T) - bs::delta_call(S - h, K, r, sigma, T)) / (2 * h);
    check(std::fabs(bs::gamma(S, K, r, sigma, T) - fd_gamma) < 1e-6, "gamma matches finite difference of delta");
    check(std::fabs(bs::gamma(S, K, r, sigma, T) -
                    ((bs::put(S + 1.0, K, r, sigma, T) - 2 * bs::put(S, K, r, sigma, T) +
                      bs::put(S - 1.0, K, r, sigma, T)))) < 1e-3,
          "gamma matches second difference of put price");

    // Vega vs. a central finite difference in sigma (call and put share it).
    const double fd_vega_call = (bs::call(S, K, r, sigma + h, T) - bs::call(S, K, r, sigma - h, T)) / (2 * h);
    const double fd_vega_put = (bs::put(S, K, r, sigma + h, T) - bs::put(S, K, r, sigma - h, T)) / (2 * h);
    check(std::fabs(bs::vega(S, K, r, sigma, T) - fd_vega_call) < 1e-6, "vega matches finite difference (call)");
    check(std::fabs(bs::vega(S, K, r, sigma, T) - fd_vega_put) < 1e-6, "vega matches finite difference (put)");

    // Theta is minus the derivative with respect to time remaining.
    const double fd_theta_call = -(bs::call(S, K, r, sigma, T + h) - bs::call(S, K, r, sigma, T - h)) / (2 * h);
    const double fd_theta_put = -(bs::put(S, K, r, sigma, T + h) - bs::put(S, K, r, sigma, T - h)) / (2 * h);
    check(std::fabs(bs::theta_call(S, K, r, sigma, T) - fd_theta_call) < 1e-6, "theta matches finite difference (call)");
    check(std::fabs(bs::theta_put(S, K, r, sigma, T) - fd_theta_put) < 1e-6, "theta matches finite difference (put)");
    return 0;
}
