# CMakeList.txt : CMake project for libtorch-test, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8.0)
project(main)
#set(CMAKE_CXX_STANDARD 14)

message(STATUS "CURDIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "TorchDIR : " ${Torch_DIR})
#set(Torch_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtorch_android/share/cmake/Torch/")
#find_package(Torch REQUIRED)

set(TORCH_INCLUDE_DIRS
  ${Torch_DIR}/include/
  ${Torch_DIR}/include/torch/csrc/api/include
  )

# specify pytroch_qnnpack twice for the workaround of resolving a cyclic dependency of symbols in pytorch_qnnpack and torch
set(TORCH_LIBRARIES
  ${Torch_DIR}/lib/libeigen_blas.a
  ${Torch_DIR}/lib/libnnpack.a
  ${Torch_DIR}/lib/libpytorch_qnnpack.a
  ${Torch_DIR}/lib/libtorch.a
  ${Torch_DIR}/lib/libpytorch_qnnpack.a
  ${Torch_DIR}/lib/libc10.a
  ${Torch_DIR}/lib/libcpuinfo.a
  ${Torch_DIR}/lib/libclog.a
  )

set(ANDROID_LIBRARIES
  log
  android
  )

include_directories(${TORCH_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (main "libtorch-test.cpp" "libtorch-test.h")
target_link_libraries(main "${TORCH_LIBRARIES}" ${ANDROID_LIBRARIES})

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET main
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:main>)
endif (MSVC)
