include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_helloworld_example
SRCDIR += . ../common
INCDIR += utils/include

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON =  makefile enet_helloworld_example.c

# No RTOS for this example
ENET_CFLAGS += -DBAREMETAL

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_baremetal

# Source files
SRCS_COMMON += enet_helloworld_example.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES += pdk
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(ENET_CFLAGS)

# Include common make files
ifeq ($(MAKERULEDIR), )
  #Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk