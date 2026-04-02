#!/bin/bash

C7X_MMA_TIDL_PATH=${C7X_MMA_TIDL_PATH:-""}
BUILD_TYPE=${BUILD_TYPE:-"x86_64"}

if [ "${BUILD_TYPE}" == "x86_64" ]; then
    BUILD_DIR="build_x86_64"
    PLAT_NAME="linux_x86_64"
elif [ "${BUILD_TYPE}" == "aarch64" ]; then
    BUILD_DIR="build_aarch64"
    PLAT_NAME="linux_aarch64"
else
    echo "Invalid BUILD_TYPE=${BUILD_TYPE}. Allowed values are (x86_64, aarch64)"
    exit 1
fi

python3 setup.py build --build-base ${BUILD_DIR} build_ext bdist_wheel --plat-name ${PLAT_NAME}