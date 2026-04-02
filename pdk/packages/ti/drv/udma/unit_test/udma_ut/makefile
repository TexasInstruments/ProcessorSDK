#
# This file is the makefile for building UDMA unit test app for TI RTOS
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = udma_unit_testapp_$(BUILD_OS_TYPE)
ifeq ($(UDMA_UT_MANUAL_ENTRY), yes)
APP_NAME = udma_user_input_unit_testapp_$(BUILD_OS_TYPE)
endif
ifeq ($(UDMA_UT_BAREMETAL), yes)
APP_NAME = udma_baremetal_unit_testapp
BUILD_OS_TYPE=baremetal
endif
ifeq ($(UDMA_UT_DYNAMIC_ANALYSIS), yes)
APP_NAME = udma_dynamic_unit_testapp
BUILD_OS_TYPE=baremetal
endif
ifeq ($(USE_UDMA_MERGED), yes)
APP_NAME = udma_merged_unit_testapp_$(BUILD_OS_TYPE)
endif

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

#
# This file is common makefile for building UDMA unit test app for RTOS
#
SRCDIR = . ./src ./src/soc/$(SOC_DIR) ./rtos
INCDIR = . ./src ./src/soc/$(SOC_DIR) ./rtos

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

ifeq ($(BUILD_OS_TYPE), freertos)
  COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += freertos
  SRCS_COMMON = ut_main_rtos.c
endif

ifeq ($(BUILD_OS_TYPE), safertos)
  CFLAGS_OS_DEFINES = -DSAFERTOS
  COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += safertos
  SRCS_COMMON = ut_main_rtos.c
endif
# List all the specific components required by the application
COMP_LIST_COMMON += udma_apputils

# Using UDMA_merged for this application
ifeq ($(USE_UDMA_MERGED), yes)
COMP_LIST_COMMON := $(filter-out udma$(HS_SUFFIX), $(COMP_LIST_COMMON))
COMP_LIST_COMMON += udma_merged
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ./src ../udma_ut_component.mk
SRCS_COMMON += main_common.c
SRCS_COMMON += udma_test_parser.c udma_test_blkcpy.c udma_test_common.c
SRCS_COMMON += udma_test_ring.c udma_test_ch.c udma_test_ch_neg.c udma_test_rm.c
SRCS_COMMON += udma_test_event.c udma_test_flow.c
SRCS_COMMON += udma_test_bug.c udma_test_misc.c
SRCS_COMMON += udma_test_soc.c
SRCS_COMMON += udma_test_proxy.c udma_test_ring_monitor.c
SRCS_COMMON += utils_trace.c
SRCS_COMMON += udma_test_drv_neg.c
SRCS_COMMON += udma_test_utils_neg.c
SRCS_COMMON += udma_test_flow_neg.c
SRCS_COMMON += udma_test_ring_normal.c
SRCS_COMMON += udma_test_ring_common.c
SRCS_COMMON += udma_test_proxy_neg.c
SRCS_COMMON += udma_test_event_neg.c
SRCS_COMMON += udma_test_ring_monitor_neg.c
SRCS_COMMON += udma_test_osal_neg.c
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2 j721s2 j721e j7200))
SRCS_COMMON += udma_test_csl_ringacc.c udma_test_csl_udmap.c udma_test_csl_proxy.c udma_test_csl_intaggr.c
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2 j721s2 j721e))
SRCS_COMMON += udma_test_csl_dru.c
endif
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2 j721s2))
SRCS_COMMON += udma_test_csl_lcdma_ringacc.c udma_test_csl_bcdma.c
endif
ifeq ($(ISA), c7x)
SRCS_COMMON += udma_test_csl_clec.c
endif
endif
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
SRCS_COMMON += udma_test_ring_lcdma.c
endif
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
SRCS_COMMON += udma_test_ring_lcdma_neg.c
endif
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
SRCS_COMMON += udma_test_utc.c
endif

ifeq ($(BUILD_OS_TYPE), safertos)
  SRCS_COMMON += utils_mem_baremetal.c
else
  SRCS_COMMON += utils_mem.c
endif
ifeq ($(BUILD_OS_TYPE), freertos)
  SRCS_COMMON += utils_prf.c
else
  SRCS_COMMON += utils_prf_baremetal.c
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(UDMA_CFLAGS) $(UDMAUT_CFLAGS) $(CFLAGS_OS_DEFINES)
ifeq ($(UDMA_UT_MANUAL_ENTRY), yes)
  CFLAGS_LOCAL_COMMON += -DUDMA_UT_ENABLE_MANUAL_ENTRY
endif
ifeq ($(UDMA_UT_BAREMETAL), yes)
  CFLAGS_LOCAL_COMMON += -DUDMA_UT_BAREMETAL
endif
ifeq ($(UDMA_UT_DYNAMIC_ANALYSIS), yes)
  CFLAGS_LOCAL_COMMON += -DUDMA_UT_DYNAMIC_ANALYSIS -DUDMA_UT_BAREMETAL
endif
ifeq ($(ISA), c7x)
  APPEND_LNKCMD_FILE = rtos/$(SOC)/linker_append_$(CORE).cmd
else
  APPEND_LNKCMD_FILE = rtos/$(SOC)/linker_append_$(ISA).cmd
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

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
