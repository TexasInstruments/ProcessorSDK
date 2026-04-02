#
# Copyright (c) 2013-2024, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# Filename: build_config.mk
#
# Build configuration make file - This file contains all the options that are
#                                 frequently changed
#
# This file changes when:
#     1. Format needs to change (eg: COFF/ELF)
#     2. Endian-ness needs to change (little/big)
#     3. Global compiler and linker options need to change (one entry for each core)
#

# Global options

# Endianness : Allowed values = little | big
ENDIAN = little

# Format : Allowed values = COFF | ELF
FORMAT = ELF

#
# BOARD specific
#
CFLAGS_GLOBAL_j721e_hostemu     = -DSOC_J721E -DHOST_EMULATION
CFLAGS_GLOBAL_j721e_evm         = -DSOC_J721E -Dj721e_evm=j721e_evm
CFLAGS_GLOBAL_j7200_hostemu     = -DSOC_J7200 -DHOST_EMULATION
CFLAGS_GLOBAL_j7200_evm         = -DSOC_J7200 -Dj7200_evm=j7200_evm
CFLAGS_GLOBAL_j721s2_evm        = -DSOC_J721S2 -Dj721s2_evm=j721s2_evm
CFLAGS_GLOBAL_j721s2_hostemu    = -DSOC_J721S2 -DHOST_EMULATION
CFLAGS_GLOBAL_j784s4_evm        = -DSOC_J784S4 -Dj784s4_evm=j784s4_evm
CFLAGS_GLOBAL_j784s4_hostemu    = -DSOC_J784S4 -DHOST_EMULATION
CFLAGS_GLOBAL_j742s2_evm        = -DSOC_J742S2 -Dj742s2_evm=j742s2_evm
CFLAGS_GLOBAL_j742s2_hostemu    = -DSOC_J742S2 -DHOST_EMULATION

#
# SOC specific
#
CFLAGS_GLOBAL_j721e          = -DSOC_J721E
CFLAGS_GLOBAL_j7200          = -DSOC_J7200
CFLAGS_GLOBAL_j721s2         = -DSOC_J721S2
CFLAGS_GLOBAL_j784s4         = -DSOC_J784S4
CFLAGS_GLOBAL_j742s2         = -DSOC_J742S2
#
# Core specific options
#

# MCU Cores
ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 m4f_0))
  CFLAGS_GLOBAL_$(CORE) = -g -DMAKEFILE_BUILD
  LNKFLAGS_GLOBAL_$(CORE) = -Xlinker -x -Xlinker --zero_init=on
endif

# MPU Cores
ifeq ($(CORE),$(filter $(CORE), mpu1_0 mpu1_1 mpu1_2 mpu1_3 mpu2_0 mpu2_1 mpu2_2 mpu2_3))
  CFLAGS_GLOBAL_$(CORE) = -DMAKEFILE_BUILD
  LNKFLAGS_GLOBAL_$(CORE) =
endif

# C7x DSP
CFLAGS_GLOBAL_c7x_1 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c7x_1 = -x
CFLAGS_GLOBAL_c7x_2 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c7x_2 = -x
CFLAGS_GLOBAL_c7x_3 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c7x_3 = -x
CFLAGS_GLOBAL_c7x_4 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c7x_4 = -x

# C7x HOST
CFLAGS_GLOBAL_c7x-hostemu = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c7x-hostemu =

# C66x DSP
CFLAGS_GLOBAL_c66xdsp_1 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c66xdsp_1 = -x
CFLAGS_GLOBAL_c66xdsp_2 = -DMAKEFILE_BUILD
LNKFLAGS_GLOBAL_c66xdsp_2 = -x

# Nothing beyond this point