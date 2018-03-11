#! /bin/bash

set -e
cd $(git rev-parse --show-toplevel)

bindir="$PWD/build-and-test"
function cleanup()
{
  rm -r $bindir
}
trap cleanup EXIT

mkdir $bindir
cd $bindir
cmake ..
cmake --build .
cmake --build . --target test

exit 0
