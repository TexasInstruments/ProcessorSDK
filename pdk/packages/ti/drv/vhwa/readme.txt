Steps to build
==============

- Open makefile

- Edit paths for the below variables according to your directory structure for SDK_install and PDK_install
    It should be something like this:
        SDK_INSTALL_PATH: ~/work_area/SOC
        PDK_INSTALL_PATH: ~/work_area/SOC/pdk/packages

IMPORTANT:
    Make sure the paths specified above DO NOT have any spaces in them.

- Make sure "gmake" is in your system path.
    Typically when XDC is installed "gmake" is installed along with it and it becomes part of the system path by default.

- Open command prompt and type the below
    make allclean

    This will clean all pdk drivers and vhwa drivers

- Build vhwa drivers for a specific SOC, BOARD, CORE and build profile

    make vhwa BOARD=j721s2_evm SOC=j721s2 CORE=mcu2_0 BUILD_PROFILE=debug -sj12

- Build a specific example

    make vhwa_msc_baremetal_testapp BOARD=j721s2_evm SOC=j721s2 CORE=mcu2_0 BUILD_PROFILE=debug -sj12
