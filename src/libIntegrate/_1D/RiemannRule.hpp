#ifndef _1D_RiemannRule_hpp
#define _1D_RiemannRule_hpp

#include<functional>
#include<boost/optional.hpp>

namespace _1D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T>
class RiemannRule
{
  public:
    RiemannRule() = default;

    // This version will integrate a callable between two points
    template<typename F, typename X>
    T operator()( F f, X a, X b, size_t N ) const;

    // This version will integrate a set of discrete points
    template<typename X, typename Y>
    T operator()( X &x, Y &y ) const;

  protected:
};


template<typename T>
template<typename F, typename X>
T RiemannRule<T>::operator()( F f, X a, X b, size_t N ) const
{
  T sum = 0;
  X dx = (b-a)/N; // make sure we don't get integer rounding
  X x = a;
  for(int i = 0; i < N; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

template<typename T>
template<typename X, typename Y>
T RiemannRule<T>::operator()( X &x, Y &y ) const
{
  T sum = 0;
  for(int i = 0; i < x.size()-1; i++)
    sum += y[i]*(x[i+1]-x[i]);

  return sum;
}

}


#endif // include protector
