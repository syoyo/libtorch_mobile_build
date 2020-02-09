#!/bin/bash

torch_dir=`pwd`/build_mobile

CMAKE_BIN=cmake

BUILD_DIR=`pwd`/build

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cd ${BUILD_DIR}

$CMAKE_BIN \
  -DCMAKE_VERBOSE_MAKEFILE=1 \
  -DTorch_DIR=$torch_dir \
  ..
