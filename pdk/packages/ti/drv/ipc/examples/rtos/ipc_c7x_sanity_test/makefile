#
# This is the makefile for building IPC single core echo test app for RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_c7x_sanity_test_$(BUILD_OS_TYPE)

SRCS_COMMON = ipc_c7x_sanity_test.c

ifeq ($(CORE), mcu1_0)
    CFLAGS_LOCAL_COMMON = -DENABLE_UART_PRINT
else 
ifeq ($(CORE), c7x_1)
    CFLAGS_LOCAL_COMMON = -DENABLE_UART_PRINT -DIPC_SANITY_C7X	
endif
endif

PACKAGE_SRCS_COMMON = .

#Path for including common files
IPC_COMMON_PATH = ../../common

include $(IPC_COMMON_PATH)/makefile.mk
