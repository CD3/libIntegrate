#pragma once

#include<cstddef>
#include "./DiscretizedIntegratorWrapper.hpp"
#include "../_1D/RiemannRule.hpp"

namespace _2D {

/** @class 
  * @brief A class that implements Riemann sums.
  * @author C.D. Clark III
  */
template<typename T>
class RiemannRule : public DiscretizedIntegratorWrapper<_1D::RiemannRule<T>>
{ 
  public:

    using BaseType = DiscretizedIntegratorWrapper<_1D::RiemannRule<T>>;
    using BaseType::operator();


};



}
