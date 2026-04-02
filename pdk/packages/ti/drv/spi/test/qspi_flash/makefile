# Makefile for SPI unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

QSPI_OS_TESTPREFIX=
QSPI_OS_TESTPOSTFIX=

ifeq ($(BUILD_OS_TYPE),baremetal)
QSPI_OS_TESTPREFIX=_Baremetal
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
QSPI_OS_TESTPREFIX=
QSPI_OS_TESTPOSTFIX=_tirtos
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON    = $(PDK_COMMON_TIRTOS_COMP) 

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_mcu1_0 = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_r5f.cfg
endif
endif

ifeq ($(BUILD_OS_TYPE),freertos)
QSPI_OS_TESTPREFIX=
QSPI_OS_TESTPOSTFIX=_freertos
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
endif

SRCDIR = . src src/Flash_S25FL
INCDIR = . src src/Flash_S25FL

# Common source files across all platforms and cores
SRCS_COMMON += main_qspi_flash_test.c S25FL.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

ifeq ($(DMA), enable)
    ifeq ($(SMP), enable)
        #Name of the directory created under packages/ti/binary/
        APP_NAME = QSPI$(QSPI_OS_TESTPREFIX)_Flash_Dma_SMP_TestApp$(QSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        LOCAL_APP_NAME =  QSPI$(QSPI_OS_TESTPREFIX)_Flash_Dma_$(BOARD)_$(CORE)_SMP_TestApp$(QSPI_OS_TESTPOSTFIX)
    else
        #Name of the directory created under packages/ti/binary/
        APP_NAME = QSPI$(QSPI_OS_TESTPREFIX)_Flash_Dma_TestApp$(QSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        LOCAL_APP_NAME =  QSPI$(QSPI_OS_TESTPREFIX)_Flash_Dma_$(BOARD)_$(CORE)TestApp$(QSPI_OS_TESTPOSTFIX)
    endif

COMP_LIST_COMMON += spi_dma
CFLAGS_SPI_DMA    = -DSPI_DMA_ENABLE
else
    ifeq ($(SMP), enable)
        APP_NAME = QSPI$(QSPI_OS_TESTPREFIX)_Flash_SMP_TestApp$(QSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        LOCAL_APP_NAME =  QSPI$(QSPI_OS_TESTPREFIX)_Flash_$(BOARD)_$(CORE)_SMP_TestApp$(QSPI_OS_TESTPOSTFIX)
    else
        APP_NAME = QSPI$(QSPI_OS_TESTPREFIX)_Flash_TestApp$(QSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        LOCAL_APP_NAME =  QSPI$(QSPI_OS_TESTPREFIX)_Flash_$(BOARD)_$(CORE)TestApp$(QSPI_OS_TESTPOSTFIX)
    endif

COMP_LIST_COMMON += spi
CFLAGS_SPI_DMA    =
endif

PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_SPI_DMA) $(CFLAGS_OS_DEFINES)
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
