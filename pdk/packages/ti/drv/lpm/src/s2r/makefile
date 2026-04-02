#
# This makefile has for goal to generate the lpm_s2r.h file.
# It is an header file that contains the code that will be loaded to SRAM for S2R support.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = lpm_s2r

SRCDIR = src/s2r
PACKAGE_SRCS_COMMON = $(SRCDIR)

INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = board

SRCS_COMMON = lpm_s2r.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -nostdlib -fomit-frame-pointer -fno-builtin

# Include common make files
ifeq ($(MAKERULEDIR), )
Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

