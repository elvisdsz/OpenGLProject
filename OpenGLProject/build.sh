#!/bin/sh

echo "Running cmake . . ."
rm -r ./build/
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug

echo "Building with ninja . . ."
ninja

# echo "Running executable..."
# ./GfxEngine
