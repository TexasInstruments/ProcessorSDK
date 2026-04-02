#
# Makefile for generating multicore app for Memory Benchmarking Apps
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make


APP_NAME = $(MEM)_multicore_memory_benchmarking_app_freertos
SRCDIR += $(PDK_CSL_COMP_PATH)/example/ospi/memory_benchmarking_apps/$(MEM)_memory_benchmarking
RPRC_PREFIX = $(MEM)_dual_core_memory_benchmarking_app_freertos
INPUT_BINARY_PATH = $(pdk_PATH)/ti/binary/$(RPRC_PREFIX)/bin/$(BOARD)
OUTPUT_BINARY_PATH = $(pdk_PATH)/ti/binary/$(APP_NAME)/bin/$(BOARD)
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# Populate the string CORE_ID followed by RPRC image for all single core images
MULTICORE_IMG_PARAMS_XIP = $(foreach SOC_CORE_ID, $($(MEM)_dual_core_memory_benchmarking_app_freertos_$(SOC)_CORELIST), $(SBL_CORE_ID_$(SOC_CORE_ID)) $(INPUT_BINARY_PATH)/$(RPRC_PREFIX)_$(SOC_CORE_ID)_$(BUILD_PROFILE).rprc_xip)
MULTICORE_IMG_PARAMS = $(foreach SOC_CORE_ID, $($(MEM)_dual_core_memory_benchmarking_app_freertos_$(SOC)_CORELIST), $(SBL_CORE_ID_$(SOC_CORE_ID)) $(INPUT_BINARY_PATH)/$(RPRC_PREFIX)_$(SOC_CORE_ID)_$(BUILD_PROFILE).rprc)


# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

# List all the components required by the application
COMP_LIST_COMMON =

SRCS_COMMON = force_multi_core_img_gen.c

force_multi_core_img_gen.c:

	$(MKDIR) -p $(OUTPUT_BINARY_PATH)

	$(ECHO) "# Combining RPRC images to generate multicore image...."
	$(ECHO) "# INPUT_BINARY_PATH is $(INPUT_BINARY_PATH)"
	$(ECHO) "# CORELIST is $($(MEM)_dual_core_memory_benchmarking_app_freertos_$(SOC)_CORELIST)"
	$(ECHO) "# MULTICORE_IMG_PARAMS are $(MULTICORE_IMG_PARAMS)"
	$(SBL_IMAGE_GEN) LE $(SBL_DEV_ID) $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage $(MULTICORE_IMG_PARAMS)
	$(ECHO) "#"
	$(ECHO) "# Dual Core Memory Benchmarking App image $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage created."
	$(ECHO) "#"
	$(ECHO) "# Signing the multicore image...."
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SBL_CERT_GEN)
endif
	$(SBL_CERT_GEN) -b $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage -o $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage.signed -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY_HS)

ifeq ($(MEM), xip)
	$(ECHO) "# Combining RPRC images to generate multicore xip image...."
	$(ECHO) "# INPUT_BINARY_PATH is $(INPUT_BINARY_PATH)"
	$(ECHO) "# CORELIST is $($(MEM)_dual_core_memory_benchmarking_app_freertos_$(SOC)_CORELIST)"
	$(ECHO) "# MULTICORE_IMG_PARAMS are $(MULTICORE_IMG_PARAMS_XIP)"
	$(SBL_IMAGE_GEN) LE $(SBL_DEV_ID) $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage_xip $(MULTICORE_IMG_PARAMS_XIP)
	$(ECHO) "#"
	$(ECHO) "# Dual Core Memory Benchmarking App image $(OUTPUT_BINARY_PATH)/$(APP_NAME).appimage_xip created."
endif

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# Nothing beyond this point
