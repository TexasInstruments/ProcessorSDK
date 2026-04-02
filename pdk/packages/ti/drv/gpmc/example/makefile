# Makefile for GPMC unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

BUILD_OS_TYPE = baremetal
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON =  $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =

SRCDIR = . src
INCDIR = . src
# Common source files across all platforms and cores
SRCS_COMMON += main_gpmc_probing_example.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

#Name of the directory created under packages/ti/binary/
APP_NAME = GPMC_Baremetal_Probing_Example

COMP_LIST_COMMON   += gpmc

PACKAGE_SRCS_COMMON = makefile src/

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
