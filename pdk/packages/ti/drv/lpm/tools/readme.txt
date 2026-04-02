The directory ti/drv/lpm/src/s2r contains an lpm module for Suspend to RAM
support.
Each time a modification is done in ti/drv/lpm/src/s2r/lpm_s2r.c, the script
lpm_s2r_gen.sh shall be called.
It automatically generates and commits header files (for each supported board)
which contain the binary code for Suspend to RAM support.
