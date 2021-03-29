# C++ library for numerical integration

`libIntegrate` is a collection of algorithms for doing numerical integration.

# Description

When writing numerical simulations, one often needs to integrate some function, or an approximation of some function.
This library implements several common integration algorithms in C++. It is not
as sophisticated (actually, its not very sophisticated at all), or complete
(actually, its not very complete at all), as the Gnu Scientific Library
(https://www.gnu.org/software/gsl/), Numerical Recipes
(http://numerical.recipes/), or QUADPACK (http://www.netlib.org/quadpack/), but
it is released under a permissive license, and you are free to do whatever you
want with it.

Methods currently implemented include (but are not limited to):

- 1D
    - Riemann sum (the one that every undergrad physics major writes)
    - Trapezoid rule
    - Simpson's rule
    - Gauss-Legendre Quadrature of order 8, 16, 32, and 64.
- 2D
    - Riemann sum
    - Gauss-Legendre Quadrature of order 8, 16, 32, and 64.

# Usage

## Background


There are two basic types of numerical integration. In the first case, the value
of the function to be integrated can be calculated for any argument value. In the second
case, the value of the function to be integrated is only know at some discrete set
of argument values. Any method that can handle the second case can be used to handle
the first case as well by first discrediting the function to be integrated.
Methods that can only handle the first case (for example, Gauss-Legendre Quadrature
requires the function to be evaluated at specific argument values) can be used on discretized
functions, but require the discretized function to be interpolated to the argument values required by the method first.
The [libInterpolate](https://github.com/CD3/libInterpolate) library is useful for this.

## Examples

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
For Case 1, we can simply use a lambda.
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

For Case 2, a lambda function cannot be used because the integrator will make calls to the subscript operator. We could compute the weighted integrand and store
the result in another vector, but if will it only be used one, the memory allocation may be too expensive.
The library provides a class `SubscriptLambdaObj<T>` for this.
The helper function `SubscriptLambda(...)` is used to create an instance that wraps a lambda function and provides a subscript operator.
The lambda function can then capture any other data that is needed to compute the integrand, for example other vectors. Note that these should be captured by *reference*, or
else they will be copied when creating the `SubscriptLambdaObj<T>` instance.

```
#include <libIntegrate/_1D/RiemannRule.hpp>
#include <libIntegrate/_1D/SubscriptLambda.hpp>
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
auto f1 = _1D::SubscriptLambda( [&x, &rho](int i){return x[i]*rho[i];} );
CHECK(integrate(x,f1) == Approx(1./2).epsilon(0.01));
auto f3 = _1D::SubscriptLambda( [&x, &rho](int i){return (x[i]-0.5)*(x[i]-0.5)*rho[i];} );
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
    template<typename X, typename Y>
    T operator()( X &x, Y &y ) const;
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
    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T a, T b, size_t N ) const;

    // This version will integrate a set of discrete points
    template<typename X, typename Y>
    T operator()( X &x, Y &y ) const;

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
    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T a, T b, size_t N ) const;
    // Integrating a set of discrete points is not supported
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
