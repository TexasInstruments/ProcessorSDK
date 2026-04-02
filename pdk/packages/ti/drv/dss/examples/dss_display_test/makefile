#
# This file is the makefile for building DSS display test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

ifeq ($(BUILD_OS_TYPE), baremetal)
    APP_NAME = dss_$(BUILD_OS_TYPE)_display_testapp
else
    APP_NAME = dss_display_testapp_$(BUILD_OS_TYPE)
endif

include makefile.mk
