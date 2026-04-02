#
# This file is the makefile for building DSS display test app for both TI RTOS
# and baremetal
#

SRCDIR = src
INCDIR = . src

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

# List all the components required by the application
COMP_LIST_COMMON = fvid2 csirx
ifeq ($(BUILD_OS_TYPE), baremetal)
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
  SRCS_COMMON += main_baremetal.c
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
  CFLAGS_OS_DEFINES = -DUSE_BIOS
  INCLUDE_EXTERNAL_INTERFACES += xdc bios
  COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)
  SRCS_COMMON += main_rtos.c
  # Enable XDC build for application by providing XDC CFG File per core
  XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
  SRCS_COMMON += main_rtos.c
  INCLUDE_EXTERNAL_INTERFACES += freertos
endif

# Common source files and CFLAGS across all platforms and cores
# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += csirx_asf_esm_test_main.c csirx_esm.c serdes_sensor_config.c
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CSIRX_CFLAGS) $(CFLAGS_OS_DEFINES)

# SDL Include Files
SDL_INSTALL_PATH=$(PDK_INSTALL_PATH)/../../sdl
INCDIR += $(SDL_INSTALL_PATH)/
INCDIR += $(SDL_INSTALL_PATH)/src/sdl
INCDIR += $(SDL_INSTALL_PATH)/src/sdl/esm
INCDIR += $(SDL_INSTALL_PATH)/src/sdl/esm/soc/$(SOC_DIR)
INCDIR += $(SDL_INSTALL_PATH)/include/soc/$(SOC_DIR)
# SDL Integration
EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/osal/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_osal.$(LIBEXT)
EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/ip/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_ip.$(LIBEXT)
EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/sdl/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_api.$(LIBEXT)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
