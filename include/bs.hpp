#pragma once

namespace bs {

// Standard normal CDF: P(Z <= x) for Z ~ N(0,1).
double norm_cdf(double x);

// Standard normal density.
double norm_pdf(double x);

// S: spot, K: strike, r: risk-free rate (annual, continuous),
// sigma: volatility (annual), T: time to expiry in years.
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
