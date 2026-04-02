#
# Copyright (c) 2016 - 2020, Texas Instruments Incorporated
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
SOC_DEP_LIB_SOCS=j721e j7200 j721s2 j784s4 j742s2

PACKAGE_SRCS_COMMON = makefile SPI.h MCSPI.h spi_component.mk .gitignore ospi_modes.md \
                      docs/ReleaseNotes_SPI_LLD.pdf docs/ESPI_FW_DESIGN_GUIDE.docx \
                      src/SPI_drv.c src/SPI_osal.h src/src_files_common.mk build

ifeq ($(SOC),$(filter $(SOC),$(SOC_DEP_LIB_SOCS) ))
  # TDA & J7 targets include SPI_V1, QSPI_V1(for Sitara) , OSPI_V1(For J7)
  SRCDIR = . src src/v1
  INCDIR = . src src/v1

  SRCS_COMMON += SPI_drv.c SPI_v1.c
  PACKAGE_SRCS_COMMON += src/v1/SPI_v1.c soc/SPI_v1.h

  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
    SRCDIR += src/v0
    INCDIR += src/v0
    SRCS_COMMON += OSPI_v0.c
    PACKAGE_SRCS_COMMON += src/v0/OSPI_v0.c src/v0/OSPI_v0.h
  endif

endif

