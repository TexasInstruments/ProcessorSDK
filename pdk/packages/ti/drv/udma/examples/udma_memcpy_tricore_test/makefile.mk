#
# This file is common makefile for building UDMA memcpy test app for both TI-RTOS/baremetal
#
SRCDIR = . $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/common
INCDIR = . $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/common

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient_merged 

ifeq ($(BUILD_OS_TYPE), freertos)
  COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP) sciserver_tirtos
  INCLUDE_EXTERNAL_INTERFACES += freertos
  SRCS_COMMON = memcpy_tricore_main_rtos.c
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/udma/examples/udma_memcpy_tricore_test/$(SOC)/linker_r5_common.lds
endif

SRCS_COMMON += sci_app_common.c

ifeq ($(CORE), mcu1_0)
COMP_LIST_COMMON := $(filter-out sciclient_direct$(HS_SUFFIX), $(COMP_LIST_COMMON))
else
COMP_LIST_COMMON := $(filter-out sciclient$(HS_SUFFIX), $(COMP_LIST_COMMON))
COMP_LIST_COMMON += rm_pm_hal
endif
COMP_LIST_COMMON += sciclient_merged$(HS_SUFFIX)

# Linker files for each core
APPEND_LNKCMD_FILE = $(PDK_INSTALL_PATH)/ti/drv/udma/examples/udma_memcpy_tricore_test/$(SOC)/linker_mcu1_0.lds
APPEND2_LNKCMD_FILE = $(PDK_INSTALL_PATH)/ti/drv/udma/examples/udma_memcpy_tricore_test/$(SOC)/linker_mcu2_0.lds
APPEND3_LNKCMD_FILE = $(PDK_INSTALL_PATH)/ti/drv/udma/examples/udma_memcpy_tricore_test/$(SOC)/linker_mcu3_0.lds

# List all the specific components required by the application
COMP_LIST_COMMON += udma_apputils

# Using UDMA_merged for this application
COMP_LIST_COMMON := $(filter-out udma$(HS_SUFFIX), $(COMP_LIST_COMMON))
COMP_LIST_COMMON += udma_merged

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += udma_memcpy_tricore_test.c

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
