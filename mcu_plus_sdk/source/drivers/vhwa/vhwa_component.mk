# File: vhwa_component.mk
#       This file is component include make file of VHWA driver library.
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
ifeq ($(vhwa_component_make_include), )

vhwa_default_SOCLIST         = j721e j721s2 j784s4 j742s2
vhwa_default_BOARDLIST       = j721e_evm j721s2_evm j784s4_evm j742s2_evm

export vhwa_default_j721e_CORELIST  = mcu2_0 mcu2_1
export vhwa_vpac_j721e_CORELIST     = mcu2_0
export vhwa_dmpac_j721e_CORELIST    = mcu2_1
export vhwa_default_j721s2_CORELIST = mcu2_0 mcu2_1
export vhwa_vpac_j721s2_CORELIST    = mcu2_0
export vhwa_dmpac_j721s2_CORELIST   = mcu2_1
export vhwa_default_j784s4_CORELIST = mcu2_0 mcu2_1 mcu4_0
export vhwa_vpac_j784s4_CORELIST    = mcu2_0 mcu4_0
export vhwa_dmpac_j784s4_CORELIST   = mcu2_1
export vhwa_default_j742s2_CORELIST = mcu2_0 mcu2_1 mcu4_0
export vhwa_vpac_j742s2_CORELIST    = mcu2_0 mcu4_0
export vhwa_dmpac_j742s2_CORELIST   = mcu2_1
vhwa_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

define DRV_VHWA_RTOS_BOARDLIST_RULE

vhwa_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(vhwa_default_BOARDLIST))

endef

DRV_VHWA_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST), $(call DRV_VHWA_RTOS_BOARDLIST_RULE,$(curos)))

$(eval ${DRV_VHWA_RTOS_BOARDLIST_MACRO_LIST})

############################
# vhwa package
# List of components included under vhwa lib
# The components included here are built and will be part of vhwa lib
############################
vhwa_LIB_LIST =

############################
# vhwa examples
# List of examples under vhwa (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
vhwa_EXAMPLE_LIST =

#
# VHWA Modules
#

# VHWA library
vhwa_COMP_LIST = vhwa
vhwa_RELPATH = ti/drv/vhwa
vhwa_PATH = $(VHWA_INSTALL_PATH)/vhwa
vhwa_LIBNAME = vhwa
vhwa_LIBPATH = $(VHWA_INSTALL_PATH)/vhwa/lib
vhwa_MAKEFILE = -fsrc/makefile
export vhwa_MAKEFILE
export vhwa_LIBNAME
export vhwa_LIBPATH
vhwa_BOARD_DEPENDENCY = no
ifeq ($(BOARD),$(filter $(BOARD), j721e_sim j721e_qt))
  vhwa_BOARD_DEPENDENCY = yes
endif
vhwa_CORE_DEPENDENCY = yes
export vhwa_COMP_LIST
export vhwa_BOARD_DEPENDENCY
export vhwa_CORE_DEPENDENCY
vhwa_PKG_LIST = vhwa
vhwa_INCLUDE = $(vhwa_PATH)
vhwa_SOCLIST = $(vhwa_default_SOCLIST)
export vhwa_SOCLIST
vhwa_$(SOC)_CORELIST = $(vhwa_default_$(SOC)_CORELIST)
export vhwa_$(SOC)_CORELIST
vhwa_LIB_LIST += vhwa

# VHWA MSC baremetal test app
vhwa_msc_baremetal_testapp_COMP_LIST = vhwa_msc_baremetal_testapp
vhwa_msc_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_msc_test
vhwa_msc_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_msc_test
vhwa_msc_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_msc_baremetal_testapp_MAKEFILE
vhwa_msc_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_msc_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_msc_baremetal_testapp_COMP_LIST
export vhwa_msc_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_msc_baremetal_testapp_CORE_DEPENDENCY
vhwa_msc_baremetal_testapp_PKG_LIST = vhwa_msc_baremetal_testapp
vhwa_msc_baremetal_testapp_INCLUDE = $(vhwa_msc_baremetal_testapp_PATH)
vhwa_msc_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_msc_baremetal_testapp_BOARDLIST
vhwa_msc_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_vpac_$(SOC)_CORELIST)
export vhwa_msc_baremetal_testapp_$(SOC)_CORELIST
export vhwa_msc_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_msc_baremetal_testapp

export vhwa_msc_testapp_freertos_COMP_LIST = vhwa_msc_testapp_freertos
vhwa_msc_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_msc_test
vhwa_msc_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_msc_test
export vhwa_msc_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_msc_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_msc_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_msc_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_msc_testapp_freertos_PKG_LIST = vhwa_msc_testapp_freertos
vhwa_msc_testapp_freertos_INCLUDE = $(vhwa_msc_testapp_freertos_PATH)
export vhwa_msc_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_msc_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_msc_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_msc_testapp_freertos

ifeq ($(VHWA_NEGATIVE_TEST), true)
export vhwa_msc_testapp_freertos_COMP_LIST = vhwa_msc_negative_testapp_freertos
vhwa_msc_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_msc_test
vhwa_msc_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_msc_test
export vhwa_msc_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_msc_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_msc_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_msc_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_msc_negative_testapp_freertos_PKG_LIST = vhwa_msc_negative_testapp_freertos
vhwa_msc_negative_testapp_freertos_INCLUDE = $(vhwa_msc_negative_testapp_freertos_PATH)
export vhwa_msc_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_msc_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_msc_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_msc_negative_testapp_freertos
endif  

export vhwa_msc_testapp_safertos_COMP_LIST = vhwa_msc_testapp_safertos
vhwa_msc_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_msc_test
vhwa_msc_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_msc_test
export vhwa_msc_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_msc_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_msc_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_msc_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_msc_testapp_safertos_PKG_LIST = vhwa_msc_testapp_safertos
vhwa_msc_testapp_safertos_INCLUDE = $(vhwa_msc_testapp_safertos_PATH)
export vhwa_msc_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_msc_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_msc_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_msc_testapp_safertos


VHWA_MSC_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_MSC_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_MSC_TESTAPP_MACRO_LIST})

# VHWA LDC baremetal test app
vhwa_ldc_baremetal_testapp_COMP_LIST = vhwa_ldc_baremetal_testapp
vhwa_ldc_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_ldc_test
vhwa_ldc_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_ldc_test
vhwa_ldc_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_ldc_baremetal_testapp_MAKEFILE
vhwa_ldc_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_ldc_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_ldc_baremetal_testapp_COMP_LIST
export vhwa_ldc_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_ldc_baremetal_testapp_CORE_DEPENDENCY
vhwa_ldc_baremetal_testapp_PKG_LIST = vhwa_ldc_baremetal_testapp
vhwa_ldc_baremetal_testapp_INCLUDE = $(vhwa_ldc_baremetal_testapp_PATH)
vhwa_ldc_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_ldc_baremetal_testapp_BOARDLIST
vhwa_ldc_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_vpac_$(SOC)_CORELIST)
export vhwa_ldc_baremetal_testapp_$(SOC)_CORELIST
export vhwa_ldc_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_ldc_baremetal_testapp

export vhwa_ldc_testapp_freertos_COMP_LIST = vhwa_ldc_testapp_freertos
vhwa_ldc_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_ldc_test
vhwa_ldc_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_ldc_test
export vhwa_ldc_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_ldc_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_ldc_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_ldc_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_ldc_testapp_freertos_PKG_LIST = vhwa_ldc_testapp_freertos
vhwa_ldc_testapp_freertos_INCLUDE = $(vhwa_ldc_testapp_freertos_PATH)
export vhwa_ldc_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_ldc_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_ldc_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_ldc_testapp_freertos

export vhwa_ldc_testapp_safertos_COMP_LIST = vhwa_ldc_testapp_safertos
vhwa_ldc_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_ldc_test
vhwa_ldc_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_ldc_test
export vhwa_ldc_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_ldc_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_ldc_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_ldc_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_ldc_testapp_safertos_PKG_LIST = vhwa_ldc_testapp_safertos
vhwa_ldc_testapp_safertos_INCLUDE = $(vhwa_ldc_testapp_safertos_PATH)
export vhwa_ldc_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_ldc_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_ldc_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_ldc_testapp_safertos

VHWA_LDC_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_LDC_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_LDC_TESTAPP_MACRO_LIST})

# VHWA DOF baremetal test app
vhwa_dof_baremetal_testapp_COMP_LIST = vhwa_dof_baremetal_testapp
vhwa_dof_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_dof_test
vhwa_dof_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_dof_test
vhwa_dof_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_dof_baremetal_testapp_MAKEFILE
vhwa_dof_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_dof_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_dof_baremetal_testapp_COMP_LIST
export vhwa_dof_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_dof_baremetal_testapp_CORE_DEPENDENCY
vhwa_dof_baremetal_testapp_PKG_LIST = vhwa_dof_baremetal_testapp
vhwa_dof_baremetal_testapp_INCLUDE = $(vhwa_dof_baremetal_testapp_PATH)
vhwa_dof_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_dof_baremetal_testapp_BOARDLIST
vhwa_dof_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_dmpac_$(SOC)_CORELIST)
export vhwa_dof_baremetal_testapp_$(SOC)_CORELIST
export vhwa_dof_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_dof_baremetal_testapp


export vhwa_dof_testapp_freertos_COMP_LIST = vhwa_dof_testapp_freertos
vhwa_dof_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_dof_test
vhwa_dof_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_dof_test
export vhwa_dof_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_dof_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_dof_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_dof_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_dof_testapp_freertos_PKG_LIST = vhwa_dof_testapp_freertos
vhwa_dof_testapp_freertos_INCLUDE = $(vhwa_dof_testapp_freertos_PATH)
export vhwa_dof_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_dof_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_dmpac_$(SOC)_CORELIST))
export vhwa_dof_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_dof_testapp_freertos

ifeq ($(VHWA_NEGATIVE_TEST), true)
export vhwa_dof_negative_testapp_freertos_COMP_LIST = vhwa_dof_negative_testapp_freertos
vhwa_dof_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_dof_test
vhwa_dof_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_dof_test
export vhwa_dof_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_dof_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_dof_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_dof_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_dof_negative_testapp_freertos_PKG_LIST = vhwa_dof_negative_testapp_freertos
vhwa_dof_negative_testapp_freertos_INCLUDE = $(vhwa_dof_negative_testapp_freertos_PATH)
export vhwa_dof_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_dof_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_dof_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_dof_negative_testapp_freertos
endif

ifeq ($(VHWA_NEGATIVE_FOCO_TEST), true)
export vhwa_dof_negative_testapp_freertos_COMP_LIST = vhwa_dof_negative_testapp_freertos
vhwa_dof_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_dof_test
vhwa_dof_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_dof_test
export vhwa_dof_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_dof_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_dof_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_dof_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_dof_negative_testapp_freertos_PKG_LIST = vhwa_dof_negative_testapp_freertos
vhwa_dof_negative_testapp_freertos_INCLUDE = $(vhwa_dof_negative_testapp_freertos_PATH)
export vhwa_dof_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_dof_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_dof_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_dof_negative_testapp_freertos
endif

export vhwa_dof_testapp_safertos_COMP_LIST = vhwa_dof_testapp_safertos
vhwa_dof_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_dof_test
vhwa_dof_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_dof_test
export vhwa_dof_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_dof_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_dof_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_dof_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_dof_testapp_safertos_PKG_LIST = vhwa_dof_testapp_safertos
vhwa_dof_testapp_safertos_INCLUDE = $(vhwa_dof_testapp_safertos_PATH)
export vhwa_dof_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_dof_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_dmpac_$(SOC)_CORELIST))
export vhwa_dof_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_dof_testapp_safertos


VHWA_DOF_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_DOF_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_DOF_TESTAPP_MACRO_LIST})

# VHWA VISS baremetal test app
vhwa_viss_baremetal_testapp_COMP_LIST = vhwa_viss_baremetal_testapp
vhwa_viss_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_viss_test
vhwa_viss_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_viss_test
vhwa_viss_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_viss_baremetal_testapp_MAKEFILE
vhwa_viss_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_viss_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_viss_baremetal_testapp_COMP_LIST
export vhwa_viss_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_viss_baremetal_testapp_CORE_DEPENDENCY
vhwa_viss_baremetal_testapp_PKG_LIST = vhwa_viss_baremetal_testapp
vhwa_viss_baremetal_testapp_INCLUDE = $(vhwa_viss_baremetal_testapp_PATH)
vhwa_viss_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_viss_baremetal_testapp_BOARDLIST
vhwa_viss_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_vpac_$(SOC)_CORELIST)
export vhwa_viss_baremetal_testapp_$(SOC)_CORELIST
export vhwa_viss_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_viss_baremetal_testapp

export vhwa_viss_testapp_freertos_COMP_LIST = vhwa_viss_testapp_freertos
vhwa_viss_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_viss_test
vhwa_viss_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_viss_test
export vhwa_viss_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_viss_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_viss_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_viss_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_viss_testapp_freertos_PKG_LIST = vhwa_viss_testapp_freertos
vhwa_viss_testapp_freertos_INCLUDE = $(vhwa_viss_testapp_freertos_PATH)
export vhwa_viss_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_viss_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_viss_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_viss_testapp_freertos

ifeq ($(VHWA_NEGATIVE_TEST), true)
export vhwa_viss_negative_testapp_freertos_COMP_LIST = vhwa_viss_negative_testapp_freertos
vhwa_viss_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_viss_test
vhwa_viss_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_viss_test
export vhwa_viss_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_viss_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_viss_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_viss_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_viss_negative_testapp_freertos_PKG_LIST = vhwa_viss_negative_testapp_freertos
vhwa_viss_negative_testapp_freertos_INCLUDE = $(vhwa_viss_negative_testapp_freertos_PATH)
export vhwa_viss_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_viss_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_viss_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_viss_negative_testapp_freertos
endif

export vhwa_viss_testapp_safertos_COMP_LIST = vhwa_viss_testapp_safertos
vhwa_viss_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_viss_test
vhwa_viss_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_viss_test
export vhwa_viss_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_viss_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_viss_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_viss_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_viss_testapp_safertos_PKG_LIST = vhwa_viss_testapp_safertos
vhwa_viss_testapp_safertos_INCLUDE = $(vhwa_viss_testapp_safertos_PATH)
export vhwa_viss_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_viss_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_viss_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_viss_testapp_safertos


VHWA_VISS_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_VISS_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_VISS_TESTAPP_MACRO_LIST})

# VHWA NF baremetal test app
vhwa_nf_baremetal_testapp_COMP_LIST = vhwa_nf_baremetal_testapp
vhwa_nf_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_nf_test
vhwa_nf_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_nf_test
vhwa_nf_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_nf_baremetal_testapp_MAKEFILE
vhwa_nf_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_nf_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_nf_baremetal_testapp_COMP_LIST
export vhwa_nf_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_nf_baremetal_testapp_CORE_DEPENDENCY
vhwa_nf_baremetal_testapp_PKG_LIST = vhwa_nf_baremetal_testapp
vhwa_nf_baremetal_testapp_INCLUDE = $(vhwa_nf_baremetal_testapp_PATH)
vhwa_nf_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_nf_baremetal_testapp_BOARDLIST
vhwa_nf_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_vpac_$(SOC)_CORELIST)
export vhwa_nf_baremetal_testapp_$(SOC)_CORELIST
export vhwa_nf_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_nf_baremetal_testapp

export vhwa_nf_testapp_freertos_COMP_LIST = vhwa_nf_testapp_freertos
vhwa_nf_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_nf_test
vhwa_nf_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_nf_test
export vhwa_nf_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_nf_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_nf_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_nf_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_nf_testapp_freertos_PKG_LIST = vhwa_nf_testapp_freertos
vhwa_nf_testapp_freertos_INCLUDE = $(vhwa_nf_testapp_freertos_PATH)
export vhwa_nf_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_nf_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_nf_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_nf_testapp_freertos

export vhwa_nf_testapp_safertos_COMP_LIST = vhwa_nf_testapp_safertos
vhwa_nf_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_nf_test
vhwa_nf_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_nf_test
export vhwa_nf_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_nf_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_nf_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_nf_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_nf_testapp_safertos_PKG_LIST = vhwa_nf_testapp_safertos
vhwa_nf_testapp_safertos_INCLUDE = $(vhwa_nf_testapp_safertos_PATH)
export vhwa_nf_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_nf_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_nf_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_nf_testapp_safertos

VHWA_NF_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_NF_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_NF_TESTAPP_MACRO_LIST})

# VHWA SDE baremetal test app
vhwa_sde_baremetal_testapp_COMP_LIST = vhwa_sde_baremetal_testapp
vhwa_sde_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_sde_test
vhwa_sde_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_sde_test
vhwa_sde_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_sde_baremetal_testapp_MAKEFILE
vhwa_sde_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_sde_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_sde_baremetal_testapp_COMP_LIST
export vhwa_sde_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_sde_baremetal_testapp_CORE_DEPENDENCY
vhwa_sde_baremetal_testapp_PKG_LIST = vhwa_sde_baremetal_testapp
vhwa_sde_baremetal_testapp_INCLUDE = $(vhwa_sde_baremetal_testapp_PATH)
vhwa_sde_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_sde_baremetal_testapp_BOARDLIST
vhwa_sde_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_dmpac_$(SOC)_CORELIST)
export vhwa_sde_baremetal_testapp_$(SOC)_CORELIST
export vhwa_sde_baremetal_testapp_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_sde_baremetal_testapp

export vhwa_sde_testapp_freertos_COMP_LIST = vhwa_sde_testapp_freertos
vhwa_sde_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_sde_test
vhwa_sde_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_sde_test
export vhwa_sde_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_sde_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_sde_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_sde_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_sde_testapp_freertos_PKG_LIST = vhwa_sde_testapp_freertos
vhwa_sde_testapp_freertos_INCLUDE = $(vhwa_sde_testapp_freertos_PATH)
export vhwa_sde_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_sde_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_dmpac_$(SOC)_CORELIST))
export vhwa_sde_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_sde_testapp_freertos

ifeq ($(VHWA_NEGATIVE_TEST), true)
export vhwa_sde_negative_testapp_freertos_COMP_LIST = vhwa_sde_negative_testapp_freertos
vhwa_sde_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_sde_test
vhwa_sde_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_sde_test
export vhwa_sde_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_sde_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_sde_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_sde_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_sde_negative_testapp_freertos_PKG_LIST = vhwa_sde_negative_testapp_freertos
vhwa_sde_negative_testapp_freertos_INCLUDE = $(vhwa_sde_negative_testapp_freertos_PATH)
export vhwa_sde_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_sde_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_sde_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_sde_negative_testapp_freertos
endif

ifeq ($(VHWA_NEGATIVE_FOCO_TEST), true)
export vhwa_sde_negative_testapp_freertos_COMP_LIST = vhwa_sde_negative_testapp_freertos
vhwa_sde_negative_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_sde_test
vhwa_sde_negative_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_sde_test
export vhwa_sde_negative_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_sde_negative_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_sde_negative_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_sde_negative_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_sde_negative_testapp_freertos_PKG_LIST = vhwa_sde_negative_testapp_freertos
vhwa_sde_negative_testapp_freertos_INCLUDE = $(vhwa_sde_negative_testapp_freertos_PATH)
export vhwa_sde_negative_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_sde_negative_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_sde_negative_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_sde_negative_testapp_freertos
endif

export vhwa_sde_testapp_safertos_COMP_LIST = vhwa_sde_testapp_safertos
vhwa_sde_testapp_safertos_RELPATH = ti/drv/vhwa/examples/vhwa_sde_test
vhwa_sde_testapp_safertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_sde_test
export vhwa_sde_testapp_safertos_BOARD_DEPENDENCY = yes
export vhwa_sde_testapp_safertos_CORE_DEPENDENCY = yes
export vhwa_sde_testapp_safertos_XDC_CONFIGURO = $(if $(findstring tirtos, safertos), yes, no)
export vhwa_sde_testapp_safertos_MAKEFILE = -f makefile BUILD_OS_TYPE=safertos
vhwa_sde_testapp_safertos_PKG_LIST = vhwa_sde_testapp_safertos
vhwa_sde_testapp_safertos_INCLUDE = $(vhwa_sde_testapp_safertos_PATH)
export vhwa_sde_testapp_safertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_safertos), $(vhwa_default_BOARDLIST) )
export vhwa_sde_testapp_safertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_safertos), $(vhwa_dmpac_$(SOC)_CORELIST))
export vhwa_sde_testapp_safertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_sde_testapp_safertos


VHWA_SDE_TESTAPP_MACRO_LIST := $(foreach curos, $(vhwa_RTOS_LIST) safertos, $(call VHWA_SDE_TESTAPP_RULE,$(curos)))

$(eval ${VHWA_SDE_TESTAPP_MACRO_LIST})

# VHWA INT baremetal test app
vhwa_int_baremetal_testapp_COMP_LIST = vhwa_int_baremetal_testapp
vhwa_int_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_int_test
vhwa_int_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_int_test
vhwa_int_baremetal_testapp_MAKEFILE = -fmakefile_baremetal
export vhwa_int_baremetal_testapp_MAKEFILE
vhwa_int_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_int_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_int_baremetal_testapp_COMP_LIST
export vhwa_int_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_int_baremetal_testapp_CORE_DEPENDENCY
vhwa_int_baremetal_testapp_PKG_LIST = vhwa_int_baremetal_testapp
vhwa_int_baremetal_testapp_INCLUDE = $(vhwa_int_baremetal_testapp_PATH)
vhwa_int_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_int_baremetal_testapp_BOARDLIST
vhwa_int_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_$(SOC)_CORELIST)
export vhwa_int_baremetal_testapp_$(SOC)_CORELIST
vhwa_EXAMPLE_LIST += vhwa_int_baremetal_testapp

VHWA_CFLAGS =

VHWA_CFLAGS += -DFVID2_CFG_TRACE_ENABLE
VHWA_CFLAGS += -DFVID2_CFG_ASSERT_ENABLE

# Enable asserts and prints
VHWA_CFLAGS += -DVHWA_CFG_ASSERT_ENABLE
VHWA_CFLAGS += -DVHWA_CFG_USE_STD_ASSERT
VHWA_CFLAGS += -DVHWA_CFG_PRINT_ENABLE
VHWA_CFLAGS += -O0

ifeq ($(VHWA_NEGATIVE_TEST), true)
VHWA_CFLAGS += -DVHWA_TEST_NEGATIVE
endif

ifeq ($(VHWA_NEGATIVE_FOCO_TEST), true)
VHWA_CFLAGS += -DVHWA_TEST_NEGATIVE_FOCO
endif
export VHWA_CFLAGS

vhwa_component_make_include := 1
endif

# VHWA Flex Connect baremetal test app
vhwa_flexconnect_baremetal_testapp_COMP_LIST = vhwa_flexconnect_baremetal_testapp
vhwa_flexconnect_baremetal_testapp_RELPATH = ti/drv/vhwa/examples/vhwa_flexconnect_test
vhwa_flexconnect_baremetal_testapp_PATH = $(VHWA_COMP_PATH)/examples/vhwa_flexconnect_test
vhwa_flexconnect_baremetal_testapp_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export vhwa_flexconnect_baremetal_testapp_MAKEFILE
vhwa_flexconnect_baremetal_testapp_BOARD_DEPENDENCY = yes
vhwa_flexconnect_baremetal_testapp_CORE_DEPENDENCY = yes
export vhwa_flexconnect_baremetal_testapp_COMP_LIST
export vhwa_flexconnect_baremetal_testapp_BOARD_DEPENDENCY
export vhwa_flexconnect_baremetal_testapp_CORE_DEPENDENCY
vhwa_flexconnect_baremetal_testapp_PKG_LIST = vhwa_flexconnect_baremetal_testapp
vhwa_flexconnect_baremetal_testapp_INCLUDE = $(vhwa_flexconnect_baremetal_testapp_PATH)
vhwa_flexconnect_baremetal_testapp_BOARDLIST = $(vhwa_default_BOARDLIST)
export vhwa_flexconnect_baremetal_testapp_BOARDLIST
vhwa_flexconnect_baremetal_testapp_$(SOC)_CORELIST = $(vhwa_vpac_$(SOC)_CORELIST)
export vhwa_flexconnect_baremetal_testapp_$(SOC)_CORELIST
vhwa_EXAMPLE_LIST += vhwa_flexconnect_baremetal_testapp

export vhwa_flexconnect_testapp_freertos_COMP_LIST = vhwa_flexconnect_testapp_freertos
vhwa_flexconnect_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_flexconnect_test
vhwa_flexconnect_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_flexconnect_test
export vhwa_flexconnect_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_flexconnect_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_flexconnect_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_flexconnect_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_flexconnect_testapp_freertos_PKG_LIST = vhwa_flexconnect_testapp_freertos
vhwa_flexconnect_testapp_freertos_INCLUDE = $(vhwa_flexconnect_testapp_freertos_PATH)
export vhwa_flexconnect_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_flexconnect_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_flexconnect_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_flexconnect_testapp_freertos

export vhwa_flexconnect_m2m_testapp_freertos_COMP_LIST = vhwa_flexconnect_m2m_testapp_freertos
vhwa_flexconnect_m2m_testapp_freertos_RELPATH = ti/drv/vhwa/examples/vhwa_flexconnect_m2m_rtos_test
vhwa_flexconnect_m2m_testapp_freertos_PATH = $(VHWA_COMP_PATH)/examples/vhwa_flexconnect_m2m_rtos_test
export vhwa_flexconnect_m2m_testapp_freertos_BOARD_DEPENDENCY = yes
export vhwa_flexconnect_m2m_testapp_freertos_CORE_DEPENDENCY = yes
export vhwa_flexconnect_m2m_testapp_freertos_XDC_CONFIGURO = $(if $(findstring tirtos, freertos), yes, no)
export vhwa_flexconnect_m2m_testapp_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
vhwa_flexconnect_m2m_testapp_freertos_PKG_LIST = vhwa_flexconnect_m2m_testapp_freertos
vhwa_flexconnect_m2m_testapp_freertos_INCLUDE = $(vhwa_flexconnect_m2m_testapp_freertos_PATH)
export vhwa_flexconnect_m2m_testapp_freertos_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_freertos), $(vhwa_default_BOARDLIST) )
export vhwa_flexconnect_m2m_testapp_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(vhwa_vpac_$(SOC)_CORELIST))
export vhwa_flexconnect_m2m_testapp_freertos_SBL_APPIMAGEGEN = yes
vhwa_EXAMPLE_LIST += vhwa_flexconnect_m2m_testapp_freertos

export vhwa_LIB_LIST
export vhwa_EXAMPLE_LIST

export vhwa_PKG_LIST_ALL = $(vhwa_LIB_LIST) $(vhwa_EXAMPLE_LIST)

