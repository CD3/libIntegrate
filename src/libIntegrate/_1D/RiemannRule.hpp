#pragma once
#include<cstddef>

#include <type_traits>

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
    auto operator()( const X &x, const Y &y ) const -> decltype(x.size(),x[0],y[0],T());

    // This version will integrate a set of discrete points that are equally spaced
    template<typename Y>
    auto operator()( Y &y, T dx = 1 ) const -> decltype(y.size(),dx*y[0],T());

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

template<typename T, std::size_t NN>
template<typename X, typename Y>
auto RiemannRule<T,NN>::operator()( const X &x, const Y &y ) const -> decltype(x.size(),x[0],y[0], T())
{
  T sum = 0;
  for(std::size_t i = 0; i < x.size()-1; i++)
    sum += y[i]*(x[i+1]-x[i]);

  return sum;
}

template<typename T, std::size_t NN>
template<typename Y>
auto RiemannRule<T,NN>::operator()( Y &y, T dx ) const -> decltype(y.size(),dx*y[0],T())
{
  T sum = 0;
  for(std::size_t i = 0; i < y.size(); i++)
    sum += y[i];
  sum *= dx;

  return sum;
}

}
