include $(PRELUDE)
TARGET      := ptk_base
TARGETTYPE  := library
CSOURCES    :=
FILE_LIST   := $(call all-c-files-in,src/base)
CSOURCES    += $(foreach f, $(FILE_LIST),base/$(f))

# Flag for resolving __isoc99_scanf link error seen for bare metal
# A72 compilation
CFLAGS      += -D_GNU_SOURCE

IDIRS       += $(HOST_ROOT)/include

include $(FINALE)

