PSDK_PATH ?= $(abspath ..)
PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder

# Inherit common build flags from root repo in SDK
include $(PSDK_BUILDER_PATH)/build_flags.mak

# These flags are only needed within this repo
ifeq ($(SOC),am62a)
	BUILD_VIDEO_IO_KERNELS=yes
	BUILD_CAPTURE=yes
	BUILD_CSITX=no
	BUILD_DISPLAY=no
	BUILD_DISPLAY_M2M=no
else ifeq ($(SOC),j722s)
	BUILD_VIDEO_IO_KERNELS?=yes
	BUILD_CAPTURE?=yes
	BUILD_CSITX?=yes
	BUILD_DISPLAY?=yes
	BUILD_DISPLAY_M2M=no
else
	BUILD_VIDEO_IO_KERNELS?=yes
	BUILD_CAPTURE?=yes
	BUILD_CSITX?=yes
	BUILD_DISPLAY?=yes
	BUILD_DISPLAY_M2M?=yes
endif

BUILD_CT_TIOVX_VIDEO_IO=yes
BUILD_CT_TIOVX_VIDEO_IO_CAPTURE_TESTS=yes
BUILD_CT_TIOVX_VIDEO_IO_DISPLAY_TESTS=yes
