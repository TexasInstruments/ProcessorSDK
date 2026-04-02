
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_vpfe.h src/ip/vpfe/src_files_vpfe.mk src/ip/vpfe/V0
INCDIR += src/ip/vpfe/V0
endif
