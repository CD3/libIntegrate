#ifndef _1D_TrapezoidRule_hpp
#define _1D_TrapezoidRule_hpp

#include<functional>

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

    template<typename F>
    T operator()( F f, T a, T b, size_t N );

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

}


#endif // include protector
