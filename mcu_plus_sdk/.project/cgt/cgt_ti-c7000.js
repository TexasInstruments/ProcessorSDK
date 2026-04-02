
const common = require(`../common.js`);

const cgt_common = {
    path: "CGT_TI_C7000_PATH",
    cc: "cl7x",
    ar: "ar7x",
    lnk: "cl7x",
    strip: "strip7x",
    objcopy: "",

    includes: {
        common: [
            "${CG_TOOL_ROOT}/include",
            "${MCU_PLUS_SDK_PATH}/source"
        ],
    },
    cflags: {
        common: [
            "-mv7524",
            "--abi=eabi",
            "-q",
            "-mo",
            "-pden",
            "-pds=238",
            "-pds=880",
            "-pds1110",
            "--endian=little",
            "--disable_inlining",
            "-ea.se71",
            "--emit_warnings_as_errors",
            "--diag_suppress=770", // to suppress pointer to small int conversion error
            "--diag_suppress=69", // to suppress -1 to uint32t error
            "--diag_suppress=70", // to suppress char pointer to uint32t error
            "--advice:performance=none", // to suppress optimization advice
        ],
        debug: [
            "-Dxdc_target_name__=C71",
            "-D_DEBUG_=1",
            //"-Dxdc_target_types__=ti/targets/elf/std.h",
            "--opt_level=0",
            "--symdebug:dwarf",
            "-DBUILD_C75X",
        ],
        release: [
            "-Dxdc_target_name__=C71",
            //"-Dxdc_target_types__=ti/targets/elf/std.h",
            "--opt_level=3",
            // "--symdebug:none",
            "-DBUILD_C75X",
        ],
    },
    arflags: {
        common: [
            "rq",
        ],
    },
    lflags: {
        common: [
            "--silicon_version=7524",
            "-z",
            "--emit_warnings_as_errors",
            "--diag_suppress=10063",
            "--ram_model",
            "--warn_sections",
            "-q",
            "-x",
            ""
        ],
    },
    libdirs: {
        common: [
            "${CG_TOOL_ROOT}/lib",
        ],
    },
    libs: {
        common: [
            "libc.a",
        ],
    },
};

const cgt_common_c75ss0 = {
    cflags: {
        common: [
            "-DBUILD_C75X_1",
        ],
    },
};

const cgt_common_c75ss1 = {
    cflags: {
        common: [
            "-DBUILD_C75X_2",
        ],
    },
};

const cgt_common_freertos = {
    cflags: {
        release: [
            "--symdebug:none",
        ],
    },
}

function getCgtOptions(cpu,os)
{
    let cgtOptions = cgt_common;

    if(cpu.match(/c75ss0-0*/))
    {
        cgtOptions = common.mergeCgtOptions(cgt_common, cgt_common_c75ss0);
    }
    else if(cpu.match(/c75ss1-0*/))
    {
        cgtOptions = common.mergeCgtOptions(cgt_common, cgt_common_c75ss1);
    }
    else
    {
        cgtOptions = cgt_common;
    }

    if(buildOption.os === undefined || buildOption.os.match(/freertos/)){
        cgtOptions = common.mergeCgtOptions(cgtOptions, cgt_common_freertos);
    }

    return cgtOptions;
}

module.exports = {

    getCgtOptions,
};
