# CMakeList.txt : CMake project for libtorch-test, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8.0)
project(main)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME torchtest)

message(STATUS "CURDIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "TorchDIR : " ${Torch_DIR})
#set(Torch_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtorch_android/share/cmake/Torch/")
#find_package(Torch REQUIRED)

find_package(Threads REQUIRED)

set(TORCH_INCLUDE_DIRS
  ${Torch_DIR}/include/
  ${Torch_DIR}/include/torch/csrc/api/include
  )

if (ANDROID)

  function(import_static_lib name)
    add_library(${name} STATIC IMPORTED)
    set_property(
        TARGET ${name}
        PROPERTY IMPORTED_LOCATION
        ${Torch_DIR}/lib/${name}.a)
  endfunction(import_static_lib)

  import_static_lib(libtorch)
  import_static_lib(libc10)
  import_static_lib(libnnpack)
  import_static_lib(libpytorch_qnnpack)
  import_static_lib(libeigen_blas)
  import_static_lib(libcpuinfo)
  import_static_lib(libclog)

  # Add source to this project's executable.
  add_executable (${TARGET_NAME} "libtorch-test.cpp" "libtorch-test.h")
  #target_include_directories(testmain PRIVATE ${TORCH_INCLUDE_DIRS})
  #add_library (${TARGET_NAME} SHARED "libtorch-test.cpp")

  target_link_libraries(${TARGET_NAME}
      -Wl,--gc-sections
      -Wl,--whole-archive
      libtorch
      -Wl,--no-whole-archive
      libc10
      libnnpack
      libpytorch_qnnpack
      libeigen_blas
      libcpuinfo
      libclog
      android
      log
      Threads::Threads
  )
  target_include_directories(${TARGET_NAME} PRIVATE ${TORCH_INCLUDE_DIRS})

  # strip symbols. it drastically reduce .so size(480MB before strip -> 20MB after strip for pytorch v1.4.0)
  set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)

  ## Link most things statically on Android.
  #target_link_libraries(torch
  #    -Wl,--gc-sections
  #    -Wl,--whole-archive
  #    libtorch
  #    -Wl,--no-whole-archive
  #    libc10
  #    libnnpack
  #    libpytorch_qnnpack
  #    libeigen_blas
  #    libcpuinfo
  #    libclog
  #)

  ## Link most things statically on Android.
  #target_link_libraries(testmain
  #    -Wl,--gc-sections
  #    -Wl,--whole-archive
  #    libtorch
  #    -Wl,--no-whole-archive
  #    libc10
  #    libnnpack
  #    libpytorch_qnnpack
  #    libeigen_blas
  #    libcpuinfo
  #    libclog
  #  log
  #  android
  #)


else (ANDROID)

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

endif (ANDROID)


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
