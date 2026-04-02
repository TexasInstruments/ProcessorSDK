let path = require('path');

let device = "am243x";

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
const filedirs_evm = {
    common: [
        "..",       /* core_os_combo base */
        "../../..", /* Example base */
        "../../../..",
        "../../../../appHw/eeprom",
        "../../../../appHw/led/am64xx_am243x",
        "../../../../customPhy/src",
        "../../../../appHw/phy/am243x_evm",
    ],
};

const filedirs_lp = {
    common: [
        "..",       /* core_os_combo base */
        "../../..", /* Example base */
        "../../../..",
        "../../../../appHw/eeprom",
        "../../../../appHw/led/am64xx_am243x",
        "../../../../customPhy/src",
        "../../../../appHw/phy/am243x_lp",
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

const includes_freertos_r5f_evm = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am243x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/inc",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/api",
        "${MCU_PLUS_SDK_PATH}/source",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/led/am64xx_am243x",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom/am243x_evm",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/customPhy/inc",
    ],
};

const includes_freertos_r5f_lp = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am243x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/inc",
        "${MCU_PLUS_SDK_PATH}/source/industrial_comms/profinet_device/stack/api",
        "${MCU_PLUS_SDK_PATH}/source",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/led/am64xx_am243x",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/appHw/eeprom/am243x_lp",
        "${MCU_PLUS_SDK_PATH}/examples/industrial_comms/profinet_device_demo/customPhy/inc",
    ],
};

const libs_freertos_r5f_evm = {
    common: [
        "freertos.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "drivers.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "board.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "icss_emac.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinet_device_rt_mrp_mii_icss_fwhal.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinetiodevice_mii.am243x-evm.r5f.ti-arm-clang.release.lib",
    ],
};

const libs_freertos_r5f_lp = {
    common: [
        "freertos.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "drivers.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "board.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "icss_emac.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinet_device_rt_mrp_mii_icss_fwhal.am243x.r5f.ti-arm-clang.${ConfigName}.lib",
        "profinetiodevice_mii.am243x-lp.r5f.ti-arm-clang.release.lib",
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
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am243x-evm", os: "freertos"},
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am243x-lp", os: "freertos"},
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
    build_property.lnkfiles = lnkfiles;
    build_property.syscfgfile = syscfgfile;
    build_property.readmeDoxygenPageTag = readmeDoxygenPageTag;
    build_property.projecspecFileAction = "link";

    if(buildOption.cpu.match(/r5f*/)) {
        if(buildOption.os.match(/freertos*/) )
        {
            build_property.libdirs = libdirs_freertos;
            build_property.defines = defines_r5f;
            if(buildOption.board.match(/am243x-evm*/) )
            {
                build_property.includes = includes_freertos_r5f_evm;
                build_property.filedirs = filedirs_evm;
                build_property.libs = libs_freertos_r5f_evm;
            }
            else if(buildOption.board.match(/am243x-lp*/) )
            {
                build_property.includes = includes_freertos_r5f_lp;
                build_property.filedirs = filedirs_lp;
                build_property.libs = libs_freertos_r5f_lp;
            }
        }
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
