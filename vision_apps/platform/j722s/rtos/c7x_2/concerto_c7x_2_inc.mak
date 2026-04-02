DEFS+=SOC_J722S
DEFS+=BUILD_C75X
DEFS+=BUILD_C75X_2

CSOURCES += generated/ti_board_config.c
CSOURCES += generated/ti_board_open_close.c
CSOURCES += generated/ti_dpl_config.c
CSOURCES += generated/ti_drivers_config.c
CSOURCES += generated/ti_drivers_open_close.c
CSOURCES += generated/ti_pinmux_config.c
CSOURCES += generated/ti_power_clock_config.c

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/$(SOC)_linker_$(RTOS_TYPE).cmd
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map_$(RTOS_TYPE).cmd

ifeq ($(RTOS),SAFERTOS)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT_c7x)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/api/NoWrapper
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/config
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/kernel/include_api
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_c7x)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_c7x}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_c7x)/$(SAFERTOS_COMPILER_EXT_c7x)
endif

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/c7x_2/generated

LDIRS += $(MCU_PLUS_SDK_PATH)/source/kernel/$(RTOS_TYPE)/lib/
LDIRS += $(MCU_PLUS_SDK_PATH)/source/drivers/lib/
LDIRS += $(MCU_PLUS_SDK_PATH)/source/board/lib/

include $($(_MODULE)_SDIR)/../concerto_c7x_inc.mak

# CPU instance specific libraries
STATIC_LIBS += vx_target_kernels_img_proc_c66
STATIC_LIBS += app_rtos_common_c7x_2
STATIC_LIBS += vx_target_kernels_srv_c66

ifeq ($(RTOS), $(filter $(RTOS), FREERTOS SAFERTOS))
	STATIC_LIBS += app_rtos
endif

ADDITIONAL_STATIC_LIBS += dmautils.j722s.c75ss1-0.ti-c7000.${TARGET_BUILD}.lib
ADDITIONAL_STATIC_LIBS += drivers.j722s.c75ss1-0.ti-c7000.${TARGET_BUILD}.lib

#
# Suppress this warning, 10063-D: entry-point symbol other than "_c_int00" specified
# c7x boots in secure mode and to switch to non-secure mode we need to start at a special entry point '_c_int00_secure'
# and later after switching to non-secure mode, sysbios jumps to usual entry point of _c_int00
# Hence we need to suppress this warning
CFLAGS+=--diag_suppress=10063
CFLAGS+=--diag_suppress=770
CFLAGS+=--diag_suppress=69
CFLAGS+=--diag_suppress=70
