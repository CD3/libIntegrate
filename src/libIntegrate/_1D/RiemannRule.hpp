#pragma once

#include<type_traits>

namespace _1D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T, size_t NN = 0>
class RiemannRule
{
  public:
    RiemannRule() = default;

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


template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T RiemannRule<T,NN>::operator()( F f, T a, T b, size_t N ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/N; // make sure we don't get integer rounding
  T x = a;
  for(size_t i = 0; i < N; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T RiemannRule<T,NN>::operator()( F f, T a, T b) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/NN; // make sure we don't get integer rounding
  T x = a;
  for(size_t i = 0; i < NN; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

template<typename T, size_t NN>
template<typename X, typename Y>
T RiemannRule<T,NN>::operator()( X &x, Y &y ) const
{
  T sum = 0;
  for(size_t i = 0; i < x.size()-1; i++)
    sum += y[i]*(x[i+1]-x[i]);

  return sum;
}

}
