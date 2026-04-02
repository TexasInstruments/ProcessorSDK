# File: adcbuf_component.mk
#       This file is component include makefile of adcbuf driver library.
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
ifeq ($(adcbuf_component_make_include), )

drvadcbuf_RTOS_LIST          = $(DEFAULT_RTOS_LIST)
drvadcbuf_SOCLIST            = awr294x
drvadcbuf_awr294x_CORELIST   = mcu1_0 c66xdsp_1
drvadcbuf_BOARDLIST          = awr294x_evm
drvadcbuf_awr294x_BOARDLIST  = awr294x_evm

############################
# adcbuf package
# List of components included under adcbuf lib
# The components included here are built and will be part of adcbuf lib
############################
adcbuf_LIB_LIST = adcbuf

############################
# adcbuf examples
# List of examples under adcbuf (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
adcbuf_EXAMPLE_LIST =

#
# adcbuf Modules
#

# adcbuf library
export adcbuf_COMP_LIST = adcbuf
adcbuf_RELPATH = ti/drv/adcbuf
adcbuf_PATH = $(PDK_ADCBUF_COMP_PATH)
export adcbuf_LIBNAME = adcbuf
export adcbuf_LIBPATH = $(PDK_ADCBUF_COMP_PATH)/lib
export adcbuf_MAKEFILE = -fsrc/makefile
export adcbuf_BOARD_DEPENDENCY = no
export adcbuf_CORE_DEPENDENCY = yes
adcbuf_PKG_LIST = adcbuf
adcbuf_INCLUDE = $(adcbuf_PATH)
export adcbuf_SOCLIST = $(drvadcbuf_SOCLIST)
export adcbuf_$(SOC)_CORELIST = $(drvadcbuf_$(SOC)_CORELIST)

#
# adcbuf Examples
#
define adcbuf_test_RULE

export adcbuf_test_$(1)_COMP_LIST = adcbuf_test_$(1)
export adcbuf_test_$(1)_RELPATH = ti/drv/adcbuf/example
export adcbuf_test_$(1)_PATH = $(PDK_ADCBUF_COMP_PATH)/example
export adcbuf_test_$(1)_BOARD_DEPENDENCY = yes
export adcbuf_test_$(1)_CORE_DEPENDENCY = yes
export adcbuf_test_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export adcbuf_test_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export adcbuf_test_$(1)_PKG_LIST = adcbuf_test_$(1)
export adcbuf_test_$(1)_INCLUDE = $(adcbuf_test_$(1)_PATH)
export adcbuf_test_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvadcbuf_BOARDLIST))
export adcbuf_test_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvadcbuf_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
adcbuf_EXAMPLE_LIST += adcbuf_test_$(1)
else
ifneq ($(wildcard $(PDK_SAFERTOS_COMP_PATH)),)
adcbuf_EXAMPLE_LIST += adcbuf_test_$(1)
endif
endif
export adcbuf_test_$(1)_SBL_APPIMAGEGEN = yes

endef

adcbuf_test_MACRO_LIST := $(foreach curos,$(drvadcbuf_RTOS_LIST),$(call adcbuf_test_RULE,$(curos)))

$(eval ${adcbuf_test_MACRO_LIST})

export adcbuf_LIB_LIST
export adcbuf_EXAMPLE_LIST

ADCBUF_CFLAGS =
export ADCBUF_CFLAGS

adcbuf_component_make_include := 1
endif
