#!/bin/bash

torch_dir=`pwd`/build_android_arm64-v8a

## Please edit android sdk/ndk/cmake path
ANDROID_SDK_ROOT=$HOME/Android/Sdk/
#ANDROID_NDK_ROOT=$HOME/Android/Sdk/ndk-bundle
#CMAKE_BIN=$ANDROID_SDK_ROOT/cmake/3.6.4111459/bin/cmake
ANDROID_NDK_ROOT=/home/syoyo/Android/Sdk/ndk/21.0.6113669/
CMAKE_BIN=$ANDROID_SDK_ROOT/cmake/3.10.2.4988404/bin/cmake

curdir=`pwd`
BUILD_DIR=${curdir}/build

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

# For ninja build
# -DCMAKE_MAKE_PROGRAM=$ANDROID_SDK_ROOT/cmake/3.6.4111459/bin/ninja \

cd ${BUILD_DIR}

## NOTE: gnustl fails to compile json.hpp : https://github.com/nlohmann/json/issues/219
$CMAKE_BIN \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_VERBOSE_MAKEFILE=1 \
-DTorch_DIR=$torch_dir \
-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a \
-DANDROID_NATIVE_API_LEVEL=28 \
-DANDROID_STL=c++_static \
..
