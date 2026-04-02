let path = require('path');

let device = "am62px";


const files_r5f={
    common: [
        "safety_checkers_pm.c",
		"safety_checkers_rm.c",
        "safety_checkers_tifs.c",
    ],
};

const filedirs = {
    common: [
		"${MCU_PLUS_SDK_PATH}/source/safety_checkers/src",
    ],
};

const includedirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/",
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/soc/",
        "${MCU_PLUS_SDK_PATH}/source/safety_checkers/src/soc/am62px/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/hw_include/",
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
    build_property.includes = includedirs;
    if(buildOption.cpu.match(/r5f*/))
    {
        build_property.files = files_r5f;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
