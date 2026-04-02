#
# This file is the makefile for building lwip-port library.
#
ifeq ($(RULES_MAKE), )
  include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
  include $(RULES_MAKE)
endif

MODULE_NAME = lwipport_$(BUILD_OS_TYPE)

SRCDIR += $(BUILD_OS_TYPE)
INCDIR += $(BUILD_OS_TYPE)/include
INCDIR += $(BUILD_OS_TYPE)/include/arch
INCDIR += config/$(SOC)
INCDIR += config

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
else ifeq ($(BUILD_OS_TYPE),safertos)
  ENET_CFLAGS += -DSAFERTOS
endif

ifeq ($(BUILD_PROFILE_$(CORE)),release)
  ENET_CFLAGS += -O3 -flto
  LNKFLAGS_LOCAL_COMMON += -flto
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
ifeq ($(BUILD_OS_TYPE),freertos)
  INCLUDE_EXTERNAL_INTERFACES = pdk osal freertos
endif
ifeq ($(BUILD_OS_TYPE),safertos)
  INCLUDE_EXTERNAL_INTERFACES = pdk osal safertos
endif

INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += sys_arch.c

PACKAGE_SRCS_COMMON =  makefile
PACKAGE_SRCS_COMMON += freertos safertos
PACKAGE_SRCS_COMMON += config/$(SOC)
PACKAGE_SRCS_COMMON += config/lwipopts_common.h

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Temporary override for "_SYS_SELECT_H" redefined warning
ifeq ($(CORE), mpu1_0)
  CFLAGS_LOCAL_COMMON += -std=c99
endif

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
