#ifndef _2D_GaussianQuadrature_hpp
#define _2D_GaussianQuadrature_hpp

/** @file GaussianQuadrature.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 08/03/17
  */

namespace _2D {

/** @class GaussianQuadrature
  * @brief 
  * @author C.D. Clark III
  */
template<typename T>
class GaussianQuadrature
{
  public:
    GaussianQuadrature(){};
    virtual ~GaussianQuadrature(){};

    // This version will integrate a callable between four points
    template<typename F, typename X>
    T operator()( F f, X xa, X xb, size_t xN, X ya, X yb, size_t yN );

  protected:
};

}

#endif // include protector
