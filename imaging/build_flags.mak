# Inherit common build flags from root repo in SDK
include $(PSDK_BUILDER_PATH)/build_flags.mak

# These flags are only needed within this repo
BUILD_HWA_KERNELS?=yes
BUILD_VPAC_VISS?=yes
BUILD_VPAC_MSC?=yes
BUILD_VPAC_LDC?=yes
BUILD_VPAC_FC?=yes

ifeq ($(BUILD_VPAC_FC),yes)
	BUILD_VPAC_VISS=yes
	BUILD_VPAC_MSC=yes
endif

ifeq ($(SOC),am62a)
	BUILD_DMPAC_DOF=no
	BUILD_DMPAC_SDE=no
	BUILD_VPAC_NF=no
else ifeq ($(SOC),j722s)
	BUILD_DMPAC_DOF?=yes
	BUILD_DMPAC_SDE?=yes
	BUILD_VPAC_NF?=no
else
	BUILD_DMPAC_DOF?=yes
	BUILD_DMPAC_SDE?=yes
	BUILD_VPAC_NF?=yes
endif

BUILD_CT_TIOVX_HWA=yes
BUILD_CT_TIOVX_HWA_NEGATIVE_TESTS=yes
BUILD_CT_TIOVX_HWA_NON_CRITICAL_ERROR=no
BUILD_IMAGING_OPENACC?=no
