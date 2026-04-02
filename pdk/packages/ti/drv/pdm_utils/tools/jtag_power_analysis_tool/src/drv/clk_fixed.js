const ClkDrv = require("./clk_drv.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvFixed extends ClkDrv {
    constructor(clkName) {
        super(clkName);
    }
    getParentClock() {
        let mul = 0;
        // Hacky way to get the multiplier
        if (this.clkName.includes("32K")) {
            mul = 32 / 1000;
        } else if (this.clkName.includes("12P5M") || this.clkName.includes("RCOSC_CLKOUT")) {
            mul = 12.5;
        } else if (this.clkName.includes("97P65K")) {
            mul = 97.65 / 1000;
        } else if (this.clkName.includes("LPXOSC")) {
            mul = 32768 / 1000000;
        } else {
            console.warn("Fixed clock frequency unknown", this.clkName);
        }

        return {mul: mul, parent: null};
    }
}

registerDriverClass(ClkDrvFixed);

module.exports = {ClkDrvFixed};
