#
# Copyright (c) 2021, Texas Instruments Incorporated
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

# File: keywriter_component.mk
#       This file is component include make file of SBL.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             platform and the compiled obj/lib has to be kept
#                             under <platform> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no platform dependent code and hence
#                             the obj/libs are not kept under <platform> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(keywriter_component_make_include), )
keywriter_BOARDLIST = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
keywriter_combined_BOARDLIST = j7200_evm j721s2_evm j784s4_evm j742s2_evm
keywriter_SOCLIST = j721e j7200 j721s2 j784s4 j742s2
keywriter_j721e_CORELIST := mcu1_0
keywriter_j7200_CORELIST := mcu1_0

############################
# keywriter package
# List of components included under keywriter
# The components included here are built and will be part of keywriter
############################

############################
# keywriter example
# List of examples under keywriter (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################

# KEYWRITER Image
export keywriter_img_COMP_LIST        = keywriter_img
export keywriter_img_RELPATH          = ti/boot/keywriter/build
export keywriter_img_CUSTOM_BINPATH   = $(PDK_KEYWR_COMP_PATH)/binary/$(SOC)
export keywriter_img_PATH             = $(PDK_KEYWR_COMP_PATH)/build
export keywriter_img_MAKEFILE         = -f$(PDK_KEYWR_COMP_PATH)/build/keywriter.mk
export keywriter_img_KEYWR_CERT_KEY   = $(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem
export keywriter_img_BOARD_DEPENDENCY = yes
export keywriter_img_SOC_DEPENDENCY   = yes
export keywriter_img_CORE_DEPENDENCY  = yes
export keywriter_img_PKG_LIST         = sbl
export keywriter_img_INCLUDE          = $(keywriter_img_PATH)
export keywriter_img_BOARDLIST        = $(keywriter_BOARDLIST)
export keywriter_img_$(SOC)_CORELIST  = mcu1_0
export keywriter_img_KEYWR_IMAGEGEN   = yes

keywriter_EXAMPLE_LIST                = keywriter_img

export keywriter_img_combined_COMP_LIST        = keywriter_img_combined
export keywriter_img_combined_RELPATH          = ti/boot/keywriter/build
export keywriter_img_combined_CUSTOM_BINPATH   = $(PDK_KEYWR_COMP_PATH)/binary/$(SOC)
export keywriter_img_combined_PATH             = $(PDK_KEYWR_COMP_PATH)/build
export keywriter_img_combined_MAKEFILE         = -f$(PDK_KEYWR_COMP_PATH)/build/keywriter.mk SBL_IMAGE_TYPE=combined BUILD_HS_FS=yes
export keywriter_img_combined_KEYWR_CERT_KEY   = $(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem
export keywriter_img_combined_BOARD_DEPENDENCY = yes
export keywriter_img_combined_SOC_DEPENDENCY   = yes
export keywriter_img_combined_CORE_DEPENDENCY  = yes
export keywriter_img_combined_PKG_LIST         = sbl
export keywriter_img_combined_INCLUDE          = $(keywriter_img_combined_PATH)
export keywriter_img_combined_BOARDLIST        = $(keywriter_combined_BOARDLIST)
export keywriter_img_combined_$(SOC)_CORELIST  = mcu1_0
export keywriter_img_combined_KEYWR_IMAGEGEN   = yes

keywriter_EXAMPLE_LIST                        += keywriter_img_combined

# KEYWRITER Test image generation
export keywriter_UTILS_LIST = keywriter_cfg_test_gen
export keywriter_cfg_test_gen_COMP_LIST = keywriter_cfg_test_gen
export keywriter_cfg_test_gen_RELPATH = $(PDK_KEYWR_COMP_PATH)/scripts
export keywriter_cfg_test_gen_PATH = $(PDK_KEYWR_COMP_PATH)
export keywriter_cfg_test_gen_MAKEFILE = -f scripts/keywriter_cfg_test_gen.mk
export keywriter_cfg_test_gen_BOARD_DEPENDENCY = yes
export keywriter_cfg_test_gen_CORE_DEPENDENCY = yes
export keywriter_cfg_test_gen_BOARDLIST = $(keywriter_BOARDLIST)
export keywriter_cfg_test_gen_$(SOC)_CORELIST  = mcu1_0
export keywriter_cfg_test_gen_CORELIST = $(keywriter_SOCLIST)
export keywriter_cfg_test_gen_LIBNAME = keywriter_cfg_test_gen

###########END BOOT PERF KNOBS#############

# KEYWRITER not supported for any profile
# other than release
ifneq ($(BUILD_PROFILE), release)
keywriter_LIB_LIST =
keywriter_EXAMPLE_LIST =
KEYWR_CFLAGS =
endif

export KEYWR_COMBINED_TIFS_BRDCFG=$(PDK_KEYWR_COMP_PATH)/tifs_bin/$(SOC)/combined-tifs-cfg.bin
export KEYWR_SYSFW_PATH=$(PDK_KEYWR_COMP_PATH)/tifs_bin/$(SOC)/ti-fs-firmware-hs-fs-enc.bin
export KEYWR_SYSFW_INNER_CERT=$(PDK_KEYWR_COMP_PATH)/tifs_bin/$(SOC)/ti-fs-firmware-hs-fs-cert.bin
export keywriter_LIB_LIST
export keywriter_EXAMPLE_LIST
export KEYWR_CFLAGS

keywriter_component_make_include := 1
endif
