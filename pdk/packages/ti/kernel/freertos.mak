#
# This file is the makefile for building freertos.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = freertos

SRCDIR = freertos/portable/TI_CGT/$(ISA)
SRCDIR += ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/ \
          ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/portable/MemMang \

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

INCDIR = freertos/config/$(SOC_DIR)/$(ISA)
INCDIR += ${FREERTOS_KERNEL_INSTALL_PATH}/FreeRTOS-Kernel/include \
          freertos/portable/TI_CGT/$(ISA) \
          $(PDK_CSL_COMP_PATH)/arch/$(ISA)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
ifeq ($(ISA),$(filter $(ISA), c7x))
SRCS_COMMON += \
            Hwi_vector_table.c                         \
            c7x_module_config_mmu_init.c               \
            TaskSupport.c                              \
            TimestampProvider.c                        \
            port_c7x.c

endif

SRCS_COMMON += \
    timers.c \
    queue.c \
    list.c

# FreeRTOS tasks, event_groups does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    tasks.c \
    event_groups.c \
    heap_4.c
endif

#ISA specific C files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_COMMON += \
    port_Hwi_c66.c \
    port_c66.c
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    port_Hwi_r5f.c \
    port_r5f.c
endif

#ISA specific assembly files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_ASM_COMMON := \
    port_Hwi_asm_switch.asm \
    port_Hwi_disp_always.asm \
    port_Hwi_intcIsrDispatch.asm \
    port_TaskSupport_asm.asm
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_ASM_COMMON := \
    portASM.asm \
    port_Hwi_handlers_asm.asm
endif

ifeq ($(ISA),$(filter $(ISA), c7x))
SRCS_ASM_COMMON := \
    TaskSupport_asm.asm
endif



CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

PACKAGE_SRCS_COMMON = freertos.mak freertos_component.mk makefile
PACKAGE_SRCS_COMMON += freertos
PACKAGE_SRCS_COMMON += test/freertos/task_switch
PACKAGE_SRCS_COMMON += test/freertos/ut
PACKAGE_SRCS_COMMON += test/freertos/build


# Processor SDK is using FreeRTOS V10.5.1. POSIX builds generate builds errors with FreeRTOS V10.5.1
# Include posix support files if present
# -include freertos_posix.mak

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
