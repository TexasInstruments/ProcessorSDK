const Driver = require("./driver.js");
const {registerDriverClass, getDriverClass} = require("./driver_registry.js");
const {readMmrField} = require("./common.js");

class PllDrv extends Driver {
    static OFFSET_PLL_MMR_CFG = 0x00000008;
    static OFFSET_PLL_CTRL = 0x00000020;
    static OFFSET_PLL_FREQ_CTRL0 = 0x00000030;
    static OFFSET_PLL_FREQ_CTRL1 = 0x00000034;
    static OFFSET_PLL_DIV_CTRL = 0x00000038;
    static OFFSET_PLL_HSDIV0_CTRL = 0x00000080;

    static PLL_MMR_SIZE = 0x1000;
    static PLL_HSDIV_MMR_SIZE = 0x4;

    static bitFields = {
        refDiv: {startBit: 0, endBit: 5},
        freqValInt: {startBit: 0, endBit: 11},
        freqValFrac: {startBit: 0, endBit: 23},
        postDivEn: {startBit: 4, endBit: 4},
        postDiv1: {startBit: 16, endBit: 18},
        postDiv2: {startBit: 24, endBit: 26},
        pllEn: {startBit: 15, endBit: 15},
        hsdivPrsnc: {startBit: 16, endBit: 31},
        hsdivClkEn: {startBit: 15, endBit: 15},
        hsdivDiv: {startBit: 0, endBit: 6}
    };

    checkMmrResult(mmrResult) {
        if (mmrResult.status === -1) {
            throw new Error("Fatal error: MMR read failed. MMR key value data is missing.");
        }
    }

    constructor(pllName) {
        super();
        this.pllName = pllName;
    }

    /**
     * Retrieves the PLL dividers.
     *
     * @param {Object} pllInst - The PLL instance.
     * @return {Object} An object containing the PLL dividers.
     */
    getPllDivs(pllInst) {
        let divs = {};
        let refDiv = 1;
        let postDiv1 = 0xffffffff;
        let postDiv2 = 0xffffffff;
        let baseAddr = parseInt(pllInst.base, 16);
        let freqValInt = 0;
        let freqValFracReg = 0;
        let postDivEn = -1;
        let mmrResult;
        // read voc div value
        let regAddrRefDiv = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_DIV_CTRL;
        mmrResult = readMmrField(regAddrRefDiv, PllDrv.bitFields.refDiv.startBit, PllDrv.bitFields.refDiv.endBit);
        this.checkMmrResult(mmrResult);
        refDiv = mmrResult.value;

        let regAddrFreqValInt = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_FREQ_CTRL0;
        let regAddrFreqValFrac = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_FREQ_CTRL1;

        mmrResult = readMmrField(regAddrFreqValInt, PllDrv.bitFields.freqValInt.startBit, PllDrv.bitFields.freqValInt.endBit);
        this.checkMmrResult(mmrResult);
        freqValInt = mmrResult.value;

        mmrResult = readMmrField(regAddrFreqValFrac, PllDrv.bitFields.freqValFrac.startBit, PllDrv.bitFields.freqValFrac.endBit);
        this.checkMmrResult(mmrResult);
        freqValFracReg = mmrResult.value;

        let freqValFrac = freqValFracReg / (1 << 24);

        let vcoFreqMul = (freqValInt + freqValFrac) / refDiv;

        /* Read fout_post_div_freq */
        let regAddrPostDivEn = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_CTRL;
        mmrResult = readMmrField(regAddrPostDivEn, PllDrv.bitFields.postDivEn.startBit, PllDrv.bitFields.postDivEn.endBit);
        this.checkMmrResult(mmrResult);
        postDivEn = mmrResult.value;

        if (postDivEn == 1) {
            let regAddrPostDiv = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_DIV_CTRL;

            mmrResult = readMmrField(regAddrPostDiv, PllDrv.bitFields.postDiv1.startBit, PllDrv.bitFields.postDiv1.endBit);
            this.checkMmrResult(mmrResult);
            postDiv1 = mmrResult.value;

            mmrResult = readMmrField(regAddrPostDiv, PllDrv.bitFields.postDiv2.startBit, PllDrv.bitFields.postDiv2.endBit);
            this.checkMmrResult(mmrResult);
            postDiv2 = mmrResult.value;
        }

        divs.postDiv1 = postDiv1;
        divs.postDiv2 = postDiv2;
        divs.vcoFreqMul = vcoFreqMul;
        divs.M = freqValInt;
        divs.fracM = freqValFracReg;
        divs.isPostDivEn = postDivEn;

        return divs;
    }
    /**
     * Check if the PLL is enabled.
     *
     * @param {Object} pllInst - The PLL instance.
     * @return {number} 1 if enabled, 0 if disabled, -1 if status unknown or MMR read failed.
     */
    isPllEnabled(pllInst) {
        let isEnable = -1;
        let baseAddr = parseInt(pllInst.base, 16);

        let regAddrPllEn = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_CTRL;
        let mmrResult = readMmrField(regAddrPllEn, PllDrv.bitFields.pllEn.startBit, PllDrv.bitFields.pllEn.endBit);

        this.checkMmrResult(mmrResult);
        isEnable = mmrResult.value == 1 ? 1 : 0;

        return isEnable;
    }

    getNumHsdivs(pllInst) {
        let numHsdiv = 0;
        let baseAddr = parseInt(pllInst.base, 16);

        let regAddrHsdivPrsnc = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_MMR_CFG;
        let mmrResult = readMmrField(regAddrHsdivPrsnc, PllDrv.bitFields.hsdivPrsnc.startBit, PllDrv.bitFields.hsdivPrsnc.endBit);

        this.checkMmrResult(mmrResult);
        let hsdivPrsnc = mmrResult.value;
        // count num of 1s
        while (hsdivPrsnc != 0) {
            hsdivPrsnc = hsdivPrsnc & (hsdivPrsnc - 1);
            numHsdiv += 1;
        }

        return numHsdiv;
    }

    getHsdivOutFreq(pllInst, id) {
        let clkEn = 0;
        let hsdivDiv = 0;
        let hsdivFreqOut = 0;
        let pllFreq = this.getPllOutFreq(pllInst);
        let baseAddr = parseInt(pllInst.base, 16);
        let mmrResult;

        let regAddrClkEn = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_HSDIV0_CTRL + PllDrv.PLL_HSDIV_MMR_SIZE * id;
        mmrResult = readMmrField(regAddrClkEn, PllDrv.bitFields.pllEn.startBit, PllDrv.bitFields.pllEn.endBit);
        this.checkMmrResult(mmrResult);
        clkEn = mmrResult.value;

        if (clkEn == 1) {
            let regAddrHsdivDiv =
                baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_HSDIV0_CTRL + PllDrv.PLL_HSDIV_MMR_SIZE * id;
            mmrResult = readMmrField(regAddrHsdivDiv, PllDrv.bitFields.hsdivDiv.startBit, PllDrv.bitFields.hsdivDiv.endBit);

            this.checkMmrResult(mmrResult);
            hsdivDiv = mmrResult.value;
            // 0-5 hsdiv take input from vco_freq, while rest take input from fout_post_div
            if (id < 5) {
                hsdivFreqOut = (pllFreq.foutVco * 1000000) / (hsdivDiv + 1); // +1 in formula
            } else {
                hsdivFreqOut = (pllFreq.foutPostDiv * 1000000) / (hsdivDiv + 1);
            }
        }
        return {
            idx: id,
            isEnable: clkEn,
            freqOut: Math.trunc(hsdivFreqOut) / 1000000,
            hsDiv: hsdivDiv + 1
        };
    }

    getPllAllHsdivsOutFreq(pllInst) {
        let baseAddr = parseInt(pllInst.base, 16);
        let id = 0;
        let hsdivOutArr = [];
        let mmrResult;

        let regAddrHsdivPrsnc = baseAddr + pllInst.idx * PllDrv.PLL_MMR_SIZE + PllDrv.OFFSET_PLL_MMR_CFG;
        mmrResult = readMmrField(regAddrHsdivPrsnc, PllDrv.bitFields.hsdivPrsnc.startBit, PllDrv.bitFields.hsdivPrsnc.endBit);

        this.checkMmrResult(mmrResult);
        let hsdivPrsnc = mmrResult.value;
        // check the set bits in hsdivPrsnc , bit pos is hsdiv id
        while (hsdivPrsnc) {
            if (hsdivPrsnc & 1) {
                hsdivOutArr.push(this.getHsdivOutFreq(pllInst, id));
            }

            id++;
            hsdivPrsnc = hsdivPrsnc >> 1;
        }

        return hsdivOutArr;
    }

    getPllOutFreq(pllInst) {
        let div = this.getPllDivs(pllInst);
        let hfosc = new (getDriverClass("ClkDrvSocHfosc0"))();
        let hfoscFreq = hfosc.getHfoscFreq() * 1000000;

        let foutVco = hfoscFreq * div.vcoFreqMul;
        let foutPostDiv = (hfoscFreq * div.vcoFreqMul) / (div.postDiv1 * div.postDiv2);
        return {
            foutVco: Math.trunc(foutVco) / 1000000,
            foutPostDiv: Math.trunc(foutPostDiv) / 1000000
        };
    }
    getPllStatus(pllInst) {
        return {
            isEnable: this.isPllEnabled(pllInst),
            numHsdiv: this.getNumHsdivs(pllInst),
            freqOut: this.getPllOutFreq(pllInst),
            hsdivOut: this.getPllAllHsdivsOutFreq(pllInst),
            settings: this.getPllDivs(pllInst)
        };
    }
}

registerDriverClass(PllDrv);
module.exports = {PllDrv};
