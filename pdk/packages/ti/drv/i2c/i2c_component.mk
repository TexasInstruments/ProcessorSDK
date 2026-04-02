#
# Copyright (c) 2016 - 2024, Texas Instruments Incorporated
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

# File: i2c_component.mk
#       This file is component include make file of I2C library.
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
ifeq ($(i2c_component_make_include), )

drvi2c_BOARDLIST           = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvi2c_SOCLIST             = j721e j7200 j721s2 j784s4 j742s2
drvi2c_j721e_CORELIST      = $(DEFAULT_j721e_CORELIST)
drvi2c_j721e_CORELISTARM   = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvi2c_j721s2_CORELIST     = mpu1_0 mcu1_0 mcu2_0 mcu3_0 c7x_1
drvi2c_j721s2_CORELISTARM  = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1
drvi2c_j784s4_CORELIST     = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1
drvi2c_j784s4_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvi2c_j742s2_CORELIST     = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1
drvi2c_j742s2_CORELISTARM  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
drvi2c_j7200_CORELIST      = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvi2c_RTOS_LIST           = $(DEFAULT_RTOS_LIST)

############################
# i2c package
# List of components included under i2c lib
# The components included here are built and will be part of i2c lib
############################
i2c_LIB_LIST = i2c
drvi2c_LIB_LIST = $(i2c_LIB_LIST)

############################
# i2c examples
# List of examples under i2c
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
i2c_EXAMPLE_LIST = 

#
# I2C Modules
#

# I2C LIB
i2c_COMP_LIST = i2c
i2c_RELPATH = ti/drv/i2c
i2c_PATH = $(PDK_I2C_COMP_PATH)
i2c_LIBNAME = ti.drv.i2c
export i2c_LIBNAME
i2c_LIBPATH = $(i2c_PATH)/lib
export i2c_LIBPATH
i2c_OBJPATH = $(i2c_RELPATH)/i2c
export i2c_OBJPATH
i2c_MAKEFILE = -f build/makefile.mk
export i2c_MAKEFILE
i2c_BOARD_DEPENDENCY = no
i2c_CORE_DEPENDENCY = no
i2c_SOC_DEPENDENCY = yes
export i2c_COMP_LIST
export i2c_BOARD_DEPENDENCY
export i2c_CORE_DEPENDENCY
export i2c_SOC_DEPENDENCY
i2c_PKG_LIST = i2c
export i2c_PKG_LIST
i2c_INCLUDE = $(i2c_PATH)
i2c_SOCLIST = $(drvi2c_SOCLIST)
export i2c_SOCLIST
i2c_$(SOC)_CORELIST = $(drvi2c_$(SOC)_CORELIST)
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
i2c_$(SOC)_CORELIST += mcu2_1 mcu1_1
endif
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
i2c_$(SOC)_CORELIST += mcu3_0
endif
ifeq ($(SOC),$(filter $(SOC),j784s4 j742s2))
i2c_$(SOC)_CORELIST += mcu4_0
endif
export i2c_$(SOC)_CORELIST

#
# I2C Examples
#

# I2C baremetal EEPROM test
i2c_baremetal_eeprom_testApp_COMP_LIST = i2c_baremetal_eeprom_testApp
i2c_baremetal_eeprom_testApp_RELPATH = ti/drv/i2c/test/eeprom_read
i2c_baremetal_eeprom_testApp_PATH = $(PDK_I2C_COMP_PATH)/test/eeprom_read
i2c_baremetal_eeprom_testApp_BOARD_DEPENDENCY = yes
i2c_baremetal_eeprom_testApp_CORE_DEPENDENCY = no
i2c_baremetal_eeprom_testApp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export i2c_baremetal_eeprom_testApp_COMP_LIST
export i2c_baremetal_eeprom_testApp_BOARD_DEPENDENCY
export i2c_baremetal_eeprom_testApp_CORE_DEPENDENCY
export i2c_baremetal_eeprom_testApp_MAKEFILE
i2c_baremetal_eeprom_testApp_PKG_LIST = i2c_baremetal_eeprom_testApp
i2c_baremetal_eeprom_testApp_INCLUDE = $(i2c_baremetal_eeprom_testApp_PATH)
i2c_baremetal_eeprom_testApp_BOARDLIST = j721e_evm j784s4_evm j742s2_evm j7200_evm j721s2_evm
export i2c_baremetal_eeprom_testApp_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), j721e))
i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST = $(drvi2c_j721e_CORELISTARM)
else ifeq ($(SOC),$(filter $(SOC), j784s4))
i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST = $(drvi2c_j784s4_CORELISTARM)
else ifeq ($(SOC),$(filter $(SOC), j742s2))
i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST = $(drvi2c_j742s2_CORELISTARM)
else ifeq ($(SOC),$(filter $(SOC), j721s2))
i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST = $(drvi2c_j721s2_CORELISTARM)
else
i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST = $(drvi2c_$(SOC)_CORELIST)
endif
export i2c_baremetal_eeprom_testApp_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_baremetal_eeprom_testApp
export i2c_baremetal_eeprom_testApp_SBL_APPIMAGEGEN = yes

# I2C extended testapp
i2c_extended_testapp_COMP_LIST = i2c_extended_testapp
i2c_extended_testapp_RELPATH = ti/drv/i2c/test/i2c_extended_testapp
i2c_extended_testapp_PATH = $(PDK_I2C_COMP_PATH)/test/i2c_extended_testapp
i2c_extended_testapp_BOARD_DEPENDENCY = yes
i2c_extended_testapp_CORE_DEPENDENCY = no
i2c_extended_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export i2c_extended_testapp_COMP_LIST
export i2c_extended_testapp_BOARD_DEPENDENCY
export i2c_extended_testapp_CORE_DEPENDENCY
export i2c_extended_testapp_MAKEFILE
i2c_extended_testapp_PKG_LIST = i2c_extended_testapp
i2c_extended_testapp_INCLUDE = $(i2c_extended_testapp_PATH)
i2c_extended_testapp_BOARDLIST = j784s4_evm j742s2_evm j721s2_evm j721e_evm j7200_evm
export i2c_extended_testapp_BOARDLIST
i2c_extended_testapp_$(SOC)_CORELIST = mcu1_0
export i2c_extended_testapp_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_extended_testapp
export i2c_extended_testapp_SBL_APPIMAGEGEN = yes

# I2C slave extended testapp
i2c_slave_extended_testapp_COMP_LIST = i2c_slave_extended_testapp
i2c_slave_extended_testapp_RELPATH = ti/drv/i2c/test/i2c_slave_extended_testapp
i2c_slave_extended_testapp_PATH = $(PDK_I2C_COMP_PATH)/test/i2c_slave_extended_testapp
i2c_slave_extended_testapp_BOARD_DEPENDENCY = yes
i2c_slave_extended_testapp_CORE_DEPENDENCY = no
i2c_slave_extended_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export i2c_slave_extended_testapp_COMP_LIST
export i2c_slave_extended_testapp_BOARD_DEPENDENCY
export i2c_slave_extended_testapp_CORE_DEPENDENCY
export i2c_slave_extended_testapp_MAKEFILE
i2c_slave_extended_testapp_PKG_LIST = i2c_slave_extended_testapp
i2c_slave_extended_testapp_INCLUDE = $(i2c_slave_extended_testapp_PATH)
i2c_slave_extended_testapp_BOARDLIST = j721s2_evm
export i2c_slave_extended_testapp_BOARDLIST
i2c_slave_extended_testapp_$(SOC)_CORELIST = mcu1_0
export i2c_slave_extended_testapp_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_slave_extended_testapp
export i2c_slave_extended_testapp_SBL_APPIMAGEGEN = yes

# I2C master extended testapp
i2c_master_extended_testapp_COMP_LIST = i2c_master_extended_testapp
i2c_master_extended_testapp_RELPATH = ti/drv/i2c/test/i2c_master_extended_testapp
i2c_master_extended_testapp_PATH = $(PDK_I2C_COMP_PATH)/test/i2c_master_extended_testapp
i2c_master_extended_testapp_BOARD_DEPENDENCY = yes
i2c_master_extended_testapp_CORE_DEPENDENCY = no
i2c_master_extended_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
export i2c_master_extended_testapp_COMP_LIST
export i2c_master_extended_testapp_BOARD_DEPENDENCY
export i2c_master_extended_testapp_CORE_DEPENDENCY
export i2c_master_extended_testapp_MAKEFILE
i2c_master_extended_testapp_PKG_LIST = i2c_master_extended_testapp
i2c_master_extended_testapp_INCLUDE = $(i2c_master_extended_testapp_PATH)
i2c_master_extended_testapp_BOARDLIST = j721s2_evm
export i2c_master_extended_testapp_BOARDLIST
i2c_master_extended_testapp_$(SOC)_CORELIST = mcu1_0
export i2c_master_extended_testapp_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_master_extended_testapp
export i2c_master_extended_testapp_SBL_APPIMAGEGEN = yes

# I2C rtos EEPROM test
define i2c_eeprom_testApp_RULE

i2c_eeprom_testApp_$(1)_COMP_LIST = i2c_eeprom_testApp_$(1)
i2c_eeprom_testApp_$(1)_RELPATH = ti/drv/i2c/test/eeprom_read
i2c_eeprom_testApp_$(1)_PATH = $(PDK_I2C_COMP_PATH)/test/eeprom_read
i2c_eeprom_testApp_$(1)_BOARD_DEPENDENCY = yes
i2c_eeprom_testApp_$(1)_CORE_DEPENDENCY = yes
i2c_eeprom_testApp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export i2c_eeprom_testApp_$(1)_COMP_LIST
export i2c_eeprom_testApp_$(1)_BOARD_DEPENDENCY
export i2c_eeprom_testApp_$(1)_CORE_DEPENDENCY
export i2c_eeprom_testApp_$(1)_MAKEFILE
i2c_eeprom_testApp_$(1)_PKG_LIST = i2c_eeprom_testApp_$(1)
i2c_eeprom_testApp_$(1)_INCLUDE = $(i2c_eeprom_testApp_$(1)_PATH)
i2c_eeprom_testApp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvi2c_BOARDLIST))
export i2c_eeprom_testApp_$(1)_BOARDLIST
i2c_eeprom_testApp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(i2c_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
    i2c_EXAMPLE_LIST += i2c_eeprom_testApp_$(1)
else
    ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
        i2c_EXAMPLE_LIST += i2c_eeprom_testApp_$(1)
    endif
endif

export i2c_eeprom_testApp_$(1)_$(SOC)_CORELIST
export i2c_eeprom_testApp_$(1)_SBL_APPIMAGEGEN = yes

endef
#i2c_EXAMPLE_LIST is exported at the end of the file

i2c_eeprom_testApp_MACRO_LIST := $(foreach curos,$(drvi2c_RTOS_LIST),$(call i2c_eeprom_testApp_RULE,$(curos)))
$(eval ${i2c_eeprom_testApp_MACRO_LIST})

# I2C Utility
drv_i2c_utility_COMP_LIST = drv_i2c_utility
drv_i2c_utility_RELPATH = ti/drv/i2c/example/i2c_utility
drv_i2c_utility_PATH = $(PDK_I2C_COMP_PATH)/example/i2c_utility
drv_i2c_utility_BOARD_DEPENDENCY = yes
drv_i2c_utility_CORE_DEPENDENCY = no
drv_i2c_utility_MAKEFILE = -f makefile
export drv_i2c_utility_COMP_LIST
export drv_i2c_utility_BOARD_DEPENDENCY
export drv_i2c_utility_CORE_DEPENDENCY
export drv_i2c_utility_MAKEFILE
drv_i2c_utility_PKG_LIST = drv_i2c_utility
drv_i2c_utility_INCLUDE = $(drv_i2c_utility_PATH)
drv_i2c_utility_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export drv_i2c_utility_BOARDLIST
ifeq ($(SOC),$(filter $(SOC), j721e))
drv_i2c_utility_$(SOC)_CORELIST = $(drvi2c_j721e_CORELISTARM)
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
drv_i2c_utility_$(SOC)_CORELIST = $(drvi2c_j7200_CORELIST)
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
drv_i2c_utility_$(SOC)_CORELIST = $(drvi2c_j721s2_CORELISTARM)
endif
ifeq ($(SOC),$(filter $(SOC), j784s4))
drv_i2c_utility_$(SOC)_CORELIST = $(drvi2c_j784s4_CORELISTARM)
endif
ifeq ($(SOC),$(filter $(SOC), j742s2))
drv_i2c_utility_$(SOC)_CORELIST = $(drvi2c_j742s2_CORELISTARM)
endif
export drv_i2c_utility_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += drv_i2c_utility

# I2C Slave Mode testapp
i2c_slave_mode_app_freertos_COMP_LIST = i2c_slave_mode_app_freertos
i2c_slave_mode_app_freertos_RELPATH = ti/drv/i2c/test/i2c_master_slave/i2c_slave_mode_app
i2c_slave_mode_app_freertos_PATH = $(PDK_I2C_COMP_PATH)/test/i2c_master_slave/i2c_slave_mode_app
i2c_slave_mode_app_freertos_BOARD_DEPENDENCY = yes
i2c_slave_mode_app_freertos_CORE_DEPENDENCY = no
i2c_slave_mode_app_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
export i2c_slave_mode_app_freertos_COMP_LIST
export i2c_slave_mode_app_freertos_BOARD_DEPENDENCY
export i2c_slave_mode_app_freertos_CORE_DEPENDENCY
export i2c_slave_mode_app_freertos_MAKEFILE
i2c_slave_mode_app_freertos_PKG_LIST = i2c_slave_mode_app_freertos
i2c_slave_mode_app_freertos_INCLUDE = $(i2c_slave_mode_app_freertos_PATH)
i2c_slave_mode_app_freertos_BOARDLIST = j721s2_evm
export i2c_slave_mode_app_freertos_BOARDLIST
i2c_slave_mode_app_freertos_$(SOC)_CORELIST = mcu1_0
export i2c_slave_mode_app_freertos_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_slave_mode_app_freertos
export i2c_slave_mode_app_freertos_SBL_APPIMAGEGEN = yes

# I2C Master Mode testapp
i2c_master_mode_app_freertos_COMP_LIST = i2c_master_mode_app_freertos
i2c_master_mode_app_freertos_RELPATH = ti/drv/i2c/test/i2c_master_slave/i2c_master_mode_app
i2c_master_mode_app_freertos_PATH = $(PDK_I2C_COMP_PATH)/test/i2c_master_slave/i2c_master_mode_app
i2c_master_mode_app_freertos_BOARD_DEPENDENCY = yes
i2c_master_mode_app_freertos_CORE_DEPENDENCY = no
i2c_master_mode_app_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
export i2c_master_mode_app_freertos_COMP_LIST
export i2c_master_mode_app_freertos_BOARD_DEPENDENCY
export i2c_master_mode_app_freertos_CORE_DEPENDENCY
export i2c_master_mode_app_freertos_MAKEFILE
i2c_master_mode_app_freertos_PKG_LIST = i2c_master_mode_app_freertos
i2c_master_mode_app_freertos_INCLUDE = $(i2c_master_mode_app_freertos_PATH)
i2c_master_mode_app_freertos_BOARDLIST = j721s2_evm
export i2c_master_mode_app_freertos_BOARDLIST
i2c_master_mode_app_freertos_$(SOC)_CORELIST = mcu1_0
export i2c_master_mode_app_freertos_$(SOC)_CORELIST
i2c_EXAMPLE_LIST += i2c_master_mode_app_freertos
export i2c_master_mode_app_freertos_SBL_APPIMAGEGEN = yes

# Export all supported examples
drvi2c_EXAMPLE_LIST = $(i2c_EXAMPLE_LIST)

export drvi2c_LIB_LIST
export drvi2c_EXAMPLE_LIST
export drvi2c_FIRM_LIST
export i2c_FIRM_LIST
export i2c_LIB_LIST
export i2c_EXAMPLE_LIST

i2c_component_make_include := 1
endif
