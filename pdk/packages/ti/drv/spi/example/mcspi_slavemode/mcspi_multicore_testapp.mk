#
# This file is the makefile for building a testcase
# to boot all spi images using SBL.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

#Baremetal Specifics
ifeq ($(BUILD_OS_TYPE), baremetal)
    MCSPI_OS_TESTPREFIX=_baremetal
    MCSPI_OS_TESTPOSTFIX=
	SCISERVER_OS=freertos
endif

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)
    MCSPI_OS_TESTPREFIX=
    MCSPI_OS_TESTPOSTFIX=_freertos
	SCISERVER_OS=freertos
endif

# DMA enabled build
ifeq ($(DMA), enable)
    MCSPI_DMA_MODE_INFIX = _dma
else
    MCSPI_DMA_MODE_INFIX =
endif

SRCDIR = . src
INCDIR = . src

APP_NAME = mcspi$(MCSPI_OS_TESTPREFIX)_masterslave$(MCSPI_DMA_MODE_INFIX)_testapp$(MCSPI_OS_TESTPOSTFIX)

# Local name of SPI test app
RPRC_PREFIX_MASTER = mcspi$(MCSPI_OS_TESTPREFIX)_master$(MCSPI_DMA_MODE_INFIX)_testapp$(MCSPI_OS_TESTPOSTFIX)
RPRC_PREFIX_SLAVE = mcspi$(MCSPI_OS_TESTPREFIX)_slave$(MCSPI_DMA_MODE_INFIX)_testapp$(MCSPI_OS_TESTPOSTFIX)
RPRC_PREFIX_MASTERSLAVE = mcspi$(MCSPI_OS_TESTPREFIX)_masterslave$(MCSPI_DMA_MODE_INFIX)_testapp$(MCSPI_OS_TESTPOSTFIX)

define BIN_PATH_PREFIX_RULE
BIN_PATH_PREFIX_$(1) = $(if $(findstring $(1), mcu2_0),\
							$(pdk_PATH)/ti/binary/$(RPRC_PREFIX_MASTER)/bin/$(BOARD)/$(RPRC_PREFIX_MASTER),\
							$(pdk_PATH)/ti/binary/$(RPRC_PREFIX_SLAVE)/bin/$(BOARD)/$(RPRC_PREFIX_SLAVE))

endef

BIN_PATH_PREFIX_MACRO_LIST := $(foreach core,mcu2_0 mcu2_1, $(call BIN_PATH_PREFIX_RULE,$(core)))

$(eval ${BIN_PATH_PREFIX_MACRO_LIST})

CORES_IN_TEST = mcu2_0 mcu2_1
MULTICORE_IMG_PARAMS = $(foreach SOC_CORE_ID, $(CORES_IN_TEST), $(SBL_CORE_ID_$(SOC_CORE_ID)) $(BIN_PATH_PREFIX_$(SOC_CORE_ID))_$(SOC_CORE_ID)_$(BUILD_PROFILE_$(CORE)).rprc)
MULTICORE_IMG_PARAMS +=$(SBL_CORE_ID_mcu1_0) $(pdk_PATH)/ti/drv/sciclient/tools/ccsLoadDmsc/$(SOC)/sciserver_testapp_$(SCISERVER_OS)_mcu1_0_release.rprc

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = . 

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

# List all the components required by the application
COMP_LIST_COMMON =

SRCS_COMMON = force_multi_core_img_gen.c

force_multi_core_img_gen.c:
	$(MKDIR) -p $(BINDIR)
	$(ECHO) "# Combining RPRC images to generate multicore image...."
	$(ECHO) "# BINDIR is $(BINDIR) CORELIST is mcu2_0 mcu2_1"
	$(ECHO) "# MULTICORE_IMG_PARAMS are $(MULTICORE_IMG_PARAMS)"
	$(SBL_IMAGE_GEN) LE $(SBL_DEV_ID) $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage $(MULTICORE_IMG_PARAMS)
	$(ECHO) "#"
	$(ECHO) "# Multicore IPC App image $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage created."
	$(ECHO) "#"
	$(ECHO) "# Signing the multicore image...."
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(SBL_CERT_GEN)
endif
	$(SBL_CERT_GEN) -b $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage -o $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage.signed -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY_HS)
	$(SBL_CERT_GEN) -b $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage -o $(BINDIR)/$(RPRC_PREFIX_MASTERSLAVE)_mcu2_1_$(BUILD_PROFILE_$(CORE)).appimage.hs_fs -c R5 -l $(SBL_RUN_ADDRESS) -k $(SBL_CERT_KEY)

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
