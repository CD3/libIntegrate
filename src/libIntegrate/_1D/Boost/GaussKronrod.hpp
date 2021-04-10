#pragma once
#include<cstddef>

/** @file GaussKronrod.hpp
 * @brief
 * @author C.D. Clark III
 * @date 05/25/20
 */

#include <boost/math/quadrature/gauss_kronrod.hpp>

namespace _1D
{
namespace Boost
{
  /**
   * A wrapper around the Boost.Math gauss_kronrob function
   */
template<typename T, std::size_t Order>
class GaussKronrodQuadrature{
  public:
    
    template<typename F>
    auto operator()(F f, T a, T b, unsigned max_depth, T tol, T* error, T* pL1) const -> T
    {
      return boost::math::quadrature::gauss_kronrod<T,Order>::integrate(f,a,b,max_depth,tol,error,pL1);
    }

    template<typename F>
    auto operator()(F f, T a, T b) const -> T
    {
      return boost::math::quadrature::gauss_kronrod<T,Order>::integrate(f,a,b);
    }

};
}  // namespace boost
}  // namespace _1D
