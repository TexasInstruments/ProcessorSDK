BUILD_OS_TYPE = freertos

include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(BUILD_HS), yes)
    HS_SUFFIX =_hs
else
    HS_SUFFIX =
endif

ifeq ($(HLOSBOOT), qnx)
    HLOS_SUFFIX =_qnx
else ifeq ($(HLOSBOOT), linux)
    HLOS_SUFFIX =_linux
else
    HLOS_SUFFIX =
endif

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

CFLAGS_LOCAL_COMMON  = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON  = .

# FLAGS FOR ECU USECASES OPTIMIZATION
ECU_BUILD ?=no

ifeq ($(ECU_BUILD), fc)
  CFLAGS_LOCAL_COMMON += -DSBL_ECU_BUILD_FC
endif
ifeq ($(ECU_BUILD), srv)
  CFLAGS_LOCAL_COMMON += -DSBL_ECU_BUILD_SRV
endif
ifeq ($(ECU_BUILD), avp4)
  CFLAGS_LOCAL_COMMON += -DSBL_ECU_BUILD_AVP4
endif

ifeq ($(SAFETY_LOOP), yes)
    ifeq ($(BOOTMODE), mmcsd)
        APP_NAME = boot_app_$(BOOTMODE)_safety
        LOCAL_APP_NAME = boot_app_$(BOOTMODE)_safety_$(BOARD)_$(CORE)_$(BUILD_OS_TYPE)
        CFLAGS_LOCAL_COMMON += -DSAFETY_CHECKER_LOOP_ENABLED
        #Enable below flag to take the register configuration for PM, RM and TIFS modules
        #CFLAGS_LOCAL_COMMON += -DSC_REGDUMP_ENABLE
    endif
else ifeq ($(SDL_SAFETY_TASK_ENABLED), yes)
    APP_NAME = boot_app_$(BOOTMODE)_sdl_safety$(HLOS_SUFFIX)
    LOCAL_APP_NAME = sbl_boot_app_$(BOOTMODE)_sdl_safety$(HLOS_SUFFIX)_$(BOARD)_$(CORE)_$(BUILD_OS_TYPE)_TestApp
    CFLAGS_LOCAL_COMMON += -DSDL_SAFETY_TASK_ENABLED
else
    APP_NAME = boot_app_$(BOOTMODE)$(HLOS_SUFFIX)$(HS_SUFFIX)
    ifeq ($(ECU_BUILD), no)
        LOCAL_APP_NAME = sbl_boot_app_$(BOOTMODE)$(HLOS_SUFFIX)$(HS_SUFFIX)_$(BOARD)_$(CORE)_$(BUILD_OS_TYPE)_TestApp
    else
        LOCAL_APP_NAME = sbl_boot_app_$(BOOTMODE)_$(ECU_BUILD)$(HLOS_SUFFIX)$(HS_SUFFIX)_$(BOARD)_$(CORE)_$(BUILD_OS_TYPE)_TestApp
    endif
endif

SRCDIR      = $(PDK_SBL_COMP_PATH)/example/boot_app

INCDIR      =  $(PDK_INSTALL_PATH)/ti/boot/sbl/soc/k3
INCDIR      += $(PDK_SBL_COMP_PATH)/
INCDIR      += $(PDK_SBL_COMP_PATH)/example/boot_app

ifeq ($(SAFETY_LOOP), yes)
    INCDIR      += $(SAFETY_CHECKERS_COMP_PATH)/src $(SAFETY_CHECKERS_COMP_PATH)/src/soc
    INCDIR      += $(SAFETY_CHECKERS_COMP_PATH)/src/soc/$(SOC_DIR)/
    INCDIR      += $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/
endif

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP) sciserver_tirtos

ifeq ($(SAFETY_LOOP), yes)
    COMP_LIST_COMMON += safety_checkers
endif

SRCS_COMMON = boot_app_main.c soc/$(SOC_DIR)/boot_core_defs.c r5_mpu_freertos.c
ifeq ($(BOOTMODE), $(filter $(BOOTMODE),ospi ospi_nand))
    SRCS_COMMON += boot_app_ospi.c
else ifeq ($(BOOTMODE), $(filter $(BOOTMODE),mmcsd emmc_uda))
    SRCS_COMMON += boot_app_mmcsd.c
endif
ifeq ($(BOOTMODE), mmcsd)
    ifeq ($(BUILD_HS), yes)
        COMP_LIST_COMMON += mmcsd sbl_lib_mmcsd_hs fatfs_indp
    else
        COMP_LIST_COMMON += mmcsd sbl_lib_mmcsd fatfs_indp
    endif
    CFLAGS_LOCAL_COMMON += -DBOOT_MMCSD
endif

ifeq ($(BOOTMODE), emmc_uda)
    COMP_LIST_COMMON += mmcsd sbl_lib_emmc fatfs_indp
    CFLAGS_LOCAL_COMMON += -DBOOT_EMMC_UDA
endif

ifeq ($(BOOTMODE), $(filter $(BOOTMODE),ospi ospi_nand))
    ifeq ($(BUILD_HS), yes)
        COMP_LIST_COMMON += spi_dma sbl_lib_cust_hs
    else
        COMP_LIST_COMMON += spi_dma sbl_lib_cust
    endif
    CFLAGS_LOCAL_COMMON += -DBOOT_OSPI
    ifeq ($(BOOT_OSPI_NAND),yes)
        CFLAGS_LOCAL_COMMON += -DBOOT_OSPI_NAND
    endif
endif

ifeq ($(CANRESP), enabled)
    CFLAGS_LOCAL_COMMON += -DCAN_RESP_TASK_ENABLED
    SRCS_COMMON += boot_app_can.c
endif

ifeq ($(SDL_SAFETY_TASK_ENABLED),yes)
    # SDL Include Files
    SDL_INSTALL_PATH=$(PDK_INSTALL_PATH)/../../sdl
    INCDIR += $(SDL_INSTALL_PATH)/
    INCDIR += $(SDL_INSTALL_PATH)/src/sdl
    INCDIR += $(SDL_INSTALL_PATH)/include
    INCDIR += $(SDL_INSTALL_PATH)/include/soc/$(SOC_DIR)
    INCDIR += $(SDL_INSTALL_PATH)/osal/
    INCDIR += $(SDL_INSTALL_PATH)/bist/pbist/
    INCDIR += $(SDL_INSTALL_PATH)/bist/lbist/
    INCDIR += $(SDL_INSTALL_PATH)/bist/soc/$(SOC_DIR)/
    INCDIR += $(SDL_INSTALL_PATH)/src/ip/r5

    # SDL Integration
    EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/osal/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_osal.$(LIBEXT)
    EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/ip/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_ip.$(LIBEXT)
    EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/sdl/lib/$(SOC)/r5f/$(BUILD_PROFILE)/sdl_api.$(LIBEXT)
    SRCS_COMMON += boot_app_osal_wrap.c
    SRCS_COMMON += bist.c bist_core_defs.c
    SRCS_COMMON += lbist_utils.c lbist_defs.c
    SRCS_COMMON += pbist_utils.c pbist_defs.c
    SRCS_COMMON += power_seq.c armv8_power_utils.c
    ifneq ($(SOC), j742s2)
        SRCS_COMMON += vtm.c event_trigger.c
        SRCS_COMMON += pok.c
        SRCS_COMMON += tog.c
        SRCS_ASM_COMMON += tog_utils.asm
    endif
    ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
        SRCS_COMMON += ecc.c soc_ecc_func.c
        SRCS_ASM_COMMON += sdl_arm_r5_mpu.asm sdl_arm_r5_pmu.asm
    endif

    # PDK Include Files
    INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/bist
    INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/soc/$(SOC_DIR)/bist
    ifneq ($(SOC), j742s2)
        INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/vtm
        INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/pok
        INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/tog
    endif
    ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
        INCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/ecc/soc/$(SOC)
    endif

    # SDL Source File Paths
    SRCDIR += $(SDL_INSTALL_PATH)/bist/pbist/
    SRCDIR += $(SDL_INSTALL_PATH)/bist/lbist/
    SRCDIR += $(SDL_INSTALL_PATH)/bist/soc/$(SOC_DIR)/
    ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
        SRCDIR += $(SDL_INSTALL_PATH)/src/ip/r5/src
    endif

    # PDK Source File Paths
    SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/bist
    SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/soc/$(SOC_DIR)/bist
    ifneq ($(SOC), j742s2)
        SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/vtm
        SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/pok
        SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/tog
    endif
    ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
        SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/ecc/soc/$(SOC)
        SRCDIR += $(PDK_SBL_COMP_PATH)/example/boot_app/ecc
    endif
endif

ifeq ($(HLOSBOOT), linux)
    CFLAGS_LOCAL_COMMON += -DMPU1_HLOS_BOOT_ENABLED -DHLOS_BOOT_LINUX_OS
else ifeq ($(HLOSBOOT), qnx)
    CFLAGS_LOCAL_COMMON += -DMPU1_HLOS_BOOT_ENABLED -DHLOS_BOOT_QNX_OS
endif

ifeq ($(SAFETY_LOOP), yes)
    EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/linker_r5_freertos_safety.lds
else ifeq ($(SDL_SAFETY_TASK_ENABLED), yes)
    ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
        ifneq ($(HLOSBOOT), linux)
            CFLAGS_LOCAL_COMMON += -DECC_ENABLED
            ifeq ($(HLOSBOOT), qnx)
                EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/soc/$(SOC)/linker_r5_freertos_sdl_safety_qnx.lds
            else
                EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/soc/$(SOC)/linker_r5_freertos_sdl_safety.lds
            endif
        else
            EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/linker_r5_freertos.lds
        endif
    else
        EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/linker_r5_freertos.lds
    endif
else
    EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/boot_app/linker_r5_freertos.lds
endif

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk