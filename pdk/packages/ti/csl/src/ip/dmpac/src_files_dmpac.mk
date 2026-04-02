
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += cslr_dmpac.h src/ip/dmpac/src_files_dmpac.mk src/ip/dmpac/V0
INCDIR += src/ip/dmpac/V0
endif
