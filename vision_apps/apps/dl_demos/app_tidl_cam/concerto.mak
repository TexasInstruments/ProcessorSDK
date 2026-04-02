ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72 A53))

include $(PRELUDE)

TARGET      := vx_app_tidl_cam
TARGETTYPE  := exe

CSOURCES    := main.c
CSOURCES    += app_pre_proc_module.c
CSOURCES    += app_post_proc_module.c
CSOURCES    += imagenet_class_labels.c

ifeq ($(TARGET_CPU),x86_64)
include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak
CSOURCES    += main_x86.c
# Not building for PC
SKIPBUILD=1
endif

ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))
ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak
CSOURCES    += main_linux_arm.c
endif
endif

IDIRS += $(IMAGING_IDIRS)
IDIRS += $(VISION_APPS_KERNELS_IDIRS)
IDIRS += $(VISION_APPS_MODULES_IDIRS)
ifeq ($(SOC),$(filter $(SOC), am62a))
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), QNX))
IDIRS += $(EDGEAI_KERNELS_PATH)/include
IDIRS += $(TIOVX_PATH)/source/include
endif
endif
STATIC_LIBS += $(IMAGING_LIBS)
STATIC_LIBS += $(VISION_APPS_KERNELS_LIBS)
STATIC_LIBS += $(VISION_APPS_MODULES_LIBS)
STATIC_LIBS += $(TIADALG_LIBS)

IDIRS       += $(EDGEAI_IDIRS)
SHARED_LIBS += edgeai-apps-utils
SHARED_LIBS += edgeai-tiovx-kernels

ifeq ($(SOC),j722s)
SKIPBUILD=1
endif

include $(FINALE)

endif
