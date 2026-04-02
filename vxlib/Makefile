#******************************************************************************
#*      Copyright (C) 2013 Texas Instruments Incorporated.                    *
#*                      All Rights Reserved                                   *
#******************************************************************************

include tools_path.mak

C6X_MIGRATION=1

CONCERTO_ROOT=concerto
ifeq ($(wildcard $(CONCERTO_ROOT)),)
$(error You must define CONCERTO_ROOT!)
endif
# Define the VLIB specific build locations
DIRECTORIES:= packages/ti
BUILD_TARGET := $(abspath .)/target.mak
include $(CONCERTO_ROOT)/rules.mak

doxy_docs:
	rm -Rf ./docs/doxygen/html
	mkdir -p ./docs/doxygen/html/vxlib_html
	$(COPY) ./docs/doxygen/config/images/* ./docs/doxygen/html/.
	$(COPY) ./docs/doxygen/config/images/* ./docs/doxygen/html/vxlib_html/.
	doxygen ./docs/doxygen/config/Doxyfile_top 2> doxy_warnings.txt
	doxygen ./docs/doxygen/config/Doxyfile_vxlib 2>> doxy_warnings.txt

