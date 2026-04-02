ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), R5F))
ifeq ($(BUILD_HWA_KERNELS),yes)

include $(PRELUDE)
TARGET      := imaging_coverage
TARGETTYPE  := library


ifeq ($(TARGET_CPU),R5F)
    CSOURCES    := ldra_remote_core_coverage_main.c 
endif



include $(FINALE)

endif
endif

