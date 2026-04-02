
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 R5F C66))
ifeq ($(BUILD_HWA_KERNELS),yes)

include $(PRELUDE)
TARGET      := vx_target_kernels_j7_arm
TARGETTYPE  := library
CSOURCES    := vx_kernels_hwa_target.c vx_dof_visualize_target.c

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64))
  CSOURCES    += vx_kernels_hwa_target_utils.c
endif

IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/hwa/include
IDIRS       += $(VXLIB_PATH)/packages

ifeq ($(BUILD_DMPAC_DOF),yes)
DEFS += BUILD_DMPAC_DOF
endif

ifeq ($(TARGET_CPU)$(BUILD_VLAB),R5Fyes)
DEFS += VLAB_HWA
endif

include $(FINALE)

endif
endif

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721S2 J784S4 J722S J742S2))

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C71 C7120 C7524))

include $(PRELUDE)
TARGET      := vx_target_kernels_j7_arm
TARGETTYPE  := library
CSOURCES    := vx_kernels_hwa_target.c vx_dof_visualize_target.c

IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/hwa/include
IDIRS       += $(VXLIB_PATH)/packages

ifeq ($(BUILD_DMPAC_DOF),yes)
DEFS += BUILD_DMPAC_DOF
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C71 C7100 C7120 C7524))
DEFS += C6X_MIGRATION _TMS320C6600
endif

include $(FINALE)

endif

endif
