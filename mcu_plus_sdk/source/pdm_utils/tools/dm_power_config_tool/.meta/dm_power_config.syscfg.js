let common = system.getScript("/common");
let socName = common.getSocName();
let isPdk = common.PDK !== undefined && common.PDK === true;
let basePath = isPdk ? "/dm_power_config_tool":"/pdm_utils/tools/dm_power_config_tool";
let pll_module = system.getScript(basePath + "/pll/pll").pet_pll_module;
let custom_pll_module = system.getScript(basePath + "/pll/pll").custom_pll_module;
let psc_module = system.getScript(basePath + "/psc/psc").pet_psc_module;
let custom_psc_module = system.getScript(basePath + "/psc/psc").custom_psc_module;
let templates,maxInstances=1;
let psc_disable_module = system.getScript(basePath + "/psc/psc").psc_disable_module;

if (!isPdk) {
    templates = pll_module.templates; // pll and psc use same templates
}else{
    // template already passed in top level
    maxInstances=3;
}

let topModules={
    name: "dm_power_config",
    templates:{...templates},
    displayName: "DM Power Config",
    maxInstances:maxInstances,
    longDescription:
        "DM Power Config Tool helps user to configure the power and clock settings for desired use case. \
        This tool allows the user to fine-tune the PLL and PSC optimization for the system. \
        In PDK, user can create multiple configurations and select the desired one using Board_ECU API.\
        When an ECU instance or configuration is selected using this tool,\
        the tool generates a corresponding #define. The user needs to pass this generated #define\
        to the Board_ecuInit(uint32_t cfg) function to select the desired configuration.\
        In MCU+SDK, you can only select one configuration which is saved in the syscfg file \
        and corresponding code is generated in ti_power_clock_cfg.c.\
        To learn more about how to add more PSCs and IP frequencies to the tool, please refer to the user guide \
        ",

    defaultInstanceName: "BOARD_ECU_",
    getClockFrequencies: function(inst) {
        return pll_module.getClockFrequencies(inst);
    },
    getClockEnableIds: function(inst) {
        return [...psc_module.getClockEnableIds(inst), ...pll_module.getClockEnableIds(inst)];
    },
    getClockDisableIds: function(inst) {
        return psc_disable_module.getClockDisableIds(inst);
    },
    config: [
        {
        name: "pet_config",
        displayName: "PET Config",
        collapsed:true,
        config: [
            {
                name: "ip_freq_mhz",
                displayName: "IP Frequency (MHz)",
                config: [
                    ...pll_module.config
                ],
            },
            {
                name: "psc_config",
                displayName: "PSC Config",
                config: [
                    ...psc_module.config
                ],
            },
        ],
    },
    {
        name: "custom_config",
        displayName: "Custom Config",
        collapsed:true,
        config: [
            {
                name: "custom_ip_freq_mhz",
                displayName: "Custom IP Frequency (MHz)",
                config: [
                    ...custom_pll_module.config
                ],
            },
            {
                name: "custom_psc_config",
                displayName: "Custom PSC Config",
                config: [
                    ...custom_psc_module.config
                ],
            },
        ],
    },
]
}

if(!isPdk){
    topModules.config.unshift({
        name: "skipDeinitFromSbl",
        displayName: "Skip De-init from SBL (applicable to J722S only)",
        description: `Enable the parameter in case the DM config tool is used in SBL.`,
        default: false,
    });
}

if(isPdk){
    topModules.config.push({
        name: "Advanced Config",
        displayName: "Advanced Config",
        longDescription: `Disables PSC modules based on the configuration. \n Board_ecuDisableClock(uint32_t cfg) is the function which is called to disable PSC.`,
        collapsed:true,
        config:[
            {
                name: "disable_psc_config",
                displayName: "Disable PSC",
                config: [
                    ...psc_disable_module.config
                ],
            },
        ]
    });
}

exports = topModules;