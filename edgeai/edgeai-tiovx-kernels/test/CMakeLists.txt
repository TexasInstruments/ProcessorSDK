cmake_minimum_required(VERSION 3.10.0)

set(PROJ_NAME edgeai-tiovx-kernels-test)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

set(SRC_FILES
    app_tiovx_dl_color_convert_node_test.c
    app_tiovx_dl_pre_proc_node_test.c
    main.c)

build_app(${PROJ_NAME}
          ${SRC_FILES})
