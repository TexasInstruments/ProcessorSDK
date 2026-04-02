#
# Copyright (c) 2024, Texas Instruments Incorporated
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

cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm/include
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/pm/include
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm/include/soc/$(TARGET_SOC)
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm
cppflags-$(CONFIG_LPM_32_BIT_DDR) += -I$(srctree)/lpm/cdns_generated_defines

obj-y += soc/

obj-y += main.o
obj-y += psc_raw.o
obj-y += pll_16fft_raw.o
obj-y += ctrlmmr_raw.o
obj-$(CONFIG_LPM_DM_TRACE_UART) += 8250.o
obj-$(CONFIG_LPM_DM_TRACE_BUFFER) += lpm_trace_buffer.o
obj-y += timeout.o
obj-y += sec_proxy.o
obj-y += ddr.o
obj-y += vim_raw.o
obj-y += lpm_string.o
obj-y += lpm_trace.o
obj-$(CONFIG_LPM_32_BIT_DDR) += cdns_ddr_reg_config.o
obj-y += cache.o
