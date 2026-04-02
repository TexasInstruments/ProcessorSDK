ifeq ($(SOC),$(filter $(SOC), am64x am62x am263x am275x))

  PACKAGE_SRCS_COMMON += m4/src/startup
  ifeq ($(ARCH),armv7m)
    SRCDIR += m4/src/startup
    INCDIR += m4/src/startup
    INCDIR += m4
    SRCS_ASM_COMMON += boot.asm
    SRCS_COMMON += startup.c
  endif

endif
