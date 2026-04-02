#
# Macro definitions referenced below
#

empty =
space =$(empty) $(empty)

ARMV7OBJDIR ?= ./obj/$(DEVICE)
ARMV7BINDIR ?= ./bin/$(DEVICE)
ARMV7LIBDIR ?= ./lib

ARMV7OBJDIR := $(ARMV7OBJDIR)/sa/util/coreDump
ARMV7BINDIR := $(ARMV7BINDIR)/sa/util

# Cross tools
ifdef CROSS_TOOL_INSTALL_PATH
CC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
AC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)as
AR = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ar -r
LD = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
endif

# INCLUDE Directories
SA_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/sa

SA_SRC_DIR ?= $(PDK_INSTALL_PATH)/ti/drv/sa
SA_UTIL_DIR               = $(SA_SRC_DIR)/util
SA_UTIL_SRC_DIR           = $(SA_UTIL_DIR)/coreDump
SA_UTIL_ARM_LIN_DIR = $(SA_UTIL_SRC_DIR)/armv7/linux

INCDIR := $(PDK_INSTALL_PATH);$(SA_UTIL_SRC_DIR);$(SA_UTIL_DIR)

# Libraries
SA_LIB   = -lsa

LIBS     = -Wl,-Bstatic $(SA_LIB) -Wl,-Bdynamic

# Compiler options
CFLAGS_L+=-mno-unaligned-access
INTERNALDEFS += -g -D__ARMv7 -DDEVICE_K2K -D_VIRTUAL_ADDR_SUPPORT -D__LINUX_USER_SPACE -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD -DSA_ROOT_DIR=./

# Linker options
INTERNALLINKDEFS = -Wl,--start-group -L $(ARMV7LIBDIR) -L $(ARMV7LIBDIR) $(LIBS) -lrt -lpthread -Wl,--end-group

EXE=saCoreDumpUtil_$(DEVICE).out
OBJEXT = o 
SRCDIR = $(SA_UTIL_SRC_DIR) $(SA_UTIL_DIR)

INCS = -I. -I$(strip $(subst ;, -I,$(INCDIR)))

VPATH=$(SRCDIR)

#List the COMMONSRC Files
COMMONSRCC = \
    utilSaCoreDumpUtil.c \
	utilSaOsal.c
    
SALLDSIMSRCC = \

SALLDFWSRCC = \

# FLAGS for the COMMONSRC Files
COMMONSRCCFLAGS = -I.

# Make Rule for the COMMONSRC Files
COMMONSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(COMMONSRCC))
SALLDSIMSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(SALLDSIMSRCC))
SALLDFWSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(SALLDFWSRCC))

CFLAGS_L += $(COMMONSRCCFLAGS) $(INTERNALDEFS)

all:$(ARMV7BINDIR)/$(EXE)

$(ARMV7BINDIR)/$(EXE): $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) $(ARMV7BINDIR)/.created
	@echo linking $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) into $@ ...
	@$(CC) $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) $(INTERNALLINKDEFS) -o $@	

$(ARMV7OBJDIR)/%.$(OBJEXT): %.c $(ARMV7OBJDIR)/.created
	@echo compiling $< ...	
	$(CC) -c $(CFLAGS_L) $(INCS)  $< -o $@

$(ARMV7OBJDIR)/.created:
	@mkdir -p $(ARMV7OBJDIR)
	@touch $(ARMV7OBJDIR)/.created

$(ARMV7BINDIR)/.created:
	@mkdir -p $(ARMV7BINDIR)
	@touch $(ARMV7BINDIR)/.created

clean:
	@rm -fr $(ARMV7OBJDIR)
	@rm -fr $(ARMV7BINDIR)

