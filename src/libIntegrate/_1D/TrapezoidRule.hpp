#pragma once
#include<cstddef>
#include<type_traits>

#include "./Utils.hpp"

namespace _1D {

/** @class 
  * @brief A class that implements the trapezoid rule.
  * @author C.D. Clark III
  */
template<typename T, std::size_t NN = 0>
class TrapezoidRule
{
  public:
    TrapezoidRule() = default;

    // This version will integrate a callable between two points
    template<typename F, std::size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_==0)>::type>
    T operator()( F f, T a, T b, std::size_t N ) const;

    template<typename F, std::size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_>0)>::type>
    T operator()( F f, T a, T b) const;

    // This version will integrate a set of discrete points
    template<typename X, typename Y>
    auto operator()( const X &x, const Y &y, long ai = 0, long bi = -1 ) const -> decltype(libIntegrate::getSize(x),libIntegrate::getElement(x,0),libIntegrate::getElement(y,0),T())
    {
      using libIntegrate::getSize;
      using libIntegrate::getElement;
      T sum = 0;

      auto N = getSize(x);
      if(N == 0)
        return sum;

      // support for negative indices.
      // interpret -n to mean the N-n index
      while( ai < 0 )
        ai += N;
      while( bi < 0 )
        bi += N;

      for(long i = ai; i < bi; i++)
        sum += (getElement(y,i+1)+getElement(y,i))*(getElement(x,i+1)-getElement(x,i));
      sum *= 0.5;

      return sum;
    }

    // This version will integrate a set of discrete points that are equally spaced
    template<typename Y>
    auto operator()( const Y &y, T dx = 1 ) const -> decltype(libIntegrate::getSize(y),dx*libIntegrate::getElement(y,0),T())
    {
      using libIntegrate::getSize;
      using libIntegrate::getElement;
      T sum = 0;
      for(decltype(getSize(y)) i = 0; i < getSize(y)-1; i++)
        sum += (getElement(y,i+1)+getElement(y,i));
      sum *= 0.5*dx;

      return sum;
    }

  protected:
};


/**
 * @param f function or functor to be integrated.
 * @param a lower limit of integration.
 * @param b upper limit of integration.
 * @param N number of *sub-intervals* to divide the integral [a,b] into.
 **/
template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T TrapezoidRule<T,NN>::operator()( F f, T a, T b, std::size_t N ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/N; // NOTE: N is the number of sub-intervals here
  T x = a;
  for(std::size_t i = 0; i < N; ++i, x += dx)
  {
    sum += f(x) + f(x + dx);
  }
  sum *= 0.5*dx;
  return sum;
}

template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T TrapezoidRule<T,NN>::operator()( F f, T a, T b ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/NN; // NOTE: N is the number of sub-intervals here
  T x = a;
  for(std::size_t i = 0; i < NN; ++i, x += dx)
  {
    sum += f(x) + f(x + dx);
  }
  sum *= 0.5*dx;
  return sum;
}

}


