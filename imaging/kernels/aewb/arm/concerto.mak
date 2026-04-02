ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 A72 A53 R5F))

include $(PRELUDE)
TARGET      := vx_target_kernels_imaging_aewb
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/aewb/host
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(HOST_ROOT)/algos/awb/include
IDIRS       += $(HOST_ROOT)/algos/ae/include
IDIRS       += $(HOST_ROOT)/algos/dcc/include
IDIRS       += $(HOST_ROOT)/sensor_drv/include
IDIRS       += $(HOST_ROOT)/itt_server_remote/include
IDIRS       += $(APP_UTILS_PATH)/utils/remote_service/include
IDIRS       += $(APP_UTILS_PATH)/utils/ipc/include

ifeq ($(SOC), am62a)
ifeq ($(TISDK_IMAGE), edgeai)
ifneq ($(TARGET_OS), QNX)
SKIPBUILD=1
endif
endif
endif

include $(FINALE)

endif
