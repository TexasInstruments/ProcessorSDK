NUM_PROCS=$(cat /proc/cpuinfo | grep processor | wc -l)
log_file="/tmp/sdk_build_errors.log"
time make firmware -j${NUM_PROCS} 2> >(tee ${log_file})
if [ $? -ne 0 ]
then
    echo ""
    echo "###################################################"
    echo "BUILD ERRORS:"
    echo "###################################################"
    echo ""
    cat ${log_file}
    echo ""
    echo "###################################################"
    echo "Error log file also here: ${log_file}"
    echo "###################################################"
    echo ""
fi
