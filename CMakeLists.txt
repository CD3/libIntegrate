cmake_minimum_required( VERSION 3.14 )

if( TARGET libIntegrate )
  return()
endif( TARGET libIntegrate )

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR} )
  set(STANDALONE ON)
endif( )

option( BUILD_TESTS "Build unit tests" ON )
if( ${BUILD_TESTS} )
enable_testing()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libIntegrate)
include(macro-git_version)
GIT_VERSION(libIntegrate)
message( STATUS "libIntegrate version: ${${PROJECT_NAME}_VERSION}")

# need to run configure_file twice
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/${PROJECT_NAME}_version.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_version.h")
configure_file( "${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_version.h"
                "${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_version.h")


find_package( Boost REQUIRED )

string( REGEX REPLACE "^lib" "" LIB_NAME ${PROJECT_NAME} )
add_library( ${LIB_NAME} INTERFACE )
add_library( ${PROJECT_NAME}::${LIB_NAME} ALIAS ${LIB_NAME} )
target_include_directories( ${LIB_NAME} INTERFACE
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${${PROJECT_NAME}_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_sources(
  ${LIB_NAME}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/Integrate.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_2D/RiemannRule.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_2D/GaussianQuadratures/GaussLegendre.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/Experimental/_1D/GaussianQuadratures/GaussLegendre.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_1D/SimpsonRule.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_1D/RiemannRule.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_1D/TrapezoidRule.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_1D/GaussianQuadratures/GaussLegendre.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/_1D/Boost/GaussKronrod.hpp>
)

target_link_libraries(${LIB_NAME} INTERFACE Boost::boost)
target_compile_features( ${LIB_NAME} INTERFACE cxx_std_11 )

if( STANDALONE AND BUILD_TESTS )
add_subdirectory( testing )
endif()








install( TARGETS ${LIB_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include/${PROJECT_NAME}
)
install(DIRECTORY src/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION cmake
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
"include(CMakeFindDependencyMacro)
find_dependency(Boost)
include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)
"
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION cmake
)




INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ library for numerical integration.")
SET(CPACK_PACKAGE_VENDOR "C.D. Clark III")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
if( ${PROJECT_NAME}_VERSION_MAJOR)
  SET(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
endif()
if( ${PROJECT_NAME}_VERSION_MINOR)
  SET(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
endif()
if( ${PROJECT_NAME}_VERSION_PATCH)
  SET(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
endif()
INCLUDE(CPack)



