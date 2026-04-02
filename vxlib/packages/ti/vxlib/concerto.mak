#******************************************************************************
#*      Copyright (C) 2015 Texas Instruments Incorporated.                    *
#*                      All Rights Reserved                                   *
#******************************************************************************

ifeq ($(TARGET_CPU),C66)
TARGETFOLDER := c66
else
TARGETFOLDER := c71
endif

# Copies the proper linker file to the proper output directory
_MODULE = linker_cmd
include $(PRELUDE)
TARGET = lnk
TARGETTYPE = prebuilt
PREBUILT = $(call PATH_CONV,concerto/$(TARGETFOLDER)/lnk.cmd)
include $(FINALE)

# Copies the proper linker file to the proper output directory
_MODULE = regress_linker_cmd
include $(PRELUDE)
TARGET = lnk_reg
TARGETTYPE = prebuilt
PREBUILT = lnk_reg.cmd
include $(FINALE)

ifneq ($(TARGET_SCPU),C71)
_MODULE := regress
include $(PRELUDE)
TARGET = $(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS := vxlib_d_$(TARGET_CPU) vxlib_$(TARGET_CPU) vxlib_cn_$(TARGET_CPU) common_$(TARGET_CPU)
ifneq ($(TARGET_SCPU),)
STATIC_LIBS += c6xsim_$(TARGET_CPU)_$(TARGET_SCPU)
endif
ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)
CSOURCES := main_c7x.c
else
CSOURCES := main.c
endif
LINKER_FILES := lnk.cmd
include $(FINALE)
endif
