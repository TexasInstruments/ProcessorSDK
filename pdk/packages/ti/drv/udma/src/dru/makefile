
SRCDIR += src/dru/V0/src
PACKAGE_SRCS_COMMON += src/dru/V0/src
PACKAGE_SRCS_COMMON += src/dru/csl_dru.h src/dru/cslr_dru.h src/dru/makefile
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += src/dru/V0/V0_2/csl_dru.h
PACKAGE_SRCS_COMMON += src/dru/V0/V0_2/cslr_dru.h
else
PACKAGE_SRCS_COMMON += src/dru/V0/V0_1/csl_dru.h
PACKAGE_SRCS_COMMON += src/dru/V0/V0_1/cslr_dru.h
endif
SRCS_COMMON += csl_dru.c 