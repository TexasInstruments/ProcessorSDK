#
# This file is the makefile for building TSN gPTP example app
#
include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_tsn_est_example$(ENET_OS_TYPE_POSTFIX)

# TSN example app dir
SRCDIR += . ../
INCDIR += . ../

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
else ifeq ($(BUILD_OS_TYPE),safertos)
  ENET_CFLAGS += -DSAFERTOS
endif

ifeq ($(BUILD_PROFILE_$(CORE)),release)
  ENET_CFLAGS += -mthumb -O3
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk
INCLUDE_INTERNAL_INTERFACES += tsn_gptp tsn_combase tsn_unibase tsn_uniconf

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)
COMP_LIST_COMMON += tsn_combase tsn_unibase tsn_gptp tsn_uniconf mmcsd fatfs_indp

# Yangemb is the licensing code which provides license to TSN Stack at runtime.
# This is provided to customers as a compiled library.
LNKFLAGS_LOCAL_COMMON += $(PDK_TSN_COMP_PATH)/tsn-stack/license_lib/yangemb-freertos.${BOARD}.r5f.ti-arm-clang.lib

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS) \
    -include $(PDK_TSN_COMP_PATH)/tsn-stack/tsn_buildconf/jacinto_buildconf.h \
    -DQSGMII_EXPANSION_BOARD \
    -DTSN_USE_LOG_BUFFER=1 \
    -I$(PDK_TSN_COMP_PATH)/tsn-stack/tsn_combase/tilld/jacinto \
    -DDISABLE_FAT_FS \
    -DGPTP_ENABLED \
    -DEST_APP_ENABLED \
    -DTSNAPP_LOGLEVEL=\"4,ubase:45,cbase:45,uconf:45,gptp:66,lldp:45,avtp:45,nconf:45\" \

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += main.c tsninit.c est_init.c qosapp_misc.c gptp_init.c debug_log.c

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = main.c tsninit.c est_init.c qosapp_misc.c gptp_init.c debug_log.c makefile
PACKAGE_SRCS_COMMON += qosapp_misc.h tsnapp_porting.h common.h debug_log.h tsninit.h

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
