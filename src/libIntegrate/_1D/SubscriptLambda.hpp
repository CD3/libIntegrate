#pragma once

namespace _1D
{
/**
 * A wrapper class for calling a lambda function using the subscript operator[]
 * instead of the call operator().
 */
template<class Lambda>
class SubscriptLambdaObj
{
  Lambda f;

 public:
  SubscriptLambdaObj(Lambda &&_f) : f(std::forward<Lambda>(_f)) {}
  auto operator[](int i) -> decltype(f(0)) { return f(i); }
};

template<class Lambda>
SubscriptLambdaObj<Lambda> SubscriptLambda(Lambda &&_f)
{
  return {std::forward<Lambda>(_f)};
}

}  // namespace _1D
