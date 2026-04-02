
ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2 am62x am62a am62px))
  # CSI-RX has compilation errors in host emulation build. This is not required in host emu. Hence disable
  ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
    PACKAGE_SRCS_COMMON += cslr_csirx.h csl_csirx.h src/ip/csirx/src_files_csirx.mk src/ip/csirx/V0
    SRCDIR += src/ip/csirx/V0/priv
    INCDIR += src/ip/csirx/V0 src/ip/csirx/V0/priv
    SRCS_COMMON += csirx.c csirx_ss.c csirx_sanity.c
  endif
endif

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
  PACKAGE_SRCS_COMMON += cslr_csirx.h src/ip/csirx/src_files_csirx.mk src/ip/csirx/V1
endif
