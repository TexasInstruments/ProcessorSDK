#
# Copyright (c) 2016 - 2023, Texas Instruments Incorporated
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

# File: safertos_component.mk
#       This file is component include make file of safertos library.
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
ifeq ($(safertos_component_make_include), )

safertos_BOARDLIST       = tpr12_evm awr294x_evm j721e_evm j7200_evm j721s2_evm j784s4_evm
safertos_SOCLIST         = tpr12 awr294x j721e j7200 j721s2 j784s4
safertos_j721e_CORELIST  = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
safertos_j7200_CORELIST  = mcu1_0 mcu1_1 mcu2_0 mcu2_1
safertos_j721s2_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
safertos_j784s4_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
safertos_tpr12_CORELIST  = c66xdsp_1
safertos_awr294x_CORELIST  = c66xdsp_1

############################
# safertos package
# List of components included under safertos lib
# The components included here are built and will be part of safertos lib
############################
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safertos_LIB_LIST = safertos
endif


############################
# safertos examples
# List of examples under safertos
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
safertos_EXAMPLE_LIST =

ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
ifeq ($(SOC),$(filter $(SOC), $(safertos_SOCLIST)))
safertos_EXAMPLE_LIST =  task_switch_safertos
safertos_EXAMPLE_LIST +=  unit_test_safertos
# safertos_EXAMPLE_LIST +=  safertos_demo 
endif
endif

#
# safertos Modules
#

# safertos LIB
safertos_COMP_LIST = safertos
safertos_RELPATH = ti/kernel/safertos
safertos_PATH = $(PDK_SAFERTOS_COMP_PATH)
safertos_LIBNAME = ti.kernel.safertos
export safertos_LIBNAME
safertos_LIBPATH = $(safertos_PATH)/lib
export safertos_LIBPATH
safertos_OBJPATH = $(safertos_RELPATH)
export safertos_OBJPATH
safertos_MAKEFILE = -f safertos.mak
export safertos_MAKEFILE
safertos_BOARD_DEPENDENCY = no
safertos_CORE_DEPENDENCY = yes
safertos_SOC_DEPENDENCY = yes
export safertos_COMP_LIST
export safertos_BOARD_DEPENDENCY
export safertos_CORE_DEPENDENCY
export safertos_SOC_DEPENDENCY
safertos_PKG_LIST = safertos
export safertos_PKG_LIST
safertos_INCLUDE = ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/checkpoints
safertos_INCLUDE +=  ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT_$(ISA))
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/kernel/include_api
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))/$(SAFERTOS_COMPILER_EXT_$(ISA))
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/api/NoWrapper
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/kernel/include_prv
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/config
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/runtimestats
safertos_INCLUDE += ${SAFERTOS_KERNEL_INSTALL_PATH_${ISA}}/source_code_and_projects/SafeRTOS/runtimestats/PrivWrapperStd
safertos_INCLUDE += $(PDK_SAFERTOS_COMP_PATH)/TI_CGT/c7x
safertos_INCLUDE += $(PDK_CSL_COMP_PATH)/arch/c7x

export safertos_SOCLIST
export safertos_$(SOC)_CORELIST



#
# safertos Examples
#
# safertos rtos baremetal test app

# safertos rtos task switch test app
task_switch_safertos_COMP_LIST = task_switch_safertos
task_switch_safertos_RELPATH = ti/kernel/safertos
task_switch_safertos_PATH = $(PDK_FREERTOS_COMP_PATH)/test/safertos/task_switch
task_switch_safertos_BOARD_DEPENDENCY = yes
task_switch_safertos_CORE_DEPENDENCY = no
task_switch_safertos_XDC_CONFIGURO = no
task_switch_safertos_MAKEFILE = -f makefile BAREMETAL=yes
export task_switch_safertos_COMP_LIST
export task_switch_safertos_BOARD_DEPENDENCY
export task_switch_safertos_CORE_DEPENDENCY
export task_switch_safertos_XDC_CONFIGURO
export task_switch_safertos_MAKEFILE
task_switch_safertos_PKG_LIST = task_switch_safertos
export task_switch_safertos_PKG_LIST
task_switch_safertos_INCLUDE = $(task_switch_safertos_PATH)
task_switch_safertos_BOARDLIST = $(safertos_BOARDLIST)
export task_switch_safertos_BOARDLIST
task_switch_safertos_$(SOC)_CORELIST = $(safertos_$(SOC)_CORELIST)
export task_switch_safertos_$(SOC)_CORELIST
export task_switch_safertos_SBL_APPIMAGEGEN = yes

# safertos rtos unit test app
unit_test_safertos_COMP_LIST = unit_test_safertos
unit_test_safertos_RELPATH = ti/kernel/safertos/
unit_test_safertos_PATH = $(PDK_FREERTOS_COMP_PATH)/test/safertos/ut
unit_test_safertos_BOARD_DEPENDENCY = yes
unit_test_safertos_CORE_DEPENDENCY = no
unit_test_safertos_XDC_CONFIGURO = no
unit_test_safertos_MAKEFILE = -f makefile BAREMETAL=yes
export unit_test_safertos_COMP_LIST
export unit_test_safertos_BOARD_DEPENDENCY
export unit_test_safertos_CORE_DEPENDENCY
export unit_test_safertos_XDC_CONFIGURO
export unit_test_safertos_MAKEFILE
unit_test_safertos_PKG_LIST = unit_test_safertos
export unit_test_safertos_PKG_LIST
unit_test_safertos_INCLUDE = $(unit_test_safertos_PATH)
unit_test_safertos_BOARDLIST = $(safertos_BOARDLIST)
export unit_test_safertos_BOARDLIST
unit_test_safertos_$(SOC)_CORELIST = $(safertos_$(SOC)_CORELIST)
export unit_test_safertos_$(SOC)_CORELIST
export unit_test_safertos_SBL_APPIMAGEGEN = yes

# safertos demo
safertos_demo_COMP_LIST = safertos_demo
safertos_demo_RELPATH = ti/kernel/test/safertos/safertos_demo
safertos_demo_PATH = $(PDK_SAFERTOS_COMP_PATH)/../test/safertos/safertos_demo
safertos_demo_BOARD_DEPENDENCY = yes
safertos_demo_CORE_DEPENDENCY = no
safertos_demo_XDC_CONFIGURO = no
safertos_demo_MAKEFILE = -f makefile BAREMETAL=yes
export safertos_demo_COMP_LIST
export safertos_demo_BOARD_DEPENDENCY
export safertos_demo_CORE_DEPENDENCY
export safertos_demo_XDC_CONFIGURO
export safertos_demo_MAKEFILE
safertos_demo_PKG_LIST = safertos_demo
export safertos_demo_PKG_LIST
safertos_demo_INCLUDE = $(safertos_demo_PATH)
safertos_demo_BOARDLIST = $(safertos_BOARDLIST)
export safertos_demo_BOARDLIST
safertos_demo_$(SOC)_CORELIST = $(safertos_$(SOC)_CORELIST)
export safertos_demo_$(SOC)_CORELIST

export safertos_LIB_LIST
export safertos_EXAMPLE_LIST

safertos_component_make_include := 1
endif
