
ifeq ($(SOC),$(filter $(SOC), j7200 j721e))
PACKAGE_SRCS_COMMON += cslr_vpac.h src/ip/vpac/src_files_vpac.mk src/ip/vpac/V0
INCDIR += src/ip/vpac/V0
endif

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_vpac3.h src/ip/vpac/src_files_vpac.mk src/ip/vpac/V3
INCDIR += src/ip/vpac/V3
endif

ifeq ($(SOC),$(filter $(SOC), am62a))
PACKAGE_SRCS_COMMON += cslr_vpac.h src/ip/vpac/src_files_vpac.mk src/ip/vpac/V4
INCDIR += src/ip/vpac/V4
endif
