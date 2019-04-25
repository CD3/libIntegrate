cmake_minimum_required(VERSION 3.10)
project(libIntegratePackageTest CXX)

find_package(libIntegrate REQUIRED)
add_executable(example example.cpp)
target_link_libraries(example libIntegrate::Integrate)
