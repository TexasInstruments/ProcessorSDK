#
# Copyright (c) 2016 - 2024, Texas Instruments Incorporated
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

# File: fatfs_component.mk
#       This file is component include make file of FATFS library.
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
ifeq ($(fatfs_component_make_include), )

# List with various rtos_types(such as freertos,safertos) to build RTOS apps. 
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
drvfatfs_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

# under other list
drvfatfs_BOARDLIST       = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvfatfs_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvfatfs_j721e_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvfatfs_j7200_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1
drvfatfs_j721s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvfatfs_j784s4_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvfatfs_j742s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1

# Define the rule to generate FATFS Drivers BOARDLIST for each rtos_type
# Default BOARDLIST for each rtos_type is defined in 'ti/build/makerules/component.mk'
# The following rule filters out FATFS Drivers BOARDLIST for each rtos_type.
# Here $(1) refers tot the first argument passed to the rule.
# In this case it is $(curos), each instance in "drvfatfs_RTOS_LIST" (ie, freertos/safertos..)
define DRV_FATFS_BOARDLIST_RULE

drvfatfs_$(1)_BOARDLIST     = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvfatfs_BOARDLIST))

endef

# Define the macro list with rules of all rtos_types
DRV_FATFS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvfatfs_RTOS_LIST), $(call DRV_FATFS_BOARDLIST_RULE,$(curos)))

# Evaluate the macro list to generate BOARDLIST for all rtos_types
$(eval ${DRV_FATFS_BOARDLIST_MACRO_LIST})

############################
# uart package
# List of components included under uart lib
# The components included here are built and will be part of uart lib
############################
fatfs_LIB_LIST = fatfs_indp fatfs_multi_indp
drvfatfs_LIB_LIST = $(fatfs_LIB_LIST)
fatfs_EXAMPLE_LIST = 

#
# FATFS Modules
#

# FATFS LIB DEVICE INDEPENDENT
fatfs_indp_COMP_LIST = fatfs_indp
fatfs_indp_RELPATH = ti/fs/fatfs
fatfs_indp_PATH = $(PDK_FATFS_COMP_PATH)
fatfs_indp_LIBNAME = ti.fs.fatfs
export fatfs_indp_LIBNAME
fatfs_indp_LIBPATH = $(fatfs_indp_PATH)/lib
export fatfs_indp_LIBPATH
fatfs_indp_OBJPATH = $(fatfs_indp_RELPATH)/fatfs_indp
export fatfs_indp_OBJPATH
fatfs_indp_MAKEFILE = -f build/makefile_indp.mk
export fatfs_indp_MAKEFILE
fatfs_indp_BOARD_DEPENDENCY = no
fatfs_indp_CORE_DEPENDENCY = no
fatfs_indp_SOC_DEPENDENCY = no
export fatfs_indp_COMP_LIST
export fatfs_indp_BOARD_DEPENDENCY
export fatfs_indp_CORE_DEPENDENCY
export fatfs_indp_SOC_DEPENDENCY
fatfs_indp_PKG_LIST = fatfs_indp
export fatfs_indp_PKG_LIST
fatfs_indp_INCLUDE = $(fatfs_indp_PATH)
fatfs_indp_SOCLIST = $(drvfatfs_SOCLIST)
export fatfs_indp_SOCLIST
fatfs_indp_$(SOC)_CORELIST = $(drvfatfs_$(SOC)_CORELIST)
export fatfs_indp_$(SOC)_CORELIST

# EMMC Readwrite test
# RTOS FATFS Console test apps
define FATFS_CONSOLE_TESTAPP_RULE

export FATFS_Console_TestApp_$(1)_COMP_LIST = FATFS_Console_TestApp_$(1)
FATFS_Console_TestApp_$(1)_RELPATH = ti/fs/fatfs/example/console
FATFS_Console_TestApp_$(1)_PATH = $(PDK_FATFS_COMP_PATH)/example/console
export FATFS_Console_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export FATFS_Console_TestApp_$(1)_BOARD_DEPENDENCY = yes
export FATFS_Console_TestApp_$(1)_CORE_DEPENDENCY = no
FATFS_Console_TestApp_$(1)_PKG_LIST = FATFS_Console_TestApp_$(1)
FATFS_Console_TestApp_$(1)_INCLUDE = $(FATFS_Console_TestApp_$(1)_PATH)
export FATFS_Console_TestApp_$(1)_BOARDLIST = $(drvfatfs_$(1)_BOARDLIST)
export FATFS_Console_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvfatfs_$(SOC)_CORELIST))

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  export FATFS_Console_TestApp_$(1)_SBL_APPIMAGEGEN = yes
endif

ifneq ($(1),$(filter $(1), safertos))
fatfs_EXAMPLE_LIST += FATFS_Console_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
fatfs_EXAMPLE_LIST += FATFS_Console_TestApp_$(1)
endif
endif

endef

FATFS_CONSOLE_TESTAPP_MACRO_LIST := $(foreach curos, $(drvfatfs_RTOS_LIST), $(call FATFS_CONSOLE_TESTAPP_RULE,$(curos)))

$(eval ${FATFS_CONSOLE_TESTAPP_MACRO_LIST})

export drvfatfs_LIB_LIST
export fatfs_LIB_LIST
export fatfs_EXAMPLE_LIST

drvfatfs_EXAMPLE_LIST = $(fatfs_EXAMPLE_LIST)
export drvfatfs_EXAMPLE_LIST

fatfs_component_make_include := 1
endif
