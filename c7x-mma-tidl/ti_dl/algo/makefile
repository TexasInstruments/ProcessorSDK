#
# module name       :TIDL
#
# module descripton :TI Deep learning Library module is TIs CNN/DNN
#                    acceleration on EVE and DSP
#
# Copyright (C) 2016-2017 Texas Instruments Incorporated - http://www.ti.com/
#
# ALL RIGHTS RESERVED
#
#
#
ALGBASE_PATH ?= $(abspath ../../)
PSDK_INSTALL_PATH ?= $(abspath ../../../)
include $(ALGBASE_PATH)/makerules/config.mk

##############################################################
DSP_COMMON_DIR= ../../common

##############################################################

SUBDIRS = ./inc
SUBDIRS+= ./src
##############################################################
#Including the reference .h file folder
##############################################################
INCLUDE_PATHS+= -I ./inc/ref
##############################################################

########################################################################
#Reference source files will be compiled only in HOST_EMULATION mode
########################################################################
ifeq ($(TARGET_PLATFORM), PC)
SUBDIRS+= ./src/ref
CFILES+= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/ref/*.c))
HFILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/ref/*.h))
endif
########################################################################

##############################################################
#Including the stats collection .h file folder
##############################################################
INCLUDE_PATHS+= -I ./inc/stats
##############################################################

########################################################################
#Stats collection source files will be compiled only in HOST_EMULATION mode
########################################################################
ifeq ($(TARGET_PLATFORM), PC)
SUBDIRS+= ./src/stats
CFILES+= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/stats/*.c))
HFILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/stats/*.h))
endif
########################################################################

CFILES= $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
HFILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.h))
CPPFILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))

########################################################################
#Common files is getting executed in Target and other modules
########################################################################
INCLUDE_PATHS+= -I ../common/inc
HFILES += ../common/inc/tidl_common_utils_infer_import.h
CFILES += ../common/src/tidl_common_utils_infer_import.c

########################################################################
#Reference file is getting executed in Target for the following layers
########################################################################
HFILES += ./inc/ref/tidl_innerProduct_ref.h
CFILES += ./src/ref/tidl_innerProduct_ref.c

HFILES += ./inc/ref/tidl_resize_ref.h
CFILES += ./src/ref/tidl_resize_ref.c

HFILES += ./inc/ref/tidl_softmax_ref.h
CFILES += ./src/ref/tidl_softmax_ref.c
########################################################################

########################################################################
#Stats Collection file is getting executed in Target for the following layers
########################################################################
HFILES += ./inc/tidl_traceUtils.h
CFILES += ./src/tidl_traceUtils.c
########################################################################

CMD_FILE= tidl_alg.cmd

LIB_DIR=algo

ifeq ($(TIDL_BUILD_PATHS), LEGACY)
ifeq ($(TARGET_PLATFORM) , PC)
LIBDIR  = ../lib/PC/$(CORE)/$(LIB_DIR)/$(TARGET_BUILD)
else
LIBDIR  = ../lib/$(CORE)/$(LIB_DIR)/$(TARGET_BUILD)
endif
else
ifeq ($(TARGET_PLATFORM) , PC)
LIBDIR  = ../lib/$(TARGET_SOC)/PC/$(LIB_DIR)/$(TARGET_BUILD)
else
LIBDIR  = ../lib/$(TARGET_SOC)/$(CORE)/$(LIB_DIR)/$(TARGET_BUILD)
endif
endif

ifeq ($(TARGET_PLATFORM) , PC)
ifdef SystemRoot
OUTFILE=$(LIBDIR)/tidl_algo.lib
else
OUTFILE=$(LIBDIR)/libtidl_algo.a
endif
else
OUTFILE=$(LIBDIR)/tidl_algo.lib
endif

ifeq ($(CODE_COVERAGE_ENABLED_FOR_TIDL),yes)
APP_UTILS_PATH?=$(PSDK_INSTALL_PATH)/app_utils
LDIRS += $(APP_UTILS_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
STATIC_LIBS += app_utils_file_io
STATIC_LIBS += app_utils_console_io
STATIC_LIBS += app_utils_rtos
INCLUDE_PATHS+= -I $(APP_UTILS_PATH)/utils/file_io/include/
INCLUDE_PATHS+= -I $(APP_UTILS_PATH)/utils/console_io/include/
INCLUDE_PATHS+= -I $(APP_UTILS_PATH)/utils/timer/include/
endif
##############################################################


##############################################################
include $(ALGBASE_PATH)/makerules/rules.mk
include $(ALGBASE_PATH)/makerules/common.mk
#used inside makerules, but okay to define it afterwards
ifeq ($(BUILD_WITH_OPENACC), 1)
	CC = nvc++ -c -fast -acc -Minfo=accel -gpu=ccall -tp=px
endif
INCLUDE_PATHS+= -I .
INCLUDE_PATHS+= -I ./inc
INCLUDE_PATHS+= -I ./inc/dummy
INCLUDE_PATHS+= -I ../inc
INCLUDE_PATHS+= -I $(DSP_COMMON_DIR)
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), AM62A am62a J722S j722s))
INCLUDE_PATHS+= -I ./src/tidsp/mmalib_dummy
endif
INCLUDE_PATHS+= -I ../utils/perfsim
INCLUDE_PATHS+= -I src/tidsp/inc
INCLUDE_PATHS+= -I src/tidsp/c7x_test
INCLUDE_PATHS+= -I src/tidsp/inc_priv

#mv7100 should be in rules.mk
CFLAGS += $(INCLUDE_PATHS)


##############################################################


##############################################################
LDFLAGS= -l "./tidl_alg.cmd"
ifeq ($(CORE) , eve)
$(OUTFILE) : libfile
else ifeq ($(CORE) , dsp)
$(OUTFILE) : libfile
endif

##############################################################

