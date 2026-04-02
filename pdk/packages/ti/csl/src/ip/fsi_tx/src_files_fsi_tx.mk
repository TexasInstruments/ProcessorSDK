
ifeq ($(SOC),$(filter $(SOC), am64x))
PACKAGE_SRCS_COMMON += cslr_fsi_tx.h csl_fsi_tx.h src/ip/fsi_tx/src_files_fsi_tx.mk src/ip/fsi_tx/V0
SRCDIR += src/ip/fsi_tx/V0/priv
INCDIR += . src/ip/fsi_tx/V0
SRCS_COMMON += csl_fsi_tx.c
endif
