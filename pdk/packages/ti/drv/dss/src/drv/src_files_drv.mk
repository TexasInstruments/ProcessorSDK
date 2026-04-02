SRCDIR += src/drv/common src/drv/dctrl src/drv/disp
INCDIR += .

PACKAGE_SRCS_COMMON += src/drv/common src/drv/dctrl src/drv/disp
# Common driver source files across all platforms and cores
SRCS_COMMON +=
include src/drv/common/src_files_common.mk
include src/drv/dctrl/src_files_dctrl.mk
include src/drv/disp/src_files_disp.mk

ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
SRCDIR += src/drv/m2m
PACKAGE_SRCS_COMMON += src/drv/m2m
include src/drv/m2m/src_files_m2m.mk
endif