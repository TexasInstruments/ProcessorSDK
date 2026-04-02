# Makefile for I2C EEPROM unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

#Baremetal Specifics
ifeq ($(BUILD_OS_TYPE), baremetal)

CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON =  $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =

#Name of the directory created under packages/ti/binary/
APP_NAME = i2c_baremetal_eeprom_testApp

endif

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)

CFLAGS_OS_DEFINES =  -DFREERTOS -DRTOS_ENV
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

#Name of the directory created under packages/ti/binary/
APP_NAME = i2c_eeprom_testApp_$(BUILD_OS_TYPE)

endif

SRCDIR = . src
INCDIR = . src
# Common source files across all platforms and cores
SRCS_COMMON += main_test.c

ifeq ($(SOC),$(filter $(SOC),j721e j7200))
SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
SRCS_COMMON += unity_config.c unity.c
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# List all the specific components required by the application
#COMP_LIST_COMMON  =

CFLAGS_I2C_UT =

PACKAGE_SRCS_COMMON = src makefile
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_I2C_UT) $(CFLAGS_OS_DEFINES)

ifeq ($(SOC),$(filter $(SOC), j721e j7200))
CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
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
