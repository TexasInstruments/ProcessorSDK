#*******************************************************************************
#* FILE PURPOSE: Top level makefile for Creating Component Libraries for ARM
#* architectures
#*******************************************************************************
#* FILE NAME: makefile
#*
#* DESCRIPTION: Defines Compiler tools paths, libraries , Build Options 
#*
#*
#*******************************************************************************
#*
# (Mandatory) Specify where various tools are installed.

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#Default Do not use the shared object libraries for the test/example applications
export USEDYNAMIC_LIB ?= "no"

#export SOC ?= AM572x
export SOC
export BOOTMODE
export OPPMODE
export SECUREMODE

export LLD_NAME=sbl

# ROOT Directory
export ROOTDIR := ../..

#Check to identify between windows or linux environment
ifeq ($(OS), )
  OS := linux
endif

# INCLUDE Directory
export INCDIR := ../..;$(PDK_INSTALL_PATH);$(ROOTDIR);$(SBL_INC_DIR)

# Common Macros used in make

ifndef RM
export RM = rm -f
endif

ifndef CP
export CP = cp -p
endif

export MKDIR = mkdir -p

ifndef RMDIR
export RMDIR = rm -rf
endif

ifndef SED
export SED = sed
endif

ifndef MAKE
export MAKE = make
endif

# PHONY Targets
.PHONY: all clean spi_flashwriter spi_flashwriter_clean flashwriter flashwriter_clean mmcsd_flashwriter mmcsd_flashwriter_clean example example_clean tools sbl_lib sbl_lib_clean all_sbl_images all_sbl_images_clean

# all rule
all:
	@$(MAKE) -f ./board/$(BOARD)/build/makefile all

# This target is used to build all configurations for a particular SBL_PLATFORM
all_sbl_images:
ifeq ($(SBL_PLATFORM),$(filter $(SBL_PLATFORM), j721e j7200 j721s2 j784s4 j742s2))
	# All keystone 3
	$(MAKE) -C $(PDK_INSTALL_PATH)/ti/boot/sbl/build all
	$(MAKE) -C $(PDK_INSTALL_PATH)/ti/boot/sbl/build all
endif


spi_flashwriter:
	@$(MAKE) -f ./tools/flashWriter/spi/src/makefile $@

flashwriter:
	@$(MAKE) -f ./tools/flashWriter/qspi/src/makefile $@

mmcsd_flashwriter:
	@$(MAKE) -f ./tools/flashWriter/mmcsd/src/makefile $@

example:
	@$(MAKE) -f ./example/mpuMulticoreApp/makefile $@
	@$(MAKE) -f ./example/dsp1MulticoreApp/makefile $@
	@$(MAKE) -f ./example/ipu1MulticoreApp/makefile $@

tools:
	@$(MAKE) -C tools/btoccs
	@$(MAKE) -C tools/byteswap
	@$(MAKE) -C tools/ccsutil
	@$(MAKE) -C tools/tiImageGen

sbl_lib:
	@$(MAKE) -f ./src/sbl_eve/sbl_lib/src/makefile $@


# Rule to clean sbl binary
clean:
	@$(MAKE) -f ./board/$(BOARD)/build/makefile $@
	@$(RMDIR) $(PDK_INSTALL_PATH)/ti/boot/$(LLD_NAME)/binary/$(BOARD)

spi_flashwriter_clean:
	@$(MAKE) -f ./tools/flashWriter/spi/src/makefile $@

flashwriter_clean:
	@$(MAKE) -f ./tools/flashWriter/qspi/src/makefile $@
	@$(RMDIR) $(PDK_INSTALL_PATH)/ti/boot/sbl/tools/flashWriter/qspi/bin/$(BOARD)

mmcsd_flashwriter_clean:
	@$(MAKE) -f ./tools/flashWriter/mmcsd/src/makefile $@

example_clean:
	@$(MAKE) -f ./example/mpuMulticoreApp/makefile $@
	@$(MAKE) -f ./example/dsp1MulticoreApp/makefile $@
	@$(MAKE) -f ./example/ipu1MulticoreApp/makefile $@
	@$(RMDIR) $(PDK_INSTALL_PATH)/ti/boot/sbl/binary/sbl_multicore_app/$(BOARD)

sbl_lib_clean:
	@$(MAKE) -f ./src/sbl_eve/sbl_lib/src/makefile $@

# This target is used to clean all images for a particular SBL_PLATFORM
all_sbl_images_clean:
ifeq ($(SBL_PLATFORM),$(filter $(SBL_PLATFORM), j721e j7200 j721s2 j784s4 j742s2))
	# All keystone3
	$(MAKE) -C $(PDK_INSTALL_PATH)/ti/boot/sbl/build clean
	$(MAKE) -C $(PDK_INSTALL_PATH)/ti/boot/sbl/build clean
endif
