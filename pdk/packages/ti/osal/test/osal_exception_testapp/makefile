include $(PDK_INSTALL_PATH)/ti/build/Rules.make

#Name of the directory created under packages/ti/binary/
APP_NAME = osal_exception_testapp_$(BUILD_OS_TYPE)

PACKAGE_SRCS_COMMON = . ../src

SRCDIR = . ../src ../
# Common source files across all platforms and cores
SRCS_COMMON += osal_exception_testapp.c

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
EXTERNAL_INTERFACES = freertos
COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
EXTERNAL_INTERFACES = safertos
COMP_LIST_COMMON    = $(PDK_COMMON_SAFERTOS_COMP)
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OSAL_UT) $(CFLAGS_OS_DEFINES)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk


