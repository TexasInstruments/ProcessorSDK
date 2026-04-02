#
# Copyright (c) 2017-2020, Texas Instruments Incorporated
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

ifneq ($(CONFIG_PM),)

obj-$(CONFIG_UDELAY) +=		udelay_m3.o

obj-y +=			cm3.o

endif

obj-y += lib/

CROSS_COMPILE ?= arm-linux-gnueabihf-

__src := $(src)

cppflags-y += -I$(__src)/include

cflags-y += --unaligned_access=off

ifeq ($(CONFIG_ARMM4),y)
cflags-y += --silicon_version=7M4 --code_state=16 --little_endian
aflags-y += --silicon_version=7M4 --code_state=16 --little_endian
else
cflags-y += --silicon_version=7M3 --code_state=16 --little_endian
aflags-y += --silicon_version=7M3 --code_state=16 --little_endian
endif

ldflags-y += --disable_auto_rts

LD_SCRIPT = $(objtree)/app/linkercmd/arm.ld
extra-y += $(LD_SCRIPT)

TARGET = ti-sci-user-firmware.elf
