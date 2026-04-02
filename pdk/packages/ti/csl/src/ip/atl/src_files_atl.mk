
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2 tpr12 awr294x))
PACKAGE_SRCS_COMMON += cslr_atl.h src/ip/atl/src_files_atl.mk src/ip/atl/V0
INCDIR += src/ip/atl/V0
endif
