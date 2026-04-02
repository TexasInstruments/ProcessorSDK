
ifeq ($(SOC),$(filter $(SOC), am65xx j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_hyperbus.h csl_hyperbus.h src/ip/hyperbus/src_files_hyperbus.mk src/ip/hyperbus/V0
SRCDIR += src/ip/hyperbus/V0/priv
INCDIR += src/ip/hyperbus/V0
SRCS_COMMON += csl_hyperbus.c
endif
