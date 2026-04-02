#
# This file is the makefile for building IPC driver library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = ipc

CFLAGS_LOCAL_COMMON +=  -DIPC_CFG_PRINT_ENABLE

include src/makefile.mk
