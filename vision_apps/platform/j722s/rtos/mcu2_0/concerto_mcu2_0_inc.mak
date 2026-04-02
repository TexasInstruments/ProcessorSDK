DEFS+=CPU_mcu2_0
DEFS+=BUILD_MAIN_R5
DEFS+=SOC_J722S

#DEFS+=BUILD_MCU
#DEFS+=VIM_DIRECT_REGISTRATION

# This enables ARM Thumb mode which reduces firmware size and enables faster boot
#COPT +=--code_state=16
CSOURCES += generated/ti_board_config.c
CSOURCES += generated/ti_board_open_close.c
CSOURCES += generated/ti_dpl_config.c
CSOURCES += generated/ti_drivers_config.c
CSOURCES += generated/ti_drivers_open_close.c
CSOURCES += generated/ti_pinmux_config.c
CSOURCES += generated/ti_power_clock_config.c

LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/$(SOC)_linker_$(RTOS_TYPE).cmd
LINKER_CMD_FILES +=  $($(_MODULE)_SDIR)/linker_mem_map_$(RTOS_TYPE).cmd

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos

ifeq ($(RTOS),SAFERTOS)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/api/$(SAFERTOS_ISA_EXT_r5f)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/api/PrivWrapperStd
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/config
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/kernel/include_api
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/kernel/include_prv
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_r5f)
	IDIRS+=${SAFERTOS_KERNEL_INSTALL_PATH_r5f}/source_code_and_projects/SafeRTOS/portable/$(SAFERTOS_ISA_EXT_r5f)/$(SAFERTOS_COMPILER_EXT_r5f)
endif

IDIRS+=$(VISION_APPS_PATH)/platform/$(SOC)/rtos/mcu2_0/generated

LDIRS += $(MCU_PLUS_SDK_PATH)/source/kernel/$(RTOS_TYPE)/lib/
LDIRS += $(MCU_PLUS_SDK_PATH)/source/drivers/lib/
LDIRS += $(MCU_PLUS_SDK_PATH)/source/board/lib/
LDIRS += $(MCU_PLUS_SDK_PATH)/source/drivers/vhwa/lib

include $($(_MODULE)_SDIR)/../concerto_r5f_inc.mak

# CPU instance specific libraries
STATIC_LIBS += app_rtos_common_mcu2_0
ifeq ($(RTOS), $(filter $(RTOS), FREERTOS SAFERTOS))
	STATIC_LIBS += app_rtos
endif

SYS_STATIC_LIBS += app_utils_sciclient
SYS_STATIC_LIBS += app_utils_hwa

ifeq ($(BUILD_MCU_BOARD_DEPENDENCIES),yes)
SYS_STATIC_LIBS += app_utils_dss
endif

ADDITIONAL_STATIC_LIBS += drivers.j722s.main-r5f.ti-arm-clang.${TARGET_BUILD}.lib
ADDITIONAL_STATIC_LIBS += vhwa.j722s.main-r5fss0-0.ti-arm-clang.${TARGET_BUILD}.lib

DEFS        += $(RTOS)
