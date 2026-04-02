#
# This file is the makefile for building IPC multi-core echo test app for baremetal
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_baremetal_echo_test

SRCS_COMMON = ipc_baremetal_echo_test.c
BUILD_OS_TYPE = baremetal

ifeq ($(CORE), mcu1_0)
    CFLAGS_LOCAL_COMMON = -DENABLE_UART_PRINT
endif

PACKAGE_SRCS_COMMON = .

#Path for including common files
IPC_COMMON_PATH = ../common

include $(IPC_COMMON_PATH)/makefile.mk
