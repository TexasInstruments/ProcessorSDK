#
# This file is the makefile for building MCAN unit test application.
#
SRCDIR = testApp testLib
INCDIR = testInput testLib

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES =

# List all the components required by the application
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x j721s2))
    COMP_LIST_COMMON = udma_apputils
endif
ifeq ($(SOC),$(filter $(SOC), tda2px tda3xx dra78x))
  COMP_LIST_COMMON = csl_utils_common csl_uart_console
endif

ifeq ($(BUILD_OS_TYPE), baremetal)
  SRCS_COMMON = main_baremetal.c
  CFLAGS_OS_DEFINES = -DBAREMETAL
  COMP_LIST_COMMON += csl_arch
  ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x j721s2))
    COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
  else
    COMP_LIST_COMMON += csl
    ifeq ($(SOC),$(filter $(SOC), tda2px tda3xx dra78x))
      COMP_LIST_COMMON += pm_hal pm_lib
    endif
  endif
  ifeq ($(CORE),ipu1_0)
    LNKCMD_FILE = $(PDK_CSL_COMP_PATH)/example/lnk_m4.cmd
  endif
  ifeq ($(CORE),mpu1_0)
    LNKFLAGS_LOCAL_mpu1_0 += --entry Entry
    EXTERNAL_LNKCMD_FILE_LOCAL = arm_a53.cmd
  endif
endif

ifeq ($(BUILD_OS_TYPE), freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  INCLUDE_EXTERNAL_INTERFACES += freertos
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
  SRCS_COMMON = main_rtos.c utils_prf.c
endif

XDC_CFG_UPDATE_$(CORE) = mcan_ut.cfg

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += st_mcanApp.c st_mcanParser.c st_mcanCommon.c st_mcanTxApp.c st_mcanRxApp.c

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)
PACKAGE_SRCS_COMMON = . ../csl_test_component.mk

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
