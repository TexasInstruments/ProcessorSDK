
ifeq ($(SOC),$(filter $(SOC), j721e))
# CSI-TX has compilation errors in host emulation build. This is not required in host emu. Hence disable
ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
PACKAGE_SRCS_COMMON += cslr_csitx.h csl_csitx.h src/ip/csitx/src_files_csitx.mk src/ip/csitx/V0
SRCDIR += src/ip/csitx/V0/priv
INCDIR += src/ip/csitx/V0 src/ip/csitx/V0/priv
SRCS_COMMON += csitx_sanity.c csitx.c csitx_ss.c
endif
endif

ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
# CSI-TX has compilation errors in host emulation build. This is not required in host emu. Hence disable
ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
PACKAGE_SRCS_COMMON += cslr_csitx.h csl_csitx.h src/ip/csitx/src_files_csitx.mk src/ip/csitx/V0 src/ip/csitx/V1
SRCDIR += src/ip/csitx/V0/priv
INCDIR += src/ip/csitx/V0 src/ip/csitx/V0/priv
SRCS_COMMON += csitx_sanity.c csitx.c csitx_ss.c
endif
endif
