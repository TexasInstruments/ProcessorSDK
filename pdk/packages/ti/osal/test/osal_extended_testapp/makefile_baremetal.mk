#
# Makefile for OSAL extended baremetal test app.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#App Name
APP_NAME = osal_baremetal_extended_testapp
PACKAGE_SRCS_COMMON = src osal_extended_test.h makefile_baremetal.mk
BUILD_OS_TYPE = baremetal

SRCDIR = src
INCDIR = . ../src
SRCS_COMMON += osal_extended_testapp.c
SRCS_COMMON += osal_extended_testapp_hwi.c osal_extended_testapp_mutex.c osal_extended_testapp_cache.c
SRCS_COMMON += osal_extended_testapp_semaphore.c osal_extended_testapp_utils.c osal_extended_testapp_cycleProfiler.c
SRCS_COMMON += osal_extended_testapp_registerinterrupt.c osal_extended_testapp_timer.c

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl_arch

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DBARE_METAL

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
