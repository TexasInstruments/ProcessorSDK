#
# This file is the makefile for building mailbox LLD perf app.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = mailbox_perf_testapp
SRCDIR = .
INCDIR =

PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = xdc pdk bios

# List all the components required by the application
ifeq ($(BOARD),$(filter $(BOARD), am64x_evm))
  COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)
else
  COMP_LIST_COMMON += csl 
endif
COMP_LIST_COMMON += csl_utils_common csl_uart_console mailbox
ifeq ($(BOARD),$(filter $(BOARD), am64x_evm))
  ifeq ($(ISA),$(filter $(ISA), r5f))
    XDC_CFG_FILE_$(CORE) = $(SOC)/sysbios_$(ISA).cfg
  else
    XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
  endif
else
XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif

ifeq ($(SOC),$(filter $(SOC), am64x))
SRCS_COMMON = mailbox_perf_main.c mailbox_perf_app.c
SRCDIR += ./$(SOC)
INCDIR += ./$(SOC)
endif

ifeq ($(SOC),$(filter $(SOC), am64x))
EXTERNAL_LNKCMD_FILE_LOCAL = $(SOC)/linker_$(CORE)_sysbios.lds
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# RTOS Interrupt mode
ifeq ($(ISA),$(filter $(ISA), r5f a53))
CFLAGS_LOCAL_COMMON += -DMEASURE_LATENCY_DISTRIBUTION
endif

# RTOS Interrupt mode with detailed profiling
#ifeq ($(ISA),$(filter $(ISA), r5f a53))
#CFLAGS_LOCAL_COMMON += -DMEASURE_LATENCY_DISTRIBUTION -DDETAILED_PROFILING
#endif

ifeq ($(CORE),mpu1_0)
CFLAGS_LOCAL_COMMON += -DSYSTEM_MASTER
endif

CFLAGS_LOCAL_COMMON += -DBUILD_RTOS

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
