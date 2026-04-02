# Makefile for UART unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_OS_TYPE),baremetal)
CFLAGS_OS_DEFINES = -DBAREMETAL
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON  = $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
endif

ifeq ($(BUILD_OS_TYPE),tirtos)
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON  = $(PDK_COMMON_TIRTOS_COMP)

ifeq ($(SOC),$(filter $(SOC), j721e j7200 am65xx am64x tpr12 awr294x j721s2 j784s4 j742s2))
  XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
else
  XDC_CFG_FILE_mpu1_0 = ./$(SOC)/uartUnitTest_$(ISA).cfg
  XDC_CFG_FILE_mcu1_0 = ./$(SOC)/uartUnitTest_r5.cfg
endif

ifeq ($(SOC),$(filter $(SOC), am64x))
ifeq ($(ISA),$(filter $(ISA), a53))
  LIB_PATHS_DIR=$(BIOS_INSTALL_PATH)/packages/gnu/targets/arm/libs/install-native/aarch64-none-elf/lib/
endif
endif

endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
COMP_LIST_COMMON  = $(PDK_COMMON_FREERTOS_COMP)
EXTERNAL_INTERFACES = freertos
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
COMP_LIST_COMMON  = $(PDK_COMMON_SAFERTOS_COMP)
EXTERNAL_INTERFACES = safertos
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
endif

SRCDIR = . src
INCDIR += . src
# Common source files across all platforms and cores
SRCS_COMMON += main_uart_test.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

UART_OS_TYPE_PREFIX=
UART_OS_TYPE_POSTFIX=

ifeq ($(BUILD_OS_TYPE), tirtos)
UART_OS_TYPE_POSTFIX=_tirtos
endif
ifeq ($(BUILD_OS_TYPE), baremetal)
UART_OS_TYPE_PREFIX=_Baremetal
endif
ifeq ($(BUILD_OS_TYPE),freertos)
UART_OS_TYPE_POSTFIX=_freertos
endif
ifeq ($(BUILD_OS_TYPE),safertos)
UART_OS_TYPE_POSTFIX=_safertos
endif

# DMA enabled build
ifeq ($(DMA), enable)
    #Name of the directory created under packages/ti/binary/
    APP_NAME = UART$(UART_OS_TYPE_PREFIX)_DMA_TestApp$(UART_OS_TYPE_POSTFIX)
    COMP_LIST_COMMON := $(filter-out uart,$(COMP_LIST_COMMON))
    COMP_LIST_COMMON   += uart_dma
    #COMP_LIST_COMMON   += uart_dma_profile
    CFLAGS_UART_DMA    = -DUART_DMA_ENABLE
else
    #Name of the directory created under packages/ti/binary/
    APP_NAME = UART$(UART_OS_TYPE_PREFIX)_TestApp$(UART_OS_TYPE_POSTFIX)
    # Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>

    #COMP_LIST_COMMON   += uart_profile
    CFLAGS_UART_DMA    =
endif
# List all the specific components required by the application
#COMP_LIST_COMMON  += profiling

PACKAGE_SRCS_COMMON = .

ifeq ($(SOC),$(filter $(SOC), am64x))
ifeq ($(CORE),$(filter $(CORE), m4f_0))
CONFIG_BLD_LNK_m4f = am64x/uart_linker_m4f.lds
PACKAGE_SRCS_COMMON += $(SOC)
endif
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_UART_DMA) $(CFLAGS_OS_DEFINES)

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
