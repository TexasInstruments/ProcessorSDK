# Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
#
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Macro definitions referenced below
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

# Soc based on board
ifeq ($(BOARD), idkAM571x)
SOC_NAME = am571x
SOC = AM571x
endif

ifeq ($(BOARD), idkAM572x)
SOC_NAME = am572x
SOC = AM572x
endif

ifeq ($(BOARD), idkAM574x)
SOC_NAME = am574x
SOC = AM574x
endif

ifeq ($(BOARD), $(filter $(BOARD), evmK2G iceK2G))
SOC_NAME = k2g
SOC = K2G
endif

ifeq ($(BOARD), idkAM437x)
SOC_NAME = am437x
SOC = AM437x
endif

ifeq ($(BOARD), $(filter $(BOARD), evmAM335x icev2AM335x iceAMIC110))
SOC_NAME = am335x
SOC = AM335x
endif

export SOC

FLASH_SRC_DIR ?= $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/src
FLASH_PGM_BINDIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/bin/$(BOARD)
FLASH_PGM_OBJDIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/obj/$(BOARD)
ARMV7LIBDIR ?= ./lib
BINFLAGS = -O binary

#Cross tools
# Support backwards compatibility with KeyStone1 approach
CC = $(TOOLCHAIN_PATH_A15)/bin/$(CROSS_TOOL_PRFX)gcc
AC = $(TOOLCHAIN_PATH_A15)/bin/$(CROSS_TOOL_PRFX)as
AR = $(TOOLCHAIN_PATH_A15)/bin/$(CROSS_TOOL_PRFX)ar
LD = $(TOOLCHAIN_PATH_A15)/bin/$(CROSS_TOOL_PRFX)gcc
BIN = $(TOOLCHAIN_PATH_A15)/bin/$(CROSS_TOOL_PRFX)objcopy

# INCLUDE Directories
CSL_DIR = $(PDK_INSTALL_PATH)/ti/csl
BOARD_DIR = $(PDK_INSTALL_PATH)/ti/board

ifeq ($(SOC), $(filter $(SOC), AM335x AM437x))
STARTERWARE_DIR = $(PDK_INSTALL_PATH)/ti/starterware
else
SBL_SRCDIR = $(PDK_INSTALL_PATH)/ti/boot/sbl
endif

INCDIR := $(CSL_DIR);$(PDK_INSTALL_PATH);$(BOARD_DIR);$(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc;$(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc/$(SOC_NAME);$(FLASH_SRC_DIR)/qspi;$(PDK_INSTALL_PATH)/ti/board/src/idkAM572x/device;$(PDK_INSTALL_PATH)/ti/drv/uart;$(PDK_INSTALL_PATH)/ti/csl/src/ip/qspi/V1;$(FLASH_SRC_DIR)/spi;$(FLASH_SRC_DIR)/ospi;$(FLASH_SRC_DIR)/emmc;$(PDK_INSTALL_PATH)/ti/starterware/bootloader/src/am335x;$(PDK_INSTALL_PATH)/ti/starterware/include;$(PDK_INSTALL_PATH)/ti/starterware/include/hw;$(PDK_INSTALL_PATH)/ti/starterware/include/am335x;$(PDK_INSTALL_PATH)/ti/starterware/include/utils;$(PDK_INSTALL_PATH)/ti/starterware/bootloader/include;$(PDK_INSTALL_PATH)/ti/starterware/board;$(STARTERWARE_DIR);$(PDK_INSTALL_PATH)/ti/starterware/dal;$(PDK_INSTALL_PATH)/ti/starterware/utils;$(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/board/$(BOARD)/include;$(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/include;$(PDK_INSTALL_PATH)/ti/board/diag/common/$(SOC)

# Libraries
ifeq ($(SOC), K2G)
	WRTR_SOC = k2g
	WRTR_SOC_DIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc/$(SOC_NAME)
	WRTR_LINKER = linker.cmd
	CSL_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a15/release/ti.csl.aa15fg"
	CSL_INIT_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a15/release/ti.csl.init.aa15fg"
	OSAL_LIB = "$(PDK_INSTALL_PATH)/ti/osal/lib/nonos/$(SOC_NAME)/a15/release/ti.osal.aa15fg"
	QSPI_LIB = "$(PDK_INSTALL_PATH)/ti/drv/spi/lib/$(SOC_NAME)/a15/release/ti.drv.spi.aa15fg"
	MMCSD_LIB = "$(PDK_INSTALL_PATH)/ti/drv/mmcsd/lib/$(SOC_NAME)/a15/release/ti.drv.mmcsd.aa15fg"
	GPIO_LIB = "$(PDK_INSTALL_PATH)/ti/drv/gpio/lib/$(SOC_NAME)/a15/release/ti.drv.gpio.aa15fg"
	BOARD_LIB = "$(PDK_INSTALL_PATH)/ti/board/lib/$(BOARD)/a15/release/ti.board.aa15fg"
	I2C_LIB = "$(PDK_INSTALL_PATH)/ti/drv/i2c/lib/a15/release/ti.drv.i2c.aa15fg"
	UART_LIB = "$(PDK_INSTALL_PATH)/ti/drv/uart/lib/$(SOC_NAME)/a15/release/ti.drv.uart.aa15fg"
	PERIPHERAL_LIB = $(UART_LIB) $(I2C_LIB) $(QSPI_LIB) $(CSL_LIB) $(CSL_INIT_LIB) $(OSAL_LIB) $(BOARD_LIB) $(MMCSD_LIB) $(GPIO_LIB)
endif

ifeq ($(SOC), $(filter $(SOC), AM571x AM572x AM574x))
	WRTR_SOC = $(SOC_NAME)
	WRTR_SOC_DIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc/am57xx
	WRTR_LINKER = linker.cmd
	BOARD_LIB = "$(PDK_INSTALL_PATH)/ti/board/lib/$(BOARD)/a15/release/ti.board.aa15fg"
	CSL_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a15/release/ti.csl.aa15fg"
	CSL_INIT_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a15/release/ti.csl.init.aa15fg"
	OSAL_LIB = "$(PDK_INSTALL_PATH)/ti/osal/lib/nonos/$(SOC_NAME)/a15/release/ti.osal.aa15fg"
	QSPI_LIB = "$(PDK_INSTALL_PATH)/ti/drv/spi/lib/$(SOC_NAME)/a15/release/ti.drv.spi.aa15fg"
	I2C_LIB = "$(PDK_INSTALL_PATH)/ti/drv/i2c/lib/$(SOC_NAME)/a15/release/ti.drv.i2c.aa15fg"
	UART_LIB = "$(PDK_INSTALL_PATH)/ti/drv/uart/lib/$(SOC_NAME)/a15/release/ti.drv.uart.aa15fg"
	PERIPHERAL_LIB = $(UART_LIB) $(QSPI_LIB) $(I2C_LIB) $(MMCSD_LIB) $(CSL_LIB) $(CSL_INIT_LIB) $(OSAL_LIB) $(BOARD_LIB)
endif

ifeq ($(SOC), AM437x)
	WRTR_SOC = am437x
	WRTR_SOC_DIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc/$(SOC_NAME)
	WRTR_LINKER = linker.cmd
	BOARD_LIB = "$(PDK_INSTALL_PATH)/ti/board/lib/$(BOARD)/a9/release/ti.board.aa9fg"
	CSL_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a9/release/ti.csl.aa9fg"
	OSAL_LIB = "$(PDK_INSTALL_PATH)/ti/osal/lib/nonos/$(SOC_NAME)/a9/release/ti.osal.aa9fg"
	QSPI_LIB = "$(PDK_INSTALL_PATH)/ti/drv/spi/lib/$(SOC_NAME)/a9/release/ti.drv.spi.aa9fg"
	UART_LIB = "$(PDK_INSTALL_PATH)/ti/drv/uart/lib/$(SOC_NAME)/a9/release/ti.drv.uart.aa9fg"
	PERIPHERAL_LIB = $(UART_LIB) $(QSPI_LIB) $(MMCSD_LIB) $(CSL_LIB) $(OSAL_LIB) $(BOARD_LIB)
endif

ifeq ($(SOC), AM335x)
	WRTR_SOC = am335x
	WRTR_SOC_DIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/soc/$(SOC_NAME)
	WRTR_BOARD_DIR = $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/board/src
	WRTR_BOARD_DIR += $(PDK_INSTALL_PATH)/ti/board/utils/uniflash/target/board/$(BOARD)
	WRTR_LINKER = linker.cmd
	BOARD_LIB = "$(PDK_INSTALL_PATH)/ti/board/lib/$(BOARD)/a8/release/ti.board.aa8fg"
	CSL_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/$(SOC_NAME)/a8/release/ti.csl.aa8fg"
	OSAL_LIB = "$(PDK_INSTALL_PATH)/ti/osal/lib/nonos/$(SOC_NAME)/a8/release/ti.osal.aa8fg"
	SPI_LIB = "$(PDK_INSTALL_PATH)/ti/drv/spi/lib/$(SOC_NAME)/a8/release/ti.drv.spi.aa8fg"
	UART_LIB = "$(PDK_INSTALL_PATH)/ti/drv/uart/lib/$(SOC_NAME)/a8/release/ti.drv.uart.aa8fg"
	I2C_LIB = "$(PDK_INSTALL_PATH)/ti/drv/i2c/lib/$(SOC_NAME)/a8/release/ti.drv.i2c.aa8fg"
	PERIPHERAL_LIB = $(UART_LIB) $(SPI_LIB) $(CSL_LIB) $(OSAL_LIB) $(BOARD_LIB) $(I2C_LIB)
endif

LIBDIR :=

# Compiler options
INTERNALDEFS += -g -gdwarf-3 -gstrict-dwarf -Wall $(DEBUG_FLAG) -D__ARMv7 -DSOC_$(SOC) -D$(BOARD) -mtune=cortex-a15 -march=armv7-a -marm -mfloat-abi=hard -mfpu=neon
# Linker options
INTERNALLINKDEFS = -mfloat-abi=hard -Wl,--undefined,__aeabi_uidiv -Wl,--undefined,__aeabi_idiv --entry Entry -nostartfiles -static -Wl,--gc-sections -Wl,-T $(WRTR_SOC_DIR)/$(WRTR_LINKER) -Wl,--start-group -lgcc -lc -lrdimon $(BOARD_LIB) $(PERIPHERAL_LIB) -Wl,--end-group $(LDFLAGS)

OBJEXT = o
ASMOBJEXT = ao

# Executable using device independent library and device object file
EXE=uart_$(BOARD)_flash_programmer.out
BIN_NAME=uart_$(BOARD)_flash_programmer.bin

ifeq ($(OS),Windows_NT)
export TIIMAGE=tiimage.exe
else
export TIIMAGE=tiimage.out
endif

SRCDIR = $(FLASH_SRC_DIR)/

INCS = -I. -I$(strip $(subst ;, -I,$(INCDIR), -I,$(INCDIR)))

VPATH=$(SRCDIR):$(PDK_INSTALL_PATH)/ti/drv/uart/soc/$(WRTR_SOC):$(PDK_INSTALL_PATH)/ti/drv/spi/soc/$(WRTR_SOC):$(WRTR_SOC_DIR):$(WRTR_BOARD_DIR):$(FLASH_SRC_DIR)/qspi:$(FLASH_SRC_DIR)/ospi:$(PDK_INSTALL_PATH)/ti/drv/uart:$(FLASH_SRC_DIR)/spi:$(FLASH_SRC_DIR)/emmc:$(PDK_INSTALL_PATH)/ti/starterware/bootloader/src/am335x:$(PDK_INSTALL_PATH)/ti/starterware/include:$(PDK_INSTALL_PATH)/ti/starterware/include/hw:$(PDK_INSTALL_PATH)/ti/starterware/include/am335x:$(PDK_INSTALL_PATH)/ti/starterware/include/utils:$(PDK_INSTALL_PATH)/ti/starterware/bootloader/include:$(PDK_INSTALL_PATH)/ti/starterware/board:$(STARTERWARE_DIR):$(PDK_INSTALL_PATH)/ti/starterware/bootloader/src:$(PDK_INSTALL_PATH)/ti/starterware/examples/gpio:$(PDK_INSTALL_PATH)/ti/starterware/utils:$(STARTERWARE_DIR)/include:$(STARTERWARE_DIR)/board:$(STARTERWARE_DIR)/soc/am335x:$(STARTERWARE_DIR)/soc/armv7a/gcc:$(PDK_INSTALL_PATH)/ti/board/diag/common/$(SOC)

#List the Source Files
SRC_C = \
	xmodem.c \
	uart_main.c

ifeq ($(SOC), K2G)
SRC_DRV = a15.c
endif

ifeq ($(BOARD), $(filter $(BOARD), idkAM571x idkAM572x idkAM574x idkAM437x iceK2G evmK2G))
SRC_DRV += qspi.c
endif

ifeq ($(BOARD), $(filter $(BOARD), evmAM335x iceAMIC110 icev2AM335x))
SRC_DRV += spi.c
endif

SRC_DRV += \
    soc.c

ifeq ($(SOC), AM335x)
SRC_DRV += \
	board.c \
	soc_platform_pll.c
endif

ifeq ($(OS),Windows_NT)
  BIN_CERT_GEN=powershell -executionpolicy unrestricted -command $(ROOTDIR)/ti/build/makerules/x509CertificateGen.ps1
else
  BIN_CERT_GEN=$(ROOTDIR)/ti/build/makerules/x509CertificateGen.sh
endif
BIN_CERT_KEY=$(ROOTDIR)/ti/build/makerules/k3_dev_mpk.pem

#Common entry object
ifeq ($(SOC), $(filter $(SOC), AM335x AM437x))
ENTRY_SRC = sbl_init.S
endif
ifeq ($(SOC), K2G)
ENTRY_SRC = init.S
endif

# FLAGS for the SourceFiles
CFLAGS += -Wall -O2
SRC_CFLAGS = -I. $(CFLAGS) -g -gdwarf-3 -gstrict-dwarf -Wall

# Make Rule for the SRC Files
SRC_OBJS = $(patsubst %.c, $(FLASH_PGM_OBJDIR)/%.$(OBJEXT), $(SRC_C))
ENTRY_OBJ = $(patsubst %.S, $(FLASH_PGM_OBJDIR)/%.$(ASMOBJEXT), $(ENTRY_SRC))

SRC_DRV_OBJS = $(patsubst %.c, $(FLASH_PGM_OBJDIR)/%.$(OBJEXT), $(SRC_DRV))

ifeq ($(SOC), $(filter $(SOC), AM335x))
uart_$(BOARD)_flash_programmer:$(FLASH_PGM_BINDIR)/$(BIN_NAME)
else
uart_$(BOARD)_flash_programmer:$(FLASH_PGM_BINDIR)/$(EXE)
endif

$(FLASH_PGM_BINDIR)/$(EXE): $(SRC_OBJS) $(SRC_DRV_OBJS) $(ENTRY_OBJ) $(FLASH_PGM_BINDIR)/.created $(FLASH_PGM_OBJDIR)/.created
	@echo linking $(SRC_OBJS) $(SRC_DRV_OBJS) $(ENTRY_OBJ) into $@ ...
	@$(CC) $(SRC_OBJS) $(SRC_DRV_OBJS) $(ENTRY_OBJ) $(INTERNALLINKDEFS) -Wl,-Map=$(FLASH_PGM_BINDIR)/uart_$(BOARD)_flash_programmer.map -o $@

ifeq ($(SOC), AM335x)
$(FLASH_PGM_BINDIR)/$(BIN_NAME) : $(FLASH_PGM_BINDIR)/$(EXE)
	$(BIN) $(BINFLAGS) $(FLASH_PGM_BINDIR)/$(EXE) $@

else
$(FLASH_PGM_BINDIR)/$(BIN_NAME) : $(FLASH_PGM_BINDIR)/$(EXE)
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(BIN_CERT_GEN)
endif
	$(BIN) --gap-fill=0xff -O binary $(FLASH_PGM_BINDIR)/$(EXE) $(FLASH_PGM_BINDIR)/$(BIN_NAME)
	$(BIN_CERT_GEN) -b $(FLASH_PGM_BINDIR)/$(BIN_NAME) -o $(FLASH_PGM_BINDIR)/uart_$(BOARD)_flash_programmer.tiimage -c R5 -l 0x41c00100 -k $(BIN_CERT_KEY) -d DEBUG -j DBG_FULL_ENABLE
endif

$(FLASH_PGM_OBJDIR)/%.$(OBJEXT): %.c $(FLASH_PGM_OBJDIR)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS) $(INCS)  $< -o $@

$(FLASH_PGM_OBJDIR)/%.$(ASMOBJEXT): %.S $(FLASH_PGM_OBJDIR)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS) $(INCS)  $< -o $@

$(FLASH_PGM_OBJDIR)/.created:
	@mkdir -p $(FLASH_PGM_OBJDIR)
	@touch $(FLASH_PGM_OBJDIR)/.created

$(FLASH_PGM_BINDIR)/.created:
	@mkdir -p $(FLASH_PGM_BINDIR)
	@touch $(FLASH_PGM_BINDIR)/.created

$(STARTERWARE_DIR)/tools/ti_image/$(TIIMAGE):
	gcc -o $(STARTERWARE_DIR)/tools/ti_image/$(TIIMAGE) $(STARTERWARE_DIR)/tools/ti_image/tiimage.c

clean: uart_$(BOARD)_flash_programmer_clean

uart_$(BOARD)_flash_programmer_clean:
	@rm -f $(FLASH_PGM_BINDIR)/$(EXE) $(FLASH_PGM_BINDIR)/$(BIN_NAME)
	@rm -f $(SRC_OBJS) $(SRC_DRV_OBJS) $(ENTRY_OBJ)
	@rm -f $(FLASH_PGM_BINDIR)/uart_$(BOARD)_flash_programmer.map
