#
# This file is the makefile for building FreeRTOS Unit Test
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif


APP_NAME = freertos_test_ut
BUILD_OS_TYPE=freertos
CFLAGS_TEST_DEFINES =


SRCDIR = .
INCDIR = . ../

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk freertos
ifeq ($(BUILD_OS_TYPE), freertos)
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
else
# List all the components required by the application
endif

#COMP_LIST_COMMON += freertos

ifeq ($(ISA),$(filter $(ISA), c66))
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/linker_c66_freertos.cmd
endif
ifeq ($(ISA),$(filter $(ISA), r5f))
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/linker_$(CORE)_freertos.lds
else
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/linker_r5_freertos.lds
endif
endif

ifeq ($(SOC),$(filter $(SOC), tpr12))
# Append local auxillary linker command file
APPEND_LNKCMD_FILE = ../build/$(SOC)/linker_$(ISA).cmd
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../FREERTOS_log.h
SRCS_COMMON += main_ut.c test_freertos.c

SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
SRCS_COMMON += unity_config.c unity.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_TEST_DEFINES)
CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H -DUNITY_INCLUDE_DOUBLE

#disabling C99 as 'asm()' is not available in C99
#ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1))
# CFLAGS_LOCAL_COMMON += -std=c99
#endif
ifeq ($(CORE),$(filter $(CORE), c66xdsp_1 c66xdsp_2 c7x_1 c7x_2 c7x_3 c7x_4))
 CFLAGS_LOCAL_COMMON += --c99
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
