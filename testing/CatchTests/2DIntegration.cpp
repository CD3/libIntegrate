#include <cmath>
#include <iostream>
#include <numeric>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/_1D/RiemannRule.hpp>
#include <libIntegrate/_1D/SimpsonRule.hpp>
#include <libIntegrate/_1D/TrapezoidRule.hpp>
#include <libIntegrate/_2D/DiscretizedIntegratorWrapper.hpp>
#include <libIntegrate/_2D/RiemannRule.hpp>
#include <libIntegrate/_2D/SimpsonRule.hpp>
#include <libIntegrate/_2D/TrapezoidRule.hpp>
using namespace Catch;

TEST_CASE("Test the 2D discretized integration with non-rectangular limits")
{
  _1D::SimpsonRule<double> integrate;
  double                   I;

  double R  = 1;
  auto   f1 = [](double y) { return 1; };
  auto   f2 = [&f1, &R, &integrate](double y) -> double {
    auto Lim = sqrt(R * R - y * y);
    if(y * y > R * R) {
      return 0.0;
    }
    return integrate(f1, -Lim, Lim, 100);
  };

  I = integrate(f2, -R, R, 100);

  CHECK(I == Approx(M_PI * 1 * 1).epsilon(0.001));
}
