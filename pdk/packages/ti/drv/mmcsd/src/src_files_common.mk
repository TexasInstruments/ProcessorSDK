
# Common source files across all platforms and cores
SRCS_COMMON += MMCSD_drv.c

PACKAGE_SRCS_COMMON = makefile MMCSD.h mmcsd_component.mk src/MMCSD_osal.h \
                      docs/ReleaseNotes_MMCSD_LLD.pdf \
                      src/MMCSD_drv.c src/MMCSD_drv_log.h \
                      soc/MMCSD_soc.h \
                      build  src/src_files_common.mk MMCSDver.h

ifeq ($(SOC), $(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
SRCDIR = . src src/v2
INCDIR = . src src/v2
SRCS_COMMON += MMCSD_v2.c
PACKAGE_SRCS_COMMON += src/v2 soc/MMCSD_v2.h
endif
