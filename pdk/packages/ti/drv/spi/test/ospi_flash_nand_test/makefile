# Makefile for SPI unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

SRCDIR += . src
INCDIR += . src

ifeq ($(BUILD_OS_TYPE), baremetal)
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
SRCS_COMMON += main_ospi_flash_nand_otp_test.c
OSPI_OS_TESTPREFIX=_Baremetal
OSPI_OS_TESTPOSTFIX=
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk $(EXTERNAL_INTERFACES)

APP_NAME = OSPI_NAND_Flash_OTP_TestApp
COMP_LIST_COMMON += spi
CFLAGS_SPI_DMA    =
CFLAGS_SPI_CACHE    =

PACKAGE_SRCS_COMMON = .
PACKAGE_SRCS_COMMON += ../src/SPI_log.h ../src/SPI_log.c
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_SPI_DMA) $(CFLAGS_OS_DEFINES) $(CFLAGS_SPI_CACHE)

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