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

# File: spi_component.mk
#       This file is component include make file of SPI library.
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
ifeq ($(spi_component_make_include), )

drvspi_RTOS_LIST       = $(DEFAULT_RTOS_LIST)
drvspi_BOARDLIST       = j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvspi_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvspi_dma_SOCLIST     = j721e j7200 j721s2 j784s4 j742s2
drvspi_j721e_CORELIST              = $(DEFAULT_j721e_CORELIST)
drvspi_j721e_CORELISTARM           = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721e_CORELIST_DMA          = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721e_CORELIST_DMA_CACHE    = mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721e_CORELISTARM_CACHE     = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j7200_CORELIST              = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvspi_j7200_CORELIST_CACHE        = mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvspi_j721s2_CORELIST             = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721s2_CORELIST_CACHE       = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721s2_CORELIST_DMA         = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j721s2_CORELIST_DMA_CACHE   = mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvspi_j784s4_CORELIST             = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j784s4_CORELIST_CACHE       = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j784s4_CORELIST_DMA         = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j784s4_CORELIST_DMA_CACHE   = mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j742s2_CORELIST             = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j742s2_CORELIST_CACHE       = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j742s2_CORELIST_DMA         = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvspi_j742s2_CORELIST_DMA_CACHE   = mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1

ospi_RTOS_LIST       = $(DEFAULT_RTOS_LIST)
define DRV_OSPI_RTOS_BOARDLIST_RULE
ospi_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST))
endef
DRV_OSPI_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(ospi_RTOS_LIST), $(call DRV_OSPI_RTOS_BOARDLIST_RULE,$(curos)))
$(eval ${DRV_OSPI_RTOS_BOARDLIST_MACRO_LIST})
############################
# spi package
# List of components included under spi lib
# The components included here are built and will be part of spi lib
############################
spi_LIB_LIST = spi spi_dma spi_indp
drvspi_LIB_LIST = $(spi_LIB_LIST)

############################
# spi Firmwares
# All the firmware mentioned in list are built when build target is called
# List below all frimware for allowed values
############################
spi_FIRM_LIST = icss_espi
drvspi_FIRM_LIST = $(spi_FIRM_LIST)

############################
# spi examples
# List of examples under spi
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
spi_EXAMPLE_LIST = drv_mcspi_loopback_app
spi_EXAMPLE_LIST += mcspi_baremetal_master_testapp mcspi_baremetal_slave_testapp
spi_EXAMPLE_LIST += mcspi_baremetal_master_dma_testapp mcspi_baremetal_slave_dma_testapp
spi_EXAMPLE_LIST += OSPI_Baremetal_Flash_TestApp  OSPI_Baremetal_Flash_Dma_TestApp OSPI_Flash_TestApp OSPI_Flash_Dma_TestApp OSPI_Baremetal_Flash_Cache_TestApp  OSPI_Baremetal_Flash_Dma_Cache_TestApp OSPI_Flash_Cache_TestApp OSPI_Flash_Dma_Cache_TestApp OSPI_NAND_Flash_OTP_TestApp QSPI_Baremetal_Flash_TestApp QSPI_Baremetal_Flash_Dma_TestApp QSPI_FileFlashWrite_Dma_TestApp

#
# SPI Modules
#

# SPI LIB
spi_COMP_LIST = spi
spi_RELPATH = ti/drv/spi
spi_PATH = $(PDK_SPI_COMP_PATH)
spi_LIBNAME = ti.drv.spi
export spi_LIBNAME
spi_LIBPATH = $(spi_PATH)/lib
export spi_LIBPATH
spi_OBJPATH = $(spi_RELPATH)/spi
export spi_OBJPATH
spi_MAKEFILE = -f build/makefile.mk
export spi_MAKEFILE
spi_BOARD_DEPENDENCY = no
spi_CORE_DEPENDENCY = no
spi_SOC_DEPENDENCY = yes
export spi_COMP_LIST
export spi_BOARD_DEPENDENCY
export spi_CORE_DEPENDENCY
export spi_SOC_DEPENDENCY
spi_PKG_LIST = spi
export spi_PKG_LIST
spi_INCLUDE = $(spi_PATH)
spi_SOCLIST = $(drvspi_SOCLIST)
export spi_SOCLIST
spi_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
export spi_$(SOC)_CORELIST

# SPI DMA LIB
spi_dma_COMP_LIST = spi_dma
spi_dma_RELPATH = ti/drv/spi
spi_dma_PATH = $(PDK_SPI_COMP_PATH)
spi_dma_LIBNAME = ti.drv.spi.dma
export spi_dma_LIBNAME
spi_dma_LIBPATH = $(spi_dma_PATH)/lib
export spi_dma_LIBPATH
spi_dma_OBJPATH = $(spi_dma_RELPATH)/spi_dma
export spi_dma_OBJPATH
spi_dma_MAKEFILE = -f build/makefile_dma.mk
export spi_dma_MAKEFILE
spi_dma_BOARD_DEPENDENCY = no
spi_dma_CORE_DEPENDENCY = no
spi_dma_SOC_DEPENDENCY = yes
export spi_dma_COMP_LIST
export spi_dma_BOARD_DEPENDENCY
export spi_dma_CORE_DEPENDENCY
export spi_dma_SOC_DEPENDENCY
spi_dma_PKG_LIST = spi_dma
export spi_dma_PKG_LIST
spi_dma_INCLUDE = $(spi_dma_PATH)
spi_dma_SOCLIST = $(drvspi_dma_SOCLIST)
export spi_dma_SOCLIST
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
spi_dma_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST_DMA)
else
spi_dma_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
endif
export spi_dma_$(SOC)_CORELIST

#
# SPI Firmwares
#
icss_espi_COMP_LIST = icss_espi
# temporary fix for nightly build
# icss_espi_RELPATH = ti/drv/spi/firmware/icss_espi
icss_espi_RELPATH = icss_espi
icss_espi_PATH = $(PDK_SPI_COMP_PATH)/firmware/icss_espi
icss_espi_HEADERNAME = icss_espi
export icss_espi_HEADERNAME
icss_espi_HEADERPATH = $(icss_espi_PATH)/bin
export icss_espi_HEADERPATH
icss_espi_OBJPATH = $(icss_espi_RELPATH)
export icss_espi_OBJPATH
icss_espi_MAKEFILE = -f ../../build/makefile_icss_espi.mk
export icss_espi_MAKEFILE
icss_espi_BOARD_DEPENDENCY = no
icss_espi_CORE_DEPENDENCY = yes
icss_espi_SOC_DEPENDENCY = yes
export icss_espi_COMP_LIST
export icss_espi_BOARD_DEPENDENCY
export icss_espi_CORE_DEPENDENCY
export icss_espi_SOC_DEPENDENCY
icss_espi_PKG_LIST = icss_espi
export icss_espi_PKG_LIST
icss_espi_INCLUDE = $(icss_espi_PATH)
icss_espi_SOCLIST = am437x am335x
export icss_espii_SOCLIST
icss_espi_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
export icss_espi_$(SOC)_CORELIST

#
# SPI Examples
#
# SPI Polling mode Test app
drv_mcspi_loopback_app_COMP_LIST = drv_mcspi_loopback_app
drv_mcspi_loopback_app_RELPATH = ti/drv/spi/example/mcspiLoopbackApp
drv_mcspi_loopback_app_PATH = $(PDK_SPI_COMP_PATH)/example/mcspiLoopbackApp
drv_mcspi_loopback_app_BOARD_DEPENDENCY = yes
drv_mcspi_loopback_app_CORE_DEPENDENCY = no
drv_mcspi_loopback_app_XDC_CONFIGURO = yes
export drv_mcspi_loopback_app_COMP_LIST
export drv_mcspi_loopback_app_BOARD_DEPENDENCY
export drv_mcspi_loopback_app_CORE_DEPENDENCY
export drv_mcspi_loopback_app_XDC_CONFIGURO
drv_mcspi_loopback_app_PKG_LIST = drv_mcspi_loopback_app
drv_mcspi_loopback_app_INCLUDE = $(drv_mcspi_loopback_app_PATH)
drv_mcspi_loopback_app_SOCLIST = tda2xx tda2px tda2ex tda3xx dra72x dra75x dra78x
export drv_mcspi_loopback_app_SOCLIST
drv_mcspi_loopback_app_$(SOC)_CORELIST = $(spi_$(SOC)_CORELIST)
export drv_mcspi_loopback_app_$(SOC)_CORELIST

# SPI baremetal master Test app
mcspi_baremetal_master_testapp_COMP_LIST = mcspi_baremetal_master_testapp
mcspi_baremetal_master_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_master_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
mcspi_baremetal_master_testapp_BOARD_DEPENDENCY = yes
mcspi_baremetal_master_testapp_CORE_DEPENDENCY = yes
mcspi_baremetal_master_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export mcspi_baremetal_master_testapp_COMP_LIST
export mcspi_baremetal_master_testapp_BOARD_DEPENDENCY
export mcspi_baremetal_master_testapp_CORE_DEPENDENCY
export mcspi_baremetal_master_testapp_MAKEFILE
mcspi_baremetal_master_testapp_PKG_LIST = mcspi_baremetal_master_testapp
mcspi_baremetal_master_testapp_INCLUDE = $(mcspi_baremetal_master_testapp_PATH)
mcspi_baremetal_master_testapp_BOARDLIST = $(drvspi_BOARDLIST)
export mcspi_baremetal_master_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx))
mcspi_baremetal_master_testapp_$(SOC)_CORELIST = mcu1_0
else
mcspi_baremetal_master_testapp_$(SOC)_CORELIST = mcu2_0
endif
export mcspi_baremetal_master_testapp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
export mcspi_baremetal_master_testapp_SBL_APPIMAGEGEN = yes
endif

# SPI baremetal slave Test app
mcspi_baremetal_slave_testapp_COMP_LIST = mcspi_baremetal_slave_testapp
mcspi_baremetal_slave_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_slave_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
mcspi_baremetal_slave_testapp_BOARD_DEPENDENCY = yes
mcspi_baremetal_slave_testapp_CORE_DEPENDENCY = yes
mcspi_baremetal_slave_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export mcspi_baremetal_slave_testapp_COMP_LIST
export mcspi_baremetal_slave_testapp_BOARD_DEPENDENCY
export mcspi_baremetal_slave_testapp_CORE_DEPENDENCY
export mcspi_baremetal_slave_testapp_MAKEFILE
mcspi_baremetal_slave_testapp_PKG_LIST = mcspi_baremetal_slave_testapp
mcspi_baremetal_slave_testapp_INCLUDE = $(mcspi_baremetal_slave_testapp_PATH)
mcspi_baremetal_slave_testapp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export mcspi_baremetal_slave_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx))
mcspi_baremetal_slave_testapp_$(SOC)_CORELIST = mcu1_1
else
mcspi_baremetal_slave_testapp_$(SOC)_CORELIST = mcu2_1
endif
export mcspi_baremetal_slave_testapp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 j721s2 j784s4 j742s2))
export mcspi_baremetal_slave_testapp_SBL_APPIMAGEGEN = yes
endif

# SPI baremetal MasterSlave Test app
export mcspi_baremetal_masterslave_testapp_COMP_LIST = mcspi_baremetal_masterslave_testapp
mcspi_baremetal_masterslave_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_masterslave_testapp_BINPATH = $(PDK_INSTALL_PATH)/ti/binary/mcspi_baremetal_masterslave_testapp/bin
mcspi_baremetal_masterslave_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
export mcspi_baremetal_masterslave_testapp_BOARD_DEPENDENCY = yes
export mcspi_baremetal_masterslave_testapp_CORE_DEPENDENCY = yes
export mcspi_baremetal_masterslave_testapp_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export mcspi_baremetal_masterslave_testapp_MAKEFILE = -f$(PDK_SPI_COMP_PATH)/example/mcspi_slavemode/mcspi_multicore_testapp.mk BUILD_OS_TYPE=baremetal
export mcspi_baremetal_masterslave_testapp_DEPENDS_ON = mcspi_baremetal_master_testapp mcspi_baremetal_slave_testapp
mcspi_baremetal_masterslave_testapp_PKG_LIST = mcspi_baremetal_masterslave_testapp
mcspi_baremetal_masterslave_testapp_INCLUDE = $(mcspi_baremetal_masterslave_testapp_PATH)
export mcspi_baremetal_masterslave_testapp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export mcspi_baremetal_masterslave_testapp_$(SOC)_CORELIST:= mcu2_1
export mcspi_baremetal_masterslave_testapp_SBL_APPIMAGEGEN = no
spi_EXAMPLE_LIST +=mcspi_baremetal_masterslave_testapp

# SPI baremetal master Test app
mcspi_baremetal_master_dma_testapp_COMP_LIST = mcspi_baremetal_master_dma_testapp
mcspi_baremetal_master_dma_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_master_dma_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
mcspi_baremetal_master_dma_testapp_BOARD_DEPENDENCY = yes
mcspi_baremetal_master_dma_testapp_CORE_DEPENDENCY = yes
mcspi_baremetal_master_dma_testapp_MAKEFILE = -f makefile  BUILD_OS_TYPE=baremetal DMA=enable
export mcspi_baremetal_master_dma_testapp_COMP_LIST
export mcspi_baremetal_master_dma_testapp_BOARD_DEPENDENCY
export mcspi_baremetal_master_dma_testapp_CORE_DEPENDENCY
export mcspi_baremetal_master_dma_testapp_MAKEFILE
mcspi_baremetal_master_dma_testapp_PKG_LIST = mcspi_baremetal_master_dma_testapp
mcspi_baremetal_master_dma_testapp_INCLUDE = $(mcspi_baremetal_master_dma_testapp_PATH)
mcspi_baremetal_master_dma_testapp_BOARDLIST = $(drvspi_BOARDLIST)
export mcspi_baremetal_master_dma_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx))
mcspi_baremetal_master_dma_testapp_$(SOC)_CORELIST = mcu1_0
else
mcspi_baremetal_master_dma_testapp_$(SOC)_CORELIST = mcu2_0
endif
export mcspi_baremetal_master_dma_testapp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
export mcspi_baremetal_master_dma_testapp_SBL_APPIMAGEGEN = yes
endif

# SPI baremetal slave Test app
mcspi_baremetal_slave_dma_testapp_COMP_LIST = mcspi_baremetal_slave_dma_testapp
mcspi_baremetal_slave_dma_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_slave_dma_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
mcspi_baremetal_slave_dma_testapp_BOARD_DEPENDENCY = yes
mcspi_baremetal_slave_dma_testapp_CORE_DEPENDENCY = yes
mcspi_baremetal_slave_dma_testapp_MAKEFILE = -f makefile  BUILD_OS_TYPE=baremetal DMA=enable
export mcspi_baremetal_slave_dma_testapp_COMP_LIST
export mcspi_baremetal_slave_dma_testapp_BOARD_DEPENDENCY
export mcspi_baremetal_slave_dma_testapp_CORE_DEPENDENCY
export mcspi_baremetal_slave_dma_testapp_MAKEFILE
mcspi_baremetal_slave_dma_testapp_PKG_LIST = mcspi_baremetal_slave_dma_testapp
mcspi_baremetal_slave_dma_testapp_INCLUDE = $(mcspi_baremetal_slave_dma_testapp_PATH)
mcspi_baremetal_slave_dma_testapp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export mcspi_baremetal_slave_dma_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx))
mcspi_baremetal_slave_dma_testapp_$(SOC)_CORELIST = mcu1_1
else
mcspi_baremetal_slave_dma_testapp_$(SOC)_CORELIST = mcu2_1
endif
export mcspi_baremetal_slave_dma_testapp_$(SOC)_CORELIST
ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 j721s2 j784s4 j742s2))
export mcspi_baremetal_slave_dma_testapp_SBL_APPIMAGEGEN = yes
endif

# SPI baremetal MasterSlave Dma Test app
export mcspi_baremetal_masterslave_dma_testapp_COMP_LIST = mcspi_baremetal_masterslave_dma_testapp
mcspi_baremetal_masterslave_dma_testapp_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_baremetal_masterslave_dma_testapp_BINPATH = $(PDK_INSTALL_PATH)/ti/binary/mcspi_baremetal_masterslave_dma_testapp/bin
mcspi_baremetal_masterslave_dma_testapp_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
export mcspi_baremetal_masterslave_dma_testapp_BOARD_DEPENDENCY = yes
export mcspi_baremetal_masterslave_dma_testapp_CORE_DEPENDENCY = yes
export mcspi_baremetal_masterslave_dma_testapp_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export mcspi_baremetal_masterslave_dma_testapp_MAKEFILE = -f$(PDK_SPI_COMP_PATH)/example/mcspi_slavemode/mcspi_multicore_testapp.mk  BUILD_OS_TYPE=baremetal DMA=enable
export mcspi_baremetal_masterslave_dma_testapp_DEPENDS_ON = mcspi_baremetal_master_dma_testapp mcspi_baremetal_slave_dma_testapp
mcspi_baremetal_masterslave_dma_testapp_PKG_LIST = mcspi_baremetal_masterslave_dma_testapp
mcspi_baremetal_masterslave_dma_testapp_INCLUDE = $(mcspi_baremetal_masterslave_dma_testapp_PATH)
export mcspi_baremetal_masterslave_dma_testapp_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export mcspi_baremetal_masterslave_dma_testapp_$(SOC)_CORELIST:= mcu2_1
export mcspi_baremetal_masterslave_dma_testapp_SBL_APPIMAGEGEN = no
spi_EXAMPLE_LIST +=mcspi_baremetal_masterslave_dma_testapp

# SPI master RTOS Test apps
define mcspi_master_testapp_RULE

    export mcspi_master_testapp_$(1)_COMP_LIST = mcspi_master_testapp_$(1)
    export mcspi_master_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
    export mcspi_master_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
    export mcspi_master_testapp_$(1)_BOARD_DEPENDENCY = yes
    export mcspi_master_testapp_$(1)_CORE_DEPENDENCY = yes
    export mcspi_master_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    mcspi_master_testapp_$(1)_PKG_LIST = mcspi_master_testapp_$(1)
    mcspi_master_testapp_$(1)_INCLUDE = $(mcspi_master_testapp_$(1)_PATH)
    export mcspi_master_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST))

    ifeq ($(SOC),$(filter $(SOC), am65xx))
    export mcspi_master_testapp_$(1)_$(SOC)_CORELIST = mcu1_0
    else
    export mcspi_master_testapp_$(1)_$(SOC)_CORELIST = mcu2_0
    endif

    ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
        export mcspi_master_testapp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        spi_EXAMPLE_LIST += mcspi_master_testapp_$(1)
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
           spi_EXAMPLE_LIST += mcspi_master_testapp_$(1)
        endif
    endif

endef

mcspi_master_testapp_MACRO_LIST := $(foreach curos,$(drvspi_RTOS_LIST),$(call mcspi_master_testapp_RULE,$(curos)))

$(eval ${mcspi_master_testapp_MACRO_LIST})


# SPI slave RTOS Test apps
define mcspi_slave_testapp_RULE

    export mcspi_slave_testapp_$(1)_COMP_LIST = mcspi_slave_testapp_$(1)
    export mcspi_slave_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
    export mcspi_slave_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
    export mcspi_slave_testapp_$(1)_BOARD_DEPENDENCY = yes
    export mcspi_slave_testapp_$(1)_CORE_DEPENDENCY = yes
    export mcspi_slave_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)

    mcspi_slave_testapp_$(1)_PKG_LIST = mcspi_slave_testapp_$(1)
    mcspi_slave_testapp_$(1)_INCLUDE = $(mcspi_slave_testapp_$(1)_PATH)
    export mcspi_slave_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm j7200_evm j784s4_evm j742s2_evm j721s2_evm)

    ifeq ($(SOC),$(filter $(SOC), am65xx))
    export mcspi_slave_testapp_$(1)_$(SOC)_CORELIST = mcu1_1
    else
    export mcspi_slave_testapp_$(1)_$(SOC)_CORELIST = mcu2_1
    endif

    ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
        export mcspi_slave_testapp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        spi_EXAMPLE_LIST += mcspi_slave_testapp_$(1)
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
          spi_EXAMPLE_LIST += mcspi_slave_testapp_$(1)
        endif
    endif
endef

mcspi_slave_testapp_MACRO_LIST := $(foreach curos,$(drvspi_RTOS_LIST),$(call mcspi_slave_testapp_RULE,$(curos)))

$(eval ${mcspi_slave_testapp_MACRO_LIST})

# SPI MasterSlave RTOS Test apps
define mcspi_masterslave_testapp_RULE

export mcspi_masterslave_testapp_$(1)_COMP_LIST = mcspi_masterslave_testapp_$(1)
mcspi_masterslave_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_masterslave_testapp_$(1)_BINPATH = $(PDK_INSTALL_PATH)/ti/binary/mcspi_masterslave_testapp_$(1)/bin
mcspi_masterslave_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
export mcspi_masterslave_testapp_$(1)_BOARD_DEPENDENCY = yes
export mcspi_masterslave_testapp_$(1)_CORE_DEPENDENCY = yes
export mcspi_masterslave_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export mcspi_masterslave_testapp_$(1)_MAKEFILE =  -f$(PDK_SPI_COMP_PATH)/example/mcspi_slavemode/mcspi_multicore_testapp.mk BUILD_OS_TYPE=$(1)
export mcspi_masterslave_testapp_$(1)_DEPENDS_ON=mcspi_master_testapp_$(1) mcspi_slave_testapp_$(1)
mcspi_masterslave_testapp_$(1)_PKG_LIST = mcspi_masterslave_testapp_$(1)
mcspi_masterslave_testapp_$(1)_INCLUDE = $(mcspi_masterslave_testapp_$(1)_PATH)
export mcspi_masterslave_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721s2_evm j721e_evm j7200_evm j784s4_evm j742s2_evm)
export mcspi_masterslave_testapp_$(1)_$(SOC)_CORELIST := mcu2_1
export mcspi_masterslave_testapp_SBL_APPIMAGEGEN = no
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += mcspi_masterslave_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += mcspi_masterslave_testapp_$(1)
endif
endif
endef

mcspi_masterslave_testapp_MACRO_LIST := $(foreach curos, $(filter-out tirtos, $(drvipc_RTOS_LIST)), $(call mcspi_masterslave_testapp_RULE,$(curos)))

$(eval ${mcspi_masterslave_testapp_MACRO_LIST})

# SPI master DMA RTOS Test apps
define mcspi_master_dma_testapp_RULE

    export mcspi_master_dma_testapp_$(1)_COMP_LIST = mcspi_master_dma_testapp_$(1)
    export mcspi_master_dma_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
    export mcspi_master_dma_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
    export mcspi_master_dma_testapp_$(1)_BOARD_DEPENDENCY = yes
    export mcspi_master_dma_testapp_$(1)_CORE_DEPENDENCY = yes
    export mcspi_master_dma_testapp_$(1)_MAKEFILE = -f makefile DMA=enable BUILD_OS_TYPE=$(1)

    mcspi_master_dma_testapp_$(1)_PKG_LIST = mcspi_master_dma_testapp_$(1)
    mcspi_master_dma_testapp_$(1)_INCLUDE = $(mcspi_master_dma_testapp_$(1)_PATH)
    export mcspi_master_dma_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST))

    ifeq ($(SOC),$(filter $(SOC), am65xx))
    export mcspi_master_dma_testapp_$(1)_$(SOC)_CORELIST = mcu1_0
    else
    export mcspi_master_dma_testapp_$(1)_$(SOC)_CORELIST = mcu2_0
    endif

    ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
        export mcspi_master_dma_testapp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        spi_EXAMPLE_LIST += mcspi_master_dma_testapp_$(1)
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
           spi_EXAMPLE_LIST += mcspi_master_dma_testapp_$(1)
        endif
    endif
endef

mcspi_master_dma_testapp_MACRO_LIST := $(foreach curos,$(drvspi_RTOS_LIST),$(call mcspi_master_dma_testapp_RULE,$(curos)))

$(eval ${mcspi_master_dma_testapp_MACRO_LIST})


# SPI slave DMA RTOS Test apps
define mcspi_slave_dma_testapp_RULE

    export mcspi_slave_dma_testapp_$(1)_COMP_LIST = mcspi_slave_dma_testapp_$(1)
    export mcspi_slave_dma_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
    export mcspi_slave_dma_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
    export mcspi_slave_dma_testapp_$(1)_BOARD_DEPENDENCY = yes
    export mcspi_slave_dma_testapp_$(1)_CORE_DEPENDENCY = yes
    export mcspi_slave_dma_testapp_$(1)_MAKEFILE = -f makefile DMA=enable BUILD_OS_TYPE=$(1)

    mcspi_slave_dma_testapp_$(1)_PKG_LIST = mcspi_slave_dma_testapp_$(1)
    mcspi_slave_dma_testapp_$(1)_INCLUDE = $(mcspi_slave_dma_testapp_$(1)_PATH)
    export mcspi_slave_dma_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm j7200_evm j784s4_evm j742s2_evm j721s2_evm)

    ifeq ($(SOC),$(filter $(SOC), am65xx))
    export mcspi_slave_dma_testapp_$(1)_$(SOC)_CORELIST = mcu1_1
    else
    export mcspi_slave_dma_testapp_$(1)_$(SOC)_CORELIST = mcu2_1
    endif

    ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
        export mcspi_slave_dma_testapp_$(1)_SBL_APPIMAGEGEN = yes
    endif

    ifneq ($(1),$(filter $(1), safertos))
        spi_EXAMPLE_LIST += mcspi_slave_dma_testapp_$(1)
    else
        ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
           spi_EXAMPLE_LIST += mcspi_slave_dma_testapp_$(1)
        endif
    endif
endef

mcspi_slave_dma_testapp_MACRO_LIST := $(foreach curos,$(drvspi_RTOS_LIST),$(call mcspi_slave_dma_testapp_RULE,$(curos)))

$(eval ${mcspi_slave_dma_testapp_MACRO_LIST})

# SPI MasterSlave DMA RTOS Test apps
define mcspi_masterslave_dma_testapp_RULE

export mcspi_masterslave_dma_testapp_$(1)_COMP_LIST = mcspi_masterslave_dma_testapp_$(1)
mcspi_masterslave_dma_testapp_$(1)_RELPATH = ti/drv/spi/example/mcspi_slavemode
mcspi_masterslave_dma_testapp_$(1)_BINPATH = $(PDK_INSTALL_PATH)/ti/binary/mcspi_masterslave_dma_testapp_$(1)/bin
mcspi_masterslave_dma_testapp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/example/mcspi_slavemode
export mcspi_masterslave_dma_testapp_$(1)_BOARD_DEPENDENCY = yes
export mcspi_masterslave_dma_testapp_$(1)_CORE_DEPENDENCY = yes
export mcspi_masterslave_dma_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export mcspi_masterslave_dma_testapp_$(1)_MAKEFILE =  -f$(PDK_SPI_COMP_PATH)/example/mcspi_slavemode/mcspi_multicore_testapp.mk BUILD_OS_TYPE=$(1) DMA=enable
export mcspi_masterslave_dma_testapp_$(1)_DEPENDS_ON=mcspi_master_dma_testapp_$(1) mcspi_slave_dma_testapp_$(1)
mcspi_masterslave_dma_testapp_$(1)_PKG_LIST = mcspi_masterslave_dma_testapp_$(1)
mcspi_masterslave_dma_testapp_$(1)_INCLUDE = $(mcspi_masterslave_dma_testapp_$(1)_PATH)
export mcspi_masterslave_dma_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)),j721s2_evm j721e_evm j7200_evm j784s4_evm j742s2_evm )
export mcspi_masterslave_dma_testapp_$(1)_$(SOC)_CORELIST := mcu2_1
export mcspi_masterslave_dma_testapp_SBL_APPIMAGEGEN = no
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += mcspi_masterslave_dma_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += mcspi_masterslave_dma_testapp_$(1)
endif
endif
endef

mcspi_masterslave_dma_testapp_MACRO_LIST := $(foreach curos, $(filter-out tirtos, $(drvipc_RTOS_LIST)), $(call mcspi_masterslave_dma_testapp_RULE,$(curos)))

$(eval ${mcspi_masterslave_dma_testapp_MACRO_LIST})

# OSPI baremetal Flash Test app
OSPI_Baremetal_Flash_TestApp_COMP_LIST = OSPI_Baremetal_Flash_TestApp
OSPI_Baremetal_Flash_TestApp_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Baremetal_Flash_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
OSPI_Baremetal_Flash_TestApp_BOARD_DEPENDENCY = yes
OSPI_Baremetal_Flash_TestApp_CORE_DEPENDENCY = no
OSPI_Baremetal_Flash_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export OSPI_Baremetal_Flash_TestApp_COMP_LIST
export OSPI_Baremetal_Flash_TestApp_BOARD_DEPENDENCY
export OSPI_Baremetal_Flash_TestApp_CORE_DEPENDENCY
export OSPI_Baremetal_Flash_TestApp_MAKEFILE
OSPI_Baremetal_Flash_TestApp_PKG_LIST = OSPI_Baremetal_Flash_TestApp
OSPI_Baremetal_Flash_TestApp_INCLUDE = $(OSPI_Baremetal_Flash_TestApp_PATH)
OSPI_Baremetal_Flash_TestApp_BOARDLIST = $(drvspi_BOARDLIST)
export OSPI_Baremetal_Flash_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), j721e am64x))
OSPI_Baremetal_Flash_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELISTARM)
else
OSPI_Baremetal_Flash_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
endif
export OSPI_Baremetal_Flash_TestApp_$(SOC)_CORELIST

ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_TestApp_SBL_APPIMAGEGEN = yes
export OSPI_Baremetal_Flash_TestApp_SBL_APPIMAGEGEN
endif

# OSPI baremetal dma Flash Test app
OSPI_Baremetal_Flash_Dma_TestApp_COMP_LIST = OSPI_Baremetal_Flash_Dma_TestApp
OSPI_Baremetal_Flash_Dma_TestApp_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Baremetal_Flash_Dma_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
OSPI_Baremetal_Flash_Dma_TestApp_BOARD_DEPENDENCY = yes
OSPI_Baremetal_Flash_Dma_TestApp_CORE_DEPENDENCY = no
OSPI_Baremetal_Flash_Dma_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal DMA=enable
export OSPI_Baremetal_Flash_Dma_TestApp_COMP_LIST
export OSPI_Baremetal_Flash_Dma_TestApp_BOARD_DEPENDENCY
export OSPI_Baremetal_Flash_Dma_TestApp_CORE_DEPENDENCY
export OSPI_Baremetal_Flash_Dma_TestApp_MAKEFILE
OSPI_Baremetal_Flash_Dma_TestApp_PKG_LIST = OSPI_Baremetal_Flash_Dma_TestApp
OSPI_Baremetal_Flash_Dma_TestApp_INCLUDE = $(OSPI_Baremetal_Flash_Dma_TestApp_PATH)
OSPI_Baremetal_Flash_Dma_TestApp_BOARDLIST = $(drvspi_BOARDLIST)
export OSPI_Baremetal_Flash_Dma_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
OSPI_Baremetal_Flash_Dma_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELISTARM)
else
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_Dma_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST_DMA)
else
OSPI_Baremetal_Flash_Dma_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
endif
endif
export OSPI_Baremetal_Flash_Dma_TestApp_$(SOC)_CORELIST

ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_Dma_TestApp_SBL_APPIMAGEGEN = yes
export OSPI_Baremetal_Flash_Dma_TestApp_SBL_APPIMAGEGEN
endif

# OSPI rtos Flash Test app
define OSPI_FLASH_TESTAPP_RULE

export OSPI_Flash_TestApp_$(1)_COMP_LIST = OSPI_Flash_TestApp_$(1)
OSPI_Flash_TestApp_$(1)_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Flash_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
export OSPI_Flash_TestApp_$(1)_BOARD_DEPENDENCY = yes
export OSPI_Flash_TestApp_$(1)_CORE_DEPENDENCY = no
export OSPI_Flash_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
OSPI_Flash_TestApp_$(1)_PKG_LIST = OSPI_Flash_TestApp_$(1)
OSPI_Flash_TestApp_$(1)_INCLUDE = $(OSPI_Flash_TestApp_$(1)_PATH)
export OSPI_Flash_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST) )
ifeq ($(SOC),$(filter $(SOC), j721e am64x))
OSPI_Flash_TestApp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELISTARM))
else
OSPI_Flash_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
endif
export OSPI_Flash_TestApp_$(1)_$(SOC)_CORELIST
export OSPI_Flash_TestApp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += OSPI_Flash_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += OSPI_Flash_TestApp_$(1)
endif
endif

endef

OSPI_FLASH_TESTAPP_MACRO_LIST := $(foreach curos, $(ospi_RTOS_LIST) safertos, $(call OSPI_FLASH_TESTAPP_RULE,$(curos)))

$(eval ${OSPI_FLASH_TESTAPP_MACRO_LIST})

# OSPI rtos DMA Flash Test app
define OSPI_FLASH_DMA_TESTAPP_RULE

export OSPI_Flash_Dma_TestApp_$(1)_COMP_LIST = OSPI_Flash_Dma_TestApp_$(1)
OSPI_Flash_Dma_TestApp_$(1)_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Flash_Dma_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
export OSPI_Flash_Dma_TestApp_$(1)_BOARD_DEPENDENCY = yes
export OSPI_Flash_Dma_TestApp_$(1)_CORE_DEPENDENCY = no
export OSPI_Flash_Dma_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1) DMA=enable
OSPI_Flash_Dma_TestApp_$(1)_PKG_LIST = OSPI_Flash_Dma_TestApp_$(1)
OSPI_Flash_Dma_TestApp_$(1)_INCLUDE = $(OSPI_Flash_Dma_TestApp_$(1)_PATH)
export OSPI_Flash_Dma_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST) )
ifeq ($(SOC),$(filter $(SOC), am64x))
OSPI_Flash_Dma_TestApp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELISTARM))
else
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
OSPI_Flash_Dma_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST_DMA))
else
OSPI_Flash_Dma_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
endif
endif
export OSPI_Flash_Dma_TestApp_$(1)_$(SOC)_CORELIST
export OSPI_Flash_Dma_TestApp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += OSPI_Flash_Dma_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += OSPI_Flash_Dma_TestApp_$(1)
endif
endif

endef

OSPI_FLASH_DMA_TESTAPP_MACRO_LIST := $(foreach curos, $(ospi_RTOS_LIST), $(call OSPI_FLASH_DMA_TESTAPP_RULE,$(curos)))

$(eval ${OSPI_FLASH_DMA_TESTAPP_MACRO_LIST})

# OSPI baremetal Flash Test app with cacheable OSPI memory
OSPI_Baremetal_Flash_Cache_TestApp_COMP_LIST = OSPI_Baremetal_Flash_Cache_TestApp
OSPI_Baremetal_Flash_Cache_TestApp_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Baremetal_Flash_Cache_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
OSPI_Baremetal_Flash_Cache_TestApp_BOARD_DEPENDENCY = yes
OSPI_Baremetal_Flash_Cache_TestApp_CORE_DEPENDENCY = no
OSPI_Baremetal_Flash_Cache_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal CACHE=enable
export OSPI_Baremetal_Flash_Cache_TestApp_COMP_LIST
export OSPI_Baremetal_Flash_Cache_TestApp_BOARD_DEPENDENCY
export OSPI_Baremetal_Flash_Cache_TestApp_CORE_DEPENDENCY
export OSPI_Baremetal_Flash_Cache_TestApp_MAKEFILE
OSPI_Baremetal_Flash_Cache_TestApp_PKG_LIST = OSPI_Baremetal_Flash_Cache_TestApp
OSPI_Baremetal_Flash_Cache_TestApp_INCLUDE = $(OSPI_Baremetal_Flash_Cache_TestApp_PATH)
OSPI_Baremetal_Flash_Cache_TestApp_BOARDLIST = $(drvspi_BOARDLIST)
export OSPI_Baremetal_Flash_Cache_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), j721e am64x))
OSPI_Baremetal_Flash_Cache_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELISTARM_CACHE)
else
OSPI_Baremetal_Flash_Cache_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST_CACHE)
endif
export OSPI_Baremetal_Flash_Cache_TestApp_$(SOC)_CORELIST

ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_Cache_TestApp_SBL_APPIMAGEGEN = yes
export OSPI_Baremetal_Flash_Cache_TestApp_SBL_APPIMAGEGEN
endif

# OSPI baremetal dma Flash Test app with cacheable OSPI memory
OSPI_Baremetal_Flash_Dma_Cache_TestApp_COMP_LIST = OSPI_Baremetal_Flash_Dma_Cache_TestApp
OSPI_Baremetal_Flash_Dma_Cache_TestApp_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Baremetal_Flash_Dma_Cache_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
OSPI_Baremetal_Flash_Dma_Cache_TestApp_BOARD_DEPENDENCY = yes
OSPI_Baremetal_Flash_Dma_Cache_TestApp_CORE_DEPENDENCY = no
OSPI_Baremetal_Flash_Dma_Cache_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal DMA=enable CACHE=enable
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_COMP_LIST
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_BOARD_DEPENDENCY
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_CORE_DEPENDENCY
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_MAKEFILE
OSPI_Baremetal_Flash_Dma_Cache_TestApp_PKG_LIST = OSPI_Baremetal_Flash_Dma_Cache_TestApp
OSPI_Baremetal_Flash_Dma_Cache_TestApp_INCLUDE = $(OSPI_Baremetal_Flash_Dma_Cache_TestApp_PATH)
OSPI_Baremetal_Flash_Dma_Cache_TestApp_BOARDLIST = $(drvspi_BOARDLIST)
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am64x))
OSPI_Baremetal_Flash_Dma_Cache_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELISTARM_CACHE)
else
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_Dma_Cache_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST_DMA_CACHE)
else
OSPI_Baremetal_Flash_Dma_Cache_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST_CACHE)
endif
endif
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_$(SOC)_CORELIST

ifeq ($(SOC),$(filter $(SOC), j721e am65xx j7200 am64x j721s2 j784s4 j742s2))
OSPI_Baremetal_Flash_Dma_Cache_TestApp_SBL_APPIMAGEGEN = yes
export OSPI_Baremetal_Flash_Dma_Cache_TestApp_SBL_APPIMAGEGEN
endif

# OSPI rtos Flash Test app with cacheable OSPI memory
define OSPI_FLASH_CACHE_TESTAPP_RULE

export OSPI_Flash_Cache_TestApp_$(1)_COMP_LIST = OSPI_Flash_Cache_TestApp_$(1)
OSPI_Flash_Cache_TestApp_$(1)_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Flash_Cache_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
export OSPI_Flash_Cache_TestApp_$(1)_BOARD_DEPENDENCY = yes
export OSPI_Flash_Cache_TestApp_$(1)_CORE_DEPENDENCY = no
export OSPI_Flash_Cache_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1) CACHE=enable
OSPI_Flash_Cache_TestApp_$(1)_PKG_LIST = OSPI_Flash_Cache_TestApp_$(1)
OSPI_Flash_Cache_TestApp_$(1)_INCLUDE = $(OSPI_Flash_Cache_TestApp_$(1)_PATH)
export OSPI_Flash_Cache_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST) )
ifeq ($(SOC),$(filter $(SOC), j721e am64x))
OSPI_Flash_Cache_TestApp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELISTARM))
else
OSPI_Flash_Cache_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
endif
export OSPI_Flash_Cache_TestApp_$(1)_$(SOC)_CORELIST
export OSPI_Flash_Cache_TestApp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += OSPI_Flash_Cache_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += OSPI_Flash_Cache_TestApp_$(1)
endif
endif

endef

OSPI_FLASH_CACHE_TESTAPP_MACRO_LIST := $(foreach curos, $(ospi_RTOS_LIST), $(call OSPI_FLASH_CACHE_TESTAPP_RULE,$(curos)))

$(eval ${OSPI_FLASH_CACHE_TESTAPP_MACRO_LIST})

# OSPI rtos DMA Flash Test app with cacheable OSPI memory
define OSPI_FLASH_DMA_CACHE_TESTAPP_RULE

export OSPI_Flash_Dma_Cache_TestApp_$(1)_COMP_LIST = OSPI_Flash_Dma_Cache_TestApp_$(1)
OSPI_Flash_Dma_Cache_TestApp_$(1)_RELPATH = ti/drv/spi/test/ospi_flash
OSPI_Flash_Dma_Cache_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash
export OSPI_Flash_Dma_Cache_TestApp_$(1)_BOARD_DEPENDENCY = yes
export OSPI_Flash_Dma_Cache_TestApp_$(1)_CORE_DEPENDENCY = no
export OSPI_Flash_Dma_Cache_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1) DMA=enable CACHE=enable
OSPI_Flash_Dma_Cache_TestApp_$(1)_PKG_LIST = OSPI_Flash_Dma_Cache_TestApp_$(1)
OSPI_Flash_Dma_Cache_TestApp_$(1)_INCLUDE = $(OSPI_Flash_Dma_Cache_TestApp_$(1)_PATH)
export OSPI_Flash_Dma_Cache_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvspi_BOARDLIST) )
ifeq ($(SOC),$(filter $(SOC), am64x))
OSPI_Flash_Dma_Cache_TestApp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELISTARM))
else
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
OSPI_Flash_Dma_Cache_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST_DMA_CACHE))
else
OSPI_Flash_Dma_Cache_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
endif
endif
export OSPI_Flash_Dma_Cache_TestApp_$(1)_$(SOC)_CORELIST
export OSPI_Flash_Dma_Cache_TestApp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += OSPI_Flash_Dma_Cache_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += OSPI_Flash_Dma_Cache_TestApp_$(1)
endif
endif

endef

OSPI_FLASH_DMA_CACHE_TESTAPP_MACRO_LIST := $(foreach curos, $(ospi_RTOS_LIST), $(call OSPI_FLASH_DMA_CACHE_TESTAPP_RULE,$(curos)))

$(eval ${OSPI_FLASH_DMA_CACHE_TESTAPP_MACRO_LIST})

# OSPI NAND Flash OTP Test app
OSPI_NAND_Flash_OTP_TestApp_COMP_LIST = OSPI_NAND_Flash_OTP_TestApp
OSPI_NAND_Flash_OTP_TestApp_RELPATH = ti/drv/spi/test/ospi_flash_nand_test
OSPI_NAND_Flash_OTP_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/ospi_flash_nand_test
OSPI_NAND_Flash_OTP_TestApp_BOARD_DEPENDENCY = yes
OSPI_NAND_Flash_OTP_TestApp_CORE_DEPENDENCY = no
OSPI_NAND_Flash_OTP_TestApp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export OSPI_NAND_Flash_OTP_TestApp_COMP_LIST
export OSPI_NAND_Flash_OTP_TestApp_BOARD_DEPENDENCY
export OSPI_NAND_Flash_OTP_TestApp_CORE_DEPENDENCY
export OSPI_NAND_Flash_OTP_TestApp_MAKEFILE
OSPI_NAND_Flash_OTP_TestApp_PKG_LIST = OSPI_NAND_Flash_OTP_TestApp
OSPI_NAND_Flash_OTP_TestApp_INCLUDE = $(OSPI_NAND_Flash_OTP_TestApp_PATH)
OSPI_NAND_Flash_OTP_TestApp_BOARDLIST = j721s2_evm j784s4_evm j742s2_evm
export OSPI_NAND_Flash_OTP_TestApp_BOARDLIST
OSPI_NAND_Flash_OTP_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)
export OSPI_NAND_Flash_OTP_TestApp_$(SOC)_CORELIST

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
OSPI_NAND_Flash_OTP_TestApp_SBL_APPIMAGEGEN = yes
export OSPI_NAND_Flash_OTP_TestApp_SBL_APPIMAGEGEN
endif

# QSPI Baremetal Flash Test app
QSPI_Baremetal_Flash_TestApp_COMP_LIST = QSPI_Baremetal_Flash_TestApp
QSPI_Baremetal_Flash_TestApp_RELPATH = ti/drv/spi/test/qspi_flash
QSPI_Baremetal_Flash_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/qspi_flash
QSPI_Baremetal_Flash_TestApp_BOARD_DEPENDENCY = yes
QSPI_Baremetal_Flash_TestApp_CORE_DEPENDENCY = yes
QSPI_Baremetal_Flash_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export QSPI_Baremetal_Flash_TestApp_COMP_LIST
export QSPI_Baremetal_Flash_TestApp_BOARD_DEPENDENCY
export QSPI_Baremetal_Flash_TestApp_CORE_DEPENDENCY
export QSPI_Baremetal_Flash_TestApp_MAKEFILE
QSPI_Baremetal_Flash_TestApp_PKG_LIST = QSPI_Baremetal_Flash_TestApp
QSPI_Baremetal_Flash_TestApp_INCLUDE = $(QSPI_Baremetal_Flash_TestApp_PATH)
QSPI_Baremetal_Flash_TestApp_BOARDLIST = tpr12_evm tpr12_qt awr294x_evm
export QSPI_Baremetal_Flash_TestApp_BOARDLIST
QSPI_Baremetal_Flash_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)

# QSPI Flash Test app
define QSPI_Flash_TestApp_RULE
export QSPI_Flash_TestApp_$(1)_COMP_LIST = QSPI_Flash_TestApp_$(1)
export QSPI_Flash_TestApp_$(1)_RELPATH = ti/drv/spi/test/qspi_flash
export QSPI_Flash_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/qspi_flash
export QSPI_Flash_TestApp_$(1)_BOARD_DEPENDENCY = yes
export QSPI_Flash_TestApp_$(1)_CORE_DEPENDENCY = yes
export QSPI_Flash_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export QSPI_Flash_TestApp_$(1)_PKG_LIST = QSPI_Flash_TestApp_$(1)
export QSPI_Flash_TestApp_$(1)_INCLUDE = $(QSPI_Flash_TestApp_$(1)_PATH)
export QSPI_Flash_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvqspi_BOARDLIST))
export QSPI_Flash_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += QSPI_Flash_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += QSPI_Flash_TestApp_$(1)
endif
endif
export QSPI_Flash_TestApp_$(1)_SBL_APPIMAGEGEN = yes

endef

QSPI_Flash_TestApp_MACRO_LIST := $(foreach curos,$(drvqspi_RTOS_LIST),$(call QSPI_Flash_TestApp_RULE,$(curos)))

$(eval ${QSPI_Flash_TestApp_MACRO_LIST})

# QSPI Baremetal dma Flash Test app
QSPI_Baremetal_Flash_Dma_TestApp_COMP_LIST = QSPI_Baremetal_Flash_Dma_TestApp
QSPI_Baremetal_Flash_Dma_TestApp_RELPATH = ti/drv/spi/test/qspi_flash
QSPI_Baremetal_Flash_Dma_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/qspi_flash
QSPI_Baremetal_Flash_Dma_TestApp_BOARD_DEPENDENCY = yes
QSPI_Baremetal_Flash_Dma_TestApp_CORE_DEPENDENCY = yes
QSPI_Baremetal_Flash_Dma_TestApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal DMA=enable
export QSPI_Baremetal_Flash_Dma_TestApp_COMP_LIST
export QSPI_Baremetal_Flash_Dma_TestApp_BOARD_DEPENDENCY
export QSPI_Baremetal_Flash_Dma_TestApp_CORE_DEPENDENCY
export QSPI_Baremetal_Flash_Dma_TestApp_MAKEFILE
QSPI_Baremetal_Flash_Dma_TestApp_PKG_LIST = QSPI_Baremetal_Flash_Dma_TestApp
QSPI_Baremetal_Flash_Dma_TestApp_INCLUDE = $(QSPI_Baremetal_Flash_Dma_TestApp_PATH)
QSPI_Baremetal_Flash_Dma_TestApp_BOARDLIST = tpr12_evm tpr12_qt awr294x_evm
export QSPI_Baremetal_Flash_Dma_TestApp_BOARDLIST
QSPI_Baremetal_Flash_Dma_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)

# QSPI Flash Dma Test app
define QSPI_Flash_Dma_TestApp_RULE
export QSPI_Flash_Dma_TestApp_$(1)_COMP_LIST = QSPI_Flash_Dma_TestApp_$(1)
export QSPI_Flash_Dma_TestApp_$(1)_RELPATH = ti/drv/spi/test/qspi_flash
export QSPI_Flash_Dma_TestApp_$(1)_PATH = $(PDK_SPI_COMP_PATH)/test/qspi_flash
export QSPI_Flash_Dma_TestApp_$(1)_BOARD_DEPENDENCY = yes
export QSPI_Flash_Dma_TestApp_$(1)_CORE_DEPENDENCY = yes
export QSPI_Flash_Dma_TestApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1) DMA=enable
export QSPI_Flash_Dma_TestApp_$(1)_PKG_LIST = QSPI_Flash_Dma_TestApp_$(1)
export QSPI_Flash_Dma_TestApp_$(1)_INCLUDE = $(QSPI_Flash_Dma_TestApp_$(1)_PATH)
export QSPI_Flash_Dma_TestApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvqspi_BOARDLIST))
export QSPI_Flash_Dma_TestApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvspi_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
spi_EXAMPLE_LIST += QSPI_Flash_Dma_TestApp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
spi_EXAMPLE_LIST += QSPI_Flash_Dma_TestApp_$(1)
endif
endif
export QSPI_Flash_Dma_TestApp_$(1)_SBL_APPIMAGEGEN = yes

endef

QSPI_Flash_Dma_TestApp_MACRO_LIST := $(foreach curos,$(drvqspi_RTOS_LIST),$(call QSPI_Flash_Dma_TestApp_RULE,$(curos)))

$(eval ${QSPI_Flash_Dma_TestApp_MACRO_LIST})

# QSPI dma File Flash Write Test app.
# This app allows us to write files to flash device using CCS.
# This is temporary till we got correct uniflash support in TPR12.
QSPI_FileFlashWrite_Dma_TestApp_COMP_LIST = QSPI_FileFlashWrite_Dma_TestApp
QSPI_FileFlashWrite_Dma_TestApp_RELPATH = ti/drv/spi/test/qspi_flash
QSPI_FileFlashWrite_Dma_TestApp_PATH = $(PDK_SPI_COMP_PATH)/test/qspi_flash
QSPI_FileFlashWrite_Dma_TestApp_BOARD_DEPENDENCY = yes
QSPI_FileFlashWrite_Dma_TestApp_CORE_DEPENDENCY = yes
QSPI_FileFlashWrite_Dma_TestApp_XDC_CONFIGURO = yes
QSPI_FileFlashWrite_Dma_TestApp_MAKEFILE = -f makefile_ccs_flash_write DMA=enable
export QSPI_FileFlashWrite_Dma_TestApp_COMP_LIST
export QSPI_FileFlashWrite_Dma_TestApp_BOARD_DEPENDENCY
export QSPI_FileFlashWrite_Dma_TestApp_CORE_DEPENDENCY
export QSPI_FileFlashWrite_Dma_TestApp_XDC_CONFIGURO
export QSPI_FileFlashWrite_Dma_TestApp_MAKEFILE
QSPI_FileFlashWrite_Dma_TestApp_PKG_LIST = QSPI_FileFlashWrite_Dma_TestApp
QSPI_FileFlashWrite_Dma_TestApp_INCLUDE = $(QSPI_FileFlashWrite_Dma_TestApp_PATH)
QSPI_FileFlashWrite_Dma_TestApp_BOARDLIST = tpr12_evm tpr12_qt awr294x_evm
export QSPI_FileFlashWrite_Dma_TestApp_BOARDLIST
QSPI_FileFlashWrite_Dma_TestApp_$(SOC)_CORELIST = $(drvspi_$(SOC)_CORELIST)

drvspi_EXAMPLE_LIST = $(spi_EXAMPLE_LIST)

export drvspi_LIB_LIST
export spi_LIB_LIST
export spi_EXAMPLE_LIST
export drvspi_FIRM_LIST
export spi_FIRM_LIST
export drvspi_EXAMPLE_LIST

spi_component_make_include := 1
endif