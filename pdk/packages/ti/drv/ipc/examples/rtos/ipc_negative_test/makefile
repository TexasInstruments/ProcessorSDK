#
# This file is the makefile for building IPC multi-core echo test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_negative_test_$(BUILD_OS_TYPE)

SRCS_COMMON += ipc_negative_test.c ipc_neg_testsetup.c
PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON += -DIPC_NEGATIVE_TEST

#Path for including common files
IPC_COMMON_PATH = ../../common

include $(IPC_COMMON_PATH)/makefile.mk
