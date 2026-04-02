THIS_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${THIS_SCRIPT_DIR}/board_env.sh

bootfs_file=$PWD/boot-${TISDK_IMAGE}-${TI_DEV_BOARD}.tar.gz
rootfs_file=$PWD/tisdk-${TISDK_IMAGE}-image-${TI_DEV_BOARD}.tar.xz

bootfs_folder=/media/$USER/boot
rootfs_folder=/media/$USER/root

tar_bootfs()
{
    if [ -d $bootfs_folder ]
    then
        echo "Creating $bootfs_file from $bootfs_folder ..."
        if [ ! -f $bootfs_file ]
        then
            cd $bootfs_folder
            tar czf $bootfs_file .
            cd -
            sync
            echo "Creating $bootfs_file from $bootfs_folder ... Done"
        else
            echo "ERROR: $bootfs_file exists NOT overwriting it, delete this file manually to create a new one !!!"
        fi
    else
        echo "ERROR: $bootfs_folder not found !!!"
    fi
}

tar_rootfs()
{
    if [ -d $rootfs_folder ]
    then
        echo "Creating $rootfs_file from $rootfs_folder ..."
        if [ ! -f $rootfs_file ]
        then
            cd $rootfs_folder
            sudo tar cpzf $rootfs_file .
            cd -
            sync
            echo "Creating $rootfs_file from $rootfs_folder ... Done"
        else
            echo "ERROR: $rootfs_file exists NOT overwriting it, delete this file manually to create a new one !!!"
        fi
    else
        echo "ERROR: $rootfs_folder not found !!!"
    fi

}

tar_bootfs
tar_rootfs
