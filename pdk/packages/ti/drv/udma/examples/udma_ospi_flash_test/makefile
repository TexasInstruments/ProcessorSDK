#
# This file is the makefile for building UDMA OSPI Flash test app for Baremetal
#

BUILD_OS_TYPE=baremetal

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = udma_baremetal_ospi_flash_testapp

SRCDIR = .
INCDIR =

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
COMP_LIST_COMMON += udma_apputils

# Common source files and CFLAGS across all platforms and cores 
PACKAGE_SRCS_COMMON = . ../../../../csl/example/ospi/ospi_flash/common/ ../../../../csl/example/ospi/ospi_flash/common/soc/$(SOC)
INCDIR = . ../../../../csl/example/ospi/ospi_flash/common/ ../../../../csl/example/ospi/ospi_flash/common/soc/$(SOC)
SRCDIR = . ../../../../csl/example/ospi/ospi_flash/common/ ../../../../csl/example/ospi/ospi_flash/common/soc/$(SOC)
SRCS_COMMON = ospi_flash_main_baremetal.c udma_ospi_flash_test.c ospi_flash_common.c ospi_flash_phy_tune.c ospi_flash_patterns.c
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

EXTERNAL_LNKCMD_FILE_LOCAL = baremetal/$(SOC)/linker_r5.lds

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