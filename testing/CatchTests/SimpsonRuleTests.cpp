#include <iostream>
#include <numeric>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/_1D/SimpsonRule.hpp>
using namespace Catch;

namespace SimpsonRuleTests
{
double linear_func(double x) { return 2 * x + 3; }

double box_func(double x)
{
  if(x < 1) return 0;
  if(x <= 5) return 1;
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

    for(std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }

  SECTION("4 point data set")
  {
    // Simpson's rule will use three points at a time. If the data sent in is
    // not odd, the last piece needs to be handed correctly
    std::vector<double> x(4), f(4);
    x[0] = 1;
    x[1] = 3;
    x[2] = 8;
    x[3] = 10;

    for(std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }

  SECTION("5 point data set")
  {
    std::vector<double> x(5), f(5);
    x[0] = 1;
    x[1] = 3;
    x[2] = 8;
    x[3] = 9;
    x[4] = 10;

    for(std::size_t i = 0; i < x.size(); ++i)
      f[i] = x[i] * x[i] + 2 * x[i] + 3;

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));

    I = integrate(x, f, 0, 2);
    CHECK(I == Approx(8 * 8 * 8 / 3. + 8 * 8 + 3 * 8 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));

    I = integrate(x, f, 2, 4);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 8 * 8 * 8 / 3. -
                      8 * 8 - 3 * 8));

    I = integrate(x, f, 1, -2);
    CHECK(I == Approx(9 * 9 * 9 / 3. + 9 * 9 + 3 * 9 - 3 * 3 * 3 / 3. -
                      3 * 3 - 3 * 3));
  }

  SECTION("Single-vector dataset")
  {
    SECTION("5 point data set")
    {
      std::vector<double> f(5);
      for(std::size_t i = 0; i < f.size(); ++i)
        f[i] = i * i + 2 * i + 3;

      double I;
      I = integrate(f);
      CHECK(I == Approx(4. * 4. * 4. / 3. + 2. * 4. * 4. / 2. + 3. * 4.));
    }

    SECTION("6 point data set")
    {
      std::vector<double> f(6);
      for(std::size_t i = 0; i < f.size(); ++i)
        f[i] = i * i + 2 * i + 3;

      double I;
      I = integrate(f);
      CHECK(I == Approx(5. * 5. * 5. / 3. + 2. * 5. * 5. / 2. + 3. * 5.));
      I = integrate(f, 0.1);
      CHECK(I == Approx(0.1 * (5. * 5. * 5. / 3. + 2. * 5. * 5. / 2. + 3. * 5.)));
      I = integrate(f, -0.1);
      CHECK(I == Approx(-0.1 * (5. * 5. * 5. / 3. + 2. * 5. * 5. / 2. + 3. * 5.)));
    }
  }

  SECTION("Lambda function transform")
  {
    std::vector<double> x(3);
    x[0] = 1;
    x[1] = 3;
    x[2] = 10;

    auto f = [&x](int i) { return x[i] * x[i] + 2 * x[i] + 3; };

    I = integrate(x, f);
    CHECK(I == Approx(10 * 10 * 10 / 3. + 10 * 10 + 3 * 10 - 1 * 1 * 1 / 3. -
                      1 * 1 - 3 * 1));
  }
}

TEST_CASE("Simpson rule benchmarks.", "[.][benchmarks]")
{
  _1D::SimpsonRule<double> integrate;
  SECTION("Even vs. Odd Datasets")
  {
    std::vector<double> f1(5), f2(6), f3(201), f4(202);
    std::iota(f1.begin(), f1.end(), 0);
    std::iota(f2.begin(), f2.end(), 0);
    std::iota(f3.begin(), f3.end(), 0);
    std::iota(f4.begin(), f4.end(), 0);

    BENCHMARK("5 Point")
    {
      return integrate(f1);
    };
    BENCHMARK("6 Point")
    {
      return integrate(f2);
    };
    BENCHMARK("201 Point")
    {
      return integrate(f3);
    };
    BENCHMARK("202 Point")
    {
      return integrate(f4);
    };
  }
  SECTION("Uniform vs. Non-Uniform Methods")
  {
    std::vector<double> x1(5), x2(6), x3(201), x4(202);
    std::vector<double> f1(5), f2(6), f3(201), f4(202);
    std::iota(x1.begin(), x1.end(), 0);
    std::iota(x2.begin(), x2.end(), 0);
    std::iota(x3.begin(), x3.end(), 0);
    std::iota(x4.begin(), x4.end(), 0);

    for(std::size_t i = 0; i < x1.size(); i++)
      f1[i] = 0.1 * x1[i];
    for(std::size_t i = 0; i < x2.size(); i++)
      f2[i] = 0.1 * x2[i];
    for(std::size_t i = 0; i < x3.size(); i++)
      f3[i] = 0.1 * x3[i];
    for(std::size_t i = 0; i < x4.size(); i++)
      f4[i] = 0.1 * x4[i];

    BENCHMARK("5 Point Non-Uniform")
    {
      return integrate(x1, f1);
    };
    BENCHMARK("5 Point Uniform")
    {
      return integrate(f1);
    };

    BENCHMARK("6 Point Non-Uniform")
    {
      return integrate(x2, f2);
    };
    BENCHMARK("6 Point Uniform")
    {
      return integrate(f2);
    };

    BENCHMARK("201 Point Non-Uniform")
    {
      return integrate(x3, f3);
    };
    BENCHMARK("201 Point Uniform")
    {
      return integrate(f3);
    };

    BENCHMARK("202 Point Non-Uniform")
    {
      return integrate(x4, f4);
    };
    BENCHMARK("202 Point Uniform")
    {
      return integrate(x4, f4);
    };
  }
}

}  // namespace SimpsonRuleTests
