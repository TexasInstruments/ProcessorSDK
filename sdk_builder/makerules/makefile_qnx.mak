#
# Utility makefile to build QNX libaries and related components
#
# Edit this file to suit your specific build needs
#

QNX_AUX_FS_PATH=$(PSDK_QNX_PATH)/rootfs

ifeq ($(PROFILE), $(filter $(PROFILE),debug))
QNX_APP_PROFILE=debug
endif
ifeq ($(PROFILE), $(filter $(PROFILE),release all))
QNX_APP_PROFILE=release
endif

ifeq ($(SOC),j721e)
QNX_FIRMWARE_PREFIX=j7
else
QNX_FIRMWARE_PREFIX=$(SOC)
endif

qnx:
ifeq ($(BUILD_QNX_MPU),yes)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx all        VISION_APPS_BUILD_FLAGS_MAK=$(VISION_APPS_BUILD_FLAGS_MAK) QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD) -s
endif

qnx_clean:
ifeq ($(BUILD_QNX_MPU),yes)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx clean      QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD)
endif

qnx_scrub:
ifeq ($(BUILD_QNX_MPU),yes)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx scrub      QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD)
endif

qnx_fs_create:
ifeq ($(BUILD_QNX_MPU),yes)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx qnx_fs_clean       QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx qnx_fs_create      QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD)
endif

qnx_fs_copy_spl_uboot:
ifeq ($(BUILD_QNX_MPU),yes)
	$(MAKE) -C $(PSDK_QNX_PATH)/qnx qnx_fs_copy_spl_uboot      QNX_BASE=$(QNX_BASE) PROFILE=$(PROFILE) BOARD=$(BOARD)
endif

qnx_fs_install:
ifeq ($(BUILD_CPU_MPU1),yes)
	# copy application binaries and scripts
	mkdir -p $(QNX_FS_PATH)/vision_apps
	mkdir -p $(QNX_FS_PATH)/tilib
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(MPU_CPU)/QNX/$(QNX_APP_PROFILE)/*.out $(QNX_FS_PATH)/vision_apps
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(MPU_CPU)/QNX/$(LINUX_APP_PROFILE)/libtivision_apps.so.$(PSDK_VERSION) $(QNX_FS_PATH)/tilib
	# app_linux_fs_files are not very OS specific. Only input file paths change
	cp -r $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/vision_apps_all/* $(QNX_FS_PATH)/vision_apps
	cp -r $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/vision_apps_evm/* $(QNX_FS_PATH)/vision_apps
	-cp -r $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/*.txt $(QNX_FS_PATH)/vision_apps
	cp $(VISION_APPS_PATH)/apps/basic_demos/app_linux_fs_files/vision_apps_init.sh $(QNX_FS_PATH)/vision_apps/.
	# Rename file paths in app cfg files
	sed -i 's/\/opt\//\/ti_fs\//g' $(QNX_FS_PATH)/vision_apps/*.cfg
	sed -i 's/\/opt\//\/ti_fs\//g' $(QNX_FS_PATH)/vision_apps/app_srv_avp_cfg/*.cfg
	sed -i 's/\/opt\//\/ti_fs\//g' $(QNX_FS_PATH)/vision_apps/*.sh
	# Remove files not needed for QNX
	rm -rf $(QNX_FS_PATH)/vision_apps/limits.conf

	chmod +x $(QNX_FS_PATH)/vision_apps/*.sh

	$(MAKE) EDGEAI_INSTALL_PATH=$(QNX_FS_PATH) edgeai_qnx_install
endif

	# remove old remote firmware files from filesystem
	mkdir -p $(QNX_AUX_FS_PATH)/lib/firmware
	-rm $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-*-fw -f

ifeq ($(BUILD_CPU_MCU1_0),yes)
ifneq ($(SOC),j722s)
	# copy remote firmware files for mcu1_0
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu1_0.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-mcu-r5f0_0-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-mcu-r5f0_0-fw
endif
endif

ifeq ($(BUILD_CPU_MCU2_0),yes)
	# copy remote firmware files for mcu2_0
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu2_0.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_0-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_0-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_0-fw-sec
endif
endif
ifeq ($(BUILD_CPU_MCU2_1),yes)
	# copy remote firmware files for mcu2_1
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu2_1.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_1-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_1-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f0_1-fw-sec
endif
endif
ifeq ($(BUILD_CPU_MCU3_0),yes)
	# copy remote firmware files for mcu3_0
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu3_0.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_0-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_0-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_0-fw-sec
endif
endif
ifeq ($(BUILD_CPU_MCU3_1),yes)
	# copy remote firmware files for mcu3_1
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu3_1.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_1-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_1-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f1_1-fw-sec
endif
endif
ifeq ($(BUILD_CPU_MCU4_0),yes)
	# copy remote firmware files for mcu4_0
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu4_0.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_0-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_0-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_0-fw-sec
endif
endif
ifeq ($(BUILD_CPU_MCU4_1),yes)
	# copy remote firmware files for mcu4_1
	$(eval IMAGE_NAME := vx_app_rtos_qnx_mcu4_1.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_1-fw
	$(TIARMCGT_LLVM_ROOT)/bin/tiarmstrip -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_1-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/R5F/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-main-r5f2_1-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C6x_1),yes)
	# copy remote firmware files for c6x_1
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c6x_1.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_0-fw
	$(CGT6X_ROOT)/bin/strip6x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_0-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_0-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C6x_2),yes)
	# copy remote firmware files for c6x_2
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c6x_2.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_1-fw
	$(CGT6X_ROOT)/bin/strip6x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_1-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/C66/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c66_1-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C7x_1),yes)
	# copy remote firmware files for c7x_1
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c7x_1.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_0-fw
	$(CGT7X_ROOT)/bin/strip7x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_0-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_0-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C7x_2),yes)
	# copy remote firmware files for c7x_2
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c7x_2.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_1-fw
	$(CGT7X_ROOT)/bin/strip7x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_1-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_1-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C7x_3),yes)
	# copy remote firmware files for c7x_3
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c7x_3.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_2-fw
	$(CGT7X_ROOT)/bin/strip7x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_2-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_2-fw-sec
endif
endif
ifeq ($(BUILD_CPU_C7x_4),yes)
	# copy remote firmware files for c7x_4
	$(eval IMAGE_NAME := vx_app_rtos_qnx_c7x_4.out)
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_3-fw
	$(CGT7X_ROOT)/bin/strip7x -p $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_3-fw
ifeq ($(HS),1)
	$(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME) $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed
	cp $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(C7X_TARGET)/$(RTOS)/$(QNX_APP_PROFILE)/$(IMAGE_NAME).signed $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-c71_3-fw-sec
endif
endif
	sync


# MODIFY_QNX_SD_FS macro for making PSDK RTOS modifications to the QNX to file system
define MODIFY_QNX_SD_FS =
	# copy all staged files
	cp -rfv $(QNX_BOOT_PATH)/* $(QNX_SD_FS_BOOT_PATH)/
	cp -rfv $(QNX_FS_PATH)/* $(QNX_SD_FS_QNX_PATH)/
	# copy vision apps binaries
	mkdir -p $(QNX_SD_FS_QNX_PATH)/vision_apps
	cp -r $(QNX_FS_PATH)/vision_apps $(QNX_SD_FS_QNX_PATH)/
	sync
endef

qnx_fs_create_sd: qnx_fs_create
	sudo chmod 777 -R $(QNX_SD_FS_ROOT_PATH)
	rm -rf $(QNX_SD_FS_BOOT_PATH)/*
	rm -rf $(QNX_SD_FS_QNX_PATH)/*
	rm -rf $(QNX_SD_FS_ROOT_PATH)/*
	cp -rfv $(QNX_BOOT_PATH)/* $(QNX_SD_FS_BOOT_PATH)/
	cp -rfv $(QNX_FS_PATH)/* $(QNX_SD_FS_QNX_PATH)/

qnx_fs_install_firmware:
	# copy remote core firmware's
	mkdir -p $(QNX_SD_FS_ROOT_PATH)/lib/firmware
	rm $(QNX_SD_FS_ROOT_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-*-fw -f
	cp -v $(QNX_AUX_FS_PATH)/lib/firmware/$(QNX_FIRMWARE_PREFIX)-*-fw $(QNX_SD_FS_ROOT_PATH)/lib/firmware/
	sync

qnx_fs_install_sd: qnx_fs_copy_spl_uboot qnx_fs_install qnx_fs_install_firmware
	$(call MODIFY_QNX_SD_FS)
ifeq ($(BUILD_CPU_MCU1_0),yes)
ifeq ($(BUILD_TARGET_MODE),yes)
	$(call UBOOT_INSTALL,qnx,$(QNX_SD_FS_BOOT_PATH))
endif
endif
	sync
	$(call SET_AUTORUN_ECU_TO_QNX_SD_FS)

qnx_fs_install_nfs: qnx_fs_copy_spl_uboot qnx_fs_install
ifeq ($(BUILD_CPU_MCU1_0),yes)
ifeq ($(BUILD_TARGET_MODE),yes)
	$(call UBOOT_INSTALL,qnx,$(QNX_BOOT_PATH))
endif
endif
	sync

qnx_fs_install_nfs_test_data:
	$(call INSTALL_TEST_DATA,$(QNX_FS_PATH),vision_apps)
	sync

qnx_fs_install_tar: qnx_fs_install_nfs qnx_fs_install_nfs_test_data
	# Creating bootfs tar
	cd $(QNX_BOOT_PATH) && tar czf $(VISION_APPS_PATH)/spl_bootfs.tar.gz .
	# Creating qnxfs tar
	cd $(QNX_FS_PATH) && tar czf $(VISION_APPS_PATH)/spl_qnxfs.tar.gz .
	# Creating rootfs tar
	cd $(QNX_AUX_FS_PATH) && sudo tar cpzf $(VISION_APPS_PATH)/spl_rootfs.tar.xz .

qnx_fs_install_nfs_tar: qnx_fs_install_tar
	# Creating rootfs tar
	mkdir -p $(QNX_AUX_FS_PATH)/qnxfs
	mkdir -p $(QNX_AUX_FS_PATH)/bootfs
	cp -rfv $(QNX_FS_PATH)/* $(QNX_AUX_FS_PATH)/qnxfs
	cp -rfv $(QNX_BOOT_PATH)/* $(QNX_AUX_FS_PATH)/bootfs

	# using lzma compression, but parallelized to increase performance (-I 'xz -T0')
	# (-J would do lzma compression, but without parallelization)
	cd $(QNX_AUX_FS_PATH) && sudo tar -I 'xz -T0' -cpf $(VISION_APPS_PATH)/spl_rootfs_nfs.tar.xz .

	# Cleanup rootfs directory
	rm -rf $(QNX_AUX_FS_PATH)/bootfs
	rm -rf $(QNX_AUX_FS_PATH)/qnxfs

qnx_fs_install_sd_sbl: qnx_fs_install sbl_bootimage_install_sd
	$(call MODIFY_QNX_SD_FS)

qnx_fs_install_sd_sbl_hs: qnx_fs_install sbl_bootimage_hs_install_sd
	$(call MODIFY_QNX_SD_FS)

qnx_fs_install_ospi: qnx_fs_install sbl_bootimage_install_ospi
	$(call MODIFY_QNX_SD_FS)

qnx_fs_install_sd_test_data:
	$(call INSTALL_TEST_DATA,$(QNX_SD_FS_QNX_PATH),vision_apps)

qnx_fs_install_sd_sbl_combined: qnx_fs_install sbl_combined_bootimage_install_sd
	$(call MODIFY_QNX_SD_FS)

.PHONY: qnx qnx_clean qnx_scrub
