#
# This file is common makefile for building UDMA memcpy test app for both TI-RTOS/baremetal
#
SRCDIR = .
INCDIR =

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

ifeq ($(BUILD_OS_TYPE), baremetal)
  COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
  SRCS_COMMON = memcpy_main_baremetal.c
  ifeq ($(ISA),$(filter $(ISA), a53 a72))
    LNKFLAGS_LOCAL_$(CORE) += --entry Entry
  endif
endif
ifeq ($(BUILD_OS_TYPE), tirtos)
  COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += xdc bios
  SRCS_COMMON = memcpy_main_rtos.c
  XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif
ifeq ($(BUILD_OS_TYPE), freertos)
  COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += freertos
  SRCS_COMMON = memcpy_main_rtos.c
endif

ifeq ($(BUILD_OS_TYPE), safertos)
  CFLAGS_OS_DEFINES = -DSAFERTOS
  COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += safertos
  SRCS_COMMON = memcpy_main_rtos.c
endif

# List all the specific components required by the application
COMP_LIST_COMMON += udma_apputils

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += udma_memcpy_test.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

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
