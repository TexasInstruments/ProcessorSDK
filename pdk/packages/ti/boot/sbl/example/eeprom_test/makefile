#
# This file is a makefile to build sbl_eeprom_test image which is used to check whether the data dumped to eeprom is proper or not.
#

include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = sbl_eeprom_test
BUILD_OS_TYPE = baremetal
LOCAL_APP_NAME = $(APP_NAME)_$(BOARD)_$(CORE)_TestApp

SBL_SRC_DIR =  $(PDK_INSTALL_PATH)/ti/boot/sbl
SRCDIR      += $(PDK_SBL_COMP_PATH)/example/eeprom_test
INCDIR      += $(PDK_SBL_COMP_PATH)/example/eeprom_test
INCDIR      += $(PDK_INSTALL_PATH)

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)

SRCS_COMMON += sbl_eeprom_test.c

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
