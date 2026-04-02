# Makefile for UART unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
export DISABLE_RECURSE_DEPS

#Name of the directory created under packages/ti/binary/
APP_NAME = osal_baremetal_cache_test

PACKAGE_SRCS_COMMON = . ../src

SRCDIR = . ../src ../
INCDIR = . ../src ../
# Common source files across all platforms and cores
SRCS_COMMON += osal_cache_test.c


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OSAL_UT) $(CFLAGS_OS_DEFINES)
EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/osal/test/osal_cache_test/linkers/linker_$(CORE)_baremetal.lds

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
