ifneq ($(TARGET_PLATFORM),PC)
	ifeq ($(TARGET_OS),$(filter $(TARGET_OS),SYSBIOS FREERTOS SAFERTOS THREADX))

	include $(PRELUDE)
	TARGET      := app_utils_sciclient
	TARGETTYPE  := library

	CSOURCES    := app_sciclient.c

	include $(FINALE)

	endif
endif
