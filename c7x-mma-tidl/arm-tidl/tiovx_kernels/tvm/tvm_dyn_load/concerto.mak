ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C71 C7120 C7504 C7524))

include $(PRELUDE)
TARGET      := vx_target_kernels_tvm_dynload
TARGETTYPE  := library

CSOURCES    := dsp_load.c
CSOURCES    += DLOAD/ArrayList.c
CSOURCES    += DLOAD/dload.c
CSOURCES    += DLOAD/dload_endian.c
CSOURCES    += DLOAD/elf32.c
CSOURCES    += DLOAD_SYM/symtab.c
CSOURCES    += C70_DLOAD_DYN/c70_dynamic.c
CSOURCES    += C70_DLOAD_REL/c70_reloc.c

CPPSOURCES  += cpp_symbols.cpp

IDIRS       += $(SDIR)/DLOAD
IDIRS       += $(SDIR)/DLOAD_API
IDIRS       += $(SDIR)/C70_DLOAD_DYN
IDIRS       += $(SDIR)/C70_DLOAD_REL
IDIRS       += $(APP_UTILS_PATH)

ifeq ($(RTOS_SDK), mcu_plus_sdk)
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source
  DEFS += MCU_PLUS_SDK
else
  IDIRS       += $(PDK_PATH)
endif

CFLAGS      += -DDEVICE_J721E -D_SYS_BIOS -DPSDK_RTOS_APP -DC70_TARGET -DELF64=1

include $(FINALE)

endif
