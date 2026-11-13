#pragma once

namespace bs {

// Standard normal CDF: P(Z <= x) for Z ~ N(0,1).
double norm_cdf(double x);

// S: spot, K: strike, r: risk-free rate (annual, continuous),
// sigma: volatility (annual), T: time to expiry in years.
double call(double S, double K, double r, double sigma, double T);
double put(double S, double K, double r, double sigma, double T);

}  // namespace bs
