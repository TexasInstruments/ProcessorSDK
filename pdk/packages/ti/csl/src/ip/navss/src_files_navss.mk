
ifeq ($(SOC),$(filter $(SOC), am65xx j7200 j721e j721s2 j784s4 j742s2))

  PACKAGE_SRCS_COMMON += cslr_navss.h cslr_navss_mcu.h cslr_navss_main.h src/ip/navss/src_files_navss.mk
  PACKAGE_SRCS_COMMON += csl_navss_main.h csl_navss_mcu.h
  ifeq ($(SOC),$(filter $(SOC), am65xx))
    PACKAGE_SRCS_COMMON += src/ip/navss/V0
  endif
  ifeq ($(SOC),$(filter $(SOC), j721e))
    PACKAGE_SRCS_COMMON += src/ip/navss/V1
  endif
  ifeq ($(SOC),$(filter $(SOC), j7200))
    PACKAGE_SRCS_COMMON += src/ip/navss/V2
  endif
  ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
    PACKAGE_SRCS_COMMON += src/ip/navss/V3
  endif
endif
