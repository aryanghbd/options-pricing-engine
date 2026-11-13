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
    return 0;
}
