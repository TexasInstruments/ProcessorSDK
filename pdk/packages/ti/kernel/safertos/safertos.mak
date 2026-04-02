#
# This file is the makefile for building safertos.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = safertos

BUILD_OS_TYPE = safertos

SRCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/kernel
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))/$(SAFERTOS_COMPILER_EXT_$(ISA))
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/api/${SAFERTOS_ISA_EXT_$(ISA)}
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/runtimestats
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/runtimestats/PrivWrapperStd
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
SRCDIR += $(PDK_SAFERTOS_COMP_PATH)/TI_CGT/c7x
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/checkpoints
endif

INCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/checkpoints
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT_$(ISA))
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/config
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/kernel/include_prv
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/kernel/include_api
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_$(ISA))/$(SAFERTOS_COMPILER_EXT_$(ISA))
ifeq ($(ISA),$(filter $(ISA), r5f))
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/runtimestats
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/runtimestats/PrivWrapperStd
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/SafeRTOS/api/NoWrapper
INCDIR += $(PDK_SAFERTOS_COMP_PATH)/TI_CGT/c7x
INCDIR += $(PDK_CSL_COMP_PATH)/arch/c7x
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += \
    timers.c \
    evtmplx.c \
    mutex.c \
    streambuffer.c \
    queue.c \
    eventgroups.c \
    semaphore.c \
    list.c

# SafeRTOS task and portable does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    task.c \
    portable.c
endif

#ISA specific C files
ifeq ($(ISA),$(filter $(ISA), r5f c66))
SRCS_COMMON += \
    apiSemaphoreWrapper.c \
    apiTimersWrapper.c \
    apiMutexWrapper.c \
    apiWrapper.c \
    apiEventGroupsWrapper.c \
    apiStreamBufferWrapper.c \
    apiEvtMplxWrapper.c 
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    mpuARM.c \
    portmpu.c \
    apiMPU.c

endif

#Runtime stats C files
ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_COMMON += \
    runtimestats.c \
    apiRuntimestatsWrapper.c
endif

ifeq ($(ISA),$(filter $(ISA), c7x))
# Following does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    Hwi_vector_table.c \
    c7x_module_config_mmu_init.c

endif
endif

#ISA specific assembly files
ifeq ($(ISA),$(filter $(ISA), c66))
SRCS_ASM_COMMON := \
	portasm.asm
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
SRCS_S_COMMON := \
	portasm.S
endif

ifeq ($(ISA),$(filter $(ISA), c7x))
SRCS_ASM_COMMON := \
    portasm.asm 
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
ifeq ($(ISA),$(filter $(ISA), c7x))
CFLAGS_LOCAL_COMMON += -DHwi_bootToNonSecure__D=true
CFLAGS_LOCAL_COMMON += -DException_vectors__D
endif

PACKAGE_SRCS_COMMON = safertos.mak safertos_component.mk
PACKAGE_SRCS_COMMON += ../test/safertos/task_switch
PACKAGE_SRCS_COMMON += ../test/safertos/ut
PACKAGE_SRCS_COMMON += ../test/safertos/common
PACKAGE_SRCS_COMMON += ../safertos/TI_CGT/c7x

#include posix support files if present
-include safertos_posix.mak

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
