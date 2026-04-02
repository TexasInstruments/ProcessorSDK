#
# Copyright (c) 2017 - 2022, Texas Instruments Incorporated
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

# File: uart_component.mk
#       This file is component include make file of UART library.
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
ifeq ($(uart_component_make_include), )

# under other list
drvuart_RTOS_LIST 		= $(DEFAULT_RTOS_LIST)

drvuart_BOARDLIST       = am65xx_evm am65xx_idk j721e_sim j721e_evm j7200_evm am64x_evm
drvuart_BOARDLIST      += tpr12_evm tpr12_qt awr294x_evm j721s2_evm j784s4_evm j742s2_evm

drvuart_SOCLIST         = tda2xx tda2px tda2ex tda3xx dra78x am574x am572x am571x dra72x
drvuart_SOCLIST        += dra75x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x omapl137
drvuart_SOCLIST        += omapl138 am65xx j721e j7200 am64x tpr12 awr294x j721s2 j784s4 j742s2

drvuart_dma_SOCLIST     = am574x am572x am571x dra72x dra75x dra78x k2h k2k k2l k2e k2g
drvuart_dma_SOCLIST    += c6678 c6657 omapl137 omapl138 am437x am65xx j721e j7200 am335x
drvuart_dma_SOCLIST    += am64x tpr12 awr294x j721s2 j784s4 j742s2

drvuart_tda2xx_CORELIST = ipu1_0
drvuart_tda2px_CORELIST = ipu1_0
drvuart_tda2ex_CORELIST = ipu1_0
drvuart_tda3xx_CORELIST = ipu1_0
drvuart_dra78x_CORELIST = c66x ipu1_0
drvuart_am574x_CORELIST = c66x a15_0 ipu1_0
drvuart_am572x_CORELIST = c66x a15_0 ipu1_0
drvuart_am571x_CORELIST = c66x a15_0 ipu1_0
drvuart_dra72x_CORELIST = c66x a15_0 ipu1_0
drvuart_dra75x_CORELIST = c66x a15_0 ipu1_0
drvuart_k2h_CORELIST    = c66x a15_0
drvuart_k2k_CORELIST    = c66x a15_0
drvuart_k2l_CORELIST    = c66x a15_0
drvuart_k2e_CORELIST    = c66x a15_0
drvuart_k2g_CORELIST    = c66x a15_0
drvuart_omapl137_CORELIST    = c674x arm9_0
drvuart_omapl138_CORELIST    = c674x arm9_0
drvuart_c6678_CORELIST  = c66x
drvuart_c6657_CORELIST  = c66x
drvuart_am437x_CORELIST = a9host
drvuart_am335x_CORELIST = a8host pru_0 pru_1

drvuart_am65xx_CORELIST = mpu1_0 mcu1_0 mcu1_1
drvuart_j721e_CORELIST  = $(DEFAULT_j721e_CORELIST)
drvuart_j7200_CORELIST  = $(DEFAULT_j7200_CORELIST)
drvuart_am64x_CORELIST  = $(DEFAULT_am64x_CORELIST)
drvuart_j721s2_CORELIST = $(DEFAULT_j721s2_CORELIST)
drvuart_j784s4_CORELIST = $(DEFAULT_j784s4_CORELIST)
drvuart_j742s2_CORELIST = $(DEFAULT_j742s2_CORELIST)
drvuart_tpr12_CORELIST   = mcu1_0 c66xdsp_1
drvuart_awr294x_CORELIST = mcu1_0 c66xdsp_1
drvuart_j721e_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvuart_j7200_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvuart_am64x_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvuart_j721s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvuart_j784s4_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvuart_j742s2_CORELISTARM = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1


############################
# uart package
# List of components included under uart lib
# The components included here are built and will be part of uart lib
############################
uart_LIB_LIST = uart uart_dma uart_indp uart_console
drvuart_LIB_LIST = $(uart_LIB_LIST)

############################
# uart Firmwares
# All the firmware mentioned in list are built when build target is called
# List below all firmware for allowed values
############################
uart_FIRM_LIST = icss_uart
drvuart_FIRM_LIST = $(uart_FIRM_LIST)

############################
# uart examples
# List of examples under uart
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
#uart_EXAMPLE_LIST = drv_uart_unit_test drv_uart_polling_mode_app drv_uart_intr_mode_app
uart_EXAMPLE_LIST = UART_Baremetal_TestApp UART_Baremetal_DMA_TestApp UART_BasicExample_mcu_routetest

drvuart_EXAMPLE_LIST = $(uart_EXAMPLE_LIST)

#
# UART Modules
#

# UART LIB
export uart_COMP_LIST = uart
uart_RELPATH = ti/drv/uart
uart_PATH = $(PDK_UART_COMP_PATH)
export uart_LIBNAME = ti.drv.uart
export uart_LIBPATH = $(uart_PATH)/lib
export uart_OBJPATH = $(uart_RELPATH)/uart
export uart_MAKEFILE = -f build/makefile.mk
export uart_BOARD_DEPENDENCY = no
export uart_CORE_DEPENDENCY = no
export uart_SOC_DEPENDENCY = yes
uart_PKG_LIST = uart
uart_INCLUDE = $(uart_PATH)
export uart_SOCLIST = $(drvuart_SOCLIST)
export uart_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART LIB DEVICE INDEPENDENT
export uart_indp_COMP_LIST = uart_indp
uart_indp_RELPATH = ti/drv/uart
uart_indp_PATH = $(PDK_UART_COMP_PATH)
export uart_indp_LIBNAME = ti.drv.uart
export uart_indp_LIBPATH = $(uart_indp_PATH)/lib
export uart_indp_OBJPATH = $(uart_indp_RELPATH)/uart_indp
export uart_indp_MAKEFILE = -f build/makefile_indp.mk
export uart_indp_BOARD_DEPENDENCY = no
export uart_indp_CORE_DEPENDENCY = no
export uart_indp_SOC_DEPENDENCY = no
uart_indp_PKG_LIST = uart_indp
uart_indp_INCLUDE = $(uart_indp_PATH)
export uart_indp_SOCLIST = tda2xx tda2px tda2ex tda3xx dra78x am574x am572x am571x dra72x dra75x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x omapl137 omapl138
export uart_indp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART DMA SOC LIB
export uart_dma_COMP_LIST = uart_dma
uart_dma_RELPATH = ti/drv/uart
uart_dma_PATH = $(PDK_UART_COMP_PATH)
export uart_dma_LIBNAME = ti.drv.uart.dma
export uart_dma_LIBPATH = $(uart_dma_PATH)/lib
export uart_dma_OBJPATH = $(uart_dma_RELPATH)/uart_dma
export uart_dma_MAKEFILE = -f build/makefile_dma.mk
export uart_dma_BOARD_DEPENDENCY = no
export uart_dma_CORE_DEPENDENCY = no
export uart_dma_SOC_DEPENDENCY = yes
uart_dma_PKG_LIST = uart_dma
uart_dma_INCLUDE = $(uart_dma_PATH)
export uart_dma_SOCLIST = $(drvuart_dma_SOCLIST)
export uart_dma_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART PROFILING SOC LIB
export uart_profile_COMP_LIST = uart_profile
uart_profile_RELPATH = ti/drv/uart
uart_profile_PATH = $(PDK_UART_COMP_PATH)
export uart_profile_LIBNAME = ti.drv.uart.profiling
export uart_profile_LIBPATH = $(uart_profile_PATH)/lib
export uart_profile_OBJPATH = $(uart_profile_RELPATH)/uart_profile
export uart_profile_MAKEFILE = -f build/makefile_profile.mk
export uart_profile_BOARD_DEPENDENCY = no
export uart_profile_CORE_DEPENDENCY = no
export uart_profile_SOC_DEPENDENCY = yes
uart_profile_PKG_LIST = uart_profile
uart_profile_INCLUDE = $(uart_profile_PATH)
export uart_profile_SOCLIST = am574x am572x am571x dra72x dra75x dra78x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x omapl137 omapl138
export uart_profile_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART PROFILING SOC INDEPENDENT LIB
export uart_profile_indp_COMP_LIST = uart_profile_indp
uart_profile_indp_RELPATH = ti/drv/uart
uart_profile_indp_PATH = $(PDK_UART_COMP_PATH)
export uart_profile_indp_LIBNAME = ti.drv.uart.profiling
export uart_profile_indp_LIBPATH = $(uart_profile_indp_PATH)/lib
export uart_profile_indp_OBJPATH = $(uart_profile_indp_RELPATH)/uart_profile_indp
export uart_profile_indp_MAKEFILE = -f build/makefile_profile_indp.mk
export uart_profile_indp_BOARD_DEPENDENCY = no
export uart_profile_indp_CORE_DEPENDENCY = no
export uart_profile_indp_SOC_DEPENDENCY = no
uart_profile_indp_PKG_LIST = uart_profile_indp
uart_profile_indp_INCLUDE = $(uart_profile_indp_PATH)
export uart_profile_indp_SOCLIST = am574x am572x am571x dra72x dra75x dra78x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x omapl137 omapl138
export uart_profile_indp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART DMA PROFILING SOC LIB
export uart_dma_profile_COMP_LIST = uart_dma_profile
uart_dma_profile_RELPATH = ti/drv/uart
uart_dma_profile_PATH = $(PDK_UART_COMP_PATH)
export uart_dma_profile_LIBNAME = ti.drv.uart.profiling.dma
export uart_dma_profile_LIBPATH = $(uart_dma_profile_PATH)/lib
export uart_dma_profile_OBJPATH = $(uart_dma_profile_RELPATH)/uart_dma_profile
export uart_dma_profile_MAKEFILE = -f build/makefile_dma_profile.mk
export uart_dma_profile_BOARD_DEPENDENCY = no
export uart_dma_profile_CORE_DEPENDENCY = no
export uart_dma_profile_SOC_DEPENDENCY = yes
uart_dma_profile_PKG_LIST = uart_dma_profile
uart_dma_profile_INCLUDE = $(uart_dma_profile_PATH)
export uart_dma_profile_SOCLIST = am574x am572x am571x dra72x dra75x dra78x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x
export uart_dma_profile_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART console LIB
export uart_console_COMP_LIST = uart_console
uart_console_RELPATH = ti/drv/uart
uart_console_PATH = $(PDK_UART_COMP_PATH)
export uart_console_LIBNAME = ti.drv.uart_console
export uart_console_LIBPATH = $(uart_console_PATH)/lib
export uart_console_OBJPATH = $(uart_console_RELPATH)/uart_console
export uart_console_MAKEFILE = -f build/makefile_console.mk
export uart_console_BOARD_DEPENDENCY = no
export uart_console_CORE_DEPENDENCY = no
export uart_console_SOC_DEPENDENCY = yes
uart_console_PKG_LIST = uart_console
uart_console_INCLUDE = $(uart_console_PATH)
export uart_console_SOCLIST = tda2xx tda2px tda2ex tda3xx am65xx j721e j7200 tpr12 awr294x j721s2 j784s4 j742s2
export uart_console_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

#
# UART Firmwares
#
export icss_uart_COMP_LIST = icss_uart
# temporary fix for nightly build
# icss_uart_RELPATH = ti/drv/uart/firmware/icss_uart
icss_uart_RELPATH = icss_uart
icss_uart_PATH = $(PDK_UART_COMP_PATH)/firmware/icss_uart
export icss_uart_HEADERNAME = icss_uart
export icss_uart_HEADERPATH = $(icss_uart_PATH)/bin
export icss_uart_OBJPATH = $(icss_uart_RELPATH)
export icss_uart_MAKEFILE = -f ../../build/makefile_icss_uart.mk
export icss_uart_BOARD_DEPENDENCY = no
export icss_uart_CORE_DEPENDENCY = yes
export icss_uart_SOC_DEPENDENCY = yes
export icss_uart_PKG_LIST = icss_uart
icss_uart_INCLUDE = $(icss_uart_PATH)
#export icss_uart_SOCLIST = am437x am574x am572x am571x am335x k2g
export icss_uart_SOCLIST = am335x
export icss_uart_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

#
# UART Examples
#

# UART unit test baremetal app
export UART_Baremetal_TestApp_COMP_LIST = UART_Baremetal_TestApp
UART_Baremetal_TestApp_RELPATH = ti/drv/uart/test
UART_Baremetal_TestApp_PATH = $(PDK_UART_COMP_PATH)/test
export UART_Baremetal_TestApp_BOARD_DEPENDENCY = yes
export UART_Baremetal_TestApp_CORE_DEPENDENCY = no
export UART_Baremetal_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
UART_Baremetal_TestApp_PKG_LIST = UART_Baremetal_TestApp
UART_Baremetal_TestApp_INCLUDE = $(UART_Baremetal_TestApp_PATH)
export UART_Baremetal_TestApp_BOARDLIST = $(drvuart_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 am64x j721s2 j784s4 j742s2))
export UART_Baremetal_TestApp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELISTARM)
else
export UART_Baremetal_TestApp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)
endif
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x j721s2 j784s4 j742s2))
export UART_Baremetal_TestApp_SBL_APPIMAGEGEN = yes
else
export UART_Baremetal_TestApp_SBL_APPIMAGEGEN = no
endif

# UART unit test baremetal dma app
export UART_Baremetal_DMA_TestApp_COMP_LIST = UART_Baremetal_DMA_TestApp
UART_Baremetal_DMA_TestApp_RELPATH = ti/drv/uart/test
UART_Baremetal_DMA_TestApp_PATH = $(PDK_UART_COMP_PATH)/test
export UART_Baremetal_DMA_TestApp_BOARD_DEPENDENCY = yes
export UART_Baremetal_DMA_TestApp_CORE_DEPENDENCY = no
export UART_Baremetal_DMA_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal DMA=enable
UART_Baremetal_DMA_TestApp_PKG_LIST = UART_Baremetal_DMA_TestApp
UART_Baremetal_DMA_TestApp_INCLUDE = $(UART_Baremetal_DMA_TestApp_PATH)
export UART_Baremetal_DMA_TestApp_BOARDLIST = $(drvuart_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 am64x j721s2 j784s4 j742s2))
export UART_Baremetal_DMA_TestApp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELISTARM)
else
export UART_Baremetal_DMA_TestApp_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)
endif
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x j721s2 j784s4 j742s2))
export UART_Baremetal_DMA_TestApp_SBL_APPIMAGEGEN = yes
endif


# UART RTOS Unit Test Apps
define UART_TESTAPP_RULE

export UART_TestApp_$(1)_COMP_LIST = UART_TestApp_$(1)
export UART_TestApp_$(1)_RELPATH = ti/drv/uart/test
export UART_TestApp_$(1)_PATH = $(PDK_UART_COMP_PATH)/test
export UART_TestApp_$(1)_BOARD_DEPENDENCY = yes
export UART_TestApp_$(1)_CORE_DEPENDENCY = no
export UART_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export UART_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
UART_TestApp_$(1)_PKG_LIST = UART_TestApp_$(1)
UART_TestApp_$(1)_INCLUDE = $(UART_TestApp_$(1)_PATH)
export UART_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvuart_BOARDLIST))
ifeq ($(SOC),$(filter $(SOC), am64x))
export UART_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1)
else
export UART_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvuart_$(SOC)_CORELIST))
endif
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x tpr12 awr294x j721s2 j784s4 j742s2))
export UART_TestApp_$(1)_SBL_APPIMAGEGEN = yes
endif
ifneq ($(1),$(filter $(1), safertos))
uart_EXAMPLE_LIST += UART_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
uart_EXAMPLE_LIST += UART_TestApp_$(1)
endif
endif

endef

UART_TESTAPP_MACRO_LIST := $(foreach curos,$(drvuart_RTOS_LIST) safertos,$(call UART_TESTAPP_RULE,$(curos)))

$(eval ${UART_TESTAPP_MACRO_LIST})


# UART DMA RTOS Unit Test Apps
define UART_DMA_TESTAPP_RULE

export UART_DMA_TestApp_$(1)_COMP_LIST = UART_DMA_TestApp_$(1)
export UART_DMA_TestApp_$(1)_RELPATH = ti/drv/uart/test
export UART_DMA_TestApp_$(1)_PATH = $(PDK_UART_COMP_PATH)/test
export UART_DMA_TestApp_$(1)_BOARD_DEPENDENCY = yes
export UART_DMA_TestApp_$(1)_CORE_DEPENDENCY = no
export UART_DMA_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export UART_DMA_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1) DMA=enable
UART_DMA_TestApp_$(1)_PKG_LIST = UART_DMA_TestApp_$(1)
UART_DMA_TestApp_$(1)_INCLUDE = $(UART_DMA_TestApp_$(1)_PATH)
export UART_DMA_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvuart_BOARDLIST))
ifeq ($(SOC),$(filter $(SOC), j721e j7200 am64x j721s2 j784s4 j742s2))
export UART_DMA_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvuart_$(SOC)_CORELISTARM))
else
export UART_DMA_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvuart_$(SOC)_CORELIST))
endif
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x tpr12 awr294x j721s2 j784s4 j742s2))
export UART_DMA_TestApp_$(1)_SBL_APPIMAGEGEN = yes
endif
ifneq ($(1),$(filter $(1), safertos))
uart_EXAMPLE_LIST += UART_DMA_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
uart_EXAMPLE_LIST += UART_DMA_TestApp_$(1)
endif
endif

endef

UART_DMA_TESTAPP_MACRO_LIST := $(foreach curos,$(drvuart_RTOS_LIST),$(call UART_DMA_TESTAPP_RULE,$(curos)))

$(eval ${UART_DMA_TESTAPP_MACRO_LIST})


# UART Polling mode Test app
export UART_BasicExample_Polling_ExampleProject_COMP_LIST = UART_BasicExample_Polling_ExampleProject
UART_BasicExample_Polling_ExampleProject_RELPATH = ti/drv/uart/example/UART_BasicExample_Polling_ExampleProject
UART_BasicExample_Polling_ExampleProject_PATH = $(PDK_UART_COMP_PATH)/example/UART_BasicExample_Polling_ExampleProject
export UART_BasicExample_Polling_ExampleProject_BOARD_DEPENDENCY = yes
export UART_BasicExample_Polling_ExampleProject_CORE_DEPENDENCY = no
export UART_BasicExample_Polling_ExampleProject_XDC_CONFIGURO = yes
UART_BasicExample_Polling_ExampleProject_PKG_LIST = UART_BasicExample_Polling_ExampleProject
UART_BasicExample_Polling_ExampleProject_INCLUDE = $(UART_BasicExample_Polling_ExampleProject_PATH)
export UART_BasicExample_Polling_ExampleProject_BOARDLIST = tda2xx-evm tda2px-evm tda2ex-evm tda3xx-evm evmDRA75x evmDRA78x
export UART_BasicExample_Polling_ExampleProject_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART Interrupt mode Test app
export UART_BasicExample_Intr_ExampleProject_COMP_LIST = UART_BasicExample_Intr_ExampleProject
UART_BasicExample_Intr_ExampleProject_RELPATH = ti/drv/uart/example/UART_BasicExample_Intr_ExampleProject
UART_BasicExample_Intr_ExampleProject_PATH = $(PDK_UART_COMP_PATH)/example/UART_BasicExample_Intr_ExampleProject
export UART_BasicExample_Intr_ExampleProject_BOARD_DEPENDENCY = yes
export UART_BasicExample_Intr_ExampleProject_CORE_DEPENDENCY = no
export UART_BasicExample_Intr_ExampleProject_XDC_CONFIGURO = yes
UART_BasicExample_Intr_ExampleProject_PKG_LIST = UART_BasicExample_Intr_ExampleProject
UART_BasicExample_Intr_ExampleProject_INCLUDE = $(UART_BasicExample_Intr_ExampleProject_PATH)
export UART_BasicExample_Intr_ExampleProject_BOARDLIST = tda2xx-evm tda2px-evm tda2ex-evm tda3xx-evm evmDRA75x evmDRA78x
export UART_BasicExample_Intr_ExampleProject_$(SOC)_CORELIST = $(drvuart_$(SOC)_CORELIST)

# UART MAIN TO MCU ROUTING Test app
export UART_BasicExample_mcu_routetest_COMP_LIST = UART_BasicExample_mcu_routetest
UART_BasicExample_mcu_routetest_RELPATH = ti/drv/uart/example/UART_BasicExample_mcu_routetest
UART_BasicExample_mcu_routetest_PATH = $(PDK_UART_COMP_PATH)/example/UART_BasicExample_mcu_routetest
export UART_BasicExample_mcu_routetest_BOARD_DEPENDENCY = yes
export UART_BasicExample_mcu_routetest_CORE_DEPENDENCY = no
export UART_BasicExample_mcu_routetest_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
UART_BasicExample_mcu_routetest_PKG_LIST = UART_BasicExample_mcu_routetest
UART_BasicExample_mcu_routetest_INCLUDE = $(UART_BasicExample_mcu_routetest_PATH)
export UART_BasicExample_mcu_routetest_BOARDLIST = j721e_evm
export UART_BasicExample_mcu_routetest_$(SOC)_CORELIST = mcu1_0
export UART_BasicExample_mcu_routetest_SBL_APPIMAGEGEN = yes


export drvuart_LIB_LIST
export uart_LIB_LIST
export uart_EXAMPLE_LIST
export drvuart_EXAMPLE_LIST
export drvuart_FIRM_LIST
export uart_FIRM_LIST

uart_component_make_include := 1
endif
