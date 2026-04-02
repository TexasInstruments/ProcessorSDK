#  ============================================================================
#  (C) Copyright 2020 Texas Instruments, Inc.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  ============================================================================
# File: csl_test_component.mk
#       This file is component include make file of STW unit test.
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
ifeq ($(csl_test_component_make_include), )

csl_test_default_SOCLIST = tda2xx tda2px tda2ex tda3xx
csl_test_default_tda2xx_CORELIST = a15_0
csl_test_default_tda2px_CORELIST = a15_0
csl_test_default_tda2ex_CORELIST = a15_0
csl_test_default_tda3xx_CORELIST = ipu1_0

############################
# csl_test package
# List of components included under csl_test
# The components included here are built and will be part of csl_test lib
############################
csl_test_LIB_LIST =

############################
# csl_test examples
# List of examples under csl_test (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
csl_test_EXAMPLE_LIST =

# List with various rtos_types(such as tirtos(sysbios),freertos,safertos) to build RTOS apps. 
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
cslmcanut_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

# MCAN unit test app
csl_mcan_unit_test_app_COMP_LIST = csl_mcan_unit_test_app
csl_mcan_unit_test_app_RELPATH = ti/csl/test/mcanUt
csl_mcan_unit_test_app_PATH = $(PDK_CSL_COMP_PATH)/test/mcanUt
csl_mcan_unit_test_app_MAKEFILE = -fmakefile_baremetal
csl_mcan_unit_test_app_BOARD_DEPENDENCY = yes
csl_mcan_unit_test_app_CORE_DEPENDENCY = yes
export csl_mcan_unit_test_app_COMP_LIST
export csl_mcan_unit_test_app_BOARD_DEPENDENCY
export csl_mcan_unit_test_app_CORE_DEPENDENCY
csl_mcan_unit_test_app_PKG_LIST = csl_mcan_unit_test_app
csl_mcan_unit_test_app_INCLUDE = $(csl_mcan_unit_test_app_PATH)
csl_mcan_unit_test_app_BOARDLIST = tda3xx-evm tda2px-evm am65xx_idk j721e_evm j7200_evm am64x_evm j721s2_evm
export csl_mcan_unit_test_app_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx am64x))
csl_mcan_unit_test_app_$(SOC)_CORELIST = mcu1_0 mcu1_1
endif
ifeq ($(SOC),$(filter $(SOC), j721e))
csl_mcan_unit_test_app_$(SOC)_CORELIST = mcu1_0 mcu2_1
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
csl_mcan_unit_test_app_$(SOC)_CORELIST = mcu1_0 mcu2_1
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
csl_mcan_unit_test_app_$(SOC)_CORELIST = mcu1_0 mcu2_1
endif
ifeq ($(SOC),$(filter $(SOC), tda3xx tda2px))
csl_mcan_unit_test_app_$(SOC)_CORELIST = ipu1_0
endif
export csl_mcan_unit_test_app_$(SOC)_CORELIST

# packaged for CSL build
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
csl_test_EXAMPLE_LIST += csl_mcan_unit_test_app
endif
csl_mcan_unit_test_app_SBL_APPIMAGEGEN = yes
export csl_mcan_unit_test_app_SBL_APPIMAGEGEN


# MCAN unit test app RTOS
define CSL_MCAN_RTOS_UT_RULE

# MCAN unit test app rtos
export csl_mcan_unit_test_app_$(1)_COMP_LIST = csl_mcan_unit_test_app_$(1)
csl_mcan_unit_test_app_$(1)_RELPATH = ti/csl/test/mcanUt
csl_mcan_unit_test_app_$(1)_PATH = $(PDK_CSL_COMP_PATH)/test/mcanUt
export csl_mcan_unit_test_app_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export csl_mcan_unit_test_app_$(1)_BOARD_DEPENDENCY = yes
export csl_mcan_unit_test_app_$(1)_CORE_DEPENDENCY = yes
export csl_mcan_unit_test_app_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
csl_mcan_unit_test_app_$(1)_PKG_LIST = csl_mcan_unit_test_app_$(1)
csl_mcan_unit_test_app_$(1)_INCLUDE = $(csl_mcan_unit_test_app_$(1)_PATH)
export csl_mcan_unit_test_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm)
ifeq ($(SOC),$(filter $(SOC), j721e))
csl_mcan_unit_test_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu1_0 mcu2_1)
endif
export csl_mcan_unit_test_app_$(1)_$(SOC)_CORELIST
export csl_mcan_unit_test_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csl_test_EXAMPLE_LIST += csl_mcan_unit_test_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csl_test_EXAMPLE_LIST += csl_mcan_unit_test_app_$(1)
endif
endif


endef

CSL_MCAN_RTOS_UT_MACRO_LIST := $(foreach curos, $(cslmcanut_RTOS_LIST), $(call CSL_MCAN_RTOS_UT_RULE,$(curos)))

$(eval ${CSL_MCAN_RTOS_UT_MACRO_LIST})

# DCC unit test app
csl_dcc_unit_testapp_COMP_LIST = csl_dcc_unit_testapp
csl_dcc_unit_testapp_RELPATH = ti/csl/test/dccUt
csl_dcc_unit_testapp_PATH = $(PDK_CSL_COMP_PATH)/test/dccUt
csl_dcc_unit_testapp_BOARD_DEPENDENCY = yes
csl_dcc_unit_testapp_CORE_DEPENDENCY = yes
export csl_dcc_unit_testapp_COMP_LIST
export csl_dcc_unit_testapp_BOARD_DEPENDENCY
export csl_dcc_unit_testapp_CORE_DEPENDENCY
csl_dcc_unit_testapp_PKG_LIST = csl_dcc_unit_testapp
csl_dcc_unit_testapp_INCLUDE = $(csl_dcc_unit_testapp_PATH)
csl_dcc_unit_testapp_BOARDLIST = am65xx_evm
export csl_dcc_unit_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx))
csl_dcc_unit_testapp_$(SOC)_CORELIST = mcu1_0
endif
export csl_dcc_unit_testapp_$(SOC)_CORELIST

# packaged for below CSL BUILDS
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
csl_test_EXAMPLE_LIST += csl_dcc_unit_testapp
endif

ifeq ($(SOC),$(filter $(SOC), am65xx))
csl_dcc_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export csl_dcc_unit_testapp_SBL_APPIMAGEGEN

# CRC unit test app
csl_crc_unit_testapp_COMP_LIST = csl_crc_unit_testapp
csl_crc_unit_testapp_RELPATH = ti/csl/test/crcUt
csl_crc_unit_testapp_PATH = $(PDK_CSL_COMP_PATH)/test/crcUt
csl_crc_unit_testapp_BOARD_DEPENDENCY = yes
csl_crc_unit_testapp_CORE_DEPENDENCY = yes
export csl_crc_unit_testapp_COMP_LIST
export csl_crc_unit_testapp_BOARD_DEPENDENCY
export csl_crc_unit_testapp_CORE_DEPENDENCY
csl_crc_unit_testapp_PKG_LIST = csl_crc_unit_testapp
csl_crc_unit_testapp_INCLUDE = $(csl_crc_unit_testapp_PATH)
csl_crc_unit_testapp_BOARDLIST = am65xx_evm j7200_evm j721e_evm
export csl_crc_unit_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx j7200 j721e))
csl_crc_unit_testapp_$(SOC)_CORELIST = mcu1_0
endif
export csl_crc_unit_testapp_$(SOC)_CORELIST

# packaged for CSL BUILD only
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL CSL_TRIM))
csl_test_EXAMPLE_LIST += csl_crc_unit_testapp
endif

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x))
csl_crc_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export csl_crc_unit_testapp_SBL_APPIMAGEGEN

# RTI unit test app
csl_rti_unit_testapp_COMP_LIST = csl_rti_unit_testapp
csl_rti_unit_testapp_RELPATH = ti/csl/test/rtiUt
csl_rti_unit_testapp_PATH = $(PDK_CSL_COMP_PATH)/test/rtiUt
csl_rti_unit_testapp_BOARD_DEPENDENCY = yes
csl_rti_unit_testapp_CORE_DEPENDENCY = yes
export csl_rti_unit_testapp_COMP_LIST
export csl_rti_unit_testapp_BOARD_DEPENDENCY
export csl_rti_unit_testapp_CORE_DEPENDENCY
csl_rti_unit_testapp_PKG_LIST = csl_rti_unit_testapp
csl_rti_unit_testapp_INCLUDE = $(csl_rti_unit_testapp_PATH)
csl_rti_unit_testapp_BOARDLIST = am65xx_evm j721e_evm j7200_evm tpr12_evm awr294x_evm
export csl_rti_unit_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 tpr12 awr294x))
csl_rti_unit_testapp_$(SOC)_CORELIST = mcu1_0
endif
export csl_rti_unit_testapp_$(SOC)_CORELIST

# packaged for CSL BUILD only
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL CSL_TRIM))
csl_test_EXAMPLE_LIST += csl_rti_unit_testapp
endif

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
csl_rti_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export csl_rti_unit_testapp_SBL_APPIMAGEGEN

# RTI Timer unit test app
csl_rtitmr_unit_testapp_COMP_LIST = csl_rtitmr_unit_testapp
csl_rtitmr_unit_testapp_RELPATH = ti/csl/test/rtiTmrUt
csl_rtitmr_unit_testapp_PATH = $(PDK_CSL_COMP_PATH)/test/rtiTmrUt
csl_rtitmr_unit_testapp_BOARD_DEPENDENCY = yes
csl_rtitmr_unit_testapp_CORE_DEPENDENCY = yes
export csl_rtitmr_unit_testapp_COMP_LIST
export csl_rtitmr_unit_testapp_BOARD_DEPENDENCY
export csl_rtitmr_unit_testapp_CORE_DEPENDENCY
csl_rtitmr_unit_testapp_PKG_LIST = csl_rtitmr_unit_testapp
csl_rtitmr_unit_testapp_INCLUDE = $(csl_rtitmr_unit_testapp_PATH)
csl_rtitmr_unit_testapp_BOARDLIST = tpr12_qt tpr12_evm awr294x_evm
export csl_rtitmr_unit_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
csl_rtitmr_unit_testapp_$(SOC)_CORELIST = mcu1_0
endif
export csl_rtitmr_unit_testapp_$(SOC)_CORELIST

# packaged for CSL BUILD only
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL CSL_TRIM))
csl_test_EXAMPLE_LIST += csl_rtitmr_unit_testapp
endif

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
csl_rtitmr_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export csl_rtitmr_unit_testapp_SBL_APPIMAGEGEN

# ADC unit test app
csl_adc_unit_testapp_COMP_LIST = csl_adc_unit_testapp
csl_adc_unit_testapp_RELPATH = ti/csl/test/adcUt
csl_adc_unit_testapp_PATH = $(PDK_CSL_COMP_PATH)/test/adcUt
csl_adc_unit_testapp_BOARD_DEPENDENCY = yes
csl_adc_unit_testapp_CORE_DEPENDENCY = yes
export csl_adc_unit_testapp_COMP_LIST
export csl_adc_unit_testapp_BOARD_DEPENDENCY
export csl_adc_unit_testapp_CORE_DEPENDENCY
csl_adc_unit_testapp_PKG_LIST = csl_adc_unit_testapp
csl_adc_unit_testapp_INCLUDE = $(csl_adc_unit_testapp_PATH)
csl_adc_unit_testapp_BOARDLIST = am65xx_evm am64x_evm
export csl_adc_unit_testapp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), am65xx am64x))
csl_adc_unit_testapp_$(SOC)_CORELIST = mcu1_0
endif
export csl_adc_unit_testapp_$(SOC)_CORELIST

# packaged for CSL BUILD only
ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
csl_test_EXAMPLE_LIST += csl_adc_unit_testapp
endif

ifeq ($(SOC),$(filter $(SOC), am65xx am64x))
csl_adc_unit_testapp_SBL_APPIMAGEGEN = yes
endif
export csl_adc_unit_testapp_SBL_APPIMAGEGEN

export csl_test_LIB_LIST
export csl_test_EXAMPLE_LIST

csl_test_component_make_include := 1
endif
