# File: edma_component.mk
#       This file is component include makefile of EDMA driver library.
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
ifeq ($(edma_component_make_include), )

drvedma_RTOS_LIST = $(DEFAULT_RTOS_LIST)
drvedma_SOCLIST          = tpr12 awr294x
drvedma_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)
drvedma_awr294x_CORELIST = $(DEFAULT_awr294x_CORELIST)
drvedma_BOARDLIST   = tpr12_evm awr294x_evm

############################
# edma package
# List of components included under edma lib
# The components included here are built and will be part of edma lib
############################
edma_LIB_LIST = edma

############################
# edma examples
# List of examples under edma (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
edma_EXAMPLE_LIST =

#
# EDMA Modules
#

# EDMA library
edma_COMP_LIST = edma
edma_RELPATH = ti/drv/edma
edma_PATH = $(PDK_EDMA_COMP_PATH)
edma_LIBNAME = edma
edma_LIBPATH = $(PDK_EDMA_COMP_PATH)/lib
edma_MAKEFILE = -fsrc/makefile
export edma_MAKEFILE
export edma_LIBNAME
export edma_LIBPATH
edma_BOARD_DEPENDENCY = no
edma_CORE_DEPENDENCY = no
export edma_COMP_LIST
export edma_BOARD_DEPENDENCY
export edma_CORE_DEPENDENCY
edma_PKG_LIST = edma
edma_INCLUDE ?= $(edma_PATH)
edma_SOCLIST = $(drvedma_SOCLIST)
export edma_SOCLIST
edma_$(SOC)_CORELIST = $(drvedma_$(SOC)_CORELIST)
export edma_$(SOC)_CORELIST

#
# EDMA Examples
#

# EDMA baremetal memcpy test app
export edma_baremetal_memcpy_testapp_COMP_LIST = edma_baremetal_memcpy_testapp
edma_baremetal_memcpy_testapp_RELPATH = ti/drv/edma/examples/edma_memcpy_test
edma_baremetal_memcpy_testapp_PATH = $(PDK_EDMA_COMP_PATH)/examples/edma_memcpy_test
export edma_baremetal_memcpy_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export edma_baremetal_memcpy_testapp_BOARD_DEPENDENCY = yes
export edma_baremetal_memcpy_testapp_CORE_DEPENDENCY = yes
export edma_baremetal_memcpy_testapp_XDC_CONFIGURO = yes
edma_baremetal_memcpy_testapp_PKG_LIST = edma_baremetal_memcpy_testapp
edma_baremetal_memcpy_testapp_INCLUDE = $(edma_baremetal_memcpy_testapp_PATH)
export edma_baremetal_memcpy_testapp_BOARDLIST = $(drvedma_BOARDLIST)
export edma_baremetal_memcpy_testapp_$(SOC)_CORELIST = $(drvedma_$(SOC)_CORELIST)
edma_EXAMPLE_LIST += edma_baremetal_memcpy_testapp

# EDMA memcpy Test App
define edma_memcpy_testapp_RULE

export edma_memcpy_testapp_$(1)_COMP_LIST = edma_memcpy_testapp_$(1)
export edma_memcpy_testapp_$(1)_RELPATH = ti/drv/edma/examples/edma_memcpy_test
export edma_memcpy_testapp_$(1)_PATH = $(PDK_EDMA_COMP_PATH)/examples/edma_memcpy_test
export edma_memcpy_testapp_$(1)_BOARD_DEPENDENCY = yes
export edma_memcpy_testapp_$(1)_CORE_DEPENDENCY = no
export edma_memcpy_testapp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export edma_memcpy_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export edma_memcpy_testapp_$(1)_PKG_LIST = edma_memcpy_testapp_$(1)
export edma_memcpy_testapp_$(1)_INCLUDE = $(edma_memcpy_testapp_$(1)_PATH)
export edma_memcpy_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvedma_BOARDLIST))
export edma_memcpy_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvedma_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
edma_EXAMPLE_LIST += edma_memcpy_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
edma_EXAMPLE_LIST += edma_memcpy_testapp_$(1)
endif
endif
export edma_memcpy_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

edma_memcpy_testapp_MACRO_LIST := $(foreach curos,$(drvedma_RTOS_LIST),$(call edma_memcpy_testapp_RULE,$(curos)))

$(eval ${edma_memcpy_testapp_MACRO_LIST})


# EDMA unit test app
define edma_unit_testapp_RULE

export edma_unit_testapp_$(1)_COMP_LIST = edma_unit_testapp_$(1)
export edma_unit_testapp_$(1)_RELPATH = ti/drv/edma/unit_test/edma_ut
export edma_unit_testapp_$(1)_PATH = $(PDK_EDMA_COMP_PATH)/unit_test/edma_ut
export edma_unit_testapp_$(1)_BOARD_DEPENDENCY = yes
export edma_unit_testapp_$(1)_CORE_DEPENDENCY = no
export edma_unit_testapp_$(1)_XDC_CONFIGURO =  $(if $(findstring tirtos,$(1)),yes,no)
export edma_unit_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export edma_unit_testapp_$(1)_PKG_LIST = edma_unit_testapp_$(1)
export edma_unit_testapp_$(1)_INCLUDE = $(edma_unit_testapp_$(1)_PATH)
export edma_unit_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvedma_BOARDLIST))
export edma_unit_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvedma_$(SOC)_CORELIST))
ifneq ($(1),$(filter $(1), safertos))
edma_EXAMPLE_LIST += edma_unit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
edma_EXAMPLE_LIST += edma_unit_testapp_$(1)
endif
endif
export edma_unit_testapp_$(1)_SBL_APPIMAGEGEN = yes

endef

edma_unit_testapp_MACRO_LIST := $(foreach curos,$(drvedma_RTOS_LIST),$(call edma_unit_testapp_RULE,$(curos)))

$(eval ${edma_unit_testapp_MACRO_LIST})

export edma_LIB_LIST
export edma_EXAMPLE_LIST
export drvedma_LIB_LIST = $(edma_LIB_LIST)
export drvedma_EXAMPLE_LIST = $(edma_EXAMPLE_LIST)

EDMA_CFLAGS =
export EDMA_CFLAGS

edma_component_make_include := 1
endif
