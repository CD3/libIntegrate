#include "catch.hpp"

#include <libIntegrate/_1D/SimpsonRule.hpp>

namespace SimpsonRuleTests
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

TEST_CASE( "Testing Simpson rule on linear functions." ) {

  _1D::SimpsonRule<double> integrate;
  double I;

  I = integrate( linear_func, 2., 5., 1 );
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate( linear_func, 2., 5., 2 );
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return 2*x + 3;},2.,5.,2);
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return linear_func(x);},2.,5.,2);
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

}

TEST_CASE( "Testing Simpson rule on quadratic functions." ) {

  _1D::SimpsonRule<double> integrate;
  double I;

  I = integrate([](double x){return x*x + 2*x + 3;},1.,10.,1);
  CHECK( I == Approx( 10*10*10/3. + 10*10 + 3*10 - 1*1*1/3. - 1*1 - 3*1 ) );

  I = integrate([](double x){return x*x + 2*x + 3;},1.,10.,2);
  CHECK( I == Approx( 10*10*10/3. + 10*10 + 3*10 - 1*1*1/3. - 1*1 - 3*1 ) );

}


}
