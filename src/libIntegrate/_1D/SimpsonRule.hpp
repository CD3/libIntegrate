#pragma once
#include <cstddef>
#include <type_traits>

namespace _1D
{
/** @class
 * @brief A class that implements Simposon's (1/3) rule.
 * @author C.D. Clark III
 */
template<typename T, std::size_t NN = 0>
class SimpsonRule
{
 public:
  SimpsonRule() = default;

  // This version will integrate a callable between two points
  template<typename F, std::size_t NN_ = NN,
           typename SFINAE = typename std::enable_if<(NN_ == 0)>::type>
  T operator()(F f, T a, T b, std::size_t N) const;

  template<typename F, std::size_t NN_ = NN,
           typename SFINAE = typename std::enable_if<(NN_ > 0)>::type>
  T operator()(F f, T a, T b) const;

  // This version will integrate a set of discrete points
  template<typename X, typename Y>
  T operator()(X &x, Y &y) const;

 protected:
  T LagrangePolynomial(T x, T A, T B, T C) const;
};

template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T SimpsonRule<T, NN>::operator()(F f, T a, T b, std::size_t N) const
{
  T sum = 0;
  T dx  = static_cast<T>(b - a) / N;  // size of each interval
  T x;
  for (x = a; x < b; x += dx) {
    sum += f(x) + 4 * f(x + dx / 2) + f(x + dx);
  }
  // note that 2*h = dx
  sum *= dx / 6;
  return sum;
}

template<typename T, std::size_t NN>
template<typename F, std::size_t NN_, typename SFINAE>
T SimpsonRule<T, NN>::operator()(F f, T a, T b) const
{
  T sum = 0;
  T dx  = static_cast<T>(b - a) / NN;  // size of each interval
  T x;
  for (x = a; x < b; x += dx) {
    sum += f(x) + 4 * f(x + dx / 2) + f(x + dx);
  }
  // note that 2*h = dx
  sum *= dx / 6;
  return sum;
}

template<typename T, std::size_t NN>
template<typename X, typename Y>
T SimpsonRule<T, NN>::operator()(X &x, Y &y) const
{
  T sum = 0;
  decltype(x.size()) i;
  for (i = 0; i < x.size() - 2; i += 2) {
    // Integrate segment using three points
    // \int_a^b f(x) dx = (b-a)/6 [ f(a) + 4*f(m) + f(b) ]
    // a = x[i]
    // b = x[i+2]
    // m = (a+b)/2
    //
    // f(a) = y[i]
    // f(b) = y[i+2]
    // and we need to interpolate f(m)
    // clang-format off
    T m = (x[i] + x[i+2])/2;
    T ym = y[i  ]*LagrangePolynomial(m, x[i+1], x[i+2], x[i  ])
         + y[i+1]*LagrangePolynomial(m, x[i  ], x[i+2], x[i+1])
         + y[i+2]*LagrangePolynomial(m, x[i  ], x[i+1], x[i+2]);

    sum += (x[i+2]-x[i])/6 * (y[i] + 4*ym + y[i+2]);
    // clang-format on
  }

  if( x.size() % 2 == 0) // note: this tests if the last *index* is odd. the size will be even.
  {
    // there is on extra point at the end we need to handle
    // we will use the last *three* points to fit the polynomial
    // but then integrate between the last *two* points.
    i = x.size()-3;
    T m = (x[i+1] + x[i+2])/2;
    T ym = y[i  ]*LagrangePolynomial(m, x[i+1], x[i+2], x[i  ])
         + y[i+1]*LagrangePolynomial(m, x[i  ], x[i+2], x[i+1])
         + y[i+2]*LagrangePolynomial(m, x[i  ], x[i+1], x[i+2]);

    sum += (x[i+2]-x[i+1])/6 * (y[i+1] + 4*ym + y[i+2]);
  }

  return sum;
}

template<typename T, std::size_t NN>
T SimpsonRule<T, NN>::LagrangePolynomial(T x, T A, T B, T C) const
{
  return (x - A) * (x - B) / (C - A) / (C - B);
}

}  // namespace _1D
