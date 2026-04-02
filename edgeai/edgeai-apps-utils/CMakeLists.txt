cmake_minimum_required(VERSION 3.10.0)

project(edgeai-apps-utils)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake)

set(SRC_FILES
    src/edgeai_dl_color_convert_armv8_utils.c
    src/edgeai_dl_pre_proc_armv8_utils.c
    src/edgeai_dl_scaler_armv8_utils.c
    src/edgeai_nv12_drawing_utils.c
    src/edgeai_nv12_font_utils.c
    src/edgeai_perf_stats_utils.c
    src/edgeai_overlay_perf_stats_utils.c
    src/edgeai_arm_neon_utils.c)

build_lib(${PROJECT_NAME}  # Named argument: library name
          SHARED           # Named argument: library type
          0.1.0            # Named argument: library version
          # The following all will be clubbed under ${ARGN}
          ${SRC_FILES})

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
