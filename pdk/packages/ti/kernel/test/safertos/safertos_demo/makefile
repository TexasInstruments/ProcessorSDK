#
# This file is the makefile for building SafeRTOS Task Switch Example App
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif


APP_NAME = safertos_demo
BUILD_OS_TYPE=safertos
CFLAGS_TEST_DEFINES =
SAFERTOS_DEMO_EXT_r5f=SafeRTOS_TDA4VM_R5_Demo
SAFERTOS_DEMO_EXT_c66=SafeRTOS_TDA4VM_C66x_Demo
SAFERTOS_DEMO_EXT_c7x=SafeRTOS_TDA4VM_C7x_Demo

SRCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/$(SAFERTOS_DEMO_EXT_$(ISA))/Common/
SRCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/$(SAFERTOS_DEMO_EXT_$(ISA))/source/

INCDIR = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/$(SAFERTOS_DEMO_EXT_$(ISA))/Common/include/
INCDIR += ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/$(SAFERTOS_DEMO_EXT_$(ISA))/include/


# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_EXTERNAL_INTERFACES += safertos
ifeq ($(BUILD_OS_TYPE), safertos)
COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
else
# List all the components required by the application
endif

ifeq ($(ISA),$(filter $(ISA), r5f))
  EXTERNAL_LNKCMD_FILE_LOCAL = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_R5_Demo/linker_core_r5.lds
endif
ifeq ($(ISA),$(filter $(ISA), c66))
  EXTERNAL_LNKCMD_FILE_LOCAL = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_C66x_Demo/linker_c66_safertos.cmd
endif
ifeq ($(ISA),$(filter $(ISA), c7x))
  EXTERNAL_LNKCMD_FILE_LOCAL = ${SAFERTOS_KERNEL_INSTALL_PATH_$(ISA)}/source_code_and_projects/demo_projects/SafeRTOS_TDA4VM_C7x_Demo/linker_c7x_safertos.cmd
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON = \
  TaskCreateDeleteDemo.c \
  PollQ.c \
  StreamBufferDemo.c \
  countsem.c \
  semtest.c \
  dynamic.c \
  TaskNotify.c \
  TaskMutex.c \
  EvtMplxDemo.c \
  TimerDemo.c \
  BlockQ.c \
  blocktim.c \
  flash.c \
  comtest.c \
  MathsTest.c \
  PortSpecifics.c \
  FullDemo.c \
  SafeRTOSConfig.c \
  partest.c \
  main.c \
  interruptHooks.c \
  serial.c \
  aborts.c \
  countsemTimer.c

ifeq ($(ISA),$(filter $(ISA), r5f))
  SRCS_S_COMMON += utils_asm.S
endif
ifeq ($(ISA),$(filter $(ISA), c66))
  SRCS_ASM_COMMON += demoasm.asm
endif

ifeq ($(ISA),$(filter $(ISA), c7x))
  SRCS_COMMON += \
    freeRTOS_stubs.c \
    initialiseMMU.c
  SRCS_ASM_COMMON += demoasm.asm
endif


CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_TEST_DEFINES)

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
