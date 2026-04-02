
ifeq ($(SOC),$(filter $(SOC), j721e))
PACKAGE_SRCS_COMMON += csl_pok.h src/ip/pok/src_files_pok.mk src/ip/pok/V0
SRCS_COMMON += csl_pok.c csl_pokId2Addr.c
SRCDIR += src/ip/pok/V0/priv
INCDIR += src/ip/pok_0
endif

