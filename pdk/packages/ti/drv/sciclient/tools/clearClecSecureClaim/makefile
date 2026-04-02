# Makefile for C7x image to Clear 'Secure Claim' bit in CLEC
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

#Name of the directory created under packages/ti/binary/
APP_NAME = sciclient_clear_clec_secure_claim

SRCDIR = . 
SRCS_COMMON = boot.c

# This application doesn't have any dependency on PDK libs.
# Also this is Board independent.

EXTERNAL_LNKCMD_FILE_LOCAL = ./linker_c7x.cmd

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
