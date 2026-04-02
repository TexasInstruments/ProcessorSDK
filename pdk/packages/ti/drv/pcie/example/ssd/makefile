# Makefile for PCIE SSD app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

CFLAGS_OS_DEFINES += -DBAREMETAL
COMP_LIST_COMMON  += $(PDK_COMMON_BAREMETAL_COMP)

INCLUDE_EXTERNAL_INTERFACES += pdk fatfs
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES) -DSSD

#Name of the directory created under packages/ti/binary/
APP_NAME = PCIE_ssd_ExampleProject

SRCDIR = . src ../sample/$(SOC)/src
INCDIR = . src ../sample/$(SOC)/src
# Common source files across all platforms and cores
SRCS_COMMON += nvme.c pcie_ssd.c pcie_sample_board.c fs_shell_app_utils.c fatfs_pcie_interface.c
PACKAGE_SRCS_COMMON = .

COMP_LIST_COMMON += fatfs_indp pcie

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
