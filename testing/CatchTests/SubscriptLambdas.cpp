#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

#include <vector>
#include <cmath>
#include <functional>
#include <libIntegrate/_1D/SubscriptLambda.hpp>
#include <libIntegrate/_1D/SubscriptFunction.hpp>
#include <libIntegrate/_1D/RiemannRule.hpp>




TEST_CASE("Performance testing for using lambdas with discrete functions", "[.][benchmarks]")
{
  SECTION("SubscriptFunction class")
  {
    _1D::SubscriptFunction<int(int)> f([](int i) { return i; });

    CHECK(f[1] == 1);
    CHECK(f[2] == 2);

    f = [](int i) { return 2 * i; };

    CHECK(f[1] == 2);
    CHECK(f[2] == 4);

    SECTION("Benchmarks", "[.][benchmarks]")
    {
      BENCHMARK("SubscriptFunction")
      {
        return f[1];
      };
      BENCHMARK("Lambda Expression")
      {
        return [](int i) { return 2*i; }(1);
      };
      auto ff = [](int i) { return 2*i; };
      BENCHMARK("Lambda Object")
      {
        return ff(1);
      };
    }
  }

  SECTION("SubscriptLambda")
  {
    double x = 2.0;
    auto f = _1D::SubscriptLambda([&x](int i){ return i*x;});
    CHECK( f[0] == 2.0*0 );
    CHECK( f[2] == 2.0*2 );

    SECTION("Benchmarks", "[benchmarks]")
    {
      BENCHMARK("SubscriptLambda")
      {
        return f[2];
      };
      BENCHMARK("Lambda Expression")
      {
        return [&x](int i) { return i*x; }(2);
      };
      auto ff = [&x](int i) { return i*x; };
      BENCHMARK("Lambda Object")
      {
        return ff(2);
      };
    }
  }


  SECTION("Riemann Sum w/ SubscriptLambda")
  {
    int N = 100;
    std::vector<double> x(N),f(N);
    for(int i = 0; i < N; i++)
    {
      x[i] = 0.1*i;
      f[i] = std::sin(x[i]);
    }

    _1D::RiemannRule<double> integrate;

    BENCHMARK("Integrate Vector Directly")
    {
      return integrate(x,f);
    };
    BENCHMARK("Integrate Vector with SubscriptLambda")
    {
      auto ff = _1D::SubscriptLambda([&f](int i){return f[i];});
      return integrate(x, ff);
    };

    SECTION("Integral with weight function")
    {
      BENCHMARK("using temporary vector")
      {
        std::vector<double> ff(N);
        for(int i = 0; i < N; i++)
        {
          ff[i] = x[i]*f[i];
        }
        return integrate(x, ff);
      };

      BENCHMARK("using SubscriptLambda")
      {
        auto ff = _1D::SubscriptLambda( [&x, &f](int i){return x[i]*f[i];} );
        return integrate(x, ff);
      };


    }

  }


  
}
