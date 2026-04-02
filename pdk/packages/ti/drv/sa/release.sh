#########################################################
#sa-lld git tree and checkout to build label
#########################################################

#Build the SA LLD package
export MCSDK_INSTALLER_LINK=http://software-dl.ti.com/sdoemb/sdoemb_public_sw/mcsdk/03_00_02_13/exports
export MCSDK_INSTALLER_NAME=mcsdk_3_00_02_13_setuplinux.bin
export MCSDK_INSTALL_DIR=`echo $MCSDK_INSTALLER_NAME|sed -e 's/_setuplinux\.bin//g'`
export PARTNO=keystone2
export BLDDIR_SAPKG=$PWD 
export SA_LLD_REL=$BLDDIR_SAPKG/../DEP_TOOLS
# XDC TOOLS SETUP
export PV_XDC=3_25_02_70
export XDCTOOLS_INSTALLER_LINK=http://www.sanb.design.ti.com/tisb_releases/XDCtools/$PV_XDC/exports/
export XDCTOOLS_INSTALLER_NAME=xdctools_setuplinux_$PV_XDC.bin
export XDCTOOLS_INSTALL_DIR=`echo $XDCTOOLS_INSTALLER_NAME|sed -e "s/_setuplinux_${PV_XDC}\.bin/_${PV_XDC}/g"`

echo "MCSDK_INSTALL_DIR: $MCSDK_INSTALL_DIR"
echo "SA_LLD_REL: $SA_LLD_REL"
echo "XDCTOOLS_INSTALL_DIR: $XDCTOOLS_INSTALL_DIR"

# Get the Doxygen
sudo apt-get --force-yes --assume-yes install doxygen

# Save the current directory
pushd .

############################################################
# Remove old files
############################################################
mkdir -p $SA_LLD_REL

###########################################################
# Grab the dependent packages (MCSDK 3.0) and install
###########################################################
# Get the MCSDK on which SA LLD needs to be built
# Install the MCSDK
cd $SA_LLD_REL
if [ ! -d $SA_LLD_REL/$MCSDK_INSTALL_DIR ]
then
	wget -nc --no-proxy $MCSDK_INSTALLER_LINK/$MCSDK_INSTALLER_NAME
	chmod a+x $MCSDK_INSTALLER_NAME
	./$MCSDK_INSTALLER_NAME --prefix $SA_LLD_REL/$MCSDK_INSTALL_DIR --mode silent
	rm $MCSDK_INSTALLER_NAME
fi

# Install the XDC TOOLS 
if [ ! -d $SA_LLD_REL/$XDCTOOLS_INSTALL_DIR ]
then
	wget -nc --no-proxy $XDCTOOLS_INSTALLER_LINK/$XDCTOOLS_INSTALLER_NAME
	chmod a+x $XDCTOOLS_INSTALLER_NAME
	./$XDCTOOLS_INSTALLER_NAME --prefix $SA_LLD_REL/$XDCTOOLS_INSTALL_DIR --mode silent
	rm $XDCTOOLS_INSTALLER_NAME
fi

# Get the XDC Eclipse Plugin Generator
if [ ! -f xdc_eclipse_plugin_gen.zip ] 
then

	wget --no-proxy http://tigt_dev.gt.design.ti.com/dev/CTOOLSLIB_SDK/ECLIPSEPLUGINGEN/xdc_eclipse_plugin_gen.zip
	unzip xdc_eclipse_plugin_gen.zip
fi

# Get the CGT for DSP
if [ ! -d $HOME/ti/TI_CGT_C6000_7.4.2 ]
then
        wget --no-proxy -nc http://syntaxerror.dal.design.ti.com/release/releases/c60/rel7_4_2/build/install/ti_cgt_c6000_7.4.2_setup_linux_x86.bin
        chmod a+x ti_cgt_c6000_7.4.2_setup_linux_x86.bin
       ./ti_cgt_c6000_7.4.2_setup_linux_x86.bin --prefix $HOME/ti/TI_CGT_C6000_7.4.2 --mode silent
fi

# Get the CGT for DSP
#if [ ! -d $HOME/ti/TI_CGT_C6000_7.3.4 ]
#then
#        wget --no-proxy -nc http://syntaxerror.dal.design.ti.com/release/releases/c60/rel7_3_4/build/install/ti_cgt_c6000_7.3.4_setup_linux_x86.bin
#        chmod a+x ti_cgt_c6000_7.3.4_setup_linux_x86.bin
#       ./ti_cgt_c6000_7.3.4_setup_linux_x86.bin --prefix $HOME/ti/TI_CGT_C6000_7.3.4 --mode silent
#fi

# Get the Linaro Tools for ARM
if [ ! -d $HOME/linaro-2013.03 ]
then
		cd $HOME
		wget -nc https://launchpad.net/linaro-toolchain-binaries/trunk/2013.03/+download/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux.tar.bz2
        tar -jxf gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux.tar.bz2
		mv gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux linaro-2013.03
fi

# Get the Webgen tools for the release 
if [ ! -d $SA_LLD_REL/webgen ]
then
        wget  -nc --no-proxy http://tigt_qa.gt.design.ti.com/qacm/test_area/nightlytools/webgen/webgen.tgz
        tar -xzf webgen.tgz
fi

################################################################
# Install Jammer 
################################################################
# SETUP INSTALLJAMMER  
cd $HOME  
 if [ -d "installjammer" ]  
   then  
       echo Installjammer already untarred  
 else  
       wget --no-proxy -nc http://tigt_qa.gt.design.ti.com/qacm/test_area/nightlytools/installjammer/installjammer_v1_2_15.tar.gz  
       tar -xzf installjammer_v1_2_15.tar.gz  
 fi

##############################################################
# Build the SA target from the GIT tree
##############################################################
# Pop the current directory
popd

### Now Export the Build dependencies
# C66X_GEN_git INSTALL_DIR   - C6x Code gen tools install directory
export C6X_GEN_INSTALL_PATH=$HOME/ti/TI_CGT_C6000_7.4.2

# GNU_ARM_CORTEXA8_GEN_INSTALL_DIR- GNU ARM Cortex-A8 tools install directory
export CROSS_TOOL_INSTALL_PATH=$HOME/linaro-2013.03/bin
export CROSS_TOOL_PRFX=arm-linux-gnueabihf-

#XDC install dir
export XDC_INSTALL_PATH=$SA_LLD_REL/$XDCTOOLS_INSTALL_DIR

# PATH TO  PDK INSTALL PATH
export PDK_INSTALL_PATH=$SA_LLD_REL/$MCSDK_INSTALL_DIR/pdk_keystone2_3_00_02_13/packages

# PATH TO  SA INSTALL DIR
export SA_INSTALL_DIR=$PWD
export SA_INSTALL_PATH=$SA_INSTALL_DIR

# PATH for the XDC Plugin gen path
export XDCPLUGIN_INSTALL_DIR=$SA_LLD_REL/xdc_eclipse_plugin_gen/20091203

export CGXML_INSTALL_PATH=$SA_LLD_REL/$MCSDK_INSTALL_DIR/cg_xml

export INSTALL_JAMMER_PATH=$HOME/installjammer

# call the setup environment script
source ./setupenv.sh

# check if the environment is setup correctly
if [ "$DIR_NOT_EXIST" == "" ];then
	echo SA LLD BUILD ENVIRONMENT SUCCESFULLY CONFIGURED
   # Build
   export ARMV7SALIBDIR=./lib/armv7
   export ARMV7SAOBJDIR=./obj
   export ARMV7SABINDIR=./bin
   export ARMV7LIBDIR=./lib/armv7
   export ARMV7OBJDIR=./obj
   export ARMV7BINDIR=./bin
   echo "PWD: $PWD"
   echo "PATH: $PATH"
   echo "XDCPATH: $XDCPATH"
   echo "Now making SA LLD:"
   xdc clean
   xdc release XDCARGS="SETUP"
   echo "Now making SA 3gpp Enabler:"
   cd sa3gppEnabler
   xdc clean
   xdc release XDCARGS="SETUP"	
   cd ..

# Run Webgen for the release 
cd webgen 
pwd 
mkdir -p exports 
cd exports 
pwd 

chmod +x *

cp $BLDDIR_SAPKG/packages/setupLinux_salld_keystone2_${UVER}.bin . 
cp $BLDDIR_SAPKG/packages/setupwin32_salld_keystone2_${UVER}.exe . 
cp $BLDDIR_SAPKG/docs/ReleaseNotes_SA_LLD.pdf . 
cp $BLDDIR_SAPKG/docs/SA_LLD_2_0_SoftwareManifest.pdf . 
cp $BLDDIR_SAPKG/docs/UserGuide_SA_LLD.pdf . 
cp $BLDDIR_SAPKG/sa3gppEnabler/packages/setupLinux_sa3gpp_keystone2_${UVER}.bin . 
cp $BLDDIR_SAPKG/sa3gppEnabler/packages/setupwin32_sa3gpp_keystone2_${UVER}.exe . 
cp $BLDDIR_SAPKG/sa3gppEnabler/docs/ReleaseNotes_SA_3GPP.pdf . 
cp $BLDDIR_SAPKG/sa3gppEnabler/docs/SA_LLD_3GPP_2_0_SoftwareManifest.pdf . 
cd $BLDDIR_SAPKG/webgen 
./runmake 
./runwebgen   
   
else
   echo SA LLD BUILD ENVIRONMENT NOT CONFIGURED 
   echo SA LLD SDF FAILED
fi
