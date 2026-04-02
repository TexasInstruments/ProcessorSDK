ifeq ($(SOC),$(filter $(SOC), am62lx))
PACKAGE_SRCS_COMMON += cslr_xlcdma.h src/ip/xlcdma/src_files_xlcdma.mk src/ip/xlcdma/V0
INCDIR += src/ip/xlcdma/V0
endif
