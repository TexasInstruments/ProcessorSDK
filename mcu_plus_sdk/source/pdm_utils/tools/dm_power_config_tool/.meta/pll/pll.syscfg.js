let common = system.getScript("/common");
let soc = common.getSocName();
let isPdk = common.PDK !== undefined;
const basePath = isPdk ? "/config/" + soc:"/pdm_utils/tools/config/" + soc;
const pathPetIpMapping = basePath + "/pet_ip_mapping.json";
const pathCustomIpMapping = basePath + "/custom_ip_mapping.json";
const customIpMap = system.getScript(pathCustomIpMapping);
const petIpMap = system.getScript(pathPetIpMapping);

function getConfig(ipMap){
    let uiConfig =[]

	for (const ipDisplayName in ipMap) {
		let defaultFreqMhz = ipMap[ipDisplayName]?.default_freq_mhz ?? 0;
		let programmable = ipMap[ipDisplayName].programmable ?? false;
		let ip_name = ipDisplayName.replace(/\s+/g, "_");
		uiConfig.push({
			name: ip_name,
			displayName: ipDisplayName,
            collapsed: true,
            config:[
                {   name: "enable_"+ip_name,
                    displayName: "Custom Clock Enable",
                    default: true,
                    readOnly: !programmable,
                },
                {
                    name: ip_name,
			        displayName: ipDisplayName,
			        readOnly: !programmable,
			        default: defaultFreqMhz,
                }
            ]
		});
	}

    return uiConfig;
}

function getClockEnableIds(inst) {
    let clockEnableIds = [];
    /* If Lpsc is enabled is true, then add its psc and dependent psc */
    for (const ipDisplayName in petIpMap) {
		let ip_name = ipDisplayName.replace(/\s+/g, "_");
		if (petIpMap[ipDisplayName].enableLpsc==true && petIpMap[ipDisplayName].tisci_ip_name != "" &&
            inst[ip_name] > 0 && inst["enable_"+ip_name]) {
            let dependencies = petIpMap[ipDisplayName].dependent_psc_list;
			if (dependencies) {
				clockEnableIds.push(...dependencies);
			}
            clockEnableIds.push(petIpMap[ipDisplayName].tisci_ip_name);
		}
	}

      for (const ipDisplayName in customIpMap) {
		let ip_name = ipDisplayName.replace(/\s+/g, "_");
		if (customIpMap[ipDisplayName].enableLpsc==true && customIpMap[ipDisplayName].tisci_ip_name != "" &&
            inst[ip_name] > 0 && inst["enable_"+ip_name]) {
            let dependencies = customIpMap[ipDisplayName].dependent_psc_list;
			if (dependencies) {
				clockEnableIds.push(...dependencies);
			}
            clockEnableIds.push(customIpMap[ipDisplayName].tisci_ip_name);
		}
	}

    return clockEnableIds;
}

function getClockFrequencies(inst) {
	// return an array of {moduleId, clkId, clkRate} for each programmable clock,
	//  get the user defined frequency from inst
	let clockFreq = [];

	for (const ipDisplayName in petIpMap) {
		let ip_name = ipDisplayName.replace(/\s+/g, "_");
		if (petIpMap[ipDisplayName].programmable && inst[ip_name] > 0 && inst["enable_"+ip_name]) {
			clockFreq.push({
				moduleId: petIpMap[ipDisplayName].tisci_ip_name,
				clkId: petIpMap[ipDisplayName].tisci_input_name,
				clkRate: inst[ip_name] * 1000000,
			});
		}
	}

    	for (const ipDisplayName in customIpMap) {
		let ip_name = ipDisplayName.replace(/\s+/g, "_");
		if (customIpMap[ipDisplayName].programmable && inst[ip_name] > 0 && inst["enable_"+ip_name]) {
			clockFreq.push({
				moduleId: customIpMap[ipDisplayName].tisci_ip_name,
				clkId: customIpMap[ipDisplayName].tisci_input_name,
				clkRate: inst[ip_name] * 1000000,
			});
		}
	}

	return clockFreq;
}
function getTemplates() {
	// use same template as system
	return {
		"/drivers/system/power_clock_config.c.xdt": {
			moduleName: "/pdm_utils/tools/dm_power_config_tool/dm_power_config",
		},
	};
}

let pet_pll_module = {
	displayName: "IP Frequency (MHz)",
	defaultInstanceName: "PowerConfig",
    templates: getTemplates(),
	config:getConfig(petIpMap),
	getClockFrequencies: getClockFrequencies,
	getClockEnableIds: getClockEnableIds,
};

let custom_pll_module = {
	displayName: "IP Frequency (MHz)",
	defaultInstanceName: "PowerConfig",
    templates: getTemplates(),
	config:getConfig(customIpMap),
	getClockFrequencies: getClockFrequencies,
	getClockEnableIds: getClockEnableIds,
};

exports = {
	pet_pll_module,
	custom_pll_module
};
