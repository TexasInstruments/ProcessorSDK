ifeq ($(SOC),$(filter $(SOC), am65xx j7200 j721e j721s2 j784s4 j742s2 am64x))

  PACKAGE_SRCS_COMMON += a53/src/startup

  ifeq ($(ARCH),armv8a)
    SRCDIR += a53/src/startup/aarch64
    INCDIR += a53/src/startup/aarch64
	INCDIR += a53/src/startup/common
    INCDIR += a53

    SRCS_COMMON += csl_a53v8_startup.c
    SRCS_ASM_COMMON += bootcode.asm
    SRCS_ASM_COMMON += vectors.asm exceptionhandler.asm
  endif

endif
