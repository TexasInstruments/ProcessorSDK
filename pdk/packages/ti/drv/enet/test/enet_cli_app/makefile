#
# Enet CLI Application makefile
#

include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_cli_app

SRCDIR += ../enet_cli_app
INCDIR += ../enet_cli_app \
  ../../examples/utils/include \
  ../../enet_cli/enet_cli_core/portable/freertos_plus_cli/include \


ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
endif


ifeq ($(BUILD_PROFILE_$(CORE)),release)
  ENET_CFLAGS += -O3 -flto
  LNKFLAGS_LOCAL_COMMON += -flto
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk enet

INCLUDE_INTERNAL_INTERFACES += lwipif_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipcontrib_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipport_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += freertos_cli

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full$(ENET_OS_TYPE_POSTFIX) freertos_cli enet_cli
COMP_LIST_COMMON += lwipif$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipstack$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipcontrib$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipport$(ENET_OS_TYPE_POSTFIX)

# Source files
SRCS_COMMON += cli_main.c
SRCS_COMMON += ale_unicast.c ale_vlan.c cli_common.c cli_lwip.c
SRCS_COMMON += l2_networking.c shell.c 

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON +=  makefile ale_unicast.c ale_vlan.c cli_common.c cli_lwip.c cli_main.c gptp_log.c
PACKAGE_SRCS_COMMON +=  gptp_stack.c l2_networking.c shell.c 
PACKAGE_SRCS_COMMON +=  ale_unicast.h ale_vlan.h cli_common.h cli_lwip.h gptp_log.h gptp_stack.h l2_networking.h shell.h

# Temporary override for "_SYS_SELECT_H" redefined warning
ifeq ($(CORE), mpu1_0)
  CFLAGS_LOCAL_COMMON += -std=c99
endif

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS) \


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
