app-setup:
  mkdir -p build-app

app-deps:
  just app-setup
  conan install applications -of build-app

app-configure:
  just app-deps
  cmake -S applications -B build-app -DBUILD_TESTS=OFF -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

app-build:
  just app-configure
  cmake --build build-app --config Release

lib-setup:
  mkdir -p build-lib

lib-deps:
  just lib-setup
  conan install . -of build-lib

lib-configure:
  just lib-deps
  cmake -B build-lib -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

lib-build:
  just lib-configure
  cmake --build build-lib --config Release

lib-test:
  just lib-build
  ./build-lib/testing/Release/libIntegrate_CatchTests
