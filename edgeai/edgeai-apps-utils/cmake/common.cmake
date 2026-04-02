include(GNUInstallDirs)

add_compile_options(-Wall)

# Specific compile optios across all targets
#add_compile_definitions(MINIMAL_LOGGING)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE} PROJECT_NAME = ${PROJECT_NAME}")

SET(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so")

if(NOT CMAKE_OUTPUT_DIR)
    set(CMAKE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/../)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_INSTALL_LIBDIR           lib)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr CACHE PATH "Installation Prefix" FORCE)
endif()

if (NOT DEFINED ENV{SOC})
    message(FATAL_ERROR "SOC not defined.")
endif()

set(TARGET_SOC_LOWER $ENV{SOC})

if ("${TARGET_SOC_LOWER}" STREQUAL "j721e")
    set(TARGET_PLATFORM     J7)
    set(TARGET_CPU          A72)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          J721E)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "j721s2")
    set(TARGET_PLATFORM     J7)
    set(TARGET_CPU          A72)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          J721S2)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "j784s4")
    set(TARGET_PLATFORM     J7)
    set(TARGET_CPU          A72)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          J784S4)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "j742s2")
    set(TARGET_PLATFORM     J7)
    set(TARGET_CPU          A72)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          J742S2)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "j722s")
    set(TARGET_PLATFORM     J7)
    set(TARGET_CPU          A53)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          J722S)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "tda54")
    set(TARGET_PLATFORM     TDA5)
    set(TARGET_CPU          A720)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          TDA54)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "am62a")
    set(TARGET_PLATFORM     SITARA)
    set(TARGET_CPU          A53)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          AM62A)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "am62x")
    set(TARGET_PLATFORM     SITARA)
    set(TARGET_CPU          A53)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          AM62X)
elseif ("${TARGET_SOC_LOWER}" STREQUAL "am62p")
    set(TARGET_PLATFORM     SITARA)
    set(TARGET_CPU          A53)
    set(TARGET_OS           LINUX)
    set(TARGET_SOC          AM62P)
else()
    message(FATAL_ERROR "SOC ${TARGET_SOC_LOWER} is not supported.")
endif()

set(TARGET_CPU_TEMP $ENV{TARGET_CPU})
if(NOT ("${TARGET_CPU_TEMP}" STREQUAL ""))
    set(TARGET_CPU ${TARGET_CPU_TEMP})
endif()

message("SOC=${TARGET_SOC_LOWER}")
message("TARGET_CPU=${TARGET_CPU}")

add_definitions(
    -DTARGET_CPU_${TARGET_CPU}
    -DTARGET_OS_${TARGET_OS}
    -DSOC_${TARGET_SOC}
)

#message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
#message("CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")

set(PSDK_INCLUDE_PATH $ENV{PSDK_INCLUDE_PATH})
if ("${PSDK_INCLUDE_PATH}" STREQUAL "")
    set(PSDK_INCLUDE_PATH ${TARGET_FS}/usr/include/)
endif()

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/include
                    ${PSDK_INCLUDE_PATH}/processor_sdk/vision_apps
                    ${PSDK_INCLUDE_PATH}/processor_sdk/app_utils
                   )

set(COMMON_LINK_LIBS
    edgeai-apps-utils
    )

# Function for building a node:
# app_name: app name
# ${ARGN} expands everything after the last named argument to the end
# usage: build_app(app_name a.c b.c....)
function(build_app app_name)
    add_executable(${app_name} ${ARGN})
    target_link_libraries(${app_name}
                          ${COMMON_LINK_LIBS}
                         )

    set(BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
    set(BINS ${CMAKE_OUTPUT_DIR}/bin/${CMAKE_BUILD_TYPE}/${app_name})

    install(FILES ${BINS}
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            DESTINATION ${BIN_INSTALL_DIR})

endfunction()

# Function for building a node:
# lib_name: Name of the library
# lib_type: (STATIC, SHARED)
# lib_ver: Version string of the library
# ${ARGN} expands everything after the last named argument to the end
# usage: build_lib(lib_name lib_type lib_ver a.c b.c ....)
function(build_lib lib_name lib_type lib_ver)
    add_library(${lib_name} ${lib_type} ${ARGN})

    SET_TARGET_PROPERTIES(${lib_name}
                          PROPERTIES
                          VERSION ${lib_ver}
                         )

    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    FILE(GLOB HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

    install(TARGETS ${lib_name}
            EXPORT ${lib_name}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}  # Shared Libs
            ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}  # Static Libs
            RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}  # Executables, DLLs
           )

    # Specify the header files to install
    install(FILES ${HDRS} DESTINATION ${INCLUDE_INSTALL_DIR})

endfunction()

