#ifndef _1D_AdaptiveQuadrature_hpp
#define _1D_AdaptiveQuadrature_hpp

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <cmath>

/** @file AdaptiveQuadrature.hpp
  * @author C.D. Clark III
  * @date 05/18/17
  */

namespace _1D {

/** @class AdaptiveQuadrature
  * @brief An adaptive integration method based on the trapezoid rule.
  * @author C.D. Clark III
  */
template<template <typename> class Quadrature, typename T>
class AdaptiveQuadrature
{
  public:
    AdaptiveQuadrature () {};
    virtual ~AdaptiveQuadrature () {};

    template<typename F>
    T operator()( F f, T a, T b, size_t N = 2, boost::optional<T> e = boost::none, size_t d = 0);

    double tol = 1e-2;
    size_t max_recurssion = 20;

#ifdef TESTING
    size_t num_calls = 0;
#endif
    

  protected:
    Quadrature<T> integrate;
};




/**
 * @param f function or functor to be integrated.
 * @param a lower limit of integration.
 * @param b upper limit of integration.
 * @param N number of sub-intervals to divide the interval [a,b] into.
 * @param I an estimate of the integral, used to estimate error.
 * @param d current depth of recursion.
 */
template<template <typename> class Q, typename T>
template<typename F>
T AdaptiveQuadrature<Q,T>::operator()( F f, T a, T b, size_t N, boost::optional<T> I, size_t d)
{
#ifdef TESTING
  num_calls++;
#endif

  // if we don't have an estimate, then use the non-adaptive integrator.
  if(!I)
    return this->operator()(f,a,b,N,integrate(f,a,b,N),0);

  if(d > max_recurssion)
    return *I;
  // if we have exceeded the max recursion depth, return the estimate

  // we will break the integral up into N intervals
  // and compute the integral for each sub-interval.
  T *sums = new T[N];   // storage for the sums of each sub-interval
  T sum = 0;            // the sum of all sub-interval sums
  T dx = (b-a)/N;       // the width of each sub-interval
  T x = a;
  for(int i = 0; i < N; i++)
  {
    // use the integrator to evaluate sum for each each sub-interval
    sums[i] = integrate(f,x,x+dx,N);
    x += dx;
  }
  // now add up the sub-interval sums
  for( int i = 0; i < N; i++)
    sum += sums[i];
  // if the difference between the new sum and the estimate exceeds the tolerance
  // we will re-evaluate each sub-interval with a recursive call.
  if( 2*std::abs(sum - *I)/(sum + *I) > tol )
  {
    sum = 0;
    x = a;
    for( int i = 0; i < N; i++)
    {
      sum += this->operator()(f,x,x+dx,N,sums[i],d+1);
      x += dx;
    }
  }
  delete[] sums;
  return sum;
}

}

#endif // include protector
