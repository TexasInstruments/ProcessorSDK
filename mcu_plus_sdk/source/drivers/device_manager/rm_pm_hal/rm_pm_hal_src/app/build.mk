# Copyright (C) 2020-2021 Texas Instruments Incorporated - http://www.ti.com/
# ALL RIGHTS RESERVED

obj-$(CONFIG_LPM_DM_STUB) += dmstub/

ifdef CONFIG_LPM_DM_STUB_STANDALONE

ifneq ($(CONFIG_BINARY_POST_FIX),"")
TARGET_POSTFIX = -$(shell echo $(CONFIG_BINARY_POST_FIX))
else
TARGET_POSTFIX =
endif

TARGET_PREFIX = ti-$(shell echo $(CONFIG_BIN_CONTENT))-firmware-$(shell echo $(CONFIG_SOC_STRING))-$(shell echo $(CONFIG_SOC_TYPE_STRING))$(TARGET_POSTFIX)

TARGET = $(TARGET_PREFIX).elf
TARGET_STRIPPED = $(TARGET_PREFIX).stripped.elf
TARGET_BIN = $(TARGET_PREFIX).bin
TARGET_VBIN = $(TARGET_PREFIX).vectors.bin

cppflags-y += -I$(srcroot)/include

target-y += $(objtree)/$(TARGET)
target-y += $(objtree)/$(TARGET_STRIPPED)
target-y += $(objtree)/$(TARGET_BIN)
ifneq ($(CONFIG_ROM_VECTORS),)
target-y += $(objtree)/$(TARGET_VBIN)
endif

ifndef CONFIG_SOC_LD_SCRIPT
# Note, make commands that are run with an old .config without a
# CONFIG_SOC_LD_SCRIPT will fail (even make defconfig). Adding a default
# fixes the breakage and makes transition easier.
LD_SCRIPT = $(srctree)/app/linkercmd/arm.ld
else
# Strings within Kconfig are enclosed in quotes. Remove the quotes.
LD_SCRIPT = $(srctree)/app/linkercmd/$(patsubst "%",%,$(CONFIG_SOC_LD_SCRIPT))
endif

extra-y += $(LD_SCRIPT) $(objtree)/secure.o
clean_scripts += $(LD_SCRIPT)

BIN_SCRIPT = $(srctree)/app/linkercmd/arm_bin.cmd
clean_scripts += $(BIN_SCRIPT)
VECTOR_BIN_SCRIPT = $(srctree)/app/linkercmd/arm_vec_bin.cmd
clean_scripts += $(VECTOR_BIN_SCRIPT)


OUTPUT_LIBS += $(TOOLCHAIN_PATH_M3)/lib/libc.a

# Collect all the kernel/secure code into a single object file so it can be
# easily referenced within the LD script.
$(objtree)/secure.o: $(objtree)/built-in.o $(OUTPUT_LIBS) FORCE
	$(call make,rule_ld_o_o)

$(objtree)/$(TARGET): $(objtree)/secure.o $(OUTPUT_OTHER_LIBS) $(LD_SCRIPT) FORCE
	$(call make,rule_armcl_elf_o)

$(objtree)/$(TARGET_STRIPPED): $(objtree)/$(TARGET)
	$(call make,rule_strip_elf_elf)

$(objtree)/$(TARGET_BIN): $(objtree)/$(TARGET_STRIPPED) $(BIN_SCRIPT)
	$(call make,rule_bin_elf)

$(objtree)/$(TARGET_VBIN): $(objtree)/$(TARGET_STRIPPED) $(VECTOR_BIN_SCRIPT)
	$(call make,rule_bin_elf)
endif