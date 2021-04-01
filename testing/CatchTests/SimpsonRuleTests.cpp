#include "catch.hpp"

#include<iostream>
#include <libIntegrate/_1D/SimpsonRule.hpp>

namespace SimpsonRuleTests
{
double linear_func(double x) { return 2 * x + 3; }

double box_func(double x)
{
  if (x < 1) return 0;
  if (x <= 5) return 1;
  return 0;
}

TEST_CASE("Testing Simpson rule on linear functions.")
{
  _1D::SimpsonRule<double> integrate;
  double                   I;

  I = integrate(linear_func, 2., 5., 1);
  CHECK(I == Approx(5 * 5 + 5 * 3 - 2 * 2 - 2 * 3));

  I = integrate(linear_func, 2., 5., 2);
  CHECK(I == Approx(5 * 5 + 5 * 3 - 2 * 2 - 2 * 3));

  I = integrate([](double x) { return 2 * x + 3; }, 2., 5., 2);
  CHECK(I == Approx(5 * 5 + 5 * 3 - 2 * 2 - 2 * 3));

  I = integrate([](double x) { return linear_func(x); }, 2., 5., 2);
  CHECK(I == Approx(5 * 5 + 5 * 3 - 2 * 2 - 2 * 3));
}

TEST_CASE("Testing Simpson rule on quadratic functions.")
{
  _1D::SimpsonRule<double> integrate;
  double                   I;

  I = integrate([](double x) { return x * x + 2 * x + 3; }, 1., 10., 1);
  CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                    1 * 1 - 3 * 1));

  I = integrate([](double x) { return x * x + 2 * x + 3; }, 1., 10., 2);
  CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                    1 * 1 - 3 * 1));
}

TEST_CASE("Simpson rule on discretized functions.")
{
  _1D::SimpsonRule<double> integrate;
  double                   I;

  SECTION("Simple 3 point data set")
  {
    std::vector<double> x(3), f(3);
    x[0] = 1;
    x[1] = 3;
    x[2] = 10;

    for (std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }

  SECTION("4 point data set")
  {
    // Simpson's rule will use three points at a time. If the data sent in is
    // not a multiple of three, the last piece needs to be handed correctly
    std::vector<double> x(4), f(4);
    x[0] = 1;
    x[1] = 3;
    x[2] = 8;
    x[3] = 10;

    for (std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }

  SECTION("5 point data set")
  {
    // Simpson's rule will use three points at a time. If the data sent in is
    // not a multiple of three, the last piece needs to be handed correctly
    std::vector<double> x(5), f(5);
    x[0] = 1;
    x[1] = 3;
    x[2] = 8;
    x[3] = 9;
    x[4] = 10;

    for (std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }
}

}  // namespace SimpsonRuleTests
