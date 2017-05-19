#ifndef _1D_RiemannRule_hpp
#define _1D_RiemannRule_hpp

#include<functional>

namespace _1D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T>
class RiemannRule
{
  public:
    RiemannRule (){};
    virtual ~RiemannRule (){};

    template<typename F>
    T operator()( F f, T a, T b, size_t N );

  protected:
};


template<typename T>
template<typename F>
T RiemannRule<T>::operator()( F f, T a, T b, size_t N )
{
  T sum = 0;
  T dx = (b-a)/N;
  T x = a;
  for(int i = 0; i < N; i++)
  {
    sum += f(x);
    x += dx;
  }
  sum *= dx;
  return sum;
}

}


#endif // include protector
