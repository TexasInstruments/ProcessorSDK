
#Definitions pertaining to PDK when built from Processor SDK.
# Processor SDK defines "PDK_SOC" which is the SOC name used in the processor sdk installer.
# Many SOCs can be included in the processor sdk installer and the pdk directory takes the form "pdk_<PDK_SOC>_<version>
# For example, pdk_am57xx_x_x_x holds am571x,am572x,am574x. The PDK_SOC here is "am57xx"
# In this file, the make variables below are derived from PDK_SOC

include $(PDK_INSTALL_PATH)/ti/build/soc_info.mk

# Below are the supported PDK_SOCs in Processor SDK
export PROCSDK_SUPPORTED_PDK_SOCS = j7 j7-hs j721e

#if PDK_SOC is specified , derive LIMIT_SOCS/LIMIT_BOARDS/LIMIT_CORES from it (if not specified explicitly)
ifneq ($(PDK_SOC),)
#Check if PDK_SOC exists within the supported SOCs
ifeq ($(PDK_SOC),$(filter $(PDK_SOC), $(PROCSDK_SUPPORTED_PDK_SOCS)))

# Filter out c7x-hostemu as Processor SDK does not build use it
LIMIT_CORES_j7        = $(filter-out c7x-hostemu,$(sort $(CORE_LIST_j721e) $(CORE_LIST_j7200) $(CORE_LIST_j721s2) $(CORE_LIST_j784s4) $(CORE_LIST_j742s2)))
LIMIT_CORES_j7-hs     = $(filter-out c7x-hostemu,$(sort $(CORE_LIST_j721e) $(CORE_LIST_j7200) $(CORE_LIST_j721s2) $(CORE_LIST_j784s4) $(CORE_LIST_j742s2)))

export LIMIT_CORES ?= $(LIMIT_CORES_$(PDK_SOC))

LIMIT_SOCS_j7        = j721e j7200 j721s2 j784s4 j742s2
LIMIT_SOCS_j7-hs     = j721e j7200 j721s2 j784s4 j742s2

export LIMIT_SOCS ?= $(LIMIT_SOCS_$(PDK_SOC))

ifeq ($(findstring hs,$(PDK_SOC)),hs)
export BUILD_HS ?= yes
else
export BUILD_HS ?= no
endif

LIMIT_BOARDS_j7        = $(BOARD_LIST_j721e) $(BOARD_LIST_j7200) $(BOARD_LIST_j721s2) $(BOARD_LIST_j784s4) $(BOARD_LIST_j742s2)
LIMIT_BOARDS_j7-hs     = $(BOARD_LIST_j721e) $(BOARD_LIST_j7200) $(BOARD_LIST_j721s2) $(BOARD_LIST_j784s4) $(BOARD_LIST_j742s2)
export LIMIT_BOARDS ?= $(LIMIT_BOARDS_$(PDK_SOC))

endif
endif
