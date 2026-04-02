let path = require('path');

const files_main_r5f = {
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
        "vhwa_utils.c",
        "csl_dof.c",
        "csl_sde.c",
        "csl_dmpac_hts.c",
        "csl_dmpacTop.c",
        "vhwa_m2mDofApi.c",
        "vhwa_m2mDofIntr.c",
        "vhwa_m2mDofUdma.c",
        "vhwa_m2mSdeApi.c",
        "vhwa_m2mSdeIntr.c",
        "vhwa_m2mSdeUdma.c",
        "vhwa_dmpac_soc.c"
    ]
};

const includes_main_r5f = {
    common: [
        "soc/j722s",
        "../include",
    ],
};

const filedirs_main_r5f = {
    common: [
        "src/drv",
        "src/csl/src",
        "soc/V2",
        "soc/j722s",
        "../hw_include/dru/v2",
        "ringacc/V0/priv",
    ],
};

const defines_main_r5f = {
    common: [
        "MCU_PLUS_SDK",
        "MAIN_R5",
        "VPAC3L",
        "DMPAC",
        "SOC_J722S",
    ],
};

const cflags = {
    common: [
        "-Wno-address-of-packed-member",
    ],
};

const buildOptionCombos = [
    { device: device, cpu: "main-r5fss0-0", cgt: "ti-arm-clang"},
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

    if(buildOption.cpu == "main-r5fss0-0") {
        build_property.files = files_main_r5f;
        build_property.includes = includes_main_r5f;
        build_property.filedirs = filedirs_main_r5f;
        build_property.defines = defines_main_r5f;
        build_property.cflags = cflags;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
