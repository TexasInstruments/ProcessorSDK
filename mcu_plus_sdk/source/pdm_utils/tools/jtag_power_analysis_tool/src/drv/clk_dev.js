const ClkDrv = require("./clk_drv.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvFromDevice extends ClkDrv {
    constructor(clkName) {
        super(clkName);
    }
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

registerDriverClass(ClkDrvFromDevice);

module.exports = {
    ClkDrvFromDevice
};
