#
# This file is the makefile for building CBUFF test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)
XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
EXTERNAL_INTERFACES = safertos
COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
endif

SRCDIR = .
INCDIR = .

#Name of the directory created under packages/ti/binary/
APP_NAME = cbuff_manual_test_$(BUILD_OS_TYPE)

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# List all the components required by the application
COMP_LIST_COMMON += cbuff adcbuf

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_mcu1_0 += main.c
SRCS_c66xdsp_1 += main.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

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
