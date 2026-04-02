let path = require('path');

let device = "j722s";

const files_wkup_r5f = {
    common: [
        "pdm_utils_clock.c",
        "pdm_utils_pet.c",
        "pdm_utils_psc.c",
        "pdm_utils_pll.c",
        "pdm_utils_soc.c",
    ],
};

const files_mcu_r5f = {
    common: [
        "pdm_utils_pet.c",
        "pdm_utils_psc.c",
        "pdm_utils_pll.c",
        "pdm_utils_soc.c",
    ],
};

const files_main_r5f = {
    common: [
        "pdm_utils_pet.c",
        "pdm_utils_psc.c",
        "pdm_utils_pll.c",
        "pdm_utils_soc.c",
    ],
};

const files_c75ss0 = {
    common: [
        "pdm_utils_pet.c",
        "pdm_utils_psc.c",
        "pdm_utils_pll.c",
        "pdm_utils_soc.c",
    ],
};

const files_c75ss1 = {
    common: [
        "pdm_utils_pet.c",
        "pdm_utils_psc.c",
        "pdm_utils_pll.c",
        "pdm_utils_soc.c",
    ],
};

const filedirs_wkup_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/include/"
    ],
};

const filedirs_mcu_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
    ],
};

const filedirs_main_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
    ],
};

const filedirs_c75ss0 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
    ],
};

const filedirs_c75ss1 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
    ],
};

const defines_wkup_r5 = {
    common: [
        "WKUP_R5",
        "ENABLE_SCICLIENT_DIRECT",
    ],
};

const defines_mcu_r5 = {
    common: [
        "MCU_R5",
    ],
};

const defines_main_r5 = {
    common: [
        "MAIN_R5",
    ],
};

const defines_c75ss0 = {
    common: [
    ],
};

const defines_c75ss1 = {
    common: [
    ],
};

const includes_wkup_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/j722s/r5f",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/include",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/include",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciclient_direct/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/",
    ],
};

const includes_mcu_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/j722s/r5f",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/",
    ],
};

const includes_main_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/j722s/r5f",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/",
    ],
};

const includes_c75ss0 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/j722s/r5f",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/",
    ],
};

const includes_c75ss1 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/j722s/r5f",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/src/soc/j722s",
        "${MCU_PLUS_SDK_PATH}/source/pdm_utils/",
    ],
};

const buildOptionCombos = [
    {device: device, cpu: "wkup-r5fss0-0", cgt: "ti-arm-clang"},
    {device: device, cpu: "mcu-r5fss0-0",  cgt: "ti-arm-clang"},
    {device: device, cpu: "main-r5fss0-0", cgt: "ti-arm-clang"},
    {device: device, cpu: "c75ss0-0",      cgt: "ti-c7000"},
    {device: device, cpu: "c75ss1-0",      cgt: "ti-c7000"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "pdm_utils";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    if(buildOption.cpu.match(/wkup-r5f*/)) {
        build_property.files = files_wkup_r5f;
        build_property.defines = defines_wkup_r5;
        build_property.filedirs = filedirs_wkup_r5f;
        build_property.includes = includes_wkup_r5f;
    }else if(buildOption.cpu.match(/mcu-r5f*/)) {
        build_property.files = files_mcu_r5f;
        build_property.defines = defines_mcu_r5;
        build_property.filedirs = filedirs_mcu_r5f;
        build_property.includes = includes_mcu_r5f;
    }else if(buildOption.cpu.match(/main-r5f*/)) {
        build_property.files = files_main_r5f;
        build_property.defines = defines_main_r5;
        build_property.filedirs = filedirs_main_r5f;
        build_property.includes = includes_main_r5f;
    }else if(buildOption.cpu.match(/c75ss0-0*/)) {
        build_property.files = files_c75ss0;
        build_property.defines = defines_c75ss0;
        build_property.filedirs = filedirs_c75ss0;
        build_property.includes = includes_c75ss0;
    }else if(buildOption.cpu.match(/c75ss1-0*/)) {
        build_property.files = files_c75ss1;
        build_property.defines = defines_c75ss1;
        build_property.filedirs = filedirs_c75ss1;
        build_property.includes = includes_c75ss1;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
