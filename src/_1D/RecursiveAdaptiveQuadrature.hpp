#ifndef _1D_RecursiveAdaptiveQuadrature_hpp
#define _1D_RecursiveAdaptiveQuadrature_hpp

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <cmath>

/** @file RecursiveAdaptiveQuadrature.hpp
  * @author C.D. Clark III
  * @date 05/18/17
  */

namespace _1D {

/** @class RecursiveAdaptiveQuadrature
  * @brief An adaptive integration method based on the trapezoid rule.
  * @author C.D. Clark III
  */
template<template <typename> class Quadrature, typename T>
class RecursiveAdaptiveQuadrature
{
  public:
    RecursiveAdaptiveQuadrature () {};
    virtual ~RecursiveAdaptiveQuadrature () {};

    template<typename F>
    T operator()( F f, T a, T b, size_t N = 2, boost::optional<T> estimate = boost::none);

    double tol = 1e-2;
    

  protected:
    Quadrature<T> integrate;
};




template<template <typename> class Q, typename T>
template<typename F>
T RecursiveAdaptiveQuadrature<Q,T>::operator()( F f, T a, T b, size_t N, boost::optional<T> estimate )
{
  if(N < 2)
    throw std::runtime_error("Cannot use trapezoid rule to evaluate integral with less than 2 points.");

  // if we don't have an estimate, then use the trapezoid rule.
  if(!estimate)
    return this->operator()(f,a,b,3,integrate(f,a,b,N));

  // we will break the integral up into N intervals
  // and compute the integral for each sub-interval.
  T *sums = new T[N-1]; // storage for the sums of each sub-interval
  T sum = 0;            // the sum of all sub-interval sums
  T dx = (b-a)/(N-1);   // the width of each sub-interval
  T x = a;
  for(int i = 0; i < N-1; i++)
  {
    // use the trapezoid rule to evaluate the sum of each sub-interval
    sums[i] = integrate(f,x,x+dx,N);
    x += dx;
  }
  // now add up the sub-interval sums
  for( int i = 0; i < N-1; i++)
    sum += sums[i];
  // if the difference between the new sum and the estimate exceeds the tolerance
  // we will re-evaluate each sub-interval with a recursive call.
  if( 2*std::abs(sum - *estimate)/(sum + *estimate) > tol )
  {
    sum = 0;
    x = a;
    for( int i = 0; i < N-1; i++)
    {
      sum += this->operator()(f,x,x+dx,N,sums[i]);
      x += dx;
    }
  }
  delete[] sums;
  return sum;
}

}

#endif // include protector
