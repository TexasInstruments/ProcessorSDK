#
# Copyright (c) 2017-2022, Texas Instruments Incorporated
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

# File: mmcsd_component.mk
#       This file is component include make file of MMCSD library.
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
ifeq ($(mmcsd_component_make_include), )

# note SOC="devind"is not supported for DMA as it is covered
# under other list
drvmmcsd_BOARDLIST       = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvmmcsd_dma_SOCLIST     = j721e j7200 j721s2 j784s4 j742s2
drvmmcsd_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvmmcsd_j721e_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvmmcsd_j7200_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1
drvmmcsd_j721s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvmmcsd_j784s4_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvmmcsd_j742s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvmmcsd_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

############################
# mmcsd package
# List of components included under mmcsd lib
# The components included here are built and will be part of mmcsd lib
############################
mmcsd_LIB_LIST = mmcsd mmcsd_dma mmcsd_indp
drvmmcsd_LIB_LIST = $(mmcsd_LIB_LIST)
mmcsd_EXAMPLE_LIST =
#
# MMCSD Modules
#

# MMCSD LIB
mmcsd_COMP_LIST = mmcsd
mmcsd_RELPATH = ti/drv/mmcsd
mmcsd_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_LIBNAME = ti.drv.mmcsd
export mmcsd_LIBNAME
mmcsd_LIBPATH = $(mmcsd_PATH)/lib
export mmcsd_LIBPATH
mmcsd_OBJPATH = $(mmcsd_RELPATH)/mmcsd
export mmcsd_OBJPATH
mmcsd_MAKEFILE = -f build/makefile.mk
export mmcsd_MAKEFILE
mmcsd_BOARD_DEPENDENCY = no
mmcsd_CORE_DEPENDENCY = no
mmcsd_SOC_DEPENDENCY = yes
export mmcsd_COMP_LIST
export mmcsd_BOARD_DEPENDENCY
export mmcsd_CORE_DEPENDENCY
export mmcsd_SOC_DEPENDENCY
mmcsd_PKG_LIST = mmcsd
export mmcsd_PKG_LIST
mmcsd_INCLUDE = $(mmcsd_PATH)
mmcsd_SOCLIST = $(drvmmcsd_SOCLIST)
export mmcsd_SOCLIST
mmcsd_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_$(SOC)_CORELIST

# MMCSD LIB DEVICE INDEPENDENT
mmcsd_indp_COMP_LIST = mmcsd_indp
mmcsd_indp_RELPATH = ti/drv/mmcsd
mmcsd_indp_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_indp_LIBNAME = ti.drv.mmcsd
export mmcsd_indp_LIBNAME
mmcsd_indp_LIBPATH = $(mmcsd_indp_PATH)/lib
export mmcsd_indp_LIBPATH
mmcsd_indp_OBJPATH = $(mmcsd_indp_RELPATH)/mmcsd_indp
export mmcsd_indp_OBJPATH
mmcsd_indp_MAKEFILE = -f build/makefile_indp.mk
export mmcsd_indp_MAKEFILE
mmcsd_indp_BOARD_DEPENDENCY = no
mmcsd_indp_CORE_DEPENDENCY = no
mmcsd_indp_SOC_DEPENDENCY = no
export mmcsd_indp_COMP_LIST
export mmcsd_indp_BOARD_DEPENDENCY
export mmcsd_indp_CORE_DEPENDENCY
export mmcsd_indp_SOC_DEPENDENCY
mmcsd_indp_PKG_LIST = mmcsd_indp
mmcsd_indp_INCLUDE = $(mmcsd_indp_PATH)
mmcsd_indp_SOCLIST = $(drvmmcsd_SOCLIST)
export mmcsd_indp_SOCLIST
mmcsd_indp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_indp_$(SOC)_CORELIST

# MMCSD DMA SOC LIB
mmcsd_dma_COMP_LIST = mmcsd_dma
mmcsd_dma_RELPATH = ti/drv/mmcsd
mmcsd_dma_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_dma_LIBNAME = ti.drv.mmcsd.dma
export mmcsd_dma_LIBNAME
mmcsd_dma_LIBPATH = $(mmcsd_dma_PATH)/lib
export mmcsd_dma_LIBPATH
mmcsd_dma_OBJPATH = $(mmcsd_dma_RELPATH)/mmcsd_dma
export mmcsd_dma_OBJPATH
mmcsd_dma_MAKEFILE = -f build/makefile_dma.mk
export mmcsd_dma_MAKEFILE
mmcsd_dma_BOARD_DEPENDENCY = no
mmcsd_dma_CORE_DEPENDENCY = no
mmcsd_dma_SOC_DEPENDENCY = yes
export mmcsd_dma_COMP_LIST
export mmcsd_dma_BOARD_DEPENDENCY
export mmcsd_dma_CORE_DEPENDENCY
export mmcsd_dma_SOC_DEPENDENCY
mmcsd_dma_PKG_LIST = mmcsd_dma
mmcsd_dma_INCLUDE = $(mmcsd_dma_PATH)
mmcsd_dma_SOCLIST = $(drvmmcsd_dma_SOCLIST)
export mmcsd_dma_SOCLIST
mmcsd_dma_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_dma_$(SOC)_CORELIST

# MMCSD PROFILING SOC LIB
mmcsd_profile_COMP_LIST = mmcsd_profile
mmcsd_profile_RELPATH = ti/drv/mmcsd
mmcsd_profile_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_profile_LIBNAME = ti.drv.mmcsd.profiling
export mmcsd_profile_LIBNAME
mmcsd_profile_LIBPATH = $(mmcsd_profile_PATH)/lib
export mmcsd_profile_LIBPATH
mmcsd_profile_OBJPATH = $(mmcsd_profile_RELPATH)/mmcsd_profile
export mmcsd_profile_OBJPATH
mmcsd_profile_MAKEFILE = -f build/makefile_profile.mk
export mmcsd_profile_MAKEFILE
mmcsd_profile_BOARD_DEPENDENCY = no
mmcsd_profile_CORE_DEPENDENCY = no
mmcsd_profile_SOC_DEPENDENCY = yes
export mmcsd_profile_COMP_LIST
export mmcsd_profile_BOARD_DEPENDENCY
export mmcsd_profile_CORE_DEPENDENCY
export mmcsd_profile_SOC_DEPENDENCY
mmcsd_profile_PKG_LIST = mmcsd_profile
mmcsd_profile_INCLUDE = $(mmcsd_profile_PATH)
mmcsd_profile_SOCLIST = $(drvmmcsd_SOCLIST)
export mmcsd_profile_SOCLIST
mmcsd_profile_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_profile_$(SOC)_CORELIST

# MMCSD PROFILING SOC INDEPENDENT LIB
mmcsd_profile_indp_COMP_LIST = mmcsd_profile_indp
mmcsd_profile_indp_RELPATH = ti/drv/mmcsd
mmcsd_profile_indp_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_profile_indp_LIBNAME = ti.drv.mmcsd.profiling
export mmcsd_profile_indp_LIBNAME
mmcsd_profile_indp_LIBPATH = $(mmcsd_profile_indp_PATH)/lib
export mmcsd_profile_indp_LIBPATH
mmcsd_profile_indp_OBJPATH = $(mmcsd_profile_indp_RELPATH)/mmcsd_profile_indp
export mmcsd_profile_indp_OBJPATH
mmcsd_profile_indp_MAKEFILE = -f build/makefile_profile_indp.mk
export mmcsd_profile_indp_MAKEFILE
mmcsd_profile_indp_BOARD_DEPENDENCY = no
mmcsd_profile_indp_CORE_DEPENDENCY = no
mmcsd_profile_indp_SOC_DEPENDENCY = no
export mmcsd_profile_indp_COMP_LIST
export mmcsd_profile_indp_BOARD_DEPENDENCY
export mmcsd_profile_indp_CORE_DEPENDENCY
export mmcsd_profile_indp_SOC_DEPENDENCY
mmcsd_profile_indp_PKG_LIST = mmcsd_profile_indp
mmcsd_profile_indp_INCLUDE = $(mmcsd_profile_indp_PATH)
mmcsd_profile_indp_SOCLIST = $(drvmmcsd_SOCLIST)
export mmcsd_profile_indp_SOCLIST
mmcsd_profile_indp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_profile_indp_$(SOC)_CORELIST

# MMCSD DMA PROFILING SOC LIB
mmcsd_dma_profile_COMP_LIST = mmcsd_dma_profile
mmcsd_dma_profile_RELPATH = ti/drv/mmcsd
mmcsd_dma_profile_PATH = $(PDK_MMCSD_COMP_PATH)
mmcsd_dma_profile_LIBNAME = ti.drv.mmcsd.profiling.dma
export mmcsd_dma_profile_LIBNAME
mmcsd_dma_profile_LIBPATH = $(mmcsd_dma_profile_PATH)/lib
export mmcsd_dma_profile_LIBPATH
mmcsd_dma_profile_OBJPATH = $(mmcsd_dma_profile_RELPATH)/mmcsd_dma_profile
export mmcsd_dma_profile_OBJPATH
mmcsd_dma_profile_MAKEFILE = -f build/makefile_dma_profile.mk
export mmcsd_dma_profile_MAKEFILE
mmcsd_dma_profile_BOARD_DEPENDENCY = no
mmcsd_dma_profile_CORE_DEPENDENCY = no
mmcsd_dma_profile_SOC_DEPENDENCY = yes
export mmcsd_dma_profile_COMP_LIST
export mmcsd_dma_profile_BOARD_DEPENDENCY
export mmcsd_dma_profile_CORE_DEPENDENCY
export mmcsd_dma_profile_SOC_DEPENDENCY
mmcsd_dma_profile_PKG_LIST = mmcsd_dma_profile
mmcsd_dma_profile_INCLUDE = $(mmcsd_dma_profile_PATH)
mmcsd_dma_profile_SOCLIST = $(drvmmcsd_dma_SOCLIST)
export mmcsd_dma_profile_SOCLIST
mmcsd_dma_profile_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export mmcsd_dma_profile_$(SOC)_CORELIST

# SD Readwrite test
define MMCSD_TestApp_RULE

    export MMCSD_TestApp_$(1)_COMP_LIST = MMCSD_TestApp_$(1)
    export MMCSD_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_TestApp
    export MMCSD_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_TestApp
    export MMCSD_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_TestApp_$(1)_CORE_DEPENDENCY = no
    export MMCSD_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
    MMCSD_TestApp_$(1)_PKG_LIST = MMCSD_TestApp_$(1)
    MMCSD_TestApp_$(1)_INCLUDE = $(MMCSD_TestApp_$(1)_PATH)
    export MMCSD_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))

    export MMCSD_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))

    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_TestApp_$(1)
        endif
    endif

endef

MMCSD_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_TestApp_RULE,$(curos)))
$(eval ${MMCSD_TestApp_MACRO_LIST})


# EMMC Readwrite test
define MMCSD_EMMC_TestApp_RULE

    export MMCSD_EMMC_TestApp_$(1)_COMP_LIST = MMCSD_EMMC_TestApp_$(1)
    export MMCSD_EMMC_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_EMMC_TestApp
    export MMCSD_EMMC_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_EMMC_TestApp
    export MMCSD_EMMC_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_EMMC_TestApp_$(1)_CORE_DEPENDENCY = no
    export MMCSD_EMMC_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
    MMCSD_EMMC_TestApp_$(1)_PKG_LIST = MMCSD_EMMC_TestApp_$(1)
    MMCSD_EMMC_TestApp_$(1)_INCLUDE = $(MMCSD_EMMC_TestApp_$(1)_PATH)
    export MMCSD_EMMC_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))

    export MMCSD_EMMC_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))
    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_EMMC_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_TestApp_$(1)
        endif
    endif

endef

MMCSD_EMMC_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_EMMC_TestApp_RULE,$(curos)))
$(eval ${MMCSD_EMMC_TestApp_MACRO_LIST})

# EMMC ReadwriteDMA test
define MMCSD_EMMC_DMA_TestApp_RULE

    export MMCSD_EMMC_DMA_TestApp_$(1)_COMP_LIST = MMCSD_EMMC_DMA_TestApp_$(1)
    export MMCSD_EMMC_DMA_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_EMMC_DMA_TestApp
    export MMCSD_EMMC_DMA_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_EMMC_DMA_TestApp
    export MMCSD_EMMC_DMA_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_EMMC_DMA_TestApp_$(1)_CORE_DEPENDENCY = no
    export MMCSD_EMMC_DMA_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    MMCSD_EMMC_DMA_TestApp_$(1)_PKG_LIST = MMCSD_EMMC_DMA_TestApp_$(1)
    MMCSD_EMMC_DMA_TestApp_$(1)_INCLUDE = $(MMCSD_EMMC_DMA_TestApp_$(1)_PATH)
    export MMCSD_EMMC_DMA_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))
    export MMCSD_EMMC_DMA_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))

    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_EMMC_DMA_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_DMA_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_DMA_TestApp_$(1)
        endif
    endif

endef

MMCSD_EMMC_DMA_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_EMMC_DMA_TestApp_RULE,$(curos)))
$(eval ${MMCSD_EMMC_DMA_TestApp_MACRO_LIST})


# Baremetal SD Readwrite test
MMCSD_Baremetal_TestApp_COMP_LIST = MMCSD_Baremetal_TestApp
MMCSD_Baremetal_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_Baremetal_TestApp
MMCSD_Baremetal_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Baremetal_TestApp
MMCSD_Baremetal_TestApp_BOARD_DEPENDENCY = yes
MMCSD_Baremetal_TestApp_CORE_DEPENDENCY = no
MMCSD_Baremetal_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_Baremetal_TestApp_XDC_CONFIGURO = no
export MMCSD_Baremetal_TestApp_COMP_LIST
export MMCSD_Baremetal_TestApp_BOARD_DEPENDENCY
export MMCSD_Baremetal_TestApp_CORE_DEPENDENCY
export MMCSD_Baremetal_TestApp_MAKEFILE
export MMCSD_Baremetal_TestApp_XDC_CONFIGURO
MMCSD_Baremetal_TestApp_PKG_LIST = MMCSD_Baremetal_TestApp
MMCSD_Baremetal_TestApp_INCLUDE = $(MMCSD_Baremetal_TestApp_PATH)
MMCSD_Baremetal_TestApp_BOARDLIST = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export MMCSD_Baremetal_TestApp_BOARDLIST
MMCSD_Baremetal_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_Baremetal_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_Baremetal_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_Baremetal_TestApp_SBL_APPIMAGEGEN
endif

# Baremetal SD Readwrite DMA test
MMCSD_Baremetal_DMA_TestApp_COMP_LIST = MMCSD_Baremetal_DMA_TestApp
MMCSD_Baremetal_DMA_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_Baremetal_DMA_TestApp
MMCSD_Baremetal_DMA_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Baremetal_DMA_TestApp
MMCSD_Baremetal_DMA_TestApp_BOARD_DEPENDENCY = yes
MMCSD_Baremetal_DMA_TestApp_CORE_DEPENDENCY = no
MMCSD_Baremetal_DMA_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_Baremetal_DMA_TestApp_XDC_CONFIGURO = no
export MMCSD_Baremetal_DMA_TestApp_COMP_LIST
export MMCSD_Baremetal_DMA_TestApp_BOARD_DEPENDENCY
export MMCSD_Baremetal_DMA_TestApp_CORE_DEPENDENCY
export MMCSD_Baremetal_DMA_TestApp_MAKEFILE
export MMCSD_Baremetal_DMA_TestApp_XDC_CONFIGURO
MMCSD_Baremetal_DMA_TestApp_PKG_LIST = MMCSD_Baremetal_DMA_TestApp
MMCSD_Baremetal_DMA_TestApp_INCLUDE = $(MMCSD_Baremetal_DMA_TestApp_PATH)
MMCSD_Baremetal_DMA_TestApp_BOARDLIST = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export MMCSD_Baremetal_DMA_TestApp_BOARDLIST
MMCSD_Baremetal_DMA_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_Baremetal_DMA_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_Baremetal_DMA_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_Baremetal_DMA_TestApp_SBL_APPIMAGEGEN
endif

# SD DMA Unit test
define MMCSD_DMA_TestApp_RULE

    export MMCSD_DMA_TestApp_$(1)_COMP_LIST = MMCSD_DMA_TestApp_$(1)
    export MMCSD_DMA_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_DMA_TestApp
    export MMCSD_DMA_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_DMA_TestApp
    export MMCSD_DMA_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_DMA_TestApp_$(1)_CORE_DEPENDENCY = no
    export MMCSD_DMA_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    MMCSD_DMA_TestApp_$(1)_PKG_LIST = MMCSD_DMA_TestApp_$(1)
    MMCSD_DMA_TestApp_$(1)_INCLUDE = $(MMCSD_DMA_TestApp_$(1)_PATH)
    export MMCSD_DMA_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))
    export MMCSD_DMA_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))

    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_DMA_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_DMA_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_DMA_TestApp_$(1)
        endif
    endif

endef

MMCSD_DMA_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_DMA_TestApp_RULE,$(curos)))
$(eval ${MMCSD_DMA_TestApp_MACRO_LIST})


# Baremetal eMMC Readwrite test
MMCSD_Baremetal_EMMC_TestApp_COMP_LIST = MMCSD_Baremetal_EMMC_TestApp
MMCSD_Baremetal_EMMC_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_Baremetal_EMMC_TestApp
MMCSD_Baremetal_EMMC_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Baremetal_EMMC_TestApp
MMCSD_Baremetal_EMMC_TestApp_BOARD_DEPENDENCY = yes
MMCSD_Baremetal_EMMC_TestApp_CORE_DEPENDENCY = no
MMCSD_Baremetal_EMMC_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_Baremetal_EMMC_TestApp_XDC_CONFIGURO = no
export MMCSD_Baremetal_EMMC_TestApp_COMP_LIST
export MMCSD_Baremetal_EMMC_TestApp_BOARD_DEPENDENCY
export MMCSD_Baremetal_EMMC_TestApp_CORE_DEPENDENCY
export MMCSD_Baremetal_EMMC_TestApp_MAKEFILE
export MMCSD_Baremetal_EMMC_TestApp_XDC_CONFIGURO
MMCSD_Baremetal_EMMC_TestApp_PKG_LIST = MMCSD_Baremetal_EMMC_TestApp
MMCSD_Baremetal_EMMC_TestApp_INCLUDE = $(MMCSD_Baremetal_EMMC_TestApp_PATH)
MMCSD_Baremetal_EMMC_TestApp_BOARDLIST = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm
export MMCSD_Baremetal_EMMC_TestApp_BOARDLIST
MMCSD_Baremetal_EMMC_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_Baremetal_EMMC_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_Baremetal_EMMC_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_Baremetal_EMMC_TestApp_SBL_APPIMAGEGEN
endif

# Baremetal eMMC DMA Readwrite test
MMCSD_Baremetal_EMMC_DMA_TestApp_COMP_LIST = MMCSD_Baremetal_EMMC_DMA_TestApp
MMCSD_Baremetal_EMMC_DMA_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_Baremetal_EMMC_DMA_TestApp
MMCSD_Baremetal_EMMC_DMA_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Baremetal_EMMC_DMA_TestApp
MMCSD_Baremetal_EMMC_DMA_TestApp_BOARD_DEPENDENCY = yes
MMCSD_Baremetal_EMMC_DMA_TestApp_CORE_DEPENDENCY = no
MMCSD_Baremetal_EMMC_DMA_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_Baremetal_EMMC_DMA_TestApp_XDC_CONFIGURO = no
export MMCSD_Baremetal_EMMC_DMA_TestApp_COMP_LIST
export MMCSD_Baremetal_EMMC_DMA_TestApp_BOARD_DEPENDENCY
export MMCSD_Baremetal_EMMC_DMA_TestApp_CORE_DEPENDENCY
export MMCSD_Baremetal_EMMC_DMA_TestApp_MAKEFILE
export MMCSD_Baremetal_EMMC_DMA_TestApp_XDC_CONFIGURO
MMCSD_Baremetal_EMMC_DMA_TestApp_PKG_LIST = MMCSD_Baremetal_EMMC_DMA_TestApp
MMCSD_Baremetal_EMMC_DMA_TestApp_INCLUDE = $(MMCSD_Baremetal_EMMC_DMA_TestApp_PATH)
MMCSD_Baremetal_EMMC_DMA_TestApp_BOARDLIST = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export MMCSD_Baremetal_EMMC_DMA_TestApp_BOARDLIST
MMCSD_Baremetal_EMMC_DMA_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_Baremetal_EMMC_DMA_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_Baremetal_EMMC_DMA_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_Baremetal_EMMC_DMA_TestApp_SBL_APPIMAGEGEN
endif

# SD Regression Test App
define MMCSD_Regression_TestApp_RULE

    export MMCSD_Regression_TestApp_$(1)_COMP_LIST = MMCSD_Regression_TestApp_$(1)
    export MMCSD_Regression_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_Regression_TestApp
    export MMCSD_Regression_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Regression_TestApp
    export MMCSD_Regression_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_Regression_TestApp_$(1)_CORE_DEPENDENCY = yes
    export MMCSD_Regression_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    MMCSD_Regression_TestApp_$(1)_PKG_LIST = MMCSD_Regression_TestApp_$(1)
    MMCSD_Regression_TestApp_$(1)_INCLUDE = $(MMCSD_Regression_TestApp_$(1)_PATH)
    export MMCSD_Regression_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))
    export MMCSD_Regression_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))

    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_Regression_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_Regression_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_Regression_TestApp_$(1)
        endif
    endif

endef

MMCSD_Regression_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_Regression_TestApp_RULE,$(curos)))
$(eval ${MMCSD_Regression_TestApp_MACRO_LIST})


# SD Regression Test App
MMCSD_Baremetal_Regression_TestApp_COMP_LIST = MMCSD_Baremetal_Regression_TestApp
MMCSD_Baremetal_Regression_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_Baremetal_Regression_TestApp
MMCSD_Baremetal_Regression_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_Baremetal_Regression_TestApp
MMCSD_Baremetal_Regression_TestApp_BOARD_DEPENDENCY = yes
MMCSD_Baremetal_Regression_TestApp_CORE_DEPENDENCY = no
MMCSD_Baremetal_Regression_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_Baremetal_Regression_TestApp_XDC_CONFIGURO = no
export MMCSD_Baremetal_Regression_TestApp_COMP_LIST
export MMCSD_Baremetal_Regression_TestApp_BOARD_DEPENDENCY
export MMCSD_Baremetal_Regression_TestApp_CORE_DEPENDENCY
export MMCSD_Baremetal_Regression_TestApp_MAKEFILE
export MMCSD_Baremetal_Regression_TestApp_XDC_CONFIGURO
MMCSD_Baremetal_Regression_TestApp_PKG_LIST = MMCSD_Baremetal_Regression_TestApp
MMCSD_Baremetal_Regression_TestApp_INCLUDE = $(MMCSD_Baremetal_Regression_TestApp_PATH)
MMCSD_Baremetal_Regression_TestApp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export MMCSD_Baremetal_Regression_TestApp_BOARDLIST
MMCSD_Baremetal_Regression_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_Baremetal_Regression_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_Baremetal_Regression_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_Baremetal_Regression_TestApp_SBL_APPIMAGEGEN
endif

# MMC Regression Test App
define MMCSD_EMMC_Regression_TestApp_RULE

    export MMCSD_EMMC_Regression_TestApp_$(1)_COMP_LIST = MMCSD_EMMC_Regression_TestApp_$(1)
    export MMCSD_EMMC_Regression_TestApp_$(1)_RELPATH = ti/drv/mmcsd/test/MMCSD_EMMC_Regression_TestApp
    export MMCSD_EMMC_Regression_TestApp_$(1)_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_EMMC_Regression_TestApp
    export MMCSD_EMMC_Regression_TestApp_$(1)_BOARD_DEPENDENCY = yes
    export MMCSD_EMMC_Regression_TestApp_$(1)_CORE_DEPENDENCY = no
    export MMCSD_EMMC_Regression_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    MMCSD_EMMC_Regression_TestApp_$(1)_PKG_LIST = MMCSD_EMMC_Regression_TestApp_$(1)
    MMCSD_EMMC_Regression_TestApp_$(1)_INCLUDE = $(MMCSD_EMMC_Regression_TestApp_$(1)_PATH)
    export MMCSD_EMMC_Regression_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvmmcsd_BOARDLIST))
    export MMCSD_EMMC_Regression_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvmmcsd_$(SOC)_CORELIST))

    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
        export MMCSD_EMMC_Regression_TestApp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        ifeq ($(1),$(filter $(1), freertos))
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_Regression_TestApp_$(1)
        endif
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
            mmcsd_EXAMPLE_LIST += MMCSD_EMMC_Regression_TestApp_$(1)
        endif
    endif
endef

MMCSD_EMMC_Regression_TestApp_MACRO_LIST := $(foreach curos,$(drvmmcsd_RTOS_LIST),$(call MMCSD_EMMC_Regression_TestApp_RULE,$(curos)))
$(eval ${MMCSD_EMMC_Regression_TestApp_MACRO_LIST})


# SD Regression Test App
MMCSD_EMMC_Baremetal_Regression_TestApp_COMP_LIST = MMCSD_EMMC_Baremetal_Regression_TestApp
MMCSD_EMMC_Baremetal_Regression_TestApp_RELPATH = ti/drv/mmcsd/test/MMCSD_EMMC_Baremetal_Regression_TestApp
MMCSD_EMMC_Baremetal_Regression_TestApp_PATH = $(PDK_MMCSD_COMP_PATH)/test/MMCSD_EMMC_Baremetal_Regression_TestApp
MMCSD_EMMC_Baremetal_Regression_TestApp_BOARD_DEPENDENCY = yes
MMCSD_EMMC_Baremetal_Regression_TestApp_CORE_DEPENDENCY = no
MMCSD_EMMC_Baremetal_Regression_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
MMCSD_EMMC_Baremetal_Regression_TestApp_XDC_CONFIGURO = no
export MMCSD_EMMC_Baremetal_Regression_TestApp_COMP_LIST
export MMCSD_EMMC_Baremetal_Regression_TestApp_BOARD_DEPENDENCY
export MMCSD_EMMC_Baremetal_Regression_TestApp_CORE_DEPENDENCY
export MMCSD_EMMC_Baremetal_Regression_TestApp_MAKEFILE
export MMCSD_EMMC_Baremetal_Regression_TestApp_XDC_CONFIGURO
MMCSD_EMMC_Baremetal_Regression_TestApp_PKG_LIST = MMCSD_EMMC_Baremetal_Regression_TestApp
MMCSD_EMMC_Baremetal_Regression_TestApp_INCLUDE = $(MMCSD_EMMC_Baremetal_Regression_TestApp_PATH)
MMCSD_EMMC_Baremetal_Regression_TestApp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export MMCSD_EMMC_Baremetal_Regression_TestApp_BOARDLIST
MMCSD_EMMC_Baremetal_Regression_TestApp_$(SOC)_CORELIST = $(drvmmcsd_$(SOC)_CORELIST)
export MMCSD_EMMC_Baremetal_Regression_TestApp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
MMCSD_EMMC_Baremetal_Regression_TestApp_SBL_APPIMAGEGEN = yes
export MMCSD_EMMC_Baremetal_Regression_TestApp_SBL_APPIMAGEGEN
endif

export drvmmcsd_LIB_LIST
export mmcsd_LIB_LIST

mmcsd_EXAMPLE_LIST += MMCSD_Baremetal_TestApp
mmcsd_EXAMPLE_LIST += MMCSD_Baremetal_DMA_TestApp
mmcsd_EXAMPLE_LIST += MMCSD_Baremetal_EMMC_TestApp
mmcsd_EXAMPLE_LIST += MMCSD_Baremetal_EMMC_DMA_TestApp
mmcsd_EXAMPLE_LIST += MMCSD_Baremetal_Regression_TestApp
mmcsd_EXAMPLE_LIST += MMCSD_EMMC_Baremetal_Regression_TestApp

export mmcsd_EXAMPLE_LIST

drvmmcsd_EXAMPLE_LIST = $(mmcsd_EXAMPLE_LIST)
export drvmmcsd_EXAMPLE_LIST

mmcsd_component_make_include := 1
endif
