ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
  PACKAGE_SRCS_COMMON += c7x/Cache.h  c7x/Clobber.h  c7x/cslr_C7X_CPU.h  c7x/Exception.h  c7x/Hwi.h
  PACKAGE_SRCS_COMMON += c7x/IntrinsicsSupport.h  c7x/Mmu.h  c7x/src_files_c7x.mk  c7x/TimestampProvider.h 
  PACKAGE_SRCS_COMMON += c7x/src/boot.c c7x/src/Cache.c c7x/src/Exception_asm.asm c7x/src/Hwi_asm_switch.asm
  PACKAGE_SRCS_COMMON += c7x/src/IntrinsicsSupport.c c7x/src/Mmu_table.c c7x/src/c7x_module_config.c c7x/src/Exception.c
  PACKAGE_SRCS_COMMON += c7x/src/Hwi.c c7x/src/Mmu_asm.asm c7x/src/Startup.c c7x/src/Cache_asm.asm
  PACKAGE_SRCS_COMMON += c7x/src/Clobber_asm.asm c7x/src/Hwi_asm.asm c7x/src/Hwi_disp_always.asm c7x/src/Mmu.c 
  ifeq ($(ARCH),c71)
    SRCDIR += c7x/src
    INCDIR += c7x
    SRCS_ASM_COMMON += Cache_asm.asm Clobber_asm.asm Exception_asm.asm Hwi_asm.asm
    SRCS_ASM_COMMON += Hwi_asm_switch.asm Hwi_disp_always.asm Mmu_asm.asm
    SRCS_COMMON += boot.c c7x_module_config.c Cache.c Exception.c Hwi.c IntrinsicsSupport.c Mmu.c
    SRCS_COMMON += Mmu_table.c Startup.c
    CFLAGS_LOCAL_COMMON += -DHwi_bootToNonSecure__D=true
    CFLAGS_LOCAL_COMMON += -DException_vectors__D
  endif
endif

ifeq ($(SOC),$(filter $(SOC), j721e j721s2))
  ifeq ($(ARCH),$(filter $(ARCH), c71 c7x))
    PACKAGE_SRCS_COMMON += c7x/src/clec/csl_clec.h c7x/src/clec/cslr_clec.h c7x/src/clec/V0
    SRCDIR += ./c7x/src/clec/V0/priv
    INCDIR += ./c7x/src/clec/V0
    SRCS_COMMON += csl_clec.c
  endif
endif

ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
  ifeq ($(ARCH),$(filter $(ARCH), c71 c7x))
    PACKAGE_SRCS_COMMON += c7x/src/clec/csl_clec.h c7x/src/clec/cslr_clec.h c7x/src/clec/V1
    SRCDIR += ./c7x/src/clec/V1/priv
    INCDIR += ./c7x/src/clec/V1
    SRCS_COMMON += csl_clec.c
  endif
endif