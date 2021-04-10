#pragma once

#include<utility>

namespace _1D
{
/**
 * A wrapper class for associating a lambda function with a size() function
 * so that it can be used like a random access container.
 */
template<class ElementLambda, class SizeLambda>
class RandomAccessLambdaObj
{
  ElementLambda m_ElementLambda;
  SizeLambda m_SizeLambda;

 public:
  RandomAccessLambdaObj(ElementLambda &&e, SizeLambda &&s) : m_ElementLambda(std::forward<ElementLambda>(e)),m_SizeLambda(std::forward<SizeLambda>(s)) {}
  template<typename T>
  auto operator()(T i) const -> decltype(m_ElementLambda(i)) { return m_ElementLambda(i); }
  auto size() const -> decltype(m_SizeLambda()) { return m_SizeLambda(); }
};

template<class ElementLambda, class SizeLambda>
RandomAccessLambdaObj<ElementLambda,SizeLambda> RandomAccessLambda(ElementLambda &&e, SizeLambda &&s)
{
  return {std::forward<ElementLambda>(e),std::forward<SizeLambda>(s)};
}

}  // namespace _1D
