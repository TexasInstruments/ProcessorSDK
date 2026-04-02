#
# Enet CLI library
#
ifeq ($(RULES_MAKE),)
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = enet_cli

SRCDIR += ../enet_cli_core/src \
  ../enet_cli_core/portable/freertos_plus_cli/src \
    
INCDIR += ../../../enet/examples/utils/include \
../enet_cli_core/portable/freertos_plus_cli/include \
../enet_cli_core/include \

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
endif

ifeq ($(BUILD_PROFILE_$(CORE)),release)
  ENET_CFLAGS += -O3 -flto
  LNKFLAGS_LOCAL_COMMON += -flto
endif


INCLUDE_INTERNAL_INTERFACES += freertos_cli

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk freertos enet 

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += enet_cli_phy.c \
    enet_cli_utils.c \
    enet_cli_debug.c \
    enet_cli_config.c \
    enet_cli_port.c \
    enet_cli.c \
    test_config_$(SOC).c

PACKAGE_SRCS_COMMON += ../enet_cli_core


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
