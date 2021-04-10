#pragma once

#include<utility>

namespace _2D
{
/**
 * A wrapper class for associating a lambda function with a size(int) function
 * so that it can be used like a 2D random access container.
 */
template<class ElementLambda, class SizeLambda>
class RandomAccessLambdaObj
{
  ElementLambda m_ElementLambda;
  SizeLambda m_SizeLambda;

 public:
  RandomAccessLambdaObj(ElementLambda &&e, SizeLambda &&s) : m_ElementLambda(std::forward<ElementLambda>(e)),m_SizeLambda(std::forward<SizeLambda>(s)) {}
  template<typename I, typename J>
  auto operator()(I i, J j) const -> decltype(m_ElementLambda(i,j)) { return m_ElementLambda(i,j); }
  auto size(int i) const -> decltype(m_SizeLambda(i)) { return m_SizeLambda(i); }
};

template<class ElementLambda, class SizeLambda>
RandomAccessLambdaObj<ElementLambda,SizeLambda> RandomAccessLambda(ElementLambda &&e, SizeLambda &&s)
{
  return {std::forward<ElementLambda>(e),std::forward<SizeLambda>(s)};
}

}  // namespace _2D
