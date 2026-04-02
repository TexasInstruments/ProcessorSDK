# Makefile for McASP device loopback unit test app
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = MCASP_DeviceLoopback_TestApp_$(BUILD_OS_TYPE)

#TI RTOS Specifics
ifeq ($(BUILD_OS_TYPE), tirtos)

    CFLAGS_OS_DEFINES = -DUSE_BIOS -DRTOS_ENV

    # Enable XDC build for application by providing XDC CFG File per core
    XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg

    ifeq ($(SOC),$(filter $(SOC), j721e j7200))
    else
        XDC_CFG_UPDATE_$(CORE) = ../$(SOC)/mcasp_extra_config.xs
    endif

    INCLUDE_EXTERNAL_INTERFACES = bios xdc pdk

    COMP_LIST_COMMON = $(PDK_COMMON_TIRTOS_COMP)

endif

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)

    CFLAGS_OS_DEFINES = -DFREERTOS -DRTOS_ENV
    INCLUDE_EXTERNAL_INTERFACES = freertos pdk
    COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

endif

#Safertos Specifics
ifeq ($(BUILD_OS_TYPE), safertos)
    CFLAGS_OS_DEFINES = -DSAFERTOS -DRTOS_ENV
    INCLUDE_EXTERNAL_INTERFACES = safertos pdk
    COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
endif

SRCDIR = ../src ../../
INCDIR = ../src ../include ../../

SRCS_COMMON = audioSample_io.c audioSample_main.c deviceloopback.c audio_evmInit.c mcasp_cfg.c mcasp_debug.c


COMP_LIST_COMMON += mcasp

CFLAGS_MCASP_UT = -DMCASP_MASTER -DDEVICE_LOOPBACK

PACKAGE_SRCS_COMMON = . ../src ../include

# Enable below to log various events
CFLAGS_MCASP_UT += -DMCASP_ENABLE_DEBUG_LOG

ifeq ($(SOC),$(filter $(SOC), am65xx))

  SRCDIR += ../evmAM65xx/src
  INCDIR += ../evmAM65xx/include

  PACKAGE_SRCS_COMMON += ../evmAM65xx
endif

ifeq ($(SOC), $(filter $(SOC), j721e))
  SRCDIR += ../j721e/src
  INCDIR += ../j721e/include

  PACKAGE_SRCS_COMMON += ../$(SOC)
endif

ifeq ($(SOC), $(filter $(SOC), j721s2))
  SRCDIR += ../j721s2/src
  INCDIR += ../j721s2/include

  PACKAGE_SRCS_COMMON += ../$(SOC)
endif

ifeq ($(SOC), $(filter $(SOC), j7200))
  SRCDIR += ../j7200/src
  INCDIR += ../j7200/include

  PACKAGE_SRCS_COMMON += ../$(SOC)
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_MCASP_UT) $(CFLAGS_OS_DEFINES)

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
