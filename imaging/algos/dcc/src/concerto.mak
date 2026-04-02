ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 R5F))

include $(PRELUDE)
TARGET      := ti_imaging_dcc
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

IDIRS       += $(HOST_ROOT)/algos/awb/include
IDIRS       += $(HOST_ROOT)/algos/dcc/include

include $(FINALE)

endif
