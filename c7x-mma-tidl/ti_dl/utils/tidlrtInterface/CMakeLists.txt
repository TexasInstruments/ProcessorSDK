cmake_minimum_required(VERSION 3.15)
project (TIDLRUNTIME LANGUAGES CXX)

option (BUILD_PYTHON_BINDINGS "Build Python bindings using pybind11" ON)
option (BUILD_COMPILE "Build Compilation APIs" ON)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif()

if (NOT C7X_MMA_TIDL_PATH)
    message(FATAL_ERROR "Please define C7X_MMA_TIDL_PATH!")
endif()

message(STATUS "BUILD_PYTHON_BINDINGS: ${BUILD_PYTHON_BINDINGS}")
message(STATUS "BUILD_COMPILE: ${BUILD_COMPILE}")
message(STATUS "C7X_MMA_TIDL_PATH: ${C7X_MMA_TIDL_PATH}")

set(LIB_NAME "tidlruntime" CACHE STRING "Name of C++ Lib file")
set(SO_NAME "tidlruntime_core" CACHE STRING "Name of Pybind SO file")

if (BUILD_PYTHON_BINDINGS)
    set(PYBIND11_FINDPYTHON ON)
    find_package(Python3 COMPONENTS Interpreter Development)
    if (Python3_FOUND)
        message(STATUS "Found Python interpreter: ${Python3_EXECUTABLE}")
        message(STATUS "Python version: ${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}.${Python3_VERSION_PATCH}")
        message(STATUS "Python include dirs: ${Python3_INCLUDE_DIRS}")
        message(STATUS "Python library: ${Python3_LIBRARIES}")
    else()
        message(FATAL_ERROR "Python not found!")
    endif()

    message(STATUS "Fetching pybind11: REPO-https://github.com/pybind/pybind11.git TAG-v2.13.1")
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY
        https://github.com/pybind/pybind11.git
        GIT_TAG v2.13.1
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# Build static library that can be used in C++ application as well

include_directories(${C7X_MMA_TIDL_PATH}/arm-tidl/rt/inc)
include_directories(${C7X_MMA_TIDL_PATH}/arm-tidl/tidlrt_ep/src)
include_directories(${C7X_MMA_TIDL_PATH}/arm-tidl/common)
include_directories(${C7X_MMA_TIDL_PATH}/common)
include_directories(${C7X_MMA_TIDL_PATH}/inc)
include_directories(${C7X_MMA_TIDL_PATH}/ti_dl/utils/tidlModelImport)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_NAME} PUBLIC include) 

#[[
    SOC is defined as J784S4 for setting maximum TIDL_MAX_NUM_CORES for 
    maximum static allocations in IOBufDesc. TIDL_IMPORT_TOOL is defined to not 
    include ivision as that is not necessary for building 
]]
target_compile_options(${LIB_NAME} PUBLIC -DSOC_J721S2 -DTIDL_IMPORT_TOOL -O3 -Wall -shared -std=c++14 -fPIC)
if(BUILD_COMPILE)
    target_compile_options(${LIB_NAME} PUBLIC -DBUILD_COMPILE)
endif()

if (BUILD_PYTHON_BINDINGS)
    pybind11_add_module(pybind binding/tidlruntime_pybind.cpp)
    target_link_libraries(pybind PRIVATE ${LIB_NAME})
    target_include_directories(pybind PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_compile_options(pybind PRIVATE -O3 -Wall -shared -std=c++14 -fPIC)
    if(BUILD_COMPILE)
        target_compile_options(pybind PRIVATE -DBUILD_COMPILE)
    endif()

    # Rename so from x86_64-linux-gnu->aarch64-linux-gnu for aatch64 build
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))"
            OUTPUT_VARIABLE PYTHON_EXT_SUFFIX_RAW
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        string(REPLACE "x86_64-linux-gnu" "aarch64-linux-gnu" PYTHON_EXT_SUFFIX ${PYTHON_EXT_SUFFIX_RAW})
        set_target_properties(pybind PROPERTIES PREFIX "" OUTPUT_NAME ${SO_NAME} SUFFIX ${PYTHON_EXT_SUFFIX})
    else()
        set_target_properties(pybind PROPERTIES OUTPUT_NAME ${SO_NAME})
    endif()
    
endif()
