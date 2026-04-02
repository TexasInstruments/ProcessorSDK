ifeq ($(CORE),$(filter $(CORE), $(vhwa_vpac_$(SOC)_CORELIST)))
 SRCS_COMMON += csl_vpacTop.c csl_nf.c \
 							  csl_glbce.c csl_h3a.c csl_ee.c
endif

ifeq ($(CORE),$(filter $(CORE), $(vhwa_dmpac_$(SOC)_CORELIST)))
 SRCS_COMMON += csl_dmpacTop.c csl_dof.c csl_sde.c
endif
