ifeq ($(SOC),$(filter $(SOC), am263x am62lx))
PACKAGE_SRCS_COMMON += src/ip/sha/V1/cslr_sha.h
endif
