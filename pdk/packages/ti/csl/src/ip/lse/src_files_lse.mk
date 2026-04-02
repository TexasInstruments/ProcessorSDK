
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_lse.h src/ip/lse/src_files_lse.mk src/ip/lse/V0
INCDIR += src/ip/lse/V0
endif

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += src/ip/lse/V1
INCDIR += src/ip/lse/V1
endif
