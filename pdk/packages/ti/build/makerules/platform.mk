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

# Filename: platform.mk
#
# Platforms make file - Board/SoC/targets are defined/derived in this file
#
# This file needs to change when:
#     1. a new Board/SoC is added, which also might have its own cores/ISAs
#

#
# Derive SOC from BOARD
#
#

# J721E
ifeq ($(BOARD),$(filter $(BOARD),j721e_hostemu j721e_evm))
 SOC = j721e
 SBL_RUN_ADDRESS=0x41C00100
 SBL_DEV_ID=55
endif

# J7200
ifeq ($(BOARD),$(filter $(BOARD),j7200_hostemu j7200_evm))
 SOC = j7200
 SBL_RUN_ADDRESS=0x41C00100
 SYSFW_LOADADDR=0x40000
 COMBINED_TIFS_BRDCFG_LOADADDR=0x7f000
 COMBINED_DM_BRDCFG_LOADADDR=0x41c80000
 SBL_DEV_ID=55
endif

# J721S2
ifeq ($(BOARD),$(filter $(BOARD), j721s2_evm j721s2_hostemu))
 SOC = j721s2
 SBL_RUN_ADDRESS=0x41C00100
 SYSFW_LOADADDR=0x40000
 COMBINED_TIFS_BRDCFG_LOADADDR= 0x67000
 COMBINED_DM_BRDCFG_LOADADDR=0x41c80000
 SBL_DEV_ID=55
endif

# J784S4
ifeq ($(BOARD),$(filter $(BOARD), j784s4_evm j784s4_hostemu j742s2_evm j742s2_hostemu))
 SOC = j784s4
 SBL_RUN_ADDRESS=0x41C00100
 SYSFW_LOADADDR=0x40000
 COMBINED_TIFS_BRDCFG_LOADADDR=0x66800
 COMBINED_DM_BRDCFG_LOADADDR=0x41c80000
 SBL_DEV_ID=55
ifeq ($(BOARD),$(filter $(BOARD), j742s2_evm j742s2_hostemu))
 SOC = j742s2
endif
endif

# SBL related macro
export SBL_CERT_KEY_HS=$(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem
export SBL_CERT_KEY=$(ROOTDIR)/ti/build/makerules/rom_degenerateKey.pem

#
# Derive Target/ISA from CORE
#

# MCU
ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
 ISA = r5f
 ISA_EXT = r5f
 ARCH = armv7r
endif

# MPU
ifeq ($(CORE),$(filter $(CORE), mpu1_0 mpu1_1 mpu1_2 mpu1_3 mpu2_0 mpu2_1 mpu2_2 mpu2_3))
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    ISA = a72
    ISA_EXT = a72
    ARCH = armv8a
  endif
endif

# C7x DSP
ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x_2 c7x_3 c7x_4))
 ISA = c7x
 ISA_EXT = 71
 ARCH = c71
 ifeq ($(SOC),$(filter $(SOC), j721e))
  SI_VER = 7100
 endif
 ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
  SI_VER = 7120
 endif
endif

# C7x DSP
ifeq ($(CORE),$(filter $(CORE), c7x-hostemu))
 ISA = c7x-hostemu
 ISA_EXT = c7x-hostemu
 ARCH = c7x
 ifeq ($(SOC),$(filter $(SOC), j721e))
  SI_VER = 7100
 endif
 ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j742s2))
  SI_VER = 7120
 endif
endif

# DSP for c66xdsp_1/c66xdsp_2
ifeq ($(CORE),$(filter $(CORE), c66xdsp_1 c66xdsp_2))
 ISA = c66
 ISA_EXT = 66
 ARCH = c66x
endif

ifeq ($(ISA),r5f)
  ifeq ($(FORMAT),ELF)
    FORMAT_EXT = e
  endif

  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)

  # Retaining the r5f extension for backward compatibility
  LIBEXT = a$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
endif

ifeq ($(ISA),a72)
  ENDIAN_EXT = fg
  FORMAT_EXT =

  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
endif

#########C7x DSP Support############
ifeq ($(ISA),c7x)
  ifeq ($(FORMAT),ELF)
    FORMAT_EXT = e
  endif

  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
endif

#########C7x Host Support############
ifeq ($(ISA),c7x-hostemu)
  # Define the file extensions
  OBJEXT = obj
  LIBEXT = lib
  EXEEXT = exe
endif

#########DSP Support############
ifeq ($(ISA),c66)
  ifeq ($(FORMAT),ELF)
    FORMAT_EXT = e
  endif

  # If ENDIAN is set to "big", set ENDIAN_EXT to "e", that would be used in
  #    in the filename extension of object/library/executable files
  ifeq ($(ENDIAN),big)
    ENDIAN_EXT = e
  endif

  # Define the file extensions
  OBJEXT = o$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  LIBEXT = a$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  EXEEXT = x$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
  ASMEXT = s$(FORMAT_EXT)$(ISA_EXT)$(ENDIAN_EXT)
endif

ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
  SBL_CORE_ID_mpu1_0 = 0
  SBL_CORE_ID_mpu1_1 = 1
  SBL_CORE_ID_mpu1_2 = 2
  SBL_CORE_ID_mpu1_3 = 3
  SBL_CORE_ID_mpu2_0 = 4
  SBL_CORE_ID_mpu2_1 = 5
  SBL_CORE_ID_mpu2_2 = 6
  SBL_CORE_ID_mpu2_3 = 7
  SBL_CORE_ID_mcu1_0 = 8
  SBL_CORE_ID_mcu1_1 = 9
  SBL_CORE_ID_mcu2_0 = 10
  SBL_CORE_ID_mcu2_1 = 11
  SBL_CORE_ID_mcu3_0 = 12
  SBL_CORE_ID_mcu3_1 = 13
  SBL_CORE_ID_mcu4_0 = 14
  SBL_CORE_ID_mcu4_1 = 15
  SBL_CORE_ID_c66xdsp_1 = 16
  SBL_CORE_ID_c66xdsp_2 = 17
  SBL_CORE_ID_c7x_1 = 18
  SBL_CORE_ID_c7x_2 = 19
  SBL_CORE_ID_c7x_3 = 20
  SBL_CORE_ID_c7x_4 = 21
  SBL_CORE_ID_c7x-hostemu = 22
  SBL_CORE_ID_hsm = 23
  SBL_CORE_ID_mpu1_smp = 24
  SBL_CORE_ID_mpu2_smp = 25
  SBL_CORE_ID_mpu_smp = 26
  SBL_CORE_ID_mcu1_0_smp = 27
  SBL_CORE_ID_mcu2_0_smp = 28
  SBL_CORE_ID_mcu3_0_smp = 29
  SBL_CORE_ID_mcu4_0_smp = 30
  SBL_CORE_ID_load_only = 31
endif

export SOC
export ARCH

# Nothing beyond this point