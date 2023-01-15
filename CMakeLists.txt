cmake_minimum_required(VERSION 3.16)
project(libIntegrate)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CMakePrintHelpers)

option(BUILD_TESTS "Build unit tests" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(macro-git_version)
git_version(libIntegrate)
message(STATUS "libIntegrate version: ${libIntegrate_VERSION}")

find_package(Boost REQUIRED)

# need to run configure_file twice
configure_file(
  "${CMAKE_SOURCE_DIR}/src/libIntegrate/libIntegrate_version.h.in"
  "${CMAKE_BINARY_DIR}/include/libIntegrate/libIntegrate_version.h")
configure_file(
  "${CMAKE_BINARY_DIR}/include/libIntegrate/libIntegrate_version.h"
  "${CMAKE_BINARY_DIR}/include/libIntegrate/libIntegrate_version.h")

add_library(Integrate INTERFACE)
add_library(libIntegrate::Integrate ALIAS Integrate)
target_include_directories(
  Integrate
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_sources(
  Integrate
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/Integrate.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/Utils.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/RiemannRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/TrapezoidRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/SimpsonRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/GaussianQuadratures/GaussLegendre.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/RandomAccessLambda.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_2D/Utils.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/RiemannRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/TrapezoidRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/SimpsonRule.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/GaussianQuadratures/GaussLegendre.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/Boost/GaussKronrod.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/RandomAccessLambda.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/_1D/Utils.hpp>
)

target_link_libraries(Integrate INTERFACE Boost::boost)
target_compile_features(Integrate INTERFACE cxx_std_11)

if(BUILD_TESTS)
  add_subdirectory(testing)
endif()

install(TARGETS Integrate EXPORT libIntegrateTargets)

install(
  DIRECTORY src/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")
install(
  DIRECTORY ${CMAKE_BINARY_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")
install(
  EXPORT libIntegrateTargets
  FILE libIntegrateTargets.cmake
  NAMESPACE libIntegrate::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
file(
  WRITE ${CMAKE_BINARY_DIR}/libIntegrateConfig.cmake
  "include(CMakeFindDependencyMacro)
find_dependency(Boost)
include(\${CMAKE_CURRENT_LIST_DIR}/libIntegrateTargets.cmake)
")
write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/libIntegrateConfigVersion.cmake
  VERSION ${libIntegrate_VERSION}
  COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)
install(
  FILES ${CMAKE_BINARY_DIR}/libIntegrateConfig.cmake
        ${CMAKE_BINARY_DIR}/libIntegrateConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "A C++ library for numerical integration.")
set(CPACK_PACKAGE_VENDOR "C.D. Clark III")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
if(libIntegrate_VERSION_MAJOR)
  set(CPACK_PACKAGE_VERSION_MAJOR ${libIntegrate_VERSION_MAJOR})
endif()
if(libIntegrate_VERSION_MINOR)
  set(CPACK_PACKAGE_VERSION_MINOR ${libIntegrate_VERSION_MINOR})
endif()
if(libIntegrate_VERSION_PATCH)
  set(CPACK_PACKAGE_VERSION_PATCH ${libIntegrate_VERSION_PATCH})
endif()
include(CPack)
