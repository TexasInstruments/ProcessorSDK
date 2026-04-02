ifeq ($(SOC),$(filter $(SOC), am263x))
PACKAGE_SRCS_COMMON += src/ip/dthe/V0/cslr_dthe.h
endif

ifeq ($(SOC),$(filter $(SOC), am62lx))
PACKAGE_SRCS_COMMON += cslr_dthe.h src/ip/dthe/src_files_dthe.mk src/ip/dthe/V1/
INCDIR += src/ip/dthe/V1/
endif
