
SRCDIR = . src/nonos arch/core
INCDIR = . src/nonos arch/core

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCS_COMMON += HwiP_nonos.c SwiP_nonos.c Utils_nonos.c SemaphoreP_nonos.c DebugP_nonos.c RegisterIntr_nonos.c Core_utils.c MutexP_nonos.c TimerP_restrictedTimers.c StartuphooksP_nonos.c

PACKAGE_SRCS_COMMON = makefile HwiP.h SwiP.h MuxIntcP.h osal.h osal_component.mk SemaphoreP.h StartuphooksP.h \
                      TimerP.h RegisterIntr.h EventCombinerP.h CycleprofilerP.h CacheP.h DebugP.h soc/osal_soc.h soc/$(SOC_DIR) \
                      src/nonos/Nonos_config.h src/nonos/HwiP_nonos.c src/nonos/SwiP_nonos.c src/nonos/CycleprofilerP_nonos.c src/nonos/StartuphooksP_nonos.c \
                      src/nonos/RegisterIntr_nonos.c src/nonos/SemaphoreP_nonos.c src/nonos/Utils_nonos.c src/nonos/DebugP_nonos.c \
                      src/nonos/MutexP_nonos.c TimerP_restrictedTimers.c src/src_common_nonos.mk \
                      arch/core/Core_utils.c docs/OSAL_SoftwareManifest.html docs/ReleaseNotes_OSAL.pdf \
                      build \
                      osalver.h

ifeq ($(SOC),$(filter $(SOC),j721e j721s2 j784s4 j742s2))
  SRCDIR      += soc/$(SOC_DIR)
  SRCS_COMMON += TimerP_default.c CycleprofilerP_nonos.c
endif

ifeq ($(SOC),$(filter $(SOC), j7200))
  SRCDIR      += soc/$(SOC_DIR)
  SRCS_COMMON += TimerP_default_$(ISA).c CycleprofilerP_nonos.c
endif

ifeq ($(CORE),mpu1_0)
  SRCDIR += arch/core/a53
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
  PACKAGE_SRCS_COMMON += arch/core/a53
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1 mcu1_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
  SRCDIR += arch/core/r5
  SRCS_COMMON += CacheP_nonos_r5.c Arch_util_r5.c
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
  PACKAGE_SRCS_COMMON += arch/core/r5
endif

ifeq ($(CORE),$(filter $(CORE), c66x c66xdsp_1 c66xdsp_2 c674x))
  SRCDIR += arch/core/c6x
  SRCS_COMMON += CacheP_nonos_c6x.c Arch_util_c6x.c EventCombinerP_nonos.c
  PACKAGE_SRCS_COMMON += arch/core/c6x src/nonos/EventCombinerP_nonos.c
endif

ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x_2 c7x_3 c7x_4 c7x-hostemu))
  SRCDIR += arch/core/c7x
  SRCS_COMMON += CacheP_nonos_c7x.c Arch_util_c7x.c
  PACKAGE_SRCS_COMMON += arch/core/c7x
endif


