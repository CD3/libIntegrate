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
  std::size_t m_size;

 public:
  SubscriptLambdaObj(Lambda &&_f, std::size_t s = 0) : f(std::forward<Lambda>(_f)),m_size(s) {}
  auto operator[](int i) const -> decltype(f(0)) { return f(i); }

  std::size_t size() const {return m_size;}
  void size(std::size_t s) {m_size = s;}

};

template<class Lambda>
SubscriptLambdaObj<Lambda> SubscriptLambda(Lambda &&_f, std::size_t _s = 0)
{
  return {std::forward<Lambda>(_f),_s};
}

}  // namespace _1D
