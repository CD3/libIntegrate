cmake_minimum_required( VERSION 3.20 )

project(integrate-cli)

find_package( Boost REQUIRED COMPONENTS program_options )
find_package( libInterpolate )

add_executable( integrate-cli integrate-cli.cpp )
target_link_libraries( integrate-cli Integrate Boost::program_options libInterpolate::Interpolate )
set_property( TARGET integrate-cli PROPERTY CXX_STANDARD 20 )

add_subdirectory( .. libIntegrate )
#configure_file( ../testing/CramTests/integrate-cli.t ${CMAKE_CURRENT_BINARY_DIR}/integrate-cli.t COPYONLY )

install( TARGETS integrate-cli )
