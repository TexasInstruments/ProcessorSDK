#
# EST test (TI-RTOS/FREERTOS) makefile
#

include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_est_test$(ENET_OS_TYPE_POSTFIX)

SRCDIR += .
INCDIR += .

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON =  makefile 
PACKAGE_SRCS_COMMON += enet_cpsw_est_main.c 
PACKAGE_SRCS_COMMON += enet_cpsw_est_cfg.h enet_cpsw_est_cfg.c
PACKAGE_SRCS_COMMON += enet_cpsw_est_common.h 
PACKAGE_SRCS_COMMON += enet_cpsw_est_dataflow.h enet_cpsw_est_dataflow.c
PACKAGE_SRCS_COMMON += enet_cpsw_est_ts.h enet_cpsw_est_ts.c
PACKAGE_SRCS_COMMON += test_config_$(SOC).c 

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
else ifeq ($(BUILD_OS_TYPE),safertos)
  ENET_CFLAGS += -DSAFERTOS
endif

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)

# Source files
SRCS_COMMON += enet_cpsw_est_main.c enet_cpsw_est_cfg.c
SRCS_COMMON += enet_cpsw_est_dataflow.c enet_cpsw_est_ts.c
SRCS_COMMON += test_config_$(SOC).c 

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS)

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
