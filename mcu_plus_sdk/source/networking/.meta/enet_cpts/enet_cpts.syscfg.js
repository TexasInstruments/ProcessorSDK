
let common = system.getScript("/common");
let soc = system.getScript(`/networking/soc/networking_${common.getSocName()}`);

function getModule() {

    let driverVer = soc.getDriverVer("enet_cpts");

    return system.getScript(`/networking/enet_cpts/${driverVer}/enet_cpts_${driverVer}`);
}

exports = getModule();