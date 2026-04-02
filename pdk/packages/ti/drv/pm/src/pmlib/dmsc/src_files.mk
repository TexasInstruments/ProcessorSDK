# Source files and CFLAGS across all platforms and cores

SRCDIR += dmsc src/pmlib/dmsc
INCDIR += dmsc src/pmlib/dmsc

SRCS_COMMON += pmlib_sysconfig.c pmlib_clkrate.c pmlib_thermal.c pmlib_vtm_vd.c

# Nothing beyond this point
