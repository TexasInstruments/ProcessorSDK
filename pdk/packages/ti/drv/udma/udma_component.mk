# File: udma_component.mk
#       This file is component include make file of UDMA driver library.
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
ifeq ($(udma_component_make_include), )

# List with various rtos_types(such as tirtos(sysbios),freertos,safertos) to build RTOS apps.
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
drvudma_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

drvudma_SOCLIST         = am65xx j721e j7200 am64x j721s2 j784s4 j742s2
drvudma_BOARDLIST       = am65xx_evm am65xx_idk j721e_sim j721e_evm j7200_evm am64x_evm j721s2_evm j784s4_evm j742s2_evm
drvudma_dru_BOARDLIST   = am65xx_evm am65xx_idk j721e_evm j721s2_evm j784s4_evm j742s2_evm
drvudma_am65xx_CORELIST = mpu1_0 mcu1_0 mcu1_1
drvudma_j721e_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1 c7x-hostemu
drvudma_j7200_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1
drvudma_am64x_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 m4f_0
drvudma_j721s2_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2 c7x-hostemu
drvudma_j784s4_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4 c7x-hostemu
drvudma_j742s2_CORELIST  = mpu1_0 mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x-hostemu

# Define the rule to generate UDMA Drivers BOARDLIST for each rtos_type
# Default BOARDLIST for each rtos_type is defined in 'ti/build/makerules/component.mk'
# The following rule filters out UDMA Drivers BOARDLIST for each rtos_type.
# Here $(1) refers tot the first argument passed to the rule.
# In this case it is $(curos), each instance in "drvudma_RTOS_LIST" (ie, tirtos/freertos/safertos..)
define DRV_UDMA_BOARDLIST_RULE

drvudma_$(1)_BOARDLIST     = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvudma_BOARDLIST))
drvudma_dru_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvudma_dru_BOARDLIST))

endef

# Define the macro list with rules of all rtos_types
DRV_UDMA_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call DRV_UDMA_BOARDLIST_RULE,$(curos)))

# Evaluate the macro list to generate BOARDLIST for all rtos_types
$(eval ${DRV_UDMA_BOARDLIST_MACRO_LIST})

ifneq ($(SOC),$(filter $(SOC), am64x am65xx))
drvudma_$(SOC)_example_CORELIST = $(drvudma_$(SOC)_CORELIST)
else
drvudma_am65xx_example_CORELIST = mpu1_0 mcu1_0
drvudma_am64x_example_CORELIST  = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mpu1_0 #m4f for baremetal examples only
endif

############################
# udma package
# List of components included under udma lib
# The components included here are built and will be part of udma lib
############################
udma_LIB_LIST =

############################
# udma examples
# List of examples under udma (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
udma_EXAMPLE_LIST =

#
# UDMA Modules
#

# UDMA library
udma_COMP_LIST = udma
udma_RELPATH = ti/drv/udma
udma_PATH = $(PDK_UDMA_COMP_PATH)
export udma_LIBNAME = udma
export udma_LIBPATH = $(PDK_UDMA_COMP_PATH)/lib
export udma_MAKEFILE = -fsrc/makefile
export udma_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_ccqt j721e_loki j721e_hostemu j721s2_hostemu j784s4_hostemu j742s2_hostemu))
export udma_BOARD_DEPENDENCY = yes
endif
export udma_CORE_DEPENDENCY = yes
udma_PKG_LIST = udma
udma_INCLUDE = $(udma_PATH)
export udma_SOCLIST = $(drvudma_SOCLIST)
export udma_$(SOC)_CORELIST = $(drvudma_$(SOC)_CORELIST)
udma_LIB_LIST += udma

# UDMA merged library
udma_merged_COMP_LIST = udma_merged
udma_merged_RELPATH = ti/drv/udma
udma_merged_PATH = $(PDK_UDMA_COMP_PATH)
export udma_merged_LIBNAME = udma_merged
export udma_merged_LIBPATH = $(PDK_UDMA_COMP_PATH)/lib
export udma_merged_MAKEFILE = -fsrc/udma_merged_makefile
export udma_merged_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_ccqt j721e_loki j721e_hostemu j721s2_hostemu j784s4_hostemu j742s2_hostemu))
export udma_merged_BOARD_DEPENDENCY = yes
endif
export udma_merged_CORE_DEPENDENCY = no
udma_merged_PKG_LIST = udma_merged
udma_merged_INCLUDE = $(udma_merged_PATH)
export udma_merged_SOCLIST = j721e j7200 j721s2 j784s4 j742s2
export udma_merged_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
udma_LIB_LIST += udma_merged

#
# DMA Utils
#
# DMA Utils library
export dmautils_COMP_LIST = dmautils
dmautils_RELPATH = ti/drv/udma/dmautils
dmautils_PATH = $(PDK_UDMA_COMP_PATH)/dmautils
export dmautils_LIBNAME = dmautils
export dmautils_LIBPATH = $(PDK_UDMA_COMP_PATH)/lib
export dmautils_MAKEFILE = -fmakefile
export dmautils_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_ccqt j721e_loki j721e_hostemu j721s2_hostemu j784s4_hostemu j742s2_hostemu))
export dmautils_BOARD_DEPENDENCY = yes
endif
export dmautils_CORE_DEPENDENCY = yes
dmautils_PKG_LIST = dmautils
dmautils_INCLUDE = $(dmautils_PATH)
export dmautils_SOCLIST = j721e j721s2 j784s4 j742s2
export dmautils_$(SOC)_CORELIST = c7x_1 c7x_2 c7x_3 c7x_4 c7x-hostemu
udma_LIB_LIST += dmautils

# UDMA example library
export udma_apputils_COMP_LIST = udma_apputils
udma_apputils_RELPATH = ti/drv/udma/examples/udma_apputils
udma_apputils_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_apputils
export udma_apputils_LIBNAME = udma_apputils
export udma_apputils_LIBPATH = $(PDK_UDMA_COMP_PATH)/lib
export udma_apputils_MAKEFILE = -fmakefile
export udma_apputils_BOARD_DEPENDENCY = no
export udma_apputils_CORE_DEPENDENCY = yes
udma_apputils_PKG_LIST = udma_apputils
udma_apputils_INCLUDE = $(udma_apputils_PATH)
export udma_apputils_SOCLIST = $(drvudma_SOCLIST)
export udma_apputils_$(SOC)_CORELIST = $(drvudma_$(SOC)_CORELIST)
udma_LIB_LIST += udma_apputils

#
# UDMA Examples
#

#
# DMA Utils tests
#

# DMA Utils Baremetal test apps for Host Emulation
export dmautils_baremetal_autoincrement_testapp_COMP_LIST = dmautils_baremetal_autoincrement_testapp
dmautils_baremetal_autoincrement_testapp_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoincrement_test
dmautils_baremetal_autoincrement_testapp_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoincrement_test
export dmautils_baremetal_autoincrement_testapp_BOARD_DEPENDENCY = yes
export dmautils_baremetal_autoincrement_testapp_CORE_DEPENDENCY = yes
export dmautils_baremetal_autoincrement_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
dmautils_baremetal_autoincrement_testapp_PKG_LIST = dmautils_baremetal_autoincrement_testapp
dmautils_baremetal_autoincrement_testapp_INCLUDE = $(dmautils_baremetal_autoincrement_testapp_PATH)
export dmautils_baremetal_autoincrement_testapp_BOARDLIST = j721e_hostemu j721e_sim j721e_loki j721e_ccqt j721s2_hostemu
export dmautils_baremetal_autoincrement_testapp_$(SOC)_CORELIST = c7x-hostemu
udma_EXAMPLE_LIST += dmautils_baremetal_autoincrement_testapp

export dmautils_baremetal_autoinc_1d2d3d_testapp_COMP_LIST = dmautils_baremetal_autoinc_1d2d3d_testapp
dmautils_baremetal_autoinc_1d2d3d_testapp_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_1d2d3d_test
dmautils_baremetal_autoinc_1d2d3d_testapp_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_1d2d3d_test
export dmautils_baremetal_autoinc_1d2d3d_testapp_BOARD_DEPENDENCY = yes
export dmautils_baremetal_autoinc_1d2d3d_testapp_CORE_DEPENDENCY = yes
export dmautils_baremetal_autoinc_1d2d3d_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
dmautils_baremetal_autoinc_1d2d3d_testapp_PKG_LIST = dmautils_baremetal_autoinc_1d2d3d_testapp
dmautils_baremetal_autoinc_1d2d3d_testapp_INCLUDE = $(dmautils_baremetal_autoinc_1d2d3d_testapp_PATH)
export dmautils_baremetal_autoinc_1d2d3d_testapp_BOARDLIST = j721e_hostemu j721e_sim j721e_loki j721e_ccqt j721s2_hostemu
export dmautils_baremetal_autoinc_1d2d3d_testapp_$(SOC)_CORELIST = c7x-hostemu
udma_EXAMPLE_LIST += dmautils_baremetal_autoinc_1d2d3d_testapp

export dmautils_baremetal_autoinc_circular_testapp_COMP_LIST = dmautils_baremetal_autoinc_circular_testapp
dmautils_baremetal_autoinc_circular_testapp_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_circular_test
dmautils_baremetal_autoinc_circular_testapp_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_circular_test
export dmautils_baremetal_autoinc_circular_testapp_BOARD_DEPENDENCY = yes
export dmautils_baremetal_autoinc_circular_testapp_CORE_DEPENDENCY = yes
export dmautils_baremetal_autoinc_circular_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
dmautils_baremetal_autoinc_circular_testapp_PKG_LIST = dmautils_baremetal_autoinc_circular_testapp
dmautils_baremetal_autoinc_circular_testapp_INCLUDE = $(dmautils_baremetal_autoinc_circular_testapp_PATH)
export dmautils_baremetal_autoinc_circular_testapp_BOARDLIST = j721e_hostemu j721e_sim j721e_loki j721e_ccqt j721s2_hostemu
export dmautils_baremetal_autoinc_circular_testapp_$(SOC)_CORELIST = c7x-hostemu
udma_EXAMPLE_LIST += dmautils_baremetal_autoinc_circular_testapp

export dmautils_baremetal_autoinc_compression_testapp_COMP_LIST = dmautils_baremetal_autoinc_compression_testapp
dmautils_baremetal_autoinc_compression_testapp_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_compression_test
dmautils_baremetal_autoinc_compression_testapp_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_compression_test
export dmautils_baremetal_autoinc_compression_testapp_BOARD_DEPENDENCY = yes
export dmautils_baremetal_autoinc_compression_testapp_CORE_DEPENDENCY = yes
export dmautils_baremetal_autoinc_compression_testapp_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
dmautils_baremetal_autoinc_compression_testapp_PKG_LIST = dmautils_baremetal_autoinc_compression_testapp
dmautils_baremetal_autoinc_compression_testapp_INCLUDE = $(dmautils_baremetal_autoinc_compression_testapp_PATH)
export dmautils_baremetal_autoinc_compression_testapp_BOARDLIST = j721s2_hostemu
export dmautils_baremetal_autoinc_compression_testapp_$(SOC)_CORELIST = c7x-hostemu
udma_EXAMPLE_LIST += dmautils_baremetal_autoinc_compression_testapp


# DMA Utils RTOS autoincrement test app
define DMAUTILS_AUTOINCREMENT_TESTAPP_RULE

export dmautils_autoincrement_testapp_$(1)_COMP_LIST = dmautils_autoincrement_testapp_$(1)
dmautils_autoincrement_testapp_$(1)_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoincrement_test
dmautils_autoincrement_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoincrement_test
export dmautils_autoincrement_testapp_$(1)_BOARD_DEPENDENCY = yes
export dmautils_autoincrement_testapp_$(1)_CORE_DEPENDENCY = yes
export dmautils_autoincrement_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
dmautils_autoincrement_testapp_$(1)_PKG_LIST = dmautils_autoincrement_testapp_$(1)
dmautils_autoincrement_testapp_$(1)_INCLUDE = $(dmautils_autoincrement_testapp_$(1)_PATH)
export dmautils_autoincrement_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm j721s2_evm)
export dmautils_autoincrement_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), c7x_1)
udma_EXAMPLE_LIST += dmautils_autoincrement_testapp_$(1)

endef

DMAUTILS_AUTOINCREMENT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call DMAUTILS_AUTOINCREMENT_TESTAPP_RULE,$(curos)))

$(eval ${DMAUTILS_AUTOINCREMENT_TESTAPP_MACRO_LIST})


# DMA Utils RTOS autoinc 1d2d3d test app
define DMAUTILS_AUTOINC_1D2D3D_TESTAPP_RULE

export dmautils_autoinc_1d2d3d_testapp_$(1)_COMP_LIST = dmautils_autoinc_1d2d3d_testapp_$(1)
dmautils_autoinc_1d2d3d_testapp_$(1)_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_1d2d3d_test
dmautils_autoinc_1d2d3d_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_1d2d3d_test
export dmautils_autoinc_1d2d3d_testapp_$(1)_BOARD_DEPENDENCY = yes
export dmautils_autoinc_1d2d3d_testapp_$(1)_CORE_DEPENDENCY = yes
export dmautils_autoinc_1d2d3d_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
dmautils_autoinc_1d2d3d_testapp_$(1)_PKG_LIST = dmautils_autoinc_1d2d3d_testapp_$(1)
dmautils_autoinc_1d2d3d_testapp_$(1)_INCLUDE = $(dmautils_autoinc_1d2d3d_testapp_$(1)_PATH)
export dmautils_autoinc_1d2d3d_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm j721s2_evm)
export dmautils_autoinc_1d2d3d_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), c7x_1)
udma_EXAMPLE_LIST += dmautils_autoinc_1d2d3d_testapp_$(1)

endef

DMAUTILS_AUTOINC_1D2D3D_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call DMAUTILS_AUTOINC_1D2D3D_TESTAPP_RULE,$(curos)))

$(eval ${DMAUTILS_AUTOINC_1D2D3D_TESTAPP_MACRO_LIST})


# DMA Utils RTOS autoinc 1d2d3d test app
define DMAUTILS_AUTOINC_CIRCULAR_TESTAPP_RULE

export dmautils_autoinc_circular_testapp_$(1)_COMP_LIST = dmautils_autoinc_circular_testapp_$(1)
dmautils_autoinc_circular_testapp_$(1)_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_circular_test
dmautils_autoinc_circular_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_circular_test
export dmautils_autoinc_circular_testapp_$(1)_BOARD_DEPENDENCY = yes
export dmautils_autoinc_circular_testapp_$(1)_CORE_DEPENDENCY = yes
export dmautils_autoinc_circular_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
dmautils_autoinc_circular_testapp_$(1)_PKG_LIST = dmautils_autoinc_circular_testapp_$(1)
dmautils_autoinc_circular_testapp_$(1)_INCLUDE = $(dmautils_autoinc_circular_testapp_$(1)_PATH)
export dmautils_autoinc_circular_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm j721s2_evm)
export dmautils_autoinc_circular_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), c7x_1)
udma_EXAMPLE_LIST += dmautils_autoinc_circular_testapp_$(1)

endef

DMAUTILS_AUTOINC_CIRCULAR_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call DMAUTILS_AUTOINC_CIRCULAR_TESTAPP_RULE,$(curos)))

$(eval ${DMAUTILS_AUTOINC_CIRCULAR_TESTAPP_MACRO_LIST})


# DMA Utils RTOS autoinc compression test app
define DMAUTILS_AUTOINC_COMPRESSION_TESTAPP_RULE

export dmautils_autoinc_compression_testapp_$(1)_COMP_LIST = dmautils_autoinc_compression_testapp_$(1)
dmautils_autoinc_compression_testapp_$(1)_RELPATH = ti/drv/udma/dmautils/test/dmautils_autoinc_compression_test
dmautils_autoinc_compression_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/dmautils/test/dmautils_autoinc_compression_test
export dmautils_autoinc_compression_testapp_$(1)_BOARD_DEPENDENCY = yes
export dmautils_autoinc_compression_testapp_$(1)_CORE_DEPENDENCY = yes
export dmautils_autoinc_compression_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
dmautils_autoinc_compression_testapp_$(1)_PKG_LIST = dmautils_autoinc_compression_testapp_$(1)
dmautils_autoinc_compression_testapp_$(1)_INCLUDE = $(dmautils_autoinc_compression_testapp_$(1)_PATH)
export dmautils_autoinc_compression_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), j721s2_evm)
export dmautils_autoinc_compression_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), c7x_1)
udma_EXAMPLE_LIST += dmautils_autoinc_compression_testapp_$(1)

endef

DMAUTILS_AUTOINC_COMPRESSION_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call DMAUTILS_AUTOINC_COMPRESSION_TESTAPP_RULE,$(curos)))

$(eval ${DMAUTILS_AUTOINC_COMPRESSION_TESTAPP_MACRO_LIST})


# RTOS UDMA memcpy test apps

# Define the rule to generate the 'UDMA memcpy test app' make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule.
#   - In this case it is $(curos), each instance in "drvudma_RTOS_LIST" (ie, tirtos/freertos/safertos)
# - The target name will be <app_name>_<rtos_type> (ie, udma_memcpy_testapp_tirtos/udma_memcpy_testapp_freertos/...)
# - Set XDC_CONFIGURO only for 'rtos_type = tirtos'
# - Pass the arg BUILD_OS_TYPE=<rtos_type> to the common makefile for all rtos_types
#   - In the common makefile, make use the param $(BUILD_OS_TYPE)
#   - For example:
#       APP_NAME = udma_memcpy_testapp_$(BUILD_OS_TYPE)
#       ifeq ($(BUILD_OS_TYPE), tirtos)
#           INCLUDE_EXTERNAL_INTERFACES += xdc bios
#       endif
#       ifeq ($(BUILD_OS_TYPE), tirtos)
#           INCLUDE_EXTERNAL_INTERFACES += freertos
#       endif
# - Here BOARDLIST passed for the target is drivers BOARDLIST for the particular rtos_type(ie, $(drvudma_$(rtos_type)_BOARDLIST). )
#   - If passing a custom or common BOARDLIST, one **should** always filter with Default BOARDLIST for each rtos_type(ie, $(DEFAULT_BOARDLIST_$(rtos_type)). )
#   - For example "export udma_memcpy_testapp_$(1)_BOARDLIST =  $(filter $(DEFAULT_BOARDLIST_$(1)), am64x_evm j721e_evm )
#   - This is because, in case of passing custom or common BOARDLIST, some boards may not be supporting all rtos_types
# - CORELIST passed for the target **should** always be filtered with the Default CORELIST of the SOC for each rtos_type (ie, $(DEFAULT_$(SOC)_CORELIST_$(1)). )
#   - The default CORELIST of an SOC for each rtos_type is defined in 'ti/build/makerules/component.mk'
#   - This is because some rtos_type won't be supported on specific cores. (For example, FreeRTOS is not supported on mpu1_0 core)
# - SafeRTOS example should be added to the example list only if SafeRTOS Kernel is present in the path.
define UDMA_MEMCPY_TESTAPP_RULE

export udma_memcpy_testapp_$(1)_COMP_LIST = udma_memcpy_testapp_$(1)
udma_memcpy_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_memcpy_test
udma_memcpy_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_memcpy_test
export udma_memcpy_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_memcpy_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_memcpy_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_memcpy_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_memcpy_testapp_$(1)_PKG_LIST = udma_memcpy_testapp_$(1)
udma_memcpy_testapp_$(1)_INCLUDE = $(udma_memcpy_testapp_$(1)_PATH)
export udma_memcpy_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvudma_BOARDLIST))
ifeq ($(SOC),$(filter $(SOC), j784s4))
export udma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4)
else
ifeq ($(SOC),$(filter $(SOC), j742s2))
export udma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3)
else
ifeq ($(SOC),$(filter $(SOC), j721s2))
export udma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2)
else
ifeq ($(SOC),$(filter $(SOC), j721e))
export udma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1)
else
export udma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvudma_$(SOC)_example_CORELIST))
endif
endif
endif
endif
export udma_memcpy_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_memcpy_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_memcpy_testapp_$(1)
endif
endif

endef

# Define the macro list with 'UDMA memcpy test app' rule of all rtos_types
UDMA_MEMCPY_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST) safertos, $(call UDMA_MEMCPY_TESTAPP_RULE,$(curos)))

# Evaluate the macro list to generate 'UDMA memcpy test app' make rule for all rtos_types
$(eval ${UDMA_MEMCPY_TESTAPP_MACRO_LIST})

# UDMA memcpy baremetal test app
export udma_baremetal_memcpy_testapp_COMP_LIST = udma_baremetal_memcpy_testapp
udma_baremetal_memcpy_testapp_RELPATH = ti/drv/udma/examples/udma_memcpy_test
udma_baremetal_memcpy_testapp_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_memcpy_test
export udma_baremetal_memcpy_testapp_MAKEFILE = -fmakefile_baremetal
export udma_baremetal_memcpy_testapp_BOARD_DEPENDENCY = yes
export udma_baremetal_memcpy_testapp_CORE_DEPENDENCY = yes
udma_baremetal_memcpy_testapp_PKG_LIST = udma_baremetal_memcpy_testapp
udma_baremetal_memcpy_testapp_INCLUDE = $(udma_baremetal_memcpy_testapp_PATH)
export udma_baremetal_memcpy_testapp_BOARDLIST = $(drvudma_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
export udma_baremetal_memcpy_testapp_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0
else
export udma_baremetal_memcpy_testapp_$(SOC)_CORELIST = $(drvudma_$(SOC)_example_CORELIST)
endif
export udma_baremetal_memcpy_testapp_SBL_APPIMAGEGEN = yes
udma_EXAMPLE_LIST += udma_baremetal_memcpy_testapp

# UDMA memcpy tricore freertos test app
export udma_memcpy_tricore_testapp_freertos_COMP_LIST = udma_memcpy_tricore_testapp_freertos
udma_memcpy_tricore_testapp_freertos_RELPATH = ti/drv/udma/examples/udma_memcpy_tricore_test
udma_memcpy_tricore_testapp_freertos_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_memcpy_tricore_test
export udma_memcpy_tricore_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
export udma_memcpy_tricore_testapp_freertos_BOARD_DEPENDENCY = yes
export udma_memcpy_tricore_testapp_freertos_CORE_DEPENDENCY = yes
udma_memcpy_tricore_testapp_freertos_PKG_LIST = udma_memcpy_tricore_testapp_freertos
udma_memcpy_tricore_testapp_freertos_INCLUDE = $(udma_memcpy_tricore_testapp_freertos_PATH)
# j7200 has only two r5 clusters
export udma_memcpy_tricore_testapp_freertos_BOARDLIST = j721e_evm j784s4_evm j721s2_evm j742s2_evm
# Compiling for mcu1_0 will make tricore appimage for mcu1_0, mcu2_0 and mcu3_0
export udma_memcpy_tricore_testapp_freertos_$(SOC)_CORELIST = mcu1_0
export udma_memcpy_tricore_testapp_freertos_SBL_APPIMAGEGEN = yes
export udma_memcpy_tricore_testapp_freertos_TRICORE = yes
udma_EXAMPLE_LIST += udma_memcpy_tricore_testapp_freertos

# RTOS UDMA chaining test apps
define UDMA_CHAINING_TESTAPP_RULE

export udma_chaining_testapp_$(1)_COMP_LIST = udma_chaining_testapp_$(1)
udma_chaining_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_chaining_test
udma_chaining_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_chaining_test
export udma_chaining_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_chaining_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_chaining_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_chaining_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_chaining_testapp_$(1)_PKG_LIST = udma_chaining_testapp_$(1)
udma_chaining_testapp_$(1)_INCLUDE = $(udma_chaining_testapp_$(1)_PATH)
export udma_chaining_testapp_$(1)_BOARDLIST = $(drvudma_$(1)_BOARDLIST)
export udma_chaining_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0)
export udma_chaining_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_chaining_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_chaining_testapp_$(1)
endif
endif

endef

UDMA_CHAINING_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_CHAINING_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_CHAINING_TESTAPP_MACRO_LIST})


# RTOS UDMA SW trigger test apps
define UDMA_SW_TRIGGER_TESTAPP_RULE

export udma_sw_trigger_testapp_$(1)_COMP_LIST = udma_sw_trigger_testapp_$(1)
udma_sw_trigger_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_sw_trigger_test
udma_sw_trigger_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_sw_trigger_test
export udma_sw_trigger_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_sw_trigger_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_sw_trigger_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_sw_trigger_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_sw_trigger_testapp_$(1)_PKG_LIST = udma_sw_trigger_testapp_$(1)
udma_sw_trigger_testapp_$(1)_INCLUDE = $(udma_sw_trigger_testapp_$(1)_PATH)
export udma_sw_trigger_testapp_$(1)_BOARDLIST = $(drvudma_$(1)_BOARDLIST)
export udma_sw_trigger_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu1_0)
export udma_sw_trigger_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_sw_trigger_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_sw_trigger_testapp_$(1)
endif
endif

endef

UDMA_SW_TRIGGER_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_SW_TRIGGER_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_SW_TRIGGER_TESTAPP_MACRO_LIST})


# RTOS UDMA DRU test apps
define UDMA_DRU_TESTAPP_RULE

export udma_dru_testapp_$(1)_COMP_LIST = udma_dru_testapp_$(1)
udma_dru_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_dru_test
udma_dru_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_dru_test
export udma_dru_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_dru_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_dru_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_dru_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_dru_testapp_$(1)_PKG_LIST = udma_dru_testapp_$(1)
udma_dru_testapp_$(1)_INCLUDE = $(udma_dru_testapp_$(1)_PATH)
export udma_dru_testapp_$(1)_BOARDLIST = $(drvudma_dru_$(1)_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e))
export udma_dru_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c66xdsp_1 c66xdsp_2 c7x_1)
else
ifeq ($(SOC),$(filter $(SOC), j721s2))
export udma_dru_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2)
else
ifeq ($(SOC),$(filter $(SOC), j784s4))
export udma_dru_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2 c7x_3 c7x_4)
else ifeq ($(SOC),$(filter $(SOC), j742s2))
export udma_dru_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2 c7x_3)
else
export udma_dru_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvudma_$(SOC)_example_CORELIST))
endif
endif
endif
export udma_dru_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_dru_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_dru_testapp_$(1)
endif
endif

endef

UDMA_DRU_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_DRU_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_DRU_TESTAPP_MACRO_LIST})


# RTOS UDMA DRU Direct TR test apps
define UDMA_DRU_DIRECT_TR_TESTAPP_RULE

export udma_dru_direct_tr_testapp_$(1)_COMP_LIST = udma_dru_direct_tr_testapp_$(1)
udma_dru_direct_tr_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_dru_direct_tr_test
udma_dru_direct_tr_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_dru_direct_tr_test
export udma_dru_direct_tr_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_dru_direct_tr_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_dru_direct_tr_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_dru_direct_tr_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_dru_direct_tr_testapp_$(1)_PKG_LIST = udma_dru_direct_tr_testapp_$(1)
udma_dru_direct_tr_testapp_$(1)_INCLUDE = $(udma_dru_direct_tr_testapp_$(1)_PATH)
export udma_dru_direct_tr_testapp_$(1)_BOARDLIST = $(drvudma_dru_$(1)_BOARDLIST)
ifeq ($(SOC),$(filter $(SOC), j721e))
export udma_dru_direct_tr_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c66xdsp_1 c7x_1)
else
ifeq ($(SOC),$(filter $(SOC), j721s2))
export udma_dru_direct_tr_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2)
else
ifeq ($(SOC),$(filter $(SOC), j784s4))
export udma_dru_direct_tr_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2 c7x_3 c7x_4)
else ifeq ($(SOC),$(filter $(SOC), j742s2))
export udma_dru_direct_tr_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mcu2_0 c7x_1 c7x_2 c7x_3)
else
export udma_dru_direct_tr_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvudma_$(SOC)_example_CORELIST))
endif
endif
endif
export udma_dru_direct_tr_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_dru_direct_tr_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_dru_direct_tr_testapp_$(1)
endif
endif

endef

UDMA_DRU_DIRECT_TR_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_DRU_DIRECT_TR_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_DRU_DIRECT_TR_TESTAPP_MACRO_LIST})


# RTOS UDMA CRC test apps
define UDMA_CRC_TESTAPP_RULE

export udma_crc_testapp_$(1)_COMP_LIST = udma_crc_testapp_$(1)
udma_crc_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_crc_test
udma_crc_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_crc_test
export udma_crc_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_crc_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_crc_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_crc_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_crc_testapp_$(1)_PKG_LIST = udma_crc_testapp_$(1)
udma_crc_testapp_$(1)_INCLUDE = $(udma_crc_testapp_$(1)_PATH)
export udma_crc_testapp_$(1)_BOARDLIST = $(drvudma_$(1)_BOARDLIST)
export udma_crc_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0)
export udma_crc_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_crc_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_crc_testapp_$(1)
endif
endif

endef

UDMA_CRC_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_CRC_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_CRC_TESTAPP_MACRO_LIST})


# RTOS UDMA ADC test apps
define UDMA_ADC_TESTAPP_RULE

export udma_adc_testapp_$(1)_COMP_LIST = udma_adc_testapp_$(1)
udma_adc_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_adc_test
udma_adc_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_adc_test
export udma_adc_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_adc_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_adc_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export udma_adc_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_adc_testapp_$(1)_PKG_LIST = udma_adc_testapp_$(1)
udma_adc_testapp_$(1)_INCLUDE = $(udma_adc_testapp_$(1)_PATH)
export udma_adc_testapp_$(1)_BOARDLIST = $(drvudma_$(1)_BOARDLIST)
export udma_adc_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), mpu1_0 mcu1_0)
export udma_adc_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_adc_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_adc_testapp_$(1)
endif
endif

endef

UDMA_ADC_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_ADC_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_ADC_TESTAPP_MACRO_LIST})


# UDMA Baremetal OSPI Flash test app
export udma_baremetal_ospi_flash_testapp_COMP_LIST = udma_baremetal_ospi_flash_testapp
udma_baremetal_ospi_flash_testapp_RELPATH = ti/drv/udma/examples/udma_ospi_flash_test
udma_baremetal_ospi_flash_testapp_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_ospi_flash_test
export udma_baremetal_ospi_flash_testapp_BOARD_DEPENDENCY = yes
export udma_baremetal_ospi_flash_testapp_CORE_DEPENDENCY = yes
udma_baremetal_ospi_flash_testapp_PKG_LIST = udma_baremetal_ospi_flash_testapp
udma_baremetal_ospi_flash_testapp_INCLUDE = $(udma_baremetal_ospi_flash_testapp_PATH)
export udma_baremetal_ospi_flash_testapp_BOARDLIST = $(drvudma_BOARDLIST)
export udma_baremetal_ospi_flash_testapp_$(SOC)_CORELIST = mcu1_0
export udma_baremetal_ospi_flash_testapp_SBL_APPIMAGEGEN = yes
udma_EXAMPLE_LIST += udma_baremetal_ospi_flash_testapp

# RTOS UDMA MAIN OCMC test apps
define UDMA_MAIN_OCMC_TESTAPP_RULE

# UDMA MAIN OCMC test app
export udma_main_ocmc_testapp_$(1)_COMP_LIST = udma_main_ocmc_testapp_$(1)
udma_main_ocmc_testapp_$(1)_RELPATH = ti/drv/udma/examples/udma_main_ocmc_test
udma_main_ocmc_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/examples/udma_main_ocmc_test
export udma_main_ocmc_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_main_ocmc_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_main_ocmc_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
udma_main_ocmc_testapp_$(1)_PKG_LIST = udma_main_ocmc_testapp_$(1)
udma_main_ocmc_testapp_$(1)_INCLUDE = $(udma_main_ocmc_testapp_$(1)_PATH)
export udma_main_ocmc_testapp_$(1)_BOARDLIST = $(drvudma_BOARDLIST)
export udma_main_ocmc_testapp_$(1)_$(SOC)_CORELIST = mcu2_0
export udma_main_ocmc_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_EXAMPLE_LIST += udma_main_ocmc_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_EXAMPLE_LIST += udma_main_ocmc_testapp_$(1)
endif
endif

endef

UDMA_MAIN_OCMC_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_MAIN_OCMC_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_MAIN_OCMC_TESTAPP_MACRO_LIST})

-include $(PDK_UDMA_COMP_PATH)/unit_test/udma_ut_component.mk
ifneq ($(udma_ut_LIB_LIST),)
  udma_LIB_LIST += $(udma_ut_LIB_LIST)
endif
ifneq ($(udma_ut_EXAMPLE_LIST),)
  udma_EXAMPLE_LIST += $(udma_ut_EXAMPLE_LIST)
endif

export udma_LIB_LIST
export udma_EXAMPLE_LIST
export drvudma_LIB_LIST = $(udma_LIB_LIST)
export drvudma_EXAMPLE_LIST = $(udma_EXAMPLE_LIST)

UDMA_CFLAGS =

# Enable asserts and prints
UDMA_CFLAGS += -DUDMA_CFG_ASSERT_ENABLE
UDMA_CFLAGS += -DUDMA_CFG_PRINT_ENABLE

export UDMA_CFLAGS

udma_component_make_include := 1
endif
