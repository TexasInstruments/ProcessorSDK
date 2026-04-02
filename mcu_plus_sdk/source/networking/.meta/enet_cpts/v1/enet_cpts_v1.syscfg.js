
let common = system.getScript("/common");

let cpts_input_clk_freq = 200000000;

const cpts_config = [
    {
        clockIds        : [ "SOC_RcmPeripheralId_MSS_CPTS" ],
        clockFrequencies: [
            {
                moduleId: "SOC_RcmPeripheralId_MSS_CPTS",
                clkId   : "SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2",
                clkRate : cpts_input_clk_freq,
            },
        ],
    },
];


function getClockEnableIds(instance) {
    let instConfig = cpts_config;
    return instConfig[0].clockIds;
}

function getClockFrequencies(inst) {

    let instConfig = cpts_config;

    return instConfig[0].clockFrequencies;
}

let enet_cpts_module_name = "/networking/enet_cpts/enet_cpts";

let enet_cpts_module = {

    displayName: "Enet (CPTS)",
	longDescription: "Driver for Common Platform Time Sync Module (CPTS). Supports functionality for Time Synchronization and Time Stamping",
    templates: {
        "/drivers/system/power_clock_config.c.xdt": {
            moduleName: enet_cpts_module_name,
        },
    },
	defaultInstanceName: "CONFIG_ENET_CPTS",
	    config: [

		],
    getClockEnableIds,
    getClockFrequencies,
};

exports = enet_cpts_module;
