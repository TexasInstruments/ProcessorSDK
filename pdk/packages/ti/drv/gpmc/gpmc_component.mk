#*******************************************************************************
#                                                                              *
# Copyright (c) 2016 - 2020 Texas Instruments Incorporated - http://www.ti.com *
#                        ALL RIGHTS RESERVED                                   *
#                                                                              *
#*******************************************************************************

# File: gpmc_component.mk
#       This file is component include make file of GPMC library.
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
ifeq ($(gpmc_component_make_include), )

# under other list
drvgpmc_BOARDLIST       = icev2AM335x evmAM437x am64x_evm
drvgpmc_SOCLIST         = am437x am335x am64x
drvgpmc_dma_SOCLIST     = am64x
drvgpmc_am437x_CORELIST = a9host
drvgpmc_am335x_CORELIST = a8host
drvgpmc_am64x_CORELIST  = $(DEFAULT_am64x_CORELIST)

############################
# gpmc package
# List of components included under gpmc lib
# The components included here are built and will be part of gpmc lib
############################
gpmc_LIB_LIST = gpmc gpmc_dma gpmc_profile gpmc_dma_profile gpmc_indp gpmc_profile_indp
drvgpmc_LIB_LIST = $(gpmc_LIB_LIST)

############################
# gpmc examples
# List of examples under gpmc
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
gpmc_EXAMPLE_LIST  = GPMC_Baremetal_TestApp GPMC_TestApp
gpmc_EXAMPLE_LIST += GPMC_Baremetal_Dma_TestApp GPMC_Dma_TestApp
gpmc_EXAMPLE_LIST += GPMC_Baremetal_Probing_Example
drvgpmc_EXAMPLE_LIST = $(gpmc_EXAMPLE_LIST)

#
# GPMC Modules
#

# GPMC LIB
gpmc_COMP_LIST = gpmc
gpmc_RELPATH = ti/drv/gpmc
gpmc_PATH = $(PDK_GPMC_COMP_PATH)
gpmc_LIBNAME = ti.drv.gpmc
export gpmc_LIBNAME
gpmc_LIBPATH = $(gpmc_PATH)/lib
export gpmc_LIBPATH
gpmc_OBJPATH = $(gpmc_RELPATH)/gpmc
export gpmc_OBJPATH
gpmc_MAKEFILE = -f build/makefile.mk
export gpmc_MAKEFILE
gpmc_BOARD_DEPENDENCY = no
gpmc_CORE_DEPENDENCY = no
gpmc_SOC_DEPENDENCY = yes
export gpmc_COMP_LIST
export gpmc_BOARD_DEPENDENCY
export gpmc_CORE_DEPENDENCY
export gpmc_SOC_DEPENDENCY
gpmc_PKG_LIST = gpmc
gpmc_INCLUDE = $(gpmc_PATH)
gpmc_SOCLIST = am437x am335x am64x
export gpmc_SOCLIST
gpmc_$(SOC)_CORELIST = $(drvgpmc_$(SOC)_CORELIST)
export gpmc_$(SOC)_CORELIST

# GPMC LIB DEVICE INDEPENDENT
gpmc_indp_COMP_LIST = gpmc_indp
gpmc_indp_RELPATH = ti/drv/gpmc
gpmc_indp_PATH = $(PDK_GPMC_COMP_PATH)
gpmc_indp_LIBNAME = ti.drv.gpmc
export gpmc_indp_LIBNAME
gpmc_indp_LIBPATH = $(gpmc_indp_PATH)/lib
export gpmc_indp_LIBPATH
gpmc_indp_OBJPATH = $(gpmc_indp_RELPATH)/gpmc_indp
export gpmc_indp_OBJPATH
gpmc_indp_MAKEFILE = -f build/makefile_indp.mk
export gpmc_indp_MAKEFILE
gpmc_indp_BOARD_DEPENDENCY = no
gpmc_indp_CORE_DEPENDENCY = no
gpmc_indp_SOC_DEPENDENCY = no
export gpmc_indp_COMP_LIST
export gpmc_indp_BOARD_DEPENDENCY
export gpmc_indp_CORE_DEPENDENCY
export gpmc_indp_SOC_DEPENDENCY
gpmc_indp_PKG_LIST = gpmc_indp
gpmc_indp_INCLUDE = $(gpmc_indp_PATH)
gpmc_indp_SOCLIST = am437x am335x
export gpmc_indp_SOCLIST
gpmc_indp_$(SOC)_CORELIST = $(drvgpmc_$(SOC)_CORELIST)
export gpmc_indp_$(SOC)_CORELIST

# GPMC PROFILING SOC LIB
gpmc_profile_COMP_LIST = gpmc_profile
gpmc_profile_RELPATH = ti/drv/gpmc
gpmc_profile_PATH = $(PDK_GPMC_COMP_PATH)
gpmc_profile_LIBNAME = ti.drv.gpmc.profiling
export gpmc_profile_LIBNAME
gpmc_profile_LIBPATH = $(gpmc_profile_PATH)/lib
export gpmc_profile_LIBPATH
gpmc_profile_OBJPATH = $(gpmc_profile_RELPATH)/gpmc_profile
export gpmc_profile_OBJPATH
gpmc_profile_MAKEFILE = -f build/makefile_profile.mk
export gpmc_profile_MAKEFILE
gpmc_profile_BOARD_DEPENDENCY = no
gpmc_profile_CORE_DEPENDENCY = no
gpmc_profile_SOC_DEPENDENCY = yes
export gpmc_profile_COMP_LIST
export gpmc_profile_BOARD_DEPENDENCY
export gpmc_profile_CORE_DEPENDENCY
export gpmc_profile_SOC_DEPENDENCY
gpmc_profile_PKG_LIST = gpmc_profile
gpmc_profile_INCLUDE = $(gpmc_profile_PATH)
gpmc_profile_SOCLIST = am572x am571x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x
export gpmc_profile_SOCLIST
gpmc_profile_$(SOC)_CORELIST = $(drvgpmc_$(SOC)_CORELIST)
export gpmc_profile_$(SOC)_CORELIST

# GPMC PROFILING SOC INDEPENDENT LIB
gpmc_profile_indp_COMP_LIST = gpmc_profile_indp
gpmc_profile_indp_RELPATH = ti/drv/gpmc
gpmc_profile_indp_PATH = $(PDK_GPMC_COMP_PATH)
gpmc_profile_indp_LIBNAME = ti.drv.gpmc.profiling
export gpmc_profile_indp_LIBNAME
gpmc_profile_indp_LIBPATH = $(gpmc_profile_indp_PATH)/lib
export gpmc_profile_indp_LIBPATH
gpmc_profile_indp_OBJPATH = $(gpmc_profile_indp_RELPATH)/gpmc_profile_indp
export gpmc_profile_indp_OBJPATH
gpmc_profile_indp_MAKEFILE = -f build/makefile_profile_indp.mk
export gpmc_profile_indp_MAKEFILE
gpmc_profile_indp_BOARD_DEPENDENCY = no
gpmc_profile_indp_CORE_DEPENDENCY = no
gpmc_profile_indp_SOC_DEPENDENCY = no
export gpmc_profile_indp_COMP_LIST
export gpmc_profile_indp_BOARD_DEPENDENCY
export gpmc_profile_indp_CORE_DEPENDENCY
export gpmc_profile_indp_SOC_DEPENDENCY
gpmc_profile_indp_PKG_LIST = gpmc_profile_indp
gpmc_profile_indp_INCLUDE = $(gpmc_profile_indp_PATH)
gpmc_profile_indp_SOCLIST = am572x am571x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x
export gpmc_profile_indp_SOCLIST
gpmc_profile_indp_$(SOC)_CORELIST = $(drvgpmc_$(SOC)_CORELIST)
export gpmc_profile_indp_$(SOC)_CORELIST

# GPMC DMA LIB
gpmc_dma_COMP_LIST = gpmc_dma
gpmc_dma_RELPATH = ti/drv/gpmc
gpmc_dma_PATH = $(PDK_GPMC_COMP_PATH)
gpmc_dma_LIBNAME = ti.drv.gpmc.dma
export gpmc_dma_LIBNAME
gpmc_dma_LIBPATH = $(gpmc_dma_PATH)/lib
export gpmc_dma_LIBPATH
gpmc_dma_OBJPATH = $(gpmc_dma_RELPATH)/gpmc_dma
export gpmc_dma_OBJPATH
gpmc_dma_MAKEFILE = -f build/makefile_dma.mk
export gpmc_dma_MAKEFILE
gpmc_dma_BOARD_DEPENDENCY = no
gpmc_dma_CORE_DEPENDENCY = no
gpmc_dma_SOC_DEPENDENCY = yes
export gpmc_dma_COMP_LIST
export gpmc_dma_BOARD_DEPENDENCY
export gpmc_dma_CORE_DEPENDENCY
export gpmc_dma_SOC_DEPENDENCY
gpmc_dma_PKG_LIST = gpmc_dma
export gpmc_dma_PKG_LIST
gpmc_dma_INCLUDE = $(gpmc_dma_PATH)
gpmc_dma_SOCLIST = am64x
export gpmc_dma_SOCLIST
gpmc_dma_$(SOC)_CORELIST = $(drvgpmc_$(SOC)_CORELIST)
export gpmc_dma_$(SOC)_CORELIST

#
# GPMC Examples
#

# GPMC unit test baremetal app
GPMC_Baremetal_TestApp_COMP_LIST = GPMC_Baremetal_TestApp
GPMC_Baremetal_TestApp_RELPATH = ti/drv/gpmc/test
GPMC_Baremetal_TestApp_PATH = $(PDK_GPMC_COMP_PATH)/test
GPMC_Baremetal_TestApp_BOARD_DEPENDENCY = yes
GPMC_Baremetal_TestApp_CORE_DEPENDENCY = no
GPMC_Baremetal_TestApp_MAKEFILE = -f makefile IS_BAREMETAL=yes
export GPMC_Baremetal_TestApp_COMP_LIST
export GPMC_Baremetal_TestApp_BOARD_DEPENDENCY
export GPMC_Baremetal_TestApp_CORE_DEPENDENCY
export GPMC_Baremetal_TestApp_MAKEFILE
GPMC_Baremetal_TestApp_PKG_LIST = GPMC_Baremetal_TestApp
GPMC_Baremetal_TestApp_INCLUDE = $(GPMC_Baremetal_TestApp_PATH)
GPMC_Baremetal_TestApp_BOARDLIST = $(drvgpmc_BOARDLIST)
export GPMC_Baremetal_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
GPMC_Baremetal_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
export GPMC_Baremetal_TestApp_$(SOC)_CORELIST

GPMC_Baremetal_TestApp_SBL_APPIMAGEGEN = yes
export GPMC_Baremetal_TestApp_SBL_APPIMAGEGEN

# GPMC baremetal dma Flash Test app
GPMC_Baremetal_Dma_TestApp_COMP_LIST = GPMC_Baremetal_Dma_TestApp
GPMC_Baremetal_Dma_TestApp_RELPATH = ti/drv/gpmc/test
GPMC_Baremetal_Dma_TestApp_PATH = $(PDK_GPMC_COMP_PATH)/test
GPMC_Baremetal_Dma_TestApp_BOARD_DEPENDENCY = yes
GPMC_Baremetal_Dma_TestApp_CORE_DEPENDENCY = no
GPMC_Baremetal_Dma_TestApp_MAKEFILE = -f makefile IS_BAREMETAL=yes DMA=enable
export GPMC_Baremetal_Dma_TestApp_COMP_LIST
export GPMC_Baremetal_Dma_TestApp_BOARD_DEPENDENCY
export GPMC_Baremetal_Dma_TestApp_CORE_DEPENDENCY
export GPMC_Baremetal_Dma_TestApp_MAKEFILE
GPMC_Baremetal_Dma_TestApp_PKG_LIST = GPMC_Baremetal_Dma_TestApp
GPMC_Baremetal_Dma_TestApp_INCLUDE = $(GPMC_Baremetal_Dma_TestApp_PATH)
GPMC_Baremetal_Dma_TestApp_BOARDLIST = am64x_evm
export GPMC_Baremetal_Dma_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
GPMC_Baremetal_Dma_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
export GPMC_Baremetal_Dma_TestApp_$(SOC)_CORELIST

GPMC_Baremetal_Dma_TestApp_SBL_APPIMAGEGEN = yes
export GPMC_Baremetal_Dma_TestApp_SBL_APPIMAGEGEN

# GPMC unit test rtos app
GPMC_TestApp_COMP_LIST = GPMC_TestApp
GPMC_TestApp_RELPATH = ti/drv/gpmc/test
GPMC_TestApp_PATH = $(PDK_GPMC_COMP_PATH)/test
GPMC_TestApp_BOARD_DEPENDENCY = yes
GPMC_TestApp_CORE_DEPENDENCY = no
GPMC_TestApp_MAKEFILE = -f makefile
GPMC_TestApp_XDC_CONFIGURO = yes
export GPMC_TestApp_COMP_LIST
export GPMC_TestApp_BOARD_DEPENDENCY
export GPMC_TestApp_CORE_DEPENDENCY
export GPMC_TestApp_MAKEFILE
export GPMC_TestApp_XDC_CONFIGURO
GPMC_TestApp_PKG_LIST = GPMC_TestApp
GPMC_TestApp_INCLUDE = $(GPMC_TestApp_PATH)
GPMC_TestApp_BOARDLIST = $(drvgpmc_BOARDLIST)
export GPMC_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
GPMC_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
export GPMC_TestApp_$(SOC)_CORELIST
GPMC_TestApp_SBL_APPIMAGEGEN = yes
export GPMC_TestApp_SBL_APPIMAGEGEN

# GPMC rtos DMA Flash Test app
GPMC_Dma_TestApp_COMP_LIST = GPMC_Dma_TestApp
GPMC_Dma_TestApp_RELPATH = ti/drv/gpmc/test
GPMC_Dma_TestApp_PATH = $(PDK_GPMC_COMP_PATH)/test
GPMC_Dma_TestApp_BOARD_DEPENDENCY = yes
GPMC_Dma_TestApp_CORE_DEPENDENCY = no
GPMC_Dma_TestApp_XDC_CONFIGURO = yes
GPMC_Dma_TestApp_MAKEFILE = -f makefile DMA=enable
export GPMC_Dma_TestApp_COMP_LIST
export GPMC_Dma_TestApp_BOARD_DEPENDENCY
export GPMC_Dma_TestApp_CORE_DEPENDENCY
export GPMC_Dma_TestApp_XDC_CONFIGURO
export GPMC_Dma_TestApp_MAKEFILE
GPMC_Dma_TestApp_PKG_LIST = GPMC_Dma_TestApp
GPMC_Dma_TestApp_INCLUDE = $(GPMC_Dma_TestApp_PATH)
GPMC_Dma_TestApp_BOARDLIST = am64x_evm
export GPMC_Dma_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
GPMC_Dma_TestApp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
endif
export GPMC_Dma_TestApp_$(SOC)_CORELIST

GPMC_Dma_TestApp_SBL_APPIMAGEGEN = yes
export GPMC_Dma_TestApp_SBL_APPIMAGEGEN


# GPMC probing example baremetal app
GPMC_Baremetal_Probing_Example_COMP_LIST = GPMC_Baremetal_Probing_Example
GPMC_Baremetal_Probing_Example_RELPATH = ti/drv/gpmc/example
GPMC_Baremetal_Probing_Example_PATH = $(PDK_GPMC_COMP_PATH)/example
GPMC_Baremetal_Probing_Example_BOARD_DEPENDENCY = yes
GPMC_Baremetal_Probing_Example_CORE_DEPENDENCY = yes
GPMC_Baremetal_Probing_Example_MAKEFILE = -f makefile IS_BAREMETAL=yes
export GPMC_Baremetal_Probing_Example_COMP_LIST
export GPMC_Baremetal_Probing_Example_BOARD_DEPENDENCY
export GPMC_Baremetal_Probing_Example_CORE_DEPENDENCY
export GPMC_Baremetal_Probing_Example_MAKEFILE
GPMC_Baremetal_Probing_Example_PKG_LIST = GPMC_Baremetal_Probing_Example
GPMC_Baremetal_Probing_Example_INCLUDE = $(GPMC_Baremetal_Probing_Example_PATH)
GPMC_Baremetal_Probing_Example_BOARDLIST = am64x_evm
export GPMC_Baremetal_Probing_Example_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
GPMC_Baremetal_Probing_Example_$(SOC)_CORELIST = mcu1_0
endif
export GPMC_Baremetal_Probing_Example_$(SOC)_CORELIST

GPMC_Baremetal_Probing_Example_SBL_APPIMAGEGEN = yes
export GPMC_Baremetal_Probing_Example_SBL_APPIMAGEGEN


export drvgpmc_LIB_LIST
export gpmc_LIB_LIST
export gpmc_EXAMPLE_LIST
export drvgpmc_EXAMPLE_LIST

gpmc_component_make_include := 1
endif
