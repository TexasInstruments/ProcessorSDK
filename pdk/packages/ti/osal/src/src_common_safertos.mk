
SRCDIR = . src/safertos src/nonos arch/core src/freertos
INCDIR = . src/safertos src/nonos arch/core src/freertos

SRCS_COMMON += SwiP_nonos.c DebugP_nonos.c RegisterIntr_nonos.c Core_utils.c CycleprofilerP_nonos.c
SRCS_COMMON += SemaphoreP_SafeRTOS.c TaskP_SafeRTOS.c Utils_SafeRTOS.c SafeRTOS_config.c StartuphooksP_safertos.c
SRCS_COMMON += EventP_safertos.c MailboxP_safertos.c ClockP_safertos.c MutexP_safertos.c
SRCS_COMMON += HeapP_freertos.c HeapP_freertos_internal.c QueueP_nonos.c TimerP_restrictedTimers.c

PACKAGE_SRCS_COMMON = makefile HwiP.h SwiP.h MuxIntcP.h osal.h osal_component.mk SemaphoreP.h StartuphooksP.h \
                      TimerP.h RegisterIntr.h EventCombinerP.h CycleprofilerP.h CacheP.h DebugP.h soc/osal_soc.h soc/$(SOC) \
                      src/nonos/Nonos_config.h src/nonos/SwiP_nonos.c src/nonos/CycleprofilerP_nonos.c \
                      src/nonos/RegisterIntr_nonos.c src/safertos/SemaphoreP_SafeRTOS.c src/safertos/TaskP_SafeRTOS.c src/safertos/Utils_SafeRTOS.c \
                      src/safertos/SafeRTOS_config.c src/safertos/EventP_safertos.c src/safertos/StartuphooksP_safertos.c \
                      src/safertos/MailboxP_safertos.c src/safertos/ClockP_safertos.c SafeRTOS_MPU.h \
                      src/safertos/MutexP_safertos.c src/freertos/HeapP_freertos.c src/freertos/HeapP_freertos_internal.c \
                      src/nonos/QueueP_nonos.c src/freertos/HeapP_freertos_internal.h src/nonos/DebugP_nonos.c TimerP_restrictedTimers.c \
                      src/src_common_safertos.mk \
                      arch/core/Core_utils.c docs/OSAL_SoftwareManifest.html docs/ReleaseNotes_OSAL.pdf \
                      build src/safertos/SafeRTOS_priv.h \
                      osalver.h \
                      Safertos_Aborts.h SafeRTOS_config.h

ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4))
  SRCDIR      += soc/$(SOC)
  SRCS_COMMON += TimerP_default.c
endif

ifeq ($(SOC),$(filter $(SOC), j7200))
  SRCDIR      += soc/$(SOC)
  SRCS_COMMON += TimerP_default_$(ISA).c
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1 mcu1_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
  SRCDIR += arch/core/r5
  SRCS_COMMON += CacheP_nonos_r5.c Arch_util_r5.c SafeRTOS_aborts_r5f.c SafeRTOS_mpu_r5f.c HwiP_safertos.c
  SRCS_COMMON += SafeRTOS_config_r5f.c LoadP_safertos.c
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
  SRCS_ASM_COMMON += SafeRTOS_utils_r5f.asm
  PACKAGE_SRCS_COMMON += arch/core/r5 src/safertos/SafeRTOS_aborts_r5f.c src/safertos/SafeRTOS_mpu_r5f.c
  PACKAGE_SRCS_COMMON += src/safertos/SafeRTOS_config_r5f.c src/safertos/LoadP_safertos.c src/safertos/HwiP_safertos.c
  PACKAGE_SRCS_COMMON += src/safertos/SafeRTOS_utils_r5f.asm
endif

ifeq ($(CORE),$(filter $(CORE), c66x c66xdsp_1 c66xdsp_2 c674x))
  SRCDIR += arch/core/c6x
  SRCS_COMMON += CacheP_nonos_c6x.c Arch_util_c6x.c EventCombinerP_nonos.c HwiP_safertos.c
  SRCS_COMMON += SafeRTOS_config_c66.c
  PACKAGE_SRCS_COMMON += arch/core/c6x src/nonos/EventCombinerP_nonos.c src/safertos/HwiP_safertos.c 
  PACKAGE_SRCS_COMMON += src/safertos/SafeRTOS_config_c66.c
endif

ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x_2 c7x_3 c7x_4))
  SRCDIR += arch/core/c7x
  SRCS_COMMON += Arch_util_c7x.c CacheP_nonos_c7x.c HwiP_safertos_c7x.c SafeRTOS_config_c7x.c
  PACKAGE_SRCS_COMMON += arch/core/c7x/CacheP_nonos_c7x.c src/safertos/HwiP_safertos_c7x.c arch/core/c7x/Arch_util_c7x.c src/safertos/SafeRTOS_config_c7x.c
endif
