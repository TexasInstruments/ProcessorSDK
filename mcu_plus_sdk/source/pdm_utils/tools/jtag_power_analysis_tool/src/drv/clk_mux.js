const ClkDrv = require("./clk_drv.js");
const {readMmr, readMmrField} = require("./common.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvMuxReg extends ClkDrv {
    constructor(clkName) {
        super(clkName);
    }
    /**
     * Retrieves the parent clock and its divisor for a given clock.
     *
     * @return {Object} An object containing the divisor and parent clock.
     */
    getParentClock() {
        let parent = null;
        let div = 0xffffffff;
        // get list of parents fron socClkDataParent, get the mux setting return parentList[index]
        const clkName = this.clkName;
        const clkData = this.socData.socClkData[clkName];
        const muxRegName = clkData.data;
        const muxData = this.socData.socClkDataMux[muxRegName];
        const parentsList = this.socData.socClkDataParent[muxData.parents];

        let mmrResult = this.getMuxSelSetting(muxData);

        if (mmrResult.status >= 0) {
            let index = mmrResult.value;
            parent = parentsList[index].clk_name;
            if (parent != "0") {
                // mux is connected to any clock
                div = parentsList[index].div;
            }
        }

        return {div, parent}; //return div and parent
    }
    /**
     * Retrieves the mux selection setting from the given mux data.
     *
     * @param {Object} muxData - The mux data.
     * @return {Object} An object containing the mux selection setting and the status.
     */
    getMuxSelSetting(muxData) {
        // Mux setting that will tell which input is selected
        let reg = parseInt(muxData.reg, 16);
        let bitStart = Number(muxData.start_bit);
        let bitEnd = bitStart + muxData.bitfield_width - 1;

        let mmrResult = readMmrField(reg, bitStart, bitEnd);

        return {
            value: mmrResult.value,
            status: mmrResult.status
        };
    }
}

class ClkDrvMuxDpi0Pclk extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

class ClkDrvMuxDpi1Clk extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

class ClkDrvMuxDpi2Pclk extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

class ClkDrvMuxDpi3Pclk extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

class ClkDrvMuxRegJ7Ahclko extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);
        return {mul: 0, parent: null};
    }
}

registerDriverClass(ClkDrvMuxReg);
registerDriverClass(ClkDrvMuxDpi0Pclk, "ClkDrvMuxJ7Dpi0Pclk");
registerDriverClass(ClkDrvMuxDpi1Clk, "ClkDrvMuxJ7Dpi1ClkOut");
registerDriverClass(ClkDrvMuxDpi2Pclk, "ClkDrvMuxJ7Dpi2Pclk");
registerDriverClass(ClkDrvMuxDpi3Pclk, "ClkDrvMuxRegJ7Dpi3Pclk");
registerDriverClass(ClkDrvMuxRegJ7Ahclko, "ClkDrvMuxJ7Ahclkout");

module.exports = {
    ClkDrvMuxReg,
    ClkDrvMuxDpi3Pclk,
    ClkDrvMuxDpi1Clk,
    ClkDrvMuxDpi2Pclk,
    ClkDrvMuxDpi0Pclk,
    ClkDrvMuxRegJ7Ahclko
};
