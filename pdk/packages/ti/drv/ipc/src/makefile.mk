#
# This file is the common makefile for building IPC driver library.
#

SRCDIR = src
ifeq ($(SOC),$(filter $(SOC), j721e j7200))
  SRCDIR += soc/V1
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
  SRCDIR += soc/V3
endif
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
  SRCDIR += soc/V4
endif

SRCDIR += src/mailbox/V0/priv
INCDIR += src/mailbox

#$(ECHO) $(ISA)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += ipc_api.c ipc_mp.c ipc_soc.c ipc_virtio.c ipc_utils.c ipc_mailbox.c mailbox.c
SRCS_COMMON += ipc_osal.c

PACKAGE_SRCS_COMMON = ipc.h ipc_component.mk makefile .gitignore include $(SRCDIR)
PACKAGE_SRCS_COMMON += soc/ipc_soc.h src/mailbox src/mailbox/csl_mailbox.h  src/mailbox/cslr_mailbox.h
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(IPC_CFLAGS) -DIPC_EXCLUDE_POLLED_RX -DMAILBOX_INTERRUPT_MODE

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
