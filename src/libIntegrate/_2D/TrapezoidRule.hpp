#pragma once

#include<cstddef>
#include "./DiscretizedIntegratorWrapper.hpp"
#include "../_1D/TrapezoidRule.hpp"

namespace _2D {

/** @class 
  * @brief A class that implements Trapezoid sums.
  * @author C.D. Clark III
  */
template<typename T>
class TrapezoidRule : public DiscretizedIntegratorWrapper<_1D::TrapezoidRule<T>>
{ 
  public:

    using BaseType = DiscretizedIntegratorWrapper<_1D::TrapezoidRule<T>>;
    using BaseType::operator();
};



}
