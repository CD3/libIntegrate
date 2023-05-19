#include <numeric>
#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/Utils.hpp>
#include <libIntegrate/_1D/Utils.hpp>
#include <libIntegrate/_2D/Utils.hpp>
using namespace Catch;

struct A {
  void method(int) const {}
};
struct B {
  void method(std::size_t) const {}
};
struct C {
  void method(double) const {}
};

template<typename T>
bool takeInt(const T& t)
{
  return takeInt(t, libIntegrate::priority<10>{});
}
template<typename T>
bool takeInt(const T& t, libIntegrate::priority<1>)
{
  return false;
}
template<typename T>
auto takeInt(const T& t, libIntegrate::priority<2>)
    -> decltype(t.method(libIntegrate::IntOnly{}), true)
{
  return true;
}

TEST_CASE("IntOnly Usage")
{
  using namespace libIntegrate;

  CHECK(takeInt(A()));
  CHECK(takeInt(B()));
  CHECK(!takeInt(C()));
}

TEST_CASE("1D getElment function")
{
  using namespace libIntegrate;
  SECTION("std::vector<int>")
  {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 1);
    CHECK(getElement(v, 0) == 1);
    CHECK(getElement(v, 9) == 10);
  }

  SECTION("lambda functions")
  {
    CHECK(getElement([](int i) { return 2 * i; }, 2) == 4);
    // CHECK( getElement([](double i){return 2*i;},2) == 4 ); compiler error
  }

  SECTION("Non-containers")
  {
    // getElement(0,0); error
  }
}

struct MyContainer {
  int length() const { return 3; }
};
TEST_CASE("1D getSize function")
{
  using namespace libIntegrate;
  SECTION("std::vector<int>")
  {
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 1);
    CHECK(getSize(v) == 10);
  }

  SECTION(".length() method")
  {
    CHECK(getSize(MyContainer{}) == 3);
  }
}

TEST_CASE("2D getElment function")
{
  using namespace libIntegrate;
  SECTION("std:vector of std::vector<double>")
  {
    std::vector<std::vector<double>> v(10);
    for(int i = 0; i < 10; i++) {
      v[i] = std::vector<double>(10);
      for(int j = 0; j < 10; j++) {
        v[i][j] = i + j + 1;
      }
    }

    CHECK(getElement(v, 0, 0) == 1);
    CHECK(getElement(v, 2, 3) == 6);
    CHECK(getElement(v, 9, 9) == 19);
  }

  SECTION("lambda functions")
  {
    CHECK(getElement([](int i, int j) { return i * j - 1; }, 3, 2) == 5);
  }
}

struct EigenMatrixLike {
  std::size_t rows() const { return 10; }
  std::size_t cols() const { return 20; }
};

struct libFieldFieldLike {
  std::size_t size(int dim) const { return (dim + 1) * 5; }
};

TEST_CASE("2D getSize functions")
{
  using namespace libIntegrate;
  SECTION("Eigen Matrix Like")
  {
    EigenMatrixLike a;
    CHECK(getSizeX(a) == 10);
    CHECK(getSizeY(a) == 20);
  }
  SECTION("libField Field Like")
  {
    libFieldFieldLike a;
    CHECK(getSizeX(a) == 5);
    CHECK(getSizeY(a) == 10);
  }

  SECTION("std::vector of std::vector<double>")
  {
    std::vector<std::vector<double>> v(10);
    for(int i = 0; i < 10; i++) {
      v[i] = std::vector<double>(20);
      for(int j = 0; j < 10; j++) {
        v[i][j] = i + j + 1;
      }
    }

    CHECK(getSizeX(v) == 10);
    CHECK(getSizeY(v) == 20);
  }
}
