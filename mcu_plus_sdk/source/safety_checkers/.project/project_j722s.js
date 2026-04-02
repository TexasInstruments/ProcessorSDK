let path = require('path');

let device = "j722s";

const files = {
    common: [
        "safety_checkers_pm.c",
		"safety_checkers_rm.c",
        "safety_checkers_tifs.c",
        "safety_checkers_csirx.c",
        "safety_checkers_csirx_soc.c",
    ],
};

const filedirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src",
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/soc/j722s",
    ],
};

const includedirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/",
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/soc/",
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/csirx/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/hw_include/",
        "${MCU_PLUS_SDK_PATH}/source/kernel/nortos/dpl/r5/",
    ],
};

const buildOptionCombos = [
    { device: device, cpu: "r5f", cgt: "ti-arm-clang"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "safety_checkers";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.filedirs = filedirs;
    build_property.files = files;
    build_property.includes = includedirs;

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
