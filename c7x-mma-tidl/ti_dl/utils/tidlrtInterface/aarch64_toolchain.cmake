set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Provide SDK Path here
set(SDK_ROOT "")

# SDK toolchain paths 
set(CMAKE_C_COMPILER ${SDK_ROOT}/toolchain/sysroots/x86_64-arago-linux/usr/bin/aarch64-oe-linux/aarch64-oe-linux-gcc)
set(CMAKE_CXX_COMPILER ${SDK_ROOT}/toolchain/sysroots/x86_64-arago-linux/usr/bin/aarch64-oe-linux/aarch64-oe-linux-g++)

# SDK targetfs path
set(CMAKE_SYSROOT ${SDK_ROOT}/targetfs)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

# Use PC python for interpreter
find_program(HOST_Python3_EXECUTABLE python3 REQUIRED)
set (Python3_EXECUTABLE "${HOST_Python3_EXECUTABLE}" CACHE FILEPATH "Use host python for ABI info")

# Include python libraries and includes from targetfs
set (Python3_ROOT_DIR ${CMAKE_SYSROOT}/usr)
set (Python3_INCLUDE_DIRS ${CMAKE_SYSROOT}/usr/include/python3.12)  # Change python version if SDK python version differs
set (Python3_LIBRARIES ${CMAKE_SYSROOT}/usr/lib/libpython3.12.so) # Change python version if SDK python version differs

# Dont build compilation APIs for aarch64
set (BUILD_COMPILE OFF)

message(STATUS "aarch64 toolchain loaded")