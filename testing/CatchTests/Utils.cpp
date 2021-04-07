#include "catch.hpp"

#include <numeric>
#include <vector>

#include <libIntegrate/Utils.hpp>
#include <libIntegrate/_1D/Utils.hpp>

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
    CHECK( getElement([](int i){return 2*i;},2) == 4 );
    // CHECK( getElement([](double i){return 2*i;},2) == 4 ); compiler error
  }

  SECTION("Non-containers")
  {
    // getElement(0,0); error
  }
}

struct MyContainer
{
  int length() const {return 3;}

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
