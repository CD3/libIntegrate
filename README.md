# C++ library for numerical integration

`libIntegrate` is a collection of methods for numerical integration, **including discretized data**.


**Features:**

- Integrate one and two dimensional discretized functions, with Riemann, Trapezoid, and Simpson rules.
- Simple, clean, uniform interface. Integration methods are implemented as classes with `operator()(...)` methods.
- Handles several common random access, continuous memory container interfacess (std::vector, Eigen::Vector, etc). Containers
  that provide element access with `operator[](int)` or `operator()(int)` and a method for getting the size (`.size()`, `.length()`, `.rows()`, etc) can be integrated.
- Support for non-uniform discretization.
- Apply lazy transformations to discrete data before integrating. Useful for computing weighted integrals.


Table of Contents
=================

* [Description](#description)
* [Usage](#usage)
   * [Installing](#installing)
   * [Overview](#overview)
   * [Examples](#examples)
      * [Discretized Functions](#discretized-functions)
      * [Callable Functions](#callable-functions)
      * [Weighted Integrals](#weighted-integrals)
   * [Implemented Methods](#implemented-methods)
      * [1D Riemann's Rule](#1d-riemanns-rule)
      * [1D Trapezoid Rule](#1d-trapezoid-rule)
      * [1D Simpson's Rule](#1d-simpsons-rule)
      * [1D Gaussian-Legandre Quadrature](#1d-gaussian-legandre-quadrature)
      * [1D Gaussian-Kronrod Quadrature](#1d-gaussian-kronrod-quadrature)
      * [2D Riemann's Rule](#2d-riemanns-rule)
      * [2D Trapezoid Rule](#2d-trapezoid-rule)
      * [2D Simpson's Rule](#2d-simpsons-rule)
      * [2D Gaussian-Legandre Quadrature](#2d-gaussian-legandre-quadrature)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->


# Description

It is often necessary to integrate functions that have been *discretized* when doing physics simulations or working with
experimental data. 
For example, we might use a finite-difference method to solve Schrodinger's equation and compute the time-evolution of a wavefunction describing some system.
If we then want to compute probabilities, we will need to integrate the wavefunction (actually, we will need to integrate the modulus squared of the wavefunction).
Or, we might have a measured spectrum for a broadband source that we need to integrate to compute the total power emitted.
In either case, we will have a discrete set of points to use for the integration.

Most libraries for numerical integration provide routines for integrating functions that can be *evaluated*, not
for functions that have already been *discretized*, and are therefore not suitable for this.
This library provides methods that are.

If the function you need to integrate is already discretized, there are not many options to choose from. There is the Riemann sum, the Trapezoid rule, and
perhaps Simpson's rule. Each of these methods basically interpolate the function over some interval and use the integral of the interpolating function to estimate
the integral of the data. Riemann sum uses a constant (zero'th order polynomial), the Trapezoid rule uses a linear function (first order polynomial), and the 1/3 Simpson's rule
uses a quadratic (second degree polynomial). Higher-order interpolating functions are of course possible, but may not be appropriate, just as fitting high order polynomials
to data sets is usually not appropriate.

Quickly writing a Riemann sum loop to compute the integral of some discretized function is pretty easy. Writing the Trapezoid rule isn't difficult either, but you need to be careful not
to index out of range at the end of the loop. Simpson's rule is simple enough for uniform data, but again requires care at the end of the loop. For non-uniform data, it is a bit more involved.
I noticed that, when needing to integrate some discretized function, I would often just use a Riemann sum and say to myself "I'll come back and use a better method after I get this working", but 
then wouldn't. So this library was created to make using "better" methods on discretized data easy.

While this library does provide methods for integrating functions that can be evaluated too, they are not nearly as
as sophisticated (actually, not very sophisticated at all), or complete
(actually, not very complete at all), as the Gnu Scientific Library
(https://www.gnu.org/software/gsl/), Numerical Recipes
(http://numerical.recipes/), QUADPACK (http://www.netlib.org/quadpack/),
or Boost (https://www.boost.org/doc/libs/1_75_0/libs/math/doc/html/quadrature.html).
If you need to integrate a known function with some requirement on accuracy,
you should use one of these libraries instead.

If you need to integrate **discretized data** and occasionally some simple functions that can
be evaluated, this library may be for you.
It is released under a permissive license, so you are free to do whatever you
want with it.

Methods currently implemented include:

- 1D
    - Discretized Functions
        - Riemann sum (the one that every undergrad physics major writes)
        - Trapezoid rule
        - Simpson's rule (1/3)
    - Callable Functions
        - Riemann sum
        - Trapezoid rule
        - Simpson's rule (1/3)
        - Gauss-Legendre Quadrature of order 8, 16, 32, and 64.
- 2D
    - Discretized Functions
        - Riemann sum
        - Trapezoid rule
        - Simpson's rule (1/3)
    - Callable Functions
        - Riemann sum
        - Trapezoid rule
        - Simpson's rule (1/3)
        - Gauss-Legendre Quadrature of order 8, 16, 32, and 64.

Note that the library depends on Boost, and does provide some (incomplete) wrappers to the Boost.Math quadrature functions.

- 1D
    - Gauss-Kronrod Quadrature (arbitrary order)

# Usage

## Installing

libIntegrate is a header-only library that uses CMake for building unit tests and installing. To install the library, clone this and checkout the latest release,
or download one of the release tarballs. Then,
```
$ cd libIntegrate
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target install
```

Conan packages are also provided. To install the library with conan, add the remote `https://cdc3.jfrog.io/artifactory/api/conan/default-conan` and include `libintegrate/VERSION@cd3/devel` to
you `conanfile.txt` or `conanfile.py`.
```
$ conan remote add cd3 https://cdc3.jfrog.io/artifactory/api/conan/default-conan
$ conan search -r cd3 # list all packages
```

## Overview


There are two basic types of numerical integration. In the first case, the value
of the function to be integrated can be calculated for any argument value. In the second
case, the value of the function to be integrated is only know at some discrete set
of argument values. Most numerical integration libraries handle the first case, and
there many different methods for handling a wide variety of conditions, including integrating over infinite or semi-infinite bounds.
Typically, the user should know something about the function to be integrated and carefully select
the best method to use.

Any method that can handle the second case can be used to handle
the first case as well by first discrediting the function to be integrated.
Methods that can only handle the first case (for example, Gauss-Legendre Quadrature
requires the function to be evaluated at specific argument values) can be used on discretized
functions, but require the discretized function to be interpolated to the argument values required by the method first.
The [libInterpolate](https://github.com/CD3/libInterpolate) library is useful for this.

## Examples

### Discretized Functions

In the simplest case, you have a discretized function with the argument and function values stored in two vectors.
```
std::vector<double> x(100), f(100);
double dx = M_PI/(x.size()-1);
for(int i = 0; i < x.size(); i++)
{
  x[i] = i*dx;
  f[i] = sin(x[i]);
}
```
To integrate the function, create an integrator object and call it on the discretized function.
```
#include <libIntegrate/Integrate.hpp>

_1D::RiemannRule<double> integrage;

double I = integrate(x,f); // I \approx 2
```
**Note: Discretized functions must be sorted with respect to the independent variable (`x` in this example).**

Each method is implemented in a class. To use a different method, instantiate the corresponding class.
```
_1D::TrapezoidRule<double> trap_integrage;
_1D::SimpsonRule<double> simp_integrage;
```

Now, in practice, you would not need to manually discretize a function that can be evaluated. All of the methods support integrating a callable
with a specified number of intervals. The example above is equivalent to
```
#include <libIntegrate/Integrate.hpp>

_1D::RiemannRule<double> integrage;

double I = integrate([](double x){return sin(x);}, 0, M_PI, 99);
```

However, when the discretized function is computed by solving a differential equation, or is data that was measured in an experiment, the discretized
integration is required.



Two-dimensional discretized functions can also be integrated. The 2D integrators that support discretized
functions take the arguments. The first two are 1D containers of the x and y coordinates, the third is a 2D container
of the function values. For example:

```
  _2D::TrapezoidRule<double> integrate;
  double                     I;

  // create a discretized version of sin(x)*sin(y) to integrate.
  // probably not the best way to create an initialize the function...
  std::vector<double>              x(100), y(200);
  std::vector<std::vector<double>> f(100);
  for(size_t i = 0; i < x.size(); i++) {
      x[i] = i * (M_PI / 2) / (x.size() - 1);
  }
  for(size_t i = 0; i < y.size(); i++) {
      y[i] = i * (M_PI / 2) / (y.size() - 1);
  }

  for(size_t i = 0; i < f.size(); i++) {
      f[i] = std::vector<double>(200);
      for(size_t j = 0; j < f[i].size(); j++) {
        f[i][j] = sin(x[i]) * sin(y[j]);
      }
  }

  // now integrate
  I = integrate(x, y, f);
```
We could also use more efficient 2D container here, like an Eigen matrix. Any container that we can access the elements of with either  `f(i,j)` or `f[i][j]` will work.




### Callable Functions

As mentioned in the description, some basic methods for integrating functions that can be evaluated are provided.
These methods take a callable, the limits of integration, and the number of intervals to use.
For example, to integrate a function using Simposon's Rule:
```cpp
#include <libIntegrate/_1D/SimpsonRule.hpp>
...

// define the function to integrate
double linear_func(double x)
{
  return 2*x + 3;
}

...

// create an integrator
// the template argument specifies the data type that will be used
_1D::SimpsonRule<double> integrate;

// integrate the function over the range [2:5] using 10 evaluation points
double I = integrate( linear_func, 2., 5., 10 );
CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

// we can also use lambdas
I = integrate([](double x){return 2*x + 3;},2.,5.,10);
CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

```
The first argument can be any callable (including a lambda) that takes one argument.
Note that not all methods support specifying the number
of intervals. The Gauss-Legendre methods for example evaluate the function at a set number of points, given
by the order of the method. In this case, the last argument is dropped.

```cpp
#include <libIntegrate/_1D/GaussianQuadratures/GaussLegendre.hpp>

...

_1D::GQ::GaussLegendreQuadrature<double,8> integrate;

auto f  = [&N](double x) { return 1 + x + x*x + x*x*x; }
CHECK( integrate( f, 0., 1. )  == Approx( 1 + 1/2. + 1/3. + 1/4. - 1 ) );

```



### Weighted Integrals

Sometimes it's necessary to multiply a function by some weighting function
before integrating. For example, when integrating a function in polar
coordinates, or computing an expectation value from a probability distribution.
Lambda functions can be used for this with both discretized and callable functions.

For discretized functions, the lambda should take an integral argument (the library actually requires the argument to be an integer type and will
generate a compiler error if it is not), and return the weighted integrand for each element of the function.

```
#include <libIntegrate/_1D/RiemannRule.hpp>
...

int N = 200;
std::vector<double> x(N), rho(N);
for(int i = 0; i < N; i++)
{
  x[i] = i*10./(N-1);
  rho[i] = 2*exp(-2*x[i]);
}

_1D::RiemannRule<double> integrate;

CHECK(integrate(x,rho) == Approx(1).epsilon(0.1));
auto f1 = [&x, &rho](int i){return x[i]*rho[i];};
CHECK(integrate(x,f1) == Approx(1./2).epsilon(0.01));
auto f3 = [&x, &rho](int i){return (x[i]-0.5)*(x[i]-0.5)*rho[i];};
CHECK(integrate(x,f3) == Approx(1./4).epsilon(0.1));

```
For callable functions, the lambda take the function argument should return the weighted integrand.
```
#include <libIntegrate/_1D/GaussianQuadratures/GaussLegendre.hpp>
...

// exponential probability distribution with mean free path of 1/2
double probability_density(double x)
{
  double mu = 2;
  return mu*exp(-mu*x);
}

...

_1D::GQ::GaussLegendreQuadrature<double,16> integrate;

// total probability should be one
CHECK(integrate(probability_density, 0, 10) == Approx(1));
// expected value (first moment) should be 1/2
CHECK(integrate([](double x){ return x*probability_density(x);}, 0, 10) == Approx(1./2));
// second moment should be 1/2
CHECK(integrate([](double x){ return x*x*probability_density(x);}, 0, 10) == Approx(1./4));
// variance should be 1/4
CHECK(integrate([](double x){ return (x-1./2)*(x-1./2)*probability_density(x);}, 0, 10) == Approx(1./4));
```


## Implemented Methods

### 1D Riemann's Rule
```cpp

namespace _1D {
template<typename T>
class RiemannRule
{
  public:
    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T a, T b, size_t N ) const;

    // This version will integrate a set of discrete points
    // with the x and y values stored in separate containers.
    template<typename X, typename Y>
    T operator()( const X &x, const Y &y ) const;

    // This version will integrate a set of uniformly discrete points
    template<typename Y>
    T operator()( const Y &y, T dx ) const;
};
}
```

### 1D Trapezoid Rule

```cpp
namespace _1D {
template<typename T>
class TrapezoidRule
{
  public:
    template<typename F>
    T operator()( F f, T a, T b, size_t N ) const;

    template<typename X, typename Y>
    T operator()( const X &x, const Y &y ) const;

    template<typename Y>
    T operator()( const Y &y, T dx ) const;
};
}
```

### 1D Simpson's Rule

```cpp
namespace _1D {
template<typename T>
class SimpsonRule
{
  public:
  public:
    template<typename F>
    T operator()( F f, T a, T b, size_t N ) const;

    template<typename X, typename Y>
    T operator()( const X &x, const Y &y ) const;

    template<typename Y>
    T operator()( const Y &y, T dx ) const;
};
}
```

### 1D Gaussian-Legandre Quadrature

```cpp
namespace _1D {
namespace GQ {

template<typename T, size_t Order> // Order = 8, 16, 32, or 64
class GaussLegendreQuadrature
{
  public:
    template<typename F>
    T operator()( F f, T a, T b ) const;
    // Integrating a set of discrete points is not supported
};
}
}
```

### 1D Gaussian-Kronrod Quadrature

The library also provides wrappers for some of the Boost.Math Quadrature routines. The Boost routines are implemented
as free functions. These wrappers simply provide an object-oriented interface.

```cpp
namespace _1D {
namespace Boost {
  /**
   * A wrapper around the Boost.Math gauss_kronrob function
   */
template<typename T>
class GaussKronrodQuadrature{
  public:
    
    template<typename F, typename X>
    auto operator()(F f, X a, X b, unsigned max_depth, T tol, T* error, T* pL1) const -> T

    template<typename F, typename X>
    auto operator()(F f, X a, X b) const -> T

};
}
}
```



### 2D Riemann's Rule
```cpp

namespace _2D {
template<typename T>
class RiemannRule
{
  public:
    // This version will integrate a callable between two points
    template<typename F, typename X>
    T operator()( F f, T xa, T xb, size_t xN, T ya, Ty yb, size_t yN) const;

    // This version will integrate a set of discrete points
    // with the x and y values stored in separate containers.
    template<typename X, typename Y, typename F>
    T operator()( const X &x, const Y &y, const F &f ) const;
};
}
```

### 2D Trapezoid Rule

```cpp
namespace _2D {
template<typename T>
class TrapezoidRule
{
  public:
    template<typename F, typename X>
    T operator()( F f, T xa, T xb, size_t xN, T ya, Ty yb, size_t yN) const;

    template<typename X, typename Y, typename F>
    T operator()( const X &x, const Y &y, const F &f ) const;
};
}
```

### 2D Simpson's Rule

```cpp
namespace _2D {
template<typename T>
class SimpsonRule
{
  public:
  public:
    template<typename F, typename X>
    T operator()( F f, T xa, T xb, size_t xN, T ya, Ty yb, size_t yN) const;

    template<typename X, typename Y, typename F>
    T operator()( const X &x, const Y &y, const F &f ) const;
};
}
```

### 2D Gaussian-Legandre Quadrature

```cpp
namespace _2D {
namespace GQ {

template<typename T, size_t Order> // Order = 8, 16, 32, or 64
class GaussLegendreQuadrature
{
  public:
    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T xa, T xb, T ya, T yb ) const;
};
}
}
```
