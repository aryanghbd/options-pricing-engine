#include "bs.hpp"

#include <cmath>

namespace bs {

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double norm_pdf(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2.0 * M_PI);
}

namespace {
double d1(double S, double K, double r, double sigma, double T) {
    return (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
}
}  // namespace

double call(double S, double K, double r, double sigma, double T) {
    const double D1 = d1(S, K, r, sigma, T);
    const double D2 = D1 - sigma * std::sqrt(T);
    return S * norm_cdf(D1) - K * std::exp(-r * T) * norm_cdf(D2);
}

double put(double S, double K, double r, double sigma, double T) {
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

}  // namespace bs
