#
# Copyright (c) 2025, Texas Instruments Incorporated
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

# File: gtc_component.mk
#       This file is component include make file of GTC driver.
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
#                             core and the compiled obj/rtos has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/rtoss are not kept under <core> dir.
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
ifeq ($(gtc_component_make_include), )

drvgtc_RTOS_LIST = $(DEFAULT_RTOS_LIST)
# under other list
drvgtc_BOARDLIST       = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm

drvgtc_SOCLIST        += j721e j7200 j721s2 j784s4 j742s2

drvgtc_j7200_CORELIST     = $(DEFAULT_j7200_CORELIST)
drvgtc_j7200_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvgtc_j721e_CORELIST     = $(DEFAULT_j721e_CORELIST)
drvgtc_j721e_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvgtc_j721s2_CORELIST    = $(DEFAULT_j721s2_CORELIST)
drvgtc_j721s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvgtc_j784s4_CORELIST    = $(DEFAULT_j784s4_CORELIST)
drvgtc_j784s4_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvgtc_j742s2_CORELIST    = $(DEFAULT_j742s2_CORELIST)
drvgtc_j742s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1

############################
# gtc package
# List of components included under gtc driver.
# The components included here are built and will be part of gtc driver
############################
gtc_LIB_LIST = gtc
drvgtc_LIB_LIST = $(gtc_LIB_LIST)

############################
# gtc examples
# List of examples under gtc
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
gtc_EXAMPLE_LIST=

drvgtc_EXAMPLE_LIST = $(gtc_EXAMPLE_LIST)

#
# GTC Modules
#

# GTC LIB
export gtc_COMP_LIST = gtc
gtc_RELPATH = ti/drv/gtc
gtc_PATH = $(PDK_GTC_COMP_PATH)
export gtc_LIBNAME = ti.drv.gtc
export gtc_LIBPATH = $(gtc_PATH)/lib
export gtc_OBJPATH = $(gtc_RELPATH)/gtc
export gtc_MAKEFILE = -f build/makefile.mk
export gtc_BOARD_DEPENDENCY = no
export gtc_CORE_DEPENDENCY = no
export gtc_SOC_DEPENDENCY = yes
export gtc_PKG_LIST = gtc
gtc_INCLUDE = $(gtc_PATH)
export gtc_SOCLIST = $(drvgtc_SOCLIST)
export gtc_$(SOC)_CORELIST = $(drvgtc_$(SOC)_CORELIST)

#
# GTC Examples
#

# GTC rtos Example timer test
define GTC_Timer_TestApp_RULE
export GTC_Timer_TestApp_$(1)_COMP_LIST = GTC_Timer_TestApp_$(1)
export GTC_Timer_TestApp_$(1)_RELPATH = ti/drv/gtc/examples/gtc_timer_test
export GTC_Timer_TestApp_$(1)_PATH = $(PDK_GTC_COMP_PATH)/examples/gtc_timer_test
export GTC_Timer_TestApp_$(1)_BOARD_DEPENDENCY = yes
export GTC_Timer_TestApp_$(1)_CORE_DEPENDENCY = no
export GTC_Timer_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export GTC_Timer_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export GTC_Timer_TestApp_$(1)_PKG_LIST = GTC_Timer_TestApp_$(1)
export GTC_Timer_TestApp_$(1)_INCLUDE = $(GTC_Timer_TestApp_$(1)_PATH)
export GTC_Timer_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvgtc_BOARDLIST))
export GTC_Timer_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvgtc_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
    gtc_EXAMPLE_LIST += GTC_Timer_TestApp_$(1)
endif
export GTC_Timer_TestApp_$(1)_SBL_APPIMAGEGEN = yes
endef

GTC_Timer_TestApp_MACRO_LIST := $(foreach curos,$(drvgtc_RTOS_LIST),$(call GTC_Timer_TestApp_RULE,$(curos)))

$(eval ${GTC_Timer_TestApp_MACRO_LIST})

export drvgtc_LIB_LIST
export drvgtc_EXAMPLE_LIST
export gtc_LIB_LIST
export gtc_EXAMPLE_LIST

gtc_component_make_include := 1
endif
