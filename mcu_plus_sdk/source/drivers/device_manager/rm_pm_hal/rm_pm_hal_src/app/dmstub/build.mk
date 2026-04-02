#
# Copyright (c) 2021-2024, Texas Instruments Incorporated
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

cppflags-y += -I$(objtree)/include -I$(srctree)/include -I$(srcroot)/include
cppflags-y += -I$(srctree)/security/include
cppflags-y += -I$(srctree)/tests/include
cppflags-y += -I$(TOOLCHAIN_PATH_M3)/include
cppflags-$(CONFIG_DEVICE_TYPE_HS) += -I$(srctree)/hs/security/include

cppflags-y += -I$(BIOS_PACKAGES_DIR)/ti/sysbios/posix

cppflags-y += -I$(BIOS_INSTALL_PATH)/packages
cppflags-y += -I$(XDC_INSTALL_PATH)/packages

cppflags-y += -I$(srctree)/include/soc/$(TARGET_SOC)/
cppflags-y += -I$(BIOS_INSTALL_PATH)/packages

cflags-y += $(cppflags-y)
cflags-y += -g --opt_for_speed=0 $(OPT_LEVEL) -pdr -pdew -pds=230 -pds=1463
cflags-y += --gen_func_subsections=on
cflags-y += -me
cflags-y += --display_error_number --diag_warning=255 --diag_wrap=off
aflags-y += -g $(cppflags-y)

cflags-y += --unaligned_access=off
cflags-y += -mv7R5

aflags-y += -mv7R5 -me

ldflags-y += --disable_auto_rts --unused_section_elimination=on

stripflags-y += -pdew -p

binflags-y += -b --byte --image --zero --memwidth=8 --linkerfill -fill 0x00000000 -pdew --issue_remarks -q
prepare_headers-y += $(srctree)/include/config.h

obj-y += entry.o startup.o
