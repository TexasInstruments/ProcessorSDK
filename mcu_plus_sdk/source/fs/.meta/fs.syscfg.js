let common = system.getScript("/common");

const topModules = [
    "/fs/freertos_fat/freertos_fat",
];

const topModulesNull = [
];

function getTopModules() {
	if((common.getSocName() == "am62x") || (common.getSocName() == "am64x") || (common.getSocName() == "am243x") || (common.getSocName() == "am62ax") || (common.getSocName() == "am62px") || (common.getSocName() =="j722s")) {
		return topModules;
	} else {
		return topModulesNull;
	}
}

exports = {
    displayName: "File System",
    topModules: getTopModules(),
};
