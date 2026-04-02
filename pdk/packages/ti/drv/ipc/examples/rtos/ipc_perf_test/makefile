#
# This file is the makefile for building IPC r5f-r5f test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_perf_test_$(BUILD_OS_TYPE)

PACKAGE_SRCS_COMMON = .

include makefile.mk

