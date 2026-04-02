# File: csirx_component.mk
#       This file is component include make file of CSI RX driver library.
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
ifeq ($(csirx_component_make_include), )

drvcsirx_SOCLIST         = j721e j721s2 j784s4 j742s2
drvcsirx_BOARDLIST       = j721e_sim j721e_evm j721e_qt j721s2_evm j784s4_evm j742s2_evm
drvcsirx_j721e_CORELIST  = mcu2_0
drvcsirx_j721s2_CORELIST = mcu2_0
drvcsirx_j784s4_CORELIST = mcu2_0
drvcsirx_j742s2_CORELIST = mcu2_0
drvcsirx_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

define DRV_CSIRX_RTOS_BOARDLIST_RULE

drvcsirx_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST))

endef

DRV_CSIRX_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call DRV_CSIRX_RTOS_BOARDLIST_RULE,$(curos)))

$(eval ${DRV_CSIRX_RTOS_BOARDLIST_MACRO_LIST})

############################
# CSI RX package
# List of components included under CSI RX lib
# The components included here are built and will be part of CSI RX lib
############################
csirx_LIB_LIST =

############################
# CSI RX examples
# List of examples under CSI RX(+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
csirx_EXAMPLE_LIST =

#
# CSI RX Modules
#

# CSI RX library
csirx_COMP_LIST = csirx
csirx_RELPATH = ti/drv/csirx
csirx_PATH = $(PDK_CSIRX_COMP_PATH)
csirx_LIBNAME = csirx
csirx_LIBPATH = $(PDK_CSIRX_COMP_PATH)/lib
csirx_MAKEFILE = -fsrc/makefile
export csirx_MAKEFILE
export csirx_LIBNAME
export csirx_LIBPATH
csirx_BOARD_DEPENDENCY = no
csirx_CORE_DEPENDENCY = yes
export csirx_COMP_LIST
export csirx_BOARD_DEPENDENCY
export csirx_CORE_DEPENDENCY
csirx_PKG_LIST = csirx
csirx_INCLUDE = $(csirx_PATH)
csirx_SOCLIST = $(drvcsirx_SOCLIST)
export csirx_SOCLIST
csirx_$(SOC)_CORELIST = $(drvcsirx_$(SOC)_CORELIST)
export csirx_$(SOC)_CORELIST
csirx_LIB_LIST += csirx

#
# CSIRX DRV Capture test apps
#
define CSIRX_CAPTURE_TESTAPP_RULE
export csirx_capture_testapp_$(1)_COMP_LIST = csirx_capture_testapp_$(1)
csirx_capture_testapp_$(1)_RELPATH = ti/drv/csirx/examples/csirx_capture_test
csirx_capture_testapp_$(1)_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirx_capture_test
export csirx_capture_testapp_$(1)_BOARD_DEPENDENCY = yes
export csirx_capture_testapp_$(1)_CORE_DEPENDENCY = yes
export csirx_capture_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export csirx_capture_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csirx_capture_testapp_$(1)_PKG_LIST = csirx_capture_testapp_$(1)
csirx_capture_testapp_$(1)_INCLUDE = $(csirx_capture_testapp_$(1)_PATH)
export csirx_capture_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST) )
export csirx_capture_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsirx_$(SOC)_CORELIST))
export csirx_capture_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csirx_EXAMPLE_LIST += csirx_capture_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csirx_EXAMPLE_LIST += csirx_capture_testapp_$(1)
endif
endif
endef
CSIRX_CAPTURE_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call CSIRX_CAPTURE_TESTAPP_RULE,$(curos)))
$(eval ${CSIRX_CAPTURE_TESTAPP_MACRO_LIST})

#
# CSIRX DRV ASF ESM test apps
#
define CSIRX_ASF_ESM_TESTAPP_RULE
export csirx_asf_esm_testapp_$(1)_COMP_LIST = csirx_asf_esm_testapp_$(1)
csirx_asf_esm_testapp_$(1)_RELPATH = ti/drv/csirx/examples/csirx_asf_esm_test
csirx_asf_esm_testapp_$(1)_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirx_asf_esm_test
export csirx_asf_esm_testapp_$(1)_BOARD_DEPENDENCY = yes
export csirx_asf_esm_testapp_$(1)_CORE_DEPENDENCY = yes
export csirx_asf_esm_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export csirx_asf_esm_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csirx_asf_esm_testapp_$(1)_PKG_LIST = csirx_asf_esm_testapp_$(1)
csirx_asf_esm_testapp_$(1)_INCLUDE = $(csirx_asf_esm_testapp_$(1)_PATH)
export csirx_asf_esm_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST) )
export csirx_asf_esm_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsirx_$(SOC)_CORELIST))
export csirx_asf_esm_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csirx_EXAMPLE_LIST += csirx_asf_esm_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csirx_EXAMPLE_LIST += csirx_asf_esm_testapp_$(1)
endif
endif
endef
CSIRX_ASF_ESM_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call CSIRX_ASF_ESM_TESTAPP_RULE,$(curos)))
$(eval ${CSIRX_ASF_ESM_TESTAPP_MACRO_LIST})

#
# CSIRX Recovery test apps
#
define CSIRX_RECOVERY_TESTAPP_RULE
export csirx_recovery_testapp_$(1)_COMP_LIST = csirx_recovery_testapp_$(1)
csirx_recovery_testapp_$(1)_RELPATH = ti/drv/csirx/examples/csirx_recovery_test
csirx_recovery_testapp_$(1)_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirx_recovery_test
export csirx_recovery_testapp_$(1)_BOARD_DEPENDENCY = yes
export csirx_recovery_testapp_$(1)_CORE_DEPENDENCY = yes
export csirx_recovery_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export csirx_recovery_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csirx_recovery_testapp_$(1)_PKG_LIST = csirx_recovery_testapp_$(1)
csirx_recovery_testapp_$(1)_INCLUDE = $(csirx_recovery_testapp_$(1)_PATH)
export csirx_recovery_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST) )
export csirx_recovery_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsirx_$(SOC)_CORELIST))
export csirx_recovery_testapp_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
csirx_EXAMPLE_LIST += csirx_recovery_testapp_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
csirx_EXAMPLE_LIST += csirx_recovery_testapp_$(1)
endif
endif
endef
CSIRX_RECOVERY_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call CSIRX_RECOVERY_TESTAPP_RULE,$(curos)))
$(eval ${CSIRX_RECOVERY_TESTAPP_MACRO_LIST})

#
# CSIRXTX Loopback test apps
#
define CSIRXTX_LOOPBACK_TESTAPP_RULE
export csirxtx_loopback_testapp_$(1)_COMP_LIST = csirxtx_loopback_testapp_$(1)
csirxtx_loopback_testapp_$(1)_RELPATH = ti/drv/csirx/examples/csirxtx_loopback_test
csirxtx_loopback_testapp_$(1)_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirxtx_loopback_test
export csirxtx_loopback_testapp_$(1)_BOARD_DEPENDENCY = yes
export csirxtx_loopback_testapp_$(1)_CORE_DEPENDENCY = yes
export csirxtx_loopback_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
csirxtx_loopback_testapp_$(1)_PKG_LIST = csirxtx_loopback_testapp_$(1)
csirxtx_loopback_testapp_$(1)_INCLUDE = $(csirxtx_loopback_testapp_$(1)_PATH)
export csirxtx_loopback_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvcsirx_BOARDLIST) )
export csirxtx_loopback_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvcsirx_$(SOC)_CORELIST))
export csirxtx_loopback_testapp_$(1)_SBL_APPIMAGEGEN = yes
csirx_EXAMPLE_LIST += csirxtx_loopback_testapp_$(1)
endef
CSIRXTX_LOOPBACK_TESTAPP_MACRO_LIST := $(foreach curos, $(drvcsirx_RTOS_LIST), $(call CSIRXTX_LOOPBACK_TESTAPP_RULE,$(curos)))
$(eval ${CSIRXTX_LOOPBACK_TESTAPP_MACRO_LIST})

#
# CSIRX DRV Capture baremetal test app
#
csirx_baremetal_capture_testapp_COMP_LIST = csirx_baremetal_capture_testapp
csirx_baremetal_capture_testapp_RELPATH = ti/drv/csirx/examples/csirx_capture_test
csirx_baremetal_capture_testapp_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirx_capture_test
csirx_baremetal_capture_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export csirx_baremetal_capture_testapp_MAKEFILE
csirx_baremetal_capture_testapp_BOARD_DEPENDENCY = yes
csirx_baremetal_capture_testapp_CORE_DEPENDENCY = yes
export csirx_baremetal_capture_testapp_COMP_LIST
export csirx_baremetal_capture_testapp_BOARD_DEPENDENCY
export csirx_baremetal_capture_testapp_CORE_DEPENDENCY
csirx_baremetal_capture_testapp_PKG_LIST = csirx_baremetal_capture_testapp
csirx_baremetal_capture_testapp_INCLUDE = $(csirx_baremetal_capture_testapp_PATH)
csirx_baremetal_capture_testapp_BOARDLIST = $(drvcsirx_BOARDLIST)
export csirx_baremetal_capture_testapp_BOARDLIST
csirx_baremetal_capture_testapp_$(SOC)_CORELIST = $(drvcsirx_$(SOC)_CORELIST)
export csirx_baremetal_capture_testapp_$(SOC)_CORELIST
csirx_EXAMPLE_LIST += csirx_baremetal_capture_testapp
csirx_baremetal_capture_testapp_SBL_APPIMAGEGEN = yes
export csirx_baremetal_capture_testapp_SBL_APPIMAGEGEN

#
# CSIRX DRV ASF ESM baremetal test app
#
csirx_baremetal_asf_esm_testapp_COMP_LIST = csirx_baremetal_asf_esm_testapp
csirx_baremetal_asf_esm_testapp_RELPATH = ti/drv/csirx/examples/csirx_asf_esm_test
csirx_baremetal_asf_esm_testapp_PATH = $(PDK_CSIRX_COMP_PATH)/examples/csirx_asf_esm_test
csirx_baremetal_asf_esm_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export csirx_baremetal_asf_esm_testapp_MAKEFILE
csirx_baremetal_asf_esm_testapp_BOARD_DEPENDENCY = yes
csirx_baremetal_asf_esm_testapp_CORE_DEPENDENCY = yes
export csirx_baremetal_asf_esm_testapp_COMP_LIST
export csirx_baremetal_asf_esm_testapp_BOARD_DEPENDENCY
export csirx_baremetal_asf_esm_testapp_CORE_DEPENDENCY
csirx_baremetal_asf_esm_testapp_PKG_LIST = csirx_baremetal_asf_esm_testapp
csirx_baremetal_asf_esm_testapp_INCLUDE = $(csirx_baremetal_asf_esm_testapp_PATH)
csirx_baremetal_asf_esm_testapp_BOARDLIST = $(drvcsirx_BOARDLIST)
export csirx_baremetal_asf_esm_testapp_BOARDLIST
csirx_baremetal_asf_esm_testapp_$(SOC)_CORELIST = $(drvcsirx_$(SOC)_CORELIST)
export csirx_baremetal_asf_esm_testapp_$(SOC)_CORELIST
csirx_EXAMPLE_LIST += csirx_baremetal_asf_esm_testapp
csirx_baremetal_asf_esm_testapp_SBL_APPIMAGEGEN = yes
export csirx_baremetal_asf_esm_testapp_SBL_APPIMAGEGEN

-include $(PDK_CSIRX_COMP_PATH)/unit_test/csirx_ut_component.mk
ifneq ($(csirx_ut_LIB_LIST),)
  csirx_LIB_LIST += $(csirx_ut_LIB_LIST)
endif
ifneq ($(csirx_ut_EXAMPLE_LIST),)
  csirx_EXAMPLE_LIST += $(csirx_ut_EXAMPLE_LIST)
endif

export csirx_LIB_LIST
export csirx_EXAMPLE_LIST
export drvcsirx_LIB_LIST = $(csirx_LIB_LIST)
export drvcsirx_EXAMPLE_LIST = $(csirx_EXAMPLE_LIST)

CSIRX_CFLAGS =

# Enable asserts and prints
CSIRX_CFLAGS = $(FVID2_CFLAGS)
CSIRX_CFLAGS += -DCSIRX_CFG_ASSERT_ENABLE
#CSIRX_CFLAGS += -DCSIRX_CFG_USE_STD_ASSERT
CSIRX_CFLAGS += -DCSIRX_CFG_PRINT_ENABLE

export CSIRX_CFLAGS

csirx_component_make_include := 1
endif
