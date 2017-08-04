#include "catch.hpp"
#include "fakeit.hpp"

#include "_1D/GaussianQuadratures/GaussLegendre.hpp"

namespace GausssLegendreTests
{


TEST_CASE( "Testing GQ:64 rule on polynomials." ) {


  _1D::GQ::GaussLegendreQuadrature<double,64> integrate;


  for(int N = 0; N < 100; N++)
  {
    auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };
    CHECK( integrate( f, 0., 1. )  == Approx( fi(1)  - fi(0) ) );
    CHECK( integrate( f, 1., 10. ) == Approx( fi(10) - fi(1) ) );
    CHECK( integrate( f, 0., 100. ) == Approx( fi(100) - fi(0) ) );
  }


}


TEST_CASE( "Testing GQ:64 rule on trig functions." ) {


  _1D::GQ::GaussLegendreQuadrature<double,64> integrate;


  CHECK( integrate( sin, 0., M_PI )  == Approx( 2 ) );
  CHECK( integrate( sin, M_PI, 2*M_PI )  == Approx( -2 ) );
  CHECK( integrate( sin, M_PI/2, M_PI )  == Approx( 1 ) );

}


}
