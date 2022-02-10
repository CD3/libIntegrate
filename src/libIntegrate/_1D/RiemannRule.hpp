#pragma once
#include<cstddef>
#include <type_traits>

#include "./Utils.hpp"
#include "./RandomAccessLambda.hpp"

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

    /*
     * Integrate a discretized function from the set of argument and function values.
     */
    template<typename X, typename Y>
    auto operator()( const X &x, const Y &y, long ai = 0, long bi = -1 ) const -> decltype(libIntegrate::getSize(x),libIntegrate::getElement(x,0),libIntegrate::getElement(y,0),T())
    {
      // we are using getSize and getElement here so we can support
      // containers that use methods other than .operator[](int) and .size()
      // for indexing and reporting the size.
      // 
      // for example, Eigen uses operator()(int)
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
        sum += getElement(y,i)*(getElement(x,i+1)-getElement(x,i));

      return sum;
    }

    /*
     * Integrate a discretized function assuming uniform spacing.
     */
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

    /*
     * Integrate a callable between two points by
     * dividing it into a given number of intervals.
     */
    template<typename F>
    T operator()( F f, T a, T b, std::size_t N ) const
    {
      T dx = (b-a)/N;
      return
      this->operator()(
          _1D::RandomAccessLambda(
            [&a,&dx](int i){return a + i*dx;},
            [&N](){return N+1;} // N here means number of intervals. In the discretized functions, it means the number of points.
            ),
        [&a,&dx,&f](int i){ return f(a + i*dx); }
      );
    }

    /*
     * Integrate a callable between two points by
     * dividing it into a given number of intervals set at compile time.
     */
    template<typename F>
    T operator()( F f, T a, T b) const
    {
      return this->operator()(f,a,b,NN);
    }


  protected:
};

}
