# Makefile for GPMC unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make


ifeq ($(IS_BAREMETAL),yes)
BUILD_OS_TYPE = baremetal
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON =  $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
else
BUILD_OS_TYPE = tirtos
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON    = $(PDK_COMMON_TIRTOS_COMP)

XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif

SRCDIR = . src
INCDIR = . src
# Common source files across all platforms and cores
SRCS_COMMON += main_gpmc_test.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)


ifeq ($(BUILD_OS_TYPE), tirtos)
GPMC_OS_TYPE=
else
GPMC_OS_TYPE=_Baremetal
endif

# DMA enabled build
ifeq ($(DMA), enable)
    #Name of the directory created under packages/ti/binary/
    APP_NAME = GPMC$(GPMC_OS_TYPE)_Dma_TestApp
    # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
    LOCAL_APP_NAME =  GPMC$(GPMC_OS_TYPE)_Dma_$(BOARD)_$(CORE)TestApp

    COMP_LIST_COMMON   += gpmc_dma
    CFLAGS_GPMC_DMA    = -DGPMC_DMA_ENABLE -D$(BOARD)=$(BOARD)
else
    #Name of the directory created under packages/ti/binary/
    APP_NAME = GPMC$(GPMC_OS_TYPE)_TestApp
    # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
    LOCAL_APP_NAME =  GPMC$(GPMC_OS_TYPE)_$(BOARD)_$(CORE)TestApp

    COMP_LIST_COMMON   += gpmc
    CFLAGS_GPMC_DMA    =
endif

PACKAGE_SRCS_COMMON = makefile src/

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_GPMC_DMA) $(CFLAGS_OS_DEFINES)

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
