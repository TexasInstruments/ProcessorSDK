# Makefile for PCIE sample app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_OS_TYPE),tirtos)
    CFLAGS_OS_DEFINES += -DUSE_BIOS -DTIRTOS
    # List all the external components/interfaces, whose interface header files
    #  need to be included for this component
    INCLUDE_EXTERNAL_INTERFACES += bios xdc pdk
    # List all the components required by the application
    COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)

    ifeq ($(CORE),$(filter $(CORE), mpu1_0))
        # Enable XDC build for application by providing XDC CFG File per core
        XDC_CFG_FILE_$(CORE) = ../sample/$(SOC)/pciesample_a53.cfg
    endif
    ifeq ($(CORE),$(filter $(CORE), mcu1_0))
        # Enable XDC build for application by providing XDC CFG File per core
        XDC_CFG_FILE_$(CORE) = ../sample/$(SOC)/pciesample_r5.cfg
    endif

    PCIE_OS_TESTPOSTFIX=_tirtos
endif

ifeq ($(BUILD_OS_TYPE),freertos)
    CFLAGS_OS_DEFINES += -DFREERTOS
    INCLUDE_EXTERNAL_INTERFACES += freertos pdk
    COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)

    ifeq ($(SOC),$(filter $(SOC), am65xx))
        ifeq ($(CORE),$(filter $(CORE), mcu1_0))
            EXTERNAL_LNKCMD_FILE_LOCAL += ../sample/$(SOC)/linker_r5_freertos.lds
        endif
    endif

    PCIE_OS_TESTPOSTFIX=_freertos
endif

#Name of the directory created under packages/ti/binary/
APP_NAME = PCIE_Qos_ExampleProject$(PCIE_OS_TESTPOSTFIX)

ifeq ($(SOC),$(filter $(SOC), am65xx))
    SRCDIR += . src ../sample/$(SOC)/src ../sample/udma
    INCDIR += . src ../sample/$(SOC)/src ../sample/udma
    # Common source files across all platforms and cores
    SRCS_COMMON += pcie_qos_sample.c pcie_sample_board.c pcie_udma.c
    PACKAGE_SRCS_COMMON += .
endif

# List all the components required by the application
COMP_LIST_COMMON += pcie

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES) -DQOS

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
