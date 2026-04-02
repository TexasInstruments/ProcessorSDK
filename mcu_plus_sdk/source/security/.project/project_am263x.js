let path = require('path');

let device = "am263x";

const files_r5f = {
    common: [
        //Taken from crypto library
        "device_ctrl.c",
        "dthe.c",
        "dthe_aes.c",
        "dthe_sha.c",
    ],
};

const filedirs_r5f = {
    common: [
        "crypto",
        "crypto/dthe",
    ],
};

const cflags = {
    common: [
        "-mno-unaligned-access",
        "-Wno-extra",
    ],
};

const buildOptionCombos = [
    { device: device, cpu: "r5f", cgt: "ti-arm-clang"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "security";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;
    property.isSkipTopLevelBuild = true;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    if(buildOption.cpu.match(/r5f*/)) {
        build_property.files = files_r5f;
        build_property.filedirs = filedirs_r5f;
    }
    build_property.cflags = cflags;

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};