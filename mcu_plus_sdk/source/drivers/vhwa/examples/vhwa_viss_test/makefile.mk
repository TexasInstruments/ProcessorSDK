#
# This file is common makefile for building VHWA VISS test app for both TI-RTOS/baremetal
#
SRCDIR = .
SRCDIR += ../common
INCDIR =
INCDIR += $(VHWA_COMP_PATH)

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
SRCS_COMMON += vhwa_examples_common.c vhwa_viss_api.c vhwa_common_crc.c ldra_vhwa_coverage_main.c

# List all the components required by the application
COMP_LIST_COMMON = fvid2 vhwa

ifeq ($(BUILD_OS_TYPE), baremetal)
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
  SRCS_COMMON += main_baremetal.c
  EXTERNAL_LNKCMD_FILE_LOCAL = ../common/$(SOC)/vhwa_r5.cmd
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  INCLUDE_EXTERNAL_INTERFACES += freertos
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
  SRCS_COMMON += main_rtos.c

  EXTERNAL_LNKCMD_FILE_LOCAL = ../common/$(SOC)/vhwa_freertos_r5.cmd
endif

ifeq ($(BUILD_OS_TYPE),safertos)
  CFLAGS_OS_DEFINES = -DSAFERTOS
  INCLUDE_EXTERNAL_INTERFACES += safertos
  COMP_LIST_COMMON += $(PDK_COMMON_SAFERTOS_COMP)
  SRCS_COMMON += main_rtos.c

  EXTERNAL_LNKCMD_FILE_LOCAL = ../common/vhwa_safertos_r5.cmd
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../include ../common

LNKFLAGS_LOCAL_mpu1_0 += --entry Entry
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_GLOBAL_$(BOARD)) $(CFLAGS_OS_DEFINES)

ifeq ($(SOC),$(filter $(SOC), j721e j721s2))
  # For J721E/J721S2 only VPAC Instance 0 is present
  CFLAGS_LOCAL_COMMON += -DVPAC_TEST_INSTANCE=0
endif
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
  # For J784S4/J742S2 VPAC Instance 0/1 is currently supported on  mcu2_0/mcu4_0 respectively
  CFLAGS_LOCAL_mcu2_0 += -DVPAC_TEST_INSTANCE=0
  CFLAGS_LOCAL_mcu4_0 += -DVPAC_TEST_INSTANCE=1
endif

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

LIB_LIST_VHWA = $(vhwa_LIBPATH)/$(SOC)/$(CORE)/$(BUILD_PROFILE)/vhwa.aer5f
LNK_LIBS += $(LIB_LIST_VHWA)
ifeq ($(vhwa_viss_testapp_freertos_SBL_APPIMAGEGEN),yes)
vhwa_viss_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos sbl_appimagegen
endif
ifeq ($(vhwa_viss_testapp_safertos_SBL_APPIMAGEGEN),yes)
vhwa_viss_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos sbl_appimagegen
endif
ifeq ($(vhwa_viss_baremetal_testapp_SBL_APPIMAGEGEN),yes)
vhwa_viss_baremetal_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal sbl_appimagegen
endif
# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
