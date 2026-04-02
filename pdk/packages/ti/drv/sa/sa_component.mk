#
# Copyright (c) 2017-2020, Texas Instruments Incorporated
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

# File: sa_component.mk
#       This file is component include make file of SA library.
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
ifeq ($(sa_component_make_include), )

# List with various rtos_types(such as tirtos(sysbios),freertos,safertos) to build RTOS apps. 
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
drvsa_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

# under other list
drvsa_BOARDLIST       = am65xx_evm am65xx_idk j721e_evm am64x_evm
drvsa_SOCLIST         = am65xx j721e am64x
drvsa_am65xx_CORELIST = mcu1_0
drvsa_j721e_CORELIST  = mcu1_0
drvsa_am64x_CORELIST  = mcu1_0

# Define the rule to generate SA Drivers BOARDLIST for each rtos_type
# Default BOARDLIST for each rtos_type is defined in 'ti/build/makerules/component.mk'
# The following rule filters out SA Drivers BOARDLIST for each rtos_type.
# Here $(1) refers tot the first argument passed to the rule.
# In this case it is $(curos), each instance in "drvsa_RTOS_LIST" (ie, tirtos/freertos/safertos..)
define DRV_SA_BOARDLIST_RULE

drvsa_$(1)_BOARDLIST     = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvsa_BOARDLIST))

endef

# Define the macro list with rules of all rtos_types
DRV_SA_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvsa_RTOS_LIST), $(call DRV_SA_BOARDLIST_RULE,$(curos)))

# Evaluate the macro list to generate BOARDLIST for all rtos_types
$(eval ${DRV_SA_BOARDLIST_MACRO_LIST})

############################
# sa package
# List of components included under sa lib
# The components included here are built and will be part of sa lib
############################
sa_LIB_LIST = salite2
drvsa_LIB_LIST = $(sa_LIB_LIST)

############################
# sa examples
# List of examples under sa
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
sa_EXAMPLE_LIST  =
drvsa_EXAMPLE_LIST = $(sa_EXAMPLE_LIST)

#
# sa Modules
#

# sa lite2 LIB
salite2_COMP_LIST = salite2
salite2_RELPATH = ti/drv/sa
salite2_PATH = $(PDK_SA_COMP_PATH)
salite2_LIBNAME = ti.drv.salite2
export salite2_LIBNAME
salite2_LIBPATH = $(salite2_PATH)/lib
export salite2_LIBPATH
salite2_OBJPATH = $(salite2_RELPATH)/salite2
export salite2_OBJPATH
salite2_MAKEFILE = -f build/makefile_lite2.mk
export salite2_MAKEFILE
salite2_BOARD_DEPENDENCY = no
salite2_CORE_DEPENDENCY = no
salite2_SOC_DEPENDENCY = no
export salite2_COMP_LIST
export salite2_BOARD_DEPENDENCY
export salite2_CORE_DEPENDENCY
export salite2_SOC_DEPENDENCY
salite2_PKG_LIST = salite2
salite2_INCLUDE = $(salite2_PATH)
salite2_SOCLIST = am65xx j721e am64x
export salite2_SOCLIST
salite2_$(SOC)_CORELIST = $(drvsa_$(SOC)_CORELIST)
export salite2_$(SOC)_CORELIST

#
# sa Baremetal Unit Test
#
# sa Polling mode Test app
SA_Baremetal_TestApp_COMP_LIST = SA_Baremetal_TestApp
SA_Baremetal_TestApp_RELPATH = ti/drv/sa/test/SaUnitTest
SA_Baremetal_TestApp_PATH = $(PDK_SA_COMP_PATH)/test/SaUnitTest
SA_Baremetal_TestApp_BOARD_DEPENDENCY = yes
SA_Baremetal_TestApp_CORE_DEPENDENCY = no
SA_Baremetal_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export SA_Baremetal_TestApp_MAKEFILE
# SA_Baremetal_TestApp_XDC_CONFIGURO = yes
export SA_Baremetal_TestApp_COMP_LIST
export SA_Baremetal_TestApp_BOARD_DEPENDENCY
export SA_Baremetal_TestApp_CORE_DEPENDENCY
# export SA_Baremetal_TestApp_XDC_CONFIGURO
SA_Baremetal_TestApp_PKG_LIST = SA_Baremetal_TestApp
SA_Baremetal_TestApp_INCLUDE = $(SA_Baremetal_TestApp_PATH)
SA_Baremetal_TestApp_BOARDLIST = $(drvsa_BOARDLIST)
export SA_Baremetal_TestApp_BOARDLIST
export SA_Baremetal_TestApp_SBL_APPIMAGEGEN = yes
SA_Baremetal_TestApp_$(SOC)_CORELIST = $(drvsa_$(SOC)_CORELIST)
export SA_Baremetal_TestApp_$(SOC)_CORELIST

sa_EXAMPLE_LIST  += SA_Baremetal_TestApp

#
# sa RTOS Unit Test
#
# sa Polling mode Test app
# RTOS SA unit test app
define SA_UNIT_TESTAPP_RULE

export SA_TestApp_$(1)_COMP_LIST = SA_TestApp_$(1)
SA_TestApp_$(1)_RELPATH = ti/drv/sa/test/SaUnitTest
SA_TestApp_$(1)_PATH = $(PDK_SA_COMP_PATH)/test/SaUnitTest
export SA_TestApp_$(1)_BOARD_DEPENDENCY = yes
export SA_TestApp_$(1)_CORE_DEPENDENCY = no
export SA_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export SA_TestApp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
SA_TestApp_$(1)_PKG_LIST = SA_TestApp_$(1)
SA_TestApp_$(1)_INCLUDE = $(SA_TestApp_$(1)_PATH)
export SA_TestApp_$(1)_BOARDLIST = $(drvsa_$(1)_BOARDLIST)
export SA_TestApp_$(1)_SBL_APPIMAGEGEN = yes
export SA_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvsa_$(SOC)_CORELIST))

ifneq ($(1),$(filter $(1), safertos))
sa_EXAMPLE_LIST += SA_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
sa_EXAMPLE_LIST += SA_TestApp_$(1)
endif
endif

endef

SA_UNIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvsa_RTOS_LIST), $(call SA_UNIT_TESTAPP_RULE,$(curos)))
$(eval ${SA_UNIT_TESTAPP_MACRO_LIST})

export drvsa_LIB_LIST
export sa_LIB_LIST
export sa_EXAMPLE_LIST
export drvsa_EXAMPLE_LIST

sa_component_make_include := 1
endif
