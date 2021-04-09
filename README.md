# C++ library for numerical integration

`libIntegrate` is a collection of methods for numerical integration, **including discretized data**.

# Description

Most libraries for numerical integration provide routines for integrating functions that can be *evaluated*, but not
for functions that have already been *discretized*. Often, when doing physics simulations, one needs to integrate functions
that cannot be evaluated at arbitrary points, but for which the function values are known at some discrete set of points. For example,
we might use a finite-difference method to solve Schrodinger's equation and compute the time-evolution of a wavefunction describing some system.
If we then want to compute probabilities, we will need to integrate the wavefunction (actually, we will need to integrate the modulus squared of the wavefunction).

The usual methods for numerical integration are not suitable for this. This library provides methods that are.

If the function you need to integrate is already discretized, there are not many options to choose from. There is the Riemann sum, the Trapezoid rule, and
perhaps Simpson's rule. Each of these methods basically interpolate the function over some interval and use the integral of the interpolating function to estimate
the integral of the data. Riemann sum uses a constant (zero'th order polynomial), the Trapezoid rule uses a linear function (first order polynomial), and the 1/3 Simpson's rule
uses a quadratic (second degree polynomial). Higher-order interpolating functions are of course possible, but may not be appropriate, just as fitting high order polynomials
to data sets is usually not appropriate.

Quickly writing a Riemann sum loop to compute the integral of some discretized function is pretty easy. Writing the Trapezoid rule isn't difficult either, but you need to be careful not
to index out of range at the end of the loop. Simpson's rule is simple enough for uniform data, but again requires care at the end of the loop. For non-uniform data, it is a bit more involved.
I noticed that, when needing to integrate some discretized function, I would often just use a Riemann sum and say to myself "I'll come back and use a better method this after I get it working", but 
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

Methods currently implemented include (but are not limited to):

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

**Features:**

- Integrate one and two dimensional discretized functions, with Riemann, Trapezoid, and Simpson rules.
- Simple, clean, uniform interface. Integration methods are implemented as classes with `operator()(...)` methods.
- Handles several several common randome access, continuous memory containers (std::vector, Eigen::Vector, etc). Any container
  that provides element access with `operator[](int)` or `operator()(int)` and a method for getting the size (`.size()`, `.length()`, `.rows()`, etc) can be integrated.
- Support for non-uniform discretization.
- Apply lazy transformations to discrete data before integrating. Useful for computing weighted integrals.



# Usage

## Background


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

### Case 1: Functions that can be evaluated

Integration methods are implemented as separate classes that implement `operator()` (i.e. instances
can be called like functions). All classed in `libIntegrate` support Case 1, integrating
a function that can be evaluated at arbitrary arguments. For example, to integrate a function using Simposon's Rule:
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
The function to be evaluated is passed as the first argument
to the integrator, and can be any callable (including a lambda) that takes one argument. The lower and upper integration bounds are passed next.
The last argument specifies the number of evaluation points to use. Note that not all methods support specifying the number
of evaluation points. The Gauss-Legendre methods for example evaluate the function at a set number of points, given
by the order of the method. In this case, the number of evaluation points argument is dropped.

```cpp
#include <libIntegrate/_1D/GaussianQuadratures/GaussLegendre.hpp>

...

_1D::GQ::GaussLegendreQuadrature<double,8> integrate;

auto f  = [&N](double x) { return 1 + x + x*x + x*x*x; }
CHECK( integrate( f, 0., 1. )  == Approx( 1 + 1/2. + 1/3. + 1/4. - 1 ) );

```

### Case 2: Discretized functions

Some classes in `libIntegrate` support Case 2, integrating a discretized function. These classes implement
an `operator()` overload that takes two vector-like arguments containing the argument and function values.
```cpp
#include <libIntegrate/_1D/TrapezoidRule.hpp>

...

double linear_func(double x)
{
  return 2*x + 3;
}

...

_1D::TrapezoidRule<double> integrate;

double  I = integrate( linear_func, 2., 5., 2 );
CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

_1D::TrapezoidRule<double> integrate;

std::vector<double> x(3),y(3);
x[0] = 0; y[0] = 1;
x[1] = 1; y[1] = 2;
x[2] = 2; y[2] = 3;
  
double I = integrate( x, y );
CHECK( I == Approx( 4 ) );

// can still use integrator for Case 1
I = integrate( linear_func, 2., 5., 2 );
CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );
```
The discretized function to be integrated is passed as two vectors, the first containing the argument values, and the second
containing the function values.

See the unit tests (./testing/CatchTests/) for more examples.

### Weighted Integrals

Sometimes it's necessary to multiply a function by some weighting function
before integrating. For example, when integrating a function in polar
coordinates, or computing an expectation value from a probability distribution.
For Case 1, we can use a lambda to compute the weighted integrand.
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

We can use a lambda for Case 2 as well, but the it should accept an integer specifying the element index and return the value of the weighted integrand for the given index.

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

## Implemented Methods

### Riemann's Rule
```cpp

namespace _1D {
template<typename T>
class RiemannRule
{
  public:
    // This version will integrate a callable between two points
    template<typename F, typename X>
    T operator()( F f, X a, X b, size_t N ) const;

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

### Trapezoid Rule

```cpp
namespace _1D {
template<typename T>
class TrapezoidRule
{
  public:
    template<typename F, typename X>
    T operator()( F f, X a, X b, size_t N ) const;

    template<typename X, typename Y>
    T operator()( const X &x, const Y &y ) const;

    template<typename Y>
    T operator()( const Y &y, T dx ) const;
};
}
```

### Simpson's Rule

```cpp
namespace _1D {
template<typename T>
class SimpsonRule
{
  public:
  public:
    template<typename F, typename X>
    T operator()( F f, X a, X b, size_t N ) const;

    template<typename X, typename Y>
    T operator()( const X &x, const Y &y ) const;

    template<typename Y>
    T operator()( const Y &y, T dx ) const;
};
}
```

### Gaussian-Legandre Quadrature

```cpp
namespace _1D {
namespace GQ {

template<typename T, size_t Order> // Order = 8, 16, 32, or 64
class GaussLegendreQuadrature
{
  public:
    // This version will integrate a callable between two points
    template<typename F, typename X>
    T operator()( F f, X a, X b ) const;
    // Integrating a set of discrete points is not supported
};
}
}
```

### Gaussian-Kronrod Quadrature

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
