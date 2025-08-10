cmake_minimum_required(VERSION 3.20)


include("version.cmake")
if(NOT DEFINED libIntegrate_FULL_VERSION)
  message(STATUS "Version not set, attempting to detected with git")
  execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_VARIABLE GIT_DESCRIBE_ERROR
    ERROR_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE GIT_DESCRIBE_RESULT
  )
  if("${GIT_DESCRIBE_RESULT}" STREQUAL "0")
    message(STATUS "Version detected with git")
    set(libIntegrate_FULL_VERSION "${GIT_DESCRIBE_VERSION}" CACHE STRING "Full build version number")
  else()
    message(WARNING "Could NOT detect version number with git")
    message(WARNING "STDERR: ${GIT_DESCRIBE_RESULT} ${GIT_DESCRIBE_ERROR}")
    set(libIntegrate_FULL_VERSION "0.0.0-UNKNOWN" CACHE STRING "WARNING!! Full build version number could not be determined.")
  endif()
endif()

# remove 'v' from beginning of version string if it is there
STRING(SUBSTRING "${libIntegrate_FULL_VERSION}" 0 1 LEADING_CHAR)
if( LEADING_CHAR STREQUAL "v" )
STRING(SUBSTRING "${libIntegrate_FULL_VERSION}" 1 -1 libIntegrate_FULL_VERSION)
endif()
string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*" libIntegrate_PROJECT_VERSION_NUMERIC "${libIntegrate_FULL_VERSION}")

message(STATUS "FULL VERSION: ${libIntegrate_FULL_VERSION}")  
message(STATUS "NUMERIC VERSION NUMBER: ${libIntegrate_PROJECT_VERSION_NUMERIC}")  
# NOTE: this will create a variables named libIntegrate_VERSION
project(libIntegrate VERSION ${libIntegrate_PROJECT_VERSION_NUMERIC})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CMakePrintHelpers)

option(BUILD_TESTS "Build unit tests" ON)

message(STATUS "libIntegrate project version: ${libIntegrate_VERSION}")

find_package(Boost REQUIRED)

add_library(Integrate INTERFACE)
add_library(libIntegrate::Integrate ALIAS Integrate)
target_include_directories(
  Integrate
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/libIntegrate/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/libIntegrate/version.h"
)
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
target_compile_features(Integrate INTERFACE cxx_std_14)

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

if (PROJECT_IS_TOP_LEVEL)
  if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(testing)
  endif()

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
endif()
