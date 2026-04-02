
SRCDIR += src/intaggr/V0/src
PACKAGE_SRCS_COMMON += src/intaggr/V0/src
PACKAGE_SRCS_COMMON += src/intaggr/csl_intaggr.h src/intaggr/cslr_intaggr.h src/intaggr/makefile
PACKAGE_SRCS_COMMON += src/intaggr/V0/csl_intaggr.h
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += src/intaggr/V0/V0_2/cslr_intaggr.h
else
PACKAGE_SRCS_COMMON += src/intaggr/V0/V0_1/cslr_intaggr.h
endif
SRCS_COMMON += csl_intaggr.c 