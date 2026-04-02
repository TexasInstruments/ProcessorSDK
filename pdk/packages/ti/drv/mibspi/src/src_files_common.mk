#
# Copyright (c) 2020, Texas Instruments Incorporated
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
SOC_DEP_LIB_SOCS=tpr12 awr294x

PACKAGE_SRCS_COMMON = makefile MIBSPI.h mibspi_component.mk .gitignore \
                      src/MIBSPI_osal.h src/src_files_common.mk \
                      build/makefile.mk  \
                      build/makefile_dma.mk
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc

ifeq ($(SOC),$(filter $(SOC),$(SOC_DEP_LIB_SOCS) ))
  ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
    # TPR12 AWR294X targets include SPI_V3
    SRCDIR = . src
    INCDIR = . src
    SRCS_COMMON += mibspi_priv.c MIBSPI_api.c mibspi_trace.c mibspi_utils.c
    PACKAGE_SRCS_COMMON += src/mibspi_priv.c soc/MIBSPI_v0.h
    PACKAGE_SRCS_COMMON += src/mibspi_trace.c src/mibspi_priv.h
    PACKAGE_SRCS_COMMON += src/mibspi_trace.h src/mibspi_trace_config.h
    PACKAGE_SRCS_COMMON += src/mibspi_trace_priv.h src/mibspi_utils.c
    PACKAGE_SRCS_COMMON += src/mibspi_utils.h src/MIBSPI_api.c
    PACKAGE_SRCS_COMMON += test
  endif
endif


ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
  MIBSPI_CFLAGS =

  # Enable asserts and prints
  MIBSPI_CFLAGS += -DMIBSPI_CFG_ASSERT_ENABLE
  #MIBSPI_CFLAGS += -DMIBSPI_CFG_USE_STD_ASSERT
  MIBSPI_CFLAGS += -DMIBSPI_CFG_PRINT_ENABLE
  #MIBSPI_CFLAGS += -DUART_ENABLED

  # Trace level per build profile:
  # 0 - None
  # 1 - Error
  # 2 - Warning
  # 3 - Info
  # 4 - Debug
  # 5 - Verbose
  ifeq ($(BUILD_PROFILE),debug)
    MIBSPI_CFLAGS += -DMIBSPI_TRACE_CFG_TRACE_LEVEL=4
    MIBSPI_CFLAGS += -DMIBSPI_CFG_DEV_ERROR
  else
    MIBSPI_CFLAGS += -DMIBSPI_TRACE_CFG_TRACE_LEVEL=3
  endif
endif

