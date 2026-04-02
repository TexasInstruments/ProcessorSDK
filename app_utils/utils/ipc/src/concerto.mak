ifneq ($(TARGET_PLATFORM),PC)

	include $(PRELUDE)
	TARGET      := app_utils_ipc
	TARGETTYPE  := library

	ifeq ($(LDRA_COVERAGE_ENABLED), yes)
		include $(PSDK_PATH)/tiovx/tiovx_dev/internal_docs/coverage_files/concerto_inc.mak
	else
		DEFS    += LDRA_UNTESTABLE_CODE
	endif

	ifeq ($(TARGET_OS),$(filter $(TARGET_OS),SYSBIOS FREERTOS SAFERTOS THREADX))
		CSOURCES := app_ipc_rtos.c
	endif

	ifeq ($(RTOS_SDK),mcu_plus_sdk)
		IDIRS    += $(VISION_APPS_PATH)/platform/$(SOC)/rtos
	endif

	ifeq ($(TARGET_OS),LINUX)
		CSOURCES := app_ipc_linux.c
		CSOURCES += app_ipc_linux_hw_spinlock.c
		CSOURCES += app_ipc_linux_rpmsg_char.c

	endif

	ifeq ($(TARGET_OS), QNX)
		IDIRS    += $(PDK_QNX_PATH)/packages/ti/drv/ipc/
		IDIRS    += $(PDK_QNX_PATH)/packages/
		CSOURCES := app_ipc_qnx.c
	endif

	ifeq ($(TARGET_OS),THREADX)
		## ThreadX is currently supported on only R5
		IDIRS    += $(MCU_PLUS_SDK_PATH)/source/kernel/threadx/ports/ti_arm_gcc_clang_cortex_r5/inc
	endif

	include $(FINALE)

endif
