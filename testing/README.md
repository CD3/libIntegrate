Use this directory for unit tests. Unit tests are small program that call your
code and check that it returns what you expect. If you use the
[Catch](https://github.com/philsquared/Catch) framework, all you need to do is create
.cpp files in the `CatchTests` directory. The `CMakeLists.txt` file will automatically
detect and compile it into the unit test binary.

You can use any unit test framework you want, but you will have to modify the CMakeLists.txt file
to check for it and build the test accordingly. If you do this, you need to check for the
test framework. **Do not** just add commands to build your unit tests. If somebody does not have
the unit test frame work you are using installed, then they won't be able to build your project without
installing it.

If you are not writing unit tests, then you need to start. If you don't know what framework
to use, then just use [Catch](https://github.com/philsquared/Catch). It is easy to use, does not
have to be compiled, and can be included your project repo directly because it is contained in a single
header file.

The `CMakeLists.txt` file included in this directory will automatically detect all .cpp files in the
`CatchTests` directory.
