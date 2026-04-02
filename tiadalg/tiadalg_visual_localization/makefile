ALGBASE_PATH ?= $(abspath ../)
PSDK_PATH ?= $(abspath ../../)
TARGET_CPU ?= C66
KERNEL_NAME ?= $(shell basename $(abspath ./))

ifeq ($(TARGET_CPU),C66)
LINKER_CMD_FILE = $(ALGBASE_PATH)/common/linker_dsp_c66x_ddr_heap.cmd
else
LINKER_CMD_FILE = $(ALGBASE_PATH)/common/linker_dsp_c7x_0.cmd
endif
MODULE_NAME?=$(KERNEL_NAME)

ifeq ($(test_build),1)
ifdef SystemRoot
EXT_LIBFILE=$(LIBDIR)/tiadalg_select_top_feature.lib
EXT_LIBFILE+=$(LIBDIR)/tiadalg_solve_pnp.lib
EXT_LIBFILE+=$(LIBDIR)/tiadalg_sparse_upsampling.lib
EXT_LIBFILE+=$(LIBDIR)/tiadalg_image_recursive_nms.lib
else
EXT_LIBFILE=$(LIBDIR)/libtiadalg_select_top_feature.a
EXT_LIBFILE+=$(LIBDIR)/libtiadalg_solve_pnp.a
EXT_LIBFILE+=$(LIBDIR)/libtiadalg_sparse_upsampling.a
EXT_LIBFILE+=$(LIBDIR)/libtiadalg_image_recursive_nms.a
endif
endif


include $(ALGBASE_PATH)/makerules/makefile

debug_print:
	echo $(ALGBASE_PATH)
	echo $(PSDK_PATH)
	echo $(TARGET_CPU)
	echo $(KERNEL_NAME)



