#
# This file is the makefile for building EDMA memcpy test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = edma_unit_testapp_$(BUILD_OS_TYPE)

#
# This file is common makefile for building UDMA memcpy test app for both TI-RTOS/baremetal
#
SRCDIR = .
INCDIR =

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_TEST_DEFINES = -DUSE_BIOS
COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP) 
INCLUDE_EXTERNAL_INTERFACES += xdc bios pdk
XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif
ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_TEST_DEFINES = -DFREERTOS
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
INCLUDE_EXTERNAL_INTERFACES += freertos
endif
ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_TEST_DEFINES = -DSAFERTOS
COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
INCLUDE_EXTERNAL_INTERFACES += safertos
endif

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
# Append local auxilia�ry linker command file
APPEND_LNKCMD_FILE = ./$(SOC)/linker_$(ISA).cmd
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += main.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_TEST_DEFINES)

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
