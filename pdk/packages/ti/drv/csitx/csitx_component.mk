# File: csitx_component.mk
#       This file is component include make file of CSI TX driver library.
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
ifeq ($(csitx_component_make_include), )

drvcsitx_SOCLIST         = j721e j721s2 j784s4 j742s2
drvcsitx_BOARDLIST       = j721e_evm j721s2_evm j784s4_evm j742s2_evm
drvcsitx_j721e_CORELIST  = mcu2_0
drvcsitx_j721s2_CORELIST = mcu2_0
drvcsitx_j784s4_CORELIST = mcu2_0
drvcsitx_j742s2_CORELIST = mcu2_0
drvcsitx_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

define DRV_CSITX_RTOS_BOARDLIST_RULE

drvcsitx_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsitx_BOARDLIST))

endef

DRV_CSITX_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvcsitx_RTOS_LIST), $(call DRV_CSITX_RTOS_BOARDLIST_RULE,$(curos)))

$(eval ${DRV_CSITX_RTOS_BOARDLIST_MACRO_LIST})
############################
# CSI TX package
# List of components included under CSI TX lib
# The components included here are built and will be part of CSI TX lib
############################
csitx_LIB_LIST =

############################
# CSI TX examples
# List of examples under CSI TX(+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
csitx_EXAMPLE_LIST =

#
# CSI TX Modules
#

# CSI TX library
csitx_COMP_LIST = csitx
csitx_RELPATH = ti/drv/csitx
csitx_PATH = $(PDK_CSITX_COMP_PATH)
csitx_LIBNAME = csitx
csitx_LIBPATH = $(PDK_CSITX_COMP_PATH)/lib
csitx_MAKEFILE = -fsrc/makefile
export csitx_MAKEFILE
export csitx_LIBNAME
export csitx_LIBPATH
csitx_BOARD_DEPENDENCY = no
csitx_CORE_DEPENDENCY = yes
export csitx_COMP_LIST
export csitx_BOARD_DEPENDENCY
export csitx_CORE_DEPENDENCY
csitx_PKG_LIST = csitx
csitx_INCLUDE = $(csitx_PATH)
csitx_SOCLIST = $(drvcsitx_SOCLIST)
export csitx_SOCLIST
csitx_$(SOC)_CORELIST = $(drvcsitx_$(SOC)_CORELIST)
export csitx_$(SOC)_CORELIST
csitx_LIB_LIST += csitx

#
# CSITX DRV Transmit test apps
#
define CSITX_TRANSMIT_TESTAPP_RULE

export csitx_transmit_testapp_$(1)_COMP_LIST = csitx_transmit_testapp_$(1)
csitx_transmit_testapp_$(1)_RELPATH = ti/drv/csitx/examples/csitx_transmit_test
csitx_transmit_testapp_$(1)_PATH = $(PDK_CSITX_COMP_PATH)/examples/csitx_transmit_test
export csitx_transmit_testapp_$(1)_BOARD_DEPENDENCY = yes
export csitx_transmit_testapp_$(1)_CORE_DEPENDENCY = yes
export csitx_transmit_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export csitx_transmit_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csitx_transmit_testapp_$(1)_PKG_LIST = csitx_transmit_testapp_$(1)
csitx_transmit_testapp_$(1)_INCLUDE = $(csitx_transmit_testapp_$(1)_PATH)
export csitx_transmit_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsitx_BOARDLIST) )
export csitx_transmit_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsitx_$(SOC)_CORELIST))
export csitx_transmit_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csitx_EXAMPLE_LIST += csitx_transmit_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csitx_EXAMPLE_LIST += csitx_transmit_testapp_$(1)
endif
endif

endef

CSITX_TRANSMIT_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsitx_RTOS_LIST) safertos, $(call CSITX_TRANSMIT_TESTAPP_RULE,$(curos)))

$(eval ${CSITX_TRANSMIT_TESTAPP_MACRO_LIST})


export csitx_LIB_LIST
export csitx_EXAMPLE_LIST
export drvcsitx_LIB_LIST = $(csitx_LIB_LIST)
export drvcsitx_EXAMPLE_LIST = $(csitx_EXAMPLE_LIST)

CSITX_CFLAGS =

# Enable asserts and prints
CSITX_CFLAGS = $(FVID2_CFLAGS)
CSITX_CFLAGS += -DCSITX_CFG_ASSERT_ENABLE
#CSITX_CFLAGS += -DCSITX_CFG_USE_STD_ASSERT
CSITX_CFLAGS += -DCSITX_CFG_PRINT_ENABLE

export CSITX_CFLAGS

csitx_component_make_include := 1
endif
