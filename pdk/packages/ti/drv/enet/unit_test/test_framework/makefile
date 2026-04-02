#
# This file is the makefile for building ENET unit test app for TI RTOS
#

include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_unit_testapp$(ENET_OS_TYPE_POSTFIX)

#
# This file is common makefile for building ENET unit test app for both TI-RTOS/baremetal
#
SRCDIR += ../test_framework ../test_cases $(PDK_INSTALL_PATH)/ti/drv/enet/examples/common
INCDIR += ../test_framework ../test_cases


# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_full$(ENET_OS_TYPE_POSTFIX)

SRCS_COMMON += main.c

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = ../test_framework ../test_cases ../host ../utils
PACKAGE_SRCS_COMMON += ./../enet_ut_component.mk
SRCS_COMMON += enet_test_parser.c enet_test_common.c enet_test_utils_mem.c
SRCS_COMMON += enet_test_misc.c trace.c
SRCS_COMMON += enet_test_entry.c
SRCS_COMMON += enet_test_base.c
SRCS_COMMON += cpsw_test_basicswitching.c
SRCS_COMMON += cpsw_test_autolearn.c
SRCS_COMMON += cpsw_test_aging_maxlength.c
SRCS_COMMON += cpsw_test_autolearn_vlan.c
SRCS_COMMON += cpsw_test_crcstrip.c
SRCS_COMMON += cpsw_test_fifostats.c
SRCS_COMMON += cpsw_test_stats.c
SRCS_COMMON += cpsw_test_vlan.c
SRCS_COMMON += cpsw_test_ale_srcaddr_update.c
SRCS_COMMON += cpsw_test_ale_tablefull.c
SRCS_COMMON += cpsw_test_vlan_dropuntagged.c
SRCS_COMMON += cpsw_test_multicast.c
SRCS_COMMON += cpsw_test_policer.c
SRCS_COMMON += cpsw_test_network_security.c
SRCS_COMMON += cpsw_test_hostport_rxfilter.c
SRCS_COMMON += cpsw_test_policer_nomatch.c
SRCS_COMMON += cpsw_test_intervlan.c
SRCS_COMMON += cpsw_test_pvid.c
SRCS_COMMON += cpsw_test_sanity.c
SRCS_COMMON += cpsw_test_outer_vlan.c
SRCS_COMMON += cpsw_test_traffic_shaping.c
SRCS_COMMON += cpsw_test_nway.c
SRCS_COMMON += cpsw_test_rxflow_mtu.c
SRCS_COMMON += cpsw_test_priority_regeneration.c
SRCS_COMMON += cpsw_test_bcastmcast_limit.c

ifeq ($(SOC),$(filter $(SOC), j721e j7200))
  SRCS_COMMON += cpsw_test_phystrapping.c
  SRCS_COMMON += cpsw_test_default_priority.c
  SRCS_COMMON += cpsw_test_sgmii_lpbk.c
  SRCS_COMMON += cpsw_test_cpts_event.c
  SRCS_COMMON += cpsw_test_board_j721e_gesi.c
  SRCS_COMMON += cpsw_test_directed_pkt.c
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  ENET_CFLAGS += -DFREERTOS
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(ENET_CFLAGS)
CFLAGS_LOCAL_COMMON += -DENET_CFG_TRACE_ENABLE -DENET_CFG_ASSERT_ENABLE


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
