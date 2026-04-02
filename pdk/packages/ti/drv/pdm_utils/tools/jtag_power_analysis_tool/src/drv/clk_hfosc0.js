const ClkDrv = require("./clk_drv.js");
const {readMmr, readMmrField} = require("./common.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvSocHfosc0 extends ClkDrv {
    static J7_WKUP_CTRL_MMR = 0x43000000;
    static J7_MAIN_DEVSTAT = 0x30;
    static J7_MAIN_BOOTCFG = 0x34;
    static freq_table = [19.2, 20.0, 24.0, 25.0, 26.0, 27.0];

    constructor(clkName) {
        super(clkName);
    }
    /**
     * Returns the frequency of the HFOSC clock and its parent, where the parent is null since HFOSC is the root clock source.
     *
     * @return {Object} An object containing the frequency of the HFOSC clock as the `mul` property and `null` as the `parent` property.
     */
    getParentClock() {
        var hfoscFreq = this.getHfoscFreq();

        return {mul: hfoscFreq, parent: null};
    }

    /**
     * Returns the frequency of the HFOSC clock.
     *
     * @return {number} The frequency of the HFOSC clock.
     */
    getHfoscFreq() {
        let reg_val = 0;
        let hfoscFreq = 0;

        let mmrResult = readMmr(ClkDrvSocHfosc0.J7_WKUP_CTRL_MMR + ClkDrvSocHfosc0.J7_MAIN_DEVSTAT);
        if (mmrResult.status >= 0) {
            reg_val = mmrResult.value;

            let v = BigInt(reg_val) & BigInt(0x7);

            if (v < ClkDrvSocHfosc0.freq_table.length) {
                hfoscFreq = ClkDrvSocHfosc0.freq_table[v];
            }
        } else {
            console.warn(" HFSOC Freq not available");
        }

        return hfoscFreq;
    }
}

registerDriverClass(ClkDrvSocHfosc0);

module.exports = {ClkDrvSocHfosc0};
