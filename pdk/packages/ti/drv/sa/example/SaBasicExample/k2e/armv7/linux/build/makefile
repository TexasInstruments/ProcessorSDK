#
# Macro definitions referenced below
#

empty =
space =$(empty) $(empty)

ARMV7OBJDIR ?= ./obj/$(DEVICE)
ARMV7BINDIR ?= ./bin/$(DEVICE)
ARMV7LIBDIR ?= ./lib

ARMV7OBJDIR := $(ARMV7OBJDIR)/sa/example/SaBasicExample
ARMV7BINDIR := $(ARMV7BINDIR)/sa/example

# TEST Directories	   
SA_TEST_ROOT_DIR=$(ARMV7BINDIR)
SA_TEST_OUT_DIR=$(SA_TEST_ROOT_DIR)/ti/drv/sa/example/SaBasicExample/outputs
SA_TEST_VECTOR_DIR=$(SA_TEST_ROOT_DIR)/ti/drv/sa/example/SaBasicExample/vectors

# Cross tools
ifdef CROSS_TOOL_INSTALL_PATH
# Support backwards compatibility with KeyStone1 approach
CC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
AC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)as
AR = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ar -r
LD = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
endif

# 3GPP Enable/Disable
ifeq "$(SA_EXAMPLES_3GPP_SUPPORT)" "ENABLE"
INTERNALDEFS = -DSA_3GPP_SUPPORT
SA_3GPP_LIB   = $(PDK_INSTALL_PATH)/ti/drv/sa/sa3gppEnabler/lib/armv7/libsa3gpp.a
else
INTERNALDEFS=
SA_3GPP_LIB=
endif

# INCLUDE Directories
QMSS_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/qmss
CPPI_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/cppi
PA_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/pa

SA_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/sa
SA_FW_DIR = $(PDK_INSTALL_PATH)/ti/drv/sa/fw/v1

SA_SRC_DIR ?= $(PDK_INSTALL_PATH)/ti/drv/sa
SA_TEST_DIR = $(SA_SRC_DIR)/example/SaBasicExample
SA_VECTORS_DIR = $(SA_TEST_DIR)/vectors
SA_TEST_SRC_DIR = $(SA_TEST_DIR)/src
SA_TEST_COM_DIR = $(SA_TEST_SRC_DIR)/common
SA_TEST_SIM_DIR = $(SA_TEST_SRC_DIR)/salldsim
SA_TEST_UT_DIR = $(SA_TEST_SRC_DIR)/tests
SA_TEST_ARM_LIN_DIR = $(SA_TEST_SRC_DIR)/armv7/linux

INCDIR := $(PDK_INSTALL_PATH);$(CPPI_INC_DIR);$(QMSS_INC_DIR);$(PA_INC_DIR);$(SA_TEST_SRC_DIR);$(SA_TEST_COM_DIR);$(SA_TEST_UT_DIR);$(SA_TEST_ARM_LIN_DIR);$(SA_VECTORS_DIR)

# Libraries
CPPI_LIB = -lcppi
QMSS_LIB = -lqmss
PA_LIB   = -lpa2
SA_LIB   = -lsa
SA_LIB_SO = -lsa_device
RM_LIB   = -lrm

ifeq ($(USEDYNAMIC_LIB), yes)
#presuming ARM executable would depend on dynamic library dependency
EXE_EXTN = _so
LIBS     = $(CPPI_LIB) $(QMSS_LIB) $(PA_LIB) $(RM_LIB) $(SA_LIB_SO) $(SA_3GPP_LIB)
else
#forcing ARM executable to depend on static LLD libraries
LIBS     = -Wl,-Bstatic $(CPPI_LIB) $(QMSS_LIB) $(PA_LIB) $(RM_LIB) $(SA_LIB) $(SA_3GPP_LIB) -Wl,-Bdynamic
EXE_EXTN =
endif

# Compiler options
CFLAGS_L+=-mno-unaligned-access
INTERNALDEFS += -g -D__ARMv7 -DDEVICE_K2E -DNSS_GEN2 -D_VIRTUAL_ADDR_SUPPORT -D__LINUX_USER_SPACE -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD -DSA_ROOT_DIR=./

# Linker options
INTERNALLINKDEFS = -Wl,--start-group -L $(ARMV7LIBDIR) $(LIBS) -lrt -Wl,--end-group -pthread $(LDFLAGS)

EXE=saBasicExample$(EXE_EXTN).out
OBJEXT = o 
SRCDIR = $(SA_TEST_DIR):$(SA_TEST_COM_DIR):$(SA_TEST_UT_DIR):$(SA_TEST_ARM_LIN_DIR):$(SA_TEST_SIM_DIR):$(SA_FW_DIR):$(SA_TEST_SRC_DIR):$(SA_VECTORS_DIR)

INCS = -I. -I$(strip $(subst ;, -I,$(INCDIR)))

VPATH=$(SRCDIR)

#List the COMMONSRC Files
COMMONSRCC = \
    fw_main.c \
    fw_init.c \
    fw_osal.c \
    fw_mem_allocator.c \
    fw_test_mem.c \
    sockutils.c \
    common.c \
    testutil.c \
    setuprm.c \
    test1.c \
    test2.c \
    test3.c \
    test4.c \
    test5.c \
    test6.c \
    test7.c \
    pkt_test1.c \
    pkt_test2.c \
    pkt_test3.c \
    pkt_test4.c \
    pkt_test5.c \
    pkt_test7.c \
    pkt_test1_enc.c \
    pkt_test2_enc.c \
    pkt_test3_enc.c \
    pkt_test4_enc.c \
    pkt_test5_enc.c

SALLDSIMSRCC = \
    salldsim.c \
    salldutil.c \
    salldcfg.c \
    salld_osal.c

SALLDFWSRCC = \
    saphp1_bin.c \
    saphp2_bin.c \
    saphp3_bin.c 

# FLAGS for the COMMONSRC Files
COMMONSRCCFLAGS = -I.

# Make Rule for the COMMONSRC Files
COMMONSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(COMMONSRCC))
SALLDSIMSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(SALLDSIMSRCC))
SALLDFWSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(SALLDFWSRCC))

CFLAGS_L += $(COMMONSRCCFLAGS) $(INTERNALDEFS)

all:$(ARMV7BINDIR)/$(EXE) install_vecs

$(ARMV7BINDIR)/$(EXE): $(ARMV7BINDIR)/.created $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) 
	@echo linking $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) into $@ ...
	@$(CC) $(COMMONSRCCOBJS) $(SALLDSIMSRCCOBJS) $(SALLDFWSRCCOBJS) $(INTERNALLINKDEFS) -o $@	

$(ARMV7OBJDIR)/%.$(OBJEXT): %.c $(ARMV7OBJDIR)/.created
	@echo compiling $< ...	
	@$(CC) -c $(CFLAGS_L) $(INCS)  $< -o $@

$(ARMV7OBJDIR)/.created:
	@mkdir -p $(ARMV7OBJDIR)
	@touch $(ARMV7OBJDIR)/.created

$(ARMV7BINDIR)/.created:
	@mkdir -p $(ARMV7BINDIR)
	@touch $(ARMV7BINDIR)/.created

install_vecs:
	@mkdir -p $(SA_TEST_VECTOR_DIR)
	@mkdir -p $(SA_TEST_OUT_DIR)
	@cp -f $(SA_TEST_DIR)/vectors/* $(SA_TEST_VECTOR_DIR)

clean:
	@rm -fr $(ARMV7OBJDIR)
	@rm -fr $(ARMV7BINDIR)

