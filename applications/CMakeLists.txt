cmake_minimum_required( VERSION 3.1 )

find_package( Boost REQUIRED COMPONENTS program_options )

add_executable( integrate-cli integrate-cli.cpp )
target_link_libraries( integrate-cli Integrate Boost::program_options )
set_property( TARGET integrate-cli PROPERTY CXX_STANDARD 11 )

add_subdirectory( .. libIntegrate )
#configure_file( ../testing/CramTests/integrate-cli.t ${CMAKE_CURRENT_BINARY_DIR}/integrate-cli.t COPYONLY )
