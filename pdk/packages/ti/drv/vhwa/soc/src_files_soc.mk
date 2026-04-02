SRCS_COMMON += csl_nsf4.c csl_rawfe.c
SRCS_COMMON += csl_fcp.c csl_hts.c csl_visstop.c
SRCS_COMMON += csl_lse.c csl_ldc.c csl_msc.c vhwa_soc_fc.c

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
SRCS_COMMON += csl_cac.c
endif


ifeq ($(CORE),$(filter $(CORE), $(vhwa_vpac_$(SOC)_CORELIST)))
SRCS_COMMON += vhwa_vpac_soc.c
endif

ifeq ($(CORE),$(filter $(CORE), $(vhwa_dmpac_$(SOC)_CORELIST)))
SRCS_COMMON += vhwa_dmpac_soc.c csl_dmpac_hts.c
endif
