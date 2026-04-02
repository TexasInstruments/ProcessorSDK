#
# This file is the makefile for building a testcase
# to boot all ipc images using SBL.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = ipc_baremetal_multicore_echo_test

SRCDIR += $(PDK_IPC_COMP_PATH)/examples/ipc_baremetal_echo_test

# Local name of IPC test app
RPRC_PREFIX_BAREMETAL = ipc_baremetal_echo_test
RPRC_PREFIX_RTOS = ipc_rtos_echo_test_freertos

define BIN_PATH_PREFIX_RULE
# baremetal ipc is supported for only mcu cores
# Instead of baremetal mcu1_0, FREERTOS mcu1_0 binary has been used for multicore app because of sciserver dependency
# For c66x and c7x cores FREERTOS binary has been used because baremetal ipc not supported on non-r5f cores.

BIN_PATH_PREFIX_$(1) = $(if $(findstring $(1), $(filter-out mcu1_0, $(drvipc_$(SOC)_BAREMETAL_CORELIST))),\
							$(pdk_PATH)/ti/binary/$(RPRC_PREFIX_BAREMETAL)/bin/$(BOARD)/$(RPRC_PREFIX_BAREMETAL),\
							$(pdk_PATH)/ti/binary/$(RPRC_PREFIX_RTOS)/bin/$(BOARD)/$(RPRC_PREFIX_RTOS))

endef

BIN_PATH_PREFIX_MACRO_LIST := $(foreach core, $(drvipc_$(SOC)_RTOS_CORELIST), $(call BIN_PATH_PREFIX_RULE,$(core)))

$(eval ${BIN_PATH_PREFIX_MACRO_LIST})

CORES_IN_TEST = $(filter-out mpu1_0, $(drvipc_$(SOC)_CORELIST))
MULTICORE_IMG_PARAMS = $(foreach SOC_CORE_ID, $(CORES_IN_TEST), $(SBL_CORE_ID_$(SOC_CORE_ID)) $(BIN_PATH_PREFIX_$(SOC_CORE_ID))_$(SOC_CORE_ID)_$(BUILD_PROFILE_$(CORE)).rprc)

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

# List all the components required by the application
COMP_LIST_COMMON =

SRCS_COMMON = force_multi_core_img_gen.c

force_multi_core_img_gen.c:
	$(ECHO) "# Combining RPRC images to generate multicore image...."
	$(ECHO) "# BINDIR is $(BINDIR) CORELIST is $(filter-out mpu1_0, $(drvipc_$(SOC)_CORELIST))"
	$(ECHO) "# MULTICORE_IMG_PARAMS are $(MULTICORE_IMG_PARAMS)"
	$(SBL_IMAGE_GEN) LE $(SBL_DEV_ID) $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage $(MULTICORE_IMG_PARAMS)
	$(ECHO) "#"
	$(ECHO) "# Multicore IPC App image $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage created."
	$(ECHO) "#"
	$(ECHO) "# Signing the multicore image...."
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SBL_CERT_GEN)
endif
	$(SBL_CERT_GEN) -b $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage -o $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage.signed -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY_HS)
	$(SBL_CERT_GEN) -b $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage -o $(BINDIR)/$(RPRC_PREFIX_BAREMETAL)_all_cores_$(BUILD_PROFILE_$(CORE)).appimage.hs_fs -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
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
