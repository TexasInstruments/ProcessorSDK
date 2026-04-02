#******************************************************************************
#*      Copyright (C) 2016 Texas Instruments Incorporated.                    *
#*                      All Rights Reserved                                   *
#******************************************************************************

COMMON_FILES := \
	TI_profile.c \
	TI_test.c \
	TI_memory.c \
	c6x/TI_cache.c

ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)
	COMMON_FILES += printv.c
	COMMON_FILES += paramv.c
endif

SUBMODULE_LIST :=

EXCLUSION_LIST := . common

# If compiling for C7x and C6X_MIGRATION is not defined
ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)

SUBMODULE_LIST += $(SRC_DIR)_c7x
SUBMODULE_LIST += $(SRC_DIR)_cxx
TARGETFOLDER := c71
EXCLUSION_LIST +=

# else if compiling host emulation mode for c7x
else ifeq ($(TARGET_SCPU),C71)

SUBMODULE_LIST += $(SRC_DIR)_c7x
SUBMODULE_LIST += $(SRC_DIR)_cxx
TARGETFOLDER := c71
EXCLUSION_LIST += regress vx_c7x/VXLIB_cellSum_4x4_i16u_o16u vx_c7x/VXLIB_cellSum_NxN_i16u_o16u vx_c7x/VXLIB_convolve_3x3_i8u_c16s_o16s
EXCLUSION_LIST += vx_c7x/VXLIB_convolve_5x5_i8u_c16s_o16s vx_c7x/VXLIB_convolve_5x5_i8u_c16s_o8u vx_c7x/VXLIB_convolve_7x7_i8u_c16s_o16s vx_c7x/VXLIB_convolve_7x7_i8u_c16s_o8u vx_c7x/VXLIB_convolve_9x9_i8u_c16s_o16s
EXCLUSION_LIST += vx_c7x/VXLIB_convolve_9x9_i8u_c16s_o8u vx_c7x/VXLIB_convolve_i8u_c16s_o16s vx_c7x/VXLIB_convolve_i8u_c16s_o8u vx_c7x/VXLIB_doubleThreshold_i16u_i8u
EXCLUSION_LIST += vx_c7x/VXLIB_fastCornersDetect_i8u vx_c7x/VXLIB_fastCornersNMS_i8u vx_c7x/VXLIB_fastCornersScore_i8u vx_c7x/VXLIB_findFundamentalMat_i32f_o32f
EXCLUSION_LIST += vx_c7x/VXLIB_findHomography_i32f_o32f vx_c7x/VXLIB_harrisEdgesAndCornersScore_i16s_i16s_o32u vx_c7x/VXLIB_harrisScore_7x7_i16s_o32u
EXCLUSION_LIST += vx_c7x/VXLIB_houghLineFromList_multiple_16_Separated
EXCLUSION_LIST += vx_c7x/VXLIB_integralImage_i8u_o32u vx_c7x/VXLIB_kalmanFilter_4x12_Correct_F32 vx_c7x/VXLIB_kalmanFilter_4x12_Predict_F32
EXCLUSION_LIST += vx_c7x/VXLIB_kalmanFilter_4x8_Correct_F32 vx_c7x/VXLIB_kalmanFilter_4x8_Predict_F32 vx_c7x/VXLIB_median_3x3_i16s_o16s vx_c7x/VXLIB_median_3x3_i8u_o8u
EXCLUSION_LIST += vx_c7x/VXLIB_median_5x5_i16s_o16s vx_c7x/VXLIB_median_5x5_i8u_o8u vx_c7x/VXLIB_nms_MxN_i16s_o8u vx_c7x/VXLIB_nms_MxN_i8u_o8u vx_c7x/VXLIB_solvePnPRansac_i32f_o32f
EXCLUSION_LIST += vx_c7x/VXLIB_triangulatePoints_i32f_o32f vx_c7x/VXLIB_xyGradMag_i8u_o16u
EXCLUSION_LIST += vx_c7x/VXLIB_trackFeaturesLucasKanade_7x7 vx_c7x/VXLIB_multiply_i16s_i16s_o16s vx_c7x/VXLIB_multiply_i8u_i16s_o16s vx_c7x/VXLIB_multiply_i8u_i8u_o16s

# else use c66 folder for both C66 build, and X86 simulator
else

SUBMODULE_LIST += $(SRC_DIR)
SUBMODULE_LIST += $(SRC_DIR)_cxx
TARGETFOLDER := c66
EXCLUSION_LIST +=

endif
# end of ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)

# This line is to fix a warning from the 'find' operation
# It excludes the submodule directory root folder from being searched
# for the optimized folder of a kernel.
EXCLUSION_LIST += $(SUBMODULE_LIST)

ARCHIVE_LIST_CC := \

ARCHIVE_LIST_OPT := \

ARCHIVE_LIST_CN := \

# Common Utilities Library (needed for tests)
_MODULE=common
include $(PRELUDE)
TARGET = common_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(foreach file, $(COMMON_FILES), common/$(file))
CSOURCES += $(call all-c-files-in,common/$(TARGETFOLDER))
DEFS :=
ifneq ($(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($(HEAP_L2SRAM),)
DEFS += HEAP_L2SRAM
endif
ifneq ($(SIZE_L2CACHE),)
DEFS += SIZE_L2CACHE=$(SIZE_L2CACHE)
endif
#DEFS := DISABLE_STACK_PROFILING
include $(FINALE)

#List all files and subdirectories and this level
KERNEL_DIRS := $(foreach submod, $(SUBMODULE_LIST), $(wildcard $(_MODPATH)/$(submod)/*/))
#Make the list relative
KERNEL_DIRS := $(subst $(_MODPATH)/,,$(KERNEL_DIRS))
#Only interested in directories
KERNEL_DIRS := $(dir $(KERNEL_DIRS))
#Remove trailing directory slashes
KERNEL_DIRS := $(patsubst %/,%,$(KERNEL_DIRS))
#Remove exclusion list directories
KERNEL_DIRS := $(filter-out $(EXCLUSION_LIST), $(KERNEL_DIRS))

$(foreach submod,$(SUBMODULE_LIST),$(eval $(call KERNEL_SUBMODULE_RULE,$(submod))))

#List all sub-categories of sources for building libraries (vxlib, vxlib_bamplugin, vxlib_cn, vxlib_d)
CSOURCES_D  := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_d.c,$(dir)))
CSOURCES_D  += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_idat.c,$(dir)))
CSOURCES_CN := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_cn.c,$(dir)))

#Populate LIBLIST which contains the kernel libraries which other .out files are dependent on
$(foreach submod,$(SUBMODULE_LIST),$(eval include $(wildcard $(call PATH_CONV,$(SDIR)/$(submod)/concerto_inc.mak))))
LIBLIST := $(filter-out $(EXCLUSION_LIST), $(LIBLIST))

# OPTIMIZED Library
_MODULE=vxlib
include $(PRELUDE)
TARGET = vxlib_$(TARGET_CPU)
TARGETTYPE = library
# This generates library to match package.bld
CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.c,$(dir)/$(TARGETFOLDER)))
CSOURCES += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.c,$(dir)))
CSOURCES := $(filter-out $(CSOURCES_D), $(CSOURCES))
CSOURCES := $(filter-out $(CSOURCES_CN), $(CSOURCES))
#CSOURCES += common/$(TARGETFOLDER)/VXLIB_utility.c
# This generates incomplete library for now to match package.bld
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(dir)/$(TARGETFOLDER)/$(dir).c)
#CSOURCES += $(ARCHIVE_LIST_CC) $(ARCHIVE_LIST_OPT)
# This will generate full library after redundant object files are removed.
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-c-files-in,$(dir)/$(TARGETFOLDER)))
DEFS :=
ifneq ($(CHECKPARAMS),)
DEFS += VXLIB_CHECK_PARAMS
endif
include $(FINALE)

# BAM Plugin Library
ifneq ($(BAM_PLUGIN),)

ifndef ALGFRAMEWORK_ROOT
$(error You must define ALGFRAMEWORK_ROOT!)
endif
ifndef DMAUTILS_ROOT
$(error You must define DMAUTILS_ROOT!)
endif

_MODULE=vxlib_bamplugin
include $(PRELUDE)
TARGET = vxlib_bamplugin_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.c,$(dir)/bam_plugin))
IDIRS += $(ALGFRAMEWORK_ROOT)/inc $(DMAUTILS_ROOT)/inc/baseaddress/vayu/dsp $(DMAUTILS_ROOT)/inc/edma_csl $(DMAUTILS_ROOT)/inc/edma_utils
DEFS := CORE_DSP
ifneq ($(TARGET_SCPU),)
DEFS += HOST_EMULATION
endif
include $(FINALE)

endif

# NATC Library
_MODULE=vxlib_cn
include $(PRELUDE)
TARGET = vxlib_cn_$(TARGET_CPU)
TARGETTYPE = library
# This generates library to match package.bld
CSOURCES := $(CSOURCES_CN)
# This generates incomplete library for now to match package.bld
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(dir)/$(dir)_cn.c)
#CSOURCES += $(ARCHIVE_LIST_CC) $(ARCHIVE_LIST_CN)
# This will generate full library after redundant object files are removed.
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_cn.c,$(dir)))
DEFS :=
ifneq ($(CHECKPARAMS),)
DEFS += VXLIB_CHECK_PARAMS
endif
include $(FINALE)

# Test Files Library (not used in any rule except itself)
_MODULE=vxlib_d
include $(PRELUDE)
TARGET = vxlib_d_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(CSOURCES_D)
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_d.c,$(dir)))
#CSOURCES += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_idat.c,$(dir)))
DEFS := __ONESHOTTEST
include $(FINALE)

# The remainder of this file file is concerned with generating individual kernel
#   unit tests.

# TEST_MODULE_RULE macro for compiling the unit tests for each kernel
define TEST_MODULE_RULE
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
ifneq ($$(TARGETFOLDER_E),)
_MODULE := $(1)
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS :=
include $(wildcard $(call PATH_CONV,$(SDIR)/$(1)/concerto_inc.mak))
STATIC_LIBS += common_$(TARGET_CPU)
ifeq ($$(TARGET_SCPU),C66)
STATIC_LIBS += c6xsim_$(TARGET_CPU)_$(TARGET_SCPU)
endif
ifeq ($$(TARGET_SCPU),C71)
SYS_STATIC_LIBS += c70-host-emulation
endif
CSOURCES := $(call all-type-files-in-this,*.c,$(1))
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETFOLDER))
LINKER_FILES := lnk.cmd
DIRECTORY := $(dir $(1))
LDIRS+=$(TARGET_OUT)/$(dir $(1))
DEFS :=
ifneq ($(CHECKPARAMS),)
DEFS += VXLIB_CHECK_PARAMS
endif
include $(FINALE)
endif
endef

# TEST_MODULE_RULE_FROM_LIBS macro for compiling the unit tests for each kernel
define TEST_MODULE_RULE_FROM_LIBS
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
ifneq ($$(TARGETFOLDER_E),)
_MODULE := $(1)
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS :=
STATIC_LIBS += common_$(TARGET_CPU)
STATIC_LIBS += vxlib_$(TARGET_CPU) vxlib_cn_$(TARGET_CPU)
ifeq ($$(TARGET_SCPU),C66)
STATIC_LIBS += c6xsim_$(TARGET_CPU)_$(TARGET_SCPU)
endif
ifeq ($$(TARGET_SCPU),C71)
SYS_STATIC_LIBS += c70-host-emulation
endif
CSOURCES := $(call all-type-files-in-this,*_d.c,$(1))
CSOURCES += $(call all-type-files-in-this,*_idat.c,$(1))
LINKER_FILES := lnk.cmd
DIRECTORY := $(dir $(1))
DEFS :=
ifneq ($$(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($$(AUTOTEST),)
DEFS += AUTOTEST
endif
include $(FINALE)
endif
endef

# LIB_MODULE_RULE macro for compiling the dependent libs for each kernel
define LIB_MODULE_RULE
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
ifneq ($$(TARGETFOLDER_E),)
_MODULE := $(1)_lib
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(call all-type-files-in-this,*.c,$(1))
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETFOLDER))
DIRECTORY := $(dir $(1))
DEFS :=
ifneq ($(CHECKPARAMS),)
DEFS += VXLIB_CHECK_PARAMS
endif
include $(FINALE)
endif
endef

# TEST_MODULE_OVERRIDE_RULE macro for excluding making rules for kernels that have
#   concerto.mak already (so that the individual concerto.mak overrides this makefile
EXCLUSION_LIST :=
define TEST_MODULE_OVERRIDE_RULE
ifneq ($(wildcard $(call PATH_CONV,$(SDIR)/$(1)/concerto.mak)),)
EXCLUSION_LIST += $(1)
endif
endef

# Filter out making rules for directories which already have concerto.mak files
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_OVERRIDE_RULE,$(d))))
KERNEL_DIRS := $(filter-out $(EXCLUSION_LIST), $(KERNEL_DIRS))

# Insert rules to build all the kernel executables, and dependent libraries
ifneq ($(LINK_L2SRAM),)
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE_FROM_LIBS,$(d))))
else ifneq ($(AUTOTEST),)
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE_FROM_LIBS,$(d))))
else
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE,$(d))))
$(foreach d,$(LIBLIST),$(eval $(call LIB_MODULE_RULE,$(d))))
endif

TEST_MODULE_RULE :=
LIB_MODULE_RULE :=
TEST_MODULE_OVERRIDE_RULE :=
