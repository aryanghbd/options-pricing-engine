# Options Pricing Engine

A lightweight C++17 implementation of the **Black–Scholes–Merton model** for pricing European options, calculating risk sensitivities (or.. the Greeks), and validating against analytical prices through Monte Carlo simulation.

Just for fun :P

## tl;dr features

- **Black–Scholes Pricing:** Closed-form pricing for European call and put options.
- **Option Greeks:** Analytical Δ, Γ, Θ, ν, ρ 
- **Monte Carlo Simulation:** European option pricing using some risk-neutral geometric Brownian motion.
- **Convergence Analysis:** Compare Monte Carlo estimates against analytical Black–Scholes prices, including standard errors.
- **Misc:** Put-call parity, finite-difference Greek verification, input validation, and edge-case tests.

## some math

The Black–Scholes–Merton formulas for European calls and puts goes as follows:

$$
C=S_0N(d_1)-Ke^{-rT}N(d_2)
$$

$$
P=Ke^{-rT}N(-d_2)-S_0N(-d_1)
$$

Whereupon:

$$
d_1=\frac{\ln(S_0/K)+(r+\sigma^2/2)T}{\sigma\sqrt{T}},
\qquad
d_2=d_1-\sigma\sqrt{T}
$$

- $S_0$: Current underlying price
- $K$: Strike price
- $r$: Continuously compounded risk-free rate
- $\sigma$: Annualized volatility
- $T$: Time to expiration (in years)
- $N(x)$: Standard normal cumulative distribution function

The model assumes constant volatility and interest rates, no dividends, and European-style exercise.

## build n run

Requires a C++17-compatible compiler and Make.

```bash
# Build the pricing engine
make build/price

# Run analytical pricing and Greeks
./build/price 100 100 0.05 0.20 1

# Include Monte Carlo convergence analysis
./build/price 100 100 0.05 0.20 1 mc

# Run tests
make test
```

Parameters are provided as:

```text
S K r sigma T [mc]
```

(nb: Rates and volatility are decimals (e.g., `0.05` for 5%)).

## Project Structure

```text
options-pricing-engine/
├── include/
│   ├── bs.hpp
│   └── mc.hpp
├── src/
│   ├── bs.cpp
│   ├── mc.cpp
│   └── main.cpp
├── tests/
│   └── test_bs.cpp
└── Makefile
```
