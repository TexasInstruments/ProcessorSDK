# File: safety_checkers_component.mk
#       This file is component include for safety checkers.
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
ifeq ($(safety_checkers_component_make_include), )

safety_checkers_SOCLIST               = j7200 j721e j721s2 j784s4 j742s2
safety_checkers_BOARDLIST             = j7200_evm j721e_evm j721s2_evm j784s4_evm j742s2_evm
safety_checkers_csirx_SOCLIST         = j784s4 j721e j721s2 j742s2
safety_checkers_csirx_BOARDLIST       = j784s4_evm j721e_evm j721s2_evm j742s2_evm
safety_checkers_j7200_CORELIST        = mcu1_0 mcu1_1 mcu2_0
safety_checkers_j721e_CORELIST        = mcu1_0 mcu1_1 mcu2_0
safety_checkers_j721s2_CORELIST       = mcu1_0 mcu1_1 mcu2_0
safety_checkers_j784s4_CORELIST       = mcu1_0 mcu1_1 mcu2_0
safety_checkers_j742s2_CORELIST       = mcu1_0 mcu1_1 mcu2_0
safety_checkers_csirx_j784s4_CORELIST = mcu2_0
safety_checkers_csirx_j721s2_CORELIST = mcu2_0
safety_checkers_csirx_j721e_CORELIST  = mcu2_0
safety_checkers_csirx_j742s2_CORELIST = mcu2_0
safety_checkers_RTOS_LIST             = $(DEFAULT_RTOS_LIST)

############################
# Safety checkers package
# List of components included under Safety checkers lib
# The components included here are built and will be part of Safety checkers lib
############################
safety_checkers_LIB_LIST =

############################
# Safety checkers examples
# List of examples under Safety checkers (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
safety_checkers_EXAMPLE_LIST =

# safety checker library
safety_checkers_COMP_LIST = safety_checkers
safety_checkers_RELPATH = ti/safety_checkers/src
safety_checkers_PATH = $(SAFETY_CHECKERS_COMP_PATH)/src
safety_checkers_LIBNAME = safety_checkers
safety_checkers_LIBPATH = $(SAFETY_CHECKERS_COMP_PATH)/lib
safety_checkers_MAKEFILE = -fmakefile
export safety_checkers_MAKEFILE
export safety_checkers_LIBNAME
export safety_checkers_LIBPATH
safety_checkers_BOARD_DEPENDENCY = no
safety_checkers_CORE_DEPENDENCY = yes
export safety_checkers_COMP_LIST
export safety_checkers_BOARD_DEPENDENCY
export safety_checkers_CORE_DEPENDENCY
safety_checkers_PKG_LIST = safety_checkers
safety_checkers_INCLUDE = $(safety_checkers_PATH)
export safety_checkers_SOCLIST
export safety_checkers_$(SOC)_CORELIST
safety_checkers_LIB_LIST += safety_checkers

# PM safety checkers app
define PM_CHECKERS_APP_RULE
export pm_checkers_app_$(1)_COMP_LIST = pm_checkers_app_$(1)
pm_checkers_app_$(1)_RELPATH = ti/safety_checkers/examples/pm_checkers_app
pm_checkers_app_$(1)_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/pm_checkers_app
export pm_checkers_app_$(1)_BOARD_DEPENDENCY = yes
export pm_checkers_app_$(1)_CORE_DEPENDENCY = yes
export pm_checkers_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
pm_checkers_app_$(1)_PKG_LIST = pm_checkers_app_$(1)
pm_checkers_app_$(1)_INCLUDE = $(pm_checkers_app_$(1)_PATH)
export pm_checkers_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(safety_checkers_BOARDLIST) )
export pm_checkers_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(safety_checkers_$(SOC)_CORELIST))
export pm_checkers_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
safety_checkers_EXAMPLE_LIST += pm_checkers_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safety_checkers_EXAMPLE_LIST += pm_checkers_app_$(1)
endif
endif
endef
PM_CHECKERS_APP_MACRO_LIST := $(foreach curos, $(safety_checkers_RTOS_LIST), $(call PM_CHECKERS_APP_RULE,$(curos)))
$(eval ${PM_CHECKERS_APP_MACRO_LIST})

# PM safety checkers baremetal app
pm_checkers_app_baremetal_COMP_LIST = pm_checkers_app_baremetal
pm_checkers_app_baremetal_RELPATH = ti/safety_checkers/examples/pm_checkers_app
pm_checkers_app_baremetal_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/pm_checkers_app
pm_checkers_app_baremetal_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export pm_checkers_app_baremetal_MAKEFILE
pm_checkers_app_baremetal_BOARD_DEPENDENCY = yes
pm_checkers_app_baremetal_CORE_DEPENDENCY = yes
export pm_checkers_app_baremetal_COMP_LIST
export pm_checkers_app_baremetal_BOARD_DEPENDENCY
export pm_checkers_app_baremetal_CORE_DEPENDENCY
pm_checkers_app_baremetal_PKG_LIST = pm_checkers_app_baremetal
pm_checkers_app_baremetal_INCLUDE = $(pm_checkers_app_baremetal_PATH)
pm_checkers_app_baremetal_BOARDLIST = $(safety_checkers_BOARDLIST)
export pm_checkers_app_baremetal_BOARDLIST
pm_checkers_app_baremetal_$(SOC)_CORELIST = $(safety_checkers_$(SOC)_CORELIST)
export pm_checkers_app_baremetal_$(SOC)_CORELIST
safety_checkers_EXAMPLE_LIST += pm_checkers_app_baremetal
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
pm_checkers_app_baremetal_SBL_APPIMAGEGEN = yes
export pm_checkers_app_baremetal_SBL_APPIMAGEGEN
endif

# PM safety checkers warm reset app
define PM_CHECKERS_WARM_RESET_APP_RULE
export pm_checkers_warm_reset_app_$(1)_COMP_LIST = pm_checkers_warm_reset_app_$(1)
pm_checkers_warm_reset_app_$(1)_RELPATH = ti/safety_checkers/examples/pm_checkers_warm_reset
pm_checkers_warm_reset_app_$(1)_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/pm_checkers_warm_reset
export pm_checkers_warm_reset_app_$(1)_BOARD_DEPENDENCY = yes
export pm_checkers_warm_reset_app_$(1)_CORE_DEPENDENCY = yes
export pm_checkers_warm_reset_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
pm_checkers_warm_reset_app_$(1)_PKG_LIST = pm_checkers_warm_reset_app_$(1)
pm_checkers_warm_reset_app_$(1)_INCLUDE = $(pm_checkers_warm_reset_app_$(1)_PATH)
export pm_checkers_warm_reset_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(safety_checkers_BOARDLIST) )
export pm_checkers_warm_reset_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(safety_checkers_$(SOC)_CORELIST))
export pm_checkers_warm_reset_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
safety_checkers_EXAMPLE_LIST += pm_checkers_warm_reset_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safety_checkers_EXAMPLE_LIST += pm_checkers_warm_reset_app_$(1)
endif
endif
endef
PM_CHECKERS_WARM_RESET_APP_MACRO_LIST := $(foreach curos, $(safety_checkers_RTOS_LIST), $(call PM_CHECKERS_WARM_RESET_APP_RULE,$(curos)))
$(eval ${PM_CHECKERS_WARM_RESET_APP_MACRO_LIST})

# RM safety checkers app
define RM_CHECKERS_APP_RULE
export rm_checkers_app_$(1)_COMP_LIST = rm_checkers_app_$(1)
rm_checkers_app_$(1)_RELPATH = ti/safety_checkers/examples/rm_checkers_app
rm_checkers_app_$(1)_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/rm_checkers_app
export rm_checkers_app_$(1)_BOARD_DEPENDENCY = yes
export rm_checkers_app_$(1)_CORE_DEPENDENCY = yes
export rm_checkers_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
rm_checkers_app_$(1)_PKG_LIST = rm_checkers_app_$(1)
rm_checkers_app_$(1)_INCLUDE = $(rm_checkers_app_$(1)_PATH)
export rm_checkers_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(safety_checkers_BOARDLIST) )
export rm_checkers_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(safety_checkers_$(SOC)_CORELIST))
export rm_checkers_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
safety_checkers_EXAMPLE_LIST += rm_checkers_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safety_checkers_EXAMPLE_LIST += rm_checkers_app_$(1)
endif
endif
endef
RM_CHECKERS_APP_MACRO_LIST := $(foreach curos, $(safety_checkers_RTOS_LIST), $(call RM_CHECKERS_APP_RULE,$(curos)))
$(eval ${RM_CHECKERS_APP_MACRO_LIST})

# RM safety checkers baremetal app
rm_checkers_app_baremetal_COMP_LIST = rm_checkers_app_baremetal
rm_checkers_app_baremetal_RELPATH = ti/safety_checkers/examples/rm_checkers_app
rm_checkers_app_baremetal_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/rm_checkers_app
rm_checkers_app_baremetal_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export rm_checkers_app_baremetal_MAKEFILE
rm_checkers_app_baremetal_BOARD_DEPENDENCY = yes
rm_checkers_app_baremetal_CORE_DEPENDENCY = yes
export rm_checkers_app_baremetal_COMP_LIST
export rm_checkers_app_baremetal_BOARD_DEPENDENCY
export rm_checkers_app_baremetal_CORE_DEPENDENCY
rm_checkers_app_baremetal_PKG_LIST = rm_checkers_app_baremetal
rm_checkers_app_baremetal_INCLUDE = $(rm_checkers_app_baremetal_PATH)
rm_checkers_app_baremetal_BOARDLIST = $(safety_checkers_BOARDLIST)
export rm_checkers_app_baremetal_BOARDLIST
rm_checkers_app_baremetal_$(SOC)_CORELIST = $(safety_checkers_$(SOC)_CORELIST)
export rm_checkers_app_baremetal_$(SOC)_CORELIST
safety_checkers_EXAMPLE_LIST += rm_checkers_app_baremetal
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
rm_checkers_app_baremetal_SBL_APPIMAGEGEN = yes
export rm_checkers_app_baremetal_SBL_APPIMAGEGEN
endif

# TIFS safety checkers app
define TIFS_CHECKERS_APP_RULE
export tifs_checkers_app_$(1)_COMP_LIST = tifs_checkers_app_$(1)
tifs_checkers_app_$(1)_RELPATH = ti/safety_checkers/examples/tifs_checkers_app
tifs_checkers_app_$(1)_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/tifs_checkers_app
export tifs_checkers_app_$(1)_BOARD_DEPENDENCY = yes
export tifs_checkers_app_$(1)_CORE_DEPENDENCY = yes
export tifs_checkers_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
tifs_checkers_app_$(1)_PKG_LIST = tifs_checkers_app_$(1)
tifs_checkers_app_$(1)_INCLUDE = $(tifs_checkers_app_$(1)_PATH)
export tifs_checkers_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(safety_checkers_BOARDLIST) )
export tifs_checkers_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(safety_checkers_$(SOC)_CORELIST))
export tifs_checkers_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
safety_checkers_EXAMPLE_LIST += tifs_checkers_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safety_checkers_EXAMPLE_LIST += tifs_checkers_app_$(1)
endif
endif
endef
TIFS_CHECKERS_APP_MACRO_LIST := $(foreach curos, $(safety_checkers_RTOS_LIST), $(call TIFS_CHECKERS_APP_RULE,$(curos)))
$(eval ${TIFS_CHECKERS_APP_MACRO_LIST})

# CSIRX safety checkers app
define CSIRX_CHECKERS_APP_RULE
export csirx_checkers_app_$(1)_COMP_LIST = csirx_checkers_app_$(1)
csirx_checkers_app_$(1)_RELPATH = ti/safety_checkers/examples/csirx_checkers_app
csirx_checkers_app_$(1)_PATH = $(SAFETY_CHECKERS_COMP_PATH)/examples/csirx_checkers_app
export csirx_checkers_app_$(1)_BOARD_DEPENDENCY = yes
export csirx_checkers_app_$(1)_CORE_DEPENDENCY = yes
export csirx_checkers_app_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
csirx_checkers_app_$(1)_PKG_LIST = csirx_checkers_app_$(1)
csirx_checkers_app_$(1)_INCLUDE = $(csirx_checkers_app_$(1)_PATH)
export csirx_checkers_app_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(safety_checkers_csirx_BOARDLIST) )
export csirx_checkers_app_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(safety_checkers_csirx_$(SOC)_CORELIST))
export csirx_checkers_app_$(1)_SBL_APPIMAGEGEN = yes
ifneq ($(1),$(filter $(1), safertos))
safety_checkers_EXAMPLE_LIST += csirx_checkers_app_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
safety_checkers_EXAMPLE_LIST += csirx_checkers_app_$(1)
endif
endif
endef
CSIRX_CHECKERS_APP_MACRO_LIST := $(foreach curos, $(safety_checkers_RTOS_LIST), $(call CSIRX_CHECKERS_APP_RULE,$(curos)))
$(eval ${CSIRX_CHECKERS_APP_MACRO_LIST})

export safety_checkers_LIB_LIST
export safety_checkers_EXAMPLE_LIST

safety_checkers_component_make_include := 1
endif
