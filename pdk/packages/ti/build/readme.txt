
Steps to build
==============

- Go to <PDK_INSTALL_PATH>/ti/build folder

- Open command prompt and type the below
  gmake -s all

  This will clean all and build all PDK drivers as well as all PDK
  examples

- TIP: Always use "-s" option with make to see user friendly prints
       during build.

- Optionally before build, edit the below in Rules.make file,
  depending on the kind of build you want to do:

  BOARD:=
  BUILD_PROFILE_$(CORE)?=

  Valid values for BOARD are:
  j721e_evm             - J721E EVM build
  j7200_evm             - J7200 EVM build
  j721s2_evm            - J721S2 EVM build
  j784s4_evm            - J784S4 EVM build
  j742s2_evm            - J742S2 EVM build

  Valid values for BUILD_PROFILE_$(CORE) are:
  debug                 - Debug build with NO compiler optimizations
                          and symbolic debug info included
  release               - Release build with FULL compiler optimizations
                          and symbolic debug info included

- Include or Exclude Components
  User can include or exclude a component build by changing variables like below
  in Rules.make. This could be used to disable any reference to a component when
  a particular component is not present or cloned.

  PDK_PM_INCLUDE = yes
  PDK_RTOS_I2C_INCLUDE = yes
  PDK_OSAL_INCLUDE = yes
  PDK_VPS_INCLUDE = yes

- PACKAGE_SELECT (Supported values: all, vps-hal-only, vps-vip-only, vps-vpe-only, vps-vip-dss, vps-vip-vpe)
  all: All libraries are built
  vps-hal-only: Only VPS HAL and common libraries are built
  vps-vip-only: Only VIP libraries are built
  vps-vpe-only: Only VPE libraries are built
  vps-dss-only: Only DSS libraries are built
  vps-vip-dss : VIP and DSS libraries are built
  vps-vip-vpe : VIP and VPE libraries are built

- The generated files are placed in the below folders,
  Executables   - $(DEST_ROOT)/<APP_NAME>/bin/$(BOARD)/<APP_NAME>_$(CORE)_$(BUILD_PROFILE).xe$(ISA)
  Libraries     - $(DEST_ROOT)/<MODULE_RELPATH>/lib/$(BOARD)/$(ISA)/$(BUILD_PROFILE)/*.ae$(ISA)
  Objects       - $(DEST_ROOT)/<MODULE_RELPATH>/obj/$(BOARD)/$(ISA)/$(BUILD_PROFILE)/*.oe$(ISA)
  Dependancies  - $(DEST_ROOT)/<APP_NAME>/obj/$(BOARD)/$(CORE)/$(BUILD_PROFILE)/.deps/*.d

  Make sure to do a clean build while doing the above as it may not recompile the
  PDK libarary again.

- During development the below "gmake" targets can be also used be used for
  convenience:

  gmake -s examples         - incrementally builds PDK drivers
                              and all examples

  gmake -s platforms        - incrementally builds for all supported BOARDs
  gmake -s profiles         - incrementally builds for all supported BUILD_PROFILEs

  gmake -s all              - clean all and build all drivers, examples
  gmake -s allall           - incrementally builds for all supported BOARDs
                              AND BUILD_PROFILEs

  gmake -s clean            - clean all drivers, examples
  gmake -s examplesclean    - clean all examples ONLY
  gmake -s platformsclean   - cleans for all supported BOARDs
  gmake -s profilesclean    - cleans for all supported BUILD_PROFILEs
  gmake -s allclean         - cleans for all supported BOARDs AND BUILD_PROFILEs

  gmake -s <examplename>    - incrementally builds PDK drivers
                              and the specific example ONLY.
