
ifeq ($(SOC),$(filter $(SOC), am64x))
PACKAGE_SRCS_COMMON += cslr_fsi_rx.h csl_fsi_rx.h src/ip/fsi_rx/src_files_fsi_rx.mk src/ip/fsi_rx/V0
SRCDIR += src/ip/fsi_rx/V0/priv
INCDIR += . src/ip/fsi_rx/V0
SRCS_COMMON += csl_fsi_rx.c
endif
