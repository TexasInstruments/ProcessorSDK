
let common = system.getScript("/common");
let soc = system.getScript(`/drivers/soc/drivers_${common.getSocName()}`);

function getModule() {

    let driverVer = soc.getDriverVer("spinlock");

    return system.getScript(`/drivers/spinlock/${driverVer}/spinlock_${driverVer}`);
}

exports = getModule();
