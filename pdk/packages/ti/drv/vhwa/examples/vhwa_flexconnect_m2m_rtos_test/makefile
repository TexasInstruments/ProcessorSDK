#
# This file is the makefile for building VHWA VISS test app for TI RTOS
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = vhwa_flexconnect_m2m_testapp_freertos

include $(VHWA_INSTALL_PATH)/vhwa/examples/vhwa_flexconnect_m2m_rtos_test/makefile.mk
