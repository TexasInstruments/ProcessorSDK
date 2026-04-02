#
# This file is the makefile for building Enet driver library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = enet

SRCDIR = src/core src/mod src/per src/phy src/common
INCDIR = . include src/modules/priv src/phy/priv

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = csl enetphy

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += enet.c
SRCS_COMMON += enet_trace.c enet_utils.c enet_osal.c enet_queue.c enet_rm.c
SRCS_COMMON += enet_dma_pktutils.c

# TI-RTOS and PDK based implementations
SRCS_COMMON += enet_utils_dflt.c enet_phymdio_dflt.c

# CPSW modules
SRCS_COMMON += mdio.c
SRCS_COMMON += cpsw_macport.c cpsw_macport_intervlan.c
SRCS_COMMON += cpsw_hostport.c
SRCS_COMMON += cpsw_ale.c
SRCS_COMMON += cpsw_stats.c
SRCS_COMMON += cpsw_cpts.c
SRCS_COMMON += cpsw_macport_est.c cpsw_est.c
SRCS_COMMON += cpsw_macport_iet.c

# Enet Peripheral source files
SRCS_COMMON += cpsw_intervlan.c
SRCS_COMMON += cpsw.c

# DMA modules
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
SRCDIR += src/dma/udma
INCDIR += include/dma/udma
SRCS_COMMON += enet_hostport_udma.c
SRCS_COMMON += enet_udma.c enet_udma_priv.c
endif

PACKAGE_SRCS_COMMON = enet.h enet_cfg.h enet_component.mk makefile src/makefile .gitignore
PACKAGE_SRCS_COMMON += include priv tools docs $(SRCDIR)
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(ENET_CFLAGS)

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
