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
SRCS_COMMON += main_ospi_flash_test.c
ifeq ($(SOC),$(filter $(SOC), j7200 j721e am64x am65xx j721s2 j784s4 j742s2))
    ifeq ($(CACHE), enable)
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/cached/mpu_config.c
        endif
    else
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/non_cached/mpu_config.c
        endif
    endif
endif
OSPI_OS_TESTPREFIX=_Baremetal
OSPI_OS_TESTPOSTFIX=
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_OS_DEFINES = -DUSE_BIOS -DOSPI_TESTAPP_RTOS -DOSPI_TESTAPP_TIRTOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)
OSPI_OS_TESTPREFIX=
OSPI_OS_TESTPOSTFIX=_tirtos

SRCS_COMMON += main_ospi_flash_test.c


ifeq ($(SOC),$(filter $(SOC), am65xx))
    ifeq ($(CACHE), enable)
        XDC_CFG_FILE_mpu1_0 = ./$(SOC)/cached/ospiFlashTest_a53.cfg
        XDC_CFG_FILE_mcu1_0 = ./$(SOC)/cached/ospiFlashTest_r5.cfg
    else
        XDC_CFG_FILE_mpu1_0 = ./$(SOC)/non_cached/ospiFlashTest_a53.cfg
        XDC_CFG_FILE_mcu1_0 = ./$(SOC)/non_cached/ospiFlashTest_r5.cfg
    endif
endif
ifeq ($(SOC),$(filter $(SOC), j7200 j721e am64x j721s2 j784s4 j742s2))
    ifeq ($(CACHE), enable)
        XDC_CFG_FILE_$(CORE) = ./$(SOC)/cached/sysbios_$(ISA).cfg
    else
        XDC_CFG_FILE_$(CORE) = ./$(SOC)/non_cached/sysbios_$(ISA).cfg
    endif
endif
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS -DOSPI_TESTAPP_RTOS
  EXTERNAL_INTERFACES += freertos
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)

  OSPI_OS_TESTPREFIX=
  OSPI_OS_TESTPOSTFIX=_freertos

  SRCS_COMMON += main_ospi_flash_test.c

  ifeq ($(SOC),$(filter $(SOC), j7200 j721e am64x am65xx j721s2 j784s4 j742s2))
    ifeq ($(CACHE), enable)
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/cached/mpu_config.c
        endif
    else
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/non_cached/mpu_config.c
        endif
    endif
  endif

endif

ifeq ($(BUILD_OS_TYPE),safertos)
  CFLAGS_OS_DEFINES = -DSAFERTOS -DOSPI_TESTAPP_RTOS
  EXTERNAL_INTERFACES += safertos
  COMP_LIST_COMMON += $(PDK_COMMON_SAFERTOS_COMP)

  OSPI_OS_TESTPREFIX=
  OSPI_OS_TESTPOSTFIX=_safertos

  SRCS_COMMON += main_ospi_flash_test.c

  ifeq ($(SOC),$(filter $(SOC), j7200 j721s2 j721e am64x am65xx j784s4 j742s2))
    ifeq ($(CACHE), enable)
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/cached/mpu_safertos_config.c
        endif
    else
        ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
            SRCS_COMMON += ./$(SOC)/non_cached/mpu_safertos_config.c
        endif
    endif
  endif

endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk $(EXTERNAL_INTERFACES)

ifeq ($(CACHE), enable)
    ifeq ($(DMA), enable)
        #Name of the directory created under packages/ti/binary/
        APP_NAME = OSPI$(OSPI_OS_TESTPREFIX)_Flash_Dma_Cache_TestApp$(OSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        COMP_LIST_COMMON += spi_dma
        #COMP_LIST_COMMON += spi_dma_profile
        CFLAGS_SPI_DMA    = -DSPI_DMA_ENABLE
    else
        APP_NAME = OSPI$(OSPI_OS_TESTPREFIX)_Flash_Cache_TestApp$(OSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        COMP_LIST_COMMON += spi
        #COMP_LIST_COMMON += spi_profile
        CFLAGS_SPI_DMA    =
    endif
CFLAGS_SPI_CACHE    = -DSPI_CACHE_ENABLE
else
    ifeq ($(DMA), enable)
        #Name of the directory created under packages/ti/binary/
        APP_NAME = OSPI$(OSPI_OS_TESTPREFIX)_Flash_Dma_TestApp$(OSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
        COMP_LIST_COMMON += spi_dma
        #COMP_LIST_COMMON += spi_dma_profile
        CFLAGS_SPI_DMA    = -DSPI_DMA_ENABLE
    else
        APP_NAME = OSPI$(OSPI_OS_TESTPREFIX)_Flash_TestApp$(OSPI_OS_TESTPOSTFIX)
        # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>

    COMP_LIST_COMMON += spi
    #COMP_LIST_COMMON += spi_profile
    CFLAGS_SPI_DMA    =
    endif
CFLAGS_SPI_CACHE    =
endif

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