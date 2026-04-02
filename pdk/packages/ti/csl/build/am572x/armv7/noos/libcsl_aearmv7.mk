#*******************************************************************************
#* FILE PURPOSE: Lower level makefile for Creating Component Libraries for ARMv7
#*******************************************************************************
#* FILE NAME: ./lib/libcsl_aearmv7.mk
#*
#* DESCRIPTION: Defines Source Files, Compilers flags and build rules
#*
#*******************************************************************************
#

#
# Macro definitions referenced below
#
empty =
space =$(empty) $(empty)

ARMV7OBJDIR ?= $(PDK_INSTALL_PATH)
ARMV7BINDIR ?= $(PDK_INSTALL_PATH)
ARMV7LIBDIR ?= $(PDK_INSTALL_PATH)

ARMV7CSLOBJDIR := $(ARMV7OBJDIR)/ti/$(LLD_NAME)/obj/am572x/armv7/noos
ARMV7CSLLIBDIR := $(ARMV7LIBDIR)/ti/$(LLD_NAME)/lib/am572x/armv7/noos

#Cross tools
ifdef CROSS_TOOL_INSTALL_PATH
# Support backwards compatibility with KeyStone1 approach
 CC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
 AC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)as
 AR = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ar
 LD = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ld
endif

# INCLUDE Directories
INCDIR = ../..
INCDIR += ./soc/am572x/src
INCDIR += ./src/ip/chip/V0

# Compiler options
INTERNALDEFS = -g -c -mcpu=cortex-a15 -mtune=cortex-a15 -mfpu=neon -mfloat-abi=hard
INTERNALDEFS += -mlong-calls -fdata-sections -funsigned-char -ffunction-sections -Wall
INTERNALDEFS += --specs=rdimon.specs
INTERNALDEFS += $(DEBUG_FLAG) -D__ARMv7 -DSOC_AM572x

CFLAGS += $(INTERNALDEFS) $(DEBUG_FLAG)

OBJEXT = o

# SRC INCLUDE Directories
SRCDIR := ./soc/am572x/src
SRCDIR += ./src/ip/edma/V0

VPATH=$(SRCDIR)

#List the COMMONSRC Files
SRC_C = \
    csl_device_pllc.c \
    csl_device_xbar.c

#List the ASM Source Files
SRC_ASM =

# INCLUDE Directories
INCS = -I. $(addprefix -I,$(INCDIR)) $(addprefix -I,$(SRCDIR))

# Make Rule for the COMMONSRC Files
SRC_C_OBJS = $(patsubst %.c, $(ARMV7CSLOBJDIR)/%.$(OBJEXT), $(SRC_C))
SRC_ASM_OBJS = $(patsubst %.S, $(ARMV7CSLOBJDIR)/%.$(OBJEXT), $(SRC_ASM))

all:$(ARMV7CSLLIBDIR)/lib$(LLD_NAME).a

$(SRC_C_OBJS): $(ARMV7CSLOBJDIR)/%.$(OBJEXT): %.c $(ARMV7CSLOBJDIR)/.created
	-@echo compiling $< ...
	@$(CC) -c $(CFLAGS) $(INCS)  $< -o $@

$(SRC_ASM_OBJS): $(ARMV7CSLOBJDIR)/%.$(OBJEXT): %.S $(ARMV7CSLOBJDIR)/.created
	-@echo compiling $< ...
	@$(CC) -c $(CFLAGS) $(INCS)  $< -o $@

$(ARMV7CSLLIBDIR)/lib$(LLD_NAME).a: $(SRC_C_OBJS) $(SRC_ASM_OBJS) $(ARMV7CSLLIBDIR)/.created
	@echo archiving $(SRC_C_OBJS) $(SRC_ASM_OBJS) into $@ ...
	@$(AR) -r $@ $(SRC_C_OBJS) $(SRC_ASM_OBJS)

$(ARMV7CSLOBJDIR)/.created:
	@mkdir -p $(ARMV7CSLOBJDIR)
	@touch $(ARMV7CSLOBJDIR)/.created

$(ARMV7CSLLIBDIR)/.created:
	@mkdir -p $(ARMV7CSLLIBDIR)
	@touch $(ARMV7CSLLIBDIR)/.created

clean:
	@$(RMDIR) $(ARMV7CSLOBJDIR)
	@$(RMDIR) $(ARMV7CSLLIBDIR)
