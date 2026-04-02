PSDK_PATH = $(abspath ..)
PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder

# paths for components shared between app_utils, tiovx, imaging, video_io, ti-perception,
# and vision_apps are specified in below file
include $(PSDK_BUILDER_PATH)/tools_path.mak
