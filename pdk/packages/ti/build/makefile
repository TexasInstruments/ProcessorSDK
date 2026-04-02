
PDK_INSTALL_PATH ?= $(abspath ../../)

include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(LIMIT_BOARDS),)
BOARD_LIST_ALL += $(BOARD_LIST_J6_TDA)
BOARD_LIST_ALL += $(BOARD_LIST_J7_TDA)
else
  # If LIMIT_BOARDS is defined use it
  BOARD_LIST_ALL = $(LIMIT_BOARDS)
endif

ifeq ($(LIMIT_CORES),)
  #Limit core list based on board
  ifeq ($(BOARD),$(filter $(BOARD), j721e_evm))
    CORE_LIST_ALL = $(CORE_LIST_j721e)
  endif
  ifeq ($(BOARD),$(filter $(BOARD), j7200_evm))
    CORE_LIST_ALL = $(CORE_LIST_j7200)
  endif
  ifeq ($(BOARD),$(filter $(BOARD), j721s2_evm))
    CORE_LIST_ALL = $(CORE_LIST_j721s2)
  endif
  ifeq ($(BOARD),$(filter $(BOARD), j784s4_evm))
    CORE_LIST_ALL = $(CORE_LIST_j784s4)
  endif
  ifeq ($(BOARD),$(filter $(BOARD), j742s2_evm))
    CORE_LIST_ALL = $(CORE_LIST_j742s2)
  endif
  ifeq ($(BOARD),$(filter $(BOARD), $(BOARD_LIST_J6_TDA)))
    CORE_LIST_ALL =c66xdsp_1 c66xdsp_2
    CORE_LIST_ALL += mcu1_0 mcu1_1 mpu1_0 mpu1_1
  endif
else
  # If LIMIT_CORES is defined use it
  CORE_LIST_ALL = $(LIMIT_CORES)
endif

BUILD_PROFILE_LIST_ALL ?= release debug

# Variables used


#if COMP is not defined, the build comprises of all modules in the ti/build/component.mk
ifeq ($(COMP),)

FINAL_LIB_LIST = $(pdk_LIB_LIST)
FINAL_APP_LIB_LIST = $(pdk_APP_LIB_LIST)
FINAL_EXAMPLE_LIST = $(pdk_EXAMPLE_LIST)
FINAL_FIRM_LIST = $(pdk_FIRM_LIST)
FINAL_PKG_LIST_ALL = $(pdk_PKG_LIST_ALL)
FINAL_DUP_EXAMPLE_LIST = $(pdk_DUP_EXAMPLE_LIST)
FINAL_UTILS_LIST_ALL = $(pdk_UTILS_LIST)
FINAL_EXAMPLE_LIST_INFO=$(pdk_EXAMPLE_LIST)

else
#if COMP is defined, this is built from within a component, eg) ti/drv/mmcsd> make all
# In this case only that module's LIB,APPLIB, EXAMPLES should be built

FINAL_LIB_LIST =$($(COMP)_LIB_LIST)
FINAL_APP_LIB_LIST = $($(COMP)_APP_LIB_LIST)
FINAL_EXAMPLE_LIST=$($(COMP)_EXAMPLE_LIST)
FINAL_FIRM_LIST = $($(COMP)_FIRM_LIST)
FINAL_DUP_EXAMPLE_LIST = $($(COMP)_DUP_EXAMPLE_LIST)
FINAL_PKG_LIST_ALL=$($(COMP)_LIB_LIST) $($(COMP)_APP_LIB_LIST) $($(COMP)_EXAMPLE_LIST) $($(COMP)_FIRM_LIST)
FINAL_UTILS_LIST_ALL = $($(COMP)_UTILS_LIST)
FINAL_EXAMPLE_LIST_INFO=$($(COMP)_EXAMPLE_LIST)

endif

# Consolidated list of targets to be built
FINAL_LIB_LIST_CLEAN = $(addsuffix _clean, $(FINAL_LIB_LIST))
FINAL_APP_LIB_LIST_CLEAN = $(addsuffix _clean, $(FINAL_APP_LIB_LIST))
FINAL_EXAMPLE_LIST_CLEAN = $(addsuffix _clean, $(FINAL_EXAMPLE_LIST))
FINAL_FIRM_LIST_CLEAN = $(addsuffix _clean, $(FINAL_FIRM_LIST))
FINAL_DUP_EXAMPLE_LIST_CLEAN = $(addsuffix _clean, $(FINAL_DUP_EXAMPLE_LIST))
FINAL_DUP_EXAMPLE_LIST_PACKAGE = $(addsuffix _package, $(FINAL_DUP_EXAMPLE_LIST))
FINAL_PKG_LIST_PACKAGE = $(addsuffix _package, $(FINAL_PKG_LIST_ALL))
FINAL_PKG_LIST_ALL_CLEAN = $(addsuffix _clean, $(FINAL_PKG_LIST_ALL))
FINAL_UTILS_LIST_CLEAN = $(addsuffix _clean, $(FINAL_UTILS_LIST_ALL))
FINAL_PKG_LIST_INFO = $(addsuffix _info, $(FINAL_EXAMPLE_LIST_INFO))
FINAL_DUP_EXAMPLE_INFO = $(addsuffix _info, $(FINAL_DUP_EXAMPLE_LIST))

# Create the _has_dep target for the <module> if _DEPENDS_ON is defined in the <module>_component.mk
# For example, CREATE_DEP_TARGETS(nimu_icssg,FINAL_LIB_LIST) does the following:
# 1) creates nimu_icssg_has_dep: emac_has_dep (as 'nimu_icssg_DEPENDS_ON = emac' is mentioned in nimu_component.mk)
# 2) FINAL_LIB_LIST_DEP_LIST += emac (This is to take emac out of the FINAL_LIB_LIST_HAS_DEP because it will be built by nimu_icssg_has_dep
#  However CREATE_DEP_TARGETS(mmcsd,FINAL_LIB_LIST) will create mmcsd_has_dep: (as 'mmcsd_DEPENDS_ON ' is not defined in mmcsd_component.mk)
# 3) Build the <module> after the dependency is built

define CREATE_DEP_TARGETS
ifneq ($($(1)_DEPENDS_ON),)
ifeq ($($(1)_DEPENDS_ON),$(filter $($(1)_DEPENDS_ON), $($(2))))
$(1)_has_dep: $($(1)_DEPENDS_ON)_has_dep
	+$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile $(1)

$(2)_DEP_LIST += $($(1)_DEPENDS_ON)
else
$(1)_has_dep:
	+$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile $(1)
endif
else
$(1)_has_dep:
	+$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile $(1)
endif
endef


# For all the targets to build, create the Dependency targets.
$(foreach LIB,$(FINAL_LIB_LIST),$(eval $(call CREATE_DEP_TARGETS,$(LIB),FINAL_LIB_LIST)))
$(foreach APP_LIB,$(FINAL_APP_LIB_LIST),$(eval $(call CREATE_DEP_TARGETS,$(APP_LIB),FINAL_APP_LIB_LIST)))
$(foreach EXAMPLE,$(FINAL_EXAMPLE_LIST),$(eval $(call CREATE_DEP_TARGETS,$(EXAMPLE),FINAL_EXAMPLE_LIST)))
$(foreach DUP_EXAMPLE,$(FINAL_DUP_EXAMPLE_LIST),$(eval $(call CREATE_DEP_TARGETS,$(DUP_EXAMPLE),FINAL_DUP_EXAMPLE_LIST)))
$(foreach FIRM,$(FINAL_FIRM_LIST),$(eval $(call CREATE_DEP_TARGETS,$(FIRM),FINAL_FIRM_LIST)))
$(foreach UTIL,$(FINAL_UTILS_LIST_ALL),$(eval $(call CREATE_DEP_TARGETS,$(UTIL),FINAL_UTILS_LIST_ALL)))

# Create *_has_dep targets , filtering out the duplicate entries (which already appeared as dependent targets to others
# For example nimu_icssg_has_dep already builds emac (nimu_icssg_DEPENDS_ON = emac)
FINAL_LIB_HAS_DEP_LIST = $(addsuffix _has_dep, $(filter-out $(FINAL_LIB_LIST_DEP_LIST), $(FINAL_LIB_LIST)))
FINAL_APP_LIB_HAS_DEP_LIST = $(addsuffix _has_dep,$(filter-out $(FINAL_APP_LIB_LIST_DEP_LIST),$(FINAL_APP_LIB_LIST)))
FINAL_EXAMPLE_HAS_DEP_LIST = $(addsuffix _has_dep, $(filter-out $(FINAL_EXAMPLE_LIST_DEP_LIST),$(FINAL_EXAMPLE_LIST)))
FINAL_DUP_EXAMPLE_HAS_DEP_LIST = $(addsuffix _has_dep,$(filter-out $(FINAL_DUP_EXAMPLE_LIST_DEP_LIST),$(FINAL_DUP_EXAMPLE_LIST)))
FINAL_FIRM_HAS_DEP_LIST = $(addsuffix _has_dep, $(filter-out $(FINAL_FIRM_LIST_DEP_LIST),$(FINAL_FIRM_LIST)))
FINAL_SAFERTOS_EXAMPLE_HAS_DEP_LIST = $(filter %safertos_has_dep, $(FINAL_EXAMPLE_HAS_DEP_LIST))
FINAL_SAFERTOS_EXAMPLE_HAS_DEP_LIST += $(filter %safertos_has_dep, $(FINAL_DUP_EXAMPLE_HAS_DEP_LIST))
FINAL_UTILS_LIST_ALL_HAS_DEP_LIST = $(addsuffix _has_dep, $(filter-out $(FINAL_UTILS_LIST_ALL_DEP_LIST),$(FINAL_UTILS_LIST_ALL)))


.PHONY : all all_libs all_apps all_apps_core version pdk_examples pdk_libs lib app_lib apps tar doxygen pdk_libs_clean pdk_app_libs pdk_app_libs_clean examples examples_clean clean package $(pdk_PKG_LIST_ALL) $(pdk_DUP_EXAMPLE_LIST) all_apps pdk_examples pdk_dup_apps $(FINAL_LIB_HAS_DEP_LIST) $(FINAL_APP_LIB_HAS_DEP_LIST) $(FINAL_EXAMPLE_HAS_DEP_LIST)    $(FINAL_DUP_EXAMPLE_HAS_DEP_LIST) $(FINAL_FIRM_HAS_DEP_LIST) %_has_dep $(FINAL_UTILS_LIST_ALL) $(FINAL_PKG_LIST_INFO) $(FINAL_DUP_EXAMPLE_INFO)

all: version all_apps

all_apps: all_libs
	+$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile all_apps_core

all_libs: pdk_libs pdk_app_libs

all_apps_core: pdk_examples
	$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile pdk_dup_apps

version:
	$(ECHO) ------------------------------------------------------
	$(ECHO) \# PDK MM.NN.PP.BB
	$(ECHO) ------------------------------------------------------

pdk_libs: $(FINAL_LIB_HAS_DEP_LIST)

pdk_libs_clean: $(FINAL_LIB_LIST_CLEAN)

pdk_app_libs: $(FINAL_APP_LIB_HAS_DEP_LIST)

pdk_examples: examples

pdk_dup_apps: $(FINAL_DUP_EXAMPLE_HAS_DEP_LIST)

pdk_app_libs_clean: $(FINAL_APP_LIB_LIST_CLEAN)

examples: $(FINAL_EXAMPLE_HAS_DEP_LIST)

safertos_examples: $(FINAL_SAFERTOS_EXAMPLE_HAS_DEP_LIST)

examples_clean: $(FINAL_EXAMPLE_LIST_CLEAN)

examples_info: $(FINAL_PKG_LIST_INFO) $(FINAL_DUP_EXAMPLE_INFO)

clean: pdk_libs_clean pdk_app_libs_clean examples_clean
	$(foreach dup_app, $(FINAL_DUP_EXAMPLE_LIST_CLEAN),\
	$(MAKE) $(dup_app);\
	)

allclean: destroot_clean all_libs_clean

destroot_clean:
ifneq ($(DEST_ROOT),)
	$(RM) -rf $(DEST_ROOT)
endif

all_libs_clean:
	$(RM) -rf ../*/lib
	$(RM) -rf ../*/*/lib
	$(RM) -rf ../*/*/*/lib
	$(RM) -rf ../*/*/*/*/lib
	$(RM) -rf ../drv/emac/firmware/icss_dualmac/bin/
	$(RM) -rf ../boot/sbl/binary
	$(RM) -rf ../boot/sbl/example/boot_app/binary

package: $(FINAL_PKG_LIST_PACKAGE) $(FINAL_DUP_EXAMPLE_LIST_PACKAGE)

help:
	$(ECHO) ------------------------------------------------------
	$(ECHO) \# PDK make help
	$(ECHO) ------------------------------------------------------
	$(ECHO) make -s [OPTIONAL MAKE VARIABLES] Note: use gmake for windows
	$(ECHO)
	$(ECHO) "Supported targets:"
	$(ECHO) "------------------"
	$(ECHO) "all            : Builds all libraries and examples for the provided CORE and BOARD"
	$(ECHO) "allcores       : Builds all libraries and examples for all applicable CORES for a BOARD"
	$(ECHO) "clean          : Cleans all libraries and examples for the provided CORE and BOARD"
	$(ECHO) "allcores_clean : Cleans all libraries and examples for all applicable CORES for a BOARD"
	$(ECHO) "allclean       : Removes the binary directory using rm -rf"
	$(ECHO) "pdk_examples   : Builds all examples"
	$(ECHO) "pdk_libs       : Builds all libraries"
	$(ECHO) "pdk_app_libs   : Builds all application utility libaries"
	$(ECHO) "custom_target  : Builds the target list provided by BUILD_TARGET_LIST_ALL= for all cores and profiles"
	$(ECHO) "<Module>       : Builds a module. Possible values:"
	$(ECHO) "[$(FINAL_LIB_LIST)]"
	$(ECHO) ""
	$(ECHO) "<Module_App_lib> : Builds application libraries:"
	$(ECHO) "[$(FINAL_APP_LIB_LIST)]"
	$(ECHO) ""
	$(ECHO) "<Module_App>   : Builds an application. Possible values:"
	$(ECHO) "[$(FINAL_EXAMPLE_LIST) $(FINAL_DUP_EXAMPLE_LIST)]"
	$(ECHO) ""
	$(ECHO) "<Utils>        : Runs utilities which generate code. Possible values:"
	$(ECHO) "[$(FINAL_UTILS_LIST_ALL) ]"
	$(ECHO) ""
	$(ECHO) "Optional make variables:"
	$(ECHO) "------------------------"
	$(ECHO) "BOARD=[$(BOARD_LIST_ALL)]"
	$(ECHO) "    Default: $(BOARD)"
	$(ECHO) "CORE=[$(CORE_LIST_ALL)]"
	$(ECHO) "    Default: $(CORE)"
	$(ECHO) "BUILD_PROFILE=[$(BUILD_PROFILE_LIST_ALL)]"
	$(ECHO) "    Default: $(BUILD_PROFILE)"
	$(ECHO) "OS=[Windows_NT linux]"
	$(ECHO) "    Default: Windows_NT"
	$(ECHO) "    COMP = $(COMP)"
	$(ECHO) "    PDK_INSTALL_PATH = $(PDK_INSTALL_PATH)"

allcores:
	$(foreach current_core, $(CORE_LIST_ALL),\
	$(MAKE) all CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(BUILD_PROFILE);\
	)

allcores_clean:
	$(foreach current_core, $(CORE_LIST_ALL),\
	$(MAKE) clean CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(BUILD_PROFILE);\
	)

allcores_package:
	$(foreach current_core, $(CORE_LIST_ALL),\
	$(MAKE) package CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(BUILD_PROFILE);\
	)

allboards:
	$(foreach current_board, $(BOARD_LIST_ALL),\
	$(MAKE) allcores BOARD=$(current_board) BUILD_PROFILE=$(BUILD_PROFILE);\
	)

allboards_clean:
	$(foreach current_board, $(BOARD_LIST_ALL),\
	$(MAKE) allcores_clean BOARD=$(current_board) BUILD_PROFILE=$(BUILD_PROFILE);\
	)

profiles:
	$(foreach current_profile, $(BUILD_PROFILE_LIST_ALL),\
	$(MAKE) allcores BOARD=$(BOARD) BUILD_PROFILE=$(current_profile);\
	)

profiles_clean:
	$(foreach current_profile, $(BUILD_PROFILE_LIST_ALL),\
	$(MAKE) allcores_clean BOARD=$(BOARD) BUILD_PROFILE=$(current_profile);\
	)

pdk_libs_allcores:
	$(foreach current_core, $(CORE_LIST_ALL),\
	$(MAKE) pdk_libs CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(BUILD_PROFILE);\
	)

pdk_libs_allcores_clean:
	$(foreach current_core, $(CORE_LIST_ALL),\
	$(MAKE) pdk_libs_clean CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(BUILD_PROFILE);\
	)

pdk_libs_profiles:
	$(foreach current_profile, $(BUILD_PROFILE_LIST_ALL),\
	$(MAKE) pdk_libs_allcores BOARD=$(BOARD) BUILD_PROFILE=$(current_profile);\
	)

pdk_libs_profiles_clean:
	$(foreach current_profile, $(BUILD_PROFILE_LIST_ALL),\
	$(MAKE) pdk_libs_allcores_clean BOARD=$(BOARD) BUILD_PROFILE=$(current_profile);\
	)

custom_target:
	$(foreach current_profile, $(BUILD_PROFILE_LIST_ALL),\
	   	$(foreach current_core, $(CORE_LIST_ALL),\
			$(foreach current_build_target, $(BUILD_TARGET_LIST_ALL),\
				$(MAKE) $(current_build_target) CORE=$(current_core) BOARD=$(BOARD) BUILD_PROFILE_$(current_core)=$(current_profile);\
	)))

allall:
	$(foreach current_board, $(BOARD_LIST_ALL),\
	$(MAKE) profiles BOARD=$(current_board);\
	)

allall_clean:
	$(foreach current_board, $(BOARD_LIST_ALL),\
	$(MAKE) profiles_clean BOARD=$(current_board);\
	)

lib: pdk_libs

apps: examples
app_libs: pdk_app_libs

doxygen:
ifeq ($(DOXYGEN_SUPPORT),yes)
	$(ECHO) Creating Doxygen API guide ...
	@doxygen docs/Doxyfile
else
	$(ECHO) No Doxygen Support available ...
endif

clean_doxygen:
ifeq ($(DOXYGEN_SUPPORT),yes)
	$(ECHO) cleaned Doxygen API guide ...
	$(RM) -rf docs/doxygen
else
	$(ECHO) No Doxygen Support available ...
endif

#=================================================================
#All PDKs libs and tests
$(FINAL_PKG_LIST_ALL) $(FINAL_DUP_EXAMPLE_LIST):
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($@_SOCLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(MAKE) -C $($@_PATH) $($@_MAKEFILE),$(ECHO) Nothing to be done for $(SOC) $(CORE) $@),\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($@_BOARDLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(MAKE) -C $($@_PATH) $($@_MAKEFILE),$(ECHO) Nothing to be done for $(BOARD) $(SOC) $(CORE) $@),$(ECHO) Nothing to be done for $(SOC) $@))
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($@_SOCLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_SBL_IMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) sbl_imagegen,),),\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($@_BOARDLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_SBL_IMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) sbl_imagegen,),),))
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($@_SOCLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_SBL_APPIMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) sbl_appimagegen,),),\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($@_BOARDLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_SBL_APPIMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) sbl_appimagegen,),),))
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($@_SOCLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_KEYWR_IMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) keywr_imagegen,),),\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($@_BOARDLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(if $(filter yes, $(subst emptyreplacement,,$($@_KEYWR_IMAGEGEN))),\
	            $(MAKE) -C $($@_PATH) $($@_MAKEFILE) keywr_imagegen,),),))


$(FINAL_PKG_LIST_ALL_CLEAN) $(FINAL_DUP_EXAMPLE_LIST_CLEAN):
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($(subst _clean,,$@)_SOCLIST))),\
	        $(MAKE) -C $($(subst _clean,,$@)_PATH) $($(subst _clean,,$@)_MAKEFILE) clean,\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($(subst _clean,,$@)_BOARDLIST))),\
	        $(MAKE) -C $($(subst _clean,,$@)_PATH) $($(subst _clean,,$@)_MAKEFILE) clean,))

$(FINAL_UTILS_LIST_ALL):
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($@_SOCLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(MAKE) -C $($@_PATH) $($@_MAKEFILE),$(ECHO) Nothing to be done for $(SOC) $(CORE) $@),\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($@_BOARDLIST))),\
	    $(if $(filter $(CORE), $(subst emptyreplacement,,$($@_$(SOC)_CORELIST))),\
	        $(MAKE) -C $($@_PATH) $($@_MAKEFILE),$(ECHO) Nothing to be done for $(BOARD) $(SOC) $(CORE) $@),$(ECHO) Nothing to be done for $(SOC) $@))

$(FINAL_PKG_LIST_PACKAGE) $(FINAL_DUP_EXAMPLE_LIST_PACKAGE):
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($(subst _package,,$@)_SOCLIST))),\
	        $(MAKE) -C $($(subst _package,,$@)_PATH) $($(subst _package,,$@)_MAKEFILE) package,\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($(subst _package,,$@)_BOARDLIST))),\
	        $(MAKE) -C $($(subst _package,,$@)_PATH) $($(subst _package,,$@)_MAKEFILE) package,$(ECHO) Nothing to be done for $(SOC) $(subst _package,,$@)))

$(FINAL_PKG_LIST_INFO) $(FINAL_DUP_EXAMPLE_INFO):
	$(if $(filter $(SOC), $(subst emptyreplacement,,$($(subst _info,,$@)_SOCLIST))),\
	        $(ECHO) $(subst _info,,$@) Applicable Cores: $($(subst _info,,$@)_$(SOC)_CORELIST) ,\
	$(if $(filter $(BOARD), $(subst emptyreplacement,,$($(subst _info,,$@)_BOARDLIST))),\
	        $(ECHO) $(subst _info,,$@) Applicable Cores: $($(subst _info,,$@)_$(SOC)_CORELIST) ,))

#Below is used only for checking c++ build errors during development, not to be used for any other purpose
cplusplus_build:
	+$(MAKE) -f $(PDK_INSTALL_PATH)/ti/build/makefile all BUILD_PROFILE=debug CPLUSPLUS_BUILD=yes

# Nothing beyond this point