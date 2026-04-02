/*
DM power config tool can run standalone from PDK or as a part of MCU+SDK sysconfig infrastructure. If it is invoked from PDK
then board_ec.c/h is generated using xdt templates. If it is invoked from MCU+SDK then ti_power_clock_cfg.c is generated using
syscfg templates already present in MCU+SDK.
*/
let common = system.getScript("/common");
let socName = common.getSocName();
//isPdk is a boolean variable which is set to true if the tool is invoked from PDK and undefined if invoked from MCU+SDK
let isPdk = common.PDK !== undefined && common.PDK === true;
let basePath = isPdk ? "/dm_power_config_tool":"/pdm_utils/tools/dm_power_config_tool";
let templates = [];

if (isPdk){
    templates=[
        {
            name: "dm_power_config_tool/templates/board_ecu.h.xdt",
            outputPath: "include/board_ecu.h",
            alwaysRun: true,
        },
        {
            name: "dm_power_config_tool/templates/board_ecu.c.xdt",
            outputPath: "board_ecu.c",
            alwaysRun: true,
        },
    ]
}

exports = {
	displayName: "DM Power Config",
	templates: templates,
	topModules: [basePath + "/dm_power_config.syscfg.js"],
}

