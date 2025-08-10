list:
  just --list

app-setup:
  mkdir -p build-app

app-deps:
  just app-setup
  conan install applications -of build-app --build missing

app-configure:
  just app-deps
  cmake -S applications -B build-app -DBUILD_TESTS=OFF -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

app-build:
  just app-configure
  cmake --build build-app --config Release

lib-setup:
  run-if --target build-lib -- mkdir -p build-lib

lib-deps:
  just lib-setup
  run-if --dependency conanfile.py --target build-lib/conan_toolchain.cmake -- conan install . -of build-lib --build missing

lib-configure:
  just lib-deps
  run-if --dependency CMakeLists.txt --target build-lib/CMakeCache.txt -- cmake -B build-lib -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

lib-build:
  just lib-configure
  cmake --build build-lib --config Release

lib-test:
  just lib-build
  cd build-lib && ./testing/Release/libIntegrate_CatchTests
