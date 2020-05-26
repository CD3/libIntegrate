#pragma once

#include<type_traits>

namespace _1D {

/** @class 
  * @brief A class that implements the trapezoid rule.
  * @author C.D. Clark III
  */
template<typename T, size_t NN = 0>
class TrapezoidRule
{
  public:
    TrapezoidRule() = default;

    // This version will integrate a callable between two points
    template<typename F, size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_==0)>::type>
    T operator()( F f, T a, T b, size_t N ) const;

    template<typename F, size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_>0)>::type>
    T operator()( F f, T a, T b) const;

    // This version will integrate a set of discrete points
    template<typename X, typename Y>
    T operator()( X &x, Y &y ) const;

  protected:
};


/**
 * @param f function or functor to be integrated.
 * @param a lower limit of integration.
 * @param b upper limit of integration.
 * @param N number of *sub-intervals* to divide the integral [a,b] into.
 **/
template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T TrapezoidRule<T,NN>::operator()( F f, T a, T b, size_t N ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/N; // NOTE: N is the number of sub-intervals here
  T x;
  for(x = a; x < b; x += dx)
  {
    sum += f(x) + f(x + dx);
  }
  sum *= 0.5*dx;
  return sum;
}

template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T TrapezoidRule<T,NN>::operator()( F f, T a, T b ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/NN; // NOTE: N is the number of sub-intervals here
  T x;
  for(x = a; x < b; x += dx)
  {
    sum += f(x) + f(x + dx);
  }
  sum *= 0.5*dx;
  return sum;
}

template<typename T, size_t NN>
template<typename X, typename Y>
T TrapezoidRule<T,NN>::operator()( X &x, Y &y ) const
{
  T sum = 0;
  for(size_t i = 0; i < x.size()-1; i++)
    sum += (y[i+1]+y[i])*(x[i+1]-x[i]);
  sum *= 0.5;

  return sum;
}

}


