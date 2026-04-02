# File: ip_fma_component.mk
#       This file is component include make file of ip fma .
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

############################
# ip fma package
# List of components included under ip fma lib
# The components included here are built and will be part of ip fma lib
############################
ip_fma_LIB_LIST =

############################
# ip fma examples
# List of examples under ip fma (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
ip_fma_EXAMPLE_LIST =


# IP FMA LIBRARY

ip_fma_SOCLIST         = j721s2 j784s4
ip_fma_BOARDLIST       = j721s2_evm j784s4_evm
ip_fma_j784s4_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1
ip_fma_j721s2_CORELIST = mcu1_0 mcu1_1 mcu2_0 mcu2_1

ip_fma_COMP_LIST = ip_fma
ip_fma_RELPATH   = ti/ip_fma
ip_fma_PATH      = $(IP_FMA_COMP_PATH)
ip_fma_LIBNAME   = ip_fma
ip_fma_LIBPATH   = $(IP_FMA_COMP_PATH)/lib
ip_fma_MAKEFILE  = -fmakefile
export ip_fma_MAKEFILE
export ip_fma_LIBNAME
export ip_fma_LIBPATH
ip_fma_SOC_DEPENDENCY = yes
ip_fma_CORE_DEPENDENCY = yes
export ip_fma_COMP_LIST
export ip_fma_BOARD_DEPENDENCY
export ip_fma_CORE_DEPENDENCY
ip_fma_PKG_LIST = ip_fma
ip_fma_INCLUDE  = $(ip_fma_PATH)/inc
export ip_fma_SOCLIST
export ip_fma_$(SOC)_CORELIST
ip_fma_LIB_LIST += ip_fma

export ip_fma_LIB_LIST


# RL7 IP FMA APP
ip_fma_rl7_SOCLIST           = j784s4 j721s2
ip_fma_rl7_BOARDLIST         = j784s4_evm j721s2_evm
ip_fma_rl7_CORELIST 	     = mcu1_0

define RL7_TESTAPP_RULE

rl7_ip_fma_app_$(1)_COMP_LIST = rl7_ip_fma_app_$(1)
rl7_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/rl7_ip_fma_app
rl7_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/rl7_ip_fma_app
rl7_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export rl7_ip_fma_app_$(1)_MAKEFILE
rl7_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
rl7_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
export rl7_ip_fma_app_$(1)_COMP_LIST
export rl7_ip_fma_app_$(1)_SOC_DEPENDENCY
export rl7_ip_fma_app_$(1)_CORE_DEPENDENCY
rl7_ip_fma_app_$(1)_PKG_LIST = rl7_ip_fma_app_$(1)
rl7_ip_fma_app_$(1)_INCLUDE  = $(rl7_ip_fma_app_$(1)_PATH)
rl7_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_rl7_BOARDLIST)
export rl7_ip_fma_app_$(1)_BOARDLIST
rl7_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_rl7_CORELIST)
export rl7_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += rl7_ip_fma_app_$(1)
rl7_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export rl7_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

RL7_MACRO_LIST := $(foreach curos, baremetal, $(call RL7_TESTAPP_RULE,$(curos)))

$(eval ${RL7_MACRO_LIST})


# C7X MMA SAFETY EXAMPLE APP

C7X_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

c7x_mma_SOCLIST   = j721e j721s2 j784s4 j742s2
c7x_mma_BOARDLIST = j721e_evm j721s2_evm j784s4_evm j742s2_evm
c7x_mma_CORELIST  = c7x_1

define C7X_MMA_TESTAPP_RULE
export c7x_mma_error_injection_testapp_$(1)_COMP_LIST = c7x_mma_error_injection_testapp_$(1)
c7x_mma_error_injection_testapp_$(1)_RELPATH = ti/ip_fma/examples/c7x_mma_error_injection
c7x_mma_error_injection_testapp_$(1)_PATH = $(PDK_INSTALL_PATH)/ti/ip_fma/examples/c7x_mma_error_injection
export c7x_mma_error_injection_testapp_$(1)_BOARD_DEPENDENCY = yes
export c7x_mma_error_injection_testapp_$(1)_CORE_DEPENDENCY = yes
export c7x_mma_error_injection_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export c7x_mma_error_injection_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
c7x_mma_error_injection_testapp_$(1)_PKG_LIST = c7x_mma_error_injection_testapp_$(1)
c7x_mma_error_injection_testapp_$(1)_INCLUDE = $(c7x_mma_error_injection_testapp_$(1)_PATH)
export c7x_mma_error_injection_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(c7x_mma_BOARDLIST))
export c7x_mma_error_injection_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(c7x_mma_CORELIST))
export c7x_mma_error_injection_testapp_$(1)_SBL_APPIMAGEGEN = yes
ip_fma_EXAMPLE_LIST += c7x_mma_error_injection_testapp_$(1)
endef

C7X_MMA_TESTAPP_MACRO_LIST := $(foreach curos, $(C7X_RTOS_LIST), $(call C7X_MMA_TESTAPP_RULE,$(curos)))

$(eval ${C7X_MMA_TESTAPP_MACRO_LIST})

# C7X CPU REGISTER READBACK EXAMPLE APP

C7X_CPU_RTOS_LIST = $(DEFAULT_RTOS_LIST)

c7x_cpu_register_SOCLIST   = j721e j721s2 j784s4 j742s2
c7x_cpu_register_BOARDLIST = j721e_evm j721s2_evm j784s4_evm j742s2_evm
c7x_cpu_register_CORELIST  = c7x_1

define C7X_CPU_REGISTER_READBACK_TESTAPP_RULE
export c7x_cpu_register_readback_testapp_$(1)_COMP_LIST = c7x_cpu_register_readback_testapp_$(1)
c7x_cpu_register_readback_testapp_$(1)_RELPATH = ti/ip_fma/examples/c7x_cpu_register_readback
c7x_cpu_register_readback_testapp_$(1)_PATH = $(PDK_INSTALL_PATH)/ti/ip_fma/examples/c7x_cpu_register_readback
export c7x_cpu_register_readback_testapp_$(1)_BOARD_DEPENDENCY = yes
export c7x_cpu_register_readback_testapp_$(1)_CORE_DEPENDENCY = yes
export c7x_cpu_register_readback_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export c7x_cpu_register_readback_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
c7x_cpu_register_readback_testapp_$(1)_PKG_LIST = c7x_cpu_register_readback_testapp_$(1)
c7x_cpu_register_readback_testapp_$(1)_INCLUDE = $(c7x_cpu_register_readback_testapp_$(1)_PATH)
export c7x_cpu_register_readback_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(c7x_cpu_register_BOARDLIST))
export c7x_cpu_register_readback_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(c7x_cpu_register_CORELIST))
export c7x_cpu_register_readback_testapp_$(1)_SBL_APPIMAGEGEN = yes
ip_fma_EXAMPLE_LIST += c7x_cpu_register_readback_testapp_$(1)
endef

C7X_CPU_REGISTER_READBACK_TESTAPP_MACRO_LIST := $(foreach curos, $(C7X_CPU_RTOS_LIST), $(call C7X_CPU_REGISTER_READBACK_TESTAPP_RULE,$(curos)))

$(eval ${C7X_CPU_REGISTER_READBACK_TESTAPP_MACRO_LIST})

# C7X SE ERROR INJECTION EXAMPLE APP

C7X_SE_RTOS_LIST = $(DEFAULT_RTOS_LIST)

c7x_se_SOCLIST   = j721e j721s2 j784s4 j742s2
c7x_se_BOARDLIST = j721e_evm j721s2_evm j784s4_evm j742s2_evm
c7x_se_CORELIST  = c7x_1

define C7X_SE_ERROR_INJECTION_TESTAPP_RULE
export c7x_se_error_injection_testapp_$(1)_COMP_LIST = c7x_se_error_injection_testapp_$(1)
c7x_se_error_injection_testapp_$(1)_RELPATH = ti/ip_fma/examples/c7x_se_error_injection
c7x_se_error_injection_testapp_$(1)_PATH = $(PDK_INSTALL_PATH)/ti/ip_fma/examples/c7x_se_error_injection
export c7x_se_error_injection_testapp_$(1)_BOARD_DEPENDENCY = yes
export c7x_se_error_injection_testapp_$(1)_CORE_DEPENDENCY = yes
export c7x_se_error_injection_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export c7x_se_error_injection_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
c7x_se_error_injection_testapp_$(1)_PKG_LIST = c7x_se_error_injection_testapp_$(1)
c7x_se_error_injection_testapp_$(1)_INCLUDE = $(c7x_se_error_injection_testapp_$(1)_PATH)
export c7x_se_error_injection_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(c7x_se_BOARDLIST))
export c7x_se_error_injection_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(c7x_se_CORELIST))
export c7x_se_error_injection_testapp_$(1)_SBL_APPIMAGEGEN = yes
ip_fma_EXAMPLE_LIST += c7x_se_error_injection_testapp_$(1)
endef

C7X_SE_ERROR_INJECTION_TESTAPP_MACRO_LIST := $(foreach curos, $(C7X_SE_RTOS_LIST), $(call C7X_SE_ERROR_INJECTION_TESTAPP_RULE,$(curos)))

$(eval ${C7X_SE_ERROR_INJECTION_TESTAPP_MACRO_LIST})

# C7X ILLEGAL INSTRUCTION EXAMPLE APP

C7X_ILLEGAL_INSTRUCTION_RTOS_LIST = $(DEFAULT_RTOS_LIST)

c7x_ILLEGAL_INSTRUCTION_SOCLIST   = j721e j721s2 j784s4 j742s2
c7x_ILLEGAL_INSTRUCTION_BOARDLIST = j721e_evm j721s2_evm j784s4_evm j742s2_evm
c7x_ILLEGAL_INSTRUCTION_CORELIST  = c7x_1

define C7X_ILLEGAL_INSTRUCTION_TESTAPP_RULE
export c7x_illegal_instruction_testapp_$(1)_COMP_LIST = c7x_illegal_instruction_testapp_$(1)
c7x_illegal_instruction_testapp_$(1)_RELPATH = ti/ip_fma/examples/c7x_illegal_instruction
c7x_illegal_instruction_testapp_$(1)_PATH = $(PDK_INSTALL_PATH)/ti/ip_fma/examples/c7x_illegal_instruction
export c7x_illegal_instruction_testapp_$(1)_BOARD_DEPENDENCY = yes
export c7x_illegal_instruction_testapp_$(1)_CORE_DEPENDENCY = yes
export c7x_illegal_instruction_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export c7x_illegal_instruction_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
c7x_illegal_instruction_testapp_$(1)_PKG_LIST = c7x_illegal_instruction_testapp_$(1)
c7x_illegal_instruction_testapp_$(1)_INCLUDE = $(c7x_illegal_instruction_testapp_$(1)_PATH)
export c7x_illegal_instruction_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(c7x_ILLEGAL_INSTRUCTION_BOARDLIST))
export c7x_illegal_instruction_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(c7x_ILLEGAL_INSTRUCTION_CORELIST))
export c7x_illegal_instruction_testapp_$(1)_SBL_APPIMAGEGEN = yes
ip_fma_EXAMPLE_LIST += c7x_illegal_instruction_testapp_$(1)
endef

C7X_ILLEGAL_INSTRUCTION_TESTAPP_MACRO_LIST := $(foreach curos, $(C7X_ILLEGAL_INSTRUCTION_RTOS_LIST), $(call C7X_ILLEGAL_INSTRUCTION_TESTAPP_RULE,$(curos)))

$(eval ${C7X_ILLEGAL_INSTRUCTION_TESTAPP_MACRO_LIST})

# C7X MMU SAFETY EXAMPLE APP

C7X_RTOS_LIST       = $(DEFAULT_RTOS_LIST)

c7x_mmu_SOCLIST   = j721e j721s2 j784s4 j742s2
c7x_mmu_BOARDLIST = j721e_evm j721s2_evm j784s4_evm j742s2_evm
c7x_mmu_CORELIST  = c7x_1


define C7X_MMU_TESTAPP_RULE
export c7x_mmu_error_injection_testapp_$(1)_COMP_LIST = c7x_mmu_error_injection_testapp_$(1)
c7x_mmu_error_injection_testapp_$(1)_RELPATH = ti/ip_fma/examples/c7x_mmu_error_injection
c7x_mmu_error_injection_testapp_$(1)_PATH = $(PDK_INSTALL_PATH)/ti/ip_fma/examples/c7x_mmu_error_injection
export c7x_mmu_error_injection_testapp_$(1)_BOARD_DEPENDENCY = yes
export c7x_mmu_error_injection_testapp_$(1)_CORE_DEPENDENCY = yes
export c7x_mmu_error_injection_testapp_$(1)_XDC_CONFIGURO = $(if $(findstring tirtos, $(1)), yes, no)
export c7x_mmu_error_injection_testapp_$(1)_MAKEFILE = -f makefile BUILD_OS_TYPE=$(1)
c7x_mmu_error_injection_testapp_$(1)_PKG_LIST = c7x_mmu_error_injection_testapp_$(1)
c7x_mmu_error_injection_testapp_$(1)_INCLUDE = $(c7x_mmu_error_injection_testapp_$(1)_PATH)
export c7x_mmu_error_injection_testapp_$(1)_BOARDLIST = $(filter $(DEFAULT_BOARDLIST_$(1)), $(c7x_mmu_BOARDLIST))
export c7x_mmu_error_injection_testapp_$(1)_$(SOC)_CORELIST = $(filter $(DEFAULT_$(SOC)_CORELIST_$(1)), $(c7x_mmu_CORELIST))
export c7x_mmu_error_injection_testapp_$(1)_SBL_APPIMAGEGEN = yes
ip_fma_EXAMPLE_LIST += c7x_mmu_error_injection_testapp_$(1)
endef

C7X_MMU_TESTAPP_MACRO_LIST := $(foreach curos, $(C7X_RTOS_LIST), $(call C7X_MMU_TESTAPP_RULE,$(curos)))

$(eval ${C7X_MMU_TESTAPP_MACRO_LIST})

# CSIRX IP FMA APP

ip_fma_csirx_SOCLIST         = j784s4 j721s2
ip_fma_csirx_BOARDLIST       = j784s4_evm j721s2_evm
ip_fma_csirx_CORELIST        = mcu2_0

define CSIRX_TESTAPP_RULE

csirx_ip_fma_app_$(1)_COMP_LIST = csirx_ip_fma_app_$(1)
csirx_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/csirx_ip_fma_app
csirx_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/csirx_ip_fma_app
csirx_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export csirx_ip_fma_app_$(1)_MAKEFILE
csirx_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
csirx_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
csirx_ip_fma_app_$(1)_BOARD_DEPENDENCY = yes
export csirx_ip_fma_app_$(1)_COMP_LIST
export csirx_ip_fma_app_$(1)_SOC_DEPENDENCY
export csirx_ip_fma_app_$(1)_CORE_DEPENDENCY
export csirx_ip_fma_app_$(1)_BOARD_DEPENDENCY
csirx_ip_fma_app_$(1)_PKG_LIST = csirx_ip_fma_app_$(1)
csirx_ip_fma_app_$(1)_INCLUDE  = $(csirx_ip_fma_app_$(1)_PATH)
csirx_ip_fma_app_$(1)_INCLUDE  += \
                                       $(PDK_INSTALL_PATH)/ti/csl \
                                       $(PDK_INSTALL_PATH)/ti/drv/fvid2 \
                                       $(PDK_INSTALL_PATH)/ti/drv/csirx
csirx_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_csirx_BOARDLIST)
export csirx_ip_fma_app_$(1)_BOARDLIST
csirx_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_csirx_CORELIST)
export csirx_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += csirx_ip_fma_app_$(1)
csirx_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export csirx_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

CSIRX_MACRO_LIST := $(foreach curos, baremetal, $(call CSIRX_TESTAPP_RULE,$(curos)))

$(eval ${CSIRX_MACRO_LIST})

# DRU IP FMA APP
ip_fma_dru_SOCLIST         = j784s4 j721s2
ip_fma_dru_BOARDLIST       = j784s4_evm j721s2_evm
ip_fma_dru_CORELIST        = mcu2_0

define DRU_TESTAPP_RULE

dru_ip_fma_app_$(1)_COMP_LIST = dru_ip_fma_app_$(1)
dru_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/dru_ip_fma_app
dru_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/dru_ip_fma_app
dru_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export dru_ip_fma_app_$(1)_MAKEFILE
dru_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
dru_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
dru_ip_fma_app_$(1)_BOARD_DEPENDENCY  = yes
export dru_ip_fma_app_$(1)_COMP_LIST
export dru_ip_fma_app_$(1)_SOC_DEPENDENCY
export dru_ip_fma_app_$(1)_CORE_DEPENDENCY
export dru_ip_fma_app_$(1)_BOARD_DEPENDENCY
dru_ip_fma_app_$(1)_PKG_LIST = dru_ip_fma_app_$(1)
dru_ip_fma_app_$(1)_INCLUDE  = $(dru_ip_fma_app_$(1)_PATH)
dru_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_dru_BOARDLIST)
export dru_ip_fma_app_$(1)_BOARDLIST
dru_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_dru_CORELIST)
export dru_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += dru_ip_fma_app_$(1)
dru_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export dru_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

DRU_MACRO_LIST := $(foreach curos, freertos, $(call DRU_TESTAPP_RULE,$(curos)))

$(eval ${DRU_MACRO_LIST})


# DDR IP FMA APP

ip_fma_ddr_SOCLIST         = j784s4 j721s2
ip_fma_ddr_BOARDLIST       = j784s4_evm j721s2_evm
ip_fma_ddr_CORELIST        = mcu1_0

define DDR_TESTAPP_RULE
ddr_ip_fma_app_$(1)_COMP_LIST = ddr_ip_fma_app_$(1)
ddr_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/ddr_ip_fma_app
ddr_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/ddr_ip_fma_app
ddr_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export ddr_ip_fma_app_$(1)_MAKEFILE
ddr_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
ddr_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
ddr_ip_fma_app_$(1)_BOARD_DEPENDENCY = yes
export ddr_ip_fma_app_$(1)_COMP_LIST
export ddr_ip_fma_app_$(1)_SOC_DEPENDENCY
export ddr_ip_fma_app_$(1)_CORE_DEPENDENCY
export ddr_ip_fma_app_$(1)_BOARD_DEPENDENCY
ddr_ip_fma_app_$(1)_PKG_LIST = ddr_ip_fma_app_$(1)
ddr_ip_fma_app_$(1)_INCLUDE  = $(ddr_ip_fma_app_$(1)_PATH)
ddr_ip_fma_app_$(1)_INCLUDE  += \
                                        $(PDK_INSTALL_PATH)/ti/csl \
								        $(PDK_INSTALL_PATH)/ti/drv/fvid2 \
								        $(PDK_INSTALL_PATH)/ti/drv/ddr
ddr_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_ddr_BOARDLIST)
export ddr_ip_fma_app_$(1)_BOARDLIST
ddr_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_ddr_CORELIST)
export ddr_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += ddr_ip_fma_app_$(1)
ddr_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export ddr_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

DDR_MACRO_LIST := $(foreach curos, baremetal, $(call DDR_TESTAPP_RULE,$(curos)))

$(eval $(DDR_MACRO_LIST))

# CLK IP FMA APP

ip_fma_clk_SOCLIST           = j784s4 j721s2
ip_fma_clk_BOARDLIST         = j784s4_evm j721s2_evm
ip_fma_clk_CORELIST          = mcu1_0

define CLK_TESTAPP_RULE

clk_ip_fma_app_$(1)_COMP_LIST  = clk_ip_fma_app_$(1)
clk_ip_fma_app_$(1)_RELPATH    = ti/ip_fma/examples/clk_ip_fma_app
clk_ip_fma_app_$(1)_PATH       = $(IP_FMA_COMP_PATH)/examples/clk_ip_fma_app
clk_ip_fma_app_$(1)_MAKEFILE   = -fmakefile BUILD_OS_TYPE=$(1)
export clk_ip_fma_app_$(1)_MAKEFILE
clk_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
clk_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
clk_ip_fma_app_$(1)_BOARD_DEPENDENCY  = yes
export clk_ip_fma_app_$(1)_COMP_LIST
export clk_ip_fma_app_$(1)_SOC_DEPENDENCY
export clk_ip_fma_app_$(1)_CORE_DEPENDENCY
export clk_ip_fma_app_$(1)_BOARD_DEPENDENCY
clk_ip_fma_app_$(1)_PKG_LIST  = clk_ip_fma_app_$(1)
clk_ip_fma_app_$(1)_INCLUDE   = $(clk_ip_fma_app_$(1)_PATH)
clk_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_clk_BOARDLIST)
export clk_ip_fma_app_$(1)_BOARDLIST
clk_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_clk_CORELIST)
export clk_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += clk_ip_fma_app_$(1)
clk_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export clk_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

CLK_MACRO_LIST := $(foreach curos, baremetal, $(call CLK_TESTAPP_RULE,$(curos)))

$(eval ${CLK_MACRO_LIST})


# MSMC IP FMA APP
ip_fma_msmc_SOCLIST         = j784s4 j721s2
ip_fma_msmc_BOARDLIST       = j784s4_evm j721s2_evm
ip_fma_msmc_CORELIST        = mcu1_0 mcu1_1

define MSMC_TESTAPP_RULE

msmc_ip_fma_app_$(1)_COMP_LIST = msmc_ip_fma_app_$(1)
msmc_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/msmc_ip_fma_app
msmc_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/msmc_ip_fma_app
msmc_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export msmc_ip_fma_app_$(1)_MAKEFILE
msmc_ip_fma_app_$(1)_SOC_DEPENDENCY   = yes
msmc_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
msmc_ip_fma_app_$(1)_BOARD_DEPENDENCY = yes
export msmc_ip_fma_app_$(1)_COMP_LIST
export msmc_ip_fma_app_$(1)_SOC_DEPENDENCY
export msmc_ip_fma_app_$(1)_CORE_DEPENDENCY
export msmc_ip_fma_app_$(1)_BOARD_DEPENDENCY
msmc_ip_fma_app_$(1)_PKG_LIST  = msmc_ip_fma_app_$(1)
msmc_ip_fma_app_$(1)_INCLUDE   = $(msmc_ip_fma_app_$(1)_PATH)
msmc_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_msmc_BOARDLIST)
export msmc_ip_fma_app_$(1)_BOARDLIST
msmc_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_msmc_CORELIST)
export msmc_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += msmc_ip_fma_app_$(1)
msmc_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export msmc_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

MSMC_MACRO_LIST := $(foreach curos, baremetal, $(call MSMC_TESTAPP_RULE,$(curos)))

$(eval ${MSMC_MACRO_LIST})


# DSS IP FMA APP
ip_fma_dss_SOCLIST          = j784s4 j721s2
ip_fma_dss_BOARDLIST        = j784s4_evm j721s2_evm
ip_fma_dss_CORELIST	 		= mcu2_0

define DSS_TESTAPP_RULE

dss_ip_fma_app_$(1)_COMP_LIST = dss_ip_fma_app_$(1)
dss_ip_fma_app_$(1)_RELPATH   = ti/ip_fma/examples/dss_ip_fma_app
dss_ip_fma_app_$(1)_PATH      = $(IP_FMA_COMP_PATH)/examples/dss_ip_fma_app
dss_ip_fma_app_$(1)_MAKEFILE  = -fmakefile BUILD_OS_TYPE=$(1)
export dss_ip_fma_app_$(1)_MAKEFILE
dss_ip_fma_app_$(1)_SOC_DEPENDENCY = yes
dss_ip_fma_app_$(1)_CORE_DEPENDENCY  = yes
export dss_ip_fma_app_$(1)_COMP_LIST
export dss_ip_fma_app_$(1)_SOC_DEPENDENCY
export dss_ip_fma_app_$(1)_CORE_DEPENDENCY
dss_ip_fma_app_$(1)_PKG_LIST = dss_ip_fma_app_$(1)
dss_ip_fma_app_$(1)_INCLUDE  = $(dss_ip_fma_app_$(1)_PATH)
dss_ip_fma_app_$(1)_BOARDLIST = $(ip_fma_dss_BOARDLIST)
export dss_ip_fma_app_$(1)_BOARDLIST
dss_ip_fma_app_$(1)_$(SOC)_CORELIST = $(ip_fma_dss_CORELIST)
export dss_ip_fma_app_$(1)_$(SOC)_CORELIST
ip_fma_EXAMPLE_LIST += dss_ip_fma_app_$(1)
dss_ip_fma_app_$(1)_SBL_APPIMAGEGEN = yes
export dss_ip_fma_app_$(1)_SBL_APPIMAGEGEN
endef

DSS_MACRO_LIST := $(foreach curos, baremetal, $(call DSS_TESTAPP_RULE,$(curos)))

$(eval ${DSS_MACRO_LIST})


export ip_fma_EXAMPLE_LIST

ip_fma_component_make_include := 1
