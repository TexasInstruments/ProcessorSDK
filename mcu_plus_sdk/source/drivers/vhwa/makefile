#
# Copyright (c) 2018, Texas Instruments Incorporated
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

export SDK_INSTALL_PATH  ?= $(abspath ../../../../../)
export VHWA_INSTALL_PATH ?= $(abspath ../)
export VHWA_COMP_PATH ?= $(VHWA_INSTALL_PATH)/vhwa
export PDK_INSTALL_PATH ?= $(SDK_INSTALL_PATH)/pdk/packages

vpac_example = msc viss ldc nf
dmpac_example = sde dof


COMP = drvvhwa
lib_$(COMP)_BUILD_DEPENDENCY = soc

$(COMP)_DOXYGEN_SUPPORT = yes

include $(VHWA_INSTALL_PATH)/vhwa/vhwa_component.mk

.PHONY: allclean help $(vhwa_PKG_LIST_ALL) vpac_all dmpac_all vpac_all_freertos dmpac_all_freertos vpac_all_safertos dmpac_all_safertos


allclean:
	$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile destroot_clean
	$(RM) -rf lib
	
help:
	@echo ------------------------------------------------------
	@echo \# VHWA make help
	@echo ------------------------------------------------------
	@echo make -s [MANDATORY MAKE VARIABLES] Note: use gmake for windows
	@echo
	@echo "Supported targets:"
	@echo "------------------"
	@echo "allclean             : Cleans all libraries and examples for the all CORES and BOARDS"
	@echo "vpac_all             : Builds all baremetal vpac examples for the selected CORE and BOARD"
	@echo "dmpac_all            : Builds all baremetal dmpac examples for the selected CORE and BOARD"
	@echo "vpac_all_freertos    : Builds all vpac freertos examples for the selected CORE and BOARD"
	@echo "dmpac_all_freertos   : Builds all dmpac freertos examples for the selected CORE and BOARD"
	@echo "vpac_all_safertos    : Builds all vpac safertos examples for the selected CORE and BOARD"
	@echo "dmpac_all_safertos   : Builds all dmpac safertos examples for the selected CORE and BOARD"
	@echo "vhwa                 : Builds vhwa library for the selected CORE and BOARD"
	@echo "vhwa_msc_negative_testapp_freertos	: Builds MSC negative test examples the selected CORE and BOARD"
	@echo "VHWA_NEGATIVE_TEST	: true: Build negative test examples, false: Throw an error"
	@echo "VHWA_NEGATIVE_FOCO_TEST	: true: Build negative test examples for FOCO WDTIMER Error in DMPAC, false: Throw an error"
	@echo ""
	@echo "<VHWA_example_app> : Builds vhwa examples:"
	@echo "[$(vhwa_EXAMPLE_LIST)]"
	@echo ""
	@echo "Mandatory make variables:"
	@echo "------------------------"
	@echo "SOC=[$(vhwa_default_SOCLIST)]"
	@echo "    Default: j721e"
	@echo "BOARD=[$(vhwa_default_BOARDLIST)]"
	@echo "    Default: j721e_evm"
	@echo "CORE=[mcu2_0 mcu2_1 mcu4_0]"
	@echo "    Default: mcu1_0"
	@echo "Note:"
	@echo "	#Flexconnect board support on j721e_evm"
	@echo "	-----------"
	@echo "	#Core support"
	@echo "		J721E CORE support[$(vhwa_default_j721e_CORELIST)]"
	@echo "    			Default: mcu1_0"
	@echo "		J721E VPAC CORE support[$(vhwa_vpac_j721e_CORELIST)]"
	@echo "		J721E DMPAC CORE support[$(vhwa_dmpac_j721e_CORELIST)]"
	@echo "		-----------"
	@echo "		J721S2 CORE support[$(vhwa_default_j721s2_CORELIST)]"
	@echo "    			Default: mcu1_0"
	@echo "		J721S2 VPAC CORE support[$(vhwa_vpac_j721s2_CORELIST)]"
	@echo "		J721S2 DMPAC CORE support[$(vhwa_dmpac_j721s2_CORELIST)]"
	@echo "		-----------"
	@echo "		J784S4 CORE support[$(vhwa_default_j784s4_CORELIST)]"
	@echo "    			Default: mcu1_0"
	@echo "		J784S4 VPAC CORE support[$(vhwa_vpac_j784s4_CORELIST)]"
	@echo "		J784S4 DMPAC CORE support[$(vhwa_dmpac_j784s4_CORELIST)]"
	@echo "	-----------"
	@echo "		J742S2 CORE support[$(vhwa_default_j742s2_CORELIST)]"
	@echo "    			Default: mcu1_0"
	@echo "		J742S2 VPAC CORE support[$(vhwa_vpac_j742s2_CORELIST)]"
	@echo "		J742S2 DMPAC CORE support[$(vhwa_dmpac_j742s2_CORELIST)]"
	@echo "		-----------"
	@echo "	#For safertos build, download the r5f package for the SOC at SDK_INSTALL_PATH and run the safertos setup script:"
	@echo "		./safertos_setup.sh --soc=SOC --isa=r5f --SOC_r5f_path=<path to SOC r5f package>"
	@echo "Optional make variables:"
	@echo "------------------------"
	@echo "BUILD_PROFILE=[release debug]"
	@echo "    Default: release"
	@echo "Sample VHWA example build"
	@echo "    make vhwa_msc_baremetal_testapp BOARD=j721s2_evm SOC=j721s2 CORE=mcu2_0 BUILD_PROFILE=debug"
	@echo "Sample VHWA negative test example build"
	@echo "    make vhwa_msc_negative_testapp_freertos BOARD=j742s2_evm SOC=j742s2 CORE=mcu2_0 VHWA_NEGATIVE_TEST=true"
	@echo "Sample VHWA negative foco test example build"
	@echo "    make vhwa_dof_negative_testapp_freertos BOARD=j742s2_evm SOC=j742s2 CORE=mcu2_0 VHWA_NEGATIVE_FOCO_TEST=true"
	@echo "Additional Info"
	@echo "    SDK_INSTALL_PATH = work_area/platform"
	@echo "    PDK_INSTALL_PATH = work_area/platform/pdk/packages"
	@echo "    VHWA_INSTALL_PATH = work_area/platform/pdk/packages/ti/drv"


#All VHWA libs and tests
$(vhwa_PKG_LIST_ALL):
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(MAKE) -C $($@_PATH) $($@_MAKEFILE)

vpac_all:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(vpac_example),\
	$(MAKE) -C $(vhwa_$(example)_baremetal_testapp_PATH) $(vhwa_$(example)_baremetal_testapp_MAKEFILE);)

dmpac_all:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(dmpac_example),\
	$(MAKE) -C $(vhwa_$(example)_baremetal_testapp_PATH) $(vhwa_$(example)_baremetal_testapp_MAKEFILE);)

vpac_all_freertos:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(vpac_example),\
	$(MAKE) -C $(vhwa_$(example)_testapp_freertos_PATH) $(vhwa_$(example)_testapp_freertos_MAKEFILE);)

dmpac_all_freertos:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(dmpac_example),\
	$(MAKE) -C $(vhwa_$(example)_testapp_freertos_PATH) $(vhwa_$(example)_testapp_freertos_MAKEFILE);)

vpac_all_safertos:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(vpac_example),\
	$(MAKE) -C $(vhwa_$(example)_testapp_safertos_PATH) $(vhwa_$(example)_testapp_safertos_MAKEFILE);)

dmpac_all_safertos:
	$(MAKE) -C $(vhwa_PATH) $(vhwa_MAKEFILE)
	$(foreach example, $(dmpac_example),\
	$(MAKE) -C $(vhwa_$(example)_testapp_safertos_PATH) $(vhwa_$(example)_testapp_safertos_MAKEFILE);)

