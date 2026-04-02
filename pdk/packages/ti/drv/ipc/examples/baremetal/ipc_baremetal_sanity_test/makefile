#
# This is the makefile for building IPC sanity test app for baremetal
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_baremetal_sanity_test

SRCS_COMMON = ipc_baremetal_sanity_test.c
BUILD_OS_TYPE = baremetal

ifeq ($(CORE), mcu2_0)
    CFLAGS_LOCAL_COMMON = -DENABLE_UART_PRINT
endif

PACKAGE_SRCS_COMMON = .

#Path for including common files
IPC_COMMON_PATH = ../../common

include $(IPC_COMMON_PATH)/makefile.mk
