#
# This file is the makefile for building CSL memory benchmarking app.
#
#   Copyright (c) Texas Instruments Incorporated 2021
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(MULTICORE), 1)	
	APP_NAME = $(MEM)_dual_core_memory_benchmarking_app_freertos
else
	APP_NAME = $(MEM)_memory_benchmarking_app_freertos
endif

MEMORY_BENCHMARKING_APP_PATH = $(pdk_PATH)/ti/csl/example/ospi/memory_benchmarking_apps

SRCDIR = . $(MEMORY_BENCHMARKING_APP_PATH)/src
INCDIR = .

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
INCLUDE_EXTERNAL_INTERFACES += freertos
SRCS_COMMON += main_rtos.c
SRCS_COMMON += r5_mpu_freertos.c

EXTERNAL_LNKCMD_FILE_LOCAL = $(pdk_PATH)/ti/csl/example/ospi/memory_benchmarking_apps/linker_files/$(MEM)/$(SOC)/linker_$(CORE)_$(MEM).lds

CFLAGS_LOCAL_COMMON += -DUART_ENABLED

ifeq ($(CORE), mcu2_0)
	CFLAGS_LOCAL_COMMON += -DNO_SCISERVER
	CFLAGS_LOCAL_COMMON += -DBUILD_MCU2_0
else
	CFLAGS_LOCAL_COMMON += -DBUILD_MCU1_0
	COMP_LIST_COMMON += sciserver_tirtos
endif

ifeq ($(MULTICORE), 1)
	CFLAGS_LOCAL_COMMON += -DMULTICORE
endif

ifeq ($(MEM), ocmc)
	CFLAGS_LOCAL_COMMON += -DBUILD_OCMC
else ifeq ($(MEM), msmc)
	CFLAGS_LOCAL_COMMON += -DBUILD_MSMC
else ifeq ($(MEM), ddr)
	CFLAGS_LOCAL_COMMON += -DBUILD_DDR
else ifeq ($(MEM), xip)
	CFLAGS_LOCAL_COMMON += -DBUILD_XIP
endif


SRCS_EMAC_UT_NULL =

EXT_LIB_LIST_mcu1_0 =


PACKAGE_SRCS_COMMON = .

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
