#
# This file is the makefile used for creating sciclient merged combined testapp's .appimage 
# for non-mcu1_0 R5 core.
#
# Pre-requisite: sciclient_merged_testapp's .rprc file of mcu1_0 core should be present.
#
# Brief: This makefile does not build sciclient_merged_testapp's .rprc file for given core.
# It will consume .rprc file created for mcu1_0 core using sciclient_merged_testapp target.
# This makefile only packs and creates sciclient_merged_combined_testapp's .appimage for given 
# non-mcu1_0 R5 core. It is done for validating multicore support of sciclient_merged library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = sciclient_merged_combined_testapp_$(BUILD_OS_TYPE)
COMP_LIST_COMMON =

.PHONY: gen_appimage create_app_directory

# When building app for cores other than MCU1_0, then MCU1_0 should host sciclient server.
# Not required when running app on MCU1_0.
# Please refer the user guide for more details on sciclient server.

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS = $(SBL_CORE_ID_mcu1_0) $(PDK_INSTALL_PATH)/ti/drv/sciclient/tools/ccsLoadDmsc/$(SOC)/sciserver_testapp_freertos_mcu1_0_release.rprc
else
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS =
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS =
endif

# MCUx_1 cores require a dummy application to run from MCUx_0 core
# as MCUx_1 core cannot be at a higher power state than MCUx_0 core.
# In case of mcu1_1, mcu1_0 always host the server, so dummy app is not needed for MCU1_0
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE),$(filter $(CORE), mcu2_1))
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS += $(SBL_CORE_ID_mcu2_0) $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sbl_mcux_0_dummy_app.rprc
  endif
  ifeq ($(CORE),$(filter $(CORE), mcu3_1))
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS += $(SBL_CORE_ID_mcu3_0) $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sbl_mcux_0_dummy_app.rprc
  endif
  ifeq ($(CORE),$(filter $(CORE), mcu4_1))
  SCI_CUSTOM_MULTI_CORE_APP_PARAMS += $(SBL_CORE_ID_mcu4_0) $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sbl_mcux_0_dummy_app.rprc
  endif
endif

SCI_CUSTOM_DIR = $(PDK_INSTALL_PATH)/ti/binary/$(APP_NAME)/bin/$(SOC)
SCI_CUSTOM_STDOUT_FILE = $(SCI_CUSTOM_DIR)/temp_stdout.txt
SCI_CUSTOM_RPRC_PATH = $(PDK_INSTALL_PATH)/ti/binary/sciclient_merged_testapp_$(BUILD_OS_TYPE)/bin/$(SOC)/sciclient_merged_testapp_$(BUILD_OS_TYPE)_mcu1_0_$(BUILD_PROFILE).rprc
SCI_CUSTOM_MULTI_CORE_APP_PARAMS += $(SBL_CORE_ID_$(CORE)) $(SCI_CUSTOM_RPRC_PATH)
SCI_CUSTOM_APPIMAGE_PATH = $(SCI_CUSTOM_DIR)/$(APP_NAME)_$(CORE)_$(BUILD_PROFILE).appimage
SCI_CUSTOM_APPIMAGE_PATH_BE = $(SCI_CUSTOM_DIR)/$(APP_NAME)_$(CORE)_$(BUILD_PROFILE)_BE.appimage

gen_appimage: create_app_directory
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SBL_IMAGE_GEN)
endif
	$(SBL_IMAGE_GEN) LE $(SBL_DEV_ID) $(SCI_CUSTOM_APPIMAGE_PATH) $(SCI_CUSTOM_MULTI_CORE_APP_PARAMS) >> $(SCI_CUSTOM_STDOUT_FILE)
	$(SBL_IMAGE_GEN) BE $(SBL_DEV_ID) $(SCI_CUSTOM_APPIMAGE_PATH_BE) $(SCI_CUSTOM_MULTI_CORE_APP_PARAMS) >> $(SCI_CUSTOM_STDOUT_FILE)
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SBL_CERT_GEN)
endif
	$(SBL_CERT_GEN) -b $(SCI_CUSTOM_APPIMAGE_PATH) -o $(SCI_CUSTOM_APPIMAGE_PATH).signed -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY_HS)
	$(SBL_CERT_GEN) -b $(SCI_CUSTOM_APPIMAGE_PATH) -o $(SCI_CUSTOM_APPIMAGE_PATH).hs_fs -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY)

create_app_directory:
	$(MKDIR) -p $(SCI_CUSTOM_DIR)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<coqre/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
