#
# Copyright (c) 2023, Texas Instruments Incorporated
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

# File: tsn_component.mk
#       This file is component include make file of tsn library.
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
#                             board and the compiled obj/lib has to be kept
#                             under <board> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no board dependent code and hence
#                             the obj/libs are not kept under <board> dir.
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
ifeq ($(tsn_component_make_include), )

tsn_SOCLIST         = j721e j7200 j721s2 j784s4 j742s2
tsn_BOARDLIST       = j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm
tsn_j721e_CORELIST  = mcu1_0 mcu2_0 mcu2_1
tsn_j7200_CORELIST  = mcu1_0 mcu2_0 mcu2_1
tsn_j721s2_CORELIST = mcu1_0 mcu2_0 mcu2_1
tsn_j784s4_CORELIST = mcu1_0 mcu2_0 mcu2_1
tsn_j742s2_CORELIST = mcu1_0 mcu2_0 mcu2_1

############################
# tsn lib package
# List of components included under tsn lib
# The components included here are built and will be part of tsn lib
############################
tsn_LIB_LIST =

############################
# tsn app lib package
# List of components included under enet app lib
# The components included here are built and will be part of enet app lib
############################
tsn_APP_LIB_LIST =

############################
# tsn examples
# List of examples under tsn
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
tsn_EXAMPLE_LIST =

#
# tsn_unibase library
#
export tsn_unibase_COMP_LIST = tsn_unibase
tsn_unibase_RELPATH = ti/transport/tsn/tsn-stack
tsn_unibase_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
export tsn_unibase_LIBNAME = tsn_unibase
export tsn_unibase_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export tsn_unibase_MAKEFILE = -f ../makefile_tsn_unibase
export tsn_unibase_BOARD_DEPENDENCY = no
export tsn_unibase_SOC_DEPENDENCY = no
export tsn_unibase_CORE_DEPENDENCY = no
tsn_unibase_PKG_LIST = tsn_unibase
tsn_unibase_INCLUDE = $(tsn_unibase_PATH) $(tsn_unibase_PATH)/tsn_unibase
export tsn_unibase_SOCLIST = $(tsn_SOCLIST)
export tsn_unibase_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += tsn_unibase

#
# tsn_combase library
#
export tsn_combase_COMP_LIST = tsn_combase
tsn_combase_RELPATH = ti/transport/tsn/tsn-stack
tsn_combase_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
export tsn_combase_LIBNAME = tsn_combase
export tsn_combase_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export tsn_combase_MAKEFILE = -f ../makefile_tsn_combase
export tsn_combase_BOARD_DEPENDENCY = no
export tsn_combase_SOC_DEPENDENCY = no
export tsn_combase_CORE_DEPENDENCY = no
tsn_combase_PKG_LIST = tsn_combase
tsn_combase_INCLUDE = $(tsn_combase_PATH) $(tsn_combase_PATH)/tsn_combase \
	$(tsn_combase_PATH)/tsn_combase/tilld/jacinto
export tsn_combase_SOCLIST = $(tsn_SOCLIST)
export tsn_combase_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += tsn_combase

#
# gptp2 library
#
export tsn_gptp_COMP_LIST = tsn_gptp
tsn_gptp_RELPATH = ti/transport/tsn/tsn-stack
tsn_gptp_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
export tsn_gptp_LIBNAME = tsn_gptp
export tsn_gptp_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export tsn_gptp_MAKEFILE = -f ../makefile_tsn_gptp
export tsn_gptp_BOARD_DEPENDENCY = no
export tsn_gptp_SOC_DEPENDENCY = no
export tsn_gptp_CORE_DEPENDENCY = no
tsn_gptp_PKG_LIST = tsn_gptp
tsn_gptp_INCLUDE = $(tsn_gptp_PATH) $(tsn_gptp_PATH)/tsn_gptp
export tsn_gptp_SOCLIST = $(tsn_SOCLIST)
export tsn_gptp_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += tsn_gptp

# The following 2 libraries, lldp library and tsn_yangemb library
# need not be built periodically. These makefiles and related code
# will not be packaged. Anytime there is a code change in these,
# developers will have to rebuild the .libs for these libraries
# and update it in the respective repos.
# Uncommenting these and building them will generate said .libs.

# #
# # lldp library
# #
# export tsn_lldp_COMP_LIST = tsn_lldp
# tsn_lldp_RELPATH = ti/transport/tsn/tsn-stack
# tsn_lldp_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
# export tsn_lldp_LIBNAME = tsn_lldp
# export tsn_lldp_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
# export tsn_lldp_MAKEFILE = -f ../tsn-stack/eval_src/makefile.tsn_lldp_freertos.jacinto.r5f.ti-arm-clang
# export tsn_lldp_BOARD_DEPENDENCY = no
# export tsn_lldp_SOC_DEPENDENCY = no
# export tsn_lldp_CORE_DEPENDENCY = no
# tsn_lldp_PKG_LIST = tsn_lldp
# tsn_lldp_INCLUDE = $(tsn_lldp_PATH)  $(tsn_lldp_PATH)/eval_src/tsn_lldp $(tsn_lldp_PATH)/eval_src/tsn_lldp/tilld $(tsn_lldp_PATH)/eval_src/tsn_lldp/lldpcli
# export tsn_lldp_SOCLIST = $(tsn_SOCLIST)
# export tsn_lldp_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
# tsn_LIB_LIST += tsn_lldp

# #
# # tsn_yangemb library
# #
# export tsn_yangemb_COMP_LIST = tsn_yangemb
# tsn_yangemb_RELPATH = ti/transport/tsn/tsn-stack
# tsn_yangemb_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
# export tsn_yangemb_LIBNAME = tsn_yangemb
# export tsn_yangemb_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
# export tsn_yangemb_MAKEFILE =  -f ../tsn-stack/eval_src/makefile.yangemb_freertos.jacinto.r5f.ti-arm-clang
# export tsn_yangemb_BOARD_DEPENDENCY = no
# export tsn_yangemb_SOC_DEPENDENCY = no
# export tsn_yangemb_CORE_DEPENDENCY = no
# tsn_yangemb_PKG_LIST = tsn_yangemb
# tsn_yangemb_INCLUDE = $(tsn_yangemb_PATH) $(tsn_yangemb_PATH)/eval_src/yangblock
# export tsn_yangemb_SOCLIST = $(tsn_SOCLIST)
# export tsn_yangemb_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
# tsn_LIB_LIST += tsn_yangemb

#
# tsn_uniconf library
#
export tsn_uniconf_COMP_LIST = tsn_uniconf
tsn_uniconf_RELPATH = ti/transport/tsn/tsn-stack
tsn_uniconf_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
export tsn_uniconf_LIBNAME = tsn_uniconf
export tsn_uniconf_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export tsn_uniconf_MAKEFILE =  -f ../makefile_tsn_uniconf
export tsn_uniconf_BOARD_DEPENDENCY = no
export tsn_uniconf_SOC_DEPENDENCY = no
export tsn_uniconf_CORE_DEPENDENCY = no
tsn_uniconf_PKG_LIST = tsn_uniconf
tsn_uniconf_INCLUDE = $(tsn_uniconf_PATH) $(tsn_uniconf_PATH)/tsn_uniconf
export tsn_uniconf_SOCLIST = $(tsn_SOCLIST)
export tsn_uniconf_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += tsn_uniconf

#
# tsn_netconf daemon/library
#
export tsn_netconf_COMP_LIST = tsn_netconf
tsn_netconf_RELPATH = ti/transport/tsn/tsn-stack
tsn_netconf_PATH = $(PDK_TSN_COMP_PATH)/tsn-stack
export tsn_netconf_LIBNAME = tsn_netconf
export tsn_netconf_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export tsn_netconf_MAKEFILE =  -f ../makefile_tsn_netconf
export tsn_netconf_BOARD_DEPENDENCY = no
export tsn_netconf_SOC_DEPENDENCY = no
export tsn_netconf_CORE_DEPENDENCY = no
tsn_netconf_PKG_LIST = tsn_netconf
tsn_netconf_INCLUDE = $(tsn_netconf_PATH) \
	$(tsn_netconf_PATH)/tsn_netconf/src/common \
	$(tsn_netconf_PATH)/tsn_netconf/src/daemon \
	$(tsn_netconf_PATH)/tsn_netconf/src/platform/ti-frtos
export tsn_netconf_SOCLIST = $(tsn_SOCLIST)
export tsn_netconf_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += tsn_netconf

#
# pugixml library
#
export pugixml_COMP_LIST = pugixml
pugixml_RELPATH = ti/transport/tsn/utils/pugixml
pugixml_PATH = $(PDK_TSN_COMP_PATH)/utils/pugixml
export pugixml_LIBNAME = pugixml
export pugixml_LIBPATH = $(PDK_TSN_COMP_PATH)/lib
export pugixml_MAKEFILE =  -f ../../makefile_pugixml
export pugixml_BOARD_DEPENDENCY = no
export pugixml_SOC_DEPENDENCY = no
export pugixml_CORE_DEPENDENCY = no
pugixml_PKG_LIST = pugixml
pugixml_INCLUDE = $(pugixml_PATH) $(pugixml_PATH)/src
export pugixml_SOCLIST = $(tsn_SOCLIST)
export pugixml_$(SOC)_CORELIST = $(tsn_$(SOC)_CORELIST)
tsn_LIB_LIST += pugixml

endif
# Common CFLAGS for TSN stack components
TSN_CFLAGS =
