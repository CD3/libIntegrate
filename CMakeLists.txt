cmake_minimum_required( VERSION 3.1 )

if( TARGET libIntegrate )
  return()
endif( TARGET libIntegrate )

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR} )
  set(STANDALONE ON)
endif( )

enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libIntegrate)
if( STANDALONE )
  include(GitVersion)
  GIT_VERSION(${PROJECT_NAME})
else()
  set(${PROJECT_NAME}_VERSION "X.X-subproject")
endif()

# need to run configure_file twice
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")


set( cxx_needed_features
     cxx_auto_type
   )

find_package( Boost REQUIRED )

string( REGEX REPLACE "^lib" "" LIB_NAME ${PROJECT_NAME} )
add_library( ${LIB_NAME} INTERFACE )
target_include_directories( ${LIB_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_sources(
  ${LIB_NAME}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/_1D/SimpsonRule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/_1D/TrapezoidRule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/_1D/RiemannRule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/_2D/RiemannRule.hpp
)

target_link_libraries(${LIB_NAME} INTERFACE Boost::boost)

if( STANDALONE )
add_subdirectory( testing )
endif()
