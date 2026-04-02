#
# Copyright (c) 2018-2020, Texas Instruments Incorporated
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


# Not required to update the package files as we use XDC to package files
PACKAGE_SRCS_COMMON = 

# Start with empty list
SRCS_IPSEC_FILES     =
SRCS_SRTP_FILES      =
SRCS_AIRCIPHER_FILES = 
SRCS_PKAFW_FILES       = 

# The following source files are all the common source and include files
  SRCDIR = . src src/auth src/proto/datamode src/cipher
  INCDIR = . src src/auth src/proto/datamode src/cipher

# Top level source files common across SoCs
  SRCS_COMMON           = salld.c salldcom.c salldctbl.c salldmci.c salldsc.c salldtxrx.c

# For non LITE have Have the ipsec, srtp and aircipher protocols   
ifneq ($(SOC),$(filter $(SOC), k2g am65xx j721e am64x))
  SRCS_IPSEC_FILES     += salldah.c    salldesp.c       salldipsec.c     salldipsecinit.c
  SRCS_SRTP_FILES      += salldsrtcp.c salldsrtcp_ctr.c salldsrtcp_f8.c  salldsrtcpinit.c  
  SRCS_SRTP_FILES      += salldsrtp.c  salldsrtp_ctr.c  salldsrtp_f8.c   salldsrtpinit.c   salldsrtpsc.c
  SRCS_AIRCIPHER_FILES += salldac.c  salldacinit.c
  SRCDIR += src/proto/ipsec src/proto/srtp src/proto/airciph
  INCDIR += src/proto/ipsec src/proto/srtp src/proto/airciph
else
# PKA firmware for SA LITE SoCs  
  SRCDIR += src/pkafw
  INCDIR += src/pkafw
endif
# Data Mode Protocol is common between LITE and non LITE SoCs  
  SRCS_DM_FILES    = sallddmcmdl.c sallddminit.c sallddm.c salldpka.c
  
# List all the protocol source files   
  SRCS_PROTO_FILES = $(SRCS_DM_FILES) $(SRCS_IPSEC_FILES) $(SRCS_SRTP_FILES) $(SRCS_AIRCIPHER_FILES)

# List all the Auth source files  
  SRCS_AUTH_FILES  = salldcmac.c salldhmac_md5.c salldhmac_sha1.c salldhmac_sha2.c 
  SRCS_AUTH_FILES += salldmd5.c  salldsha1.c  salldsha2.c    salldxcbc.c

# LITE library has SHA512 support for AM65XX and J721E
ifeq ($(SOC),$(filter $(SOC), am65xx j721e am64x))
  SRCS_AUTH_FILES += salldsha512.c salldhmac_sha512.c
endif
  
# List all the cipher source files
  SRCS_CIPHER_FILES = salldaes.c salldgcm.c

# non Lite SoCs also have counter and aes_f8
ifneq ($(SOC),$(filter $(SOC), k2g am65xx j721e am64x))
  SRCS_CIPHER_FILES += salldaes_ctr.c  salldaes_f8.c 
endif

# List the PKA Firmware files
ifeq ($(SOC),$(filter $(SOC), k2g am65xx j721e am64x))
  SRCS_PKAFW_FILES += firmware_eip29t2.c
endif
# Append them to the source files for the library  
  SRCS_COMMON     += $(SRCS_PROTO_FILES) $(SRCS_CIPHER_FILES) $(SRCS_AUTH_FILES) $(SRCS_PKAFW_FILES)



  
