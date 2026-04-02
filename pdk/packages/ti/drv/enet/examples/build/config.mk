#
# This file contains common linker for Enet examples and UT
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

SRCDIR =
INCDIR =
ENET_OS_TYPE_PREFIX =
ENET_OS_TYPE_POSTFIX =

ifeq ($(BUILD_OS_TYPE),baremetal)
  ENET_OS_TYPE_POSTFIX = _baremetal
  COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
  ifeq ($(ISA),$(filter $(ISA), a53 a72))
    LNKFLAGS_LOCAL_$(CORE) += --entry Entry
  endif
endif
ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_OS_TYPE_POSTFIX = _freertos
  INCLUDE_EXTERNAL_INTERFACES = freertos
  COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
  ENET_TEST_PREFIX =
  XDC_CFG_FILE_$(CORE) =

  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    ifeq ($(CORE),mcu1_0)
      COMP_LIST_COMMON += sciserver_tirtos
    endif
  endif
endif
ifeq ($(BUILD_OS_TYPE),safertos)
  ENET_OS_TYPE_POSTFIX = _safertos
  INCLUDE_EXTERNAL_INTERFACES = safertos
  COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
  ENET_TEST_PREFIX =
  XDC_CFG_FILE_$(CORE) =

  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4))
    ifeq ($(CORE),mcu1_0)
      COMP_LIST_COMMON += sciserver_tirtos
    endif
  endif
endif

# we use local linker command file to make custom changes
ifeq ($(BUILD_OS_TYPE),baremetal)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1))
       EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)/linker_r5.lds
    endif
  endif
  ifeq ($(CORE),$(filter $(CORE), mpu1_0))
    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
       EXTERNAL_LNKCMD_FILE_LOCAL   = $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)/linker_a72_mpu1_0.lds
    endif
  endif
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1))
       EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)/linker_r5_freertos.lds
       SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)
       SRCS_COMMON += r5_mpu_default.c
    endif
  endif
endif

ifeq ($(BUILD_OS_TYPE),safertos)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4))
    ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1))
       EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)/linker_r5_safertos.lds
       SRCDIR += $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/$(SOC)
    endif
  endif
endif

