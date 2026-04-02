#  ============================================================================
#  (C) Copyright 2019-2021 Texas Instruments, Inc.
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

# File: board_diag_component.mk
#       This file is component include make file of board diagnostic.
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
ifeq ($(board_diag_component_make_include), )

#board_diag_am65xx_BOARDLIST = am65xx_evm am65xx_idk
board_diag_am65xx_CORELIST = mpu1_0 mcu1_0
board_diag_j721e_BOARDLIST = j721e_evm
board_diag_j721e_CORELIST = mpu1_0 mcu1_0
board_diag_j7200_CORELIST = mpu1_0 mcu1_0
board_diag_j721s2_CORELIST = mpu1_0 mcu1_0
board_diag_j784s4_CORELIST = mpu1_0 mcu1_0
board_diag_tpr12_CORELIST = mcu1_0
board_diag_awr294x_CORELIST = mcu1_0
board_diag_am64x_CORELIST = mpu1_0 mcu1_0

############################
# Board diagnostic package
# List of diagnostics included under board diagnostic
# The components included here are built and will be part of board diagnostic
############################
board_diag_LIB_LIST =

############################
# board diagnostics
# List of diagnostics under board
# All the tests mentioned in list are built when test target is called
# List below all example for allowed values - note the list is populated in each example
############################
board_diag_EXAMPLE_LIST =

#
# Diagnostic Modules
#

# ARCH is used for diag binary folder name to align with existing platforms
board_diag_LOCAL_BINPATH =

board_diag_APPIMAGEGEN_CTRL = yes

# Board Diagnostic
board_diag_COMP_LIST = board_diag

# ADC
board_diag_adc_COMP_LIST = board_diag_adc
board_diag_adc_RELPATH = ti/board/diag/adc/build
board_diag_adc_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/adc/build
board_diag_adc_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_adc_MAKEFILE = -f makefile
board_diag_adc_BOARD_DEPENDENCY = yes
board_diag_adc_CORE_DEPENDENCY = yes
export board_diag_adc_COMP_LIST
export board_diag_adc_BOARD_DEPENDENCY
export board_diag_adc_CORE_DEPENDENCY
export board_diag_adc_MAKEFILE
board_diag_adc_PKG_LIST = board_diag_adc
board_diag_adc_INCLUDE = $(board_diag_adc_PATH)
board_diag_adc_BOARDLIST = j721e_evm j7200_evm am64x_evm j721s2_evm j784s4_evm
board_diag_adc_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_adc_$(SOC)_CORELIST
export board_diag_adc_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_adc

# AUTOMATION HEADER
board_diag_automationHeader_COMP_LIST = board_diag_automationHeader
board_diag_automationHeader_RELPATH = ti/board/diag/automation_header/build
board_diag_automationHeader_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/automation_header/build
board_diag_automationHeader_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_automationHeader_MAKEFILE = -f makefile
board_diag_automationHeader_BOARD_DEPENDENCY = yes
board_diag_automationHeader_CORE_DEPENDENCY = yes
export board_diag_automationHeader_COMP_LIST
export board_diag_automationHeader_BOARD_DEPENDENCY
export board_diag_automationHeader_CORE_DEPENDENCY
export board_diag_automationHeader_MAKEFILE
board_diag_automationHeader_PKG_LIST = board_diag_automationHeader
board_diag_automationHeader_INCLUDE = $(board_diag_automationHeader_PATH)
board_diag_automationHeader_BOARDLIST = j721e_evm j7200_evm am64x_evm j721s2_evm j784s4_evm
board_diag_automationHeader_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_automationHeader_$(SOC)_CORELIST
export board_diag_automationHeader_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_automationHeader

# BOOT EEPROM
board_diag_bootEeprom_COMP_LIST = board_diag_bootEeprom
board_diag_bootEeprom_RELPATH = ti/board/diag/boot_eeprom/build
board_diag_bootEeprom_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/boot_eeprom/build
board_diag_bootEeprom_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_bootEeprom_MAKEFILE = -f makefile
board_diag_bootEeprom_BOARD_DEPENDENCY = yes
board_diag_bootEeprom_CORE_DEPENDENCY = yes
export board_diag_bootEeprom_COMP_LIST
export board_diag_bootEeprom_BOARD_DEPENDENCY
export board_diag_bootEeprom_CORE_DEPENDENCY
export board_diag_bootEeprom_MAKEFILE
board_diag_bootEeprom_PKG_LIST = board_diag_bootEeprom
board_diag_bootEeprom_INCLUDE = $(board_diag_bootEeprom_PATH)
board_diag_bootEeprom_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm j721s2_evm j784s4_evm
board_diag_bootEeprom_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_bootEeprom_$(SOC)_CORELIST
export board_diag_bootEeprom_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_bootEeprom

# BOOST GPIO
board_diag_boostGpio_COMP_LIST = board_diag_boostGpio
board_diag_boostGpio_RELPATH = ti/board/diag/boost_gpio/build
board_diag_boostGpio_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/boost_gpio/build
board_diag_boostGpio_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_boostGpio_MAKEFILE = -f makefile
board_diag_boostGpio_BOARD_DEPENDENCY = yes
board_diag_boostGpio_CORE_DEPENDENCY = yes
export board_diag_boostGpio_COMP_LIST
export board_diag_boostGpio_BOARD_DEPENDENCY
export board_diag_boostGpio_CORE_DEPENDENCY
export board_diag_boostGpio_MAKEFILE
board_diag_boostGpio_PKG_LIST = board_diag_boostGpio
board_diag_boostGpio_INCLUDE = $(board_diag_boostGpio_PATH)
board_diag_boostGpio_BOARDLIST = am65xx_evm
board_diag_boostGpio_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_boostGpio_$(SOC)_CORELIST
export board_diag_boostGpio_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_boostGpio

# BOOT SWITCH
export board_diag_bootSwitch_COMP_LIST = board_diag_bootSwitch
board_diag_bootSwitch_RELPATH = ti/board/diag/boot_switch/build
board_diag_bootSwitch_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/boot_switch/build
board_diag_bootSwitch_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_bootSwitch_MAKEFILE = -f makefile
export board_diag_bootSwitch_BOARD_DEPENDENCY = yes
export board_diag_bootSwitch_CORE_DEPENDENCY = yes
board_diag_bootSwitch_PKG_LIST = board_diag_bootSwitch
board_diag_bootSwitch_INCLUDE = $(board_diag_bootSwitch_PATH)
board_diag_bootSwitch_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm j784s4_evm
export board_diag_bootSwitch_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_bootSwitch_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_bootSwitch

# BUTTON
export board_diag_button_COMP_LIST = board_diag_button
board_diag_button_RELPATH = ti/board/diag/button/build
board_diag_button_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/button/build
board_diag_button_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_button_MAKEFILE = -f makefile
export board_diag_button_BOARD_DEPENDENCY = yes
export board_diag_button_CORE_DEPENDENCY = yes
board_diag_button_PKG_LIST = board_diag_button
board_diag_button_INCLUDE = $(board_diag_button_PATH)
board_diag_button_BOARDLIST = am65xx_evm am65xx_idk tpr12_evm am64x_evm awr294x_evm
export board_diag_button_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_button_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_button

# CLOCK GENERATOR
board_diag_clockGen_COMP_LIST = board_diag_clockGen
board_diag_clockGen_RELPATH = ti/board/diag/clock_generator/build
board_diag_clockGen_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/clock_generator/build
board_diag_clockGen_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_clockGen_MAKEFILE = -f makefile
board_diag_clockGen_BOARD_DEPENDENCY = yes
board_diag_clockGen_CORE_DEPENDENCY = yes
export board_diag_clockGen_COMP_LIST
export board_diag_clockGen_BOARD_DEPENDENCY
export board_diag_clockGen_CORE_DEPENDENCY
export board_diag_clockGen_MAKEFILE
board_diag_clockGen_PKG_LIST = board_diag_clockGen
board_diag_clockGen_INCLUDE = $(board_diag_clockGen_PATH)
board_diag_clockGen_BOARDLIST = j721e_evm j7200_evm
board_diag_clockGen_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_clockGen_$(SOC)_CORELIST
export board_diag_clockGen_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_clockGen

# CPSW EMAC
board_diag_cpsw_COMP_LIST = board_diag_cpsw
board_diag_cpsw_RELPATH = ti/board/diag/cpsw/build
board_diag_cpsw_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/cpsw/build
board_diag_cpsw_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_cpsw_MAKEFILE = -f makefile
board_diag_cpsw_BOARD_DEPENDENCY = yes
board_diag_cpsw_CORE_DEPENDENCY = yes
export board_diag_cpsw_COMP_LIST
export board_diag_cpsw_BOARD_DEPENDENCY
export board_diag_cpsw_CORE_DEPENDENCY
export board_diag_cpsw_MAKEFILE
board_diag_cpsw_PKG_LIST = board_diag_cpsw
board_diag_cpsw_INCLUDE = $(board_diag_cpsw_PATH)
board_diag_cpsw_BOARDLIST = tpr12_evm awr294x_evm j784s4_evm
ifeq ($(SOC),$(filter $(SOC), j784s4))
board_diag_cpsw_$(SOC)_CORELIST = mcu1_0 mcu2_1
else
board_diag_cpsw_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
endif
export board_diag_cpsw_$(SOC)_CORELIST
export board_diag_cpsw_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_cpsw

# csirx
board_diag_csirx_COMP_LIST = board_diag_csirx
board_diag_csirx_RELPATH = ti/board/diag/csirx/build
board_diag_csirx_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/csirx/build
board_diag_csirx_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_csirx_MAKEFILE = -f makefile IS_BAREMETAL=yes
board_diag_csirx_BOARD_DEPENDENCY = yes
board_diag_csirx_CORE_DEPENDENCY = yes
board_diag_csirx_XDC_CONFIGURO = no
export board_diag_csirx_COMP_LIST
export board_diag_csirx_BOARD_DEPENDENCY
export board_diag_csirx_CORE_DEPENDENCY
export board_diag_csirx_MAKEFILE
export board_diag_csirx_XDC_CONFIGURO
board_diag_csirx_PKG_LIST = board_diag_csirx
board_diag_csirx_INCLUDE = $(board_diag_csirx_PATH)
board_diag_csirx_BOARDLIST = j721e_evm tpr12_evm
ifeq ($(SOC),$(filter $(SOC), tpr12))
board_diag_csirx_$(SOC)_CORELIST = mcu1_0 c66xdsp_1
else
board_diag_csirx_$(SOC)_CORELIST = mcu2_0
endif
export board_diag_csirx_$(SOC)_CORELIST
export board_diag_csirx_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)

# csirx
board_diag_csirx_tirtos_COMP_LIST = board_diag_csirx_tirtos
board_diag_csirx_tirtos_RELPATH = ti/board/diag/csirx/build
board_diag_csirx_tirtos_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/csirx/build
board_diag_csirx_tirtos_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_csirx_tirtos_MAKEFILE = -f makefile IS_BAREMETAL=no
board_diag_csirx_tirtos_BOARD_DEPENDENCY = yes
board_diag_csirx_tirtos_CORE_DEPENDENCY = yes
board_diag_csirx_tirtos_XDC_CONFIGURO = yes
export board_diag_csirx_tirtos_COMP_LIST
export board_diag_csirx_tirtos_BOARD_DEPENDENCY
export board_diag_csirx_tirtos_CORE_DEPENDENCY
export board_diag_csirx_tirtos_MAKEFILE
export board_diag_csirx_tirtos_XDC_CONFIGURO
board_diag_csirx_tirtos_PKG_LIST = board_diag_csirx_tirtos
board_diag_csirx_tirtos_INCLUDE = $(board_diag_csirx_tirtos_PATH)
board_diag_csirx_tirtos_BOARDLIST = tpr12_evm
board_diag_csirx_tirtos_$(SOC)_CORELIST = mcu1_0 c66xdsp_1
export board_diag_csirx_tirtos_$(SOC)_CORELIST
export board_diag_csirx_tirtos_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)

# Add CSIRX diagnostic test only if the CSIRX component path exists
ifneq ($(wildcard $(PDK_CSIRX_COMP_PATH)),)
board_diag_EXAMPLE_LIST += board_diag_csirx board_diag_csirx_tirtos
endif

# csitx_rx
board_diag_csitx_rx_COMP_LIST = board_diag_csitx_rx
board_diag_csitx_rx_RELPATH = ti/board/diag/csitx_rx/build
board_diag_csitx_rx_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/csitx_rx/build
board_diag_csitx_rx_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_csitx_rx_MAKEFILE = -f makefile IS_BAREMETAL=yes
board_diag_csitx_rx_BOARD_DEPENDENCY = yes
board_diag_csitx_rx_CORE_DEPENDENCY = yes
board_diag_csitx_rx_XDC_CONFIGURO = no
export board_diag_csitx_rx_COMP_LIST
export board_diag_csitx_rx_BOARD_DEPENDENCY
export board_diag_csitx_rx_CORE_DEPENDENCY
export board_diag_csitx_rx_MAKEFILE
export board_diag_csitx_rx_XDC_CONFIGURO
board_diag_csitx_rx_PKG_LIST = board_diag_csitx_rx
board_diag_csitx_rx_INCLUDE = $(board_diag_csitx_rx_PATH)
board_diag_csitx_rx_BOARDLIST = j784s4_evm
board_diag_csitx_rx_$(SOC)_CORELIST = mcu2_0
export board_diag_csitx_rx_$(SOC)_CORELIST
export board_diag_csitx_rx_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_csitx_rx


# CURRENT MONITOR
board_diag_currentMonitor_COMP_LIST = board_diag_currentMonitor
board_diag_currentMonitor_RELPATH = ti/board/diag/current_monitor/build
board_diag_currentMonitor_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/current_monitor/build
board_diag_currentMonitor_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_currentMonitor_MAKEFILE = -f makefile
board_diag_currentMonitor_BOARD_DEPENDENCY = yes
board_diag_currentMonitor_CORE_DEPENDENCY = yes
export board_diag_currentMonitor_COMP_LIST
export board_diag_currentMonitor_BOARD_DEPENDENCY
export board_diag_currentMonitor_CORE_DEPENDENCY
export board_diag_currentMonitor_MAKEFILE
board_diag_currentMonitor_PKG_LIST = board_diag_currentMonitor
board_diag_currentMonitor_INCLUDE = $(board_diag_currentMonitor_PATH)
board_diag_currentMonitor_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm tpr12_evm am64x_evm am64x_svb awr294x_evm j721s2_evm j784s4_evm
board_diag_currentMonitor_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_currentMonitor_$(SOC)_CORELIST
export board_diag_currentMonitor_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_currentMonitor

# DISPLAYPORT
board_diag_displayPort_COMP_LIST = board_diag_displayPort
board_diag_displayPort_RELPATH = ti/board/diag/display_port/build
board_diag_displayPort_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/display_port/build
board_diag_displayPort_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_displayPort_MAKEFILE = -f makefile
board_diag_displayPort_BOARD_DEPENDENCY = yes
board_diag_displayPort_CORE_DEPENDENCY = yes
export board_diag_displayPort_COMP_LIST
export board_diag_displayPort_BOARD_DEPENDENCY
export board_diag_displayPort_CORE_DEPENDENCY
export board_diag_displayPort_MAKEFILE
board_diag_displayPort_PKG_LIST = board_diag_displayPort
board_diag_displayPort_INCLUDE = $(board_diag_displayPort_PATH)
board_diag_displayPort_BOARDLIST = j721e_evm j721s2_evm j784s4_evm
board_diag_displayPort_$(SOC)_CORELIST = mcu2_0
export board_diag_displayPort_$(SOC)_CORELIST
export board_diag_displayPort_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_displayPort

# DSI to eDP bridge
board_diag_edpBridge_COMP_LIST = board_diag_edpBridge
board_diag_edpBridge_RELPATH = ti/board/diag/edp_bridge/build
board_diag_edpBridge_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/edp_bridge/build
board_diag_edpBridge_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_edpBridge_MAKEFILE = -f makefile
board_diag_edpBridge_BOARD_DEPENDENCY = yes
board_diag_edpBridge_CORE_DEPENDENCY = yes
export board_diag_edpBridge_COMP_LIST
export board_diag_edpBridge_BOARD_DEPENDENCY
export board_diag_edpBridge_CORE_DEPENDENCY
export board_diag_edpBridge_MAKEFILE
board_diag_edpBridge_PKG_LIST = board_diag_edpBridge
board_diag_edpBridge_INCLUDE = $(board_diag_edpBridge_PATH)
board_diag_edpBridge_BOARDLIST = j721s2_evm j784s4_evm
board_diag_edpBridge_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_edpBridge_$(SOC)_CORELIST
export board_diag_edpBridge_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_edpBridge

# DSI-FPD Device test
board_diag_dsi_COMP_LIST = board_diag_dsi
board_diag_dsi_RELPATH = ti/board/diag/dsi/build
board_diag_dsi_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/dsi/build
board_diag_dsi_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_dsi_MAKEFILE = -f makefile
board_diag_dsi_BOARD_DEPENDENCY = yes
board_diag_dsi_CORE_DEPENDENCY = yes
export board_diag_dsi_COMP_LIST
export board_diag_dsi_BOARD_DEPENDENCY
export board_diag_dsi_CORE_DEPENDENCY
export board_diag_dsi_MAKEFILE
board_diag_dsi_PKG_LIST = board_diag_dsi
board_diag_dsi_INCLUDE = $(board_diag_dsi_PATH)
board_diag_dsi_BOARDLIST = j721e_evm
board_diag_dsi_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_dsi_$(SOC)_CORELIST
export board_diag_dsi_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_dsi

# DSITX
board_diag_dsitx_COMP_LIST = board_diag_dsitx
board_diag_dsitx_RELPATH = ti/board/diag/dsitx/build
board_diag_dsitx_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/dsitx/build
board_diag_dsitx_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_dsitx_MAKEFILE = -f makefile
board_diag_dsitx_BOARD_DEPENDENCY = yes
board_diag_dsitx_CORE_DEPENDENCY = yes
export board_diag_dsitx_COMP_LIST
export board_diag_dsitx_BOARD_DEPENDENCY
export board_diag_dsitx_CORE_DEPENDENCY
export board_diag_dsitx_MAKEFILE
board_diag_dsitx_PKG_LIST = board_diag_dsitx
board_diag_dsitx_INCLUDE = $(board_diag_dsitx_PATH)
board_diag_dsitx_BOARDLIST = j721e_evm
board_diag_dsitx_$(SOC)_CORELIST = mcu2_0
export board_diag_dsitx_$(SOC)_CORELIST
export board_diag_dsitx_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_dsitx

# EEPROM
board_diag_eeprom_COMP_LIST = board_diag_eeprom
board_diag_eeprom_RELPATH = ti/board/diag/eeprom/build
board_diag_eeprom_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/eeprom/build
board_diag_eeprom_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_eeprom_MAKEFILE = -f makefile
board_diag_eeprom_BOARD_DEPENDENCY = yes
board_diag_eeprom_CORE_DEPENDENCY = yes
export board_diag_eeprom_COMP_LIST
export board_diag_eeprom_BOARD_DEPENDENCY
export board_diag_eeprom_CORE_DEPENDENCY
export board_diag_eeprom_MAKEFILE
board_diag_eeprom_PKG_LIST = board_diag_eeprom
board_diag_eeprom_INCLUDE = $(board_diag_eeprom_PATH)
board_diag_eeprom_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm tpr12_evm am64x_evm am64x_svb awr294x_evm j721s2_evm j784s4_evm
board_diag_eeprom_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_eeprom_$(SOC)_CORELIST
export board_diag_eeprom_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_eeprom

# EMAC
board_diag_emac_COMP_LIST = board_diag_emac
board_diag_emac_RELPATH = ti/board/diag/emac/build
board_diag_emac_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/emac/build
board_diag_emac_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_emac_MAKEFILE = -f makefile
board_diag_emac_BOARD_DEPENDENCY = yes
board_diag_emac_CORE_DEPENDENCY = yes
export board_diag_emac_COMP_LIST
export board_diag_emac_BOARD_DEPENDENCY
export board_diag_emac_CORE_DEPENDENCY
export board_diag_emac_MAKEFILE
board_diag_emac_PKG_LIST = board_diag_emac
board_diag_emac_INCLUDE = $(board_diag_emac_PATH)
board_diag_emac_BOARDLIST = am65xx_evm am65xx_idk
board_diag_emac_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_emac_$(SOC)_CORELIST
export board_diag_emac_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_emac

# EMMC
board_diag_emmc_COMP_LIST = board_diag_emmc
board_diag_emmc_RELPATH = ti/board/diag/emmc/build
board_diag_emmc_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/emmc/build
board_diag_emmc_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_emmc_MAKEFILE = -f makefile
board_diag_emmc_BOARD_DEPENDENCY = yes
board_diag_emmc_CORE_DEPENDENCY = yes
export board_diag_emmc_COMP_LIST
export board_diag_emmc_BOARD_DEPENDENCY
export board_diag_emmc_CORE_DEPENDENCY
export board_diag_emmc_MAKEFILE
board_diag_emmc_PKG_LIST = board_diag_emmc
board_diag_emmc_INCLUDE = $(board_diag_emmc_PATH)
board_diag_emmc_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm am64x_svb j721s2_evm j784s4_evm
board_diag_emmc_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_emmc_$(SOC)_CORELIST
export board_diag_emmc_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_emmc

# ENET
board_diag_enet_COMP_LIST = board_diag_enet
board_diag_enet_RELPATH = ti/board/diag/enet/build
board_diag_enet_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/enet/build
board_diag_enet_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_enet_MAKEFILE = -f makefile
board_diag_enet_BOARD_DEPENDENCY = yes
board_diag_enet_CORE_DEPENDENCY = yes
export board_diag_enet_COMP_LIST
export board_diag_enet_BOARD_DEPENDENCY
export board_diag_enet_CORE_DEPENDENCY
export board_diag_enet_MAKEFILE
board_diag_enet_PKG_LIST = board_diag_enet
board_diag_enet_INCLUDE = $(board_diag_enet_PATH)
board_diag_enet_BOARDLIST = j784s4_evm
board_diag_enet_$(SOC)_CORELIST = mcu2_0
export board_diag_enet_$(SOC)_CORELIST
export board_diag_enet_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_enet

# EXPANSION HEADER
board_diag_expHeader_COMP_LIST = board_diag_expHeader
board_diag_expHeader_RELPATH = ti/board/diag/exp_header/build
board_diag_expHeader_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/exp_header/build
board_diag_expHeader_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_expHeader_MAKEFILE = -f makefile
board_diag_expHeader_BOARD_DEPENDENCY = yes
board_diag_expHeader_CORE_DEPENDENCY = yes
export board_diag_expHeader_COMP_LIST
export board_diag_expHeader_BOARD_DEPENDENCY
export board_diag_expHeader_CORE_DEPENDENCY
export board_diag_expHeader_MAKEFILE
board_diag_expHeader_PKG_LIST = board_diag_expHeader
board_diag_expHeader_INCLUDE = $(board_diag_expHeader_PATH)
board_diag_expHeader_BOARDLIST = j721e_evm j7200_evm tpr12_evm am64x_evm j784s4_evm
board_diag_expHeader_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_expHeader_$(SOC)_CORELIST
export board_diag_expHeader_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_expHeader

# EXTERNAL RTC
board_diag_extRtc_COMP_LIST = board_diag_extRtc
board_diag_extRtc_RELPATH = ti/board/diag/ext_rtc/build
board_diag_extRtc_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/ext_rtc/build
board_diag_extRtc_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_extRtc_MAKEFILE = -f makefile
board_diag_extRtc_BOARD_DEPENDENCY = yes
board_diag_extRtc_CORE_DEPENDENCY = yes
export board_diag_extRtc_COMP_LIST
export board_diag_extRtc_BOARD_DEPENDENCY
export board_diag_extRtc_CORE_DEPENDENCY
export board_diag_extRtc_MAKEFILE
board_diag_extRtc_PKG_LIST = board_diag_extRtc
board_diag_extRtc_INCLUDE = $(board_diag_extRtc_PATH)
board_diag_extRtc_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm j721s2_evm j784s4_evm
board_diag_extRtc_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_extRtc_$(SOC)_CORELIST
export board_diag_extRtc_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_extRtc

# FPD LIB
board_diag_fpdLib_COMP_LIST = board_diag_fpdLib
board_diag_fpdLib_RELPATH = ti/board/diag/fpd_lib/build
board_diag_fpdLib_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/fpd_lib/build
board_diag_fpdLib_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_fpdLib_MAKEFILE = -f makefile
board_diag_fpdLib_BOARD_DEPENDENCY = yes
board_diag_fpdLib_CORE_DEPENDENCY = yes
export board_diag_fpdLib_COMP_LIST
export board_diag_fpdLib_BOARD_DEPENDENCY
export board_diag_fpdLib_CORE_DEPENDENCY
export board_diag_fpdLib_MAKEFILE
board_diag_fpdLib_PKG_LIST = board_diag_fpdLib
board_diag_fpdLib_INCLUDE = $(board_diag_fpdLib_PATH)
board_diag_fpdLib_BOARDLIST = j721e_evm j7200_evm
board_diag_fpdLib_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_fpdLib_$(SOC)_CORELIST
export board_diag_fpdLib_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_fpdLib

# Framework
board_diag_framework_COMP_LIST = board_diag_framework
board_diag_framework_RELPATH = ti/board/diag/framework/build
board_diag_framework_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/framework/build
board_diag_framework_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_framework_MAKEFILE = -f makefile
board_diag_framework_BOARD_DEPENDENCY = yes
board_diag_framework_CORE_DEPENDENCY = yes
export board_diag_framework_COMP_LIST
export board_diag_framework_BOARD_DEPENDENCY
export board_diag_framework_CORE_DEPENDENCY
export board_diag_framework_MAKEFILE
board_diag_framework_PKG_LIST = board_diag_framework
board_diag_framework_INCLUDE = $(board_diag_framework_PATH)
board_diag_framework_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm
board_diag_framework_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_framework_$(SOC)_CORELIST
export board_diag_framework_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
#board_diag_EXAMPLE_LIST += board_diag_framework

# GPMC
board_diag_gpmc_COMP_LIST = board_diag_gpmc
board_diag_gpmc_RELPATH = ti/board/diag/gpmc/build
board_diag_gpmc_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/gpmc/build
board_diag_gpmc_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_gpmc_MAKEFILE = -f makefile
board_diag_gpmc_BOARD_DEPENDENCY = yes
board_diag_gpmc_CORE_DEPENDENCY = yes
export board_diag_gpmc_COMP_LIST
export board_diag_gpmc_BOARD_DEPENDENCY
export board_diag_gpmc_CORE_DEPENDENCY
export board_diag_gpmc_MAKEFILE
board_diag_gpmc_PKG_LIST = board_diag_gpmc
board_diag_gpmc_INCLUDE = $(board_diag_gpmc_PATH)
board_diag_gpmc_BOARDLIST = am64x_svb am640x_svb
board_diag_gpmc_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_gpmc_$(SOC)_CORELIST
export board_diag_gpmc_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_gpmc

# HDMI
board_diag_hdmi_COMP_LIST = board_diag_hdmi
board_diag_hdmi_RELPATH = ti/board/diag/hdmi/build
board_diag_hdmi_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/hdmi/build
board_diag_hdmi_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_hdmi_MAKEFILE = -f makefile
board_diag_hdmi_BOARD_DEPENDENCY = yes
board_diag_hdmi_CORE_DEPENDENCY = yes
export board_diag_hdmi_COMP_LIST
export board_diag_hdmi_BOARD_DEPENDENCY
export board_diag_hdmi_CORE_DEPENDENCY
export board_diag_hdmi_MAKEFILE
board_diag_hdmi_PKG_LIST = board_diag_hdmi
board_diag_hdmi_INCLUDE = $(board_diag_hdmi_PATH)
board_diag_hdmi_BOARDLIST = j721e_evm
board_diag_hdmi_$(SOC)_CORELIST = mcu2_0
export board_diag_hdmi_$(SOC)_CORELIST
export board_diag_hdmi_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_hdmi

# HYPERBUS
board_diag_hyperbus_COMP_LIST = board_diag_hyperbus
board_diag_hyperbus_RELPATH = ti/board/diag/hyperbus/build
board_diag_hyperbus_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/hyperbus/build
board_diag_hyperbus_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_hyperbus_MAKEFILE = -f makefile
board_diag_hyperbus_BOARD_DEPENDENCY = yes
board_diag_hyperbus_CORE_DEPENDENCY = yes
export board_diag_hyperbus_COMP_LIST
export board_diag_hyperbus_BOARD_DEPENDENCY
export board_diag_hyperbus_CORE_DEPENDENCY
export board_diag_hyperbus_MAKEFILE
board_diag_hyperbus_PKG_LIST = board_diag_hyperbus
board_diag_hyperbus_INCLUDE = $(board_diag_hyperbus_PATH)
board_diag_hyperbus_BOARDLIST = j721e_evm j7200_evm
board_diag_hyperbus_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_hyperbus_$(SOC)_CORELIST
export board_diag_hyperbus_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_hyperbus

# enet_icssg
export board_diag_enetIcssg_COMP_LIST = board_diag_enetIcssg
board_diag_enetIcssg_RELPATH = ti/board/diag/enet_icssg/build
board_diag_enetIcssg_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/enet_icssg/build
board_diag_enetIcssg_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_enetIcssg_MAKEFILE = -f makefile
export board_diag_enetIcssg_BOARD_DEPENDENCY = yes
export board_diag_enetIcssg_CORE_DEPENDENCY = yes
board_diag_enetIcssg_PKG_LIST = board_diag_enetIcssg
board_diag_enetIcssg_INCLUDE = $(board_diag_enetIcssg_PATH)
board_diag_enetIcssg_BOARDLIST = am65xx_evm am65xx_idk
export board_diag_enetIcssg_$(SOC)_CORELIST = mcu1_0
export board_diag_enetIcssg_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_enetIcssg

# icssg_led
export board_diag_icssgLed_COMP_LIST = board_diag_icssgLed
board_diag_icssgLed_RELPATH = ti/board/diag/icssg_led/build
board_diag_icssgLed_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/icssg_led/build
board_diag_icssgLed_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_icssgLed_MAKEFILE = -f makefile
export board_diag_icssgLed_BOARD_DEPENDENCY = yes
export board_diag_icssgLed_CORE_DEPENDENCY = yes
board_diag_icssgLed_PKG_LIST = board_diag_icssgLed
board_diag_icssgLed_INCLUDE = $(board_diag_icssgLed_PATH)
board_diag_icssgLed_BOARDLIST = am65xx_idk
export board_diag_icssgLed_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_icssgLed_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_icssgLed

# lcd
export board_diag_lcd_COMP_LIST = board_diag_lcd
board_diag_lcd_RELPATH = ti/board/diag/lcd/build
board_diag_lcd_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/lcd/build
board_diag_lcd_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_lcd_MAKEFILE = -f makefile
export board_diag_lcd_BOARD_DEPENDENCY = yes
export board_diag_lcd_CORE_DEPENDENCY = yes
board_diag_lcd_PKG_LIST = board_diag_lcd
board_diag_lcd_INCLUDE = $(board_diag_lcd_PATH)
board_diag_lcd_BOARDLIST = am65xx_evm
export board_diag_lcd_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_lcd_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_lcd

# LED
board_diag_led_COMP_LIST = board_diag_led
board_diag_led_RELPATH = ti/board/diag/led/build
board_diag_led_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/led/build
board_diag_led_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_led_MAKEFILE = -f makefile
board_diag_led_BOARD_DEPENDENCY = yes
board_diag_led_CORE_DEPENDENCY = yes
export board_diag_led_COMP_LIST
export board_diag_led_BOARD_DEPENDENCY
export board_diag_led_CORE_DEPENDENCY
export board_diag_led_MAKEFILE
board_diag_led_PKG_LIST = board_diag_led
board_diag_led_INCLUDE = $(board_diag_led_PATH)
board_diag_led_BOARDLIST = j721e_evm am65xx_evm am65xx_idk tpr12_evm j7200_evm am64x_evm awr294x_evm j721s2_evm j784s4_evm
board_diag_led_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_led_$(SOC)_CORELIST
export board_diag_led_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_led

# led_industrial
export board_diag_ledIndustrial_COMP_LIST = board_diag_ledIndustrial
board_diag_ledIndustrial_RELPATH = ti/board/diag/led_industrial/build
board_diag_ledIndustrial_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/led_industrial/build
board_diag_ledIndustrial_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_ledIndustrial_MAKEFILE = -f makefile
export board_diag_ledIndustrial_BOARD_DEPENDENCY = yes
export board_diag_ledIndustrial_CORE_DEPENDENCY = yes
board_diag_ledIndustrial_PKG_LIST = board_diag_ledIndustrial
board_diag_ledIndustrial_INCLUDE = $(board_diag_ledIndustrial_PATH)
board_diag_ledIndustrial_BOARDLIST = am65xx_evm am65xx_idk am64x_evm
export board_diag_ledIndustrial_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_ledIndustrial_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_ledIndustrial

# LEO PMIC LIB
board_diag_leoPmicLib_COMP_LIST = board_diag_leoPmicLib
board_diag_leoPmicLib_RELPATH = ti/board/diag/leo_pmic_lib/build
board_diag_leoPmicLib_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/leo_pmic_lib/build
board_diag_leoPmicLib_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_leoPmicLib_MAKEFILE = -f makefile
board_diag_leoPmicLib_BOARD_DEPENDENCY = yes
board_diag_leoPmicLib_CORE_DEPENDENCY = yes
export board_diag_leoPmicLib_COMP_LIST
export board_diag_leoPmicLib_BOARD_DEPENDENCY
export board_diag_leoPmicLib_CORE_DEPENDENCY
export board_diag_leoPmicLib_MAKEFILE
board_diag_leoPmicLib_PKG_LIST = board_diag_leoPmicLib
board_diag_leoPmicLib_INCLUDE = $(board_diag_leoPmicLib_PATH)
board_diag_leoPmicLib_BOARDLIST = j721e_evm
board_diag_leoPmicLib_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_leoPmicLib_$(SOC)_CORELIST
export board_diag_leoPmicLib_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_leoPmicLib

# LIN
board_diag_lin_COMP_LIST = board_diag_lin
board_diag_lin_RELPATH = ti/board/diag/lin/build
board_diag_lin_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/lin/build
board_diag_lin_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_lin_MAKEFILE = -f makefile
board_diag_lin_BOARD_DEPENDENCY = yes
board_diag_lin_CORE_DEPENDENCY = yes
export board_diag_lin_COMP_LIST
export board_diag_lin_BOARD_DEPENDENCY
export board_diag_lin_CORE_DEPENDENCY
export board_diag_lin_MAKEFILE
board_diag_lin_PKG_LIST = board_diag_lin
board_diag_lin_INCLUDE = $(board_diag_lin_PATH)
board_diag_lin_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm
board_diag_lin_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_lin_$(SOC)_CORELIST
export board_diag_lin_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_lin

# MCAN
board_diag_mcan_COMP_LIST = board_diag_mcan
board_diag_mcan_RELPATH = ti/board/diag/mcan/build
board_diag_mcan_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/mcan/build
board_diag_mcan_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_mcan_MAKEFILE = -f makefile
board_diag_mcan_BOARD_DEPENDENCY = yes
board_diag_mcan_CORE_DEPENDENCY = yes
export board_diag_mcan_COMP_LIST
export board_diag_mcan_BOARD_DEPENDENCY
export board_diag_mcan_CORE_DEPENDENCY
export board_diag_mcan_MAKEFILE
board_diag_mcan_PKG_LIST = board_diag_mcan
board_diag_mcan_INCLUDE = $(board_diag_mcan_PATH)
board_diag_mcan_BOARDLIST = j721e_evm am65xx_idk j7200_evm am64x_evm tpr12_evm am64x_svb awr294x_evm j721s2_evm j784s4_evm
board_diag_mcan_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_mcan_$(SOC)_CORELIST
export board_diag_mcan_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_mcan

# mcasp
export board_diag_mcasp_COMP_LIST = board_diag_mcasp
board_diag_mcasp_RELPATH = ti/board/diag/mcasp/build
board_diag_mcasp_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/mcasp/build
board_diag_mcasp_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_mcasp_MAKEFILE = -f makefile
export board_diag_mcasp_BOARD_DEPENDENCY = yes
export board_diag_mcasp_CORE_DEPENDENCY = yes
board_diag_mcasp_PKG_LIST = board_diag_mcasp
board_diag_mcasp_INCLUDE = $(board_diag_mcasp_PATH)
board_diag_mcasp_BOARDLIST = am65xx_evm
export board_diag_mcasp_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_mcasp_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_mcasp

# MEM
board_diag_mem_COMP_LIST = board_diag_mem
board_diag_mem_RELPATH = ti/board/diag/mem/build
board_diag_mem_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/mem/build
board_diag_mem_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_mem_MAKEFILE = -f makefile
board_diag_mem_BOARD_DEPENDENCY = yes
board_diag_mem_CORE_DEPENDENCY = yes
export board_diag_mem_COMP_LIST
export board_diag_mem_BOARD_DEPENDENCY
export board_diag_mem_CORE_DEPENDENCY
export board_diag_mem_MAKEFILE
board_diag_mem_PKG_LIST = board_diag_mem
board_diag_mem_INCLUDE = $(board_diag_mem_PATH)
board_diag_mem_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm am64x_svb j721s2_evm j784s4_evm
board_diag_mem_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_mem_$(SOC)_CORELIST
export board_diag_mem_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_mem

# MMCSD
board_diag_mmcsd_COMP_LIST = board_diag_mmcsd
board_diag_mmcsd_RELPATH = ti/board/diag/mmcsd/build
board_diag_mmcsd_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/mmcsd/build
board_diag_mmcsd_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_mmcsd_MAKEFILE = -f makefile
board_diag_mmcsd_BOARD_DEPENDENCY = yes
board_diag_mmcsd_CORE_DEPENDENCY = yes
export board_diag_mmcsd_COMP_LIST
export board_diag_mmcsd_BOARD_DEPENDENCY
export board_diag_mmcsd_CORE_DEPENDENCY
export board_diag_mmcsd_MAKEFILE
board_diag_mmcsd_PKG_LIST = board_diag_mmcsd
board_diag_mmcsd_INCLUDE = $(board_diag_mmcsd_PATH)
board_diag_mmcsd_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm am64x_svb j721s2_evm j784s4_evm
board_diag_mmcsd_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_mmcsd_$(SOC)_CORELIST
export board_diag_mmcsd_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_mmcsd

# NOR FLASH
board_diag_norflash_COMP_LIST = board_diag_norflash
board_diag_norflash_RELPATH = ti/board/diag/norflash/build
board_diag_norflash_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/norflash/build
board_diag_norflash_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_norflash_MAKEFILE = -f makefile
board_diag_norflash_BOARD_DEPENDENCY = yes
board_diag_norflash_CORE_DEPENDENCY = yes
export board_diag_norflash_COMP_LIST
export board_diag_norflash_BOARD_DEPENDENCY
export board_diag_norflash_CORE_DEPENDENCY
export board_diag_norflash_MAKEFILE
board_diag_norflash_PKG_LIST = board_diag_norflash
board_diag_norflash_INCLUDE = $(board_diag_norflash_PATH)
board_diag_norflash_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm tpr12_evm am64x_svb awr294x_evm j721s2_evm j784s4_evm
board_diag_norflash_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_norflash_$(SOC)_CORELIST
export board_diag_norflash_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_norflash

# OCTAL NAND
board_diag_octalNand_COMP_LIST = board_diag_octalNand
board_diag_octalNand_RELPATH = ti/board/diag/octal_nand/build
board_diag_octalNand_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/octal_nand/build
board_diag_octalNand_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_octalNand_MAKEFILE = -f makefile
board_diag_octalNand_BOARD_DEPENDENCY = yes
board_diag_octalNand_CORE_DEPENDENCY = yes
export board_diag_octalNand_COMP_LIST
export board_diag_octalNand_BOARD_DEPENDENCY
export board_diag_octalNand_CORE_DEPENDENCY
export board_diag_octalNand_MAKEFILE
board_diag_octalNand_PKG_LIST = board_diag_octalNand
board_diag_octalNand_INCLUDE = $(board_diag_octalNand_PATH)
board_diag_octalNand_BOARDLIST = j721s2_evm j784s4_evm
board_diag_octalNand_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_octalNand_$(SOC)_CORELIST
export board_diag_octalNand_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_octalNand

# OLED DISPLAY
board_diag_oledDisplay_COMP_LIST = board_diag_oledDisplay
board_diag_oledDisplay_RELPATH = ti/board/diag/oled_display/build
board_diag_oledDisplay_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/oled_display/build
board_diag_oledDisplay_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_oledDisplay_MAKEFILE = -f makefile
board_diag_oledDisplay_BOARD_DEPENDENCY = yes
board_diag_oledDisplay_CORE_DEPENDENCY = yes
export board_diag_oledDisplay_COMP_LIST
export board_diag_oledDisplay_BOARD_DEPENDENCY
export board_diag_oledDisplay_CORE_DEPENDENCY
export board_diag_oledDisplay_MAKEFILE
board_diag_oledDisplay_PKG_LIST = board_diag_oledDisplay
board_diag_oledDisplay_INCLUDE = $(board_diag_oledDisplay_PATH)
board_diag_oledDisplay_BOARDLIST = am64x_evm
board_diag_oledDisplay_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_oledDisplay_$(SOC)_CORELIST
export board_diag_oledDisplay_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_oledDisplay

# OSPI
board_diag_ospi_COMP_LIST = board_diag_ospi
board_diag_ospi_RELPATH = ti/board/diag/ospi/build
board_diag_ospi_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/ospi/build
board_diag_ospi_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_ospi_MAKEFILE = -f makefile
board_diag_ospi_BOARD_DEPENDENCY = yes
board_diag_ospi_CORE_DEPENDENCY = yes
export board_diag_ospi_COMP_LIST
export board_diag_ospi_BOARD_DEPENDENCY
export board_diag_ospi_CORE_DEPENDENCY
export board_diag_ospi_MAKEFILE
board_diag_ospi_PKG_LIST = board_diag_ospi
board_diag_ospi_INCLUDE = $(board_diag_ospi_PATH)
board_diag_ospi_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm am64x_svb j721s2_evm j784s4_evm
board_diag_ospi_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_ospi_$(SOC)_CORELIST
export board_diag_ospi_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_ospi

# pcie
export board_diag_pcie_COMP_LIST = board_diag_pcie
board_diag_pcie_RELPATH = ti/board/diag/pcie/build
board_diag_pcie_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/pcie/build
board_diag_pcie_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_pcie_MAKEFILE = -f makefile
export board_diag_pcie_BOARD_DEPENDENCY = yes
export board_diag_pcie_CORE_DEPENDENCY = yes
board_diag_pcie_PKG_LIST = board_diag_pcie
board_diag_pcie_INCLUDE = $(board_diag_pcie_PATH)
board_diag_pcie_BOARDLIST = am65xx_evm am65xx_idk
export board_diag_pcie_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_pcie_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_pcie

# PMIC
board_diag_pmic_COMP_LIST = board_diag_pmic
board_diag_pmic_RELPATH = ti/board/diag/pmic/build
board_diag_pmic_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/pmic/build
board_diag_pmic_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_pmic_MAKEFILE = -f makefile
board_diag_pmic_BOARD_DEPENDENCY = yes
board_diag_pmic_CORE_DEPENDENCY = yes
export board_diag_pmic_COMP_LIST
export board_diag_pmic_BOARD_DEPENDENCY
export board_diag_pmic_CORE_DEPENDENCY
export board_diag_pmic_MAKEFILE
board_diag_pmic_PKG_LIST = board_diag_pmic
board_diag_pmic_INCLUDE = $(board_diag_pmic_PATH)
board_diag_pmic_BOARDLIST = j721e_evm j7200_evm tpr12_evm awr294x_evm j784s4_evm
board_diag_pmic_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_pmic_$(SOC)_CORELIST
export board_diag_pmic_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_pmic

# rotary_switch
export board_diag_rotarySwitch_COMP_LIST = board_diag_rotarySwitch
board_diag_rotarySwitch_RELPATH = ti/board/diag/rotary_switch/build
board_diag_rotarySwitch_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/rotary_switch/build
board_diag_rotarySwitch_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_rotarySwitch_MAKEFILE = -f makefile
export board_diag_rotarySwitch_BOARD_DEPENDENCY = yes
export board_diag_rotarySwitch_CORE_DEPENDENCY = yes
board_diag_rotarySwitch_PKG_LIST = board_diag_rotarySwitch
board_diag_rotarySwitch_INCLUDE = $(board_diag_rotarySwitch_PATH)
board_diag_rotarySwitch_BOARDLIST = am65xx_idk
export board_diag_rotarySwitch_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_rotarySwitch_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_rotarySwitch

# RS485 UART
board_diag_rs485Uart_COMP_LIST = board_diag_rs485Uart
board_diag_rs485Uart_RELPATH = ti/board/diag/rs485_uart/build
board_diag_rs485Uart_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/rs485_uart/build
board_diag_rs485Uart_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_rs485Uart_MAKEFILE = -f makefile
board_diag_rs485Uart_BOARD_DEPENDENCY = yes
board_diag_rs485Uart_CORE_DEPENDENCY = yes
export board_diag_rs485Uart_COMP_LIST
export board_diag_rs485Uart_BOARD_DEPENDENCY
export board_diag_rs485Uart_CORE_DEPENDENCY
export board_diag_rs485Uart_MAKEFILE
board_diag_rs485Uart_PKG_LIST = board_diag_rs485Uart
board_diag_rs485Uart_INCLUDE = $(board_diag_rs485Uart_PATH)
board_diag_rs485Uart_BOARDLIST = am65xx_idk j7200_evm
board_diag_rs485Uart_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_rs485Uart_$(SOC)_CORELIST
export board_diag_rs485Uart_SBL_APPIMAGEGEN = yes
board_diag_EXAMPLE_LIST += board_diag_rs485Uart

# SPI EEPROM
board_diag_spiEeprom_COMP_LIST = board_diag_spiEeprom
board_diag_spiEeprom_RELPATH = ti/board/diag/spi_eeprom/build
board_diag_spiEeprom_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/spi_eeprom/build
board_diag_spiEeprom_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_spiEeprom_MAKEFILE = -f makefile
board_diag_spiEeprom_BOARD_DEPENDENCY = yes
board_diag_spiEeprom_CORE_DEPENDENCY = yes
export board_diag_spiEeprom_COMP_LIST
export board_diag_spiEeprom_BOARD_DEPENDENCY
export board_diag_spiEeprom_CORE_DEPENDENCY
export board_diag_spiEeprom_MAKEFILE
board_diag_spiEeprom_PKG_LIST = board_diag_spiEeprom
board_diag_spiEeprom_INCLUDE = $(board_diag_spiEeprom_PATH)
board_diag_spiEeprom_BOARDLIST = am64x_evm
board_diag_spiEeprom_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_spiEeprom_$(SOC)_CORELIST
export board_diag_spiEeprom_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_spiEeprom

# TEMPERATURE
board_diag_temperature_COMP_LIST = board_diag_temperature
board_diag_temperature_RELPATH = ti/board/diag/temperature/build
board_diag_temperature_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/temperature/build
board_diag_temperature_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_temperature_MAKEFILE = -f makefile
board_diag_temperature_BOARD_DEPENDENCY = yes
board_diag_temperature_CORE_DEPENDENCY = yes
export board_diag_temperature_COMP_LIST
export board_diag_temperature_BOARD_DEPENDENCY
export board_diag_temperature_CORE_DEPENDENCY
export board_diag_temperature_MAKEFILE
board_diag_temperature_PKG_LIST = board_diag_temperature
board_diag_temperature_INCLUDE = $(board_diag_temperature_PATH)
board_diag_temperature_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm tpr12_evm am64x_evm am64x_svb awr294x_evm j721s2_evm j784s4_evm
board_diag_temperature_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_temperature_$(SOC)_CORELIST
export board_diag_temperature_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_temperature

# UART
board_diag_uart_COMP_LIST = board_diag_uart
board_diag_uart_RELPATH = ti/board/diag/uart/build
board_diag_uart_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/uart/build
board_diag_uart_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
board_diag_uart_MAKEFILE = -f makefile
board_diag_uart_BOARD_DEPENDENCY = yes
board_diag_uart_CORE_DEPENDENCY = yes
export board_diag_uart_COMP_LIST
export board_diag_uart_BOARD_DEPENDENCY
export board_diag_uart_CORE_DEPENDENCY
export board_diag_uart_MAKEFILE
board_diag_uart_PKG_LIST = board_diag_uart
board_diag_uart_INCLUDE = $(board_diag_uart_PATH)
board_diag_uart_BOARDLIST = j721e_evm am65xx_evm am65xx_idk j7200_evm tpr12_evm am64x_evm awr294x_evm j721s2_evm j784s4_evm
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
board_diag_uart_$(SOC)_CORELIST = mcu1_0 c66xdsp_1
else
board_diag_uart_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
endif
export board_diag_uart_$(SOC)_CORELIST
export board_diag_uart_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_uart

# usb_device
export board_diag_usbDevice_COMP_LIST = board_diag_usbDevice
board_diag_usbDevice_RELPATH = ti/board/diag/usb_device/build
board_diag_usbDevice_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/usb_device/build
board_diag_usbDevice_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_usbDevice_MAKEFILE = -f makefile
export board_diag_usbDevice_BOARD_DEPENDENCY = yes
export board_diag_usbDevice_CORE_DEPENDENCY = yes
board_diag_usbDevice_PKG_LIST = board_diag_usbDevice
board_diag_usbDevice_INCLUDE = $(board_diag_usbDevice_PATH)
board_diag_usbDevice_BOARDLIST = am65xx_evm am65xx_idk
export board_diag_usbDevice_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_usbDevice_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_usbDevice

# usb_host
export board_diag_usbHost_COMP_LIST = board_diag_usbHost
board_diag_usbHost_RELPATH = ti/board/diag/usb_host/build
board_diag_usbHost_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/usb_host/build
board_diag_usbHost_CUSTOM_BINPATH = $(board_diag_LOCAL_BINPATH)
export board_diag_usbHost_MAKEFILE = -f makefile
export board_diag_usbHost_BOARD_DEPENDENCY = yes
export board_diag_usbHost_CORE_DEPENDENCY = yes
board_diag_usbHost_PKG_LIST = board_diag_usbHost
board_diag_usbHost_INCLUDE = $(board_diag_usbHost_PATH)
board_diag_usbHost_BOARDLIST = am65xx_evm am65xx_idk
export board_diag_usbHost_$(SOC)_CORELIST = $(board_diag_$(SOC)_CORELIST)
export board_diag_usbHost_SBL_APPIMAGEGEN = $(board_diag_APPIMAGEGEN_CTRL)
board_diag_EXAMPLE_LIST += board_diag_usbHost

# Diagnostic test image creation for SD boot.
# Keep this at the end to get executed after building all the diagnostic tests
board_diag_img_COMP_LIST = board_diag_img
board_diag_img_RELPATH = ti/board/diag/build
board_diag_img_PATH = $(PDK_BOARD_DIAG_COMP_PATH)/build
board_diag_img_MAKEFILE = -f board_diag_img.mk
board_diag_img_BOARD_DEPENDENCY = no
board_diag_img_CORE_DEPENDENCY = no
board_diag_img_PKG_LIST = board_diag_img
export board_diag_img_COMP_LIST
export board_diag_img_BOARD_DEPENDENCY
export board_diag_img_CORE_DEPENDENCY
export board_diag_img_MAKEFILE
board_diag_img_INCLUDE = $(board_diag_img_PATH)
board_diag_img_BOARDLIST = j721e_evm
board_diag_img_$(SOC)_CORELIST = mpu1_0
export board_diag_img_$(SOC)_CORELIST
board_diag_EXAMPLE_LIST += board_diag_img

ifneq ($(BUILD_BOARD_DIAG),yes)
board_diag_EXAMPLE_LIST =
endif
export board_diag_EXAMPLE_LIST

# Diagnostic test build flags
ifeq ($(BUILD_PROFILE),debug)
BOARD_DIAG_CFLAGS = -DPDK_RAW_BOOT
PROFILE = debug
export PROFILE
export BOARD_DIAG_CFLAGS
endif

board_diag_component_make_include := 1
endif
