#pragma once

/** @file GaussLegendre.hpp
  * @brief 
  * @author C.D. Clark III
  * @date 08/04/17
  */

#include<cstddef>
#include<array>

namespace _1D {
namespace GQ {


namespace detail {
template<class Derived>
class GaussLegendreQuadrature_imp
{
  public:
    GaussLegendreQuadrature_imp() = default;

    template<typename T>
    struct GetType{ };
    template<template <typename,std::size_t> class Class, typename T, std::size_t Order>
    struct GetType<Class<T,Order>> { using type = T; };

    template<typename T>
    struct GetOrder{ };
    template<template <typename,std::size_t> class Class, typename T, std::size_t Order>
    struct GetOrder<Class<T,Order>> { static const std::size_t value = Order; };

    using DataType = typename GetType<Derived>::type;

    // This version will integrate a callable between two points
    template<typename F>
    DataType operator()( F f, DataType a, DataType b ) const
    {
      static auto x = Derived::getX();
      static auto w = Derived::getW();

      using T = typename GetType<Derived>::type;

      T apb = static_cast<T>(b + a)/2;
      T amb = static_cast<T>(b - a)/2;

      typename GetType<Derived>::type sum = 0;
      for(std::size_t i = 0; i < GetOrder<Derived>::value; i++)
        sum += w[i]*f( apb + amb*x[i] );
      sum *= amb;

      return sum;
    }

};
}

/** @class GaussLegendreQuadrature
  * @brief 
  * @author C.D. Clark III
  *
  * This is an empty class that should be specialized for different orders of integration.
  * The specializations should derive from the detail::GaussLegendreQuadrature_imp and
  * passes themselves as template parameter (CRTP). detail::GaussLegendreQuadrature_imp
  * expects there to be a getX and getW function that return std::arrays<T,Order> references
  * to the weight and abscissa. The function should create and return static arrays so that
  * these arrays are only created once for each order.
  */
template<typename T, std::size_t Order>
class GaussLegendreQuadrature
{
};

template<typename T>
class GaussLegendreQuadrature<T,8> : public detail::GaussLegendreQuadrature_imp<GaussLegendreQuadrature<T,8>>
{

  public:
    // these weights and abscissa were taken from https://pomax.github.io/bezierinfo/legendre-gauss.html#n8
    static const std::array<T,8>& getW() {
      static std::array<T,8> w = {
       0.3626837833783620
      ,0.3626837833783620
      ,0.3137066458778873
      ,0.3137066458778873
      ,0.2223810344533745
      ,0.2223810344533745
      ,0.1012285362903763
      ,0.1012285362903763
      };
      return w;
    }
    static const std::array<T,8>& getX() {
      static std::array<T,8> x = {
       -0.1834346424956498
      , 0.1834346424956498
      ,-0.5255324099163290
      , 0.5255324099163290
      ,-0.7966664774136267
      , 0.7966664774136267
      ,-0.9602898564975363
      , 0.9602898564975363
      };
      return x;
    }

};


template<typename T>
class GaussLegendreQuadrature<T,16> : public detail::GaussLegendreQuadrature_imp<GaussLegendreQuadrature<T,16>>
{

  public:
    // these weights and abscissa were taken from https://pomax.github.io/bezierinfo/legendre-gauss.html#n16
    static const std::array<T,16>& getW() {
      static std::array<T,16> w = {
       0.1894506104550685
      ,0.1894506104550685
      ,0.1826034150449236
      ,0.1826034150449236
      ,0.1691565193950025
      ,0.1691565193950025
      ,0.1495959888165767
      ,0.1495959888165767
      ,0.1246289712555339
      ,0.1246289712555339
      ,0.0951585116824928
      ,0.0951585116824928
      ,0.0622535239386479
      ,0.0622535239386479
      ,0.0271524594117541
      ,0.0271524594117541
      };
      return w;
    }
    static const std::array<T,16>& getX() {
      static std::array<T,16> x = {
       -0.0950125098376374
      , 0.0950125098376374
      ,-0.2816035507792589
      , 0.2816035507792589
      ,-0.4580167776572274
      , 0.4580167776572274
      ,-0.6178762444026438
      , 0.6178762444026438
      ,-0.7554044083550030
      , 0.7554044083550030
      ,-0.8656312023878318
      , 0.8656312023878318
      ,-0.9445750230732326
      , 0.9445750230732326
      ,-0.9894009349916499
      , 0.9894009349916499
      };
      return x;
    }

};

template<typename T>
class GaussLegendreQuadrature<T,32> : public detail::GaussLegendreQuadrature_imp<GaussLegendreQuadrature<T,32>>
{

  public:
      // these weights and abscissa were taken from https://pomax.github.io/bezierinfo/legendre-gauss.html#n32
    static const std::array<T,32>& getW() {
      static std::array<T,32> w = {
       0.0965400885147278
      ,0.0965400885147278
      ,0.0956387200792749
      ,0.0956387200792749
      ,0.0938443990808046
      ,0.0938443990808046
      ,0.0911738786957639
      ,0.0911738786957639
      ,0.0876520930044038
      ,0.0876520930044038
      ,0.0833119242269467
      ,0.0833119242269467
      ,0.0781938957870703
      ,0.0781938957870703
      ,0.0723457941088485
      ,0.0723457941088485
      ,0.0658222227763618
      ,0.0658222227763618
      ,0.0586840934785355
      ,0.0586840934785355
      ,0.0509980592623762
      ,0.0509980592623762
      ,0.0428358980222267
      ,0.0428358980222267
      ,0.0342738629130214
      ,0.0342738629130214
      ,0.0253920653092621
      ,0.0253920653092621
      ,0.0162743947309057
      ,0.0162743947309057
      ,0.0070186100094701
      ,0.0070186100094701
      };
      return w;
    }
    static const std::array<T,32>& getX() {
      static std::array<T,32> x = {
       -0.0483076656877383
      , 0.0483076656877383
      ,-0.1444719615827965
      , 0.1444719615827965
      ,-0.2392873622521371
      , 0.2392873622521371
      ,-0.3318686022821277
      , 0.3318686022821277
      ,-0.4213512761306353
      , 0.4213512761306353
      ,-0.5068999089322294
      , 0.5068999089322294
      ,-0.5877157572407623
      , 0.5877157572407623
      ,-0.6630442669302152
      , 0.6630442669302152
      ,-0.7321821187402897
      , 0.7321821187402897
      ,-0.7944837959679424
      , 0.7944837959679424
      ,-0.8493676137325700
      , 0.8493676137325700
      ,-0.8963211557660521
      , 0.8963211557660521
      ,-0.9349060759377397
      , 0.9349060759377397
      ,-0.9647622555875064
      , 0.9647622555875064
      ,-0.9856115115452684
      , 0.9856115115452684
      ,-0.9972638618494816
      , 0.9972638618494816
      };
      return x;
    }

};

template<typename T>
class GaussLegendreQuadrature<T,64> : public detail::GaussLegendreQuadrature_imp<GaussLegendreQuadrature<T,64>>
{

  public:
      // these weights and abscissa were taken from https://pomax.github.io/bezierinfo/legendre-gauss.html#n64
    static const std::array<T,64>& getW() {
      static std::array<T,64> w = {
       0.0486909570091397
      ,0.0486909570091397
      ,0.0485754674415034
      ,0.0485754674415034
      ,0.0483447622348030
      ,0.0483447622348030
      ,0.0479993885964583
      ,0.0479993885964583
      ,0.0475401657148303
      ,0.0475401657148303
      ,0.0469681828162100
      ,0.0469681828162100
      ,0.0462847965813144
      ,0.0462847965813144
      ,0.0454916279274181
      ,0.0454916279274181
      ,0.0445905581637566
      ,0.0445905581637566
      ,0.0435837245293235
      ,0.0435837245293235
      ,0.0424735151236536
      ,0.0424735151236536
      ,0.0412625632426235
      ,0.0412625632426235
      ,0.0399537411327203
      ,0.0399537411327203
      ,0.0385501531786156
      ,0.0385501531786156
      ,0.0370551285402400
      ,0.0370551285402400
      ,0.0354722132568824
      ,0.0354722132568824
      ,0.0338051618371416
      ,0.0338051618371416
      ,0.0320579283548516
      ,0.0320579283548516
      ,0.0302346570724025
      ,0.0302346570724025
      ,0.0283396726142595
      ,0.0283396726142595
      ,0.0263774697150547
      ,0.0263774697150547
      ,0.0243527025687109
      ,0.0243527025687109
      ,0.0222701738083833
      ,0.0222701738083833
      ,0.0201348231535302
      ,0.0201348231535302
      ,0.0179517157756973
      ,0.0179517157756973
      ,0.0157260304760247
      ,0.0157260304760247
      ,0.0134630478967186
      ,0.0134630478967186
      ,0.0111681394601311
      ,0.0111681394601311
      ,0.0088467598263639
      ,0.0088467598263639
      ,0.0065044579689784
      ,0.0065044579689784
      ,0.0041470332605625
      ,0.0041470332605625
      ,0.0017832807216964
      ,0.0017832807216964
      };
      return w;
    }
    static const std::array<T,64>& getX() {
      static std::array<T,64> x = {
       -0.0243502926634244
      , 0.0243502926634244
      ,-0.0729931217877990
      , 0.0729931217877990
      ,-0.1214628192961206
      , 0.1214628192961206
      ,-0.1696444204239928
      , 0.1696444204239928
      ,-0.2174236437400071
      , 0.2174236437400071
      ,-0.2646871622087674
      , 0.2646871622087674
      ,-0.3113228719902110
      , 0.3113228719902110
      ,-0.3572201583376681
      , 0.3572201583376681
      ,-0.4022701579639916
      , 0.4022701579639916
      ,-0.4463660172534641
      , 0.4463660172534641
      ,-0.4894031457070530
      , 0.4894031457070530
      ,-0.5312794640198946
      , 0.5312794640198946
      ,-0.5718956462026340
      , 0.5718956462026340
      ,-0.6111553551723933
      , 0.6111553551723933
      ,-0.6489654712546573
      , 0.6489654712546573
      ,-0.6852363130542333
      , 0.6852363130542333
      ,-0.7198818501716109
      , 0.7198818501716109
      ,-0.7528199072605319
      , 0.7528199072605319
      ,-0.7839723589433414
      , 0.7839723589433414
      ,-0.8132653151227975
      , 0.8132653151227975
      ,-0.8406292962525803
      , 0.8406292962525803
      ,-0.8659993981540928
      , 0.8659993981540928
      ,-0.8893154459951141
      , 0.8893154459951141
      ,-0.9105221370785028
      , 0.9105221370785028
      ,-0.9295691721319396
      , 0.9295691721319396
      ,-0.9464113748584028
      , 0.9464113748584028
      ,-0.9610087996520538
      , 0.9610087996520538
      ,-0.9733268277899110
      , 0.9733268277899110
      ,-0.9833362538846260
      , 0.9833362538846260
      ,-0.9910133714767443
      , 0.9910133714767443
      ,-0.9963401167719553
      , 0.9963401167719553
      ,-0.9993050417357722
      , 0.9993050417357722
      };
      return x;
    }

};

}
}

