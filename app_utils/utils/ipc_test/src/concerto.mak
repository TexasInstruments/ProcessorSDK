ifneq ($(TARGET_PLATFORM),PC)

	include $(PRELUDE)
	TARGET      := app_utils_ipc_test
	TARGETTYPE  := library

	ifeq ($(TARGET_OS),$(filter $(TARGET_OS),SYSBIOS FREERTOS SAFERTOS THREADX))
		CSOURCES := app_ipc_rtos_echo_test.c
	endif

	ifeq ($(RTOS_SDK),mcu_plus_sdk)
		IDIRS    += $(VISION_APPS_PATH)/platform/$(SOC)/rtos
	endif

	ifeq ($(TARGET_OS),THREADX)
		## ThreadX is currently supported on only R5
		IDIRS    += $(MCU_PLUS_SDK_PATH)/source/kernel/threadx/ports/ti_arm_gcc_clang_cortex_r5/inc
	endif

	include $(FINALE)

endif # ifneq ($(TARGET_PLATFORM),PC)