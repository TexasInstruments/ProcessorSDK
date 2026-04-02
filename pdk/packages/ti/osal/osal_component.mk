#
# Copyright (c) 2016-2021, Texas Instruments Incorporated
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

# File: osal_component.mk
#       This file is component include make file of OSAL library.
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
#                             this module has no baord dependent code and hence
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
ifeq ($(osal_component_make_include), )

libosal_RTOS_LIST = $(DEFAULT_RTOS_LIST)

libosal_BOARDLIST  = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm

libosal_SOCLIST   =  j721e j7200 j721s2 j784s4 j742s2

libosal_freertos_BOARDLIST  = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
libosal_freertos_SOCLIST    = j721e j7200 j721s2 j784s4 j742s2
libosal_safertos_BOARDLIST  = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
libosal_safertos_SOCLIST    = j721e j7200 j721s2 j784s4 j742s2


ifeq ($(SOC),$(filter $(SOC), $(libosal_SOCLIST)))
libosal_$(SOC)_CORELIST   = $(DEFAULT_$(SOC)_CORELIST)
libosal_freertos_$(SOC)_CORELIST = $(filter-out mpu%, $(libosal_$(SOC)_CORELIST))
endif

libosal_safertos_j721e_CORELIST   = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
libosal_safertos_j7200_CORELIST   = mcu1_0 mcu1_1 mcu2_0 mcu2_1
libosal_safertos_j721s2_CORELIST   = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
libosal_safertos_j784s4_CORELIST   = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
libosal_safertos_j742s2_CORELIST   = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3

############################
# osal package
# List of components included under osal lib
# The components included here are built and will be part of osal lib
############################
osal_LIB_LIST = osal_nonos osal_nonos_indp
osal_LIB_LIST += osal_freertos
ifeq ($(SOC),$(filter $(SOC), $(libosal_safertos_SOCLIST)))
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
osal_LIB_LIST += osal_safertos
endif
endif

libosal_LIB_LIST = $(osal_LIB_LIST)

############################
# osal examples
# List of examples under osal
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################

osal_EXAMPLE_LIST =

#
# OSAL Modules
#

# OSAL NONOS LIB
export osal_nonos_COMP_LIST = osal_nonos
osal_nonos_RELPATH = ti/osal
osal_nonos_PATH = $(PDK_OSAL_COMP_PATH)
export osal_nonos_LIBNAME = ti.osal
export osal_nonos_LIBPATH = $(osal_nonos_PATH)/lib/nonos
export osal_nonos_OBJPATH = $(osal_nonos_RELPATH)/osal_nonos
export osal_nonos_MAKEFILE = -f build/makefile_nonos.mk
export osal_nonos_PLATFORM_DEPENDENCY = no
export osal_nonos_CORE_DEPENDENCY = no
export osal_nonos_SOC_DEPENDENCY = yes
export osal_nonos_PKG_LIST = osal_nonos
osal_nonos_INCLUDE = $(osal_nonos_PATH)
export osal_nonos_SOCLIST = $(libosal_SOCLIST)
osal_nonos_$(SOC)_CORELIST = $(libosal_$(SOC)_CORELIST)
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
osal_nonos_$(SOC)_CORELIST += c7x-hostemu
endif
export osal_nonos_$(SOC)_CORELIST

# OSAL FREE RTOS LIB
export osal_freertos_COMP_LIST = osal_freertos
osal_freertos_RELPATH = ti/osal
osal_freertos_PATH = $(PDK_OSAL_COMP_PATH)
export osal_freertos_LIBNAME = ti.osal
export osal_freertos_LIBPATH = $(osal_freertos_PATH)/lib/freertos
export osal_freertos_OBJPATH = $(osal_freertos_RELPATH)/osal_freertos
export osal_freertos_MAKEFILE = -f build/makefile_freertos.mk
export osal_freertos_PLATFORM_DEPENDENCY = no
export osal_freertos_CORE_DEPENDENCY = no
export osal_freertos_SOC_DEPENDENCY = yes
export osal_freertos_PKG_LIST = osal_freertos
osal_freertos_INCLUDE = $(osal_freertos_PATH)
export osal_freertos_SOCLIST = $(libosal_freertos_SOCLIST)
export osal_freertos_$(SOC)_CORELIST = $(libosal_freertos_$(SOC)_CORELIST)

# OSAL SAFE RTOS LIB
export osal_safertos_COMP_LIST = osal_safertos
osal_safertos_RELPATH = ti/osal
osal_safertos_PATH = $(PDK_OSAL_COMP_PATH)
export osal_safertos_LIBNAME = ti.osal
export osal_safertos_LIBPATH = $(osal_safertos_PATH)/lib/safertos
export osal_safertos_OBJPATH = $(osal_safertos_RELPATH)/osal_safertos
export osal_safertos_MAKEFILE = -f build/makefile_safertos.mk
export osal_safertos_PLATFORM_DEPENDENCY = no
export osal_safertos_CORE_DEPENDENCY = no
export osal_safertos_SOC_DEPENDENCY = yes
export osal_safertos_PKG_LIST = osal_safertos
osal_safertos_INCLUDE = $(osal_safertos_PATH)
export osal_safertos_SOCLIST = $(libosal_safertos_SOCLIST)
export osal_safertos_$(SOC)_CORELIST = $(libosal_safertos_$(SOC)_CORELIST)

#
# OSAL Examples
#
export osal_baremetal_testapp_COMP_LIST = osal_baremetal_testapp
osal_baremetal_testapp_RELPATH = ti/osal/test/baremetal
osal_baremetal_testapp_PATH = $(PDK_OSAL_COMP_PATH)/test/baremetal
export osal_baremetal_testapp_BOARD_DEPENDENCY = yes
export osal_baremetal_testapp_CORE_DEPENDENCY = no
osal_baremetal_testapp_PKG_LIST = osal_baremetal_testapp
osal_baremetal_testapp_INCLUDE = $(osal_baremetal_testapp_PATH)
export osal_baremetal_testapp_BOARDLIST = $(libosal_BOARDLIST)
export osal_baremetal_testapp_SBL_APPIMAGEGEN = yes
osal_EXAMPLE_LIST += osal_baremetal_testapp

#OSAL extended test app for ensuring driver coverage
export osal_baremetal_extended_testapp_COMP_LIST = osal_baremetal_extended_testapp
osal_baremetal_extended_testapp_RELPATH = ti/osal/test/osal_extended_testapp
osal_baremetal_extended_testapp_PATH = $(PDK_OSAL_COMP_PATH)/test/osal_extended_testapp
export osal_baremetal_extended_testapp_MAKEFILE = -f makefile_baremetal.mk
export osal_baremetal_extended_testapp_BOARD_DEPENDENCY = yes
export osal_baremetal_extended_testapp_CORE_DEPENDENCY = no
osal_baremetal_extended_testapp_PKG_LIST = osal_baremetal_extended_testapp
osal_baremetal_extended_testapp_INCLUDE = $(osal_baremetal_extended_testapp_PATH)
export osal_baremetal_extended_testapp_BOARDLIST = $(libosal_BOARDLIST)
export osal_baremetal_extended_testapp_SBL_APPIMAGEGEN = yes
osal_EXAMPLE_LIST += osal_baremetal_extended_testapp

define osal_testapp_RULE

export osal_testapp_$(1)_COMP_LIST = osal_testapp_$(1)
export osal_testapp_$(1)_RELPATH = ti/osal/test/os_unit_test
export osal_testapp_$(1)_PATH = $(PDK_OSAL_COMP_PATH)/test/os_unit_test
export osal_testapp_$(1)_BOARD_DEPENDENCY = yes
export osal_testapp_$(1)_CORE_DEPENDENCY = no
export osal_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export osal_testapp_$(1)_PKG_LIST = osal_testapp_$(1)
export osal_testapp_$(1)_INCLUDE = $(osal_testapp_$(1)_PATH)
export osal_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(libosal_$(1)_BOARDLIST))
export osal_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(libosal_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
osal_EXAMPLE_LIST += osal_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
osal_EXAMPLE_LIST += osal_testapp_$(1)
endif
endif
export osal_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

osal_testapp_MACRO_LIST := $(foreach curos,$(libosal_RTOS_LIST) safertos,$(call osal_testapp_RULE,$(curos)))

$(eval ${osal_testapp_MACRO_LIST})

export osal_baremetal_cache_test_COMP_LIST = osal_baremetal_cache_test
export osal_baremetal_cache_test_RELPATH = ti/osal/test/osal_cache_test
export osal_baremetal_cache_test_PATH = $(PDK_OSAL_COMP_PATH)/test/osal_cache_test
export osal_baremetal_cache_test_BOARD_DEPENDENCY = yes
export osal_baremetal_cache_test_CORE_DEPENDENCY = yes
export osal_baremetal_cache_test_MAKEFILE = -f makefile
export osal_baremetal_cache_test_PKG_LIST = osal_baremetal_cache_test
export osal_baremetal_cache_test_INCLUDE = $(osal_baremetal_cache_test_PATH)
export osal_baremetal_cache_test_BOARDLIST = $(libosal_BOARDLIST)
export osal_baremetal_cache_test_SBL_APPIMAGEGEN = yes
osal_EXAMPLE_LIST += osal_baremetal_cache_test

export osal_cache_test_multicore_COMP_LIST = osal_cache_test_multicore
export osal_cache_test_multicore_RELPATH = ti/osal/test/osal_cache_test
export osal_cache_test_multicore_PATH = $(PDK_OSAL_COMP_PATH)/test/osal_cache_test
export osal_cache_test_multicore_BOARD_DEPENDENCY = yes
export osal_cache_test_multicore_CORE_DEPENDENCY = yes
export osal_cache_test_multicore_MAKEFILE = -f multicore_makefile.mk BUILD_PROFILE=$(BUILD_PROFILE)
export osal_cache_test_multicore_BOARDLIST = $(libosal_BOARDLIST)
export osal_cache_test_multicore_SBL_APPIMAGEGEN = no
osal_EXAMPLE_LIST += osal_cache_test_multicore

#OSAL extended test app for ensuring driver coverage
define osal_extended_testapp_RULE

export osal_extended_testapp_$(1)_COMP_LIST = osal_extended_testapp_$(1)
export osal_extended_testapp_$(1)_PATH = $(PDK_OSAL_COMP_PATH)/test/osal_extended_testapp
export osal_extended_testapp_$(1)_RELPATH = ti/osal/test/osal_extended_testapp
export osal_extended_testapp_$(1)_MAKEFILE = -f makefile_rtos.mk BUILD_OS_TYPE=$(1)
export osal_extended_testapp_$(1)_BOARD_DEPENDENCY = yes
export osal_extended_testapp_$(1)_CORE_DEPENDENCY = no
export osal_extended_testapp_$(1)_PKG_LIST = osal_extended_testapp_$(1)
export osal_extended_testapp_$(1)_INCLUDE = $(osal_extended_testapp_$(1)_PATH)
export osal_extended_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(libosal_$(1)_BOARDLIST))
export osal_extended_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(libosal_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
osal_EXAMPLE_LIST += osal_extended_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
osal_EXAMPLE_LIST += osal_extended_testapp_$(1)
endif
endif
export osal_extended_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef
osal_extended_testapp_MACRO_LIST := $(foreach curos,$(libosal_RTOS_LIST) safertos,$(call osal_extended_testapp_RULE,$(curos)))

$(eval ${osal_extended_testapp_MACRO_LIST})

define osal_exception_testapp_RULE

export osal_exception_testapp_$(1)_COMP_LIST = osal_exception_testapp_$(1)
export osal_exception_testapp_$(1)_RELPATH = ti/osal/test/osal_exception_testapp
export osal_exception_testapp_$(1)_PATH = $(PDK_OSAL_COMP_PATH)/test/osal_exception_testapp
export osal_exception_testapp_$(1)_BOARD_DEPENDENCY = yes
export osal_exception_testapp_$(1)_CORE_DEPENDENCY = no
export osal_exception_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export osal_exception_testapp_$(1)_PKG_LIST = osal_exception_testapp_$(1)
export osal_exception_testapp_$(1)_INCLUDE = $(osal_exception_testapp_$(1)_PATH)
export osal_exception_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(libosal_$(1)_BOARDLIST))
export osal_exception_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4)
ifneq ($(1),$(filter $(1), safertos))
osal_EXAMPLE_LIST += osal_exception_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
osal_EXAMPLE_LIST += osal_exception_testapp_$(1)
endif
endif
export osal_exception_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

osal_exception_testapp_MACRO_LIST := $(foreach curos,$(libosal_RTOS_LIST) safertos,$(call osal_exception_testapp_RULE,$(curos)))

$(eval ${osal_exception_testapp_MACRO_LIST})

ifeq ($(SOC),$(filter $(SOC), j721e))
 osal_baremetal_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu3_0
 osal_baremetal_cache_test_$(SOC)_CORELIST = mcu2_0 mcu2_1
 osal_cache_test_multicore_$(SOC)_CORELIST = mpu1_1
 osal_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
 osal_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
 osal_baremetal_extended_testapp_$(SOC)_CORELIST = mcu1_0 mcu2_0 mcu3_0
 osal_extended_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
 osal_extended_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
endif

ifeq ($(SOC),$(filter $(SOC), j7200))
 osal_baremetal_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0
 osal_baremetal_cache_test_$(SOC)_CORELIST = mcu2_0 mcu2_1
 osal_cache_test_multicore_$(SOC)_CORELIST = mpu1_1
 osal_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1
 osal_baremetal_extended_testapp_$(SOC)_CORELIST= mcu1_0 mcu2_0
 osal_extended_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1
 osal_extended_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif

ifeq ($(SOC),$(filter $(SOC), j721s2))
 osal_baremetal_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu3_0
 osal_baremetal_cache_test_$(SOC)_CORELIST = mcu2_0 mcu2_1
 osal_cache_test_multicore_$(SOC)_CORELIST = mpu1_1
 osal_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
 osal_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
 osal_baremetal_extended_testapp_$(SOC)_CORELIST = mcu1_0 mcu2_0 mcu3_0
 osal_extended_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
 osal_extended_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
endif

ifeq ($(SOC),$(filter $(SOC), j784s4))
 osal_baremetal_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu3_0 mcu4_0
 osal_baremetal_cache_test_$(SOC)_CORELIST = mcu2_0 mcu2_1
 osal_cache_test_multicore_$(SOC)_CORELIST = mpu1_1
 osal_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
 osal_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
 osal_baremetal_extended_testapp_$(SOC)_CORELIST = mcu1_0 mcu2_0 mcu3_0 mcu4_0
 osal_extended_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
 osal_extended_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
endif

ifeq ($(SOC),$(filter $(SOC), j742s2))
 osal_baremetal_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu3_0 mcu4_0
 osal_baremetal_cache_test_$(SOC)_CORELIST = mcu2_0 mcu2_1
 osal_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3
 osal_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3
 osal_baremetal_extended_testapp_$(SOC)_CORELIST = mcu1_0 mcu2_0 mcu3_0 mcu4_0
 osal_extended_testapp_freertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3
 osal_extended_testapp_safertos_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3
endif

export OSAL_Baremetal_TestApp_$(SOC)_CORELIST
export OSAL_freertos_TestApp_$(SOC)_CORELIST
export osal_baremetal_cache_test_$(SOC)_CORELIST
export osal_cache_test_multicore_$(SOC)_CORELIST
export osal_baremetal_extended_testapp_$(SOC)_CORELIST
export osal_extended_testapp_freertos_$(SOC)_CORELIST

export osal_LIB_LIST
export libosal_LIB_LIST
export osal_EXAMPLE_LIST

export libosal_EXAMPLE_LIST = $(osal_EXAMPLE_LIST)

osal_component_make_include := 1
endif
