
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_cgem.h src/ip/cgem/src_files_cgem.mk src/ip/cgem/V1
endif

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
PACKAGE_SRCS_COMMON += cslr_cgem.h src/ip/cgem/src_files_cgem.mk src/ip/cgem/V0
endif
