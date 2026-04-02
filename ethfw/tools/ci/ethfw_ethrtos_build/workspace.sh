#!/bin/bash
echo "Current directory: $(pwd)"

printenv | sort
#set -Eeuo pipefail
set -x
set -o functrace


#rm -rf /workdir/clone


# Variables
release_version_short="11_02_00"
ethfw_build=true
sdk_builder_tag="main"
concerto_tag="main"
ethfw_tag="master"
tsn_tag="next"
pdk_docs_tag="master"
csl_tag="release"
mcusw_tag="jacinto_master_11.0"
kw_build=false
ethfw_manifest_repo="ssh://git@bitbucket.itg.ti.com/processor-sdk-gateway/repo_manifests.git"
pdk_version=pdk_11_02_00_00

# Create an SSH agent
chmod 600 /workdir/privatekey/private_key.txt
eval $(ssh-agent -s)
# Add the private key to the SSH agent
ssh-add /workdir/privatekey/private_key.txt
# Set the SSH key
export GIT_SSH_COMMAND="ssh -i /workdir/privatekey/private_key.txt"
#echo $GIT_SSH_COMMAND
#echo "permission of the private_key.txt $(ls -l /workdir/privatekey/private_key.txt)"

total_build_start_time=`date +%s`
date=`date '+%b_%d_%Y_%I_%M_%p'`
date_print=`date '+%d-%b-%Y %I:%M:%S %p'`
git_path="/usr/bin"



   #Default value if not provided
if [ "$working_dir" == "" ]; then
  working_dir=`pwd`
fi
: ${product_family:="jacinto"}
: ${release_build:="true"}
: ${release_version:="11_02_00"}
: ${ethfw_build:="true"}
: ${quick_ethfw_build:="false"}
: ${enet_build:="false"}
: ${stable_build:="false"}
: ${docs_build:="false"}
: ${cplusplus_build:="true"}
: ${manifest_tag:="ETHFW.CICD"}
: ${ethfw_manifest_repo:="ssh://git@bitbucket.itg.ti.com/processor-sdk-gateway/repo_manifests.git"}
: ${doxygen_install_path:="/usr/bin"}
if [ "${release_build}" == "true" ]; then
   profile_list="release"
else
   profile_list="debug release"
fi



print_time_diff() {
  local start_time=$1
  local message=$2

  local end_time=$(date +%s)
  local diff=$((end_time - start_time))

  echo "$message: $diff seconds"
}

tag_replace() {
  local file=$1
  local repo_name=$2
  local tag=$3
  sed -i -e "s|${repo_name}|${tag}|g" ${file}
}

if [ -d "$clone_dir" ]; then
  # Check if the repository is properly cloned
  if [ -f "$clone_dir/ethfw_j7200.xml" ]; then
    echo "Repository is already cloned, skipping cloning step"
  else
    # Remove the existing clone directory and re-clone the repository
    echo "Repository is not properly cloned, re-cloning..."
    rm -rf "$clone_dir"
    git clone "$ethfw_manifest_repo" "$clone_dir"
    echo "Repository cloned successfully to $clone_dir"
  fi
else
  echo "Repository is not cloned, cloning..."
  git clone "$ethfw_manifest_repo" "$clone_dir"
  echo "Repository cloned successfully to $clone_dir"
fi





#Derived variables
if [ "$product_family" == "jacinto" ]; then
    if [ "$enet_build" == "true" ]; then    
       : ${board_list:="j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm"}
    else
       : ${board_list:="j721e_evm j7200_evm j784s4_evm j742s2_evm"}
    fi
fi
if [ "$product_family" == "j721e" ]; then
   : ${board_list:="j721e_evm"}
fi
if [ "$product_family" == "j7200" ]; then
   : ${board_list:="j7200_evm"}
fi
if [ "$product_family" == "j784s4" ]; then
   : ${board_list:="j784s4_evm"}
fi
if [ "$product_family" == "j742s2" ]; then
   : ${board_list:="j742s2_evm"}
fi





release_version_dot=`echo ${release_version} | sed -e "s|\_|.|g"`
release_version_short=`echo ${release_version} | cut -d "_" -f -3`
# release_version_short_dot=`echo ${release_version_short} | sed -e "s|\_|.|g"`

soc=`echo ${board_list} | cut -d "_" -f 1`


if [ "${release_build}" == "true" ]; then
   export fsdk_link=http://pdknas.dhcp.ti.com/docker_artifacts/JACINTO_RELEASE/latest/artifacts/output/webgen/exports/ti-processor-sdk-rtos-j721e-11_02_00_00.tar.gz
fi


fsdk_folder=`echo "$fsdk_link" | rev`
fsdk_folder=`echo ${fsdk_folder} | cut -d "/" -f 1`
fsdk_folder=`echo "$fsdk_folder" | rev`
fsdk_folder=`echo ${fsdk_folder} | cut -d "." -f 1`

pdk_version=`echo "$fsdk_folder" | rev`
pdk_version=`echo ${pdk_version} | cut -d "-" -f 1`
pdk_version=`echo "$pdk_version" | rev`

if [ "${release_build}" == "true" ]; then
    # RC package for j721e has "jacinto" as platform name
    if [ "$product_family" == "j721e" ]; then
      pdk_folder=pdk_jacinto_${pdk_version}
    else
      pdk_folder=pdk_${product_family}_${pdk_version}
    fi
else
    pdk_folder=pdk_${release_version}
fi



ethfw_folder=ethfw_${release_version_short}

if [ "${ethfw_build}" == "true" ]; then
product_name=ethfw
else
product_name=enet-lld
fi

release_folder=${product_name}-rtos-${soc}-${release_version}

if [ "$product_family" == "jacinto" ]; then
  release_folder=${product_name}-rtos-allSocs-${release_version}
fi

release_folder_binary_only=${release_folder}-binary_only

release_folder_docs_only=${release_folder}-docs_only

#Get the provided xml details
if [ "${release_build}" == "true" ]; then
    ethfw_manifest_xml=ethfw_rel_${soc}.xml
else
    if [ "$product_family" == "jacinto" ]; then
      ethfw_manifest_xml=ethfw_j721e.xml
      fsdk_manifest_xml=fsdk_j721e.xml
    else
      if [ "${ethfw_build}" == "true" ]; then
        ethfw_manifest_xml=ethfw_${soc}.xml
      fi
      fsdk_manifest_xml=fsdk_${soc}.xml
    fi
fi

if [ "${stable_build}" == "true" ]; then
    ethfw_manifest_xml=ethfw_${soc}_stable.xml
    fsdk_manifest_xml=fsdk_${soc}_stable.xml
fi





set_proxies() {
    echo "Set required proxies ..."
    export HTTPS_PROXY=http://webproxy.ext.ti.com:80
    export https_proxy=http://webproxy.ext.ti.com:80
    export HTTP_PROXY=http://webproxy.ext.ti.com:80
    export http_proxy=http://webproxy.ext.ti.com:80
    export ftp_proxy=http://webproxy.ext.ti.com:80
    export FTP_PROXY=http://webproxy.ext.ti.com:80
    export no_proxy=ti.com
    echo "Set required proxies ... Done"
}

create_build_target_file() {
	echo "Creating build target files...!!!"
    local file_name=${1}
    local file_ext=${2}
    local log_file=${3}
    echo "${file_name}${file_ext}"      >> ${build_targets_dir}/${file_name}.bt
    if [ -s "${log_file}" ]; then
        echo "${file_name}.bt:FAILED"   >> ${build_targets_dir}/build_targets
        echo "Error Log..."             >> ${build_targets_dir}/${file_name}.bt
        cat ${log_file}                 >> ${build_targets_dir}/${file_name}.bt
    else
        echo "${file_name}.bt:PASSED"   >> ${build_targets_dir}/build_targets
    fi
}

clean_folders() {
  rm -rf $clone_dir $workarea_dir ${artifacts_dir} ${build_targets_dir} ${log_dir}
  rm -rf $release_folder_dir ${release_folder_dir}.tar.gz
}
#clean_folders

# Define Environment Variable
work_dir=/workdir
clone_dir=/workdir/clone
workarea_dir=/workdir/workarea
artifacts_dir=/workdir/artifacts
tools_dir=/workdir/tools
release_folder_dir=$work_dir/$release_folder

build_targets_dir=${artifacts_dir}/output
repo_revs_file=${artifacts_dir}/repo-revs.txt
log_dir=${artifacts_dir}/logs

build_log=${log_dir}/build.log
build_error_log=${log_dir}/error_build.log
pdk_log=${log_dir}/pdk.log
enet_log=${log_dir}/enet.log
ethfw_log=${log_dir}/ethfw.log

pdk_clone_path=$clone_dir/pdk
sdk_install_path=$workarea_dir
ethfw_clone_path=$clone_dir
ethfw_comp_install_path=$workarea_dir/ethfw

mkdir -p ${clone_dir}
mkdir -p ${artifacts_dir}
mkdir -p ${workarea_dir}
mkdir -p ${log_dir}
mkdir -p ${build_targets_dir}
mkdir -p ${release_folder_dir}
mkdir -p $release_folder_binary_only
mkdir -p $release_folder_docs_only
mkdir -p ${tools_dir}

touch ${artifacts_dir}/fsdk_j721e.xml
touch ${repo_revs_file}
touch ${build_log}
touch ${build_error_log}
touch ${pdk_log}
touch ${enet_log}
touch ${ethfw_log}

####################### Clone all required repos ######################
repo_init() {
  echo "Doing repo init and sync in $clone_dir ..."
  local start_time=`date +%s`
  if [ ! -d ${clone_dir}/.repo ]; then
    cd $clone_dir
    rm -rf $ethfw_manifest_xml
    rm -rf $fsdk_manifest_xml
    rm -rf common.sh
    rm -rf ethfw_tag_input.cfg
    if [ "${release_build}" == "true" ]; then
      git archive --remote=$ethfw_manifest_repo $manifest_tag releases/${release_version_short}/${ethfw_manifest_xml} > temp.tar
      tar -xf temp.tar --strip-components 2
      rm -rf temp.tar
    else
      if [ "${ethfw_build}" == "true" ]; then
        git archive --remote=$ethfw_manifest_repo $manifest_tag $ethfw_manifest_xml > temp.tar
        tar -xf temp.tar --strip-components 0
        rm -rf temp.tar
      fi
      git archive --remote=$ethfw_manifest_repo $manifest_tag $fsdk_manifest_xml > temp.tar
      tar -xf temp.tar --strip-components 0
      rm -rf temp.tar
    fi
    git archive --remote=$ethfw_manifest_repo $manifest_tag scripts/common.sh > temp.tar
    tar -xf temp.tar --strip-components 1
    rm -rf temp.tar

    git archive --remote=$ethfw_manifest_repo $manifest_tag releases/${release_version_short}/ethfw_tag_input.cfg > temp.tar
    tar -xf temp.tar --strip-components 2
    rm -rf temp.tar

    source common.sh
    source ethfw_tag_input.cfg

    if [ "${release_build}" == "false" ]; then
      echo "Overrding tag ..."
      #Override the branch in repo xml file
      if [ "${ethfw_build}" == "true" ]; then
        tag_replace $ethfw_manifest_xml "sdk_builder"             $sdk_builder_tag
        tag_replace $ethfw_manifest_xml "concerto"                $concerto_tag
        tag_replace $ethfw_manifest_xml "ethfw"                   $ethfw_tag
      fi
      tag_replace $fsdk_manifest_xml  "enet-lld"                $enet_tag
      tag_replace $fsdk_manifest_xml  "enet-tsn-stack"          $tsn_tag
      tag_replace $fsdk_manifest_xml  "pdk"                     $pdk_tag
      tag_replace $fsdk_manifest_xml  "pdk_docs"                $pdk_docs_tag
      tag_replace $fsdk_manifest_xml  "common-csl-ip"           $csl_tag
      tag_replace $fsdk_manifest_xml  "mcusw"                   $mcusw_tag
    fi
    #replace the repo xml with modified file
    if [ "${release_build}" == "false" ]; then
        if [ "${ethfw_build}" == "true" ]; then
          repo init -q -u ${ethfw_manifest_repo} -b ${manifest_tag} -m $ethfw_manifest_xml
          rm -rf .repo/manifests/$ethfw_manifest_xml
          cp -f $ethfw_manifest_xml .repo/manifests
        fi
        if [ "${enet_build}" == "true" ]; then
          repo init -q -u ${ethfw_manifest_repo} -b ${manifest_tag} -m $fsdk_manifest_xml
        fi
        rm -rf .repo/manifests/$fsdk_manifest_xml
        cp -f $fsdk_manifest_xml .repo/manifests
    else
        repo init -q -u ${ethfw_manifest_repo} -b refs/tags/${manifest_tag} -m releases/${release_version_short}/$ethfw_manifest_xml
        rm -rf .repo/manifests/releases/${release_version_short}/$ethfw_manifest_xml
        cp -f ${ethfw_manifest_xml} .repo/manifests/releases/${release_version_short}
    fi

    if [ "${ethfw_build}" == "true" ]; then
      #Clone
      echo "Cloning as per provided XML: $ethfw_manifest_xml ..."

      if [ "${release_build}" == "true" ]; then
          repo init -q -m  releases/${release_version_short}/$ethfw_manifest_xml
      else
          repo init -q -m $ethfw_manifest_xml
      fi
    fi

    if [ "${enet_build}" == "true" ]; then
      #Clone
      echo "Cloning with XML: $fsdk_manifest_xml ..."
      repo init -q -m $fsdk_manifest_xml
    fi

	if [ "${release_build}" == "true" ]; then
      #Clone
      echo "Cloning with fsdk_manifest_xml: $fsdk_manifest_xml ..."
      repo init -q -m $fsdk_manifest_xml
    fi

    #repo sync -q
    repo sync -j10
    repo start dev --all
    #Show the current branch/git status
    repo forall -c "pwd;git branch -vv | cut -d ' ' -f 1-4"

    #Copy the manifest to artifacts folder
    if [ "${ethfw_build}" == "true" ]; then
      cp -f ${ethfw_manifest_xml} ${artifacts_dir}
    fi

    if [ "${release_build}" == "false" ]; then
        cp -f ${fsdk_manifest_xml} ${artifacts_dir}
    fi
    # Print clone time
    echo "Cloning completed!!"
    cd - > /dev/null

  fi
  print_time_diff $start_time "Clone Time"
  echo "Repo init and sync in $clone_dir ... Done"
  echo ""
}
repo_init


pdk_install_path=${workarea_dir}/${pdk_folder}

pdk_doxygen_build() {
    echo "Starting Doxygen Build..."
    local start_time=`date +%s`
    cd $pdk_install_path/docs/internal_docs/doxygen

    for board in $board_list
    do
        echo "  Doxygen Build for Board:${board} ..."
        make -s apiguide BOARD=$board SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_install_path/packages DOXYGEN=doxygen TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}
        echo "  Doxygen Build for Board:${board} completed!!"
    done

    cd - > /dev/null
    print_time_diff $start_time "Doxygen Time"
    echo "Doxygen Build completed!!"
    echo ""
}

echo "board_list2: ${board_list}"

pdk_sphinx_build() {
    echo "Starting Sphinx Build..."
    local start_time=`date +%s`

    cd $pdk_install_path/docs/internal_docs/sphinx
    export PATH=/workdir/.local/bin:$PATH
    make -s all > /dev/null
    cd - > /dev/null

    print_time_diff $start_time "Sphinx Time"
    echo "Sphinx Build completed!!"
    echo ""
}




get_repo_revs() {
    local xmlFile=$1
    ## Set release name and directory
    local git_name=`git remote show origin -n | grep "Fetch URL:" | cut -d":" -f2- | cut -d" " -f2 | sed -e "s|\:|;|g"`
    local repo_name=`git remote show origin -n | grep "Fetch URL:" | cut -d":" -f2- | cut -d" " -f2 | cut -d"/" -f5- | cut -d"." -f1`

    ## Get latest commit ID and branch
    local commit_id=`git log -n 1 | grep commit | grep -v 'this commit' | cut -d" " -f 2`
    local git_branch=`git branch -vv | cut -d ' ' -f 2-4`
    local commit_msg=`git log -1 --pretty=format:%s | sed -e "s|:|\||g"`

    ## Write into the repo-revs info file
    echo -n ${git_name}:${commit_id}:${git_branch}: >> ${repo_revs_file}
    echo ${commit_msg}                              >> ${repo_revs_file}

    ## Replace the branch with the fixed commit in artifact XML file
    tag_replace ${artifacts_dir}/${xmlFile} ${repo_name} ${commit_id}
    #Clone depth already set. Revert the duplicate change due to above command
    sed -i -e 's|clone-depth="1" clone-depth="1"|clone-depth="1"|g'  ${artifacts_dir}/${xmlFile}
}

repo_revs_pdk() {
    if [ -d ${clone_dir}/pdk ]; then
        cd ${clone_dir}/pdk
        get_repo_revs $fsdk_manifest_xml 
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/docs ]; then
        cd ${clone_dir}/pdk/docs
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/csl ]; then
        cd ${clone_dir}/pdk/packages/ti/csl
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/pm ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/pm
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/pmic ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/pmic
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/enet ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/enet
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/csirx ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/csirx
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/csitx ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/csitx
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/sa ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/sa
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
    if [ -d ${clone_dir}/pdk/packages/ti/drv/sciclient/src/rm_pm_hal ]; then
        cd ${clone_dir}/pdk/packages/ti/drv/sciclient/src/rm_pm_hal
        get_repo_revs $fsdk_manifest_xml
        cd - 1>/dev/null
    fi
}


download_components() {
    echo "Downloading components in $workarea_dir ..."
    local start_time=`date +%s`
    cd $workarea_dir

    for board in $board_list
    do
      soc=`echo ${board} | cut -d "_" -f 1`
      $clone_dir/.repo/manifests/download_${soc}_components.sh
    done

    cd - > /dev/null
 	#print_time_diff $start_time "Download Time"
    echo "Downloading components in $workarea_dir ... Done"
    echo ""

    #Exports the compiler path variables
    export PSDK_TOOLS_PATH=${tools_dir}
    export TOOLS_INSTALL_PATH=${tools_dir}

    if [ "${release_build}" == "true" ]; then
      echo "Downloading FSDK Tarball ..."
      # download the FSDK Tarball and use that directly to package
      cd ${workarea_dir}
      wget -q $fsdk_link
      tar -xf $fsdk_folder.tar.gz
      rm -rf $fsdk_folder.tar.gz
      cd - 1>/dev/null

      mv -f ${workarea_dir}/$fsdk_folder/*  ${workarea_dir}
    fi
}
download_components

###################### Package Build ######################
package_pdk() {
    echo "  Package PDK ..."
    local start_time=`date +%s`

    #Repo rev PDK
    repo_revs_pdk  

    cd ${clone_dir}/pdk/packages/ti/build

	for board in ${board_list}
	do
    	echo "    PDK Packaging Board:${board} ..."
    	timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
    	pdk_board_log=${log_dir}/pdk_${board}_${timestamp}.log
    	touch ${pdk_board_log}
    	make -s -j allcores_package PACKAGE_SELECT=all BOARD=${board} TOOLS_INSTALL_PATH:=${tools_dir} SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_clone_path/packages &> ${pdk_board_log}
    	echo "    PDK Packaging Board:${board} completed!!"
    	#cat ${pdk_board_log}
	done

    cd - 1>/dev/null


    #Copy the packaged files to build folder
    rm -rf ${workarea_dir}/${pdk_folder}
    cp -rf ${clone_dir}/pdk/packages/ti/binary/package/all/pdk_     ${workarea_dir}
    mv -f ${workarea_dir}/pdk_                                    ${workarea_dir}/${pdk_folder}


    #Copy non-source items not packaged through make package
    cp -rf ${clone_dir}/pdk/docs                 ${workarea_dir}/${pdk_folder}
    cp -rf ${clone_dir}/pdk/packages/makefile    ${workarea_dir}/${pdk_folder}/packages
    #echo "hear2 $(pwd)"
    if [ -d ${workarea_dir}/${pdk_folder}/packages/ti/transport/lwip/lwip-contrib/ ]; then
        #Delete .git folder of lwip-contrib
        rm -rf ${workarea_dir}/${pdk_folder}/packages/ti/transport/lwip/lwip-contrib/.git*
    fi
    if [ -d ${workarea_dir}/${pdk_folder}/packages/ti/transport/lwip/lwip-stack/ ]; then
        #Delete .git folder of lwip-stack
        rm -rf ${workarea_dir}/${pdk_folder}/packages/ti/transport/lwip/lwip-stack/.git*
    fi

    if [ -d ${workarea_dir}/${pdk_folder}/packages/ti/drv/enet* ]; then
        #enet documentation is part of driver itself and not part of pdk_docs, copy the same
        mkdir -p ${workarea_dir}/${pdk_folder}/packages/ti/drv/enet/internal_docs
        cp -rf ${clone_dir}/pdk/packages/ti/drv/enet/internal_docs/doxygen  ${workarea_dir}/${pdk_folder}/packages/ti/drv/enet/internal_docs/
    fi
    #Changes after package
    mkdir -p ${workarea_dir}/${pdk_folder}/packages/ti/build
    cd ${workarea_dir}/${pdk_folder}/packages/ti/build
    sed -i -e "s|..\...\...\...|${release_version_dot}|g"                                           makefile
    #Replace only the first occurrence - that is for release build
    if [ ! -f makefile ]; then
      echo "VERSION = ...." > makefile
    fi
    if [ ! -f Rules.make ]; then
      touch Rules.make
    fi
    if [ ! -f pdk_tools_path.mk ]; then
      echo "PDK_VERSION_STR=_\$[(]PDK_SOC[)]_\$[(]PDK_VERSION[)]" > pdk_tools_path.mk
    fi
    sed -i -e '0,/DISABLE_RECURSE_DEPS....no/s//DISABLE_RECURSE_DEPS \?\= yes/'                     Rules.make
    sed -i -e "s|PDK_VERSION_STR=_\$[(]PDK_SOC[)]_\$[(]PDK_VERSION[)]|PDK_VERSION_STR=|g"           pdk_tools_path.mk
    sed -i -e "s|PDK_VERSION_STR=|PDK_VERSION_STR=_${product_family}_${release_version}|g"          pdk_tools_path.mk
    cd - 1>/dev/null
    print_time_diff $start_time "  Package PDK Time"
    echo "  Package PDK ... Done"
}
set_proxies
if [ "${release_build}" == "false" ]; then
  package_pdk
fi



get_component_versions() {
  echo "release_version_short:${release_version_short}"
  source $clone_dir/.repo/manifests/releases/${release_version_short}/.component_versions
}

package_ethfw_comp() {
  echo "Packaging ETHFW to $workarea_dir ..."
  local start_time=`date +%s`

  #Repo rev
  cd ${clone_dir}/ethfw
  get_repo_revs $ethfw_manifest_xml
  cd - 1>/dev/null
  cd ${clone_dir}/sdk_builder
  get_repo_revs $ethfw_manifest_xml
  cd - 1>/dev/null
  cd ${clone_dir}/sdk_builder/concerto
  get_repo_revs $ethfw_manifest_xml
  cd - 1>/dev/null
  
  cd $clone_dir

  #Copy the packaged files to build folder
  cp -rf $clone_dir/ethfw $workarea_dir
  cp -rf $clone_dir/sdk_builder $workarea_dir

  #Change component paths
  sed -i -e "s|\/pdk|\/${pdk_folder}|g"   ${workarea_dir}/ethfw/ethfw_tools_path.mak

  rm -rf $workarea_dir/ethfw/.git
  rm -rf $workarea_dir/ethfw/.gitignore
  rm -rf $workarea_dir/sdk_builder/.git
  rm -rf $workarea_dir/sdk_builder/.gitignore
  rm -rf $workarea_dir/sdk_builder/concerto/.git
  rm -rf $workarea_dir/sdk_builder/concerto/.gitignore

  cd - > /dev/null
  print_time_diff $start_time "Package Time"
  echo "Packaging ETHFW to $workarea_dir ... Done"
  echo ""
}


#################### Build SafeRTOS ####################
build_safertos(){
    echo "Build SafeRTOS ..."
    local start_time=`date +%s`

    cd $pdk_install_path/packages/ti/build
    for board in $board_list
    do
        get_component_versions 

        # First setup environment and build libraries
        echo "  Setting up SafeRTOS environment for Board:${board} ..."
        if [ "$board" == "j721e_evm" ]; then
            ./safertos_setup.sh --soc=j721e --isa=r5f --path=${sdk_install_path}/safertos_j721e_r5f_${SAFERTOS_J721E_R5F_VERSION} $1 > /dev/null
            ./safertos_setup.sh --soc=j721e --isa=c66 --path=${sdk_install_path}/safertos_j721e_c66_${SAFERTOS_J721E_C66_VERSION} $1 > /dev/null
            ./safertos_setup.sh --soc=j721e --isa=c7x --path=${sdk_install_path}/safertos_j721e_c7x_${SAFERTOS_J721E_C7X_VERSION} $1 > /dev/null
        fi
        if [ "$board" == "j7200_evm" ]; then
            ./safertos_setup.sh --soc=j7200 --isa=r5f --path=${sdk_install_path}/safertos_j7200_r5f_${SAFERTOS_J7200_R5F_VERSION} $1 > /dev/null
        fi
        if [ "$board" == "j721s2_evm" ]; then
            ./safertos_setup.sh --soc=j721s2 --isa=r5f --path=${sdk_install_path}/safertos_j721s2_r5f_${SAFERTOS_J721S2_R5F_VERSION} $1 > /dev/null
            ./safertos_setup.sh --soc=j721s2 --isa=c7x --path=${sdk_install_path}/safertos_j721s2_c7x_${SAFERTOS_J721S2_C7X_VERSION} $1 > /dev/null
        fi
        if [ "$board" == "j784s4_evm" ]; then
            ./safertos_setup.sh --soc=j784s4 --isa=r5f --path=${sdk_install_path}/safertos_j784s4_r5f_${SAFERTOS_J784S4_R5F_VERSION} $1 > /dev/null
            ./safertos_setup.sh --soc=j784s4 --isa=c7x --path=${sdk_install_path}/safertos_j784s4_c7x_${SAFERTOS_J784S4_C7X_VERSION} $1 > /dev/null
        fi
        if [ "$board" == "j742s2_evm" ]; then
            echo "  SafeRTOS not supported on Board:${board}, nothing to build"
        fi
        echo "  SafeRTOS environment setup for Board:${board} completed!!"
    done
    cd - 1> /dev/null

    print_time_diff $start_time "Build SafeRTOS Time"
    echo "Build SafeRTOS ... Done"
}


###################### SDL build ######################
build_sdl() {
    #copy SDL from clone dir to sdk dir
    cp -rf ${clone_dir}/sdl ${sdk_install_path}
    if [ -d ${sdk_install_path}/sdl* ]; then
        echo "  Build SDL ..."
        local start_time=`date +%s`
        get_component_versions

        #Build SDL Libraries
        cd ${sdk_install_path}/sdl
        for profile in ${profile_list}
        do
            for board in ${board_list}
            do
                if [ "${board}" == "j784s4_evm" ] || [ "${board}" == "j721s2_evm" ] || [ "${board}" == "j7200_evm" ] || [ "${board}" == "j721e_evm" ]; then
                    soc=`echo ${board} | cut -d "_" -f 1`
                    echo "  Building SDL Libs for Board:${board} ..."
                    #/home/x1254955local/ti/ti-cgt-armllvm_4.0.4.LTS/bin this path is for local linux test
                    # make command is also modified for testing perpose
                    #make -s $jobs_option sdl_libs SOC=${soc} PROFILE=${profile} TOOLS_INSTALL_PATH=$HOME/ti TOOLCHAIN_PATH_R5=$HOME/ti/ti-cgt-armllvm_4.0.4.LTS 1>>$log_dir/build.log
                    make -j $jobs_option sdl_libs SOC=${soc} PROFILE=${profile} TOOLS_INSTALL_PATH=${tools_dir} TOOLCHAIN_PATH_R5=${tools_dir}/ti/ti-cgt-armllvm_4.0.4.LTS CFLAGS="-Wno-unknown-pragmas" 1>>$log_dir/build.log
                    echo "  Building SDL Libs for Board:${board} completed!!!"
                fi 
            done
        done
        cd - 1>/dev/null
        print_time_diff $start_time "  Build SDL Time"
        echo "  Build SDL ... Done"
    fi
}
build_sdl


pdk_cpp_build() {
    echo "C++ Build ..."
    local start_time=`date +%s`
    cd $pdk_install_path/packages/ti/build

    cpp_build_core_list="mcu1_0 mcu1_1 c66xdsp_1"
    # c++ build to be checked only in release build
    cpp_build_profile_list="release"
    for profile in $cpp_build_profile_list
    do
        for board in $board_list
        do
        echo "  Building Board:$board Profile:$profile (c++ build) ..."
        for core in $cpp_build_core_list
        do
            make -j $jobs_option pdk_libs_clean BOARD=$board BUILD_PROFILE=$profile CORE=$core SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_install_path/packages CPLUSPLUS_BUILD=yes TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}
            make -j $jobs_option pdk_libs       BOARD=$board BUILD_PROFILE=$profile CORE=$core SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_install_path/packages CPLUSPLUS_BUILD=yes TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}
        done
        echo "  Building Board:$board Profile:$profile (c++ build) completed!!"
        done
    done
    # clean all the binaries
    for board in $board_list
    do
        make -s allclean BOARD=$board SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_install_path/packages TOOLS_INSTALL_PATH=${tools_dir}
    done

    cd - > /dev/null
    print_time_diff $start_time "C++ Build Time"
    echo "C++ Build ... Done"
    echo ""
}


###################### PDK build ######################
pdk_build () {
    local start_time=`date +%s`

    # Build cpp libs first.
    if [ "${cplusplus_build}" == "true" ] && [ "${release_build}" == "false" ]; then
        pdk_cpp_build
    fi

    print_time_diff $start_time "  cpp libs first..."
    echo "  cpp libs first..."


    for profile in $profile_list
    do
      for board in $board_list
      do
        # First build libs
        echo "  Building PDK Libs for Board:${board} ..."
        cd $pdk_install_path/packages/ti/build
        make -j $jobs_option custom_target BUILD_TARGET_LIST_ALL=all_libs BOARD=$board BUILD_PROFILE=$profile BUILD_PROFILE_LIST_ALL=$profile SDK_INSTALL_PATH=$sdk_install_path PDK_INSTALL_PATH=$pdk_install_path/packages TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}
        echo "  Building PDK Libs for Board:${board} completed!!"
        cd - > /dev/null
      done
    done

    # Setup safeRTOS 
    build_safertos

    print_time_diff $start_time "  Setup safeRTOS..."
    echo "  Setup safeRTOS..."

    for board in $board_list
    do
      echo "Building Required pdk examples for Board:${board} "
      cd ${pdk_install_path}/packages/ti/build
      make -j -s sciserver_testapp_freertos BOARD=$board CORE=mcu1_0 BUILD_PROFILE=release TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}
      make -j -s sbl_uart_img BOARD=$board CORE=mcu1_0 BUILD_PROFILE=release TOOLS_INSTALL_PATH=${tools_dir} 1>>${pdk_log} 2>>${build_error_log}

      if [ "${enet_build}" == "true" ]; then
          # Build Enet-lld Apps for freeRTOS
          enet_app_freertos_build $board

          # Build Enet-lld Apps for SafeRTOS
          enet_app_safertos_build $board
      fi
      cd - > /dev/null
    done
    
    print_time_diff $start_time "  pdk examples for Board..."
    echo "  pdk examples for Board..."
    
    if [ "${release_build}" == "false" ]; then
      # PDK docs build
      pdk_doxygen_build
      pdk_sphinx_build
    fi

    print_time_diff $start_time "  Build PDK Time"
    echo "  Build PDK ... Done"
}


if [ "${docs_build}" == "false" ] && [ "${quick_ethfw_build}" == "false" ]; then
  build_sdl
  pdk_build
fi
###################### ETHFW build ######################
ethfw_build () {
  local start_time=`date +%s`

  cd $ethfw_comp_install_path

  for board in ${board_list}
  do
    local soc=`echo ${board} | cut -d "_" -f 1`
    local soc_caps=${soc^^}

    for profile in $profile_list
    do
      if [ "$kw_build" == "true" ]; then
          echo "Building Ethfw with Klockwork for Profile:${profile} ..."
          kw_out_list=$kw_out_list$log_dir/"kw_ethfw_"$board"_"$profile".out "
          
          # Build EthFw libs and apps only for KW, don't build UT test app in KW.
          $kwinject_cmd -o $kw_ethfw_out make -s -j remoteswitchcfg_all BUILD_SOC_LIST=${soc_caps} PROFILE=$profile PSDK_TOOLS_PATH=${tools_dir} PDK_PATH=${pdk_install_path} 1>>${ethfw_log} 2>>${build_error_log}
          echo "Building ETHFW for Profile:${profile} completed!!"
        else
          echo "    Building EthFw SOC:${soc_caps} Profile:${profile} ..."
          make -j -s ethfw_all BUILD_SOC_LIST=${soc_caps} PROFILE=${profile} PSDK_TOOLS_PATH=${tools_dir} 1>>${ethfw_log} 2>>${build_error_log}
          if [ "${board}" != "j742s2_evm" ]; then
            make -j -s ethfw_all BUILD_SOC_LIST=${soc_caps} PROFILE=$profile  BUILD_APP_FREERTOS?=no BUILD_APP_SAFERTOS?=yes PSDK_TOOLS_PATH=${tools_dir} 1>>${ethfw_log} 2>>${build_error_log}
          fi
          echo "    Building EthFw SOC:${soc_caps} Profile:${profile} Done"
        fi
    done

    mkdir ${build_targets_dir}/$soc
    cp -f out/$soc_caps/R5Ft/FREERTOS/release/app_remoteswitchcfg_server_strip.xer5f ${build_targets_dir}/$soc
  done

  cd - > /dev/null
  print_time_diff $start_time "  Build EthFw Time"
  echo "  Build EthFw ... Done"
}


ethfw_doxygen_build() {
  echo "Starting Doxygen Build..."
  local start_time=`date +%s`
  cd $ethfw_comp_install_path

  for board in $board_list
  do
    echo "  EthFw User and API Guide generation for Board:${board} ..."
    make -C internal_docs/doxygen -s all DOXYGEN=doxygen 1>>${ethfw_log} 2>>${build_error_log}
    echo "    EthFw User and API Guides completed!!"
    echo "    EthFw Datasheet generation..."
    make -C internal_docs/doxygen -s datasheet DOXYGEN=doxygen 1>>${ethfw_log} 2>>${build_error_log}
    cp -R internal_docs/datasheet/ docs/.
    echo "    EthFw Datasheet completed!!"
  done

  rm -rf $ethfw_comp_install_path/internal_docs

  cd - > /dev/null
  print_time_diff $start_time "Doxygen Time"
  echo "Doxygen Build completed!!"
  echo ""
}


recur_copy() {
    find ./ -iname $1  | \
    while read filepath; do
        cp --parent --target-directory=$2 "$filepath"
    done
}



copy_binary() {
    if [ -d ${release_folder_dir}/$1 ]; then
        if [ ${1} != "mcusw" ]; then
            echo "  Copying $1 ..."
        fi
        mkdir -p $2
        cd ${release_folder_dir}/$1
        #copy binary files
        recur_copy '*.xer5f'    "../../$2"
        recur_copy '*.xem4'     "../../$2"
        recur_copy '*.xem4f'    "../../$2"
        recur_copy '*.xe66'     "../../$2"
        recur_copy '*.xa53fg'   "../../$2"
        recur_copy '*.xa72fg'   "../../$2"
        recur_copy '*.xe71'     "../../$2"
        recur_copy '*.lib'      "../../$2"
        recur_copy '*.out'      "../../$2"
        #Map files
        recur_copy '*.map'      "../../$2"
        #copy sbl images
        recur_copy '*.bin'      "../../$2"
        recur_copy '*.tiimage'  "../../$2"
        #SBL App Images
        recur_copy '*.appimage'         "../../$2"
        recur_copy '*.appimage.hs_fs'   "../../$2"
        recur_copy '*.appimage.signed'  "../../$2"
        recur_copy '*.img'              "../../$2"
        recur_copy '*-image-evm.tar.gz' "../../$2"
        #XIP App Images
        recur_copy '*.appimage_xip'     "../../$2"
        #ROV files
        recur_copy '*.rov.xs'   "../../$2"
        cd - 1>/dev/null
    fi
}


copy_docs() {
    if [ -d ${release_folder_dir}/$1 ]; then
        echo "  Copying $1 ..."
        mkdir -p ${release_folder_docs_only}/$1
        cp ${release_folder_dir}/$1/*.html                  ${release_folder_docs_only}/$1      2>/dev/null
        cp ${release_folder_dir}/$1/*.htm                   ${release_folder_docs_only}/$1      2>/dev/null
        cp ${release_folder_dir}/$1/*.gif                   ${release_folder_docs_only}/$1      2>/dev/null
        cp ${release_folder_dir}/$1/*.txt                   ${release_folder_docs_only}/$1      2>/dev/null
        cp ${release_folder_dir}/$1/docs/                   ${release_folder_docs_only}/$1 -r   2>/dev/null
        cp ${release_folder_dir}/$1/doc/                    ${release_folder_docs_only}/$1 -r   2>/dev/null
        
        if [ $2 == "pdk" ]; then
          mkdir -p ${release_folder_docs_only}/$1/packages/ti/drv/udma/docs/
          mkdir -p ${release_folder_docs_only}/$1/packages/ti/drv/csirx/docs/
          mkdir -p ${release_folder_docs_only}/$1/packages/ti/drv/enet/docs/
          mkdir -p ${release_folder_docs_only}/$1/packages/ti/drv/sciclient/soc/sysfw/binaries
          cp ${release_folder}/$1/packages/ti/drv/udma/docs/*     ${release_folder_docs_only}/$1/packages/ti/drv/udma/docs/   2>/dev/null
          cp ${release_folder}/$1/packages/ti/drv/csirx/docs/*    ${release_folder_docs_only}/$1/packages/ti/drv/csirx/docs/  2>/dev/null
          cp -r ${release_folder}/$1/packages/ti/drv/enet/docs/*  ${release_folder_docs_only}/$1/packages/ti/drv/enet/docs/   2>/dev/null
          cp -r ${release_folder}/$1/packages/ti/drv/sciclient/soc/sysfw/binaries/system-firmware-public-documentation  ${release_folder_docs_only}/$1/packages/ti/drv/sciclient/soc/sysfw/binaries   2>/dev/null
        fi
        echo "  Copying $1 ... Done !!!"
    fi
}


make_docs_only_package() {
    echo "Make Docs Only Package ..."
    local start_time=`date +%s`

    if [ -d ${release_folder_docs_only} ]; then

        if [ -d ${release_folder}/ethfw* ]; then
            copy_docs ${ethfw_folder} "ethfw"
        fi

        if [ -d ${release_folder}/pdk* ]; then
            copy_docs ${pdk_folder} "pdk"
        fi

        cd ${release_folder_docs_only}
        tar czf ${release_folder_docs_only}.tar.gz ./* 1>>${build_log} 2>>${build_error_log}
        mv ${release_folder_docs_only}.tar.gz ../ 1>>${build_log} 2>>${build_error_log}
        cd - 1>/dev/null

        create_build_target_file ${release_folder_docs_only} ".tar.gz" ${build_error_log}
    fi

    print_time_diff $start_time "Make Docs Only Package Time"
    echo "Make Docs Only Package ... Done"
}

if [ "${docs_build}" == "true" ]; then
  make_docs_only_package
fi




make_tarball_package() {
    echo "Make TarBall Package ..."
    local start_time=`date +%s`
    cd $workarea_dir
    if [ "${ethfw_build}" == "true" ]; then
      cp -rf $workarea_dir/ethfw $release_folder_dir/$ethfw_folder
      cp -rf $workarea_dir/$pdk_folder $release_folder_dir/$pdk_folder
    fi
    if [ "${enet_build}" == "true" ]; then
      cp -rf $workarea_dir/$pdk_folder $release_folder_dir/$pdk_folder
    fi
    cd - > /dev/null
    if [ "${release_build}" == "true" ]; then
        if [ -d ${release_folder_dir}/ethfw* ]; then
            cd ${release_folder_dir}
            zip -rq ethfw_${release_version_short}.zip ethfw_${release_version_short} 1>>${ethfw_log} 2>>${build_error_log}
            mv ethfw_${release_version_short}.zip ../
            cd - 1>/dev/null
            create_build_target_file ethfw_${release_version_short} ".zip" ${build_error_log}
        fi
        cd $release_folder_dir
        tar czf ${release_folder_dir}.tar.gz ./* 1>>$log_dir/build.log  2>>$log_dir/error.log
        cd - > /dev/null
    fi
    print_time_diff $start_time "Make TarBall Package Time"
    echo "Make TarBall Package ... Done"
}



make_binary_package() {
    echo "Make Binary Only Package ..."
    local start_time=`date +%s`
    if [ -d ${release_folder_binary_only} ]; then
      if [ "${quick_ethfw_build}" == "false" ]; then
          if [ "${ethfw_build}" == "true" ]; then
            copy_binary ${ethfw_folder}   ${release_folder_binary_only}/$ethfw_folder
            copy_binary ${pdk_folder}     ${release_folder_binary_only}/$pdk_folder
          fi
      fi

      if [ "${enet_build}" == "true" ]; then
        copy_binary ${pdk_folder}     ${release_folder_binary_only}/$pdk_folder
      fi

        cd ${release_folder_binary_only}
        tar czf ${release_folder_binary_only}.tar.gz ./*
        mv ${release_folder_binary_only}.tar.gz ../
        cd - 1>/dev/null

        create_build_target_file ${release_folder_binary_only} ".tar.gz" ${build_error_log}
    fi

    print_time_diff $start_time "Make Binary Only Package Time"
    echo "Make Binary Only Package ... Done"
}




make_artifact_tarball_package() {
	echo "Moving the tae.gz, .zip, xlsx files"
    mv *.tar.gz   ${build_targets_dir} 1>/dev/null 2>&1
    mv *.zip      ${build_targets_dir} 1>/dev/null 2>&1
    mv *.xlsx     ${build_targets_dir} 1>/dev/null 2>&1

if [ "${docs_build}" == "true" ]; then
    cd ${build_targets_dir}
    #untar docs only package
    untar_gz ${release_folder_docs_only}.tar.gz
    rm -rf ${release_folder_docs_only}.tar.gz
    cd - 1>/dev/null
fi
    echo "Making artifacts Tarball..."
    tar -czf ./artifacts.tar.gz ./artifacts

    echo "Making artifacts Tarball... Done"
}





if [ "${ethfw_build}" == "true" ]; then
	echo "now package_ethfw_comp will call...!!!"
  package_ethfw_comp
  if [ "${docs_build}" == "true" ]; then
    ethfw_doxygen_build
  else
    ethfw_build
    if [ "$kw_build" == "true" ]; then
      ethfw_kw_build
    else
	  echo "now ethfw_doxygen_build will call...!!!"
      ethfw_doxygen_build
    fi
  fi
fi

if [ "${quick_ethfw_build}" == "false" ]; then
    make_tarball_package
fi


if [ "${docs_build}" == "false" ]; then
    make_binary_package
fi


make_artifact_tarball_package


copying_artifacts_nas() {

    NAS_HTTP_LINK="http://epswnas.itg.ti.com/eth_rtos_cicd"

    if [ "$release_build" = "true" ]; then
        NAS_DIR=/data/eth_rtos_cicd/release_area/release_builds/ethrtos_ethfw_rtos_test1/artifacts
        echo "${NAS_HTTP_LINK}/release_area/release_builds/ethrtos_ethfw_rtos_test1/artifacts"
    else
        NAS_DIR=/data/eth_rtos_cicd/release_area/nightly_builds/ethrtos_ethfw_rtos_test1/artifacts
        echo "${NAS_HTTP_LINK}/release_area/nightly_builds/ethrtos_ethfw_rtos_test1/artifacts"
    fi

        # ensure directory exists
    ssh -o StrictHostKeyChecking=no epswbld@epswnas.itg.ti.com "mkdir -p ${NAS_DIR}"

    # copy (directory-safe)
    rsync --rsync-path=/usr/bin/rsync -e "ssh -o StrictHostKeyChecking=no" -Wav --inplace /workdir/artifacts epswbld@epswnas.itg.ti.com:${NAS_DIR}

}

copying_artifacts_nas


