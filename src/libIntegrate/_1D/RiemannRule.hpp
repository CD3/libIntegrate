#pragma once
#include<cstddef>
#include <type_traits>

#include "./Utils.hpp"

namespace _1D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T, std::size_t NN = 0>
class RiemannRule
{
  public:
    RiemannRule() = default;

    // This version will integrate a callable between two points using a number of evaluations set at run-time
    template<typename F, std::size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_==0)>::type>
    T operator()( F f, T a, T b, std::size_t N ) const;

    // This version will integrate a callable between two points using a number of evaluation set at compile-time 
    template<typename F, std::size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_>0)>::type>
    T operator()( F f, T a, T b) const;

    // This version will integrate a set of discrete points
    template<typename X, typename Y>
    auto operator()( const X &x, const Y &y ) const -> decltype(libIntegrate::getSize(x),libIntegrate::getElement(x,0),libIntegrate::getElement(y,0),T())
    {
      // we are using getSize and getElement here so we can support
      // containers that use methods other than .operator[](int) and .size()
      // for indexing and reporting the size.
      // 
      // for example, Eigen uses operator()(int)
      using libIntegrate::getSize;
      using libIntegrate::getElement;
      T sum = 0;
      for(decltype(getSize(x)) i = 0; i < getSize(x)-1; i++)
        sum += getElement(y,i)*(getElement(x,i+1)-getElement(x,i));

      return sum;
    }

    // This version will integrate a set of discrete points that are equally spaced
    template<typename Y>
    auto operator()( const Y &y, T dx = 1 ) const -> decltype(libIntegrate::getSize(y),dx*libIntegrate::getElement(y,0),T())
    {
      using libIntegrate::getSize;
      using libIntegrate::getElement;
      T sum = 0;
      for(decltype(getSize(y)) i = 0; i < getSize(y); i++)
        sum += getElement(y,i);
      sum *= dx;

      return sum;
    }

  protected:
};


template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T RiemannRule<T,NN>::operator()( F f, T a, T b, std::size_t N ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/N; // make sure we don't get integer rounding
  T x = a;
  for(std::size_t i = 0; i < N; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T RiemannRule<T,NN>::operator()( F f, T a, T b) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/NN; // make sure we don't get integer rounding
  T x = a;
  for(std::size_t i = 0; i < NN; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

}
