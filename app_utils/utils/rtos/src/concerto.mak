ifeq ($(TARGET_OS), $(filter $(TARGET_OS), QNX FREERTOS SAFERTOS THREADX))

	include $(PRELUDE)

	TARGET      := app_utils_rtos
	TARGETTYPE  := library

	ifeq ($(LDRA_COVERAGE_ENABLED), yes)
		include $(PSDK_PATH)/tiovx/tiovx_dev/internal_docs/coverage_files/concerto_inc.mak
	else
		DEFS += LDRA_UNTESTABLE_CODE
	endif

	ifeq ($(TARGET_OS), $(filter $(TARGET_OS), FREERTOS SAFERTOS THREADX))
		ifeq ($(RTOS_SDK),mcu_plus_sdk)
			CSOURCES := app_rtos_mcu_plus_sdk.c
		else
			CSOURCES := app_rtos_pdk.c
		endif
	else
		CSOURCES     := app_rtos_pdk.c
	endif

	include $(FINALE)

endif
