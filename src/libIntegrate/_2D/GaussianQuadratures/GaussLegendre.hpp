#pragma once

/** @file GaussLegendre.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 08/04/17
  */

#include<array>
#include "../../_1D/GaussianQuadratures/GaussLegendre.hpp"

namespace _2D {
namespace GQ {

template<typename T, size_t Order>
class GaussLegendreQuadrature
{
  public:
    _1D::GQ::GaussLegendreQuadrature<T,Order> _1dInt;

    GaussLegendreQuadrature() = default;

    // This version will integrate a callable between four points
    template<typename F, typename X, typename Y>
    T operator()( F f, X a, X b, Y c, Y d ) const;

  protected:
};


template<typename T, size_t Order>
template<typename F, typename X, typename Y>
T GaussLegendreQuadrature<T,Order>::operator()(F f, X a, X b, Y c, Y d) const
{
  // A 2D integral I = \int \int f(x,y) dx dy
  // can be written as two 1D integrals
  //
  // g(x) = \int f(x,y) dy
  // I = \int g(x) dx
  //
  // first, integrate over y at each of the required points (i.e. create g(x_i))

  X apb = (b + a)/2;
  X amb = (b - a)/2;
  std::array<T, Order> sums;

  #pragma parallel for
  for(size_t i = 0; i < Order; i++)
    sums[i] = _1dInt( [&](Y y){ return f(apb + amb*_1dInt.getX()[i], y); }, c, d );

  // now integrate over x
  T sum = 0;
  for(size_t i = 0; i < Order; i++)
    sum += _1dInt.getW()[i]*sums[i];
  sum *= amb;

  return sum;
}

}

}
