#
# This file is the makefile for building LWIP Example library.
#
include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_lwip_example$(ENET_OS_TYPE_POSTFIX)

# lwip example dir
SRCDIR += .
INCDIR += .

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
INCLUDE_EXTERNAL_INTERFACES += pdk

INCLUDE_INTERNAL_INTERFACES += lwipif_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipcontrib_$(BUILD_OS_TYPE)
INCLUDE_INTERNAL_INTERFACES += lwipport_$(BUILD_OS_TYPE)

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipif$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipstack$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipcontrib$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += lwipport$(ENET_OS_TYPE_POSTFIX)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += main.c
SRCS_COMMON += test.c
SRCS_COMMON += udp_iperf.c
SRCS_COMMON += test_config_$(SOC).c

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS)

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON =  makefile
PACKAGE_SRCS_COMMON += main.c test.c lwipcfg.h
PACKAGE_SRCS_COMMON += udp_iperf.c udp_iperf.h
PACKAGE_SRCS_COMMON += test_config_$(SOC).c test_config.h

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
