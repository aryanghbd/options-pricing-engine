#pragma once

namespace bs {

// Standard normal CDF: P(Z <= x) for Z ~ N(0,1).
double norm_cdf(double x);

// Standard normal density.
double norm_pdf(double x);

// Throws std::invalid_argument if any input is non-finite, S <= 0, K <= 0,
// sigma < 0 or T < 0. A negative r is allowed.
void validate_inputs(double S, double K, double r, double sigma, double T);

// S: spot, K: strike, r: risk-free rate (annual, continuous),
// sigma: volatility (annual), T: time to expiry in years.
// Prices throw std::invalid_argument on invalid inputs. At T == 0 or sigma == 0
// they return the limit value (payoff / deterministic-forward value).
// Greeks below throw std::domain_error at T == 0 or sigma == 0.
double call(double S, double K, double r, double sigma, double T);
double put(double S, double K, double r, double sigma, double T);

// Delta: change in option price per $1 change in spot.
double delta_call(double S, double K, double r, double sigma, double T);
double delta_put(double S, double K, double r, double sigma, double T);

// Gamma: change in delta per $1 change in spot (same for call and put).
double gamma(double S, double K, double r, double sigma, double T);

// Vega: change in option price per 1.00 change in sigma (divide by 100 for
// "per vol point"). Same for call and put.
double vega(double S, double K, double r, double sigma, double T);

// Theta: change in option price per year of calendar time passing
// (i.e. -dPrice/dT). Divide by 365 for "per day".
double theta_call(double S, double K, double r, double sigma, double T);
double theta_put(double S, double K, double r, double sigma, double T);

// Rho: change in option price per 1.00 change in r (divide by 100 for
// "per percentage point").
double rho_call(double S, double K, double r, double sigma, double T);
double rho_put(double S, double K, double r, double sigma, double T);

}  // namespace bs
