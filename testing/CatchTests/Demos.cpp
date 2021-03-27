#include "catch.hpp"

#include <libIntegrate/_1D/GaussianQuadratures/GaussLegendre.hpp>
#include <libIntegrate/_1D/RiemannRule.hpp>
#include <libIntegrate/_1D/SubscriptLambda.hpp>

// exponential probability distribution
double probability_density(double x)
{
  double mu = 2;
  return mu*exp(-mu*x);
}

TEST_CASE( "Demos" ) {

  SECTION("Case 1 w/ weight function")
  {
  _1D::GQ::GaussLegendreQuadrature<double,16> integrate;

  CHECK(integrate(probability_density, 0, 10) == Approx(1));
  CHECK(integrate([](double x){ return   x*probability_density(x);}, 0, 10) == Approx(1./2));
  CHECK(integrate([](double x){ return x*x*probability_density(x);}, 0, 10) == Approx(2./4));
  CHECK(integrate([](double x){ return (x-1./2)*(x-1./2)*probability_density(x);}, 0, 10) == Approx(1./4));
  }

  SECTION("Case 2 with weight function")
  {
    int N = 200;
    std::vector<double> x(N), rho(N);
    for(int i = 0; i < N; i++)
    {
      x[i] = i*10./(N-1);
      rho[i] = 2*exp(-2*x[i]);
    }

    _1D::RiemannRule<double> integrate;

    CHECK(integrate(x,rho) == Approx(1).epsilon(0.1));
    auto f1 = _1D::SubscriptLambda( [&x, &rho](int i){return x[i]*rho[i];} );
    CHECK(integrate(x,f1) == Approx(1./2).epsilon(0.01));
    auto f3 = _1D::SubscriptLambda( [&x, &rho](int i){return (x[i]-0.5)*(x[i]-0.5)*rho[i];} );
    CHECK(integrate(x,f3) == Approx(1./4).epsilon(0.1));

  }


}


