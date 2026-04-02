#
# Copyright (c) 2021, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# File: lwipif_component.mk
#       This file is component include make file of lwipif driver library.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             board and the compiled obj/lib has to be kept
#                             under <board> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no board dependent code and hence
#                             the obj/libs are not kept under <board> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(lwip_component_make_include), )

# List with various rtos_types (such as tirtos(sysbios),freertos,safertos) to build RTOS apps.
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
drvlwip_RTOS_LIST       = freertos safertos

drvlwip_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvlwip_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvlwip_j721e_CORELIST  = mcu1_0 mcu2_0 mcu2_1
drvlwip_j7200_CORELIST  = mcu1_0 mcu2_0 mcu2_1
drvlwip_j721s2_CORELIST = mcu1_0 mcu2_0 mcu2_1
drvlwip_j784s4_CORELIST = mcu1_0 mcu2_0 mcu2_1
drvlwip_j742s2_CORELIST = mcu1_0 mcu2_0 mcu2_1

#
# lwip RTOS boardlist rule
#
# Define the rule to generate lwip Drivers BOARDLIST for each rtos_type
# Default BOARDLIST for each rtos_type is defined in 'ti/build/makerules/component.mk'
# The following rule filters out lwip Drivers BOARDLIST for each rtos_type.
# Here $(1) refers to the first argument passed to the rule.
# In this case it is $(curos), each instance in "drvlwip_RTOS_LIST" (ie, tirtos/freertos/safertos..)
#
define DRV_LWIP_RTOS_BOARDLIST_RULE

drvlwip_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvlwip_BOARDLIST))

endef

# Define the macro list with rules of all rtos_types
DRV_LWIP_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvlwip_RTOS_LIST), $(call DRV_LWIP_RTOS_BOARDLIST_RULE,$(curos)))

# Evaluate the macro list to generate BOARDLIST for all rtos_types
$(eval ${DRV_LWIP_RTOS_BOARDLIST_MACRO_LIST})


############################
# lwip package
# List of components included under lwip lib
# The components included here are built and will be part of lwip lib
############################
lwip_LIB_LIST =

############################
# lwip examples
# List of examples under lwip (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
lwip_EXAMPLE_LIST =

#
# LwIP contrib
#
define lwipcontrib_RULE

export lwipcontrib_$(1)_COMP_LIST = lwipcontrib_$(1)
lwipcontrib_$(1)_RELPATH = ti/transport/lwip/lwip-stack/contrib
lwipcontrib_$(1)_PATH = $(PDK_LWIP_COMP_PATH)/lwip-stack/contrib
export lwipcontrib_$(1)_LIBNAME = lwipcontrib_$(1)
export lwipcontrib_$(1)_LIBPATH = $(PDK_LWIP_COMP_PATH)/lwip-stack/contrib/lib/$(1)
export lwipcontrib_$(1)_OBJPATH = ti/transport/lwip/lwip-stack/contrib/$(1)
export lwipcontrib_$(1)_MAKEFILE = -f ../../makefile_contrib BUILD_OS_TYPE=$(1)
export lwipcontrib_$(1)_BOARD_DEPENDENCY = no
export lwipcontrib_$(1)_CORE_DEPENDENCY = no
lwipcontrib_$(1)_PKG_LIST = lwipcontrib_$(1)
lwipcontrib_$(1)_INCLUDE = $(PDK_LWIP_COMP_PATH)/lwip-stack/contrib
export lwipcontrib_$(1)_SOCLIST = $(drvlwip_SOCLIST)
export lwipcontrib_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvlwip_$(SOC)_CORELIST))
ifeq ($(1),$(filter $(1), freertos))
lwip_LIB_LIST += lwipcontrib_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
lwip_LIB_LIST += lwipcontrib_$(1)
endif
endif
endef

lwipcontrib_MACRO_LIST := $(foreach curos,$(drvlwip_RTOS_LIST),$(call lwipcontrib_RULE,$(curos)))
$(eval ${lwipcontrib_MACRO_LIST})

#
# LwIP stack library
#
define lwipstack_RULE

export lwipstack_$(1)_COMP_LIST = lwipstack_$(1)
lwipstack_$(1)_RELPATH = ti/transport/lwip/lwip-stack
lwipstack_$(1)_PATH = $(PDK_LWIP_COMP_PATH)/lwip-stack
export lwipstack_$(1)_LIBNAME = lwipstack_$(1)
export lwipstack_$(1)_LIBPATH = $(PDK_LWIP_COMP_PATH)/lwip-stack/lib/$(1)
export lwipstack_$(1)_OBJPATH = ti/transport/lwip/lwip-stack/$(1)
export lwipstack_$(1)_MAKEFILE = -f ../makefile_stack BUILD_OS_TYPE=$(1)
export lwipstack_$(1)_BOARD_DEPENDENCY = no
export lwipstack_$(1)_CORE_DEPENDENCY = no
lwipstack_$(1)_PKG_LIST = lwipstack_$(1)
lwipstack_$(1)_INCLUDE = $(PDK_LWIP_COMP_PATH)/lwip-stack/src/include
export lwipstack_$(1)_SOCLIST = $(drvlwip_SOCLIST)
export lwipstack_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvlwip_$(SOC)_CORELIST))
ifeq ($(1),$(filter $(1), freertos))
lwip_LIB_LIST += lwipstack_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
lwip_LIB_LIST += lwipstack_$(1)
endif
endif

endef

lwipstack_MACRO_LIST := $(foreach curos,$(drvlwip_RTOS_LIST),$(call lwipstack_RULE,$(curos)))
$(eval ${lwipstack_MACRO_LIST})

#
# LwIP port library
#
define lwipport_RULE

export lwipport_$(1)_COMP_LIST = lwipport_$(1)
lwipport_$(1)_RELPATH = ti/transport/lwip/lwip-port
lwipport_$(1)_PATH = $(PDK_LWIP_COMP_PATH)/lwip-port
export lwipport_$(1)_LIBNAME = lwipport_$(1)
export lwipport_$(1)_LIBPATH = $(PDK_LWIP_COMP_PATH)/lwip-port/lib/$(1)
export lwipport_$(1)_OBJPATH = ti/transport/lwip/lwip-port/$(1)
export lwipport_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export lwipport_$(1)_BOARD_DEPENDENCY = no
export lwipport_$(1)_CORE_DEPENDENCY = no
lwipport_$(1)_PKG_LIST = lwipport_$(1)
lwipport_$(1)_INCLUDE = $(PDK_LWIP_COMP_PATH)/lwip-port/$(1)/include $(PDK_LWIP_COMP_PATH)/lwip-port/config/$(SOC) $(PDK_LWIP_COMP_PATH)/lwip-port/config
export lwipport_$(1)_SOCLIST = $(drvlwip_SOCLIST)
export lwipport_$(1)_$(SOC)_CORELIST = $(drvlwip_$(SOC)_CORELIST)
ifeq ($(1),$(filter $(1), freertos))
lwip_LIB_LIST += lwipport_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
lwip_LIB_LIST += lwipport_$(1)
endif
endif

endef

lwipport_MACRO_LIST := $(foreach curos,$(drvlwip_RTOS_LIST),$(call lwipport_RULE,$(curos)))
$(eval ${lwipport_MACRO_LIST})

#
# Unit test lwip-port
#
-include $(PDK_LWIP_COMP_PATH)/lwip-port/unit_test/lwip_sysarch_ut_component.mk
ifneq ($(lwip_sysarch_ut_EXAMPLE_LIST),)
  lwip_EXAMPLE_LIST += $(lwip_sysarch_ut_EXAMPLE_LIST)
endif

lwip_component_make_include := 1
endif
