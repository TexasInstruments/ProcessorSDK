
ifeq ($(SOC),$(filter $(SOC), tda2xx tda2px dra75x tda2ex dra72x tda3xx am574x am572x am571x dra78x))

PACKAGE_SRCS_COMMON += arch/m4 cslr_unicache_cfg.h
  ifeq ($(ARCH),armv7m)
  SRCDIR += ./arch/m4/src
  INCDIR += ./arch/m4
  SRCS_COMMON += interrupt.c ipu_wugen.c ammu.c unicache.c
    ifeq ($(SOC),$(filter $(SOC), tda3xx dra78x))
      SRCS_COMMON += ipu_ecc.c
    endif
  endif

endif

ifeq ($(SOC),$(filter $(SOC), am64x am263x am62x am275x))
PACKAGE_SRCS_COMMON += arch/m4
  ifeq ($(ARCH),armv7m)
  SRCDIR += ./arch/m4/src
  INCDIR += ./arch/m4
  SRCS_ASM_COMMON += csl_arm_m4.asm
  SRCS_COMMON += interrupt.c csl_m4.c csl_m4_mpu.c default_handlers.c
  endif
endif
