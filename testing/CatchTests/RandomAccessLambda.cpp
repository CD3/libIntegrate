#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <vector>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/_1D/RandomAccessLambda.hpp>
#include <libIntegrate/_1D/TrapezoidRule.hpp>
#include <libIntegrate/_2D/RandomAccessLambda.hpp>
using namespace Catch;

TEST_CASE("1D RandomAccessLambda Usage")
{
  SECTION("wrapper on std::vector")
  {
    std::vector<double> v(10);
    auto                f = _1D::RandomAccessLambda([&v](int i) { return v[i]; }, [&v]() { return v.size(); });
    std::iota(v.begin(), v.end(), 10);
    CHECK(f.size() == 10);
    CHECK(f(0) == 10);
    CHECK(f(1) == 11);
    CHECK(f(9) == 19);
  }

  SECTION("zero-memory (well, almost) container")
  {
    auto f = _1D::RandomAccessLambda([](int i) { return 0.1 * i; }, []() { return 100; });
    CHECK(f.size() == 100);
    CHECK(f(0) == Approx(0).scale(1));
    CHECK(f(1) == Approx(0.1));
    CHECK(f(99) == Approx(9.9));
  }
}

TEST_CASE("2D RandomAccessLambda Usage")
{
  SECTION("wrapper on std::vector")
  {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 10);
    auto f = _2D::RandomAccessLambda([&v](int i, int j) { return v[i * 2 + j]; }, [&v](int i) { return i == 0 ? 5 : 2; });
    CHECK(f.size(0) == 5);
    CHECK(f.size(1) == 2);
    CHECK(f(0, 0) == 10);
    CHECK(f(0, 1) == 11);
    CHECK(f(1, 0) == 12);
    CHECK(f(4, 1) == 19);
  }
}

TEST_CASE("RandomAcessLambda Benchmarks", "[.][benchmarks]")
{
  SECTION("Raw vector vs. wrapped vector integration speed comparison")
  {
    std::vector<double> x(100), y(100);
    std::iota(x.begin(), x.end(), 0);
    std::transform(x.begin(), x.end(), y.begin(), [](double x) { return x * x; });

    _1D::TrapezoidRule<double> integrate;

    BENCHMARK("Raw vectors")
    {
      return integrate(x, y);
    };
    BENCHMARK("x vector wrapped with RandomAccessLambda")
    {
      return integrate(_1D::RandomAccessLambda([&x](int i) { return x[i]; }, [&x]() { return x.size(); }), y);
    };
    BENCHMARK("x and y vectors wrapped with RandomAccessLambda")
    {
      return integrate(_1D::RandomAccessLambda([&x](int i) { return x[i]; }, [&x]() { return x.size(); }), _1D::RandomAccessLambda([&y](int i) { return y[i]; }, [&y]() { return y.size(); }));
    };
  }
}
