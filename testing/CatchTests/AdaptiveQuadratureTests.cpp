#include "catch.hpp"
#include "fakeit.hpp"

#include "_1D/AdaptiveQuadrature.hpp"
#include "_1D/RiemannRule.hpp"
#include "_1D/TrapezoidRule.hpp"
#include "_1D/SimpsonRule.hpp"

namespace  AdaptiveQuadratureTests {

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

  _1D::AdaptiveQuadrature<_1D::TrapezoidRule,double> integrate;
  double I;

  I = integrate( linear_func, 2, 5 );
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return 2*x + 3;},2,5);
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

  I = integrate([](double x){return linear_func(x);},2,5);
  CHECK( I == Approx( 5*5+5*3 - 2*2 - 2*3 ) );

}

TEST_CASE( "Testing adaptive quadrature with trapezoid rule on box functions." ) {

  _1D::AdaptiveQuadrature<_1D::TrapezoidRule,double> integrate;
  double I;

  I = integrate( box_func, 0, 10 );
  CHECK( I == Approx( 5 ) ); // this function tricks our algorithm

  // we need to break the interval up into smaller pieces
  I = integrate( box_func, 0, 10, 4 );
  CHECK( I == Approx( 5-1 ).epsilon(1e-5) );

}

TEST_CASE( "Testing adaptive quadrature with trapezoid rule on sin functions.") {

  _1D::AdaptiveQuadrature<_1D::TrapezoidRule,double> integrate;
  double I;

#ifdef TESTING
  integrate.num_calls = 0;
#endif
  I = integrate( sin, 0, M_PI/2 );
  CHECK( I == Approx( 1 ).epsilon(0.01) );
#ifdef TESTING
  std::cout << "integrate.num_calls: " << integrate.num_calls << std::endl;
#endif

#ifdef TESTING
  integrate.num_calls = 0;
#endif
  I = integrate( sin, 0, M_PI/2, 3 );
  CHECK( I == Approx( 1 ).epsilon(0.01) );
#ifdef TESTING
  std::cout << "integrate.num_calls: " << integrate.num_calls << std::endl;
#endif

#ifdef TESTING
  integrate.num_calls = 0;
#endif
  I = integrate( sin, 0, M_PI/2, 5 );
  CHECK( I == Approx( 1 ).epsilon(0.001) );
#ifdef TESTING
  std::cout << "sin, trapezoid (5), integrate.num_calls: " << integrate.num_calls << std::endl;
#endif


}

TEST_CASE( "Testing adaptive quadrature with simpson's rule on sin functions.") {

  _1D::AdaptiveQuadrature<_1D::SimpsonRule,double> integrate;
  double I;

#ifdef TESTING
  integrate.num_calls = 0;
#endif
  I = integrate( sin, 0, M_PI/2, 3 );
  CHECK( I == Approx( 1 ).epsilon(0.001) );
#ifdef TESTING
  std::cout << "sin, simpson(3), integrate.num_calls: " << integrate.num_calls << std::endl;
#endif


}

}
