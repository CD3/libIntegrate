#ifndef Definite_TrapezoidRule_hpp
#define Definite_TrapezoidRule_hpp

#include<functional>

namespace Definite {

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


template<typename T>
template<typename F>
T TrapezoidRule<T>::operator()( F f, T a, T b, size_t N )
{
  if(N < 2)
    throw std::runtime_error("Cannot use trapezoid rule to evaluate integral with less than 2 points.");
  T sum = 0;
  T dx = (b-a)/(N-1);
  T x = a;
  for(int i = 0; i < N-1; i++)
  {
    sum += f(x) + f(x + dx);
    x += dx;
  }
  sum *= 0.5*dx;
  return sum;
}

}


#endif // include protector
