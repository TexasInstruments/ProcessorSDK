#
# Copyright (c) 2022, Texas Instruments Incorporated
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

# File: lwip_sysarch_ut_component.mk
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
ifeq ($(lwip_sysarch_ut_component_make_include), )

lwip_sysarch_ut_EXAMPLE_LIST =

#
# LwIP port unit test library
#
define lwipport_unit_test_RULE

export lwipport_unit_test_$(1)_COMP_LIST = lwipport_unit_test_$(1)
lwipport_unit_test_$(1)_RELPATH = ti/transport/lwip/lwip-port/unit_test
lwipport_unit_test_$(1)_PATH = $(PDK_LWIP_COMP_PATH)/lwip-port/unit_test
export lwipport_unit_test_$(1)_MAKEFILE = -f makefile.mk BUILD_OS_TYPE=$(1)
export lwipport_unit_test_$(1)_BOARD_DEPENDENCY = yes
export lwipport_unit_test_$(1)_CORE_DEPENDENCY = yes
export lwipport_unit_test_$(1)_SBL_APPIMAGEGEN = yes
lwipport_unit_test_$(1)_PKG_LIST = lwipport_unit_test_$(1)
lwipport_unit_test_$(1)_INCLUDE = $(PDK_LWIP_COMP_PATH)/lwip-port/$(1)/include
export lwipport_unit_test_$(1)_SOCLIST = $(drvlwip_SOCLIST)
export lwipport_unit_test_$(1)_$(SOC)_CORELIST = $(drvlwip_$(SOC)_CORELIST)
ifeq ($(1),$(filter $(1), freertos))
lwip_sysarch_ut_EXAMPLE_LIST += lwipport_unit_test_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
lwip_sysarch_ut_EXAMPLE_LIST += lwipport_unit_test_$(1)
endif
endif

endef

lwipport_unit_test_MACRO_LIST := $(foreach curos,$(drvlwip_RTOS_LIST),$(call lwipport_unit_test_RULE,$(curos)))

$(eval ${lwipport_unit_test_MACRO_LIST})

export lwip_sysarch_ut_EXAMPLE_LIST

lwip_sysarch_ut_component_make_include := 1
endif
