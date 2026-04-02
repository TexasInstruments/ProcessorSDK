# File: enet_component.mk
#       This file is component include make file of XYZ driver library.
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
ifeq ($(enet_component_make_include), )

# List with various rtos_types(such as tirtos(sysbios),freertos,safertos) to build RTOS apps.
# Use the Default List defined in 'ti/build/makerules/component.mk'
# This list will be used to generate RTOS app make rule for each rtos_type.
drvenet_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

drvenet_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvenet_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvenet_j721e_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_j7200_CORELIST  = mpu1_0 mcu1_0 mcu2_0 mcu2_1
drvenet_j721s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_j784s4_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_j742s2_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0

drvenet_cli_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
drvenet_cli_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
drvenet_cli_j721e_CORELIST  = mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_cli_j7200_CORELIST  = mcu1_0 mcu2_0 mcu2_1
drvenet_cli_j721s2_CORELIST = mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_cli_j784s4_CORELIST = mcu1_0 mcu2_0 mcu2_1 mcu3_0
drvenet_cli_j742s2_CORELIST = mcu1_0 mcu2_0 mcu2_1 mcu3_0

enet_ecc_test_SOCLIST    = j721e j7200 j721s2 j784s4
enet_ecc_test_BOARDLIST  = j721e_evm j7200_evm j721s2_evm j784s4_evm
drvenet_ecc_CORELIST  = $(filter-out mpu1_0, ${drvenet_$(SOC)_CORELIST})

# Temp SoC/board lists which excludes for libraries/apps which
# are not enabled yet
drvenet_K3_SOCLIST      = j721e j7200 j721s2 j784s4 j742s2
drvenet_K3_BOARDLIST    = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm

# List of cores where LwIP is supported (i.e. exclude A cores)
drvenet_lwip_CORELIST  = $(filter-out mpu1_0, ${drvenet_$(SOC)_CORELIST})

# List of SoCs which require lwipific and intercore library
drvenet_ic_SOCLIST      = j721e j7200 j784s4 j742s2

# List of SoCs and cores where TSN related example app is supported
enet_tsn_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
enet_tsn_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
enet_tsn_j721e_CORELIST  = mcu2_0
enet_tsn_j7200_CORELIST  = mcu2_0
enet_tsn_j721s2_CORELIST = mcu2_0
enet_tsn_j784s4_CORELIST = mcu2_0
enet_tsn_j742s2_CORELIST = mcu2_0

#
# Enet RTOS boardlist rule
#
# Define the rule to generate Enet Drivers BOARDLIST for each rtos_type
# Default BOARDLIST for each rtos_type is defined in 'ti/build/makerules/component.mk'
# The following rule filters out Enet Drivers BOARDLIST for each rtos_type.
# Here $(1) refers to the first argument passed to the rule.
# In this case it is $(curos), each instance in "drvenet_RTOS_LIST" (ie, tirtos/freertos/safertos..)
#
define DRV_ENET_RTOS_BOARDLIST_RULE

drvenet_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvenet_BOARDLIST))

endef

# Define the macro list with rules of all rtos_types
DRV_ENET_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvenet_RTOS_LIST), $(call DRV_ENET_RTOS_BOARDLIST_RULE,$(curos)))

# Evaluate the macro list to generate BOARDLIST for all rtos_types
$(eval ${DRV_ENET_RTOS_BOARDLIST_MACRO_LIST})


#
# Enet RTOS boardlist rule
# Temp SoC/board lists which excludes non-Jacinto SoCs for libraries/apps which
# are not enabled yet
#
define DRV_ENET_K3_RTOS_BOARDLIST_RULE

drvenet_K3_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(drvenet_K3_BOARDLIST))

endef

DRV_ENET_K3_RTOS_BOARDLIST_MACRO_LIST := $(foreach curos, $(drvenet_RTOS_LIST), $(call DRV_ENET_K3_RTOS_BOARDLIST_RULE,$(curos)))

$(eval ${DRV_ENET_K3_RTOS_BOARDLIST_MACRO_LIST})


############################
# enet package
# List of components included under enet lib
# The components included here are built and will be part of enet lib
############################
enet_LIB_LIST =

############################
# enet app lib package
# List of components included under enet app lib
# The components included here are built and will be part of enet app lib
############################
enet_APP_LIB_LIST =

############################
# enet examples
# List of examples under enet (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
enet_EXAMPLE_LIST =

#
# Enet Modules
#

#
# Enet library
#
export enet_COMP_LIST = enet
enet_RELPATH = ti/drv/enet
enet_PATH = $(PDK_ENET_COMP_PATH)
export enet_LIBNAME = enet
export enet_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_MAKEFILE = -fsrc/makefile
export enet_BOARD_DEPENDENCY = no
export enet_CORE_DEPENDENCY = no
enet_PKG_LIST = enet
enet_INCLUDE = $(enet_PATH)
export enet_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721s2))
export enet_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
else
export enet_$(SOC)_CORELIST = $(drvenet_$(SOC)_CORELIST)
endif
enet_LIB_LIST += enet

#
# Enet SoC library
#
export enetsoc_COMP_LIST = enetsoc
enetsoc_RELPATH = ti/drv/enet/soc
enetsoc_PATH = $(PDK_ENET_COMP_PATH)/soc
export enetsoc_LIBNAME = enetsoc
export enetsoc_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enetsoc_MAKEFILE = -fmakefile
export enetsoc_BOARD_DEPENDENCY = no
export enetsoc_CORE_DEPENDENCY = yes
enetsoc_PKG_LIST = enetsoc
enetsoc_INCLUDE = $(enetsoc_PATH)
export enetsoc_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721s2))
export enetsoc_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
else
export enetsoc_$(SOC)_CORELIST = $(drvenet_$(SOC)_CORELIST)
endif
enet_LIB_LIST += enetsoc

#
# Enet PHY library
#
export enetphy_COMP_LIST = enetphy
enetphy_RELPATH = ti/drv/enet/src/phy
enetphy_PATH = $(PDK_ENET_COMP_PATH)/src/phy
export enetphy_LIBNAME = enetphy
export enetphy_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enetphy_MAKEFILE = -fmakefile
export enetphy_BOARD_DEPENDENCY = no
export enetphy_CORE_DEPENDENCY = yes
enetphy_PKG_LIST = enetphy
enetphy_INCLUDE = $(enetphy_PATH)
export enetphy_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721s2))
export enetphy_$(SOC)_CORELIST = mpu1_0 mcu1_0 mcu2_0 mcu2_1 mcu3_0
else
export enetphy_$(SOC)_CORELIST = $(drvenet_$(SOC)_CORELIST)
endif
enet_LIB_LIST += enetphy

#
# Enet CLI library
#
export enet_cli_COMP_LIST = enet_cli
enet_cli_RELPATH = ti/drv/enet/enet_cli/enet_cli_core
enet_cli_PATH = $(PDK_ENET_COMP_PATH)/enet_cli/enet_cli_core
export enet_cli_LIBNAME = enet_cli
export enet_cli_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_cli_MAKEFILE = -fmakefile BUILD_OS_TYPE=freertos
export enet_cli_BOARD_DEPENDENCY = no
export enet_cli_CORE_DEPENDENCY = no
enet_cli_PKG_LIST = enet_cli
enet_cli_INCLUDE = $(enet_cli_PATH)
export enet_cli_SOCLIST = $(drvenet_cli_SOCLIST)
export enet_cli_$(SOC)_CORELIST = $(drvenet_cli_$(SOC)_CORELIST)
ifeq ($(1),$(filter $(1), freertos))
enet_LIB_LIST += enet_cli
endif
#
# FreeRTOS Plus CLI library
#
export freertos_cli_COMP_LIST = freertos_cli
freertos_cli_RELPATH = ti/drv/enet/enet_cli/FreeRTOS-Plus-CLI
freertos_cli_PATH = $(PDK_ENET_COMP_PATH)/enet_cli/FreeRTOS-Plus-CLI
export freertos_cli_LIBNAME = freertos_cli
export freertos_cli_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export freertos_cli_MAKEFILE = -f freertos_cli.mk BUILD_OS_TYPE=freertos
export freertos_cli_BOARD_DEPENDENCY = no
export freertos_cli_CORE_DEPENDENCY = no
freertos_cli_PKG_LIST = freertos_cli
freertos_cli_INCLUDE = $(freertos_cli_PATH)
export freertos_cli_SOCLIST = $(drvenet_cli_SOCLIST)
export freertos_cli_$(SOC)_CORELIST = $(drvenet_cli_$(SOC)_CORELIST)
ifeq ($(1),$(filter $(1), freertos))
enet_LIB_LIST += freertos_cli
endif
#
# lwipif interface library
#
define lwipif_RULE

export lwipif_$(1)_COMP_LIST = lwipif_$(1)
lwipif_$(1)_RELPATH = ti/drv/enet/lwipif
lwipif_$(1)_PATH = $(PDK_ENET_COMP_PATH)/lwipif
export lwipif_$(1)_LIBNAME = lwipif_$(1)
export lwipif_$(1)_LIBPATH = $(PDK_ENET_COMP_PATH)/lib/$(1)
export lwipif_$(1)_OBJPATH = ti/drv/enet/lwipif/$(1)
export lwipif_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export lwipif_$(1)_BOARD_DEPENDENCY = no
export lwipif_$(1)_CORE_DEPENDENCY = no
lwipif_$(1)_PKG_LIST = lwipif_$(1)
lwipif_$(1)_INCLUDE = $(PDK_ENET_COMP_PATH)/lwipif/inc
export lwipif_$(1)_SOCLIST = $(drvenet_SOCLIST)
export lwipif_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_lwip_CORELIST))
ifeq ($(1),$(filter $(1), freertos))
enet_LIB_LIST += lwipif_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_LIB_LIST += lwipif_$(1)
endif
endif

endef

lwipif_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call lwipif_RULE,$(curos)))
$(eval ${lwipif_MACRO_LIST})

#
# intercore lwipif interface library
#
define lwipific_RULE

export lwipific_$(1)_COMP_LIST = lwipific_$(1)
lwipific_$(1)_RELPATH = ti/drv/enet/lwipific
lwipific_$(1)_PATH = $(PDK_ENET_COMP_PATH)/lwipific
export lwipific_$(1)_LIBNAME = lwipific_$(1)
export lwipific_$(1)_LIBPATH = $(PDK_ENET_COMP_PATH)/lib/$(1)
export lwipific_$(1)_OBJPATH = ti/drv/enet/lwipific/$(1)
export lwipific_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
export lwipific_$(1)_BOARD_DEPENDENCY = no
export lwipific_$(1)_CORE_DEPENDENCY = no
lwipific_$(1)_PKG_LIST = lwipific_$(1)
lwipific_$(1)_INCLUDE = $(PDK_ENET_COMP_PATH)/lwipific/inc
export lwipific_$(1)_SOCLIST = $(drvenet_ic_SOCLIST)
export lwipific_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_lwip_CORELIST))
ifeq ($(1),$(filter $(1), freertos))
enet_LIB_LIST += lwipific_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_LIB_LIST += lwipific_$(1)
endif
endif

endef

lwipific_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call lwipific_RULE,$(curos)))
$(eval ${lwipific_MACRO_LIST})

#
# intercore packet transport library
#
export enet_intercore_COMP_LIST = enet_intercore
enet_intercore_RELPATH = ti/drv/enet/intercore
enet_intercore_PATH = $(PDK_ENET_COMP_PATH)/intercore
export enet_intercore_LIBNAME = enet_intercore
export enet_intercore_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_intercore_MAKEFILE = -fmakefile
export enet_intercore_BOARD_DEPENDENCY = no
export enet_intercore_CORE_DEPENDENCY = no
enet_intercore_PKG_LIST = enet_intercore
enet_intercore_INCLUDE = $(enet_intercore_PATH)
export enet_intercore_SOCLIST = $(drvenet_ic_SOCLIST)
export enet_intercore_$(SOC)_CORELIST = $(drvenet_$(SOC)_CORELIST)
enet_LIB_LIST += enet_intercore

#
# Enet config server library
#
define ENET_CFGSERVER_RULE

export enet_cfgserver_$(1)_COMP_LIST = enet_cfgserver_$(1)
enet_cfgserver_$(1)_RELPATH = ti/drv/enet/enet_cfgserver
enet_cfgserver_$(1)_PATH = $(PDK_ENET_COMP_PATH)/enet_cfgserver
export enet_cfgserver_$(1)_LIBNAME = enet_cfgserver_$(1)
export enet_cfgserver_$(1)_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_cfgserver_$(1)_OBJPATH = ti/drv/enet/enet_cfgserver_$(1)
export enet_cfgserver_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
export enet_cfgserver_$(1)_BOARD_DEPENDENCY = no
export enet_cfgserver_$(1)_CORE_DEPENDENCY = no
enet_cfgserver_$(1)_PKG_LIST = enet_cfgserver_$(1)
enet_cfgserver_$(1)_INCLUDE = $(enet_cfgserver_$(1)_PATH)
export enet_cfgserver_$(1)_SOCLIST = $(drvenet_SOCLIST)
export enet_cfgserver_$(1)_$(SOC)_CORELIST = $(drvenet_lwip_CORELIST)
ifneq ($(SOC),$(filter $(SOC), j721s2))
ifeq ($(1),$(filter $(1), freertos))
enet_APP_LIB_LIST += enet_cfgserver_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_APP_LIB_LIST += enet_cfgserver_$(1)
endif
endif
endif

endef

ENET_CFGSERVER_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call ENET_CFGSERVER_RULE,$(curos)))

$(eval ${ENET_CFGSERVER_MACRO_LIST})

#
# Enet example utils library
#
# Define the rule to generate the 'Enet example utils library' make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule.
#   - In this case it is $(curos), each instance in "drvenet_RTOS_LIST" (ie, tirtos/freertos/safertos)
# - The target name will be <app_name>_<rtos_type> (ie, enet_example_utils_tirtos/enet_example_utils_freertos/...)
# - Here BOARDLIST passed for the target is drivers BOARDLIST for the particular rtos_type(ie, $(drvenet_$(rtos_type)_BOARDLIST). )
#   - If passing a custom or common BOARDLIST, one **should** always filter with Default BOARDLIST for each rtos_type(ie, $(DEFAULT_BOARDLIST_$(rtos_type)). )
#   - For example "export enet_example_utils_$(1)_BOARDLIST =  $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm )
#   - This is because, in case of passing custom or common BOARDLIST, some boards may not be supporting all rtos_types
# - CORELIST passed for the target **should** always be filtered with the Default CORELIST of the SOC for each rtos_type (ie, $(DEFAULT_$(SOC)_CORELIST_$(1)). )
#   - The default CORELIST of an SOC for each rtos_type is defined in 'ti/build/makerules/component.mk'
#   - This is because some rtos_type won't be supported on specific cores. (For example, FreeRTOS is not supported on mpu1_0 core)
# - SafeRTOS example should be added to the example list only if SafeRTOS Kernel is present in the path.
#
define ENET_UTILS_RULE

export enet_example_utils_$(1)_COMP_LIST = enet_example_utils_$(1)
enet_example_utils_$(1)_RELPATH = ti/drv/enet/examples/utils
enet_example_utils_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/utils
export enet_example_utils_$(1)_LIBNAME = enet_example_utils_$(1)
export enet_example_utils_$(1)_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_example_utils_$(1)_OBJPATH = ti/drv/enet/examples/utils/enet_example_utils_$(1)
export enet_example_utils_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
export enet_example_utils_$(1)_BOARD_DEPENDENCY = yes
export enet_example_utils_$(1)_CORE_DEPENDENCY = yes
enet_example_utils_$(1)_PKG_LIST = enet_example_utils_$(1)
enet_example_utils_$(1)_INCLUDE = $(enet_example_utils_$(1)_PATH)
export enet_example_utils_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_$(SOC)_CORELIST))
export enet_example_utils_$(1)_BOARDLIST = $(drvenet_$(1)_BOARDLIST)
export enet_example_utils_$(1)_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(1),$(filter $(1), freertos))
enet_APP_LIB_LIST += enet_example_utils_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_APP_LIB_LIST += enet_example_utils_$(1)
endif
endif

endef

ENET_UTILS_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call ENET_UTILS_RULE,$(curos)))

$(eval ${ENET_UTILS_MACRO_LIST})

#
# Enet example utils library (full version)
#
define ENET_UTILS_FULL_RULE

export enet_example_utils_full_$(1)_COMP_LIST = enet_example_utils_full_$(1)
enet_example_utils_full_$(1)_RELPATH = ti/drv/enet/examples/utils
enet_example_utils_full_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/utils
export enet_example_utils_full_$(1)_LIBNAME = enet_example_utils_full_$(1)
export enet_example_utils_full_$(1)_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_example_utils_full_$(1)_OBJPATH = ti/drv/enet/examples/utils/enet_example_utils_full_$(1)
export enet_example_utils_full_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1) APPUTILS_TYPE=full
export enet_example_utils_full_$(1)_BOARD_DEPENDENCY = yes
export enet_example_utils_full_$(1)_CORE_DEPENDENCY = yes
enet_example_utils_full_$(1)_PKG_LIST = enet_example_utils_full_$(1)
enet_example_utils_full_$(1)_INCLUDE = $(enet_example_utils_full_$(1)_PATH)
export enet_example_utils_full_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_$(SOC)_CORELIST))
export enet_example_utils_full_$(1)_BOARDLIST = $(drvenet_$(1)_BOARDLIST)
export enet_example_utils_full_$(1)_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(1),$(filter $(1), freertos))
enet_LIB_LIST += enet_example_utils_full_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_LIB_LIST += enet_example_utils_full_$(1)
endif
endif

endef

ENET_UTILS_FULL_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call ENET_UTILS_FULL_RULE,$(curos)))

$(eval ${ENET_UTILS_FULL_MACRO_LIST})

#
# Enet example utils (baremetal) library
#
export enet_example_utils_baremetal_COMP_LIST = enet_example_utils_baremetal
enet_example_utils_baremetal_RELPATH = ti/drv/enet/examples/utils
enet_example_utils_baremetal_PATH = $(PDK_ENET_COMP_PATH)/examples/utils
export enet_example_utils_baremetal_LIBNAME = enet_example_utils_baremetal
export enet_example_utils_baremetal_LIBPATH = $(PDK_ENET_COMP_PATH)/lib
export enet_example_utils_baremetal_OBJPATH = $(enet_example_utils_baremetal_RELPATH)/baremetal
export enet_example_utils_baremetal_MAKEFILE = -fmakefile BUILD_OS_TYPE=baremetal
export enet_example_utils_baremetal_BOARD_DEPENDENCY = yes
export enet_example_utils_baremetal_CORE_DEPENDENCY = yes
enet_example_utils_baremetal_PKG_LIST = enet_example_utils_baremetal
enet_example_utils_baremetal_INCLUDE = $(enet_example_utils_baremetal_PATH)
export enet_example_utils_baremetal_SOCLIST = $(drvenet_SOCLIST)
export enet_example_utils_baremetal_$(SOC)_CORELIST = $(drvenet_$(SOC)_CORELIST)
enet_APP_LIB_LIST += enet_example_utils_baremetal

#
# ENET lwIP example app
#
define enet_lwip_example_RULE

export enet_lwip_example_$(1)_COMP_LIST = enet_lwip_example_$(1)
enet_lwip_example_$(1)_RELPATH = ti/drv/enet/examples/enet_lwip_example
enet_lwip_example_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_lwip_example
export enet_lwip_example_$(1)_BOARD_DEPENDENCY = yes
export enet_lwip_example_$(1)_CORE_DEPENDENCY = yes
export enet_lwip_example_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
enet_lwip_example_$(1)_PKG_LIST = enet_lwip_example_$(1)
enet_lwip_example_$(1)_INCLUDE = $(enet_lwip_example_$(1)_PATH)
export enet_lwip_example_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_lwip_CORELIST))
export enet_lwip_example_$(1)_BOARDLIST = $(drvenet_$(1)_BOARDLIST)
export enet_lwip_example_$(1)_SOCLIST = $(drvenet_SOCLIST)
export enet_lwip_example_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_lwip_example_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_EXAMPLE_LIST += enet_lwip_example_$(1)
endif
endif

endef

enet_lwip_example_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos,$(call enet_lwip_example_RULE,$(curos)))
$(eval ${enet_lwip_example_MACRO_LIST})

#
# HelloWorld example
#
# The HelloWorld example is a simplified version of the loopback test below.
# - This example will run on bare metal only.
# - The SOC, BOARD, and CORE options work like all other examples.
#
export enet_helloworld_example_COMP_LIST = enet_helloworld_example
enet_helloworld_example_RELPATH = ti/drv/enet/examples/enet_helloworld_example
enet_helloworld_example_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_helloworld_example
export enet_helloworld_example_BOARD_DEPENDENCY = yes
export enet_helloworld_example_CORE_DEPENDENCY = yes
export enet_helloworld_example_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
enet_helloworld_example_PKG_LIST = enet_helloworld_example
enet_helloworld_example_INCLUDE = $(enet_helloworld_example_PATH)
export enet_helloworld_example_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST), $(drvenet_$(SOC)_CORELIST))
export enet_helloworld_example_BOARDLIST = $(drvenet_BOARDLIST)
export enet_helloworld_example_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
export enet_helloworld_example_SBL_APPIMAGEGEN = yes
endif
enet_EXAMPLE_LIST += enet_helloworld_example

# Enet CLI Application

export enet_cli_app_COMP_LIST = enet_cli_app
enet_cli_app_RELPATH = ti/drv/enet/test/enet_cli_app
enet_cli_app_PATH = $(PDK_ENET_COMP_PATH)/test/enet_cli_app
export enet_cli_app_BOARD_DEPENDENCY = yes
export enet_cli_app_CORE_DEPENDENCY = yes
export enet_cli_app_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
export enet_example_utils_$(1)_OBJPATH = ti/drv/enet/examples/utils
enet_cli_app_PKG_LIST = enet_cli_app
enet_cli_app_INCLUDE = $(enet_cli_app_PATH)
export enet_cli_app_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST), $(drvenet_cli_$(SOC)_CORELIST))
export enet_cli_app_BOARDLIST = $(drvenet_cli_BOARDLIST)
export enet_cli_app_SOCLIST = $(drvenet_cli_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
export enet_cli_app_SBL_APPIMAGEGEN = yes
endif
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_cli_app
endif

# Enet CLI Test

export enet_cli_test_freertos_COMP_LIST = enet_cli_test_freertos
enet_cli_test_freertos_RELPATH = ti/drv/enet/test/enet_cli_test
enet_cli_test_freertos_PATH = $(PDK_ENET_COMP_PATH)/test/enet_cli_test
export enet_cli_test_freertos_BOARD_DEPENDENCY = yes
export enet_cli_test_freertos_CORE_DEPENDENCY = yes
export enet_cli_test_freertos_MAKEFILE = -f makefile BUILD_OS_TYPE=freertos
export enet_example_utils_freertos_OBJPATH = ti/drv/enet/examples/utils
enet_cli_test_freertos_PKG_LIST = enet_cli_test_freertos
enet_cli_test_freertos_INCLUDE = $(enet_cli_test_freertos_PATH)
export enet_cli_test_freertos_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_freertos), $(drvenet_cli_$(SOC)_CORELIST))
export enet_cli_test_freertos_BOARDLIST = $(drvenet_cli_BOARDLIST)
export enet_cli_test_freertos_SOCLIST = $(drvenet_cli_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
export enet_cli_test_freertos_SBL_APPIMAGEGEN = yes
endif
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_cli_test_freertos
endif

# ECC Test

export enet_ecc_test_COMP_LIST = enet_ecc_test
enet_ecc_test_RELPATH = ti/drv/enet/test/enet_ecc_test
enet_ecc_test_PATH = $(PDK_ENET_COMP_PATH)/test/enet_ecc_test
export enet_ecc_test_BOARD_DEPENDENCY = yes
export enet_ecc_test_CORE_DEPENDENCY = yes
export enet_ecc_test_MAKEFILE = -f makefile BUILD_OS_TYPE=baremetal
enet_ecc_test_PKG_LIST = enet_ecc_test
enet_ecc_test_INCLUDE = $(enet_ecc_test_PATH)
export enet_ecc_test_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST), $(drvenet_ecc_CORELIST))
export enet_ecc_test_BOARDLIST
export enet_ecc_test_SOCLIST
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4))
export enet_ecc_test_SBL_APPIMAGEGEN = yes
endif
enet_EXAMPLE_LIST += enet_ecc_test

#
# Loopback test
#
# Define the rule to generate the 'Enet loopback test' make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule.
#   - In this case it is $(curos), each instance in "drvenet_RTOS_LIST" (ie, tirtos/freertos/safertos)
# - The target name will be <app_name>_<rtos_type> (ie, enet_loopback_test_tirtos/enet_loopback_test_freertos/...)
# - Pass the arg BUILD_OS_TYPE=<rtos_type> to the common makefile for all rtos_types
#   - In the common makefile, make use the param $(BUILD_OS_TYPE)
#   - For example:
#       APP_NAME = enet_loopback_test_$(BUILD_OS_TYPE)
#       ifeq ($(BUILD_OS_TYPE), tirtos)
#           INCLUDE_EXTERNAL_INTERFACES += xdc bios
#       endif
#       ifeq ($(BUILD_OS_TYPE), freertos)
#           INCLUDE_EXTERNAL_INTERFACES += freertos
#       endif
# - Here BOARDLIST passed for the target is drivers BOARDLIST for the particular rtos_type(ie, $(drvenet_$(rtos_type)_BOARDLIST). )
#   - If passing a custom or common BOARDLIST, one **should** always filter with Default BOARDLIST for each rtos_type(ie, $(DEFAULT_BOARDLIST_$(rtos_type)). )
#   - For example "export enet_example_utils_$(1)_BOARDLIST =  $(filter $(DEFAULT_BOARDLIST_$(1)), j721e_evm )
#   - This is because, in case of passing custom or common BOARDLIST, some boards may not be supporting all rtos_types
# - CORELIST passed for the target **should** always be filtered with the Default CORELIST of the SOC for each rtos_type (ie, $(DEFAULT_$(SOC)_CORELIST_$(1)). )
#   - The default CORELIST of an SOC for each rtos_type is defined in 'ti/build/makerules/component.mk'
#   - This is because some rtos_type won't be supported on specific cores. (For example, FreeRTOS is not supported on mpu1_0 core)
# - SafeRTOS example should be added to the example list only if SafeRTOS Kernel is present in the path.
#
define ENET_LOOPBACK_TEST_RULE

export enet_loopback_test_$(1)_COMP_LIST = enet_loopback_test_$(1)
enet_loopback_test_$(1)_RELPATH = ti/drv/enet/examples/enet_loopback_test
enet_loopback_test_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_loopback_test
export enet_loopback_test_$(1)_BOARD_DEPENDENCY = yes
export enet_loopback_test_$(1)_CORE_DEPENDENCY = yes
export enet_loopback_test_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
enet_loopback_test_$(1)_PKG_LIST = enet_loopback_test_$(1)
enet_loopback_test_$(1)_INCLUDE = $(enet_loopback_test_$(1)_PATH)
export enet_loopback_test_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_$(SOC)_CORELIST))
export enet_loopback_test_$(1)_BOARDLIST = $(drvenet_$(1)_BOARDLIST)
export enet_loopback_test_$(1)_SOCLIST = $(drvenet_SOCLIST)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
export enet_loopback_test_$(1)_SBL_APPIMAGEGEN = yes
endif
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_loopback_test_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_EXAMPLE_LIST += enet_loopback_test_$(1)
endif
endif

endef

ENET_LOOPBACK_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos, $(call ENET_LOOPBACK_TEST_RULE,$(curos)))

$(eval ${ENET_LOOPBACK_TEST_MACRO_LIST})

#
#Enet EST Example
#
define ENET_EST_TEST_RULE

export enet_est_test_$(1)_COMP_LIST = enet_est_test_$(1)
enet_est_test_$(1)_RELPATH = ti/drv/enet/examples/enet_est_test
enet_est_test_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_est_test
export enet_est_test_$(1)_BOARD_DEPENDENCY = yes
export enet_est_test_$(1)_CORE_DEPENDENCY = yes
export enet_est_test_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
enet_est_test_$(1)_PKG_LIST = enet_est_test_$(1)
enet_est_test_$(1)_INCLUDE = $(enet_est_test_$(1)_PATH)
export enet_est_test_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(drvenet_$(SOC)_CORELIST))
export enet_est_test_$(1)_BOARDLIST = $(drvenet_$(1)_BOARDLIST)
export enet_est_test_$(1)_SOCLIST = $(drvenet_SOCLIST)
export enet_est_test_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_est_test_$(1)
else
ifneq ($(wildcard $(SAFERTOS_KERNEL_INSTALL_PATH)),)
enet_EXAMPLE_LIST += enet_est_test_$(1)
endif
endif

endef

ENET_EST_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST) safertos, $(call ENET_EST_TEST_RULE,$(curos)))

$(eval ${ENET_EST_TEST_MACRO_LIST})


#
# TSN gptp example app
#
#
# Defines the rule to generate the gPTP example app make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule. In this case it
#   is $(curos), each instance in "drvenet_RTOS_LIST" (ie, freertos/safertos)
#
define ENET_TSN_GPTP_TEST_RULE

export enet_tsn_gptp_example_$(1)_COMP_LIST = enet_tsn_gptp_example_$(1)
enet_tsn_gptp_example_$(1)_RELPATH = ti/drv/enet/examples/enet_tsn/enet_tsn_gptp_example
enet_tsn_gptp_example_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_tsn/enet_tsn_gptp_example
export enet_tsn_gptp_example_$(1)_BOARD_DEPENDENCY = yes
export enet_tsn_gptp_example_$(1)_CORE_DEPENDENCY = yes
export enet_tsn_gptp_example_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
enet_tsn_gptp_example_$(1)_PKG_LIST = enet_tsn_gptp_example_$(1)
enet_tsn_gptp_example_$(1)_INCLUDE = $(enet_tsn_gptp_example_$(1)_PATH)
export enet_tsn_gptp_example_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(enet_tsn_$(SOC)_CORELIST))
export enet_tsn_gptp_example_$(1)_BOARDLIST = $(enet_tsn_BOARDLIST)
export enet_tsn_gptp_example_$(1)_SOCLIST = $(enet_tsn_SOCLIST)
export enet_tsn_gptp_example_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_tsn_gptp_example_$(1)
endif

endef

ENET_TSN_GPTP_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST), $(call ENET_TSN_GPTP_TEST_RULE,$(curos)))
$(eval ${ENET_TSN_GPTP_TEST_MACRO_LIST})


#
# TSN LLDP example app
#
#
# Defines the rule to generate the LLDP example app make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule. In this case it
#   is $(curos), each instance in "drvenet_RTOS_LIST" (ie, freertos/safertos)
#

define ENET_TSN_LLDP_TEST_RULE

export enet_tsn_lldp_example_$(1)_COMP_LIST = enet_tsn_lldp_example_$(1)
enet_tsn_lldp_example_$(1)_RELPATH = ti/drv/enet/examples/enet_tsn/enet_tsn_lldp_example
enet_tsn_lldp_example_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_tsn/enet_tsn_lldp_example
export enet_tsn_lldp_example_$(1)_BOARD_DEPENDENCY = yes
export enet_tsn_lldp_example_$(1)_CORE_DEPENDENCY = yes
export enet_tsn_lldp_example_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
enet_tsn_lldp_example_$(1)_PKG_LIST = enet_tsn_lldp_example_$(1)
enet_tsn_lldp_example_$(1)_INCLUDE = $(enet_tsn_lldp_example_$(1)_PATH)
export enet_tsn_lldp_example_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(enet_tsn_$(SOC)_CORELIST))
export enet_tsn_lldp_example_$(1)_BOARDLIST = $(enet_tsn_BOARDLIST)
export enet_tsn_lldp_example_$(1)_SOCLIST = $(enet_tsn_SOCLIST)
export enet_tsn_lldp_example_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_tsn_lldp_example_$(1)
endif

endef

ENET_TSN_LLDP_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST), $(call ENET_TSN_LLDP_TEST_RULE,$(curos)))
$(eval ${ENET_TSN_LLDP_TEST_MACRO_LIST})

#
# TSN EST example app
#
#
# Defines the rule to generate the TSN EST example app make rule for each rtos_type
# - Here $(1) refers to the first argument passed to the rule. In this case it
#   is $(curos), each instance in "drvenet_RTOS_LIST" (ie, freertos/safertos)
#
define ENET_TSN_EST_TEST_RULE

export enet_tsn_est_example_$(1)_COMP_LIST = enet_tsn_est_example_$(1)
enet_tsn_est_example_$(1)_RELPATH = ti/drv/enet/examples/enet_tsn/enet_tsn_est_example
enet_tsn_est_example_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_tsn/enet_tsn_est_example
export enet_tsn_est_example_$(1)_BOARD_DEPENDENCY = yes
export enet_tsn_est_example_$(1)_CORE_DEPENDENCY = yes
export enet_tsn_est_example_$(1)_MAKEFILE = -fmakefile BUILD_OS_TYPE=$(1)
enet_tsn_est_example_$(1)_PKG_LIST = enet_tsn_est_example_$(1)
enet_tsn_est_example_$(1)_INCLUDE = $(enet_tsn_est_example_$(1)_PATH)
export enet_tsn_est_example_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(enet_tsn_$(SOC)_CORELIST))
export enet_tsn_est_example_$(1)_BOARDLIST = $(enet_tsn_BOARDLIST)
export enet_tsn_est_example_$(1)_SOCLIST = $(enet_tsn_SOCLIST)
export enet_tsn_est_example_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_tsn_est_example_$(1)
endif

endef


ENET_TSN_EST_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST), $(call ENET_TSN_EST_TEST_RULE,$(curos)))
$(eval ${ENET_TSN_EST_TEST_MACRO_LIST})

#
# TSN NETCONF example app
#
define ENET_TSN_NETCONF_TEST_RULE

export enet_tsn_netconf_example_$(1)_COMP_LIST = enet_tsn_netconf_example_$(1)
enet_tsn_netconf_example_$(1)_RELPATH = ti/drv/enet/examples/enet_tsn/enet_tsn_netconf_example
enet_tsn_netconf_example_$(1)_PATH = $(PDK_ENET_COMP_PATH)/examples/enet_tsn/enet_tsn_netconf_example
export enet_tsn_netconf_example_$(1)_BOARD_DEPENDENCY = yes
export enet_tsn_netconf_example_$(1)_CORE_DEPENDENCY = yes
export enet_tsn_netconf_example_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
enet_tsn_netconf_example_$(1)_PKG_LIST = enet_tsn_netconf_example_$(1)
enet_tsn_netconf_example_$(1)_INCLUDE = $(enet_tsn_netconf_example_$(1)_PATH)
export enet_tsn_netconf_example_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(enet_tsn_$(SOC)_CORELIST))
export enet_tsn_netconf_example_$(1)_BOARDLIST = $(enet_tsn_BOARDLIST)
export enet_tsn_netconf_example_$(1)_SOCLIST = $(enet_tsn_SOCLIST)
export enet_tsn_netconf_example_$(1)_SBL_APPIMAGEGEN = yes
ifeq ($(1),$(filter $(1), freertos))
enet_EXAMPLE_LIST += enet_tsn_netconf_example_$(1)
endif

endef

ENET_TSN_NETCONF_TEST_MACRO_LIST := $(foreach curos,$(drvenet_RTOS_LIST), $(call ENET_TSN_NETCONF_TEST_RULE,$(curos)))
$(eval ${ENET_TSN_NETCONF_TEST_MACRO_LIST})

#
# Unit test (TI-RTOS)
#
-include $(PDK_ENET_COMP_PATH)/unit_test/enet_ut_component.mk
ifneq ($(SOC),$(filter $(SOC), j7200 j721s2 j784s4 j742s2))
ifneq ($(enet_ut_LIB_LIST),)
  enet_LIB_LIST += $(enet_ut_LIB_LIST)
endif
ifneq ($(enet_ut_EXAMPLE_LIST),)
  enet_EXAMPLE_LIST += $(enet_ut_EXAMPLE_LIST)
endif
endif

export enet_LIB_LIST
export enet_EXAMPLE_LIST
export drvenet_LIB_LIST = $(enet_LIB_LIST)
export drvenet_EXAMPLE_LIST = $(enet_EXAMPLE_LIST)

# Enable asserts and prints
ENET_CFLAGS =
ENET_CFLAGS += -DENET_CFG_ASSERT=1
#ENET_CFLAGS += -DENET_CFG_USE_STD_ASSERT
ENET_CFLAGS += -DENET_CFG_PRINT_ENABLE
ENET_CFLAGS += -DUART_ENABLED

# Trace level per build profile:
# 0 - None
# 1 - Error
# 2 - Warning
# 3 - Info
# 4 - Debug
# 5 - Verbose
ifeq ($(BUILD_PROFILE_$(CORE)),debug)
    ENET_CFLAGS += -DENET_CFG_TRACE_LEVEL=4
    ENET_CFLAGS += -DENET_CFG_DEV_ERROR=1
    ENET_CFLAGS += -DENETDMA_INSTRUMENTATION_ENABLED
    ENET_CFLAGS += -DENETCPTS_INSTRUMENTATION_ENABLED
else
    ENET_CFLAGS += -DENET_CFG_TRACE_LEVEL=3
endif

# lwIP interface layer allows application to perform packet handling
# only on j721e/j7200, though actually needed only on mcu2_0 by EthFw.
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j784s4 j742s2))
    ENET_CFLAGS += -DLWIPIF_APP_RX_PKT_HANDLING
endif

# Enable MDIO manual mode in j721e SoC due to errata i2329.
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2))
    ENET_CFLAGS += -DENABLE_MDIO_MANUAL_MODE
endif

export ENET_CFLAGS

enet_component_make_include := 1
endif
