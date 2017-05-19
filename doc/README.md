Use this directory to store documentation for your project. Technical writeups should be placed
in the `writeups` directory, but doxygen configuration files and output could be placed here.

# Doxygen

Doxygen is a tool that can extract documentation from code comments and build HTML or PDF documentation for your project.
It is most useful for documenting your code's API.

To build your project documentation, you run the `doxygen` executable on a doxygen configuration file.
You can create a template doxygen configuration file with the `-g` option.

    > doxygen -g MyProject.doxygen 
    > # MyProject.doxygen now contains a template configuration. Edit as necessary.
    > doxygen MyProject.doxygen # this will build the documentation.

The `CMakeLists.txt` file created here will automatically detect the doxygen configuration file(s) and build
create a `doc` target to build the documentation. If no configuration file is found, a template file is generated.
So, to build the documentation for your project, you only need to run CMake and then build the `doc` target. On Linux for
example

    > mkdir build
    > cd build
    > cmake ..
    > make doc


