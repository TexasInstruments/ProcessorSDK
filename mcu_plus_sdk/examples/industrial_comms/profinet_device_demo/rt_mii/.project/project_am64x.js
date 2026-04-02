let path = require('path');

let device = "am64x";

const files = {
    common: [
        "appHwBoardInfo.c",
        "appHwLed.c",
        "appLed.c",
        "appPermanentData.c",
        "appRealIdentification.c",
        "appStackConfigure.c",
        "appStackInit.c",
        "appUserInfo.c",
        "main.c",
        "CUST_PHY_base.c",
        "CUST_PHY_dp83869.c",
        "appPhyReset.c",
    ],
};

/* Relative to where the makefile will be generated
 * Typically at <example_folder>/<BOARD>/<core_os_combo>/<compiler>
 */
const filedirs = {
    common: [
        "..",       /* core_os_combo base */
        "../../..", /* Example base */
        "../../../..",
        "../../../../appHw/eeprom",
        "../../../../appHw/led/am64xx_am243x",
        "../../../../customPhy/src",
        "../../../../appHw/phy/am64xx_evm",
    ],
};

const libdirs_freertos = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/lib",
        "${MCU_PLUS_SDK_PATH}/source/board/lib",
        "${MCU_PLUS_SDK_PATH}/source/networking/icss_emac/lib",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/icss_fwhal/lib",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack",
    ],
};

const includes_freertos_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am64x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/inc",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/api",
        "${MCU_PLUS_SDK_PATH}/source",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/led/am64xx_am243x",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom/am64xx_evm",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/customPhy/inc",
    ],
};

const libs_freertos_r5f = {
    common: [
        "freertos.am64x.r5f.ti-arm-clang.${ConfigName}.lib",
        "drivers.am64x.r5f.ti-arm-clang.${ConfigName}.lib",
        "board.am64x.r5f.ti-arm-clang.${ConfigName}.lib",
        "icss_emac.am64x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinet_device_rt_mrp_mii_icss_fwhal.am64x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinetiodevice_mii.am64x.r5f.ti-arm-clang.release.lib",
    ],
};

const defines_r5f = {
    common: [
    ],
};

const lnkfiles = {
    common: [
        "linker.cmd",
    ]
};

const syscfgfile = "../example.syscfg";

const readmeDoxygenPageTag = "EXAMPLES_INDUSTRIAL_COMMS_PROFINET_DEVICE_DEMOS";

const buildOptionCombos = [
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am64x-evm", os: "freertos"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "executable";
    property.name = "profinet_device_rt_mii_demo";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.files = files;
    build_property.filedirs = filedirs;
    build_property.lnkfiles = lnkfiles;
    build_property.syscfgfile = syscfgfile;
    build_property.readmeDoxygenPageTag = readmeDoxygenPageTag;
    build_property.projecspecFileAction = "link";

    if(buildOption.cpu.match(/r5f*/)) {
        if(buildOption.os.match(/freertos*/) )
        {
            build_property.includes = includes_freertos_r5f;
            build_property.libdirs = libdirs_freertos;
            build_property.libs = libs_freertos_r5f;
            build_property.defines = defines_r5f;
        }
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
