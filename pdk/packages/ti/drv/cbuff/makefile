#
# This file is the makefile for building cbuff.
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

COMP = drvcbuff

lib_$(COMP)_BUILD_DEPENDENCY = soc

$(COMP)_DOXYGEN_SUPPORT = yes

include $(PDK_INSTALL_PATH)/ti/build/comp_top.mk

