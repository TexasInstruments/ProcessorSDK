#
# This file is the makefile for building IPC echo test for QNX
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_qnx_echo_test_$(BUILD_OS_TYPE)

SRCS_COMMON = ipc_qnx_echo_test.c

PACKAGE_SRCS_COMMON = .

#Path for including common files
IPC_COMMON_PATH = ../common

include $(IPC_COMMON_PATH)/makefile.mk