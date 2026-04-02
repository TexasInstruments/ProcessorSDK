#
# Unit test for sys_arch makefile
#
ifeq ($(RULES_MAKE), )
  include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
  include $(RULES_MAKE)
endif

APP_NAME = lwipport_unit_test_$(BUILD_OS_TYPE)

SRCDIR += .
INCDIR += .


# List all the external components/interfaces, whose interface header files
# need to be included for this component
ifeq ($(BUILD_OS_TYPE),safertos)
  INCLUDE_EXTERNAL_INTERFACES += pdk safertos
  COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
endif
ifeq ($(BUILD_OS_TYPE),freertos)
  INCLUDE_EXTERNAL_INTERFACES += pdk freertos
  COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
endif

INCLUDE_INTERNAL_INTERFACES += lwipport_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS)

COMP_LIST_COMMON += lwipport_$(BUILD_OS_TYPE)
COMP_LIST_COMMON += lwipstack_$(BUILD_OS_TYPE)

ifeq ($(BUILD_OS_TYPE),safertos)
  CFLAGS_LOCAL_COMMON += -DSAFERTOS
endif
ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_LOCAL_COMMON += -DFREERTOS
endif

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += main.c test_sys_arch.c

PACKAGE_SRCS_COMMON = .

SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
SRCS_COMMON += unity_config.c unity.c

CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H -DUNITY_INCLUDE_DOUBLE

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
