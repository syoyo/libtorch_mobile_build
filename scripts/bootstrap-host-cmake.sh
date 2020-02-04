#!/bin/bash

torch_dir=`pwd`/build_android_arm64-v8a

## Please edit android sdk/ndk/cmake path
CMAKE_BIN=cmake

BUILD_DIR=`pwd`/build

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cd ${BUILD_DIR}

## NOTE: gnustl fails to compile json.hpp : https://github.com/nlohmann/json/issues/219
$CMAKE_BIN \
-DCMAKE_VERBOSE_MAKEFILE=1 \
-DTorch_DIR=$torch_dir \
..
