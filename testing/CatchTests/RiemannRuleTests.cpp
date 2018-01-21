#include "catch.hpp"
#include "fakeit.hpp"

#include "_1D/RiemannRule.hpp"
#include "_2D/RiemannRule.hpp"


namespace RiemannRuleTests
{

double box_func(double x)
{
  if(x < 1)
    return 0;
  if(x <= 5)
    return 1;
  return 0;
}

TEST_CASE( "Testing 1D Riemann rule on linear functions." ) {

  _1D::RiemannRule<double> integrate;
  double I;

  auto f = [](double x){ return 2*x + 3; };
  I = integrate( f, 2., 5., 2 );
  REQUIRE( I == Approx( f(2)*1.5 + f(3.5)*1.5 ) );


}

TEST_CASE( "Testing 1D Riemann rule on box functions." ) {

  _1D::RiemannRule<double> integrate;
  double I;

  I = integrate( box_func, 0., 10., 1 );
  REQUIRE( I == Approx( 0 ) );

  // dx = 1
  I = integrate( box_func, 0., 10., 10 );
  REQUIRE( I == Approx( (5-1) + 1) );

  // dx = 0.5
  I = integrate( box_func, 0., 10., 20 );
  REQUIRE( I == Approx( (5-1) + 0.5) );

  // dx = 0.25
  I = integrate( box_func, 0., 10., 40 );
  REQUIRE( I == Approx( (5-1) + 0.25) );

}

TEST_CASE( "Testing 1D Riemann rule on discrete set." ) {

  _1D::RiemannRule<double> integrate;
  double I;

  std::vector<double> x(3),y(3);
  x[0] = 0; y[0] = 1;
  x[1] = 1; y[1] = 2;
  x[2] = 2; y[2] = 3;
  

  I = integrate( x, y );
  REQUIRE( I == Approx( 3 ) );

}

TEST_CASE( "Testing 2D Riemann rule on linear functions." ) {

  _2D::RiemannRule<double> integrate;
  double I;

  auto f  = [](double x, double y){ return 10*x + 20*y + 30;};
  auto fi = [](double x, double y){ return 10*x*x*y/2 + 20*y*y*x/2 + 30*x*y;};

  I = integrate( f, 2., 5., 1000, 3., 6., 1000 );
  REQUIRE( I == Approx( ( fi(5,6) - fi(5,3) ) - ( fi(2,6) - fi(2,3) ) ).epsilon(0.001) );


}

}
