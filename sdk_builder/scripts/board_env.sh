# This environment variable is used to select the appropriate board in the name
# of the linux boot and root partitions for scripts in this directory.
# This sets default, but could be overwritten from environment variable.

# This script is for internal use only

if [ -n "${SOC}" ] && [ "${SOC}" = "j7200" ] || [ "${SOC}" = "j721e" ] || [ "${SOC}" = "j721s2" ] || [ "${SOC}" = "j784s4" ] || [ "${SOC}" = "j742s2" ] || [ "${SOC}" = "am62a" ] || [ "${SOC}" = "j722s" ]
then
    echo "SOC env variable set as ${SOC}"
else
    echo "SOC env variable should be one of (j7200, j721e, j721s2, j784s4, j742s2, am62a, j722s)"
    exit 1
fi

if [ -n "${SOC}" ] && [ ${SOC} = "j721s2" ]
then
: "${TI_DEV_BOARD:=j721s2-evm}"
elif [ -n "${SOC}" ] && [ ${SOC} = "j784s4" ]
then
: "${TI_DEV_BOARD:=j784s4-evm}"
elif [ -n "${SOC}" ] && [ ${SOC} = "j742s2" ]
then
: "${TI_DEV_BOARD:=j742s2-evm}"
: "${TISDK_IMAGE:=adas}"
elif [ -n "${SOC}" ] && [ ${SOC} = "j7200" ]
then
: "${TI_DEV_BOARD:=j7200-evm}"
elif [ -n "${SOC}" ] && [ ${SOC} = "j722s" ]
then
: "${TI_DEV_BOARD:=j722s-evm}"
elif [ -n "${SOC}" ] && [ ${SOC} = "am62a" ]
then
: "${TI_DEV_BOARD:=am62a-evm}"
else
: "${TI_DEV_BOARD:=j721e-evm}"
fi

if [ -n "${TISDK_IMAGE}" ] && [ "${TISDK_IMAGE}" = "adas" ] || [ "${TISDK_IMAGE}" = "edgeai" ] || [ "${TISDK_IMAGE}" = "default" ]
then
    echo "TISDK_IMAGE env variable set as ${TISDK_IMAGE}"
else
    echo "TISDK_IMAGE env variable should be one of [default (for J7200), adas (for J721E, J721S2, J784S4, J742S2, J722S EVMs and AM62A SK for Auto-SDK POV), edgeai (for TDA4VM, AM67A, AM68A, AM69A, AM62A SKs)]"
    exit 1
fi
