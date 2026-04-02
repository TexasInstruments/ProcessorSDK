# Makefile for CANFD test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_OS_TYPE),baremetal)
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_$(CORE) =
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)

ifeq ($(SOC),$(filter $(SOC), tpr12))
ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  XDC_CFG_FILE_$(CORE) = ./tpr12/mss.cfg
endif
endif

ifeq ($(SOC),$(filter $(SOC), awr294x))
ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  XDC_CFG_FILE_$(CORE) = ./awr294x/mss.cfg
endif
endif

endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
endif

SRCDIR = . src
INCDIR = . src

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
SRCDIR += $(SOC)
INCDIR += $(SOC)
endif

# Common source files across all platforms and cores
SRCS_COMMON += main_canfd_test.c


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

#Name of the directory created under packages/ti/binary/
APP_NAME = canfd_test_$(BUILD_OS_TYPE)

# Name of the binary if different fmake clrom the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  canfd_$(BOARD)_$(CORE)_test_$(BUILD_OS_TYPE)

COMP_LIST_COMMON += canfd esm

PACKAGE_SRCS_COMMON = ../test

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES) $(CFLAGS_UT_DEFINES)

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
