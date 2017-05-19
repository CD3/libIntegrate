cmake_minimum_required( VERSION 3.1 )

message( "Configuring unit test build." )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${PROJECT_SOURCE_DIR}/src" )

set( binDir ${CMAKE_BINARY_DIR}/testBin )
set( buildDir ${CMAKE_BINARY_DIR} )

# Unit tests written with the Catch framework are
# automatically detected. Just put the source files
# in the `CatchTests` directory.

set( unitTest_DIR ./CatchTests)
# Search for sources.
file( GLOB_RECURSE unitTest_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "${unitTest_DIR}/*.cpp" )
message(STATUS "Detected Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()


# Add unit test executable
set( testName CatchTests )
add_executable(${testName} ${unitTest_SOURCES})
target_compile_features( ${testName} PRIVATE ${cxx_needed_features})
target_compile_definitions( ${testName} PRIVATE -DTESTING )

#Link dependencies
#target_link_libraries(${testName} <TARGET> ) # If your project builds a library, link to it here

# Make sure that testing binaries are created in the binDir directory
set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${binDir} )

# Now add the unit test binary to the CMake test target
#Notice the WORKING_DIRECTORY and COMMAND
add_test(NAME ${testName} 
         WORKING_DIRECTORY ${buildDir}
         COMMAND ${binDir}/${testName} )


# Copy the data directory to the build directory
file(COPY data DESTINATION ${buildDir} )
