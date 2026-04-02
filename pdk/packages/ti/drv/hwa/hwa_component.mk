# File: hwa_component.mk
#       This file is component include makefile of HWA driver library.
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
ifeq ($(hwa_component_make_include), )

drvhwa_RTOS_LIST = $(DEFAULT_RTOS_LIST)
drvhwa_SOCLIST          = tpr12 awr294x
drvhwa_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)
drvhwa_awr294x_CORELIST   = $(DEFAULT_awr294x_CORELIST)
drvhwa_BOARDLIST        = tpr12_evm awr294x_evm

############################
# hwa package
# List of components included under hwa lib
# The components included here are built and will be part of hwa lib
############################
hwa_LIB_LIST = hwa

############################
# hwa examples
# List of examples under hwa (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################

hwa_EXAMPLE_LIST =
drvhwa_EXAMPLE_LIST = $(hwa_EXAMPLE_LIST)
#
# HWA Modules
#

# HWA library
hwa_COMP_LIST = hwa
hwa_RELPATH = ti/drv/hwa
hwa_PATH = $(PDK_HWA_COMP_PATH)
hwa_LIBNAME = hwa
hwa_LIBPATH = $(PDK_HWA_COMP_PATH)/lib
hwa_MAKEFILE = -fsrc/makefile
export hwa_MAKEFILE
export hwa_LIBNAME
export hwa_LIBPATH
hwa_BOARD_DEPENDENCY = no
hwa_CORE_DEPENDENCY = no
export hwa_COMP_LIST
export hwa_BOARD_DEPENDENCY
export hwa_CORE_DEPENDENCY
hwa_PKG_LIST = hwa
hwa_INCLUDE = $(hwa_PATH)
hwa_SOCLIST = $(drvhwa_SOCLIST)
export hwa_SOCLIST
hwa_$(SOC)_CORELIST = $(drvhwa_$(SOC)_CORELIST)
export hwa_$(SOC)_CORELIST

#
# HWA Examples
#

# HWA unit test
define hwa_testapp_RULE

export hwa_testapp_$(1)_COMP_LIST = hwa_testapp_$(1)
export hwa_testapp_$(1)_RELPATH = ti/drv/hwa/test
export hwa_testapp_$(1)_PATH = $(PDK_HWA_COMP_PATH)/test
export hwa_testapp_$(1)_BOARD_DEPENDENCY = yes
export hwa_testapp_$(1)_CORE_DEPENDENCY = yes
export hwa_testapp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export hwa_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export hwa_testapp_$(1)_PKG_LIST = hwa_testapp_$(1)
export hwa_testapp_$(1)_INCLUDE = $(hwa_testapp_$(1)_PATH)
export hwa_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvhwa_BOARDLIST))
export hwa_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvhwa_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
hwa_EXAMPLE_LIST += hwa_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
hwa_EXAMPLE_LIST += hwa_testapp_$(1)
endif
endif
export hwa_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

hwa_testapp_MACRO_LIST := $(foreach curos,$(drvhwa_RTOS_LIST),$(call hwa_testapp_RULE,$(curos)))

$(eval ${hwa_testapp_MACRO_LIST})

export hwa_LIB_LIST
export hwa_EXAMPLE_LIST
export drvhwa_LIB_LIST = $(hwa_LIB_LIST)
export drvhwa_EXAMPLE_LIST = $(hwa_EXAMPLE_LIST)

HWA_CFLAGS =
export HWA_CFLAGS

hwa_component_make_include := 1
endif
