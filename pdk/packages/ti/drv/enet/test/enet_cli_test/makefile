#
# Enet CLI Application makefile
#

include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_cli_test_freertos

SRCDIR += ../enet_cli_test \
          ../enet_cli_test/src
INCDIR += ../enet_cli_test \
          ../enet_cli_test/include \
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
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full_freertos freertos_cli enet_cli
COMP_LIST_COMMON += lwipif_freertos
COMP_LIST_COMMON += lwipstack_freertos
COMP_LIST_COMMON += lwipcontrib_freertos
COMP_LIST_COMMON += lwipport_freertos

# Source files
SRCS_COMMON += cli_main.c ./src/cli_common.c ./src/rate_limit.c ./src/l2_networking.c test_config_$(SOC).c ./src/test_phy.c ./src/test_ale.c

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON +=  makefile cli_main.c ./src/cli_common.c ./src/rate_limit.c ./src/l2_networking.c ./src/test_phy.c ./src/test_ale.c
PACKAGE_SRCS_COMMON +=  ./include/cli_common.h ./include/rate_limit.h ./include/l2_networking.h test_config_$(SOC).c test_config.h ./include/test_phy.h ./include/test_ale.h

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
