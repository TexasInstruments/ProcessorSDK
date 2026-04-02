const ClkDrv = require("./clk_drv.js");
const {getSoc} = require("./common.js");
const {registerDriverClass} = require("./driver_registry.js");

class ClkDrvSocLfosc0 extends ClkDrv {
    static J7_WKUP_CTRL_MMR = 0x43000000;
    static J7_MAIN_DEVSTAT = 0x30;
    static J7_MAIN_BOOTCFG = 0x34;
    static freq_table = [19.2, 20.0, 24.0, 25.0, 26.0, 27.0];

    constructor(clkName) {
        super(clkName);
    }
    /**
     * Returns the frequency of the LFOSC clock and its parent, where the parent is null since LFOSC is the root clock source.
     *
     * @return {Object} An object containing the frequency of the LFOSC clock as the `mul` property and `null` as the `parent` property.
     */
    getParentClock() {
        var lfoscFreq = this.getLfoscFreq();

        return {mul: lfoscFreq, parent: null};
    }

    /**
     * Returns the frequency of the LFOSC clock.
     *
     * @return {number} The frequency of the LFOSC clock.
     */
    getLfoscFreq() {
        let lfoscFreq = 1;

        if (getSoc() == "j722s") {
            lfoscFreq = 32.768 / 1000; //32.768 KHz/1000
        } else {
            console.warn("LFOSC frequency not available for soc: ", getSoc());
        }

        return lfoscFreq;
    }
}

registerDriverClass(ClkDrvSocLfosc0);

module.exports = {ClkDrvSocLfosc0};
