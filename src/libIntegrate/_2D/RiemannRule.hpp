#pragma once

#include<cstddef>
#include "./DiscretizedIntegratorWrapper.hpp"
#include "../_1D/RiemannRule.hpp"

namespace _2D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T>
class RiemannRule : public DiscretizedIntegratorWrapper<_1D::RiemannRule<T>>
{ 
  public:

    using BaseType = DiscretizedIntegratorWrapper<_1D::RiemannRule<T>>;
    using BaseType::operator();

    template<typename F>
    T operator()( F f, T xa, T xb, std::size_t xN, T ya, T yb, std::size_t yN ) const
    {
      T sum = 0;
      T dx = (xb-xa)/xN;
      T dy = (yb-ya)/yN;
      T y = ya;
      for(std::size_t i = 0; i < xN; i++)
      {
        T x = xa;
        for(std::size_t j = 0; j < yN; j++)
        {
          sum += f(x,y);
          x += dx;
        }
        y += dy;
      }
      sum *= dx*dy;
      return sum;
    }

};



}
