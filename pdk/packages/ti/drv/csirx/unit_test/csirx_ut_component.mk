# File: csirx_ut_component.mk
#       This file is component include make file of CSIRX unit test.
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
ifeq ($(csirx_ut_component_make_include), )

############################
# csirx_ut package
# List of components included under csirx_ut
# The components included here are built and will be part of csirx_ut lib
############################
csirx_ut_LIB_LIST =
csirx_ut_EXAMPLE_LIST =

# csirx unit test app
define CSIRX_UNIT_TESTAPP_RULE

export csirx_unit_testapp_$(1)_COMP_LIST = csirx_unit_testapp_$(1)
csirx_unit_testapp_$(1)_RELPATH = ti/drv/csirx/unit_test/csirx_ut/rtos
csirx_unit_testapp_$(1)_PATH = $(PDK_CSIRX_COMP_PATH)/unit_test/csirx_ut/rtos
export csirx_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export csirx_unit_testapp_$(1)_CORE_DEPENDENCY = yes
export csirx_unit_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export csirx_unit_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csirx_unit_testapp_$(1)_PKG_LIST = csirx_unit_testapp_$(1)
csirx_unit_testapp_$(1)_INCLUDE = $(csirx_unit_testapp_$(1)_PATH)
export csirx_unit_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST) )
export csirx_unit_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsirx_$(SOC)_CORELIST))
export csirx_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csirx_ut_EXAMPLE_LIST += csirx_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csirx_ut_EXAMPLE_LIST += csirx_unit_testapp_$(1)
endif
endif

endef

CSIRX_UNIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call CSIRX_UNIT_TESTAPP_RULE,$(curos)))

$(eval ${CSIRX_UNIT_TESTAPP_MACRO_LIST})


export csirx_ut_LIB_LIST
export csirx_ut_EXAMPLE_LIST

csirx_ut_component_make_include := 1
endif
