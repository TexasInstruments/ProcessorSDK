#
# This file is the makefile for building UDMA driver library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = udma

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCDIR = src
SRCDIR += soc/$(SOC_DIR)
INCDIR =

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

PACKAGE_SRCS_COMMON = udma.h src/udma_priv.h udma_component.mk makefile src/makefile .gitignore docs soc/$(SOC_DIR) 
PACKAGE_SRCS_COMMON += include/udma_cfg.h
PACKAGE_SRCS_COMMON += include/udma_ch.h
PACKAGE_SRCS_COMMON += include/udma_event.h
PACKAGE_SRCS_COMMON += include/udma_flow.h
PACKAGE_SRCS_COMMON += include/udma_osal.h
PACKAGE_SRCS_COMMON += include/udma_proxy.h
PACKAGE_SRCS_COMMON += include/udma_ring.h
PACKAGE_SRCS_COMMON += include/udma_rm.h
PACKAGE_SRCS_COMMON += include/udma_types.h
PACKAGE_SRCS_COMMON += include/udma_utils.h
PACKAGE_SRCS_COMMON += soc/udma_soc.h

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += udma.c
SRCS_COMMON += udma_rm.c udma_rmcfg.c
SRCS_COMMON += udma_ch.c
SRCS_COMMON += udma_ring_common.c udma_proxy.c udma_flow.c udma_event.c
SRCS_COMMON += udma_osal.c udma_utils.c
SRCS_COMMON += udma_soc.c
SRCS_COMMON += udma_ring_normal.c
PACKAGE_SRCS_COMMON += src/udma.c
PACKAGE_SRCS_COMMON += src/udma_rm.c udma_rmcfg.c
PACKAGE_SRCS_COMMON += src/udma_ch.c
PACKAGE_SRCS_COMMON += src/udma_ring_common.c src/udma_proxy.c src/udma_flow.c src/udma_event.c
PACKAGE_SRCS_COMMON += src/udma_osal.c src/udma_utils.c
PACKAGE_SRCS_COMMON += udma_soc.c
PACKAGE_SRCS_COMMON += src/udma_ring_normal.c

#DRU is not present in all SOC
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
SRCS_COMMON += udma_dru.c
PACKAGE_SRCS_COMMON += src/udma_dru.c 
PACKAGE_SRCS_COMMON += include/udma_dru.h
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/dru/makefile
endif

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/bcdma/makefile
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/lcdma_ringacc/makefile
SRCS_COMMON += udma_ring_lcdma.c 
PACKAGE_SRCS_COMMON += src/udma_ring_lcdma.c 
endif

include $(PDK_INSTALL_PATH)/ti/drv/udma/src/ringacc/makefile
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/udmap/makefile
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/proxy/makefile
include $(PDK_INSTALL_PATH)/ti/drv/udma/src/intaggr/makefile

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(UDMA_CFLAGS)

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
