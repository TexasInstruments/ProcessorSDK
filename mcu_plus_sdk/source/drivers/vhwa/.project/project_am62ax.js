let path = require('path');

const files_r5f = {
    common: [
        "vhwa_m2mLdcApi.c",
        "vhwa_m2mLdcIntr.c",
        "vhwa_m2mLdcUdma.c",
        "vhwa_m2mMscApi.c",
        "vhwa_m2mMscIntr.c",
        "vhwa_m2mMscUdma.c",
        "vhwa_m2mVissApi.c",
        "vhwa_m2mVissCsl.c",
        "vhwa_m2mVissPriv.c",
        "vhwa_m2mVissIntr.c",
        "vhwa_m2mVissUdma.c",
        "csl_ee.c",
        "csl_glbce.c",
        "csl_h3a.c",
        "csl_vpacTop.c",
        "csl_cac.c",
        "csl_fcp.c",
        "csl_hts.c",
        "csl_ldc.c",
        "csl_lse.c",
        "csl_msc.c",
        "csl_nsf4.c",
        "csl_pcid.c",
        "csl_rawfe.c",
        "csl_visstop.c",
        "vhwa_vpac_soc.c",
        "vhwa_soc_fc.c",
        "vhwa_utils.c"
    ]
};

const includes_r5f = {
    common: [
        "soc/am62ax",
        "../include",
    ],
};

const filedirs_r5f = {
    common: [
        "src/drv",
        "src/csl/src",
        "soc/V2",
        "soc/am62ax",
    ],
};

const defines_r5f = {
    common: [
        "MCU_PLUS_SDK",
        "BUILD_MCU1_0",
        "VPAC3L",
        "SOC_AM62A",
        "SOC_AM62AX",
    ],
};

const cflags = {
    common: [
        "-Wno-address-of-packed-member",
    ],
};

const buildOptionCombos = [
    { device: device, cpu: "r5f", cgt: "ti-arm-clang"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "vhwa";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    if(buildOption.cpu == "r5f") {
        build_property.files = files_r5f;
        build_property.includes = includes_r5f;
        build_property.filedirs = filedirs_r5f;
        build_property.defines = defines_r5f;
        build_property.cflags = cflags;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
