#
# This file is the makefile for building IPC single-core echo test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_rtos_echo_test_$(BUILD_OS_TYPE)

SRCS_COMMON = ipc_rtos_echo_test.c

ifeq ($(CORE), mcu1_0)
    CFLAGS_LOCAL_COMMON = -DENABLE_UART_PRINT
endif

PACKAGE_SRCS_COMMON = .

#Path for including common files
IPC_COMMON_PATH = ../../common

include $(IPC_COMMON_PATH)/makefile.mk
