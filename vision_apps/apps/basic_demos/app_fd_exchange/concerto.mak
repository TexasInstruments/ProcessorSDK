ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 A72 A53))
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX QNX))

_MODULE=producer
# Producer executable
include $(PRELUDE)

TARGET      := vx_app_arm_fd_exchange_producer
TARGETTYPE  := exe
CSOURCES    := main_producer.c apputils_net.c app_common.c

# Use this for all devices that use io-sock
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), QNX))
ifeq ($(SOC),j722s)
IDIRS += $(QNX_TARGET)/usr/include/io-sock
LDIRS +=$(QNX_TARGET)/aarch64le/io-sock/usr/lib/
LDIRS +=$(QNX_TARGET)/aarch64le/io-sock/lib/
endif
SYS_SHARED_LIBS += socket
endif

ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak
endif

ifeq ($(TARGET_CPU), x86_64)
include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak
endif

include $(FINALE)

_MODULE=consumer
# Consumer executable
include $(PRELUDE)

TARGET      := vx_app_arm_fd_exchange_consumer
TARGETTYPE  := exe
CSOURCES    := main_consumer.c apputils_net.c app_common.c

# Use this for all devices that use io-sock
ifeq ($(TARGET_OS),$(filter $(TARGET_OS), QNX))
ifeq ($(SOC),j722s)
IDIRS += $(QNX_TARGET)/usr/include/io-sock
LDIRS +=$(QNX_TARGET)/aarch64le/io-sock/usr/lib/
LDIRS +=$(QNX_TARGET)/aarch64le/io-sock/lib/
endif
SYS_SHARED_LIBS += socket
endif

ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak
endif

ifeq ($(TARGET_CPU), x86_64)
include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak
endif

include $(FINALE)

endif
endif
