ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 C66 A72 A53))
include $(PRELUDE)
TARGET      := vx_target_kernels_stereo_bam
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VISION_APPS_PATH)/kernels/stereo/include
IDIRS       += $(VISION_APPS_PATH)/kernels/stereo/host
IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(ALGFRAMEWORK_PATH)/inc
IDIRS       += $(ALGFRAMEWORK_PATH)/src/bam_dma_nodes
IDIRS       += $(DMAUTILS_PATH)/inc
IDIRS       += $(DMAUTILS_PATH)
IDIRS       += $(DMAUTILS_PATH)/inc/edma_utils
IDIRS       += $(DMAUTILS_PATH)/inc/edma_csl
IDIRS       += $(DMAUTILS_PATH)/inc/baseaddress/vayu/dsp
IDIRS       += $(VXLIB_PATH)/packages
# < DEVELOPER_TODO: Add any custom include paths using 'IDIRS' >
# < DEVELOPER_TODO: Add any custom preprocessor defines or build options needed using
#                   'CFLAGS'. >
# < DEVELOPER_TODO: Adjust which cores this library gets built on using 'SKIPBUILD'. >

DEFS += CORE_DSP

ifeq ($(BUILD_BAM),yes)
DEFS += BUILD_BAM
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64))
DEFS += _HOST_BUILD _TMS320C6600 TMS320C66X HOST_EMULATION
endif

ifeq ($(BUILD_BAM),yes)
SKIPBUILD=0
else
SKIPBUILD=1
endif

ifeq ($(SOC),$(filter $(SOC), j722s am62a))
SKIPBUILD=1
endif

include $(FINALE)

endif

