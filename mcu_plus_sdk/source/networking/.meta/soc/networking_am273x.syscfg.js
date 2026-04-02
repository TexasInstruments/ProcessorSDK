
let common = system.getScript("/common");

const topModules = [
    "/networking/enet_cpsw/enet_cpsw",
    "/networking/enet_cpts/enet_cpts",
];

const driverVer = {
    "enet_cpsw": {
        version: "v1",
    },
	"enet_cpts": {
        version: "v1",
    },
}

exports = {
    getTopModules: function() {
        return topModules;
    },
    getDriverVer: function(driverName) {
        return driverVer[driverName].version;
    },
};
