#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <stdexcept>

#include "bs.hpp"
#include "mc.hpp"

static void check(bool ok, const char* what) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) std::exit(1);
}

template <typename Ex, typename F>
static bool throws(F f) {
    try {
        f();
    } catch (const Ex&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
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

    // Rho vs. a central finite difference in r.
    const double fd_rho_call = (bs::call(S, K, r + h, sigma, T) - bs::call(S, K, r - h, sigma, T)) / (2 * h);
    const double fd_rho_put = (bs::put(S, K, r + h, sigma, T) - bs::put(S, K, r - h, sigma, T)) / (2 * h);
    check(std::fabs(bs::rho_call(S, K, r, sigma, T) - fd_rho_call) < 1e-6, "rho matches finite difference (call)");
    check(std::fabs(bs::rho_put(S, K, r, sigma, T) - fd_rho_put) < 1e-6, "rho matches finite difference (put)");
    check(std::fabs(bs::rho_call(S, K, r, sigma, T) - bs::rho_put(S, K, r, sigma, T) - K * T * std::exp(-r * T)) < 1e-9,
          "call rho - put rho = K*T*exp(-rT)");

    // Monte Carlo (fixed seed) should land within 4 standard errors of the closed form.
    const bs::McResult mc_c = bs::mc_call(S, K, r, sigma, T, 1000000);
    const bs::McResult mc_p = bs::mc_put(S, K, r, sigma, T, 1000000);
    check(std::fabs(mc_c.price - c) < 4 * mc_c.std_error, "Monte Carlo call within 4 SE of closed form");
    check(std::fabs(mc_p.price - p) < 4 * mc_p.std_error, "Monte Carlo put within 4 SE of closed form");
    check(bs::mc_call(S, K, r, sigma, T, 1000, 7).price == bs::mc_call(S, K, r, sigma, T, 1000, 7).price,
          "Monte Carlo is reproducible for a fixed seed");

    // Degenerate cases return limit values.
    check(bs::call(110, 100, r, sigma, 0.0) == 10.0 && bs::put(110, 100, r, sigma, 0.0) == 0.0,
          "T=0 call/put are intrinsic (ITM call)");
    check(bs::call(90, 100, r, sigma, 0.0) == 0.0 && bs::put(90, 100, r, sigma, 0.0) == 10.0,
          "T=0 call/put are intrinsic (ITM put)");
    check(std::fabs(bs::call(110, 100, r, sigma, 1e-8) - 10.0) < 1e-3, "price is continuous as T -> 0");
    check(std::fabs(bs::call(S, K, r, 0.0, T) - std::max(S - K * std::exp(-r * T), 0.0)) < 1e-12,
          "sigma=0 call is the deterministic forward value");
    check(std::fabs(bs::call(S, K, r, 1e-9, T) - bs::call(S, K, r, 0.0, T)) < 1e-6,
          "price is continuous as sigma -> 0");

    // Invalid inputs throw invalid_argument.
    const double nan = std::numeric_limits<double>::quiet_NaN();
    check(throws<std::invalid_argument>([&] { bs::call(0, K, r, sigma, T); }), "S=0 throws");
    check(throws<std::invalid_argument>([&] { bs::put(S, -1, r, sigma, T); }), "K<0 throws");
    check(throws<std::invalid_argument>([&] { bs::call(S, K, r, -0.1, T); }), "sigma<0 throws");
    check(throws<std::invalid_argument>([&] { bs::put(S, K, r, sigma, -1); }), "T<0 throws");
    check(throws<std::invalid_argument>([&] { bs::call(S, K, nan, sigma, T); }), "NaN input throws");
    check(throws<std::invalid_argument>([&] { bs::mc_call(S, K, r, sigma, T, 1); }), "Monte Carlo N<2 throws");
    check(!throws<std::invalid_argument>([&] { bs::call(S, K, -0.01, sigma, T); }), "negative r is allowed");

    // Greeks are undefined at T=0 or sigma=0.
    check(throws<std::domain_error>([&] { bs::delta_call(S, K, r, sigma, 0.0); }), "delta at T=0 throws");
    check(throws<std::domain_error>([&] { bs::gamma(S, K, r, 0.0, T); }), "gamma at sigma=0 throws");
    check(throws<std::domain_error>([&] { bs::rho_put(S, K, r, sigma, 0.0); }), "rho at T=0 throws");
    check(throws<std::invalid_argument>([&] { bs::vega(S, K, r, -0.2, T); }), "greeks reject invalid inputs");
    return 0;
}
