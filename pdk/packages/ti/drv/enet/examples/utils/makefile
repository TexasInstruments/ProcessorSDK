#
# Example utils (TI-RTOS) makefile
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif
include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

# Two flavors of the enet_example_utils_<OS_TYPE> are built depending on whether
# Enet utils can use UART or not.
ifeq ($(BUILD_OS_TYPE),baremetal)
  MODULE_NAME = enet_example_utils$(ENET_OS_TYPE_POSTFIX)
  ENET_CFLAGS += -DENETAPPUTILS_UART_ALLOWED
endif
ifeq ($(BUILD_OS_TYPE),freertos)
  ifeq ($(APPUTILS_TYPE),full)
    MODULE_NAME = enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)
    ENET_CFLAGS += -DENETAPPUTILS_UART_ALLOWED
  else
    MODULE_NAME = enet_example_utils$(ENET_OS_TYPE_POSTFIX)
  endif
endif
ifeq ($(BUILD_OS_TYPE),safertos)
  ifeq ($(APPUTILS_TYPE),full)
    MODULE_NAME = enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)
    ENET_CFLAGS += -DENETAPPUTILS_UART_ALLOWED
  else
    MODULE_NAME = enet_example_utils$(ENET_OS_TYPE_POSTFIX)
  endif
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON  = ../build/$(SOC)/ ../build/config.mk
PACKAGE_SRCS_COMMON += makefile include

SRCDIR += .
INCDIR += include/

# From CPSW LLD
ifeq ($(SOC),$(filter $(SOC), j721e))
  SRCDIR += V1
  SRCS_COMMON += enet_board_j721e_evm.c
  PACKAGE_SRCS_COMMON += V1
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
  SRCDIR += V2
  SRCS_COMMON += enet_board_j7200_evm.c
  PACKAGE_SRCS_COMMON += V2
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
  SRCDIR += V3
  SRCS_COMMON += enet_board_j721s2_evm.c
  PACKAGE_SRCS_COMMON += V3
endif
ifeq ($(SOC),$(filter $(SOC), j784s4))
  SRCDIR += V4
  SRCS_COMMON += enet_board_j784s4_evm.c
  PACKAGE_SRCS_COMMON += V4
endif
ifeq ($(SOC),$(filter $(SOC), j742s2))
  SRCDIR += V5
  SRCS_COMMON += enet_board_j742s2_evm.c
  PACKAGE_SRCS_COMMON += V5
endif

ifeq ($(BUILD_OS_TYPE),baremetal)
  ENET_CFLAGS += -DBAREMETAL
  SRCS_COMMON += enet_apputils_baremetal.c
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
  # List all the external components/interfaces, whose interface header files
  # need to be included for this component
  INCLUDE_EXTERNAL_INTERFACES +=
  CFLAGS_LOCAL_COMMON =
  INCLUDE_INTERNAL_INTERFACES += lwipif_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipcontrib_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipport_$(BUILD_OS_TYPE)
  SRCS_COMMON += enet_networkutils.c
  SRCS_COMMON += enet_mcm.c
  SRCS_COMMON += enet_apputils_rtos.c
endif

ifeq ($(BUILD_OS_TYPE),safertos)
  ENET_CFLAGS += -DSAFERTOS
  # List all the external components/interfaces, whose interface header files
  # need to be included for this component
  INCLUDE_EXTERNAL_INTERFACES +=
  CFLAGS_LOCAL_COMMON =
  INCLUDE_INTERNAL_INTERFACES += lwipif_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipstack_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipcontrib_$(BUILD_OS_TYPE)
  INCLUDE_INTERNAL_INTERFACES += lwipport_$(BUILD_OS_TYPE)
  SRCS_COMMON += enet_networkutils.c
  SRCS_COMMON += enet_mcm.c
  SRCS_COMMON += enet_apputils_rtos.c
endif

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += enet_apputils.c
SRCS_COMMON += enet_appmemutils.c
SRCS_COMMON += enet_appethpatterns.c
SRCS_COMMON += enet_apprmcfg.c
SRCS_COMMON += enet_udmautils.c
SRCS_COMMON += enet_ioctlutils.c
SRCS_COMMON += enet_apputils_k3.c

PACKAGE_SRCS_COMMON += enet_appethpatterns.c
PACKAGE_SRCS_COMMON += enet_appmemutils.c
PACKAGE_SRCS_COMMON += enet_apputils_baremetal.c
PACKAGE_SRCS_COMMON += enet_apputils.c
PACKAGE_SRCS_COMMON += enet_apputils_k3.c
PACKAGE_SRCS_COMMON += enet_apputils_rtos.c
PACKAGE_SRCS_COMMON += enet_ioctlutils.c
PACKAGE_SRCS_COMMON += enet_mcm.c
PACKAGE_SRCS_COMMON += enet_networkutils.c
PACKAGE_SRCS_COMMON += enet_udmautils.c

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS)

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
