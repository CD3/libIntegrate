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
    RiemannRule (){};
    virtual ~RiemannRule (){};

    // This version will integrate a callable between two points
    template<typename F>
    T operator()( F f, T a, T b, size_t N );

    // This version will integrate a set of discrete points
    template<typename C>
    T operator()( C &x, C &y, boost::optional<T> a = boost::none, boost::optional<T> b = boost::none);

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

template<typename T>
template<typename C>
T RiemannRule<T>::operator()( C &x, C &y, boost::optional<T> a, boost::optional<T> b )
{
  T sum = 0;
  if(!a)
    a = x[0];
  if(!b)
    b = x[x.size()-1];

  for(int i = 0; i < x.size()-1; i++)
    if(x[i] >= *a && x[i] <= *b)
      sum += y[i]*(x[i+1]-x[i]);

  return sum;
}

}


#endif // include protector
