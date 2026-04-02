# File: gpadc_component.mk
#       This file is component include makefile of GPADC driver library.
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
ifeq ($(gpadc_component_make_include), )

drvgpadc_RTOS_LIST        = $(DEFAULT_RTOS_LIST)
drvgpadc_SOCLIST          = tpr12 awr294x
drvgpadc_tpr12_CORELIST   = mcu1_0
drvgpadc_awr294x_CORELIST = mcu1_0
drvgpadc_BOARDLIST        = tpr12_evm awr294x_evm

############################
# gpadc package
# List of components included under gpadc lib
# The components included here are built and will be part of gpadc lib
############################
gpadc_LIB_LIST = gpadc
drvgpadc_LIB_LIST = $(gpadc_LIB_LIST)

############################
# gpadc examples
# List of examples under gpadc (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
gpadc_EXAMPLE_LIST =

#
# GPADC Modules
#

# GPADC library
gpadc_COMP_LIST = gpadc
gpadc_RELPATH = ti/drv/gpadc
gpadc_PATH = $(PDK_GPADC_COMP_PATH)
gpadc_LIBNAME = ti.drv.gpadc
gpadc_LIBPATH = $(PDK_GPADC_COMP_PATH)/lib
gpadc_MAKEFILE = -f src/makefile
export gpadc_MAKEFILE
export gpadc_LIBNAME
export gpadc_LIBPATH
gpadc_BOARD_DEPENDENCY = no
gpadc_CORE_DEPENDENCY = no
gpadc_SOC_DEPENDENCY = yes
export gpadc_COMP_LIST
export gpadc_BOARD_DEPENDENCY
export gpadc_CORE_DEPENDENCY
export gpadc_SOC_DEPENDENCY
gpadc_PKG_LIST = gpadc
gpadc_INCLUDE = $(gpadc_PATH)
gpadc_SOCLIST = $(drvgpadc_SOCLIST)
export gpadc_SOCLIST
gpadc_$(SOC)_CORELIST = $(drvgpadc_$(SOC)_CORELIST)
export gpadc_$(SOC)_CORELIST

#
# GPADC Examples
#
# GPADC test app
define gpadc_test_RULE

export gpadc_test_$(1)_COMP_LIST = gpadc_test_$(1)
export gpadc_test_$(1)_RELPATH = ti/drv/gpadc/test
export gpadc_test_$(1)_PATH = $(PDK_GPADC_COMP_PATH)/test
export gpadc_test_$(1)_BOARD_DEPENDENCY = yes
export gpadc_test_$(1)_CORE_DEPENDENCY = no
export gpadc_test_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export gpadc_test_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export gpadc_test_$(1)_PKG_LIST = gpadc_test_$(1)
export gpadc_test_$(1)_INCLUDE = $(gpadc_test_$(1)_PATH)
export gpadc_test_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvgpadc_BOARDLIST))
export gpadc_test_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvgpadc_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
gpadc_EXAMPLE_LIST += gpadc_test_$(1)
else
ifneq ($(wildcard $(PDK_SAFERTOS_COMP_PATH)),)
gpadc_EXAMPLE_LIST += gpadc_test_$(1)
endif
endif
export gpadc_test_$(1)_SBL_APPIMAGEGEN = yes

endef

gpadc_test_MACRO_LIST := $(foreach curos,$(drvgpadc_RTOS_LIST),$(call gpadc_test_RULE,$(curos)))

$(eval ${gpadc_test_MACRO_LIST})

export gpadc_LIB_LIST
export gpadc_EXAMPLE_LIST
export drvgpadc_LIB_LIST = $(gpadc_LIB_LIST)
export drvgpadc_EXAMPLE_LIST = $(gpadc_EXAMPLE_LIST)

GPADC_CFLAGS =
export GPADC_CFLAGS

gpadc_component_make_include := 1
endif
