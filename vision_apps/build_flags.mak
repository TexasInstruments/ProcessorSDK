PSDK_PATH ?= $(abspath ..)
PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder

# Inherit common build flags from root repo in SDK
include $(PSDK_BUILDER_PATH)/vision_apps_build_flags.mak
