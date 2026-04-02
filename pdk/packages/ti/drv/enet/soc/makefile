#
# Enet SoC library
#
ifeq ($(RULES_MAKE),)
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = enetsoc

ifeq ($(SOC),$(filter $(SOC), j721e))
  SRCDIR += j7x j7x/j721e
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
  SRCDIR += j7x j7x/j7200
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
  SRCDIR += j7x j7x/j721s2
endif
ifeq ($(SOC),$(filter $(SOC), j784s4))
  SRCDIR += j7x j7x/j784s4
endif
ifeq ($(SOC),$(filter $(SOC), j742s2))
  SRCDIR += j7x j7x/j742s2
endif

INCDIR = .

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = csl

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += enet_soc.c

# J7x common source files
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    SRCS_COMMON += j7x_soc.c
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(ENET_CFLAGS)

PACKAGE_SRCS_COMMON = enet_soc.h makefile $(SRCDIR)


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
