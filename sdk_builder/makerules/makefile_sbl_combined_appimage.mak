#
# Utility makefile for SBL boot support with combined appimage tool
#
# Edit this file to suit your build needs
#

#######################################################################################
###################################[ PDK SBL ]#########################################

# To build SBL combined appimage for QNX MPU, run the following commands:
# make sbl_combined_bootimage   -> Generate all the required files (tiboot3.bin, tifs.bin, app, lateapp1, lateapp2, atf_optee.appimage, ifs_qnx.appimage)
#
# or use below individual targets:
#
# make sbl_sd_hlos         		-> tiboot3.bin, tifs.bin
# make sbl_ospi_hlos            -> tiboot3.bin, tifs.bin
# make sbl_bootapp		     	-> app
# make sbl_lateapps_qnx    		-> lateapp1, lateapp2
# make sbl_qnx_appimage    		-> ifs_qnx.appimage, atf_optee.appimage

# make sbl_combined_bootimage_install_sd -> to copy all the generated files to the SD card QNX boot path


# Check if the RTOS SDK is set to mcu_plus_sdk or pdk
ifeq ($(RTOS_SDK),pdk)

SBL_CORE=mcu1_0
BOARD=$(BUILD_PDK_BOARD)

SBL_COMBINED_BOOTFILES_PATH=$(VISION_APPS_PATH)/out/sbl_combined_bootfiles

COMBINED_APPIMAGE_TOOL_PATH=$(SBL_REPO_PATH)/tools/combined_appimage
SBL_REPO_PATH=$(PDK_PATH)/packages/ti/boot/sbl
CROSS_COMPILE=$(GCC_LINUX_ARM_ROOT)/bin/$(CROSS_COMPILE_LINARO)

ATF_TARGET_BOARD=generic
ifeq ($(SOC),j784s4)
	ATF_TARGET_BOARD=j784s4
endif

REMOTE_CORE_LIST_LATEAPP1_COMBINED=
REMOTE_CORE_LIST_LATEAPP2_COMBINED=

ifeq ($(BUILD_CPU_MCU2_0),yes)
	REMOTE_CORE_LIST_LATEAPP1_COMBINED+=10 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu2_0.out.rprc
endif
ifeq ($(BUILD_CPU_MCU2_1),yes)
	REMOTE_CORE_LIST_LATEAPP1_COMBINED+=11 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu2_1.out.rprc
endif
ifeq ($(BUILD_CPU_MCU3_0),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=12 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu3_0.out.rprc
endif
ifeq ($(BUILD_CPU_MCU3_1),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=13 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu3_1.out.rprc
endif
ifeq ($(BUILD_CPU_MCU4_0),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=14 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu4_0.out.rprc
endif
ifeq ($(BUILD_CPU_MCU4_1),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=15 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu4_1.out.rprc
endif
ifeq ($(BUILD_CPU_C6x_1),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=16 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c6x_1.out.rprc
endif
ifeq ($(BUILD_CPU_C6x_2),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=17 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c6x_2.out.rprc
endif
ifeq ($(BUILD_CPU_C7x_1),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=18 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_1.out.rprc
endif
ifeq ($(BUILD_CPU_C7x_2),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=19 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_2.out.rprc
endif
ifeq ($(BUILD_CPU_C7x_3),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=20 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_3.out.rprc
endif
ifeq ($(BUILD_CPU_C7x_4),yes)
	REMOTE_CORE_LIST_LATEAPP2_COMBINED+=21 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_4.out.rprc
endif


##############
### Main Target
##############
sbl: sbl_combined_bootimage
sbl_clean: sbl_scrub
sbl_scrub: sbl_combined_bootimage_clean

sbl_combined_bootimage: sbl_sd_hlos sbl_ospi_hlos sbl_appimage_hlos
	@echo "------------------------------------------------------"
	@echo "SBL Combined Boot Image generation completed."
	@echo "use 'make sbl_combined_bootimage_install_sd' to install the images to the SD card"
	@echo "------------------------------------------------------"

sbl_appimage_hlos: sbl_bootapp sbl_lateapps_qnx sbl_qnx_appimage

sbl_syscfg_gui:
	$(MAKE) -C $(PDK_PATH)/packages/ti/drv/pdm_utils/tools/dm_power_config_tool syscfg_gui BOARD=$(BOARD)
	$(MAKE) -C $(PDK_PATH)/packages/ti/drv/pdm_utils/tools/dm_power_config_tool syscfg BOARD=$(BOARD)

##############
### Install
##############
sbl_combined_bootimage_install_sd:
	@echo "Installing QNX FS SBL to SD Card..."
	cp $(SBL_COMBINED_BOOTFILES_PATH)/tiboot3.bin $(SBL_SD_FS_PATH)/tiboot3.bin
	cp $(SBL_COMBINED_BOOTFILES_PATH)/tifs.bin $(SBL_SD_FS_PATH)/tifs.bin
	cp $(SBL_COMBINED_BOOTFILES_PATH)/app $(SBL_SD_FS_PATH)/app
	cp $(SBL_COMBINED_BOOTFILES_PATH)/lateapp1 $(SBL_SD_FS_PATH)/lateapp1
	cp $(SBL_COMBINED_BOOTFILES_PATH)/lateapp2 $(SBL_SD_FS_PATH)/lateapp2
ifeq ($(BUILD_QNX_MPU), yes)
	cp $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee.appimage $(SBL_SD_FS_PATH)/atf_optee.appimage
	cp $(SBL_COMBINED_BOOTFILES_PATH)/ifs_qnx.appimage $(SBL_SD_FS_PATH)/ifs_qnx.appimage
endif
	sync
	@echo "Installation complete."


##############
### SBL -> generate tiboot3.bin and tifs.bin
##############
sbl_sd_hlos:
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)    
	$(MAKE) -C $(PDK_PATH)/packages/ti/build sbl_mmcsd_img TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
	cp $(PDK_PATH)/packages/ti/boot/sbl/binary/$(BOARD)/mmcsd/bin/sbl_mmcsd_img_$(SBL_CORE)_release.tiimage $(SBL_COMBINED_BOOTFILES_PATH)/tiboot3.bin
	cp $(PDK_PATH)/packages/ti/drv/sciclient/soc/$(SCICLIENT_VERSION)/tifs.bin $(SBL_COMBINED_BOOTFILES_PATH)/tifs.bin

sbl_ospi_hlos:
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)/ospi
	$(MAKE) -C $(PDK_PATH)/packages/ti/build sbl_ospi_img TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
	$(MAKE) -C $(PDK_PATH)/packages/ti/build sbl_cust_img TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
	cp $(PDK_PATH)/packages/ti/boot/sbl/binary/$(BOARD)/ospi/bin/sbl_ospi_img_$(SBL_CORE)_release.tiimage $(SBL_COMBINED_BOOTFILES_PATH)/ospi/sbl_ospi_img_$(SBL_CORE)_release.tiimage
	cp $(PDK_PATH)/packages/ti/boot/sbl/binary/$(BOARD)/cust/bin/sbl_cust_img_$(SBL_CORE)_release.tiimage $(SBL_COMBINED_BOOTFILES_PATH)/ospi/sbl_cust_img_$(SBL_CORE)_release.tiimage
	cp $(PDK_PATH)/packages/ti/board/src/flash/nor/ospi/nor_spi_patterns.bin $(SBL_COMBINED_BOOTFILES_PATH)/ospi/nor_spi_patterns.bin
	cp $(PDK_PATH)/packages/ti/drv/sciclient/soc/$(SCICLIENT_VERSION)/tifs.bin $(SBL_COMBINED_BOOTFILES_PATH)/tifs.bin
	

#############
### Boot app -> generate app
#############

sbl_bootapp: sbl_bootapp_sd sbl_bootapp_ospi

sbl_bootapp_sd:
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)
    ifeq ($(BUILD_QNX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_mmcsd_qnx ECU_BUILD=$(ECU_BUILD) TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
        ifeq ($(ECU_BUILD), no)
			cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/mmcsd/sbl_boot_app_mmcsd_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/app
        else
			cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/mmcsd/sbl_boot_app_mmcsd_$(ECU_BUILD)_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/app
        endif
    endif
    ifeq ($(BUILD_LINUX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_mmcsd_linux TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
		cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/mmcsd/sbl_boot_app_mmcsd_linux_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/app
    endif

sbl_bootapp_ospi:
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)/ospi
    ifeq ($(BUILD_QNX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_ospi_qnx ECU_BUILD=$(ECU_BUILD) TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
        ifeq ($(ECU_BUILD), no)
			cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/ospi/sbl_boot_app_ospi_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/ospi/sbl_boot_app_ospi_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage
        else
			cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/ospi/sbl_boot_app_ospi_$(ECU_BUILD)_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/ospi/sbl_boot_app_ospi_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage
        endif
    endif
    ifeq ($(BUILD_LINUX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_ospi_linux TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) DISABLE_RECURSE_DEPS=no BOARD=$(BOARD) CORE=$(SBL_CORE) -s
		cp $(PDK_PATH)/packages/ti/boot/sbl/example/boot_app/binary/$(BOARD)/ospi/sbl_boot_app_ospi_linux_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage $(SBL_COMBINED_BOOTFILES_PATH)/ospi/sbl_boot_app_ospi_linux_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage
    endif

#############
### Lateapps -> generate lateapp1 and lateapp2
#############

sbl_vision_apps_rprc:
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)/rprcs

    # Generate RPRC files for each core's output
    ifeq ($(BUILD_CPU_MCU2_0),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu2_0.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu2_0.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu2_0.out.rprc
    endif
    ifeq ($(BUILD_CPU_MCU2_1),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu2_1.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu2_1.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu2_1.out.rprc
    endif
    ifeq ($(BUILD_CPU_MCU3_0),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu3_0.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu3_0.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu3_0.out.rprc
    endif
    ifeq ($(BUILD_CPU_MCU3_1),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu3_1.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu3_1.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu3_1.out.rprc
    endif
    ifeq ($(BUILD_CPU_MCU4_0),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu4_0.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu4_0.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu4_0.out.rprc
    endif
    ifeq ($(BUILD_CPU_MCU4_1),yes)
		$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu4_1.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_mcu4_1.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_mcu4_1.out.rprc
    endif
    ifeq ($(BUILD_CPU_C6x_1),yes)
		$(CGT6X_ROOT)/bin/strip6x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c6x_1.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c6x_1.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c6x_1.out.rprc
    endif
    ifeq ($(BUILD_CPU_C6x_2),yes)
		$(CGT6X_ROOT)/bin/strip6x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c6x_2.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c6x_2.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c6x_2.out.rprc
    endif
    ifeq ($(BUILD_CPU_C7x_1),yes)
		$(CGT7X_ROOT)/bin/strip7x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_1.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_1.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_1.out.rprc
    endif
    ifeq ($(BUILD_CPU_C7x_2),yes)
		$(CGT7X_ROOT)/bin/strip7x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_2.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_2.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_2.out.rprc
    endif
    ifeq ($(BUILD_CPU_C7x_3),yes)
		$(CGT7X_ROOT)/bin/strip7x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_3.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_3.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_3.out.rprc
    endif
    ifeq ($(BUILD_CPU_C7x_4),yes)
		$(CGT7X_ROOT)/bin/strip7x -p $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_4.out
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/vx_app_rtos_qnx_c7x_4.out $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/vx_app_rtos_qnx_c7x_4.out.rprc
    endif

sbl_lateapps_qnx: sbl_vision_apps_rprc
	$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/lateapp1 $(REMOTE_CORE_LIST_LATEAPP1_COMBINED)
	$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/lateapp2 $(REMOTE_CORE_LIST_LATEAPP2_COMBINED)


##############
### QNX appimage -> Generate ifs_qnx.appimage and atf_optee.appimage
##############
sbl_qnx_appimage: sbl_combined_atf_optee
    ifeq ($(BUILD_QNX_MPU), yes)
		mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)/rprcs
		curr_dir=$(PWD)
    ifeq ($(USE_OPTEE),$(filter $(USE_OPTEE), 1))
		@echo "Building QNX AppImage with ATF + OPTEE"
		cd $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir && \
		$(QNX_BASE)/host/linux/x86_64/usr/bin/$(QNX_CROSS_COMPILER_TOOL)ld -T $(LDS_PATH)/atf_optee.lds -o $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_optee.elf && \
		cd $(QNX_BOOT_PATH) && \
		$(QNX_BASE)/host/linux/x86_64/usr/bin/$(QNX_CROSS_COMPILER_TOOL)ld -T $(LDS_PATH)/ifs_qnx.lds -o $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.elf && \
		cd $(curr_dir)
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_optee.elf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_optee.rprc
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.elf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.rprc
		$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee.appimage 0 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_optee.rprc
		$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/ifs_qnx.appimage 0 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.rprc
    else
		@echo "Building QNX AppImage with ATF only"
		cd $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir && \
		$(QNX_BASE)/host/linux/x86_64/usr/bin/$(QNX_CROSS_COMPILER_TOOL)ld -T $(LDS_PATH)/atf_only.lds -o $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_only.elf && \
		cd $(QNX_BOOT_PATH) && \
		$(QNX_BASE)/host/linux/x86_64/usr/bin/$(QNX_CROSS_COMPILER_TOOL)ld -T $(LDS_PATH)/ifs_qnx.lds -o $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.elf && \
		cd $(curr_dir)
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_only.elf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_only.rprc
		mono $(SBL_OUT2RPRC_GEN_TOOL_PATH)/out2rprc.exe $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.elf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.rprc
		$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee.appimage 0 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/atf_only.rprc
		$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE $(DEV_ID) $(SBL_COMBINED_BOOTFILES_PATH)/ifs_qnx.appimage 0 $(SBL_COMBINED_BOOTFILES_PATH)/rprcs/ifs_qnx.rprc
    endif
    endif

sbl_combined_atf_optee:
    ifeq ($(BUILD_QNX_MPU), yes)
    ifeq ($(USE_OPTEE),$(filter $(USE_OPTEE), 1))
		# For ATF, setting HANDLE_EA_EL3_FIRST_NS=0 for QNX so that the all runtime exception to be routed to current exception level (or in EL1 if the current exception level is EL0)
		$(MAKE) -C $(VISION_APPS_PATH)/../trusted-firmware-a -s -j32 CROSS_COMPILE=$(CROSS_COMPILE) CC="$(CROSS_COMPILE)gcc --sysroot=$(LINUX_SYSROOT_ARM)" PLAT=k3 TARGET_BOARD=$(ATF_TARGET_BOARD) SPD=opteed HANDLE_EA_EL3_FIRST_NS=0 K3_USART=$(K3_USART)
    else
		# For ATF, setting HANDLE_EA_EL3_FIRST_NS=0 for QNX so that the all runtime exception to be routed to current exception level (or in EL1 if the current exception level is EL0)
		$(MAKE) -C $(VISION_APPS_PATH)/../trusted-firmware-a -s -j32 CROSS_COMPILE=$(CROSS_COMPILE) CC="$(CROSS_COMPILE)gcc --sysroot=$(LINUX_SYSROOT_ARM)" PLAT=k3 TARGET_BOARD=$(ATF_TARGET_BOARD) HANDLE_EA_EL3_FIRST_NS=0 K3_USART=$(K3_USART)
    endif
    endif

    ifeq ($(BUILD_LINUX_MPU), yes)
    ifeq ($(USE_OPTEE),$(filter $(USE_OPTEE), 1))
		$(MAKE) -C $(VISION_APPS_PATH)/../optee_os -s -j32 CROSS_COMPILE_core=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-none-linux-gnu- CROSS_COMPILE_ta_arm32=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-none-linux-gnu- CROSS_COMPILE_ta_arm64=$(GCC_LINUX_ARM_ROOT)/bin/aarch64-none-linux-gnu- NOWERROR=1 CFG_TEE_TA_LOG_LEVEL=0 CFG_TEE_CORE_LOG_LEVEL=2 CFG_ARM64_core=y ta-targets=ta_arm64 PLATFORM=k3 PLATFORM_FLAVOR=j7 CFG_CONSOLE_UART=$(CFG_CONSOLE_UART)
    endif
    endif

	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir
	cp $(VISION_APPS_PATH)/../trusted-firmware-a/build/k3/$(ATF_TARGET_BOARD)/release/bl31.bin $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir/bl31.bin

    ifeq ($(USE_OPTEE),$(filter $(USE_OPTEE), 1))
		cp $(VISION_APPS_PATH)/../optee_os/out/arm-plat-k3/core/tee-pager_v2.bin $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir/bl32.bin
    endif


##############
### clean up
##############
sbl_combined_bootimage_clean: sbl_combined_bootimage_scrub
sbl_combined_bootimage_scrub: sbl_sd_hlos_clean sbl_ospi_hlos_clean sbl_bootapp_clean sbl_lateapps_qnx_clean sbl_qnx_appimage_clean
	rm -rf $(VISION_APPS_PATH)/out/sbl_combined_bootfiles/

sbl_sd_hlos_clean:
	$(MAKE) -C $(PDK_PATH)/packages/ti/build sbl_mmcsd_img_clean TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) BOARD=$(BOARD) CORE=$(SBL_CORE)
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/tiboot3.bin
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/tifs.bin

sbl_ospi_hlos_clean:
	$(MAKE) -C $(PDK_PATH)/packages/ti/build sbl_ospi_img_clean TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) BOARD=$(BOARD) CORE=$(SBL_CORE)
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/sbl_boot_app_ospi_qnx_$(BOARD)_$(SBL_CORE)_freertos_TestApp_release.appimage
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/tifs.bin

sbl_bootapp_clean:
    ifeq ($(BUILD_QNX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_mmcsd_qnx_clean TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) BOARD=$(BOARD) CORE=$(SBL_CORE) -s
    endif
    ifeq ($(BUILD_LINUX_MPU), yes)
		$(MAKE) -C $(PDK_PATH)/packages/ti/build boot_app_mmcsd_linux_clean TOOLS_INSTALL_PATH=$(PSDK_TOOLS_PATH) BOARD=$(BOARD) CORE=$(SBL_CORE) -s
    endif
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/app

sbl_lateapps_qnx_clean:
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/lateapp*
	rm -rf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs

sbl_qnx_appimage_clean:
	rm -rf $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee_dir
	rm -rf $(SBL_COMBINED_BOOTFILES_PATH)/rprcs
	$(MAKE) -C $(VISION_APPS_PATH)/../trusted-firmware-a clean
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/atf_optee.appimage
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/ifs_qnx.appimage
	rm -rf $(VISION_APPS_PATH)/../trusted-firmware-a/build/k3/$(ATF_TARGET_BOARD)
    ifeq ($(USE_OPTEE),$(filter $(USE_OPTEE), 1))
		rm -rf $(VISION_APPS_PATH)/../optee_os/out/arm-plat-k3
    endif


endif # ifeq ($(RTOS_SDK),pdk)

#######################################################################################
#################################[ MCU PLUS SDK SBL ]##################################

# Check if the RTOS SDK is set to mcu_plus_sdk and BUILD_QNX_MPU is enabled
ifeq ($(RTOS_SDK),mcu_plus_sdk)
ifeq ($(BUILD_QNX_MPU), yes)

# Include the configuration file for the specified SOC
ifeq ($(SOC), am62a)
	include $(MCU_PLUS_SDK_PATH)/tools/boot/qnxAppimageGen/board/am62ax-sk/config.mak
else
	include $(MCU_PLUS_SDK_PATH)/tools/boot/qnxAppimageGen/board/$(SOC)-evm/config.mak
endif

# Define paths and commands for generating RPRC images and SBL binaries
SBL_COMBINED_BOOTFILES_PATH = $(VISION_APPS_PATH)/out/sbl_combined_bootfiles
OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_SDK_PATH)/tools/boot/out2rprc/elf2rprc.js
INPUT_IMG_PATH = $(VISION_APPS_PATH)/out/sbl_combined_bootfiles/vision_apps

ifeq ($(ECU_BUILD), no)
SBL_SD_HLOS_PATH = $(MCU_PLUS_SDK_PATH)/examples/drivers/boot/sbl_sd_hlos/$(SOC)-evm/wkup-r5fss0-0_nortos/ti-arm-clang
SBL_SD_HLOS_OUT_IMG = $(SBL_SD_HLOS_PATH)/sbl_sd_hlos.release.hs_fs.tiimage
SBL_OSPI_HLOS_PATH = $(MCU_PLUS_SDK_PATH)/examples/drivers/boot/sbl_ospi_hlos/$(SOC)-evm/wkup-r5fss0-0_nortos/ti-arm-clang
SBL_OSPI_HLOS_OUT_IMG = $(SBL_OSPI_HLOS_PATH)/sbl_ospi_hlos.release.hs_fs.tiimage
else
SBL_SD_HLOS_PATH = $(MCU_PLUS_SDK_PATH)/examples/drivers/boot/sbl_sd_hlos_${ECU_BUILD}/$(SOC)-evm/wkup-r5fss0-0_nortos/ti-arm-clang
SBL_SD_HLOS_OUT_IMG = $(SBL_SD_HLOS_PATH)/sbl_sd_hlos_${ECU_BUILD}.release.hs_fs.tiimage
SBL_OSPI_HLOS_PATH = $(MCU_PLUS_SDK_PATH)/examples/drivers/boot/sbl_ospi_hlos_${ECU_BUILD}/$(SOC)-evm/wkup-r5fss0-0_nortos/ti-arm-clang
SBL_OSPI_HLOS_OUT_IMG = $(SBL_OSPI_HLOS_PATH)/sbl_ospi_hlos_${ECU_BUILD}.release.hs_fs.tiimage
endif

SBL_UART_UNIFLASH_PATH = $(MCU_PLUS_SDK_PATH)/examples/drivers/boot/sbl_uart_uniflash/$(SOC)-evm/wkup-r5fss0-0_nortos/ti-arm-clang/
SBL_UART_UNIFLASH_IMG = $(SBL_UART_UNIFLASH_PATH)/sbl_uart_uniflash.release.hs_fs.tiimage
QNX_COMBINED_APP_IMG = $(MCU_PLUS_SDK_PATH)/tools/boot/qnxAppimageGen/board/$(SOC)-evm/qnx.appimage.hs_fs

# Ensure required variables are defined, else throw an error
ifndef MCU_PLUS_SDK_PATH
    $(error MCU_PLUS_SDK_PATH is not defined)
endif
ifndef VISION_APPS_PATH
    $(error VISION_APPS_PATH is not defined)
endif
ifndef SOC
    $(error SOC is not defined)
endif
ifndef QNX_BOOT_PATH
    $(error QNX_BOOT_PATH is not defined)
endif
ifndef QNX_SD_FS_BOOT_PATH
    $(error QNX_SD_FS_BOOT_PATH is not defined)
endif

# Define input images for R5F and C7x cores
IMAGE_MCU2_0 = $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/FREERTOS/release/vx_app_rtos_qnx_mcu2_0.out
IMAGE_C7X_1 = $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C7524/FREERTOS/release/vx_app_rtos_qnx_c7x_1.out
IMAGE_C7X_2 = $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C7524/FREERTOS/release/vx_app_rtos_qnx_c7x_2.out

# Define output RPRC files for R5F and C7x cores
RPRC_MCU2_0 = $(INPUT_IMG_PATH)/vx_app_rtos_qnx_mcu2_0.rprc
RPRC_C7X_1 = $(INPUT_IMG_PATH)/vx_app_rtos_qnx_c7x_1.rprc
RPRC_C7X_2 = $(INPUT_IMG_PATH)/vx_app_rtos_qnx_c7x_2.rprc

# Define the list of images to be included in the RTOS image list
IMG1 = $(BOOTIMAGE_CORE_ID_wkup-r5fss0-0) $(MCU_PLUS_SDK_PATH)/tools/sysfw/sciserver_binary/$(SOC)/sciclient_get_version.release.rprc
ifeq ($(BUILD_CPU_MCU2_0),yes)
    IMG2=$(BOOTIMAGE_CORE_ID_main-r5fss0-0) $(RPRC_MCU2_0)
else
    IMG2=
endif
ifeq ($(BUILD_CPU_C7x_1),yes)
    IMG3 = $(BOOTIMAGE_CORE_ID_c75ss0-0) $(RPRC_C7X_1)
else
    IMG3 =
endif
ifeq ($(BUILD_CPU_C7x_2),yes)
    IMG4 = $(BOOTIMAGE_CORE_ID_c75ss1-0) $(RPRC_C7X_2)
else
    IMG4 =
endif

RTOS_IMG_LIST = $(IMG1) $(IMG2) $(IMG3) $(IMG4)

# Build the SBL and app image
sbl: sbl_appimage_hlos sbl_sd_hlos sbl_ospi_hlos

# Clean up generated files
sbl_clean: sbl_scrub
sbl_scrub: sbl_appimage_hlos_clean sbl_sd_hlos_clean sbl_ospi_hlos_clean
	rm -rf $(SBL_COMBINED_BOOTFILES_PATH)

# Generate the SBL MMC SD binary
sbl_sd_hlos:
	@echo "Generating SBL MMC SD Binary..."
	make -C $(SBL_SD_HLOS_PATH) clean
	make -C $(SBL_SD_HLOS_PATH)

	@echo "Copying SBL SD HLOS binary to combined bootfiles path..."
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)
	cp $(SBL_SD_HLOS_OUT_IMG) $(SBL_COMBINED_BOOTFILES_PATH)/sbl_sd_hlos.release.hs_fs.tiimage

# Generate the OSPI binary
sbl_ospi_hlos:
	@echo "Generating SBL OSPI Binary..."
	make -C $(SBL_OSPI_HLOS_PATH) clean
	make -C $(SBL_OSPI_HLOS_PATH)
	make -C ${SBL_UART_UNIFLASH_PATH} clean
	make -C ${SBL_UART_UNIFLASH_PATH}

	@echo "Copying SBL OSPI HLOS binary to combined bootfiles path..."
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)
	cp $(SBL_OSPI_HLOS_OUT_IMG) $(SBL_COMBINED_BOOTFILES_PATH)/sbl_ospi_hlos.release.hs_fs.tiimage
	cp $(SBL_UART_UNIFLASH_IMG) $(SBL_COMBINED_BOOTFILES_PATH)/sbl_uart_uniflash.release.hs_fs.tiimage

# Generate RPRC images for vision apps
sbl_vision_apps_rprc:
	mkdir -p $(INPUT_IMG_PATH)
	@echo "SOC=$(SOC)"
	@echo "SW_VERSION=$(SW_VERSION)"
	$(call GENERATE_OUT_TO_RPRC, MCU2_0, $(IMAGE_MCU2_0), $(RPRC_MCU2_0))
	$(call GENERATE_OUT_TO_RPRC, C7X_1, $(IMAGE_C7X_1), $(RPRC_C7X_1))
	$(call GENERATE_OUT_TO_RPRC, C7X_2, $(IMAGE_C7X_2), $(RPRC_C7X_2))
	@echo "RPRC generation completed."

# Generate the combined app image for QNX
sbl_appimage_hlos: sbl_vision_apps_rprc
	@echo "------------------------------------------------------"
	@echo "Generating combined app image for QNX..."
	@echo "------------------------------------------------------"

	@echo "Copying QNX IFS image to prebuilt bin path..."
	mkdir -p ${MCU_PLUS_SDK_PATH}/tools/boot/hlos_prebuilt/${SOC}-evm/qnx/
	cp ${QNX_BOOT_PATH}/qnx-ifs ${MCU_PLUS_SDK_PATH}/tools/boot/hlos_prebuilt/${SOC}-evm/qnx/;

	@echo "Generating multicore hlos qnx app image..."
	$(MAKE) -C $(MCU_PLUS_SDK_PATH)/tools/boot/qnxAppimageGen BOARD=${SOC}-evm clean
	$(MAKE) -C $(MCU_PLUS_SDK_PATH)/tools/boot/qnxAppimageGen BOARD=${SOC}-evm RTOS_IMG_LIST="$(RTOS_IMG_LIST)"

	@echo "------------------------------------------------------"
	@echo "multicore hlos qnx app image generated successfully."
	@echo "------------------------------------------------------"

	@echo "Copying generated qnx app image to combined bootfiles path..."
	mkdir -p $(SBL_COMBINED_BOOTFILES_PATH)
	cp ${QNX_COMBINED_APP_IMG} $(SBL_COMBINED_BOOTFILES_PATH)/qnx.appimage.hs_fs

# Clean up app image files
sbl_appimage_hlos_clean:
	rm -f ${MCU_PLUS_SDK_PATH}/tools/boot/hlos_prebuilt/${SOC}-evm/qnx/qnx-ifs
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/qnx.appimage.hs_fs
	rm -rf $(INPUT_IMG_PATH)

# Clean up SBL SD HLOS files
sbl_sd_hlos_clean:
	make -C $(SBL_SD_HLOS_PATH) clean
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/sbl_sd_hlos.release.hs_fs.tiimage

# Clean up SBL OSPI HLOS files
sbl_ospi_hlos_clean:
	make -C $(SBL_OSPI_HLOS_PATH) clean
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/sbl_ospi_hlos.release.hs_fs.tiimage
	rm -f $(SBL_COMBINED_BOOTFILES_PATH)/sbl_uart_uniflash.release.hs_fs.tiimage

# Copy SBL and QNX app image to SD card
sbl_bootimage_install_sd:
	@echo "Copying SBL SD HLOS for ECU build to SD card..."
	cp $(SBL_COMBINED_BOOTFILES_PATH)/sbl_sd_hlos.release.hs_fs.tiimage $(QNX_SD_FS_BOOT_PATH)/tiboot3.bin

	@echo "Copying QNX app image for ECU build to SD card..."
	cp $(SBL_COMBINED_BOOTFILES_PATH)/qnx.appimage.hs_fs $(QNX_SD_FS_BOOT_PATH)/app
	sync

	@echo "SBL tiboot3.bin and QNX app image copied to SD card successfully."

# Flash SBL OSPI HLOS to the device
sbl_bootimage_install_ospi:
	@echo "Flashing SBL OSPI HLOS for ECU build to device..."
	@read -p "Enter UART port (default: /dev/ttyUSB2): " input; \
	UART_FLASH_PORT=$${input:-/dev/ttyUSB2}; \
	echo "Using UART port: $$UART_FLASH_PORT"; \
	cd $(MCU_PLUS_SDK_PATH)/tools/boot && \
	python uart_uniflash.py -p $$UART_FLASH_PORT --cfg=sbl_prebuilt/j722s-evm/default_sbl_ospi_qnx_hs_fs_fc.cfg

# Macro to generate RPRC images
define GENERATE_OUT_TO_RPRC =
	@echo "Creating $(1) RPRC image"
	@if [ -f $(2) ]; then \
		$(OUTRPRC_CMD) $(2) $(SW_VERSION) $(3); \
		echo "$(1) RPRC image created: $(3)"; \
	else \
		echo "Warning: $(1) image $(2) does not exist. Skipping RPRC creation."; \
	fi
endef

endif  # ifeq ($(BUILD_QNX_MPU), yes)
endif  # ifeq ($(RTOS_SDK),mcu_plus_sdk)
