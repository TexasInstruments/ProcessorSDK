
data_set_file=$1

linuxfs_folder=/media/$USER/root
qnxfs_folder=/media/$USER/qnxfs
mount_point=$linuxfs_folder

test_data_folder=""
tar_arg=""

untar_file() {
    if [ -d "$mount_point" ]; then
        echo "Installing $data_set_file to $test_data_folder ..."
        if [ -f "$data_set_file" ]; then
            mkdir -p "$test_data_folder"
            tar -xf "$data_set_file" $tar_arg -C "$test_data_folder"
            sync
            echo "Installing $data_set_file to $test_data_folder/ ... Done"
        else
            echo "ERROR: $data_set_file not found !!!"
        fi
    else
        echo "ERROR: $mount_point not found !!!"
    fi
}

qnx_mode=0
# Parse --qnx flag in any position
for arg in "$@"; do
    if [[ $arg == "--qnx" ]]; then
        qnx_mode=1
        break
    fi
done

# Remove --qnx from args and set data_set_file
args=()
for arg in "$@"; do
    if [[ $arg != "--qnx" ]]; then
        args+=("$arg")
    fi
done
if [ ${#args[@]} -gt 0 ]; then
    data_set_file="${args[0]}"
fi
if [ $qnx_mode -eq 1 ]; then
    mount_point=$qnxfs_folder
fi

if [[ $data_set_file == *_data_set_*.tar.gz ]]; then
    if [ $qnx_mode -eq 1 ]; then
        test_data_folder="$mount_point/vision_apps/test_data"
        tar_arg="--strip-components=2"
    else
        test_data_folder="$mount_point/opt/vision_apps/test_data"
        tar_arg="--strip 2"
    fi
    untar_file

    # Run SRV car model download script only for big data (no board name in filename)
    if [[ $data_set_file =~ ^.*_data_set_[0-9]+_[0-9]+_[0-9]+\.tar\.gz$ ]]; then
        car_model_script="$test_data_folder/psdkra/srv/scripts/download_car_model.sh"
        if [ -f "$car_model_script" ]; then
            echo "Running $car_model_script ..."
            bash "$car_model_script" "$mount_point"
        fi
    fi
else
    echo "Usage: $0 [--qnx] <path/to/*_data_set*.tar.gz>"
    echo "Pass one of the data sets provided on the SDK download page."
fi
