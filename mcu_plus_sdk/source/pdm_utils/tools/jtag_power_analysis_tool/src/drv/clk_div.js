const ClkDrv = require("./clk_drv.js");
const ClkDrvPll = require("./clk_pll.js");
const {readMmrField} = require("./common.js");
const {registerDriverClass} = require("./driver_registry.js");
const PllDrv = require("./pll.js");

class ClkDrvDivReg extends ClkDrv {
    /**
     * Retrieves the parent clock and divider for the current clock.
     *
     * @return {Object} An object containing the parent clock and divider.
     * @property {string} div - The divider value.
     * @property {string} parent - The parent clock.
     */
    getParentClock() {
        const parent = this.socData.socClkData[this.clkName].parent.clk_name;
        const clkData = this.socData.socClkData[this.clkName];
        const reg = this.socData.socClkDataDiv[clkData.data];

        const div = this.getDiv(reg);

        return {div, parent};
    }

    /**
     * Retrieves the divisor value from the given div register data.
     *
     * @param {Object} reg - The register object.
     * @returns {number} The divisor value.
     */
    getDiv(reg) {
        let div = 0xffffffff;
        const bitStart = reg.start_bit;
        const bitEnd = bitStart + reg.bitfield_width - 1;

        let mmrResult = readMmrField(reg.reg, bitStart, bitEnd);
        if (mmrResult.status >= 0) {
            div = mmrResult.value + 1;
        }

        return div;
    }
}

class ClkDrvDivRegGo extends ClkDrv {
    getParentClock() {
        // console.warn(`Driver not implemented in ${this.constructor.name}`);

        return {mul: 0, parent: null};
    }
}

class ClkDrvDivPll16fftHsdiv extends ClkDrvDivReg {}
class ClkDrvDivPll16fftPostdivHsdiv extends ClkDrvDivPll16fftHsdiv {}

class ClkDrvDivPll16fftPostdiv extends ClkDrvPll {
    /**
     * Retrieves the parent clock and the post divider product.
     *
     * @return {Object} An object containing the post divider product and the parent clock.
     */
    getParentClock() {
        let parent = this.socData.socClkData[this.clkName].parent.clk_name;
        const pllInst = this.socData.socPllData[this.socData.socClkData[parent].data];

        const pllDrv = new PllDrv.PllDrv();

        let divs = pllDrv.getPllDivs(pllInst);

        return {div: divs.postDiv1 * divs.postDiv2, parent: parent};
    }
}

registerDriverClass(ClkDrvDivReg);
registerDriverClass(ClkDrvDivRegGo);
registerDriverClass(ClkDrvDivPll16fftPostdiv);
registerDriverClass(ClkDrvDivPll16fftPostdivHsdiv);
registerDriverClass(ClkDrvDivPll16fftHsdiv);

module.exports = {
    ClkDrvDivReg,
    ClkDrvDivRegGo,
    ClkDrvDivPll16fftHsdiv,
    ClkDrvDivPll16fftPostdiv,
    ClkDrvDivPll16fftPostdivHsdiv
};
