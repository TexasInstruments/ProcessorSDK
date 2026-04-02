# File: enet_ut_component.mk
#       This file is component include make file of ENET unit test.
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
ifeq ($(enet_ut_component_make_include), )

############################
# enet_ut package
# List of components included under enet_ut
# The components included here are built and will be part of enet_ut lib
############################
enet_ut_LIB_LIST =
enet_ut_EXAMPLE_LIST =


#
# Enet unit test app test_framework (TI-RTOS, FreeRTOS)
#
# Define the rule to generate the 'Enet unit test pp' make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule.
#   - In this case it is $(curos), each instance in "drvenet_RTOS_LIST" (ie, tirtos/freertos/safertos)
# - The target name will be <app_name>_<rtos_type> (ie, enet_example_utils_tirtos/enet_example_utils_freertos/...)
# - Here BOARDLIST passed for the target is drivers BOARDLIST for the particular rtos_type(ie, $(drvenet_$(rtos_type)_BOARDLIST). )
#   - If passing a custom or common BOARDLIST, one **should** always filter with Default BOARDLIST for each rtos_type(ie, $(DEFAULT_BOARDLIST_$(rtos_type)). )
#   - For example "export enet_example_utils_$(1)_BOARDLIST =  $(filter $(DEFAULT_BOARDLIST_$(1)), am64x_evm j721e_evm )
#   - This is because, in case of passing custom or common BOARDLIST, some boards may not be supporting all rtos_types
# - CORELIST passed for the target **should** always be filtered with the Default CORELIST of the SOC for each rtos_type (ie, $(DEFAULT_$(SOC)_CORELIST_$(1)). )
#   - The default CORELIST of an SOC for each rtos_type is defined in 'ti/build/makerules/component.mk'
#   - This is because some rtos_type won't be supported on specific cores. (For example, FreeRTOS is not supported on mpu1_0 core)
# - SafeRTOS example should be added to the example list only if SafeRTOS Kernel is present in the path.
#
define ENET_UNIT_TESTAPP_RULE

export enet_unit_testapp_$(1)_COMP_LIST = enet_unit_testapp_$(1)
enet_unit_testapp_$(1)_RELPATH = ti/drv/enet/unit_test/test_framework
enet_unit_testapp_$(1)_PATH = $(PDK_ENET_COMP_PATH)/unit_test/test_framework
export enet_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export enet_unit_testapp_$(1)_CORE_DEPENDENCY = yes
export enet_unit_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos,$(1)),yes,no)
export enet_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes
export enet_unit_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
enet_unit_testapp_$(1)_PKG_LIST = enet_unit_testapp_$(1)
enet_unit_testapp_$(1)_INCLUDE = $(enet_unit_testapp_$(1)_PATH)
export enet_unit_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_$(SOC)_CORELIST))
export enet_unit_testapp_$(1)_BOARDLIST = $(drvenet_K3_BOARDLIST)
ifeq ($(1),$(filter $(1), freertos))
enet_ut_EXAMPLE_LIST += enet_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_ut_EXAMPLE_LIST += enet_unit_testapp_$(1)
endif
endif

endef

ENET_UNIT_TESTAPP_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST),$(call ENET_UNIT_TESTAPP_RULE,$(curos)))

$(eval ${ENET_UNIT_TESTAPP_MACRO_LIST})

export enet_ut_LIB_LIST
export enet_ut_EXAMPLE_LIST

enet_ut_component_make_include := 1
endif
