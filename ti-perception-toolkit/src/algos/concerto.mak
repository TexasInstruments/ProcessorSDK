include $(PRELUDE)
TARGET      := ptk_algos
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

FILE_LIST   := $(call all-c-files-in,src/algos/camera)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo/object_detection)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/object_detection/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo/ground_estimation)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/ground_estimation/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo/common_detection)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/common_detection/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo/postprocess)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/postprocess/$(f))

FILE_LIST   := $(call all-c-files-in,src/algos/camera/stereo/ogmap)
CSOURCES    += $(foreach f, $(FILE_LIST),camera/stereo/ogmap/$(f))

IDIRS       += $(HOST_ROOT)/include
IDIRS       += $(HOST_ROOT)/src/algos/radar

IDIRS       += $(HOST_ROOT)/include/perception/algos/c6xsim
DEFS        += TMS320C6600 TMS320C66X

include $(FINALE)

