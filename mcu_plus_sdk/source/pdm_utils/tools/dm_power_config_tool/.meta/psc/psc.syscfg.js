let common = system.getScript("/common");
let soc = common.getSocName();
let isPdk = common.PDK !== undefined;
const basePath = isPdk ? "/config/" + soc:"/pdm_utils/tools/config/" + soc;
const pathPetPscMapping = basePath + "/pet_psc_list.json";
const pathCustomPscMapping = basePath + "/custom_psc_list.json";
const petpscMap = system.getScript(pathPetPscMapping);
const customPscMap = system.getScript(pathCustomPscMapping);


function getConfig(pscMap){
    let uiConfig =[]

    for (const pscObj in pscMap) {
		const pscDisplayName = pscMap[pscObj].displayName;
		const psc_name = pscDisplayName.replace(/\s+/g, "_");
		uiConfig.push({
			name: psc_name,
			displayName: pscDisplayName,
			default: [],
			minSelections: 0,
			options: pscMap[pscObj].tisci_ip_name_list.map(function (ip_name) {
				return {
					name: ip_name,
					displayName: ip_name.replace("TISCI_DEV_",""),
				};
			}),
		});
	}
    return uiConfig;
}

function getDisableConfig(pscMap){
    let uiConfig =[]

    for (const pscObj in pscMap) {
		const pscDisplayName = pscMap[pscObj].displayName;
		const psc_name = pscDisplayName.replace(/\s+/g, "_");
		uiConfig.push({
			name: "disable_" + psc_name,
			displayName: pscDisplayName + " (Disable)",
			default: [],
			minSelections: 0,
			options: pscMap[pscObj].tisci_ip_name_list.map(function (ip_name) {
				return {
					name: ip_name,
					displayName: ip_name.replace("TISCI_DEV_",""),
				};
			}),
		});
	}
    return uiConfig;
}

function getClockEnableIds(inst) {
    let clockIds = [];

    for (const key in inst) {
		/* skip $ prefix key, like $name */
		if (inst.hasOwnProperty(key) && !key.startsWith("$") && !key.startsWith("disable_") && Array.isArray(inst[key]) && inst[key].length > 0) {
			clockIds.push(...inst[key]);
		}
	}

    return clockIds;
}

function getClockDisableIds(inst) {
	let clockIds = [];

    for (const key in inst) {
		/* skip $ prefix key, like $name, and only process disable_ prefixed keys */
		if (Object.hasOwnProperty.call(inst, key) && !key.startsWith("$") && key.startsWith("disable_") && Array.isArray(inst[key]) && inst[key].length > 0) {
			clockIds.push(...inst[key]);
		}
	}

    return clockIds;
}

function getTemplates() {
	/* use same template as system */
	return {
		"/drivers/system/power_clock_config.c.xdt": {
			moduleName: "/pdm_utils/tools/dm_power_config_tool/dm_power_config",
		},
	};
}

function getClockFrequencies(inst) {
    let clockFreq = [];

    return clockFreq;
}


let pet_psc_module = {
    displayName: "PSC",
    config: getConfig(petpscMap),
    defaultInstanceName: "psc_clock_cfg",
    maxInstances: 1,
    templates: getTemplates(),
    getClockEnableIds: getClockEnableIds,
    getClockFrequencies: getClockFrequencies,

};

let psc_disable_module = {
    displayName: "PSC Disable",
    config: getDisableConfig(petpscMap).concat(getDisableConfig(customPscMap)),
    defaultInstanceName: "psc_disable_cfg",
    maxInstances: 1,
    templates: getTemplates(),
    getClockDisableIds: getClockDisableIds,
    getClockFrequencies: getClockFrequencies,
};

let custom_psc_module = {
    displayName: "Custom PSC",
    config: getConfig(customPscMap),
    defaultInstanceName: "psc_clock_cfg",
    maxInstances: 1,
    templates: getTemplates(),
    getClockEnableIds: getClockEnableIds,
    getClockFrequencies: getClockFrequencies,

};
exports = {
    pet_psc_module,
	psc_disable_module,
    custom_psc_module
};
