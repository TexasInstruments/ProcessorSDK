# File: udma_ut_component.mk
#       This file is component include make file of UDMA unit test.
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
ifeq ($(udma_ut_component_make_include), )

drvudma_ut_am65xx_CORELIST = mpu1_0 mcu1_0
drvudma_ut_j721e_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1
drvudma_ut_j721s2_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2
drvudma_ut_j784s4_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4
drvudma_ut_j742s2_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3
drvudma_ut_j7200_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1

drvudma_dynamic_ut_am65xx_CORELIST = mcu1_0
drvudma_dynamic_ut_j721e_CORELIST  = mcu1_0 mcu2_1
drvudma_dynamic_ut_j721s2_CORELIST  = mcu1_0 mcu2_1
drvudma_dynamic_ut_j784s4_CORELIST  = mcu1_0 mcu2_1
drvudma_dynamic_ut_j742s2_CORELIST  = mcu1_0 mcu2_1
drvudma_dynamic_ut_j7200_CORELIST  = mcu1_0 mcu2_1

############################
# udma_ut package
# List of components included under udma_ut
# The components included here are built and will be part of udma_ut lib
############################
udma_ut_LIB_LIST =
udma_ut_EXAMPLE_LIST =


# RTOS udma unit test apps
define UDMA_UNIT_TESTAPP_RULE

export udma_unit_testapp_$(1)_COMP_LIST = udma_unit_testapp_$(1)
udma_unit_testapp_$(1)_RELPATH = ti/drv/udma/unit_test/udma_ut
udma_unit_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/unit_test/udma_ut
export udma_unit_testapp_$(1)_MAKEFILE = -fmakefile  BUILD_OS_TYPE=$(1)
export udma_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_unit_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_unit_testapp_$(1)_XDC_CONFIGURO = no
udma_unit_testapp_$(1)_PKG_LIST = udma_unit_testapp_$(1)
udma_unit_testapp_$(1)_INCLUDE = $(udma_unit_testapp_$(1)_PATH)
export udma_unit_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvudma_BOARDLIST))
export udma_unit_testapp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvudma_ut_$(SOC)_CORELIST))
export udma_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_ut_EXAMPLE_LIST += udma_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_ut_EXAMPLE_LIST += udma_unit_testapp_$(1)
endif
endif

endef

UDMA_UNIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST) safertos, $(call UDMA_UNIT_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_UNIT_TESTAPP_MACRO_LIST})


# RTOS udma unit test apps - with user input
define UDMA_USER_INPUT_UNIT_TESTAPP_RULE

export udma_user_input_unit_testapp_$(1)_COMP_LIST = udma_user_input_unit_testapp_$(1)
udma_user_input_unit_testapp_$(1)_RELPATH = ti/drv/udma/unit_test/udma_ut
udma_user_input_unit_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/unit_test/udma_ut
export udma_user_input_unit_testapp_$(1)_MAKEFILE = -fmakefile  BUILD_OS_TYPE=$(1) UDMA_UT_MANUAL_ENTRY=yes
export udma_user_input_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_user_input_unit_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_user_input_unit_testapp_$(1)_XDC_CONFIGURO = no
udma_user_input_unit_testapp_$(1)_PKG_LIST = udma_user_input_unit_testapp_$(1)
udma_user_input_unit_testapp_$(1)_INCLUDE = $(udma_user_input_unit_testapp_$(1)_PATH)
export udma_user_input_unit_testapp_$(1)_BOARDLIST = $(drvudma_$(1)_BOARDLIST)
export udma_user_input_unit_testapp_$(1)_$(SOC)_CORELIST =  $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvudma_ut_$(SOC)_CORELIST))
export udma_user_input_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_ut_EXAMPLE_LIST += udma_user_input_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_ut_EXAMPLE_LIST += udma_user_input_unit_testapp_$(1)
endif
endif

endef

UDMA_USER_INPUT_UNIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST), $(call UDMA_USER_INPUT_UNIT_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_USER_INPUT_UNIT_TESTAPP_MACRO_LIST})

# RTOS udma unit test apps
define UDMA_MERGED_UNIT_TESTAPP_RULE

export udma_merged_unit_testapp_$(1)_COMP_LIST = udma_merged_unit_testapp_$(1)
udma_merged_unit_testapp_$(1)_RELPATH = ti/drv/udma/unit_test/udma_ut
udma_merged_unit_testapp_$(1)_PATH = $(PDK_UDMA_COMP_PATH)/unit_test/udma_ut
export udma_merged_unit_testapp_$(1)_MAKEFILE = -fmakefile  BUILD_OS_TYPE=$(1) USE_UDMA_MERGED=yes
export udma_merged_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export udma_merged_unit_testapp_$(1)_CORE_DEPENDENCY = yes
export udma_merged_unit_testapp_$(1)_XDC_CONFIGURO = no
udma_merged_unit_testapp_$(1)_PKG_LIST = udma_merged_unit_testapp_$(1)
udma_merged_unit_testapp_$(1)_INCLUDE = $(udma_merged_unit_testapp_$(1)_PATH)
export udma_merged_unit_testapp_$(1)_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
export udma_merged_unit_testapp_$(1)_$(SOC)_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1
export udma_merged_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
udma_ut_EXAMPLE_LIST += udma_merged_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
udma_ut_EXAMPLE_LIST += udma_merged_unit_testapp_$(1)
endif
endif

endef

UDMA_MERGED_UNIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvudma_RTOS_LIST) safertos, $(call UDMA_MERGED_UNIT_TESTAPP_RULE,$(curos)))

$(eval ${UDMA_MERGED_UNIT_TESTAPP_MACRO_LIST})

export udma_ut_LIB_LIST
export udma_ut_EXAMPLE_LIST

UDMAUT_CFLAGS =

# Enable asserts and prints
UDMAUT_CFLAGS += -DUDMAUT_CFG_ASSERT_ENABLE
UDMAUT_CFLAGS += -DUDMAUT_CFG_TRACE_ENABLE

export UDMAUT_CFLAGS

udma_ut_component_make_include := 1
endif
