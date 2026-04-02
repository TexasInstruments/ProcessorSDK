const common = require("../common.js");

const component_file_list = [
    "source/board/.project/project.js",
    "source/drivers/.project/project.js",
    "source/drivers/device_manager/self_reset/.project/project.js",
    "source/drivers/device_manager/sciclient_direct/.project/project.js",
    "source/drivers/device_manager/sciclient_direct/sbl/.project/project.js",
    "source/drivers/device_manager/sciserver/.project/project.js",
    "source/fs/freertos_fat/.project/project.js",
    "source/kernel/nortos/.project/project.js",
    "source/kernel/freertos/.project/project.js",
    ".internal/docs/api_guide/doxy_samples/.project/project.js",
    "test/unity/.project/project.js",
];

// List of components where makefile is not generated.
const component_file_list_with_makefile = [
    "source/drivers/device_manager/rm_pm_hal/.project/project.js",
    "source/drivers/device_manager/rm_pm_hal/sbl/.project/project.js",
];

const device_defines = {
    common: [
        "SOC_AM62PX",
    ],
};

const example_file_list = [
    "examples/drivers/boot/sbl_emmc_linux_multistage/sbl_emmc_linux_stage1/.project/project.js",
    "examples/drivers/boot/sbl_emmc_linux_multistage/sbl_emmc_linux_stage2/.project/project.js",
    "examples/drivers/boot/sbl_null/.project/project.js",
    "examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage1/.project/project.js",
    "examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage2/.project/project.js",
    "examples/drivers/boot/sbl_uart/.project/project.js",
    "examples/drivers/boot/sbl_uart_linux/.project/project.js",
    "examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage1/.project/project.js",
    "examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage2/.project/project.js",
    "examples/drivers/dss/dss_colorbar_test/.project/project.js",
    "examples/drivers/dss/dss_display_test/.project/project.js",
    "examples/drivers/gpio/gpio_input_interrupt/.project/project.js",
    "examples/drivers/gpio/gpio_led_blink/.project/project.js",
    "examples/drivers/i2c/i2c_led_blink/.project/project.js",
    "examples/drivers/i2c/i2c_read/.project/project.js",
    "examples/drivers/i2c/i2c_temperature/.project/project.js",
    "examples/drivers/ipc/ipc_notify_echo/.project/project.js",
    "examples/drivers/ipc/ipc_rpmsg_echo/.project/project.js",
    "examples/drivers/ipc/ipc_rpmsg_echo_linux/.project/project.js",
    "examples/drivers/ipc/ipc_rpmsg_echo_qnx/.project/project.js",
    "examples/drivers/mcan/mcan_loopback_polling/.project/project.js",
    "examples/drivers/mcan/mcan_loopback_interrupt/.project/project.js",
    "examples/drivers/mcasp/mcasp_early_chime/.project/project.js",
    "examples/drivers/mcspi/mcspi_loopback/.project/project.js",
    "examples/drivers/mcspi/mcspi_performance_8bit/.project/project.js",
    "examples/drivers/mcspi/mcspi_performance_32bit/.project/project.js",
    "examples/drivers/mmcsd/mmcsd_raw_io/.project/project.js",
    "examples/drivers/ospi/ospi_flash_io/.project/project.js",
    "examples/drivers/safety/reset_isolation/.project/project.js",
    "examples/drivers/sciclient/sciclient_get_version/.project/project.js",
    "examples/drivers/uart/uart_echo/.project/project.js",
    "examples/drivers/uart/uart_echo_callback/.project/project.js",
    "examples/drivers/uart/uart_echo_low_latency_interrupt/.project/project.js",
    "examples/drivers/uart/uart_echo_low_latency_polling/.project/project.js",
    "examples/drivers/udma/udma_chaining/.project/project.js",
    "examples/drivers/udma/udma_memcpy_interrupt/.project/project.js",
    "examples/drivers/udma/udma_memcpy_polling/.project/project.js",
    "examples/drivers/udma/udma_sw_trigger/.project/project.js",
    "examples/empty/.project/project_freertos.js",
    "examples/empty/.project/project_nortos.js",
    "examples/hello_world/.project/project.js",
    "examples/hello_world_cpp/.project/project.js",
    "examples/kernel/dpl/dpl_demo/.project/project.js",
    "examples/kernel/freertos/posix_demo/.project/project.js",
    "examples/kernel/freertos/task_switch/.project/project.js",

    // Drivers and kernel tests
    "test/board/flash/.project/project.js",
    "test/board/led/.project/project.js",
    "test/drivers/ddr/.project/project.js",
    "test/drivers/gpio/.project/project.js",
    "test/drivers/gtc/.project/project.js",
    "test/drivers/mmcsd/.project/project.js",
    "test/drivers/sciclient/.project/project.js",
    "test/drivers/i2c/.project/project.js",
    "test/drivers/ipc_notify/.project/project.js",
    "test/drivers/ipc_rpmsg/.project/project.js",
    "test/drivers/ospi/.project/project.js",
    "test/drivers/uart/.project/project.js",
    "test/kernel/dpl/.project/project.js",
    "test/kernel/freertos/.project/project.js",
    "test/kernel/rov/.project/project.js",
];

function getProjectSpecCpu(cpu) {
    let projectSpecCpu =
    {
        "mcu-r5fss0-0": "MCUSS_Cortex_R5_0_0",
        "wkup-r5fss0-0": "WKUP_Cortex_R5_0_0",
        "a53ss0-0": "CortexA53_0",
        "a53ss0-1": "CortexA53_1",
        "a53ss1-0": "CortexA53_2",
        "a53ss1-1": "CortexA53_3",
    }

    return projectSpecCpu[cpu];
}

function getComponentList() {
    return component_file_list;
}

function getComponentListWithMakefile() {
    return component_file_list_with_makefile;
}

function getExampleList() {
    return example_file_list;
}

function getSysCfgDevice(board) {
    return "AM62Px";
}

function getProjectSpecDevice(board) {
    return "AM62Px";
}

function getSysCfgCpu(cpu) {
    return cpu;
}

function getSysCfgPkg(board) {
    return "AMH";
}

function getSysCfgPart(board) {
    return "Default";
}

function getDevToolTirex(board) {
    return "AM62Px_SK_EVM";
}

function getProperty() {
    let property = {};

    property.defines = device_defines;

    return property;
}

function getLinuxFwName(cpu) {

    switch(cpu) {
        case "mcu-r5fss0-0":
            return "mcu-r5f0_0";
        case "wkup-r5fss0-0":
            return "wkup-r5f0_0";
    }
    return undefined;
}

function getProductNameProjectSpec() {
    return "MCU_PLUS_SDK_AM62PX";
}

function getFlashAddr() {
    return 0x60000000;
}

module.exports = {
    getComponentList,
    getComponentListWithMakefile,
    getExampleList,
    getSysCfgDevice,
    getSysCfgCpu,
    getSysCfgPkg,
    getSysCfgPart,
    getProjectSpecDevice,
    getProjectSpecCpu,
    getDevToolTirex,
    getProperty,
    getLinuxFwName,
    getProductNameProjectSpec,
    getFlashAddr,
};
