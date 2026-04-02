#
# This file is the makefile for building adcbuf.
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

COMP = drvadcbuf

lib_$(COMP)_BUILD_DEPENDENCY = soc

$(COMP)_DOXYGEN_SUPPORT = yes

include $(PDK_INSTALL_PATH)/ti/build/comp_top.mk

