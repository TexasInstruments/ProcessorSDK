ALGBASE_PATH ?= $(abspath ./../)
PSDK_PATH ?= $(abspath ./../../)
PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder
KERNEL_NAME ?= $(shell basename $(abspath ./))
TARGET_CPU ?= C71

include $(ALGBASE_PATH)/makerules/config.mk
include $(PSDK_BUILDER_PATH)/build_flags.mak

# only for reference or debug purpose
ifeq ($(TIADALG_BUILD),CONCERTO)
	GCC_LINUX_ROOT := /usr

ifeq ($(TARGET_BUILD),debug)
  PROFILE := debug
else
  PROFILE := release
endif

	DIRECTORIES := $(ALGBASE_PATH)/$(KERNEL_NAME)/test $(ALGBASE_PATH)/common/

	TARGET_COMBOS :=

	SYSDEFS := CONCERTO_BUILD

	ifeq ($(TARGET_PLATFORM),TI_DEVICE)
	  TARGET_COMBOS += J7_CCS:$(RTOS):$(TARGET_CPU):1:$(PROFILE):CGT7X
	else
	  ifdef SystemRoot
	    TARGET_COMBOS += PC:WINDOWS:x64:1:$(PROFILE):CL
	  else
	    TARGET_COMBOS += PC:LINUX:x86_64:1:$(PROFILE):GCC_LINUX
	  endif
	endif

	BUILD_MULTI_PROJECT := 1


	TARGET_COMBOS_SPACE := $(subst :, ,$(TARGET_COMBOS))
	SET_RTOS = $(word 2, $(TARGET_COMBOS_SPACE))

	include $(CONCERTO_ROOT)/rules.mak
else
	ifeq ($(TARGET_CPU),C66)
	LINKER_CMD_FILE := $(ALGBASE_PATH)/common/linker_dsp_c66x.cmd
	else
	LINKER_CMD_FILE := $(ALGBASE_PATH)/$(KERNEL_NAME)/test/c7x_linker_freertos.cmd
	endif

	test_build?=0

	include $(ALGBASE_PATH)/makerules/makefile
endif

.PHONY: all
all:

all: all

ifeq ($(TIADALG_BUILD),CONCERTO)
	$(foreach mod, $(MODULES), $(call TIDL_EXE_COPY,  $($(mod)_TDIR)/$($(mod)_TARGET)$(BIN_EXT), $(ALGBASE_PATH)/out))
	$(foreach mod, $(MODULES), $(call TIDL_EXE_COPY,  $($(mod)_TDIR)/$($(mod)_TARGET)$(BIN_EXT).map, $(ALGBASE_PATH)/out))
endif

debug_print:
	echo $(ALGBASE_PATH)
	echo $(PSDK_PATH)
	echo $(KERNEL_NAME)
	echo $(TARGET_COMBOS_SPACE)
	echo $(SET_RTOS)
	echo $(HOST_ROOT)
	echo $(TIADALG_BUILD)

ifdef SystemRoot
define TIDL_EXE_COPY
	-$(PRINT) Copying built libraries: $(1)   to    $(2)
	-$(Q)if exist $(call PATH_CONV,$(1)) $(COPY) $(call PATH_CONV,$(1)) $(call PATH_CONV,$(2)) > NULL
endef
else
define TIDL_EXE_COPY
	-$(PRINT) Copying built libraries: $(1)   to    $(2)
	-$(Q)$(COPY) $(1) $(2)
endef
endif
