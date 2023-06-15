message( "Configuring unit test build." )

set( buildDir ${CMAKE_BINARY_DIR} )
set( binDir ${CMAKE_BINARY_DIR}/testBin )

find_package(OpenMP)
find_package(Catch2 REQUIRED)

# Find all sources.
file( GLOB_RECURSE unitTest_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./CatchTests/*.cpp"
      )
message(STATUS "Detected Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()

set( testName ${PROJECT_NAME}_CatchTests )
add_executable(${testName} ${unitTest_SOURCES})
target_compile_definitions( ${testName} PRIVATE -DTESTING $<$<BOOL:${MSVC}>:_USE_MATH_DEFINES> )
target_link_libraries(${testName} PRIVATE Integrate Catch2::Catch2WithMain)
if(OpenMP_CXX_FOUND)
  target_link_libraries(${testName} PRIVATE OpenMP::OpenMP_CXX)
endif()
target_include_directories( ${testName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" )
# set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binDir})
set_target_properties(${testName} PROPERTIES CXX_STANDARD 14)
if(MSVC)
  target_compile_options(${testName} PRIVATE /W4 /WX -wd4068)
else()
  target_compile_options(${testName} PRIVATE -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter -pedantic -Werror)
endif()

add_test(NAME ${testName} 
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${testName} )

file(COPY data DESTINATION ${CMAKE_BINARY_DIR} )
