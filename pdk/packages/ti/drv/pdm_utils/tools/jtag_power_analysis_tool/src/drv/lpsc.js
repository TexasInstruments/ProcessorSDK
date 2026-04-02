const Driver = require("./driver.js");
const {registerDriverClass} = require("./driver_registry.js");
const {readMmr, readMmrField, getJsonObject} = require("./common.js");
const {socPscData} = require("./soc_data.js");

class LpscDrv extends Driver {
    static OFFSET_MDSTAT = 0x800;
    static OFFSET_PDSTAT = 0x200;
    static PD_MMR_SIZE = 0x4;
    static MD_MMR_SIZE = 0x4;
    static bitFields = {
        pdState: {startBit: 0, endBit: 4},
        mdState: {startBit: 0, endBit: 5}
    };

    constructor(lpscName) {
        super(lpscName);
    }
    checkMmrResult(mmrResult) {
        if (mmrResult.status === -1) {
            throw new Error("Fatal error: MMR read failed. MMR key value data is missing.");
        }
    }

    /**
     * Retrieves the power state of a LPSC instance.
     *
     * @param {Object} lpscInst - The LPSC instance object containing base_addr, pd_index, and lpsc_index.
     * @return {Object} Power state information with pdState and mdState values (0: disabled, 1: enabled).
     */
    getLpscData(lpscInst) {
        let baseAddr = parseInt(lpscInst.base_addr, 16);
        let pdRegAddr = baseAddr + lpscInst.pd_index * LpscDrv.PD_MMR_SIZE + LpscDrv.OFFSET_PDSTAT;
        let mdRegAddr = baseAddr + lpscInst.lpsc_index * LpscDrv.MD_MMR_SIZE + LpscDrv.OFFSET_MDSTAT;
        let pdState = 0;
        let mdState = 0;
        let mmrResult;

        mmrResult = readMmrField(pdRegAddr, LpscDrv.bitFields.pdState.startBit, LpscDrv.bitFields.pdState.endBit);
        this.checkMmrResult(mmrResult);
        pdState = mmrResult.value === 1 ? true : false;

        mmrResult = readMmrField(mdRegAddr, LpscDrv.bitFields.mdState.startBit, LpscDrv.bitFields.mdState.endBit);
        this.checkMmrResult(mmrResult);
        mdState = mmrResult.value === 3 ? true : false;

        return {pdState: pdState, mdState: mdState};
    }
}

registerDriverClass(LpscDrv);

module.exports = {LpscDrv};
