#pragma once

/** @file IO.hpp
 * @brief A collection of I/O utilities used by libIntegrate
 * @author C.D. Clark III
 * @date 04/06/21
 */

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <istream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace libIntegrate
{
namespace gnuplot
{

struct DataPoint {
  std::optional<double> x;
  std::optional<double> y;
  std::optional<double> z;
};

// Core parser: reads from any input stream
inline std::vector<DataPoint> readGnuplotData(std::istream& input)
{
  std::vector<DataPoint> data;
  std::string            line;
  size_t                 index = 0;

  while(std::getline(input, line)) {
    size_t start = line.find_first_not_of(" \t");
    if(start == std::string::npos || line[start] == '#') continue;

    std::istringstream  iss(line);
    std::vector<double> values;
    double              val;

    while(iss >> val) {
      values.push_back(val);
    }

    if(values.empty()) continue;

    DataPoint point;
    if(values.size() == 1) {
      point.x = static_cast<double>(index++);
      point.y = values[0];
    } else if(values.size() == 2) {
      point.x = values[0];
      point.y = values[1];
    } else if(values.size() == 3) {
      point.x = values[0];
      point.y = values[1];
      point.z = values[2];
    }

    data.push_back(point);
  }

  return data;
}

// Convenience wrapper: opens file and delegates to stream-based parser
inline std::vector<DataPoint> readGnuplotData(const std::string& filename)
{
  std::ifstream file(filename);
  if(!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  return readGnuplotData(file);
}

// Extracts x and y values from a vector of DataPoints.
// DataPoints that are missing x or y values are skipped.
inline void extract(const std::vector<DataPoint>& data, std::vector<double>& x, std::vector<double>& y)
{
  x.clear();
  y.clear();
  x.reserve(data.size());
  y.reserve(data.size());

  for(const auto& p : data) {
    if(p.x.has_value() && p.y.has_value()) {
      x.push_back(p.x.value());
      y.push_back(p.y.value());
    }
  }
}

// Extracts unique x and y values, and a 2D grid of z values from a vector of DataPoints.
// DataPoints that are missing x, y, or z values are skipped.
inline void extract(const std::vector<DataPoint>& data, std::vector<double>& x, std::vector<double>& y, std::vector<std::vector<double>>& z)
{
  x.clear();
  y.clear();
  z.clear();

  std::set<double> unique_x, unique_y;
  for(const auto& p : data) {
    if(p.x.has_value() && p.y.has_value() && p.z.has_value()) {
      unique_x.insert(p.x.value());
      unique_y.insert(p.y.value());
    }
  }

  x.assign(unique_x.begin(), unique_x.end());
  y.assign(unique_y.begin(), unique_y.end());

  std::map<double, size_t> x_map, y_map;
  for(size_t i = 0; i < x.size(); ++i) x_map[x[i]] = i;
  for(size_t i = 0; i < y.size(); ++i) y_map[y[i]] = i;

  z.resize(x.size(), std::vector<double>(y.size()));

  for(const auto& p : data) {
    if(p.x.has_value() && p.y.has_value() && p.z.has_value()) {
      z[x_map[p.x.value()]][y_map[p.y.value()]] = p.z.value();
    }
  }
}

}  // namespace gnuplot

}  // namespace libIntegrate
