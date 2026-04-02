
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
PACKAGE_SRCS_COMMON += cslr_hwa.h csl_complex_math_types.h src/ip/hwa/src_files_hwa.mk src/ip/hwa/V1
INCDIR += src/ip/hwa/V1
endif


