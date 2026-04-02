# Makefile for SPI master/slave test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  XDC_CFG_FILE_$(CORE) = ./$(SOC)/mss.cfg
endif
ifeq ($(CORE),$(filter $(CORE), c66xdsp_1))
  XDC_CFG_FILE_$(CORE) = ./$(SOC)/dss.cfg
endif
endif
endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
endif
ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
EXTERNAL_INTERFACES = safertos
COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
endif

SRCDIR = . src
INCDIR = . src

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
SRCDIR += $(SOC)
INCDIR += $(SOC)
endif

# Common source files across all platforms and cores
SRCS_COMMON += main_mibspi_test.c mibspi_test_common.c


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# DMA enabled build
ifeq ($(DMA), enable)

#Name of the directory created under packages/ti/binary/
APP_NAME = MIBSPI_Loopback_Dma_TestApp_$(BUILD_OS_TYPE)
# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  MIBSPI_Loopback_Dma_$(BOARD)_$(CORE)TestApp_$(BUILD_OS_TYPE)

COMP_LIST_COMMON += mibspi_dma
CFLAGS_SPI_DMA    = -DMIBSPI_DMA_ENABLE
else

#Name of the directory created under packages/ti/binary/
APP_NAME = MIBSPI_Loopback_TestApp_$(BUILD_OS_TYPE)

# Name of the binary if different fmake clrom the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME =  MIBSPI_Loopback_$(BOARD)_$(CORE)TestApp_$(BUILD_OS_TYPE)


COMP_LIST_COMMON += mibspi
CFLAGS_SPI_DMA    =

endif

# Master running on mcu1_0 core, use the common linker command file in build
#CFLAGS_SPI_MS     = -DMIBSPI_MASTER_TASK
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))

# MIBSPI uses local linker command file to avoid code/data memory conflict with master application
ifeq ($(BUILD_OS_TYPE), tirtos)
ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  EXTERNAL_LNKCMD_FILE_LOCAL =  ./$(SOC)/mss_spi_linker.cmd
endif
ifeq ($(CORE),$(filter $(CORE), c66xdsp_1))
  EXTERNAL_LNKCMD_FILE_LOCAL =  ./$(SOC)/dss_spi_linker.cmd
endif
endif
endif

# tpr12, awr294x build
ifeq ($(BOARD),$(filter $(BOARD), tpr12_evm awr294x_evm))
#CFLAGS_SPI_UT    = -DMIBSPI_MULT_CHANNEL
endif

PACKAGE_SRCS_COMMON = ../../test/loopback

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_SPI_UT) $(CFLAGS_SPI_DMA) $(CFLAGS_SPI_MS) $(CFLAGS_OS_DEFINES)

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
