#include <cmath>
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

TEST_CASE("Test the 2D discretized integration wrapper")
{
  SECTION("Simple Riemann sum")
  {
    _2D::DiscretizedIntegratorWrapper<_1D::RiemannRule<double>> integrate;
    std::vector<double>                                         x(3);
    std::vector<double>                                         y(4);
    std::vector<std::vector<double>>                            f(3);
    std::iota(x.begin(), x.end(), 0);
    std::iota(y.begin(), y.end(), 0);
    for(std::size_t i = 0; i < x.size(); i++) {
      f[i]    = std::vector<double>(4);
      f[i][0] = 2 + i;
      f[i][1] = 3 + i;
      f[i][2] = 4 + i;
      f[i][3] = 5 + i;
    }

    // the Riemann rule does not use the last element in the function value array
    CHECK(integrate(x, y, f) == Approx(9 + 12));
  }

  SECTION("Sin(x)*Sin(x) integration")
  {
    double      xmin, ymin, xmax, ymax, dx, dy;
    std::size_t N = 100;
    xmin          = 0;
    ymin          = 0;
    xmax          = M_PI / 2;
    ymax          = M_PI / 2;
    dx            = (xmax - xmin) / (N - 1);
    dy            = (ymax - ymin) / (N - 1);

    std::vector<double>              x(N), y(N);
    std::vector<std::vector<double>> f(N);

    for(std::size_t i = 0; i < N; i++) {
      x[i] = dx * i;
      f[i] = std::vector<double>(N);
      for(std::size_t j = 0; j < N; j++) {
        y[j]    = dy * j;
        f[i][j] = sin(x[i]) * sin(y[j]);
      }
    }

    /*
     * f(x,y) = sin(x) sin(y)
     *   \int \int f(x,y) dx dy = \int \int sin(x) sin(y) dx dy
     * = \int sin(y) \int sin(x) dx dy * = \int sin(y) cos(x)|_0^M_PI/2 dy
     * = -1 \int sin(y) dy = 1
     */
    SECTION("Riemann")
    {
      _2D::RiemannRule<double> integrate;
      CHECK(integrate(x, y, f) < 1);
      CHECK(integrate(x, y, f) == Approx(1).epsilon(0.1));
      CHECK(integrate(x, y, f) != Approx(1).epsilon(0.01));
    }
    SECTION("Trapezoid")
    {
      _2D::TrapezoidRule<double> integrate;
      CHECK(integrate(x, y, f) < 1);
      CHECK(integrate(x, y, f) == Approx(1).epsilon(0.0001));
      CHECK(integrate(x, y, f) != Approx(1).epsilon(0.00001));
    }
    SECTION("Simpson")
    {
      _2D::SimpsonRule<double> integrate;
      CHECK(integrate(x, y, f) == Approx(1).epsilon(0.0000001));
    }
    SECTION("Simpson (Uniform)")
    {
      _2D::SimpsonRule<double> integrate;
      CHECK(integrate(f, dx, dy) == Approx(1).epsilon(0.0000001));
    }
  }
}
