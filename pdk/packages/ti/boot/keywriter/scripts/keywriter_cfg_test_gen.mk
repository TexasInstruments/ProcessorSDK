#
# This file is the makefile for building Keywriter image
# that is loaded by R5 ROM.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME=keywriter_cfg_test_gen

.PHONY: keywriter_cfg_test_gen

$(APP_NAME):
	./scripts/generate_test_binaries.sh

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk
