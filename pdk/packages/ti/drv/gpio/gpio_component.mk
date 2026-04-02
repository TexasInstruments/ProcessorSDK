#
# Copyright (c) 2016-2022, Texas Instruments Incorporated
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

# File: gpio_component.mk
#       This file is component include make file of GPIO rtosrary.
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
ifeq ($(gpio_component_make_include), )

drvgpio_RTOS_LIST = $(DEFAULT_RTOS_LIST)
# under other list
drvgpio_BOARDLIST       = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm

drvgpio_SOCLIST        += j721e j7200 j721s2 j784s4 j742s2

drvgpio_j721e_CORELIST     = $(DEFAULT_j721e_CORELIST)
drvgpio_j721e_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvgpio_j7200_CORELIST     = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvgpio_j7200_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvgpio_j721s2_CORELIST    = $(DEFAULT_j721s2_CORELIST)
drvgpio_j721s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvgpio_j784s4_CORELIST    = $(DEFAULT_j784s4_CORELIST)
drvgpio_j784s4_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvgpio_j742s2_CORELIST    = $(DEFAULT_j742s2_CORELIST)
drvgpio_j742s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1

############################
# gpio package
# List of components included under gpio rtos
# The components included here are built and will be part of gpio rtos
############################
gpio_LIB_LIST = gpio
drvgpio_LIB_LIST = $(gpio_LIB_LIST)

############################
# gpio examples
# List of examples under gpio
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
gpio_EXAMPLE_LIST = GPIO_Baremetal_LedBlink_TestApp

drvgpio_EXAMPLE_LIST = $(gpio_EXAMPLE_LIST)

#
# GPIO Modules
#

# GPIO LIB
export gpio_COMP_LIST = gpio
gpio_RELPATH = ti/drv/gpio
gpio_PATH = $(PDK_GPIO_COMP_PATH)
export gpio_LIBNAME = ti.drv.gpio
export gpio_LIBPATH = $(gpio_PATH)/lib
export gpio_OBJPATH = $(gpio_RELPATH)/gpio
export gpio_MAKEFILE = -f build/makefile.mk
export gpio_BOARD_DEPENDENCY = no
export gpio_CORE_DEPENDENCY = no
export gpio_SOC_DEPENDENCY = yes
export gpio_PKG_LIST = gpio
gpio_INCLUDE = $(gpio_PATH)
export gpio_SOCLIST = $(drvgpio_SOCLIST)
export gpio_$(SOC)_CORELIST = $(drvgpio_$(SOC)_CORELIST)

#
# GPIO Examples
#

# GPIO baremetal Example led blink
export GPIO_Baremetal_LedBlink_TestApp_COMP_LIST = GPIO_Baremetal_LedBlink_TestApp
GPIO_Baremetal_LedBlink_TestApp_RELPATH = ti/drv/gpio/test/led_blink
GPIO_Baremetal_LedBlink_TestApp_PATH = $(PDK_GPIO_COMP_PATH)/test/led_blink
export GPIO_Baremetal_LedBlink_TestApp_BOARD_DEPENDENCY = yes
export GPIO_Baremetal_LedBlink_TestApp_CORE_DEPENDENCY = no
export GPIO_Baremetal_LedBlink_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
GPIO_Baremetal_LedBlink_TestApp_PKG_LIST = GPIO_Baremetal_LedBlink_TestApp
GPIO_Baremetal_LedBlink_TestApp_INCLUDE = $(GPIO_Baremetal_LedBlink_TestApp_PATH)
export GPIO_Baremetal_LedBlink_TestApp_BOARDLIST = $(drvgpio_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e))
# J721e:- There are no IR path from WKUP_GPIO to mcu3_0/mcu3_1.
export GPIO_Baremetal_LedBlink_TestApp_$(SOC)_CORELIST = $(filter $(drvgpio_$(SOC)_CORELISTARM), mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1)
else ifeq ($(SOC),$(filter $(SOC), j7200))
# J7200:- There are no IR path from WKUP_GPIO to mcu2_0/mcu2_1.
export GPIO_Baremetal_LedBlink_TestApp_$(SOC)_CORELIST = $(filter $(drvgpio_$(SOC)_CORELISTARM), mpu1_0 mcu1_0 mcu1_1)
else ifeq ($(SOC),$(filter $(SOC), j721s2))
# J721S2:- There is no IR path from WKUP_GPIO to mcu3_0/mcu3_1 (no WKUP_GPIO IR allocations)
export GPIO_Baremetal_LedBlink_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
else ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
# J784S4:- There is no IR path from WKUP_GPIO to mcu3_0/mcu3_1/mcu4_0/mcu4_1 (no WKUP_GPIO IR allocations)
export GPIO_Baremetal_LedBlink_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
export GPIO_Baremetal_LedBlink_TestApp_SBL_APPIMAGEGEN = yes
endif

# GPIO rtos Example led blink
define GPIO_LedBlink_TestApp_RULE

export GPIO_LedBlink_TestApp_$(1)_COMP_LIST = GPIO_LedBlink_TestApp_$(1)
export GPIO_LedBlink_TestApp_$(1)_RELPATH = ti/drv/gpio/test/led_blink
export GPIO_LedBlink_TestApp_$(1)_PATH = $(PDK_GPIO_COMP_PATH)/test/led_blink
export GPIO_LedBlink_TestApp_$(1)_BOARD_DEPENDENCY = yes
export GPIO_LedBlink_TestApp_$(1)_CORE_DEPENDENCY = no
export GPIO_LedBlink_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export GPIO_LedBlink_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export GPIO_LedBlink_TestApp_$(1)_PKG_LIST = GPIO_LedBlink_TestApp_$(1)
export GPIO_LedBlink_TestApp_$(1)_INCLUDE = $(GPIO_LedBlink_TestApp_$(1)_PATH)
export GPIO_LedBlink_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvgpio_BOARDLIST))
export GPIO_LedBlink_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvgpio_$(SOC)_CORELIST))
ifeq ($(SOC),$(filter $(SOC), j721e))
    # J721e:- There are no IR path from WKUP_GPIO to mcu3_0/mcu3_1/c66 cores.
    export GPIO_LedBlink_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu1_0 mcu1_1 mcu2_0 mcu2_1)
else ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
    # J721S2:- There is not IR path from WKUP_GPIO to mcu3_0/mcu3_1/c7x_1/c7x_2 (no WKUP_GPIO IR allocations)
    export GPIO_LedBlink_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1)
else ifeq ($(SOC),$(filter $(SOC), j7200))
    # J7200:- There are no IR path from WKUP_GPIO to mcu2_0/mcu2_1.
    export GPIO_LedBlink_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu1_1)
endif
ifneq ($(1),$(filter $(1), safertos))
    gpio_EXAMPLE_LIST += GPIO_LedBlink_TestApp_$(1)
else
    ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
        gpio_EXAMPLE_LIST += GPIO_LedBlink_TestApp_$(1)
    endif
endif
export GPIO_LedBlink_TestApp_$(1)_SBL_APPIMAGEGEN = yes

endef

GPIO_LedBlink_TestApp_MACRO_LIST := $(foreach curos,$(drvgpio_RTOS_LIST),$(call GPIO_LedBlink_TestApp_RULE,$(curos)))

$(eval ${GPIO_LedBlink_TestApp_MACRO_LIST})

export drvgpio_LIB_LIST
export drvgpio_EXAMPLE_LIST
export gpio_LIB_LIST
export gpio_EXAMPLE_LIST

gpio_component_make_include := 1
endif
