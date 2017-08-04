# C++ library for numerical integration

`libIntegrate` is a collection of algorithms for doing numerical integration.

# Description

When writing numerical simulations, one often needs to integrate some function, or an approximation of some function.
This library implements several common integration algorithms in C++. It is not as sophisticated (actually, its not very sophisticated at all), or complete (actually, its not very complete at all), as
the Gnu Scientific Library (https://www.gnu.org/software/gsl/), Numerical Recipes (http://numerical.recipes/), or QUADPACK (http://www.netlib.org/quadpack/),
but it is released under a permissive license, and you are free to do whatever you want with it.

Methods currently implemented include (but are not limited to):

- 1D
    - Riemann sum (the one that every undergrad physics major writes)
    - Trapezoid rule
    - Simpson's rule
    - Gauss-Lengendre Quadrature (currently only of order 64)
- 2D
    - Riemann sum
    - Gauss-Lengendre Quadrature (currently only of order 64)

