

ALG_BASE_PATH ?= $(abspath ./../../)
MODULE_NAME ?= $(subst $(ALG_BASE_PATH),,$(HOST_ROOT))
#$(info MODULE_NAME=$(MODULE_NAME))

CSOURCES += $(foreach file, $(TB_FILES), ../../../$(MODULE_NAME)/$(file))

COMMON_FILES :=
COMMON_FILES += ti_mem_manager.c
COMMON_FILES += tiadalg_common_utils.c
COMMON_FILES += tiadalg_configparser.c
COMMON_FILES += tiadalg_mem_manager.c
COMMON_FILES += profile.c
COMMON_FILES += tiadalg_img_proc.c

CSOURCES += $(foreach file, $(COMMON_FILES), ../../../common/$(file))

#$(info TARGET_CPU=$(TARGET_CPU))

#CFLAGS += -DLITTLE_ENDIAN_HOST --cpp_default

#ifeq ($(TARGET_BUILD), release)
#  CFLAGS += -O3  -mo -k -mw --fp_not_associative
#else
#  CFLAGS += -g
#endif

#$(info CSOURCES=$(CSOURCES))
#$(info TB_FILES=$(TB_FILES))

# include search directories needed by all platforms
IDIRS += $(PDK_PATH)/packages
IDIRS += $(IVISION_PATH)
IDIRS += ../../include/


# defs needed by all platforms
DEFS+=SOC_J721E
DEFS+=BUILD_C7X_1

