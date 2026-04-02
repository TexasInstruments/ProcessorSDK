
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
PACKAGE_SRCS_COMMON += cslr_cpdma.h csl_cpdma.h src/ip/cpdma/src_files_cpdma.mk src/ip/cpdma/V0
SRCDIR += src/ip/cpdma/V0/priv
INCDIR += src/ip/cpdma/V0
SRCS_COMMON += csl_cpdma.c
endif
