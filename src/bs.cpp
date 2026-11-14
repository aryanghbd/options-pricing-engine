#include "bs.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace bs {

void validate_inputs(double S, double K, double r, double sigma, double T) {
    if (!std::isfinite(S) || !std::isfinite(K) || !std::isfinite(r) ||
        !std::isfinite(sigma) || !std::isfinite(T)) {
        throw std::invalid_argument("inputs must be finite numbers");
    }
    if (S <= 0.0) throw std::invalid_argument("S must be > 0");
    if (K <= 0.0) throw std::invalid_argument("K must be > 0");
    if (sigma < 0.0) throw std::invalid_argument("sigma must be >= 0");
    if (T < 0.0) throw std::invalid_argument("T must be >= 0");
}

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double norm_pdf(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2.0 * M_PI);
}

namespace {
// Greeks (and the main pricing path) go through d1, so this is where the
// undefined cases T = 0 and sigma = 0 are rejected for them.
double d1(double S, double K, double r, double sigma, double T) {
    validate_inputs(S, K, r, sigma, T);
    if (T == 0.0 || sigma == 0.0) {
        throw std::domain_error("undefined for T == 0 or sigma == 0");
    }
    return (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
}
}  // namespace

double call(double S, double K, double r, double sigma, double T) {
    validate_inputs(S, K, r, sigma, T);
    if (T == 0.0 || sigma == 0.0) {
        // Deterministic forward: the stock grows at r with no randomness.
        return std::max(S - K * std::exp(-r * T), 0.0);
    }
    const double D1 = d1(S, K, r, sigma, T);
    const double D2 = D1 - sigma * std::sqrt(T);
    return S * norm_cdf(D1) - K * std::exp(-r * T) * norm_cdf(D2);
}

double put(double S, double K, double r, double sigma, double T) {
    validate_inputs(S, K, r, sigma, T);
    if (T == 0.0 || sigma == 0.0) {
        return std::max(K * std::exp(-r * T) - S, 0.0);
    }
    const double D1 = d1(S, K, r, sigma, T);
    const double D2 = D1 - sigma * std::sqrt(T);
    return K * std::exp(-r * T) * norm_cdf(-D2) - S * norm_cdf(-D1);
}

double delta_call(double S, double K, double r, double sigma, double T) {
    return norm_cdf(d1(S, K, r, sigma, T));
}

double delta_put(double S, double K, double r, double sigma, double T) {
    return norm_cdf(d1(S, K, r, sigma, T)) - 1.0;
}

double gamma(double S, double K, double r, double sigma, double T) {
    return norm_pdf(d1(S, K, r, sigma, T)) / (S * sigma * std::sqrt(T));
}

double vega(double S, double K, double r, double sigma, double T) {
    return S * norm_pdf(d1(S, K, r, sigma, T)) * std::sqrt(T);
}

namespace {
// Decay term shared by call and put theta.
double theta_decay(double S, double K, double r, double sigma, double T) {
    return -S * norm_pdf(d1(S, K, r, sigma, T)) * sigma / (2.0 * std::sqrt(T));
}
}  // namespace

double theta_call(double S, double K, double r, double sigma, double T) {
    const double D2 = d1(S, K, r, sigma, T) - sigma * std::sqrt(T);
    return theta_decay(S, K, r, sigma, T) - r * K * std::exp(-r * T) * norm_cdf(D2);
}

double theta_put(double S, double K, double r, double sigma, double T) {
    const double D2 = d1(S, K, r, sigma, T) - sigma * std::sqrt(T);
    return theta_decay(S, K, r, sigma, T) + r * K * std::exp(-r * T) * norm_cdf(-D2);
}

double rho_call(double S, double K, double r, double sigma, double T) {
    const double D2 = d1(S, K, r, sigma, T) - sigma * std::sqrt(T);
    return K * T * std::exp(-r * T) * norm_cdf(D2);
}

double rho_put(double S, double K, double r, double sigma, double T) {
    const double D2 = d1(S, K, r, sigma, T) - sigma * std::sqrt(T);
    return -K * T * std::exp(-r * T) * norm_cdf(-D2);
}

}  // namespace bs
