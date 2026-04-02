# This make file is used for simple standalone build of each folder library and test application, and independent of which kernel lib/exe is built

# This is not primary make file hence all the environment variable and make variable let it come from the file which calls it
# No relative paths should be used in this make file, all path should be with respect to $(ALGBASE_PATH)


include $(ALGBASE_PATH)/makerules/config.mk # import default setting for various include relative paths, platform etc.

OUTDIR=$(ALGBASE_PATH)/out

ifeq ($(TARGET_PLATFORM) , PC)
LIBDIR  ?= $(ALGBASE_PATH)/lib/x86_64/$(TARGET_BUILD)
else
LIBDIR  ?= $(ALGBASE_PATH)/lib/$(TARGET_CPU)/$(TARGET_BUILD)
endif

ifdef SystemRoot
LIBFILE ?= $(LIBDIR)/$(KERNEL_NAME).lib
else
LIBFILE ?= $(LIBDIR)/lib$(KERNEL_NAME).a
endif


ifeq ($(test_build),1)
  SUBDIRS = $(ALGBASE_PATH)/$(KERNEL_NAME)/test
  SUBDIRS += $(ALGBASE_PATH)/common

  ifeq ($(TARGET_PLATFORM),PC)
  OUTFILE=$(OUTDIR)/PC_$(TARGET_CPU)_$(KERNEL_NAME)
  else
  OUTFILE=$(OUTDIR)/DEVICE_$(TARGET_CPU)_$(KERNEL_NAME).out
  endif

else
  SUBDIRS = $(ALGBASE_PATH)/$(KERNEL_NAME)/alg
  SUBDIRS += $(USERSUBDIRS)

OUTFILE=$(LIBFILE)
endif

CFILES ?= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
#CFILES ?= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))
HFILES ?= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.h))
CFILES+= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.asm))

include $(ALGBASE_PATH)/makerules/rules.mk

CFLAGS+= -I $(ALGBASE_PATH)/include
CFLAGS+= -I $(ALGBASE_PATH)/common
CFLAGS+= -I $(IVISION_PATH)/
CFLAGS+= -I $(PDK_INSTALL_PATH)/packages/

ifneq ($(TARGET_PLATFORM),PC)
# To enable embedded C++
#CFLAGS+= -pe # embedded c++ is no longer supported
CFLAGS+= --fp_not_associative
endif

ifeq ($(test_build),1)
CFLAGS+= -I $(ALGBASE_PATH)/$(KERNEL_NAME)/test
LDFILES+="$(LIBFILE)"
LDFILES+=$(EXT_LIBFILE)
endif

ifeq ($(TARGET_PLATFORM),PC)
	ifeq ($(TARGET_CPU),C71)
	    ifdef SystemRoot
	    LDFILES+=$(CGT7X_ROOT)/host_emulation/C7100-host-emulation.lib
	    else
	    LDFILES+=$(CGT7X_ROOT)/host_emulation/libC7100-host-emulation.a
	    endif
	else ifeq ($(TARGET_CPU),C7120)
	    ifdef SystemRoot
	    LDFILES+=$(CGT7X_ROOT)/host_emulation/C7100-host-emulation.lib
	    else
	    LDFILES+=$(CGT7X_ROOT)/host_emulation/libC7100-host-emulation.a
	    endif
	else
	    ifdef SystemRoot
	    LDFILES+=$(LIBDIR)\c6xsim.lib
	    else
	    LDFILES+=$(LIBDIR)/libc6xsim.a
	    endif

	endif
endif

ifneq ($(TARGET_PLATFORM),PC)
LDFLAGS+= -l $(LINKER_CMD_FILE)
LDFLAGS+= --zero_init=off
endif


ifeq ($(TARGET_PLATFORM), PC)
LDFLAGS += $(PDK_INSTALL_PATH)/packages/ti/osal/lib/nonos/$(SOC)/c7x-hostemu/$(TARGET_BUILD)/ti.osal.lib
LDFLAGS += $(PDK_INSTALL_PATH)/packages/ti/drv/sciclient/lib/$(SOC)_hostemu/c7x-hostemu/$(TARGET_BUILD)/sciclient.lib
LDFLAGS += $(PDK_INSTALL_PATH)/packages/ti/csl/lib/$(SOC)/c7x-hostemu/$(TARGET_BUILD)/ti.csl.lib
LDFILES += $(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/$(SOC)_hostemu/c7x-hostemu/$(TARGET_BUILD)/dmautils.lib
LDFILES += $(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/$(SOC)_hostemu/c7x-hostemu/$(TARGET_BUILD)/udma.lib
else
#freertos related libraries
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/$(SOC)/c7x_1/$(TARGET_BUILD)/dmautils.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/csl/lib/$(SOC)/c7x/$(TARGET_BUILD)/ti.csl.ae71

ifeq ($(RTOS),FREERTOS)
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/drv/uart/lib/$(SOC)/c7x/$(TARGET_BUILD)/ti.drv.uart.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/drv/i2c/lib/$(SOC)/c7x/$(TARGET_BUILD)/ti.drv.i2c.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/kernel/lib/$(SOC)/c7x_$(C7X_CORE_ID)/$(TARGET_BUILD)/ti.kernel.freertos.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/board/lib/$(SOC)_evm/c7x/$(TARGET_BUILD)/ti.board.ae71

CFLAGS+= -DTIADALG_FREERTOS_BUILD
endif

LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/drv/udma/lib/$(SOC)/c7x_1/$(TARGET_BUILD)/udma.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/osal/lib/freertos/$(SOC)/c7x/$(TARGET_BUILD)/ti.osal.ae71
LDFLAGS += --library=$(PDK_INSTALL_PATH)/packages/ti/drv/sciclient/lib/$(SOC)/c7x_1/$(TARGET_BUILD)/sciclient.ae71


endif


ifeq ($(test_build),1)
$(OUTFILE) : outfile
else
$(OUTFILE) : libfile
endif

test_make :
	echo $(notdir $(CURDIR))
	echo $(CFILES)
	echo $(HFILES)
	echo $(OUTDIR)
	echo $(LIBDIR)







