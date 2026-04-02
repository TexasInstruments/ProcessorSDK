const ClkDrv = require("./clk_drv.js");
const {readMmr, readMmrField} = require("./common.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvPllctrlMuxRegRo extends ClkDrv {
    static PLLCTRL_PLLCTRL = BigInt(0x100);
    static PLLCTRL_PLLCTRL_PLLEN_BIT = BigInt(1) << BigInt(0);
    static PLLCTRL_PLLCTRL_PLLENSRC_BIT = BigInt(1) << BigInt(5);
    static PLLCTRL_PLLSTAT = BigInt(0x13c);
    static PLLCTRL_PLLSTAT_LOCK = BigInt(1) << BigInt(1);

    constructor(clkName) {
        super(clkName);
    }

    getParentClock() {
        const clkName = this.clkName;
        const clkData = this.socData.socClkData[clkName];
        const muxRegName = clkData.data;
        const muxData = this.socData.socClkDataMux[muxRegName];
        const parentsList = this.socData.socClkDataParent[muxData.parents];
        let reg = BigInt(parseInt(muxData.reg, 16));
        let parent = null;
        let div = 0xffffffff;
        let reg_val = -1;
        let mmrResult;

        mmrResult = readMmr(reg + ClkDrvPllctrlMuxRegRo.PLLCTRL_PLLCTRL);

        if (mmrResult.status >= 0) {
            reg_val = BigInt(mmrResult.value);
            if ((reg_val & ClkDrvPllctrlMuxRegRo.PLLCTRL_PLLCTRL_PLLENSRC_BIT) !== BigInt(0)) {
                // When set, mux is controlled by lock state of PLL
                mmrResult = readMmr(reg + ClkDrvPllctrlMuxRegRo.PLLCTRL_PLLSTAT);
                if (mmrResult.status >= 0) {
                    reg_val = BigInt(mmrResult.value);
                    if ((reg_val & ClkDrvPllctrlMuxRegRo.PLLCTRL_PLLSTAT_LOCK) !== BigInt(0)) {
                        // PLL is locked
                        parent = parentsList[1].clk_name;
                        div = parentsList[1].div;
                    } else {
                        // PLL is in bypass
                        parent = parentsList[0].clk_name;
                        div = parentsList[0].div;
                    }
                }
            } else {
                // When cleaned, mux is controlled by PLLEN bit
                if ((reg_val & ClkDrvPllctrlMuxRegRo.PLLCTRL_PLLCTRL_PLLEN_BIT) !== BigInt(0)) {
                    // Use pll clock
                    parent = parentsList[1].clk_name;
                    div = parentsList[1].div;
                } else {
                    // Use bypass clock
                    parent = parentsList[0].clk_name;
                    div = parentsList[0].div;
                }
            }
        }
        return {div, parent};
    }
}

registerDriverClass(ClkDrvPllctrlMuxRegRo);

module.exports = {ClkDrvPllctrlMuxRegRo};
