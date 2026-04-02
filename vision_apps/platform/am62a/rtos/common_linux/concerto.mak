ifeq ($(TARGET_PLATFORM),AM62A)
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), SYSBIOS FREERTOS THREADX))

include $(PRELUDE)
TARGET      := app_rtos_linux
TARGETTYPE  := library

CSOURCES    := app_common.c

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/common

include $(FINALE)

endif
endif
