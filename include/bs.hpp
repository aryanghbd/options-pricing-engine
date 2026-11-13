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

}  // namespace bs
