# File: pdm_utils_component.mk
#       This file is component include for PDM utils.
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
ifeq ($(pdm_utils_component_make_include), )

pdm_utils_SOCLIST         = j721e j7200 j721s2 j784s4
pdm_utils_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm
pdm_utils_j784s4_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1  c7x_2 c7x_3 c7x_4
pdm_utils_j721s2_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
pdm_utils_j721e_CORELIST  = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1
pdm_utils_j7200_CORELIST  = mcu1_0 mcu1_1 mcu2_0 mcu2_1
pdm_utils_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

############################
# PDM utils package
# List of components included under PDM utils lib
# The components included here are built and will be part of PDM utils lib
############################
pdm_utils_LIB_LIST =

############################
# PDM utils examples
# List of examples under PDM utils (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
pdm_utils_EXAMPLE_LIST =

# PDM utils library
pdm_utils_COMP_LIST = pdm_utils
pdm_utils_RELPATH = ti/drv/pdm_utils/src
pdm_utils_PATH = $(PDM_UTILS_COMP_PATH)/src
pdm_utils_LIBNAME = pdm_utils
pdm_utils_LIBPATH = $(PDM_UTILS_COMP_PATH)/lib
pdm_utils_MAKEFILE = -fmakefile
export pdm_utils_MAKEFILE
export pdm_utils_LIBNAME
export pdm_utils_LIBPATH
pdm_utils_BOARD_DEPENDENCY = yes
pdm_utils_CORE_DEPENDENCY = yes
export pdm_utils_COMP_LIST
export pdm_utils_BOARD_DEPENDENCY
export pdm_utils_CORE_DEPENDENCY
pdm_utils_PKG_LIST = pdm_utils
pdm_utils_INCLUDE = $(pdm_utils_PATH)
export pdm_utils_SOCLIST
export pdm_utils_$(SOC)_CORELIST
pdm_utils_LIB_LIST += pdm_utils
export pdm_utils_LIB_LIST

# DM consolidation App
define DM_POWER_ANALYSIS_APP_RULE
export dm_power_analysis_app_$(1)_COMP_LIST = dm_power_analysis_app_$(1)
dm_power_analysis_app_$(1)_RELPATH = ti/drv/pdm_utils/examples/dm_power_analysis_app
dm_power_analysis_app_$(1)_PATH = $(PDM_UTILS_COMP_PATH)/examples/dm_power_analysis_app
export dm_power_analysis_app_$(1)_BOARD_DEPENDENCY = yes
export dm_power_analysis_app_$(1)_CORE_DEPENDENCY = yes
export dm_power_analysis_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
dm_power_analysis_app_$(1)_PKG_LIST = dm_power_analysis_app_$(1)
dm_power_analysis_app_$(1)_INCLUDE = $(dm_power_analysis_app_$(1)_PATH)
export dm_power_analysis_app_$(1)_BOARDLIST = j784s4_evm j721s2_evm j721e_evm j7200_evm
export dm_power_analysis_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(pdm_utils_$(SOC)_CORELIST))
export dm_power_analysis_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
pdm_utils_EXAMPLE_LIST += dm_power_analysis_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
pdm_utils_EXAMPLE_LIST += dm_power_analysis_app_$(1)
endif
endif
endef

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS)

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
CFLAGS_LOCAL_COMMON += -DCONFIG_PSC_PD_MAX_COUNT_64
endif

DM_POWER_ANALYSIS_APP_MACRO_LIST := $(foreach curos, $(pdm_utils_RTOS_LIST), $(call DM_POWER_ANALYSIS_APP_RULE,$(curos)))
$(eval ${DM_POWER_ANALYSIS_APP_MACRO_LIST})

export pdm_utils_LIB_LIST
export pdm_utils_EXAMPLE_LIST


pdm_utils_component_make_include := 1
endif
