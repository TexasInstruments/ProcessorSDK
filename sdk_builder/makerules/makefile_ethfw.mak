#
# Utility makefile to build ethfw libraries
#
# Edit this file to suit your specific build needs
#

ethfw:
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(RTOS),FREERTOS)
	$(MAKE) -C $(ETHFW_PATH) PSDK_TOOLS_PATH=$(PSDK_TOOLS_PATH) BUILD_APP_SAFERTOS=no BUILD_APP_FREERTOS=yes ethfw_board ethfw_callbacks eth_intervlan ethfw_common ethfw_abstract ethfw_remotecfg_server ethfw_estdemo BUILD_CPU_MCU2_1=no BUILD_SOC_LIST=$(TARGET_SOC) PROFILE=$(PROFILE) -s
endif
ifeq ($(RTOS),SAFERTOS)
	$(MAKE) -C $(ETHFW_PATH) PSDK_TOOLS_PATH=$(PSDK_TOOLS_PATH) BUILD_APP_SAFERTOS=yes BUILD_APP_FREERTOS=no ethfw_board ethfw_callbacks eth_intervlan ethfw_common ethfw_abstract ethfw_remotecfg_server ethfw_estdemo BUILD_CPU_MCU2_1=no BUILD_SOC_LIST=$(TARGET_SOC) PROFILE=$(PROFILE) -s
endif
endif

ethfw_clean:
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(RTOS),FREERTOS)
	$(MAKE) -C $(ETHFW_PATH) PSDK_TOOLS_PATH=$(PSDK_TOOLS_PATH) BUILD_APP_SAFERTOS=no BUILD_APP_FREERTOS=yes clean -s
endif
ifeq ($(RTOS),SAFERTOS)
	$(MAKE) -C $(ETHFW_PATH) PSDK_TOOLS_PATH=$(PSDK_TOOLS_PATH) BUILD_APP_SAFERTOS=yes BUILD_APP_FREERTOS=no clean -s
endif
endif

ethfw_scrub:
ifeq ($(BUILD_TARGET_MODE),yes)
	rm -rf $(ETHFW_PATH)/out $(ETHFW_PATH)/lib
endif

.PHONY: ethfw ethfw_clean ethfw_scrub
