ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 A72 A53 R5F))

include $(PRELUDE)

TARGET      := app_utils_iss
TARGETTYPE  := library
IDIRS += $(HOST_ROOT)/sensor_drv/include
IDIRS += $(HOST_ROOT)/kernels/include

ifeq ($(TARGET_CPU),R5F)
CSOURCES    := app_iss_common.c
endif

ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 A72 A53))
CSOURCES    := app_iss_common_hlos.c app_iss_hlos.c
endif

ifeq ($(TARGET_PLATFORM),PC)
CSOURCES    += app_iss_x86.c
else
CSOURCES    += app_iss_server.c
endif

include $(FINALE)

endif
