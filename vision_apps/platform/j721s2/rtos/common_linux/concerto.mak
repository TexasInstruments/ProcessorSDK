ifeq ($(TARGET_PLATFORM),J721S2)
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), SYSBIOS FREERTOS SAFERTOS))

include $(PRELUDE)
TARGET      := app_rtos_linux
TARGETTYPE  := library

CSOURCES    := app_common.c

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/common

include $(FINALE)

endif
endif
