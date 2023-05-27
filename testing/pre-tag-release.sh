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
conan install .. -s build_type=Release
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# test install
cmake --install . --prefix $PWD/INSTALL --config Release






mkdir app
cd app

cat << EOF > main.cpp
#include <iostream>
#include <libIntegrate/Integrate.hpp>

int main()
{

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
find_package( libIntegrate REQUIRED )
target_link_libraries(main libIntegrate::Integrate )
set_target_properties(main PROPERTIES CXX_STANDARD 11)
EOF

mkdir build1
cd build1
conan install ${root} -s build_type=Release
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DlibIntegrate_DIR=${bindir}/INSTALL/lib/cmake/ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
$(find . -type f -name main)

cd ..

echo "PASSED"

exit 0
