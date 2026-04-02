cmake_minimum_required(VERSION 3.10.0)

project(edgeai-tiovx-kernels)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake)

set(SRC_FILES
    host/tivx_dl_color_convert_armv8_host.c
    host/tivx_dl_post_proc_host.c
    host/tivx_dl_pre_proc_armv8_host.c
    host/tivx_edgeai_img_proc_node_api.c
    host/vx_edgeai_kernels_host.c
    target/armv8/tivx_armv8_kernels_target.c
    target/armv8/vx_dl_color_convert_armv8_target.c
    target/armv8/vx_dl_post_proc_target.c
    target/armv8/vx_dl_pre_proc_armv8_target.c
    target/edgeai_tiovx_kernels_utils.c)

build_lib(${PROJECT_NAME}  # Named argument: library name
          SHARED           # Named argument: library type
          0.1.0            # Named argument: library version
          # The following all will be clubbed under ${ARGN}
          ${SRC_FILES})

add_subdirectory(test)

option(INSTALL_SRC      "Install src under /opt" OFF)

if (INSTALL_SRC)
    install(DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/
            DESTINATION /opt/${PROJECT_NAME}
            PATTERN     "*/build*" EXCLUDE
            PATTERN     "*/lib*"   EXCLUDE
            PATTERN     "*/bin*"   EXCLUDE
            PATTERN     "*/data*"  EXCLUDE
            PATTERN     "*/.git*"  EXCLUDE)
endif()
