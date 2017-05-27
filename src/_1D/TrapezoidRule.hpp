#ifndef _1D_TrapezoidRule_hpp
#define _1D_TrapezoidRule_hpp

#include<functional>
#include<boost/optional.hpp>

namespace _1D {

/** @class 
  * @brief A class that implements the trapezoid rule.
  * @author C.D. Clark III
  */
template<typename T>
class TrapezoidRule
{
  public:
    TrapezoidRule (){};
    virtual ~TrapezoidRule (){};

    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T a, T b, size_t N );

    // This version will integrate a set of discrete points
    template<typename C>
    T operator()( C &x, C &y, boost::optional<T> a = boost::none, boost::optional<T> b = boost::none);

  protected:
};


/**
 * @param f function or functor to be integrated.
 * @param a lower limit of integration.
 * @param b upper limit of integration.
 * @param N number of *sub-intervals* to divide the integral [a,b] into.
 **/
template<typename T>
template<typename F>
T TrapezoidRule<T>::operator()( F f, T a, T b, size_t N )
{
  T sum = 0;
  T dx = (b-a)/N; // NOTE: N is the number of sub-intervals here
  T x;
  for(x = a; x < b; x += dx)
  {
    sum += f(x) + f(x + dx);
  }
  sum *= 0.5*dx;
  return sum;
}

template<typename T>
template<typename C>
T TrapezoidRule<T>::operator()( C &x, C &y, boost::optional<T> a, boost::optional<T> b )
{
  T sum = 0;
  if(!a)
    a = x[0];
  if(!b)
    b = x[x.size()-1];

  for(int i = 0; i < x.size()-1; i++)
    if(x[i] >= *a && x[i] <= *b)
      sum += (y[i+1]+y[i])*(x[i+1]-x[i]);
  sum *= 0.5;

  return sum;
}

}


#endif // include protector
