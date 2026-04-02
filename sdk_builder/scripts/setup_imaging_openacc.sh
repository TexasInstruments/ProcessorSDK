#! /bin/sh

# export variables needed for build and setup
export SOC=j721s2 #Note: Set this to your particular SOC
export CONCERTO_GCC_VERSION=11
export TISDK_IMAGE=adas #Note: This assumes the rtos SDK
export PATH="$PATH:/opt/nvidia/hpc_sdk/Linux_x86_64/23.9/compilers/bin" #Note: This assumes the default installation path.
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/opt/nvidia/hpc_sdk/Linux_x86_64/23.9/compilers/lib"
export PSDK_INSTALL_PATH="$HOME/sdk_workareas/$SOC" #Note: point this to your SDK path

# Setting up with SDK
cd $PSDK_INSTALL_PATH/sdk_builder/
make sdk_scrub -j && make sdk_clean -j
make rtos_sdk -j

export PDK_PATH="$PSDK_INSTALL_PATH/pdk/packages" #Note: Please point this to the PDK folder in your corresponding

# Rename PDK libraries so that nvc++ compiler can link them 
export PDK_PATH="$PSDK_INSTALL_PATH/pdk/packages" #Note: Please point this to the PDK folder in your corresponding SDK
cp $PDK_PATH/ti/drv/udma/lib/$SOC"_hostemu"/c7x-hostemu/release/udma.lib $PDK_PATH/ti/drv/udma/lib/$SOC"_hostemu"/c7x-hostemu/release/libudma.lib.a
cp $PDK_PATH/ti/drv/udma/lib/$SOC"_hostemu"/c7x-hostemu/release/dmautils.lib $PDK_PATH/ti/drv/udma/lib/$SOC"_hostemu"/c7x-hostemu/release/libdmautils.lib.a
cp $PDK_PATH/ti/drv/sciclient/lib/$SOC"_hostemu"/c7x-hostemu/release/sciclient.lib $PDK_PATH/ti/drv/sciclient/lib/$SOC"_hostemu"/c7x-hostemu/release/libsciclient.lib.a
cp $PDK_PATH/ti/osal/lib/nonos/$SOC/c7x-hostemu/release/ti.osal.lib $PDK_PATH/ti/osal/lib/nonos/$SOC/c7x-hostemu/release/libti.osal.lib.a
cp $PDK_PATH/ti/csl/lib/$SOC/c7x-hostemu/release/ti.csl.lib $PDK_PATH/ti/csl/lib/$SOC/c7x-hostemu/release/libti.csl.lib.a
cp $PDK_PATH/ti/csl/lib/$SOC/c7x-hostemu/release/ti.csl.init.lib $PDK_PATH/ti/csl/lib/$SOC/c7x-hostemu/release/libti.csl.init.lib.a
unset PDK_PATH

# Build SDK
bash make_sdk.sh