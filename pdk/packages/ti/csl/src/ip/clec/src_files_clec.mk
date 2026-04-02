
ifneq ($(ARCH),$(filter $(ARCH), c71 c7x))
    PACKAGE_SRCS_COMMON += cslr_clec.h csl_clec.h
    ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2))
    PACKAGE_SRCS_COMMON += src/ip/clec/src_files_clec.mk src/ip/clec/V0
    SRCDIR += src/ip/clec/V0/priv
    INCDIR += src/ip/clec/V0
    endif

    ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
    PACKAGE_SRCS_COMMON += src/ip/clec/src_files_clec.mk src/ip/clec/V1
    SRCDIR += src/ip/clec/V1/priv
    INCDIR += src/ip/clec/V1
    endif

    ifeq ($(SOC),$(filter $(SOC), am62a))
    PACKAGE_SRCS_COMMON += src/ip/clec/src_files_clec.mk src/ip/clec/V2
    SRCDIR += src/ip/clec/V2/priv
    INCDIR += src/ip/clec/V2
    endif
    SRCS_COMMON += csl_clec.c
endif