#******************************************************************************
#*      Copyright (C) 2009-2012 Texas Instruments Incorporated.               *
#*                      All Rights Reserved                                   *
#******************************************************************************

##############################################################
ALGBASE_PATH ?= $(abspath ../../../)
PSDK_INSTALL_PATH ?= $(abspath ../../../../)
include $(ALGBASE_PATH)/makerules/config.mk

COMMON_DIR = ../../../common
DSP_MODULES = $(ALGBASE_PATH)/ti_dl
CFILES := tidl_dump_main.cpp tidl_dump.cpp tidl_strings.cpp
CFILES += ../perfsim/gc_helper.c
OUTFILE= ./out/tidl_dump.out
##############################################################
TARGET_PLATFORM = PC

include $(ALGBASE_PATH)/makerules/rules.mk
include $(ALGBASE_PATH)/makerules/common.mk

#used inside makerules, but okay to define it afterwards
CFLAGS+= -I ../../inc
CFLAGS+= -I ../perfsim
CFLAGS+= -I $(COMMON_DIR)
CFLAGS+= -I $(IVISION_PATH)
CFLAGS+= -I $(DSP_MODULES)/algo/inc

OUTDIR =  "./out"

$(OUTFILE) : outfile
