#*******************************************************************************
#* FILE PURPOSE: Lower level makefile for Creating Component Libraries for ARMv7
#*******************************************************************************
#* FILE NAME: ./lib/libsa_aearmv7.mk
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

# Output for prebuilt generated libraries
ARMV7LIBDIR ?= ./lib
ARMV7OBJDIR ?= ./obj
ARMV7OBJDIR_SO := $(ARMV7OBJDIR)/sa/lib_so
ARMV7OBJDIR := $(ARMV7OBJDIR)/sa/lib
ARMV7BINDIR ?= ./bin

# Default optimization is on
DEBUG_FLAG    ?= -O2

ifdef CROSS_TOOL_INSTALL_PATH
# Support backwards compatibility with KeyStone1 approach
CC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
AC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)as
AR = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ar
LD = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
endif

INCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(INCDIR))))

CFLAGS+=-mno-unaligned-access
INTERNALDEFS = -D__ARMv7 -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD
CFLAGS += $(INTERNALDEFS)

OBJEXT = o 
INTERNALLINKDEFS =
SRCDIR = ./src

VPATH=$(SRCDIR) 

#List the COMMONSRC Files
COMMONSRCC = \
    salld.c\
    salldcom.c\
    salldtxrx.c\
    salldctbl.c\
    salldsc.c\
    salldmci.c\
    proto/srtp/salldsrtp.c\
    proto/srtp/salldsrtpinit.c\
    proto/srtp/salldsrtp_ctr.c\
    proto/srtp/salldsrtp_f8.c\
    proto/srtp/salldsrtpsc.c\
    proto/srtp/salldsrtcp.c\
    proto/srtp/salldsrtcpinit.c\
    proto/srtp/salldsrtcp_ctr.c\
    proto/srtp/salldsrtcp_f8.c\
    proto/ipsec/salldipsecinit.c\
    proto/ipsec/salldipsec.c\
    proto/ipsec/salldah.c\
    proto/ipsec/salldesp.c\
    proto/airciph/salldacinit.c\
    proto/airciph/salldac.c\
    proto/datamode/sallddminit.c\
    proto/datamode/sallddm.c\
    proto/datamode/sallddmcmdl.c\
    cipher/salldaes.c\
	cipher/salldaes_ctr.c\
    cipher/salldaes_f8.c\
    cipher/salldgcm.c\
    auth/salldcmac.c\
    auth/salldhmac_md5.c\
    auth/salldhmac_sha1.c\
    auth/salldhmac_sha2.c\
    auth/salldmd5.c\
	auth/salldsha1.c\
    auth/salldsha2.c\
    auth/salldxcbc.c

# FLAGS for the COMMONSRC Files
COMMONSRCCFLAGS = $(DEBUG_FLAG) -I$(SRCDIR) -I.
CFLAGS +=$(COMMONSRCCFLAGS)


# Make Rule for the COMMONSRC Files
COMMONSRCCOBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(COMMONSRCC))
COMMONSRCCOBJS_SO = $(patsubst %.c, $(ARMV7OBJDIR_SO)/%.$(OBJEXT), $(COMMONSRCC))

$(COMMONSRCCOBJS): $(ARMV7OBJDIR)/%.$(OBJEXT): %.c $(ARMV7OBJDIR)/.created
	-@echo compiling $< ...
	@if [ ! -d $(@D) ]; then mkdir -p $(@D) ; fi;
	@$(CC) -c $(CFLAGS) $(INCS)  $< -o $@
	
$(COMMONSRCCOBJS_SO): $(ARMV7OBJDIR_SO)/%.$(OBJEXT): %.c $(ARMV7OBJDIR_SO)/.created
	-@echo compiling $< ...
	@if [ ! -d $(@D) ]; then mkdir -p $(@D) ; fi;	
	@$(CC) -c $(CFLAGS) -fPIC $(INCS)  $< -o $@

$(ARMV7LIBDIR)/libsa.a: $(COMMONSRCCOBJS) $(ARMV7LIBDIR)/.created
	@echo archiving static library $? into $@ ...
	@$(AR) -r $@ $?

libsa.so: $(COMMONSRCCOBJS_SO)
	@echo archiving $? into $(ARMV7LIBDIR)/$@.1 ...
	@$(CC) $(DEBUG_FLAG) -ggdb2 -Wl,-soname=libsa_device.so.1 -shared -fPIC ${LDFLAGS} -o $@.1.0.0 $^
	@ln -s $@.1.0.0 $@.1
	@ln -s $@.1     $@	
	@mv -f $@.1.0.0 $(ARMV7LIBDIR)/$@.1.0.0
	@mv -f $@.1 $(ARMV7LIBDIR)/$@.1
	@mv -f $@   $(ARMV7LIBDIR)/$@	

$(ARMV7OBJDIR)/.created:
	@mkdir -p $(ARMV7OBJDIR)
	@touch $(ARMV7OBJDIR)/.created

$(ARMV7OBJDIR_SO)/.created:
	@mkdir -p $(ARMV7OBJDIR_SO)
	@touch $(ARMV7OBJDIR_SO)/.created

$(ARMV7LIBDIR)/.created:
	@mkdir -p $(ARMV7LIBDIR)
	@touch $(ARMV7LIBDIR)/.created

clean:
	@echo cleaning $(ARMV7OBJDIR)
	@$(RMDIR) $(ARMV7OBJDIR)
	@$(RMDIR) $(ARMV7OBJDIR_SO)
