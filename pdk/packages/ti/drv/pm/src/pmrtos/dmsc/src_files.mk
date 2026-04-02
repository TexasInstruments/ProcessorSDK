# Source files and CFLAGS across all platforms and cores

SRCDIR += src/pmrtos/dmsc src/pmrtos/common
INCDIR += src/pmrtos/dmsc src/pmrtos/common

ifeq ($(SOC),$(filter $(SOC), am65xx))
SRCDIR += src/pmrtos/dmsc/V0
INCDIR += src/pmrtos/dmsc/V0
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200))
SRCDIR += src/pmrtos/dmsc/V1
INCDIR += src/pmrtos/dmsc/V1
endif


SRCS_COMMON += List.c PowerDevice.c PowerDevice_tirtos.c

ifeq ($(SOC),$(filter $(SOC), am65xx))
SRCS_COMMON += PowerDMSC_deviceInit.c
endif

# Nothing beyond this point
