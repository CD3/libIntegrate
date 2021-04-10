#pragma once

#include<vector>

#include "./Utils.hpp"
#include "../_1D/RandomAccessLambda.hpp"
#include "../_2D/RandomAccessLambda.hpp"

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
    auto operator()( const X &x, const Y &y, const F &f ) const -> decltype(libIntegrate::getSize(x),libIntegrate::getSize(y),libIntegrate::getElement(x,0),libIntegrate::getElement(y,0),libIntegrate::getElement(f,0,0),DataType())
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

    template<typename F>
    auto operator()( const F &f, DataType dx, DataType dy ) const -> decltype(libIntegrate::getSizeX(f),libIntegrate::getSizeY(f),libIntegrate::getElement(f,0,0),DataType())
    {
      using libIntegrate::getSizeX;
      using libIntegrate::getSizeY;
      using libIntegrate::getElement;
      std::vector<DataType> sums(getSizeX(f));
      for(std::size_t i = 0; i < sums.size(); ++i)
      {
        sums[i] = integrate(  _1D::RandomAccessLambda( [&f,i](std::size_t j){return f[i][j];}, [&f](){return libIntegrate::getSizeY(f);}), dy);
      }
      return integrate(sums,dx);
    }

    template<typename F>
    DataType operator()( F f, DataType xa, DataType xb, std::size_t xN, DataType ya, DataType yb, std::size_t yN ) const
    {
      // discretize the function with lambda functions
      // and call the discretized function integrators
      DataType dx = (xb-xa)/xN;
      DataType dy = (yb-ya)/yN;
      return this->operator()(
          _1D::RandomAccessLambda([&xa,&dx](int i){return xa + i*dx;},[&xN](){return xN+1;}),
          _1D::RandomAccessLambda([&ya,&dy](int j){return ya + j*dy;},[&yN](){return yN+1;}),
          [&xa,&ya,&dx,&dy,&f](int i, int j){ return f(xa+i*dx,ya+j*dy); }
      );
    }

  protected:
};



}
