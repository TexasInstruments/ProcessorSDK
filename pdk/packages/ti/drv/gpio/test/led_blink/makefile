# Makefile for GPIO LED Blink unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_OS_TYPE),baremetal)
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON =  $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
GPIO_OS_TESTPREFIX=_Baremetal
GPIO_OS_TESTPOSTFIX=
endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS -DRTOS_ENV
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
GPIO_OS_TESTPREFIX=
GPIO_OS_TESTPOSTFIX=_freertos
endif

#Name of the directory created under packages/ti/binary/
APP_NAME = GPIO$(GPIO_OS_TESTPREFIX)_LedBlink_TestApp$(GPIO_OS_TESTPOSTFIX)

SRCDIR = . src
INCDIR = . src

# Common source files across all platforms and cores
SRCS_COMMON += main_led_blink.c GPIO_board.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# List all the components required by the application
#COMP_LIST_COMMON  += gpio_profile profiling

PACKAGE_SRCS_COMMON = ../../test
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_GPIO_UT) $(CFLAGS_OS_DEFINES)

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
