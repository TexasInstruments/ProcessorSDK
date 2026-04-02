#
# Copyright (c) 2020, Texas Instruments Incorporated
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

# File: watchdog_component.mk
#       This file is component include makefile of watchdog driver library.
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
#                             platform and the compiled obj/lib has to be kept
#                             under <platform> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no platform dependent code and hence
#                             the obj/libs are not kept under <platform> dir.
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
ifeq ($(watchdog_component_make_include), )

drvwatchdog_RTOS_LIST        = $(DEFAULT_RTOS_LIST)
drvwatchdog_SOCLIST          = tpr12 awr294x
drvwatchdog_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)
drvwatchdog_awr294x_CORELIST = $(DEFAULT_awr294x_CORELIST)
drvwatchdog_BOARDLIST        = tpr12_evm tpr12_qt awr294x_evm

############################
# watchdog package
# List of components included under watchdog lib
# The components included here are built and will be part of watchdog lib
############################
watchdog_LIB_LIST = watchdog

############################
# watchdog examples
# List of examples under watchdog
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
watchdog_EXAMPLE_LIST =

#
# Watchdog Modules
#

# Watchdog library
export watchdog_COMP_LIST = watchdog
watchdog_RELPATH = ti/drv/watchdog
watchdog_PATH = $(PDK_WATCHDOG_COMP_PATH)
export watchdog_LIBNAME = watchdog
export watchdog_LIBPATH = $(watchdog_PATH)/lib
export watchdog_MAKEFILE = -f build/makefile.mk
export watchdog_BOARD_DEPENDENCY = no
export watchdog_CORE_DEPENDENCY = no
export watchdog_SOC_DEPENDENCY = yes
export watchdog_PKG_LIST = watchdog
watchdog_INCLUDE = $(watchdog_PATH)
export watchdog_SOCLIST = $(drvwatchdog_SOCLIST)
export watchdog_$(SOC)_CORELIST = $(drvwatchdog_$(SOC)_CORELIST)

#
# Watchdog Examples
#
# watchdog test app
define watchdog_testapp_RULE

export watchdog_testapp_$(1)_COMP_LIST = watchdog_testapp_$(1)
export watchdog_testapp_$(1)_RELPATH = ti/drv/watchdog/test
export watchdog_testapp_$(1)_PATH = $(PDK_WATCHDOG_COMP_PATH)/test
export watchdog_testapp_$(1)_BOARD_DEPENDENCY = yes
export watchdog_testapp_$(1)_CORE_DEPENDENCY = no
export watchdog_testapp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export watchdog_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export watchdog_testapp_$(1)_PKG_LIST = watchdog_testapp_$(1)
export watchdog_testapp_$(1)_INCLUDE = $(watchdog_testapp_$(1)_PATH)
export watchdog_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvwatchdog_BOARDLIST))
export watchdog_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvwatchdog_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
watchdog_EXAMPLE_LIST += watchdog_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
watchdog_EXAMPLE_LIST += watchdog_testapp_$(1)
endif
endif
export watchdog_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

watchdog_testapp_MACRO_LIST := $(foreach curos,$(drvwatchdog_RTOS_LIST),$(call watchdog_testapp_RULE,$(curos)))

$(eval ${watchdog_testapp_MACRO_LIST})

export watchdog_LIB_LIST
export watchdog_EXAMPLE_LIST
export drvwatchdog_LIB_LIST = $(watchdog_LIB_LIST)
export drvwatchdog_EXAMPLE_LIST = $(watchdog_EXAMPLE_LIST)

export WATCHDOG_CFLAG =

watchdog_component_make_include := 1
endif
