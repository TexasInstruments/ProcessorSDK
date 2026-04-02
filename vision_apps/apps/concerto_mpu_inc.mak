
ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))

TEST_IDIRS =
TEST_IDIRS += $(TIOVX_PATH)/conformance_tests

IMAGING_IDIRS  =
IMAGING_IDIRS += $(IMAGING_PATH)/kernels/include
IMAGING_IDIRS += $(IMAGING_PATH)/sensor_drv/include
IMAGING_IDIRS += $(IMAGING_PATH)/utils/itt_server/include
IMAGING_IDIRS += $(IMAGING_PATH)/utils/network_api/include

VISION_APPS_KERNELS_IDIRS =
VISION_APPS_KERNELS_IDIRS += $(VISION_APPS_PATH)/kernels
VISION_APPS_KERNELS_IDIRS += $(VISION_APPS_PATH)/kernels/img_proc/include
VISION_APPS_KERNELS_IDIRS += $(VISION_APPS_PATH)/kernels/fileio/include

VISION_APPS_MODULES_IDIRS =
VISION_APPS_MODULES_IDIRS += $(VISION_APPS_PATH)/modules/include

VISION_APPS_SRV_IDIRS =
VISION_APPS_SRV_IDIRS += $(VISION_APPS_PATH)/kernels/srv/include
VISION_APPS_SRV_IDIRS += $(VISION_APPS_PATH)/kernels/srv/c66
VISION_APPS_SRV_IDIRS += $(VISION_APPS_PATH)/kernels/srv/gpu/3dsrv

VISION_APPS_SAMPLE_IDIRS += $(VISION_APPS_PATH)/kernels/sample/include
VISION_APPS_SAMPLE_IDIRS += $(VISION_APPS_PATH)/kernels/sample/host

VISION_APPS_APPLIBS_IDIRS =
VISION_APPS_APPLIBS_IDIRS += $(VISION_APPS_PATH)/applibs

PTK_IDIRS =
PTK_IDIRS += $(PTK_PATH)/include

VISION_APPS_STEREO_KERNELS_IDIRS =
VISION_APPS_STEREO_KERNELS_IDIRS += $(VISION_APPS_PATH)/kernels/stereo/include

EDGEAI_IDIRS =
EDGEAI_IDIRS += $(EDGEAI_UTILS_PATH)/include
EDGEAI_IDIRS += $(EDGEAI_KERNELS_PATH)/include

ifeq ($(TARGET_OS),LINUX)
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/linux/mpu1
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/linux
endif
ifeq ($(TARGET_OS),QNX)
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/qnx/mpu1
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/qnx
endif
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/rtos/common
IDIRS       += $(VISION_APPS_PATH)/platform/$(SOC)/rtos

BUILD_PROFILE_EDGEAI_REL = Release

LDIRS       += $(EDGEAI_UTILS_PATH)/$(TARGET_OS)/lib/$(BUILD_PROFILE_EDGEAI_REL)
LDIRS       += $(EDGEAI_KERNELS_PATH)/$(TARGET_OS)/lib/$(BUILD_PROFILE_EDGEAI_REL)

# These rpath-link linker options are to provide directories for
# secondary *.so file lookup
ifeq ($(TARGET_OS),LINUX)
$(_MODULE)_LOPT += -rpath-link=$(LINUX_FS_PATH)/usr/lib
$(_MODULE)_LOPT += -rpath-link=$(LINUX_FS_PATH)/lib
$(_MODULE)_LOPT += -rpath-link=$(LINUX_FS_PATH)/usr/lib/python3.8/site-packages/dlr
endif
ifeq ($(TARGET_OS),QNX)
$(_MODULE)_LOPT += -rpath-link=$(QNX_TARGET)/usr/lib
$(_MODULE)_LOPT += -rpath-link=$(QNX_TARGET)/lib
endif

CFLAGS+=-Wno-format-truncation


ifeq ($(TARGET_OS), QNX)

BUILD_PROFILE_QNX_SO = so.le
BUILD_PROFILE_QNX_A = a.le
BUILD_PROFILE_QNX_SUFFIX =

LDIRS       += $(PSDK_QNX_PATH)/qnx/pdk_libs/pdk/aarch64/$(BUILD_PROFILE_QNX_SO)
LDIRS       += $(PSDK_QNX_PATH)/qnx/pdk_libs/sciclient/aarch64/$(BUILD_PROFILE_QNX_SO)
LDIRS       += $(PSDK_QNX_PATH)/qnx/pdk_libs/udmalld/aarch64/$(BUILD_PROFILE_QNX_SO)
LDIRS       += $(PSDK_QNX_PATH)/qnx/sharedmemallocator/usr/aarch64/$(BUILD_PROFILE_QNX_SO)
LDIRS       += $(PSDK_QNX_PATH)/qnx/resmgr/ipc_qnx_rsmgr/usr/aarch64/$(BUILD_PROFILE_QNX_SO)
LDIRS       += $(PSDK_QNX_PATH)/qnx/resmgr/udma_qnx_rsmgr/usr/aarch64/$(BUILD_PROFILE_QNX_SO)

SHARED_LIBS += sharedmemallocator$(BUILD_PROFILE_QNX_SUFFIX)
SHARED_LIBS += tiipc-usr$(BUILD_PROFILE_QNX_SUFFIX)
SHARED_LIBS += tiudma-usr$(BUILD_PROFILE_QNX_SUFFIX)
SHARED_LIBS += ti-pdk$(BUILD_PROFILE_QNX_SUFFIX)
SHARED_LIBS += ti-sciclient$(BUILD_PROFILE_QNX_SUFFIX)
SHARED_LIBS += ti-udmalld$(BUILD_PROFILE_QNX_SUFFIX)
ifeq ($(TARGET_PLATFORM), AM62A)
SHARED_LIBS += screen c++ m
endif

endif # ifeq ($(TARGET_OS), QNX)

TIOVX_LIBS  =
IMAGING_LIBS =
VISION_APPS_SRV_LIBS  =
VISION_APPS_SAMPLE_LIBS  =
VISION_APPS_OPENGL_UTILS_LIBS =
VISION_APPS_KERNELS_LIBS  =
VISION_APPS_MODULES_LIBS  =
TEST_LIBS =
PTK_LIBS =
VISION_APPS_STEREO_LIBS =
VISION_APPS_UTILS_LIBS  =

# This section is for apps to link against tivision_apps library instead of static libs
ifeq ($(LINK_SHARED_OBJ)$(TARGETTYPE),yesexe)

#$(info $(TARGET) links against libtivision_apps.so)

SHARED_LIBS += tivision_apps

# This section is for apps to link against static libs instead of tivision_apps library
# Also used to create tivision_apps library (so we can maintain lib list in one place
else   # ifeq ($(LINK_SHARED_OBJ),yes)

LDIRS       += $(VISION_APPS_PATH)/out/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(APP_UTILS_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(VIDEO_IO_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(TIOVX_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(IMAGING_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(ETHFW_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(PTK_PATH)/lib/$(TARGET_PLATFORM)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS       += $(TIDL_PATH)/arm-tidl/tiovx_kernels/lib/$(TARGET_PLATFORM)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
ifeq ($(TARGET_OS), LINUX)
LDIRS       += $(LINUX_FS_PATH)/usr/lib
endif
ifeq ($(TARGET_OS), QNX)
LDIRS       += $(QNX_HOST)/usr/lib
endif


TIOVX_LIBS += vx_vxu vx_framework
TIOVX_LIBS += vx_kernels_host_utils vx_kernels_target_utils
TIOVX_LIBS += vx_platform_board_hlos
TIOVX_LIBS += vx_kernels_openvx_core
TIOVX_LIBS += vx_kernels_openvx_ext vx_target_kernels_openvx_ext
TIOVX_LIBS += vx_utils
TIOVX_LIBS += vx_tutorial
TIOVX_LIBS += vx_kernels_video_io
TIOVX_LIBS += vx_kernels_hwa 
TIOVX_LIBS += vx_kernels_tidl vx_nested_kernels_tidl
TIOVX_LIBS += vx_kernels_tvm
ifeq ($(TARGET_PLATFORM)$(TARGET_OS), AM62AQNX)
ifeq ($(TISDK_IMAGE), edgeai)
TIOVX_LIBS+=vx_target_kernels_capture
endif
endif

IMAGING_LIBS += app_utils_iss

ifeq ($(TARGET_PLATFORM), AM62A)
ifeq ($(TISDK_IMAGE), adas)
IMAGING_LIBS += vx_kernels_imaging
endif
else ifneq ($(TARGET_PLATFORM), AM62A)
IMAGING_LIBS += vx_kernels_imaging
endif

ifeq ($(TARGET_OS), LINUX)
IMAGING_LIBS += ti_imaging_aealg
IMAGING_LIBS += ti_imaging_awbalg

ifeq ($(SOC), am62a)
ifeq ($(TISDK_IMAGE), edgeai)
IMAGING_LIBS += ti_2a_wrapper
endif
endif

IMAGING_LIBS += ti_imaging_ittsrvr
IMAGING_LIBS += app_utils_network_api
IMAGING_LIBS += app_utils_itt_server
endif

ifeq ($(TARGET_PLATFORM)$(TARGET_OS), AM62AQNX)
ifeq ($(TISDK_IMAGE), edgeai)
IMAGING_LIBS = vx_target_kernels_imaging_aewb
IMAGING_LIBS += ti_2a_wrapper
IMAGING_LIBS += ti_imaging_aealg
IMAGING_LIBS += ti_imaging_awbalg
IMAGING_LIBS += ti_imaging_dcc
IMAGING_LIBS += ti_imaging_ittsrvr
IMAGING_LIBS += ti_imaging_sensordrv
endif
endif

VISION_APPS_UTILS_LIBS += app_utils_mem
VISION_APPS_UTILS_LIBS += app_utils_ipc
VISION_APPS_UTILS_LIBS += app_utils_console_io
VISION_APPS_UTILS_LIBS += app_utils_timer
VISION_APPS_UTILS_LIBS += app_utils_file_io
VISION_APPS_UTILS_LIBS += app_utils_remote_service
VISION_APPS_UTILS_LIBS += app_utils_perf_stats
ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72))
ifeq ($(CTOOLS_BUILD_ENABLED), yes)
VISION_APPS_UTILS_LIBS += app_utils_ctools
endif
endif

ifneq ($(TARGET_PLATFORM), AM62A)
VISION_APPS_UTILS_LIBS += app_utils_grpx
VISION_APPS_UTILS_LIBS += app_utils_draw2d
endif

ifeq ($(TARGET_PLATFORM)$(TARGET_OS), AM62AQNX)
ifeq ($(TISDK_IMAGE), edgeai)
VISION_APPS_UTILS_LIBS += app_utils_udma
VISION_APPS_UTILS_LIBS += app_utils_sensors
VISION_APPS_UTILS_LIBS += app_utils_iss
VISION_APPS_UTILS_LIBS += app_utils_grpx
VISION_APPS_UTILS_LIBS += app_utils_draw2d
endif
endif

ifeq ($(TARGET_PLATFORM), AM62A)
ifeq ($(TISDK_IMAGE), adas)
VISION_APPS_UTILS_LIBS += app_utils_grpx
VISION_APPS_UTILS_LIBS += app_utils_draw2d
endif
endif

VISION_APPS_UTILS_LIBS += app_utils_hwa
VISION_APPS_UTILS_LIBS += app_utils_init

VISION_APPS_OPENGL_UTILS_LIBS += app_utils_opengl

VISION_APPS_SAMPLE_LIBS  += vx_kernels_sample vx_target_kernels_sample_a72

VISION_APPS_SRV_LIBS  += vx_kernels_srv vx_target_kernels_srv_gpu
VISION_APPS_SRV_LIBS  += vx_applib_srv_bowl_lut_gen
VISION_APPS_SRV_LIBS  += vx_applib_srv_calibration
VISION_APPS_SRV_LIBS  += vx_srv_render_utils

VISION_APPS_KERNELS_LIBS += vx_kernels_img_proc
VISION_APPS_KERNELS_LIBS += vx_target_kernels_img_proc_a72
VISION_APPS_KERNELS_LIBS += vx_kernels_fileio
VISION_APPS_KERNELS_LIBS += vx_target_kernels_fileio

# vx_app_modules not used for AM62A EdgeAI Linux
ifneq ($(TARGET_PLATFORM),AM62A)
VISION_APPS_MODULES_LIBS += vx_app_modules
else
ifneq ($(TISDK_IMAGE),edgeai)
VISION_APPS_MODULES_LIBS += vx_app_modules
else
ifneq ($(TARGET_OS),LINUX)
VISION_APPS_MODULES_LIBS += vx_app_modules
endif
endif
endif

ifneq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), AM62A))
VISION_APPS_STEREO_LIBS += vx_kernels_common
VISION_APPS_STEREO_LIBS += vx_kernels_stereo
VISION_APPS_STEREO_LIBS += vx_target_kernels_stereo
endif

PTK_LIBS += ptk_base
PTK_LIBS += ptk_algos

TEST_LIBS += vx_tiovx_tests vx_tiovx_internal_tests vx_conformance_tests vx_conformance_engine vx_conformance_tests_testmodule
TEST_LIBS += vx_kernels_openvx_ext_tests
TEST_LIBS += vx_kernels_test_kernels_tests vx_kernels_test_kernels
TEST_LIBS += vx_target_kernels_source_sink vx_kernels_hwa_tests

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721E J722S J721S2 J784S4 J742S2))
TEST_LIBS += vx_target_kernels_vpac_aewb
endif
TEST_LIBS += vx_kernels_video_io_tests
TEST_LIBS += vx_tiovx_tidl_tests

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721E J721S2 J784S4 J742S2))
TEST_LIBS += vx_tiovx_tvm_tests
TEST_LIBS += vx_kernels_srv_tests
TEST_LIBS += vx_applib_tests
endif

STATIC_LIBS += $(TIOVX_LIBS)
STATIC_LIBS += $(VISION_APPS_UTILS_LIBS)
ifeq ($(TARGET_OS),LINUX)
STATIC_LIBS += app_rtos_linux_mpu1_common
endif
ifeq ($(TARGET_OS),QNX)
STATIC_LIBS += app_rtos_qnx_mpu1_common app_utils_rtos
ifneq ($(QNX_SDP_VERSION),800)
STATIC_LIBS += c++fs
endif
endif

ifeq ($(TARGET_OS),LINUX)
SYS_SHARED_LIBS += stdc++ m rt pthread ti_rpmsg_char
endif
ifeq ($(TARGET_OS),QNX)
SHARED_LIBS += c++ m
endif

endif  # ifeq ($(LINK_SHARED_OBJ),yes)


endif  # ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))

