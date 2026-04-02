# Makefile for SPI master/slave test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

#Baremetal Specifics
ifeq ($(BUILD_OS_TYPE), baremetal)

    MCSPI_OS_TESTPREFIX=_baremetal
    MCSPI_OS_TESTPOSTFIX=
    CFLAGS_OS_DEFINES = -DBARE_METAL
    LNKFLAGS_LOCAL_mpu1_0 += --entry Entry
    COMP_LIST_COMMON =  $(PDK_COMMON_BAREMETAL_COMP)
    EXTERNAL_INTERFACES =
    XDC_CFG_FILE_mpu1_0 =
    XDC_CFG_FILE_mcu1_0 =

endif

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)

    MCSPI_OS_TESTPREFIX=
    MCSPI_OS_TESTPOSTFIX=_freertos
    CFLAGS_OS_DEFINES = -DFREERTOS -DRTOS_ENV
    EXTERNAL_INTERFACES = freertos
    COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

endif

SRCDIR = . src
INCDIR = . src

# Common source files across all platforms and cores
SRCS_COMMON += main_mcspi_slave_mode.c SPI_log.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

ifeq ($(BUILD_OS_TYPE), baremetal)
    EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_INSTALL_PATH)/ti/drv/spi/example/mcspi_slavemode/$(SOC_DIR)/linker_mcu.lds
endif
ifeq ($(BUILD_OS_TYPE), freertos)
    EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_INSTALL_PATH)/ti/drv/spi/example/mcspi_slavemode/$(SOC_DIR)/linker_mcu_freertos.lds
endif

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 j721s2 j784s4 j742s2))
    ifeq ($(CORE), mcu2_1)
        # Slave running on mcu2_1 core
        CFLAGS_SPI_MS     = -DMCSPI_SLAVE_TASK
        APP_MASTER_SLAVE  = slave
        APPEND_LNKCMD_FILE = $(PDK_INSTALL_PATH)/ti/drv/spi/example/mcspi_slavemode/$(SOC_DIR)/slave_linker_append_r5f.cmd
    else
        # Master running on mcu2_0 core, use the common linker command file in build
        CFLAGS_SPI_MS     = -DMCSPI_MASTER_TASK
        APP_MASTER_SLAVE  = master
        APPEND_LNKCMD_FILE = $(PDK_INSTALL_PATH)/ti/drv/spi/example/mcspi_slavemode/$(SOC_DIR)/master_linker_append_r5f.cmd
    endif
endif

# DMA enabled build
ifeq ($(DMA), enable)
    MCSPI_DMA_MODE_INFIX = _dma
    COMP_LIST_COMMON += spi_dma
    CFLAGS_SPI_DMA    = -DSPI_DMA_ENABLE
else
    MCSPI_DMA_MODE_INFIX =
    COMP_LIST_COMMON += spi
    CFLAGS_SPI_DMA    =
endif


#Name of the directory created under packages/ti/binary/
APP_NAME = mcspi$(MCSPI_OS_TESTPREFIX)_$(APP_MASTER_SLAVE)$(MCSPI_DMA_MODE_INFIX)_testapp$(MCSPI_OS_TESTPOSTFIX)

ifeq ($(SOC),$(filter $(SOC), am64x))
CFLAGS_SPI_MS     = -DMCSPI_MASTER_TASK -Dam64x_evm=am64x_evm
APP_MASTER_SLAVE  = master
endif

ifeq ($(SOC),$(filter $(SOC), am65xx))
    ifeq ($(BOARD),$(filter $(BOARD), am65xx_evm))
        CFLAGS_SPI_MS    += -Dam65xx_evm=am65xx_evm
    endif
    ifeq ($(BOARD),$(filter $(BOARD), am65xx_idk))
        CFLAGS_SPI_MS    += -Dam65xx_idk=am65xx_idk
    endif
endif

ifeq ($(SOC),$(filter $(SOC), j7200))
CFLAGS_SPI_MS    += -Dj7200_evm=j7200_evm
endif

ifeq ($(SOC),$(filter $(SOC), j721e))
CFLAGS_SPI_MS    += -Dj721e_evm=j721e_evm
endif

ifeq ($(SOC),$(filter $(SOC), j721s2))
CFLAGS_SPI_MS    += -Dj721s2_evm=j721s2_evm
endif

ifeq ($(SOC),$(filter $(SOC), j784s4))
CFLAGS_SPI_MS    += -Dj784s4_evm=j784s4_evm
endif

ifeq ($(SOC),$(filter $(SOC), j742s2))
CFLAGS_SPI_MS    += -Dj742s2_evm=j742s2_evm
endif

ifeq ($(BOARD),$(filter $(BOARD), j721e_sim j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm))
CFLAGS_SPI_UT    = -DMCSPI_MULT_CHANNEL
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE),mcu1_0)
    COMP_LIST_COMMON += sciserver_tirtos
  endif
endif

PACKAGE_SRCS_COMMON = .
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
