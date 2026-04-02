ifneq ($(TARGET_SCPU),)
ifeq ($(TARGET_SCPU),$(filter $(TARGET_SCPU),C64P C66 C674))
include $(PRELUDE)
TARGET=c6xsim_$(TARGET_CPU)_$(TARGET_SCPU)
TARGETTYPE=library
DEFS=LITTLE_ENDIAN_HOST
CSOURCES=C6xSimulator.c
ifeq ($(TARGET_SCPU),C66)
DEFS+=TMS320C66X
CSOURCES+=c66_ag_intrins.c c66_data_sim.c
else ifeq ($(TARGET_SCPU),C674)
DEFS+=TMS320C67X
CSOURCES+=c66_ag_intrins.c c66_data_sim.c
else ifeq ($(TARGET_SCPU),C64P)
DEFS+=TMS320C64PX
else
$(error TARGET_SCPU=$(TARGET_SCPU) is not supported! Options=[C64P C66 C674])
endif
include $(FINALE)
endif
endif