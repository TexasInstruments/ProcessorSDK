#
# Makefile for OSAL extended rtos test app.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#App Name
APP_NAME = osal_extended_testapp_$(BUILD_OS_TYPE)

PACKAGE_SRCS_COMMON = src osal_extended_test.h makefile_rtos.mk

SRCDIR = src
INCDIR = . ../src ../../src/safertos
# Common source files across all platforms and cores
SRCS_COMMON += osal_extended_testapp.c
SRCS_COMMON += osal_extended_testapp_hwi.c osal_extended_testapp_mutex.c osal_extended_testapp_cache.c
SRCS_COMMON += osal_extended_testapp_mailbox.c osal_extended_testapp_task.c osal_extended_testapp_semaphore.c
SRCS_COMMON += osal_extended_testapp_event.c osal_extended_c7x_cslarch.c osal_extended_testapp_utils.c
SRCS_COMMON += osal_extended_testapp_timer.c

ifneq ($(CORE),$(filter $(CORE), c66xdsp_1 c66xdsp_2))
SRCS_COMMON += osal_extended_testapp_clock.c osal_extended_testapp_registerinterrupt.c
endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
SRCS_COMMON += osal_extended_testapp_queue.c osal_extended_testapp_heap.c osal_extended_testapp_memory.c
SRCS_COMMON += osal_extended_testapp_load.c
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
EXTERNAL_INTERFACES = safertos
COMP_LIST_COMMON    = $(PDK_COMMON_SAFERTOS_COMP)
ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x_2 c7x_3 c7x_4 c66xdsp_1 c66xdsp_2))
SRCS_COMMON += osal_extended_testapp_archutils.c
endif
endif

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x_2 c7x_3 c7x_4))
SRCS_COMMON += osal_extended_testapp_portable.c
endif
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
