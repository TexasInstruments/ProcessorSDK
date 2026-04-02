const ClkDrv = require("./clk_drv.js");
const {registerDriverClass, getDriverClass} = require("./driver_registry.js");
const PllDrv = require("./pll.js");

class ClkDrvPll extends ClkDrv {
    constructor(clkName) {
        super(clkName);
    }

    /**
     * Retrieves the parent clock and the VCO frequency multiplier.
     *
     * @return {Object} An object containing the VCO frequency multiplier and the parent clock.
     */
    getParentClock() {
        // get parent from socClkData and div value from PllDrv
        let parent = this.socData.socClkData[this.clkName].parent.clk_name;
        const pllInst = this.socData.socPllData[this.socData.socClkData[this.clkName].data];

        const pllDrv = new PllDrv.PllDrv();

        let divs = pllDrv.getPllDivs(pllInst);

        return {mul: divs.vcoFreqMul, parent: parent};
    }
}

class ClkDrvPllDeskew extends ClkDrvPll {
    getParentClock() {
        // get parent from socClkData and div value from PllDrv
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

registerDriverClass(ClkDrvPll, "ClkDrvPll16fft");
registerDriverClass(ClkDrvPllDeskew);

module.exports = ClkDrvPll;
