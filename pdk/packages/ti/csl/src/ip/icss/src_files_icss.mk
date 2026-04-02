
ifeq ($(SOC),$(filter $(SOC), am65xx j7200 j721e))
PACKAGE_SRCS_COMMON += cslr_icss.h src/ip/icss/src_files_icss.mk src/ip/icss/V0 src/ip/icss/V1 src/ip/icss/V2
endif

ifeq ($(SOC),$(filter $(SOC), am64x))
PACKAGE_SRCS_COMMON += cslr_icss.h src/ip/icss/src_files_icss.mk src/ip/icss/V1 src/ip/icss/V3
endif

ifeq ($(SOC),$(filter $(SOC), am62x am62a am62px))
PACKAGE_SRCS_COMMON += cslr_icss.h src/ip/icss/src_files_icss.mk src/ip/icss/V4
endif
