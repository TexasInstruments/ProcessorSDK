ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72 A53))

include $(PRELUDE)

TARGET      := vx_app_dense_optical_flow

CSOURCES    := main.c dof_pyramid_module.c dof_proc_module.c dof_viz_module.c dof_display_module.c

ifeq ($(TARGET_CPU),x86_64)
TARGETTYPE  := exe
CSOURCES    += main_x86.c
include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak

ifeq ($(SOC),$(filter $(SOC),j722s))
SKIPBUILD=1
endif

endif

ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))
TARGETTYPE  := exe
CSOURCES    += main_linux_arm.c
include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak
endif
endif

ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
ifeq ($(TARGET_OS),SYSBIOS)
TARGETTYPE  := library
include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak
endif
endif

include $(FINALE)

endif
