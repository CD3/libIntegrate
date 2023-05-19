#include <cmath>
#include <limits>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/_1D/Boost/GaussKronrod.hpp>

using namespace Catch;

namespace GausssKronrodTests
{

TEST_CASE("Testing 1D GaussKronrod:8 rule on polynomials.")
{
  _1D::Boost::GaussKronrodQuadrature<double, 8> integrate;
  for(int N = 0; N < 8; N++) {
    auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };
    CHECK(integrate(f, 0., 1.) == Approx(fi(1) - fi(0)));
    CHECK(integrate(f, 1., 10.) == Approx(fi(10) - fi(1)));
    CHECK(integrate(f, 0., 100.) == Approx(fi(100) - fi(0)));
  }
}

}  // namespace GausssKronrodTests

TEST_CASE("Testing 1D GaussKronrod:8 on Gaussian function.")
{
  _1D::Boost::GaussKronrodQuadrature<double, 8> integrate;

  auto f = [](double x) { return exp(-x * x); };
  CHECK(integrate(f, -1., 1.) == Approx(sqrt(M_PI) * erf(1)));
  CHECK(integrate(f, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()) == Approx(sqrt(M_PI)));
}

TEST_CASE("Testing 1D GaussKronrod:128 on Blackbody Curve.")
{
  _1D::Boost::GaussKronrodQuadrature<double, 128> integrate;
  auto                                            f = [](double x) {
    double h = 6.62607004e-34;
    double c = 299792458;
    double k = 1.38064852e-23;
    double T = 5000;

    return 2 * h * c * c / x / x / x / x / x / (exp(h * c / x / k / T) - 1);
  };
  double T     = 5000;
  double sigma = 5.6704e-8;

  CHECK(integrate(f, 0., std::numeric_limits<double>::infinity()) == Approx(sigma * T * T * T * T / M_PI));
}
