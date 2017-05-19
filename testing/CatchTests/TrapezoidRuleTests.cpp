#include "catch.hpp"
#include "fakeit.hpp"

#include "_1D/TrapezoidRule.hpp"

namespace TrapeziodRuleTests
{

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

TEST_CASE( "Testing trapezoid rule on linear functions." ) {

  _1D::TrapezoidRule<double> integrate;
  double I;

  I = integrate( linear_func, 2, 5, 2 );
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return 2*x + 3;},2,5,2);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return linear_func(x);},2,5,2);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

}

TEST_CASE( "Testing trapezoid rule on box functions." ) {

  _1D::TrapezoidRule<double> integrate;
  double I;

  I = integrate( box_func, 0, 10, 2 );
  REQUIRE( I == Approx( 0 ) );

  // dx = 1
  I = integrate( box_func, 0, 10, 11 );
  REQUIRE( I == Approx( (5-1) + 1) );

  // dx = 0.5
  I = integrate( box_func, 0, 10, 21 );
  REQUIRE( I == Approx( (5-1) + 0.5) );

  // dx = 0.25
  I = integrate( box_func, 0, 10, 41 );
  REQUIRE( I == Approx( (5-1) + 0.25) );

}

}
