
ifeq ($(SOC),$(filter $(SOC), k2h k2k k2l k2e k2g c6657 c6678 omapl137 omapl138 tpr12 awr294x))
PACKAGE_SRCS_COMMON += cslr_spi.h src/ip/spi/src_files_spi.mk src/ip/spi/V0
SRCS_COMMON += spi.c
SRCDIR += src/ip/spi/V0/priv
ifeq ($(SOC),$(filter $(SOC), k2h k2k k2l k2e k2g c6657 c6678 omapl137 omapl138))
  INCDIR += . src/ip/spi/V0_0
endif

ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
  INCDIR += . src/ip/spi/V0_1
endif

endif
