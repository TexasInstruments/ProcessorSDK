#################################################################################
# Includes
#################################################################################
#
# All path and images configuration are defined in "config.mk"
#
#     Configuration can be done by either modifying variables in this file
#     .. or by changing the requisite variables in the command line environment
#
include config.mk

# We need platform.mk for device id definitions
include $(PDK_INSTALL_PATH)/ti/build/makerules/platform.mk

#################################################################################
# Path Configuration
#################################################################################
SBL_REPO_PATH := $(PDK_INSTALL_PATH)/ti/boot/sbl
MULTICORE_APPIMAGE_GEN_TOOL_PATH := $(SBL_REPO_PATH)/tools/multicoreImageGen/bin

ifeq ($(OS),Windows_NT)
  SBL_OUT2RPRC_GEN_TOOL_PATH := $(SBL_REPO_PATH)/tools/out2rprc/bin/out2rprc.exe
else
  SBL_OUT2RPRC_GEN_TOOL_PATH := mono --runtime=v4.0 $(SBL_REPO_PATH)/tools/out2rprc/bin/out2rprc.exe
endif

#################################################################################
# Helper Functions
#################################################################################
# Individual character helpers for using special characters in call functions
comma := ,
space := $(subst ,, )

# Interleave two lists into one list (space-separated)
zip = $(subst ;, $(space), $(join $1, $(addprefix ;, $2)))

# Split a comma-separated list into a space-separated list
split = $(subst $(comma), ,$1)

# Returns an element at a specified index in a comma-separated list
index = $(word $2,$(call split,$1))

# Returns a list of elements found at a specified index in a space-separated list
# of comma-separated lists. Any NULL elements will be returned as hyphens ("-")
column = $(foreach row, $1, $(or $(call index,$(row),$2),-))

# Expand a path starting with tilde to its proper absolute path
expand_path = $(patsubst ~/%,$(HOME)/%,$1)

# Returns an element of a space-separated list that contains a specified
# element of a comma-separated list.
getrow = $(foreach row, $1, $(if $(filter $2,$(call split,$(row))),$(row),))

# Same as "getrow" but resolves paths, because we will be searching the config
# variables specifically by binary name with path
getrow_abspath = $(foreach row, $1, $(if $(filter $2,\
	$(call expand_path,$(call split,$(row)))),$(row),))

#################################################################################
# Configuration Parsing Functions
#################################################################################
# Each of these functions searches through the list of configurations by image name
# and returns a particular element of that comma-separated configuration.
# Creating named functions for these index calls makes the targets below more readable
get_dev_id = $(call index,$(call getrow_abspath,$1,$2),1)
get_img_name = $(call index,$(call getrow_abspath,$1,$2),2)
get_load_addr = $(call index,$(call getrow_abspath,$1,$2),3)
get_entry_point = $(call index,$(call getrow_abspath,$1,$2),4)

# Each of these returns the above configuration information in a way that is
# formatted as a particular flag for LD. If the index does NOT exist in a
# config list, then the flag will be ignored and LD will never see it.
fmt_load_addr = $(if $(call get_load_addr,$1,$2),\
		       -section-start=.data=$(call get_load_addr,$1,$2),)
fmt_entry_point = $(if $(call get_entry_point,$1,$2),\
		       -e $(call get_entry_point,$1,$2),)

#################################################################################
# Input Images
#################################################################################
# List of image names based on the image configurations (in order)
IMAGES := $(call column,$(IMG_LIST),2)

# Unique list of all image extensions
EXTENSIONS := $(sort $(suffix $(IMAGES)))

# List of all RPRC file names which must be generated based on the specified images
RPRC_LIST := $(addsuffix .rprc, $(basename $(IMAGES)))

# List of the Device IDs in order of the specified image configurations
_DEVICE_IDS := $(call column,$(IMG_LIST),1)
DEVICE_IDS := $(foreach _devid,$(_DEVICE_IDS),$(SBL_CORE_ID_$(_devid)))

# List of the Device IDs interleaved with the RPRC list. This is useful for the
# multicore appimage creation tool
RPRC_LIST_WITH_DEVICE_IDS := $(call zip, $(DEVICE_IDS),$(RPRC_LIST))

#################################################################################
# Extensions Configuration
#################################################################################
# Each raw binary blob should have a pattern rule for ELF creation
RAW_PATTERNS = $(foreach _ext, $(RAW_EXTENSIONS), %$(_ext))

# Any extension not considered a raw binary blob will be treated as executable.
# Generate a list of all executable extensions (including .elf by default) and
# set up patterns so we may create pattern rules for RPRC creation
EXE_EXTENSIONS = .elf $(filter-out $(RAW_EXTENSIONS), $(EXTENSIONS))
EXE_EXTENSIONS := $(sort $(EXE_EXTENSIONS))
EXE_PATTERNS = $(foreach _ext, $(EXE_EXTENSIONS), %$(_ext))

# Add an exntensionless pattern-match for the configured file type
ifeq ($(EXTENSIONLESS_TYPE),exe)
	EXE_PATTERNS += %
else
	RAW_PATTERNS += %
endif

#################################################################################
# Output Image
#################################################################################
# Combined appimage output name/directory as configured in config.mk
APP_IMAGE = $(OUT_DIR)/$(OUT_IMG)

#################################################################################
# Target Templates
#################################################################################
# Template rule for creating ELF files
define ELF_RULE
%.elf: $1
	$(GCC_LINUX_ARM_PATH)/bin/$(GCC_ARCH64_BIN_PREFIX)-ld -b binary -A aarch64 --oformat elf64-littleaarch64 \
		$$(call fmt_load_addr,$$(IMG_LIST),$$^) \
		$$(call fmt_entry_point,$$(IMG_LIST),$$^) \
		$$^ -o $$@
endef

# Template rule for creating RPRC files
define RPRC_RULE
%.rprc: $1
	$$(SBL_OUT2RPRC_GEN_TOOL_PATH) $$^ $$@
endef

#################################################################################
# Targets
#################################################################################
all: $(APP_IMAGE)

# The combined appimage is built from a number of RPRC files, and depends on the
# existence of the output directory
.INTERMEDIATE: $(RPRC_LIST)
$(APP_IMAGE): $(RPRC_LIST) | $(OUT_DIR)
	$(MULTICORE_APPIMAGE_GEN_TOOL_PATH)/MulticoreImageGen LE 55 $(APP_IMAGE) \
		$(RPRC_LIST_WITH_DEVICE_IDS)

# Generate pattern rules such that an RPRC file will be created for each
# available executable file
$(foreach _pattern, $(EXE_PATTERNS), $(eval $(call RPRC_RULE, $(_pattern))))

# Generate pattern rules such that an ELF file will be created for each
# available raw binary file
$(foreach _pattern, $(RAW_PATTERNS), $(eval $(call ELF_RULE, $(_pattern))))

# Create the output directory
$(OUT_DIR):
ifeq ($(OS),Windows_NT)
	cmd /E:ON /C mkdir $(subst /,\,$@)
else
	mkdir -p $(OUT_DIR)
endif

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
