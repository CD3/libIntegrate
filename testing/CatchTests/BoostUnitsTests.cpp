#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/energy.hpp>
#include <boost/units/systems/si/force.hpp>
#include <boost/units/systems/si/length.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/_1D/RiemannRule.hpp>
using namespace boost::units;
using namespace boost::units::si;
using namespace Catch;

namespace BoostUnitsTests
{

TEST_CASE("Testing Riemann rule with boost units.")
{
  _1D::RiemannRule<quantity<energy>> integrate;
  quantity<energy>                   I;

  std::vector<quantity<length>> x(3);
  std::vector<quantity<force>>  F(3);
  x[0] = 0. * meter;
  F[0] = 1. * newton;
  x[1] = 1. * meter;
  F[1] = 2. * newton;
  x[2] = 2. * meter;
  F[2] = 3. * newton;

  I = integrate(x, F);
  REQUIRE(quantity_cast<double>(I) == Approx(3));
}

}  // namespace BoostUnitsTests
