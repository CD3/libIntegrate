#pragma once
#include<cstddef>

#include "./Utils.hpp"

namespace _2D {

/** @class 
  * @brief A class that does 2D integration on discretized functions using various rules.
  * @author C.D. Clark III
  */
template<typename Integrator>
class DiscretizedIntegratorWrapper
{
  Integrator integrate;
  template<typename T>
  struct getDataType {};

  template<template<typename> class V, typename T>
  struct getDataType<V<T>> {using type = T;};

  template<template<typename,std::size_t> class V, typename T, std::size_t N>
  struct getDataType<V<T,N>> {using type = T;};

  using DataType = typename getDataType<Integrator>::type;

  public:
  template<typename ...Args>
    DiscretizedIntegratorWrapper(Args&& ...args):integrate(std::forward<Args>(args)...) {}

    template<typename X, typename Y, typename F>
    auto operator()( const X &x, const Y &y, const F &f ) const -> decltype(DataType())
    {
      using libIntegrate::getSize;
      using libIntegrate::getElement;
      std::vector<DataType> sums(getSize(x));
      for(std::size_t i = 0; i < sums.size(); ++i)
      {
        sums[i] = integrate(y, [&f,i](std::size_t j){ return getElement(f,i,j); });
      }
      return integrate(x,sums);
    }

  protected:
};



}
