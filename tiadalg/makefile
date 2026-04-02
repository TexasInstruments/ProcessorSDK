ALGBASE_PATH ?= $(abspath ./)

include $(ALGBASE_PATH)/makerules/config.mk

include $(PSDK_BUILDER_PATH)/build_flags.mak

BUILD_EMULATION_MODE?=yes
BUILD_TARGET_MODE?=yes
PROFILE?=all# @todo need to use this flag to enable debug/release/all build. This flag is coming from sdk_builder/build_flags.mak
BUILD_C66_HOST_EMULATION_LIB:=no
BUILD_C7x_LIB_PROG:=no

Q=@

PC_BUILD_CMD := TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) SOC=$(SOC)
TARGET_BUILD_CMD := TARGET_CPU=$(TARGET_CPU) SHOW_COMMANDS=$(SHOW_COMMANDS) SOC=$(SOC)

ifeq ($(TARGET_PLATFORM),PC)
	BUILD_EMULATION_MODE:=yes
endif

ifeq ($(TARGET_CPU),C66)
ifeq ($(BUILD_EMULATION_MODE),yes)
	BUILD_C66_HOST_EMULATION_LIB:=yes
endif
endif

ifeq ($(TARGET_CPU),C71)
	BUILD_C7x_LIB_PROG:=yes
endif
ifeq ($(TARGET_CPU),C7120)
	BUILD_C7x_LIB_PROG:=yes
endif

all: c_intrinsics_host_port
	$(MAKE) tiadalg_libs

clean: scrub

scrub:
	rm -rf lib
	rm -rf out

doxy_docs:
	$(Q)rm -rf $(PSDK_PATH)/tiadalg/docs/user_guide_html
	$(Q)doxygen $(PSDK_PATH)/tiadalg/docs/doxygen/user_guide_pages/tiadalg_user_guide_doxygen.cfg
	$(Q)cp $(PSDK_PATH)/tiadalg/docs/doxygen/ti_disclaim.htm $(PSDK_PATH)/tiadalg/docs/user_guide_html/

# common libraries which can be compiled for C6xx or C7x. It is controlled by the flag TARGET_CPU = C66/C71/C7120
COMMON_LIB_LIST = tiadalg_image_preprocessing_lib\
 tiadalg_visual_localization_lib\
 tiadalg_image_color_blending_lib\
 tiadalg_fisheye_transformation_lib\
 tiadalg_dof_plane_seperation_lib\
 tiadalg_select_top_feature_lib\
 tiadalg_solve_pnp_lib\
 tiadalg_sparse_upsampling_lib\
 tiadalg_image_recursive_nms_lib

# library list which can be only compiled for c7x. Lib is common for _cn files and optimized file. And for this typically optimized file will not be there
C7x_LIB_LIST = tiadalg_structure_from_motion_lib\
 tiadalg_voxelization_lib

COMMON_PROG_LIST = tiadalg_image_preprocessing\
 tiadalg_image_color_blending\
 tiadalg_fisheye_transformation\
 tiadalg_dof_plane_seperation\
 tiadalg_select_top_feature\
 tiadalg_solve_pnp\
 tiadalg_sparse_upsampling\
 tiadalg_image_recursive_nms\
 tiadalg_voxelization

C7x_PROG_LIST = tiadalg_structure_from_motion

tiadalg_libs: $(COMMON_LIB_LIST) $(C7x_LIB_LIST)

tiadalg_outs: $(COMMON_PROG_LIST) tiadalg_visual_localization $(C7x_PROG_LIST)

$(COMMON_LIB_LIST):
ifeq ($(BUILD_EMULATION_MODE),yes)
ifeq ($(PROFILE),all)
		$(MAKE) -C $(subst _lib,,$@) $(PC_BUILD_CMD) TARGET_BUILD=debug
		$(MAKE) -C $(subst _lib,,$@) $(PC_BUILD_CMD) TARGET_BUILD=release
else
		$(MAKE) -C $(subst _lib,,$@) $(PC_BUILD_CMD) TARGET_BUILD=$(PROFILE)
endif
endif
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(PROFILE),all)
	    $(MAKE) -C $(subst _lib,,$@) $(TARGET_BUILD_CMD) TARGET_BUILD=debug
	    $(MAKE) -C $(subst _lib,,$@) $(TARGET_BUILD_CMD) TARGET_BUILD=release
else
	    $(MAKE) -C $(subst _lib,,$@) $(TARGET_BUILD_CMD) TARGET_BUILD=$(PROFILE)
endif
endif

$(C7x_LIB_LIST):
ifeq ($(BUILD_C7x_LIB_PROG),yes)
ifeq ($(BUILD_EMULATION_MODE),yes)
ifeq ($(PROFILE),all)
			$(MAKE) -C $(subst _lib,,$@) TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=debug
			$(MAKE) -C $(subst _lib,,$@) TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=release
else
			$(MAKE) -C $(subst _lib,,$@) TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=$(PROFILE)
endif
endif
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(PROFILE),all)
			$(MAKE) -C $(subst _lib,,$@) SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=debug
			$(MAKE) -C $(subst _lib,,$@) SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=release
else
			$(MAKE) -C $(subst _lib,,$@) SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=$(TARGET_CPU) TARGET_BUILD=$(PROFILE)
endif
endif
endif

$(COMMON_PROG_LIST):$(addsuffix _lib,$@) c_intrinsics_host_port
ifeq ($(BUILD_EMULATION_MODE),yes)
ifeq ($(PROFILE),all)
	    $(MAKE) -C $@ $(PC_BUILD_CMD) test_build=1 TARGET_BUILD=debug
else
	    $(MAKE) -C $@ $(PC_BUILD_CMD) test_build=1 TARGET_BUILD=$(PROFILE)
endif
endif
ifeq ($(BUILD_TARGET_MODE),yes)
#only target test app is build with cpp flag
ifeq ($(PROFILE),all)
	    $(MAKE) -C $@ $(TARGET_BUILD_CMD) test_build=1 TARGET_BUILD=debug TIADALG_CPP_BUILD=yes
else
	    $(MAKE) -C $@ $(TARGET_BUILD_CMD) test_build=1 TARGET_BUILD=$(PROFILE) TIADALG_CPP_BUILD=yes
endif
endif

#$(C7x_PROG_LIST):$(addsuffix _lib,$@)
tiadalg_structure_from_motion:tiadalg_structure_from_motion_lib
ifeq ($(BUILD_C7x_LIB_PROG),yes)
ifeq ($(BUILD_EMULATION_MODE),yes)
ifeq ($(PROFILE),all)
			$(MAKE) final_install -C $@ TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=C71 test_build=1 TARGET_BUILD=debug
else
			$(MAKE) final_install -C $@ TARGET_PLATFORM=PC SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=C71 test_build=1 TARGET_BUILD=$(PROFILE)
endif
endif
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(PROFILE),all)
			$(MAKE) final_install -C $@ SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=C71 test_build=1 TARGET_BUILD=debug
else
			$(MAKE) final_install -C $@ SHOW_COMMANDS=$(SHOW_COMMANDS) TARGET_CPU=C71 test_build=1 TARGET_BUILD=$(PROFILE)
endif
endif
endif

tiadalg_visual_localization:tiadalg_visual_localization_lib tiadalg_sparse_upsampling_lib tiadalg_solve_pnp_lib tiadalg_select_top_feature_lib c_intrinsics_host_port
ifeq ($(BUILD_EMULATION_MODE),yes)
ifeq ($(PROFILE),all)
	    $(MAKE) -C $@ $(PC_BUILD_CMD) test_build=1 TARGET_BUILD=debug
else
	    $(MAKE) -C $@ $(PC_BUILD_CMD) test_build=1 TARGET_BUILD=$(PROFILE)
endif
endif
ifeq ($(BUILD_TARGET_MODE),yes)
ifeq ($(PROFILE),all)
	    $(MAKE) -C $@ $(TARGET_BUILD_CMD) test_build=1 TARGET_BUILD=debug TIADALG_CPP_BUILD=yes
else
	    $(MAKE) -C $@ $(TARGET_BUILD_CMD) test_build=1 TARGET_BUILD=$(PROFILE) TIADALG_CPP_BUILD=yes
endif
endif

c_intrinsics_host_port:
ifeq ($(BUILD_C66_HOST_EMULATION_LIB),yes)
ifeq ($(PROFILE),all)
		$(MAKE) -C common/$@ $(PC_BUILD_CMD) TARGET_BUILD=debug
		$(MAKE) -C common/$@ $(PC_BUILD_CMD) TARGET_BUILD=release
else
		$(MAKE) -C common/$@ $(PC_BUILD_CMD) TARGET_BUILD=$(PROFILE)
endif
endif

.PHONY: all clean scrub doxy_docs
.PHONY: tiadalg_libs \
				tiadalg_fisheye_transformation_lib \
				tiadalg_image_preprocessing_lib \
				tiadalg_dof_plane_seperation_lib \
				tiadalg_select_top_feature_lib \
				tiadalg_solve_pnp_lib \
				tiadalg_sparse_upsampling_lib \
				tiadalg_visual_localization_lib \
				tiadalg_image_color_blending_lib \
				tiadalg_image_recursive_nms_lib \
				tiadalg_voxelization_lib \

