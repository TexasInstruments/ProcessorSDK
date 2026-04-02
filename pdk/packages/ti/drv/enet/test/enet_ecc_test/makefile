#
# Ecc test (TI-RTOS/FREERTOS/SAFERTOS) makefile
#

include $(PDK_INSTALL_PATH)/ti/drv/enet/examples/build/config.mk

APP_NAME = enet_ecc_test
SRCDIR += . ../common
INCDIR += utils/include

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON =  makefile enet_ecc_test.c

SDL_INSTALL_PATH=$(PDK_INSTALL_PATH)/../../sdl
INCDIR += $(SDL_INSTALL_PATH)/include
INCDIR += $(SDL_INSTALL_PATH)/src/sdl
INCDIR += $(SDL_INSTALL_PATH)
INCDIR += $(SDL_INSTALL_PATH)/include/soc/$(SOC)

EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/sdl/lib/$(SOC)/$(ISA)/$(BUILD_PROFILE_$(CORE))/sdl_api.aer5f
EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/src/ip/lib/$(SOC)/$(ISA)/$(BUILD_PROFILE_$(CORE))/sdl_ip.aer5f
EXT_LIB_LIST_COMMON += $(SDL_INSTALL_PATH)/binary/osal/lib/$(SOC)/$(ISA)/$(BUILD_PROFILE_$(CORE))/sdl_osal.aer5f

SRCS_COMMON += $(SDL_INSTALL_PATH)/test/osal/src/osal_interface.c

# No RTOS for this test
ENET_CFLAGS += -DBAREMETAL

# List all the components required by the application
COMP_LIST_COMMON += enet enetsoc enetphy enet_example_utils_baremetal

# Source files
SRCS_COMMON += enet_ecc_test.c

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

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
