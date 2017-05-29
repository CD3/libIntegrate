#include "catch.hpp"
#include "fakeit.hpp"

#include "_1D/RecursiveAdaptiveQuadrature.hpp"
#include "_1D/TrapezoidRule.hpp"

namespace  RecursiveAdaptiveQuadratureTests {

double linear_func(double x)
{
  return 2*x + 3;
}

double box_func(double x)
{
  if(x < 1)
    return 0;
  if(x <= 5)
    return 1;
  return 0;
}

TEST_CASE( "Testing adaptive quadrature with trapezoid rule on linear functions." ) {

  _1D::RecursiveAdaptiveQuadrature<_1D::TrapezoidRule,double> integrate;
  double I;

  I = integrate( linear_func, 2., 5. );
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return 2*x + 3;},2.,5.);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return linear_func(x);},2.,5.);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

}

// need to fix the adaptive algorithm
//TEST_CASE( "Testing adaptive quadrature with trapezoid rule on box functions." ) {

  //_1D::RecursiveAdaptiveQuadrature<_1D::TrapezoidRule,double> integrate;
  //double I;

  //I = integrate( box_func, 0, 10 );
  //REQUIRE( I == Approx( 5-1 ).epsilon(1e-5) );

//}

}
