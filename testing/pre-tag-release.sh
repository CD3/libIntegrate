#! /bin/bash

set -e
root=$(git rev-parse --show-toplevel)

cd $root

bindir="$PWD/build-and-test"
function cleanup()
{
  rm -r $bindir
}
trap cleanup EXIT

mkdir $bindir
cd $bindir
cmake .. -DCMAKE_INSTALL_PREFIX=$bindir/install
cmake --build .
cmake --build . --target test

# test install
cmake --build . --target install






mkdir app
cd app

cat << EOF > main.cpp
#include <iostream>
#include <libIntegrate/libIntegrate_version.h>
#include <libIntegrate/Integrate.hpp>

int main()
{
  std::cout << "VERSION: " << libIntegrate_VERSION << std::endl;
  std::cout << "FULL VERSION: " << libIntegrate_VERSION_FULL << std::endl;

  _1D::TrapezoidRule<double> i1;
  _1D::RiemannRule<double>   i2;
  _1D::SimpsonRule<double>   i3;
  _1D::GQ::GaussLegendreQuadrature<double,8> i4;

  return 0;
}
EOF

cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
add_executable( main main.cpp )
find_package( libIntegrate REQUIRED PATHS ${bindir}/install )
target_link_libraries(main libIntegrate::Integrate )
set_target_properties(main PROPERTIES CXX_STANDARD 11)
EOF

mkdir build1
cd build1
cmake .. -DlibIntegrate_DIR=${bindir}/install/cmake/libInegrate
cmake --build .
./main

cd ..

mkdir build2
cd build2
cmake ..
cmake --build .
./main

cd ..



# BACKARD COMPATIBILITY TESTS
cd $bindir
rm app -r
mkdir app
cd app

cat << EOF > main.cpp
#include <iostream>
#include <libIntegrate_version.h>
#include <Integrate.hpp>

int main()
{
  std::cout << "VERSION: " << libIntegrate_VERSION << std::endl;
  std::cout << "FULL VERSION: " << libIntegrate_VERSION_FULL << std::endl;

  _1D::TrapezoidRule<double> i1;
  _1D::RiemannRule<double>   i2;
  _1D::SimpsonRule<double>   i3;
  _1D::GQ::GaussLegendreQuadrature<double,8> i4;

  return 0;
}
EOF

cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
add_executable( main main.cpp )
find_package( libIntegrate REQUIRED PATHS ${bindir}/install )
target_link_libraries(main libIntegrate::Integrate )
set_target_properties(main PROPERTIES CXX_STANDARD 11)
EOF

mkdir build1
cd build1
cmake .. -DlibIntegrate_DIR=${bindir}/install/cmake/libInegrate
cmake --build .
./main

cd ..

mkdir build2
cd build2
cmake ..
cmake --build .
./main







echo "PASSED"

exit 0
