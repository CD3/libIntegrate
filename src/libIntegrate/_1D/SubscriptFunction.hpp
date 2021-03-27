#pragma once
#include <functional>

namespace _1D
{
/* A wrapper to turn callable object into a subscript-able object.
 * This class just passes the argument to operator[] to the callable's
 * oeprator() method.
 */
template<typename Sig>
class SubscriptFunction
{
 private:
  std::function<Sig> f;

  template<typename T>
  struct getReturnType;
  template<typename R, typename A>
  struct getReturnType<R(A)> {
    using type = R;
  };
  template<typename T>
  struct getArgumentType;
  template<typename R, typename A>
  struct getArgumentType<R(A)> {
    using type = R;
  };

 public:
  typename getReturnType<Sig>::type operator[](
      typename getArgumentType<Sig>::type i)
  {
    return f(i);
  }

  template<typename F>
  SubscriptFunction(F _f) : f(_f)
  {
  }
};

}  // namespace _1D

