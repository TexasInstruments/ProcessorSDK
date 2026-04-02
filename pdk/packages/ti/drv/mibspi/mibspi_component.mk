#
# Copyright (c) 2016 - 2020, Texas Instruments Incorporated
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

# File: mibspi_component.mk
#       This file is component include make file of MIBSPI library.
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
ifeq ($(mibspi_component_make_include), )

drvmibspi_RTOS_LIST = $(DEFAULT_RTOS_LIST)
drvmibspi_BOARDLIST       = tpr12_evm tpr12_qt awr294x_evm
drvmibspi_SOCLIST         = tpr12 awr294x
drvmibspi_tpr12_CORELIST   = mcu1_0 c66xdsp_1
drvmibspi_awr294x_CORELIST   = mcu1_0

############################
# mibspi package
# List of components included under mibspi lib
# The components included here are built and will be part of mibspi lib
############################
mibspi_LIB_LIST = mibspi mibspi_dma
drvmibspi_LIB_LIST = $(mibspi_LIB_LIST)


############################
# mibspi examples
# List of examples under mibspi
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
mibspi_EXAMPLE_LIST = MIBSPI_Slavemode_TestApp MIBSPI_Slavemode_Dma_TestApp

############################
# spi examples
# List of examples under spi
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################

#
# MIBSPI Modules
#

# MIBSPI LIB
mibspi_COMP_LIST = mibspi
mibspi_RELPATH = ti/drv/mibspi
mibspi_PATH = $(PDK_MIBSPI_COMP_PATH)
mibspi_LIBNAME = ti.drv.mibspi
export mibspi_LIBNAME
mibspi_LIBPATH = $(mibspi_PATH)/lib
export mibspi_LIBPATH
mibspi_OBJPATH = $(mibspi_RELPATH)/mibspi
export mibspi_OBJPATH
mibspi_MAKEFILE = -f build/makefile.mk
export mibspi_MAKEFILE
mibspi_BOARD_DEPENDENCY = no
mibspi_CORE_DEPENDENCY = no
mibspi_SOC_DEPENDENCY = yes
export mibspi_COMP_LIST
export mibspi_BOARD_DEPENDENCY
export mibspi_CORE_DEPENDENCY
export mibspi_SOC_DEPENDENCY
mibspi_PKG_LIST = mibspi
export mibspi_PKG_LIST
mibspi_INCLUDE = $(mibspi_PATH)
mibspi_SOCLIST = $(drvmibspi_SOCLIST)
export mibspi_SOCLIST
mibspi_$(SOC)_CORELIST = $(drvmibspi_$(SOC)_CORELIST)
export mibspi_$(SOC)_CORELIST


# MIBSPI DMA LIB
mibspi_dma_COMP_LIST = mibspi_dma
mibspi_dma_RELPATH = ti/drv/mibspi
mibspi_dma_PATH = $(PDK_MIBSPI_COMP_PATH)
mibspi_dma_LIBNAME = ti.drv.mibspi.dma
export mibspi_dma_LIBNAME
mibspi_dma_LIBPATH = $(mibspi_dma_PATH)/lib
export mibspi_dma_LIBPATH
mibspi_dma_OBJPATH = $(mibspi_dma_RELPATH)/mibspi_dma
export mibspi_dma_OBJPATH
mibspi_dma_MAKEFILE = -f build/makefile_dma.mk
export mibspi_dma_MAKEFILE
mibspi_dma_BOARD_DEPENDENCY = no
mibspi_dma_CORE_DEPENDENCY = no
mibspi_dma_SOC_DEPENDENCY = yes
export mibspi_dma_COMP_LIST
export mibspi_dma_BOARD_DEPENDENCY
export mibspi_dma_CORE_DEPENDENCY
export mibspi_dma_SOC_DEPENDENCY
mibspi_dma_PKG_LIST = mibspi_dma
export mibspi_dma_PKG_LIST
mibspi_dma_INCLUDE = $(mibspi_dma_PATH)
mibspi_dma_SOCLIST = $(drvmibspi_SOCLIST)
export mibspi_dma_SOCLIST
mibspi_dma_$(SOC)_CORELIST = $(drvmibspi_$(SOC)_CORELIST)
export mibspi_dma_$(SOC)_CORELIST



#
# MIBSPI Examples
#
# MIBSPI baremetal master slave Test app

# MIBSPI baremetal dma master slave Test app
# MIBSPI Loopback Test App
define MIBSPI_Loopback_TestApp_RULE

export MIBSPI_Loopback_TestApp_$(1)_COMP_LIST = MIBSPI_Loopback_TestApp_$(1)
export MIBSPI_Loopback_TestApp_$(1)_RELPATH = ti/drv/mibspi/test/loopback
export MIBSPI_Loopback_TestApp_$(1)_PATH = $(PDK_MIBSPI_COMP_PATH)/test/loopback
export MIBSPI_Loopback_TestApp_$(1)_BOARD_DEPENDENCY = yes
export MIBSPI_Loopback_TestApp_$(1)_CORE_DEPENDENCY = no
export MIBSPI_Loopback_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export MIBSPI_Loopback_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export MIBSPI_Loopback_TestApp_$(1)_PKG_LIST = MIBSPI_Loopback_TestApp_$(1)
export MIBSPI_Loopback_TestApp_$(1)_INCLUDE = $(MIBSPI_Loopback_TestApp_$(1)_PATH)
export MIBSPI_Loopback_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmibspi_BOARDLIST))
export MIBSPI_Loopback_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmibspi_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
mibspi_EXAMPLE_LIST += MIBSPI_Loopback_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
mibspi_EXAMPLE_LIST += MIBSPI_Loopback_TestApp_$(1)
endif
endif
export MIBSPI_Loopback_TestApp_$(1)_SBL_APPIMAGEGEN = yes

endef

MIBSPI_Loopback_TestApp_MACRO_LIST := $(foreach curos,$(drvmibspi_RTOS_LIST),$(call MIBSPI_Loopback_TestApp_RULE,$(curos)))

$(eval ${MIBSPI_Loopback_TestApp_MACRO_LIST})

# MIBSPI Loopback Dma Test App
define MIBSPI_Loopback_Dma_TestApp_RULE

export MIBSPI_Loopback_Dma_TestApp_$(1)_COMP_LIST = MIBSPI_Loopback_Dma_TestApp_$(1)
export MIBSPI_Loopback_Dma_TestApp_$(1)_RELPATH = ti/drv/mibspi/test/loopback
export MIBSPI_Loopback_Dma_TestApp_$(1)_PATH = $(PDK_MIBSPI_COMP_PATH)/test/loopback
export MIBSPI_Loopback_Dma_TestApp_$(1)_BOARD_DEPENDENCY = yes
export MIBSPI_Loopback_Dma_TestApp_$(1)_CORE_DEPENDENCY = no
export MIBSPI_Loopback_Dma_TestApp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export MIBSPI_Loopback_Dma_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)  DMA=enable
export MIBSPI_Loopback_Dma_TestApp_$(1)_PKG_LIST = MIBSPI_Loopback_Dma_TestApp_$(1)
export MIBSPI_Loopback_Dma_TestApp_$(1)_INCLUDE = $(MIBSPI_Loopback_Dma_TestApp_$(1)_PATH)
export MIBSPI_Loopback_Dma_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmibspi_BOARDLIST))
export MIBSPI_Loopback_Dma_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmibspi_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
mibspi_EXAMPLE_LIST += MIBSPI_Loopback_Dma_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
mibspi_EXAMPLE_LIST += MIBSPI_Loopback_Dma_TestApp_$(1)
endif
endif
export MIBSPI_Loopback_Dma_TestApp_$(1)_SBL_APPIMAGEGEN = yes

endef

MIBSPI_Loopback_Dma_TestApp_MACRO_LIST := $(foreach curos,$(drvmibspi_RTOS_LIST),$(call MIBSPI_Loopback_Dma_TestApp_RULE,$(curos)))

$(eval ${MIBSPI_Loopback_Dma_TestApp_MACRO_LIST})

# MIBSPI rtos master slave Test app
MIBSPI_Slavemode_TestApp_COMP_LIST = MIBSPI_Slavemode_TestApp
MIBSPI_Slavemode_TestApp_RELPATH = ti/drv/mibspi/test/slavemode
MIBSPI_Slavemode_TestApp_PATH = $(PDK_MIBSPI_COMP_PATH)/test/slavemode
MIBSPI_Slavemode_TestApp_BOARD_DEPENDENCY = yes
MIBSPI_Slavemode_TestApp_CORE_DEPENDENCY = no
MIBSPI_Slavemode_TestApp_XDC_CONFIGURO = yes
MIBSPI_Slavemode_TestApp_MAKEFILE = -f makefile
export MIBSPI_Slavemode_TestApp_COMP_LIST
export MIBSPI_Slavemode_TestApp_BOARD_DEPENDENCY
export MIBSPI_Slavemode_TestApp_CORE_DEPENDENCY
export MIBSPI_Slavemode_TestApp_XDC_CONFIGURO
export MIBSPI_Slavemode_TestApp_MAKEFILE
MIBSPI_Slavemode_TestApp_PKG_LIST = MIBSPI_Slavemode_TestApp
export MIBSPI_Slavemode_TestApp_PKG_LIST
MIBSPI_Slavemode_TestApp_INCLUDE = $(MIBSPI_Slavemode_TestApp_PATH)
MIBSPI_Slavemode_TestApp_BOARDLIST = $(drvmibspi_BOARDLIST)
export MIBSPI_Slavemode_TestApp_BOARDLIST
MIBSPI_Slavemode_TestApp_$(SOC)_CORELIST = $(drvmibspi_$(SOC)_CORELIST)
export MIBSPI_Slavemode_TestApp_$(SOC)_CORELIST

# MIBSPI rtos master slave Dma Test app
MIBSPI_Slavemode_Dma_TestApp_COMP_LIST = MIBSPI_Slavemode_Dma_TestApp
MIBSPI_Slavemode_Dma_TestApp_RELPATH = ti/drv/mibspi/test/slavemode
MIBSPI_Slavemode_Dma_TestApp_PATH = $(PDK_MIBSPI_COMP_PATH)/test/slavemode
MIBSPI_Slavemode_Dma_TestApp_BOARD_DEPENDENCY = yes
MIBSPI_Slavemode_Dma_TestApp_CORE_DEPENDENCY = no
MIBSPI_Slavemode_Dma_TestApp_XDC_CONFIGURO = yes
MIBSPI_Slavemode_Dma_TestApp_MAKEFILE = -f makefile DMA=enable
export MIBSPI_Slavemode_Dma_TestApp_COMP_LIST
export MIBSPI_Slavemode_Dma_TestApp_BOARD_DEPENDENCY
export MIBSPI_Slavemode_Dma_TestApp_CORE_DEPENDENCY
export MIBSPI_Slavemode_Dma_TestApp_XDC_CONFIGURO
export MIBSPI_Slavemode_Dma_TestApp_MAKEFILE
MIBSPI_Slavemode_Dma_TestApp_PKG_LIST = MIBSPI_Slavemode_Dma_TestApp
export MIBSPI_Slavemode_Dma_TestApp_PKG_LIST
MIBSPI_Slavemode_Dma_TestApp_INCLUDE = $(MIBSPI_Slavemode_Dma_TestApp_PATH)
MIBSPI_Slavemode_Dma_TestApp_BOARDLIST = $(drvmibspi_BOARDLIST)
export MIBSPI_Slavemode_Dma_TestApp_BOARDLIST
MIBSPI_Slavemode_Dma_TestApp_$(SOC)_CORELIST = $(drvmibspi_$(SOC)_CORELIST)
export MIBSPI_Slavemode_Dma_TestApp_$(SOC)_CORELIST

drvmibspi_EXAMPLE_LIST = $(mibspi_EXAMPLE_LIST)

export drvmibspi_LIB_LIST
export mibspi_LIB_LIST
export mibspi_EXAMPLE_LIST
export drvmibspi_EXAMPLE_LIST

mibspi_component_make_include := 1
endif
