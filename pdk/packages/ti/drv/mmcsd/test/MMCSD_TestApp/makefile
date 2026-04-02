# Makefile for MMCSD unit test app
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)

    APP_NAME = MMCSD_TestApp_$(BUILD_OS_TYPE)
    CFLAGS_OS_DEFINES = -DFREERTOS -DRTOS_ENV
    EXTERNAL_INTERFACES = freertos
    COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

endif


SRCDIR = ../src ../../
INCDIR = ../src ../include ../../
# Common source files across all platforms and cores
SRCS_COMMON += main.c



# List all the components required by the application
COMP_LIST_COMMON += mmcsd

#CFLAGS_MMCSD_UT += -DIO_CONSOLE
# j7 build
ifeq ($(BOARD),$(filter $(BOARD), j721e-evm))
CFLAGS_MMCSD_UT    += -Dj721e_sim
endif

PACKAGE_SRCS_COMMON = ../
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_MMCSD_UT) $(CFLAGS_OS_DEFINES)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = $(EXTERNAL_INTERFACES) pdk

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
