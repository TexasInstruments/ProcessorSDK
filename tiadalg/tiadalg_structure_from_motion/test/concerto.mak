#ifeq ($(TARGET_PLATFORM), J7_CCS) || ifeq ($(TARGET_PLATFORM), PC)

#ifeq ($(TARGET_PLATFORM),$(filter $(TARGET_PLATFORM),J7_CCS PC))
include $(PRELUDE)

ifneq ($(TARGET_PLATFORM), PC)
TARGET      := TI_DEVICE_$(KERNEL_NAME)
else
TARGET      := PC_$(KERNEL_NAME)
endif

TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

# DSP build needs linker command files for memory maps
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/c7x_linker_freertos.cmd

# DSP build needs CGT, BIOS, and XDC include files
ifeq ($(TARGET_PLATFORM), PC)
IDIRS += $(CGT7X_ROOT)/host_emulation/include/C7100
else
IDIRS += $(CGT7X_ROOT)/include
IDIRS += $(PDK_INSTALL_PATH)/packages
endif

IDIRS += $(ALGBASE_PATH)/$(KERNEL_NAME)/inc
IDIRS += $(abspath $(ALGBASE_PATH)/common/)
IDIRS += $(abspath $(ALGBASE_PATH)/include/)
IDIRS += $(IVISION_PATH)

# path to algorithm library
ifeq ($(TARGET_PLATFORM), PC)
LDIRS += $(CGT7X_ROOT)/host_emulation
#LDIRS += $(PDK_INSTALL_PATH)/ti/csl/lib/j721e/c7x-hostemu/$(TARGET_BUILD)
#LDIRS += $(PDK_INSTALL_PATH)/ti/osal/lib/nonos/j721e/c7x-hostemu/$(TARGET_BUILD)
#LDIRS += $(PDK_INSTALL_PATH)/ti/drv/sciclient/lib/j721e_hostemu/c7x-hostemu/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/j721e_hostemu/c7x-hostemu/$(TARGET_BUILD)
else
LDIRS += $(CGT7X_ROOT)/lib
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/csl/lib/$(SOC)/c7x/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/osal/lib/freertos/$(SOC)/c7x/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/drv/sciclient/lib/$(SOC)/c7x_$(C7X_CORE_ID)/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/$(SOC)/c7x_$(C7X_CORE_ID)/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/drv/i2c/lib/$(SOC)/c7x/$(TARGET_BUILD)/
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/kernel/lib/$(SOC)/c7x_$(C7X_CORE_ID)/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/board/lib/$(SOC)_evm/c7x/$(TARGET_BUILD)
LDIRS += $(PDK_INSTALL_PATH)/packages/ti/drv/uart/lib/$(SOC)/c7x/$(TARGET_BUILD)
endif

LDIRS += $(ALGBASE_PATH)/lib/$(TARGET_CPU)/$(TARGET_BUILD)

# External libraries
ifeq ($(TARGET_PLATFORM), PC)
ADDITIONAL_STATIC_LIBS += dmautils.lib
ADDITIONAL_STATIC_LIBS += udma.lib
#ADDITIONAL_STATIC_LIBS += sciclient.lib
#ADDITIONAL_STATIC_LIBS += ti.csl.lib
#ADDITIONAL_STATIC_LIBS += ti.osal.lib
#ADDITIONAL_STATIC_LIBS += ti.kernel.freertos.lib
else
ADDITIONAL_STATIC_LIBS += ti.kernel.freertos.ae71
ADDITIONAL_STATIC_LIBS += ti.drv.uart.ae71
ADDITIONAL_STATIC_LIBS += ti.drv.i2c.ae71
ADDITIONAL_STATIC_LIBS += ti.board.ae71
ADDITIONAL_STATIC_LIBS += ti.csl.ae71
ADDITIONAL_STATIC_LIBS += ti.osal.ae71
ADDITIONAL_STATIC_LIBS += sciclient.ae71
ADDITIONAL_STATIC_LIBS += udma.ae71
ADDITIONAL_STATIC_LIBS += dmautils.ae71
ADDITIONAL_STATIC_LIBS += libc.a
endif

ifeq ($(TARGET_PLATFORM), PC)
ADDITIONAL_STATIC_LIBS += libC7100-host-emulation.a
endif

ADDITIONAL_STATIC_LIBS += lib$(KERNEL_NAME).a
# Suppress this warning, 10063-D: entry-point symbol other than "_c_int00" specified
# c7x boots in secure mode and to switch to non-secure mode we need to start at a special entry point '_c_int00_secure'
# and later after switching to non-secure mode, sysbios jumps to usual entry point of _c_int00
# Hence we need to suppress this warning
ifneq ($(TARGET_PLATFORM), PC)
CFLAGS+=--diag_suppress=10063
endif

# CCS build needs Host IO
#DEFS += USE_HOST_FILE_IO
#DEFS += DDR_BW_STATS
ifeq ($(TARGET_PLATFORM), PC)
DEFS  += HOST_EMULATION
override CC := g++-5
endif

# SOC_DEF is coming from build_flags.mk
DEFS  += $(SOC_DEF)

ifeq ($(TARGET_CPU), C71)
DEFS  += __C7100__
endif

ifeq ($(TARGET_CPU), C7120)
DEFS  += __C7120__
endif

ifeq ($(TARGET_CPU), C66)
DEFS  += __C6600__
endif

ifneq ($(TARGET_PLATFORM), PC) 
ifeq ($(SET_RTOS), FREERTOS)
DEFS  += TIADALG_FREERTOS_BUILD
endif
endif

# get the common make flags from test/src/<plat>/../concerto_common.mak
#include $(HOST_ROOT)/concerto_common.mak

include $(FINALE)


