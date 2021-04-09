#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

#include <numeric>
#include <libIntegrate/_1D/TrapezoidRule.hpp>
#include <libIntegrate/_2D/TrapezoidRule.hpp>

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

  I = integrate( linear_func, 2., 5., 2 );
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return 2*x + 3;},2.,5.,2);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return linear_func(x);},2.,5.,2);
  REQUIRE( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

}

TEST_CASE( "Testing trapezoid rule on box functions." ) {

  _1D::TrapezoidRule<double> integrate;
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

TEST_CASE( "Testing Trapezoid rule on discrete set." ) {

  _1D::TrapezoidRule<double> integrate;
  double I;

  std::vector<double> x(3),y(3);
  x[0] = 0; y[0] = 1;
  x[1] = 0.5; y[1] = 1.5;
  x[2] = 2; y[2] = 3;

  SECTION("Two vector data")
  {
    I = integrate( x, y );
    REQUIRE( I == Approx( 4 ) );
  }

  SECTION("Single vector data")
  {
    y[0] = 1;
    y[1] = 2;
    y[2] = 3;
  
    I = integrate( y );
    REQUIRE( I == Approx( 4 ) );
    I = integrate( y, 0.5 );
    REQUIRE( I == Approx( 2 ) );
    I = integrate( y, -0.5 );
    REQUIRE( I == Approx( -2 ) );
  }

  SECTION("Lambda function tranform")
  {
    I = integrate( x, [](int i){return 0;} );
    REQUIRE( I == Approx( 0 ).scale(1) );
    I = integrate( x, [&x](int i){return x[i]+1;} );
    REQUIRE( I == Approx( 4 ) );

  }

}

TEST_CASE( "Testing 1D Trapezoid rule with static interval number." ) {

  _1D::TrapezoidRule<double,10> integrate;
  double I;

  auto f = [](double x){ return 2*x + 3; };
  I = integrate( f, 2, 5 );
  REQUIRE( I == Approx( 5*5 + 3*5 - 2*2 - 3*2 ) );


}

}


TEST_CASE( "2D Trapezoid Rule" ) {

  _2D::TrapezoidRule<double> integrate;
  double I;

  SECTION("integrating sin(x)sin(y)")
  {
    auto f  = [](double x, double y){ return sin(x)*sin(y); };

    I = integrate( f, 0, M_PI/2, 100, 0, M_PI/2, 100 );
    CHECK( I < 1 );
    CHECK( I == Approx(1).epsilon(0.01) );

  }


}




TEST_CASE("Trapezoid Rule Benchmarks","[.][benchmarks]")
{
  int N = 1000;
  std::vector<double> x(N),y(N);
  std::iota(x.begin(), x.end(), 0);
  std::transform(x.begin(), x.end(), y.begin(), [](double x){return 2*x + 1;});

  _1D::TrapezoidRule<double> integrate;

  BENCHMARK("1000 element vectors of double")
  {
    return integrate(x,y);
  };

  BENCHMARK("1000 element vectors of double 'by hand'")
  {
    double sum = 0;
    for(std::size_t i = 0; i < x.size()-1; ++i)
      sum += (x[i+1]-x[i])*(y[i+1]+y[i]);
    sum *= 0.5;
    return sum;
  };


}
