SRCS_COMMON += dss_dctrlGraph.c dss_dctrlApi.c

ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
SRCS_COMMON += dss_dctrlExtended.c  dss_dctrlDsi.c
endif
