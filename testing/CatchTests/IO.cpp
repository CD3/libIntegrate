#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <libIntegrate/IO.hpp>

using namespace Catch;

TEST_CASE("Gnuplot data reader")
{
  SECTION("1D - single column")
  {
    const std::string filename = "test_function_data-1d-single_column.txt";

    // Step 1: Write y = sin(x) data to file
    {
      std::ofstream out(filename);
      for(int i = 0; i < 100; ++i) {
        double x = i * 0.1;
        double y = std::sin(x);
        out << y << "\n";
      }
    }

    // Step 2: Read the file using the parser
    auto data = libIntegrate::gnuplot::readGnuplotData(filename);

    // Step 3: Verify the data
    REQUIRE(data.size() == 100);
    for(size_t i = 0; i < data.size(); ++i) {
      REQUIRE(data[i].x.has_value());
      REQUIRE(data[i].y.has_value());
      double expected_x = static_cast<double>(i);
      double expected_y = std::sin(i * 0.1);
      REQUIRE(data[i].x.value() == Approx(expected_x));
      REQUIRE(data[i].y.value() == Approx(expected_y));
      REQUIRE(!data[i].z.has_value());  // z should be empty
    }

    // Cleanup
    // std::remove(filename.c_str());
  }
  SECTION("1D - two column")
  {
    const std::string filename = "test_function_data-1d-two_column.txt";

    // Step 1: Write y = sin(x) data to file
    {
      std::ofstream out(filename);
      for(int i = 0; i < 100; ++i) {
        double x = i * 0.1;
        double y = std::sin(x);
        out << x << " " << y << "\n";
      }
    }

    // Step 2: Read the file using the parser
    auto data = libIntegrate::gnuplot::readGnuplotData(filename);

    // Step 3: Verify the data
    REQUIRE(data.size() == 100);
    for(size_t i = 0; i < data.size(); ++i) {
      REQUIRE(data[i].x.has_value());
      REQUIRE(data[i].y.has_value());
      double expected_x = 0.1 * i;
      double expected_y = std::sin(expected_x);
      REQUIRE(data[i].x.value() == Approx(expected_x));
      REQUIRE(data[i].y.value() == Approx(expected_y));
      REQUIRE(!data[i].z.has_value());  // z should be empty
    }

    // Cleanup
    // std::remove(filename.c_str());
  }
  SECTION("2D - three column")
  {
    const std::string filename = "test_function_data-2d-three_column.txt";

    // Step 1: Write y = sin(x) data to file
    {
      std::ofstream out(filename);
      for(int i = 0; i < 100; ++i) {
        double x = i * 0.1;
        for(int j = 0; j < 100; ++j) {
          double y = j * 0.1;
          double z = std::sin(x) * std::sin(y);
          out << x << " " << y << " " << z << "\n";
        }
        out << "\n";
      }
    }

    // Step 2: Read the file using the parser
    auto data = libIntegrate::gnuplot::readGnuplotData(filename);

    // Step 3: Verify the data
    REQUIRE(data.size() == 100 * 100);
    for(size_t i = 0; i < data.size(); ++i) {
      REQUIRE(data[i].x.has_value());
      REQUIRE(data[i].y.has_value());
      REQUIRE(data[i].z.has_value());
      double expected_x = 0.1 * (i / 100);
      double expected_y = 0.1 * (i % 100);
      double expected_z = std::sin(expected_x) * std::sin(expected_y);
      REQUIRE(data[i].x.value() == Approx(expected_x));
      REQUIRE(data[i].y.value() == Approx(expected_y));
      REQUIRE(data[i].z.value() == Approx(expected_z));
    }

    // Cleanup
    // std::remove(filename.c_str());
  }
}

TEST_CASE("extract function")
{
  using namespace libIntegrate::gnuplot;

  SECTION("1D data")
  {
    std::vector<DataPoint> data;
    data.push_back({1.0, 10.0, {}});
    data.push_back({2.0, 20.0, {}});
    data.push_back({{}, 30.0, {}});  // missing x
    data.push_back({4.0, {}, {}});   // missing y
    data.push_back({5.0, 50.0, {}});
    data.push_back({{}, {}, {}});       // missing x and y
    data.push_back({7.0, 70.0, 700.});  // z is ignored

    std::vector<double> x, y;
    extract(data, x, y);

    REQUIRE(x.size() == 4);
    REQUIRE(y.size() == 4);

    std::vector<double> expected_x = {1.0, 2.0, 5.0, 7.0};
    std::vector<double> expected_y = {10.0, 20.0, 50.0, 70.0};

    for(size_t i = 0; i < x.size(); ++i) {
      REQUIRE(x[i] == Approx(expected_x[i]));
      REQUIRE(y[i] == Approx(expected_y[i]));
    }
  }

  SECTION("2D data")
  {
    std::vector<DataPoint> data;
    // z = x*y
    // x = 1,2
    // y = 10,20
    data.push_back({1.0, 10.0, 10.0});
    data.push_back({2.0, 10.0, 20.0});
    data.push_back({1.0, 20.0, 20.0});
    data.push_back({2.0, 20.0, 40.0});
    // some incomplete data points to be ignored
    data.push_back({{}, 10.0, 10.0});
    data.push_back({1.0, {}, 10.0});
    data.push_back({1.0, 10.0, {}});

    std::vector<double>              x, y;
    std::vector<std::vector<double>> z;
    extract(data, x, y, z);

    REQUIRE(x.size() == 2);
    REQUIRE(y.size() == 2);
    REQUIRE(z.size() == 2);
    REQUIRE(z[0].size() == 2);

    std::vector<double> expected_x = {1.0, 2.0};
    std::vector<double> expected_y = {10.0, 20.0};

    for(size_t i = 0; i < x.size(); ++i) {
      REQUIRE(x[i] == Approx(expected_x[i]));
    }
    for(size_t i = 0; i < y.size(); ++i) {
      REQUIRE(y[i] == Approx(expected_y[i]));
    }

    // z[x_idx][y_idx]
    REQUIRE(z[0][0] == Approx(10.0));
    REQUIRE(z[0][1] == Approx(20.0));
    REQUIRE(z[1][0] == Approx(20.0));
    REQUIRE(z[1][1] == Approx(40.0));
  }
}

TEST_CASE("load and extract gnuplot data")
{
  SECTION("1D")
  {
    const char* test_data = R"(# x y
0.0 1
0.1 2
0.2 1
0.3 3
    )";

    std::stringstream in(test_data);

    auto                data = libIntegrate::gnuplot::readGnuplotData(in);
    std::vector<double> x, y;
    libIntegrate::gnuplot::extract(data, x, y);

    REQUIRE(x.size() == 4);
    REQUIRE(y.size() == 4);

    CHECK(x[0] == Approx(0.0).scale(1));
    CHECK(y[0] == Approx(1));
    CHECK(x[1] == Approx(0.1));
    CHECK(y[1] == Approx(2));
    CHECK(x[2] == Approx(0.2));
    CHECK(y[2] == Approx(1));
    CHECK(x[3] == Approx(0.3));
    CHECK(y[3] == Approx(3));
  }
  SECTION("2D")
  {
    const char* test_data = R"(# x y
0.1 3 10
0.1 4 20

0.2 3 30
0.2 4 40

0.3 3 50
0.3 4 60
    )";

    std::stringstream in(test_data);

    auto                             data = libIntegrate::gnuplot::readGnuplotData(in);
    std::vector<double>              x, y;
    std::vector<std::vector<double>> z;
    libIntegrate::gnuplot::extract(data, x, y, z);

    REQUIRE(x.size() == 3);
    REQUIRE(y.size() == 2);
    REQUIRE(z.size() == 3);

    CHECK(x[0] == Approx(0.1));
    CHECK(x[1] == Approx(0.2));
    CHECK(x[2] == Approx(0.3));

    CHECK(y[0] == Approx(3));
    CHECK(y[1] == Approx(4));

    CHECK(z[0][0] == Approx(10));
    CHECK(z[0][1] == Approx(20));
    CHECK(z[1][0] == Approx(30));
    CHECK(z[1][1] == Approx(40));
    CHECK(z[2][0] == Approx(50));
    CHECK(z[2][1] == Approx(60));
  }
}
