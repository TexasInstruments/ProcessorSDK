ifneq ($(TARGET_PLATFORM),PC)

	include $(PRELUDE)
	TARGET      := app_utils_remote_service
	TARGETTYPE  := library

	CSOURCES := app_remote_service_test.c
	ifeq ($(TARGET_OS),$(filter $(TARGET_OS),SYSBIOS FREERTOS SAFERTOS QNX THREADX))
		CSOURCES += app_remote_service.c
	endif
	ifeq ($(TARGET_OS),LINUX)
		CSOURCES += app_remote_service_linux.c
	endif

	ifeq ($(RTOS_SDK),mcu_plus_sdk)
		IDIRS    += $(VISION_APPS_PATH)/platform/$(SOC)/rtos
	endif

	ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C7120 C7524 C71))
		DEFS += ENABLE_MMA_LOAD_TEST
		IDIRS += $(MMALIB_PATH)/ti/mmalib/src
	endif

	ifeq ($(TARGET_OS),THREADX)
		## ThreadX is currently supported on only R5
		IDIRS    += $(MCU_PLUS_SDK_PATH)/source/kernel/threadx/ports/ti_arm_gcc_clang_cortex_r5/inc
	endif

	include $(FINALE)

endif

