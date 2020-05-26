#pragma once

#include<type_traits>

namespace _1D {

/** @class 
  * @brief A class that implements Simposon's rule.
  * @author C.D. Clark III
  */
template<typename T, size_t NN = 0>
class SimpsonRule
{
  public:
    SimpsonRule() = default;

    // This version will integrate a callable between two points
    template<typename F, size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_==0)>::type>
    T operator()( F f, T a, T b, size_t N ) const;

    template<typename F, size_t NN_ = NN, typename SFINAE = typename std::enable_if<(NN_>0)>::type>
    T operator()( F f, T a, T b) const;

  protected:
};


template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T SimpsonRule<T,NN>::operator()( F f, T a, T b, size_t N ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/N; // size of each interval
  T x;
  for(x = a; x < b; x += dx)
  {
    sum += f(x) + 4*f(x + dx/2) + f(x + dx);
  }
  // note that 2*h = dx
  sum *= dx/6;
  return sum;
}

template<typename T, size_t NN>
template<typename F, size_t NN_, typename SFINAE>
T SimpsonRule<T,NN>::operator()( F f, T a, T b ) const
{
  T sum = 0;
  T dx = static_cast<T>(b-a)/NN; // size of each interval
  T x;
  for(x = a; x < b; x += dx)
  {
    sum += f(x) + 4*f(x + dx/2) + f(x + dx);
  }
  // note that 2*h = dx
  sum *= dx/6;
  return sum;
}

}

