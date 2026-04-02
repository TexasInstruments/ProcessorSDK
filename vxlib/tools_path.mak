
PSDK_PATH = $(abspath ..)
$(info PSDK_PATH=$(PSDK_PATH))

PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder

# paths for cgtools are specified in below
# file in tiovx, ...
include $(PSDK_BUILDER_PATH)/tools_path.mak
