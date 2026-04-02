const path = require("path");
const {getJsonObject} = require("./common.js");

/**
 * SocData class: A singleton class responsible for loading and caching SoC data.
 *
 * @class SocData
 * @param {string} soc - The name of the SOC.
 * @throws {Error} If soc is not provided and the SocData instance is not already set.
 * @returns {SocData} The singleton instance of SocData.
 */
class SocData {
    /**
     * Constructor for the SocData class.
     *
     * @param {string} soc - The name of the SOC.
     * @throws {Error} If soc is not provided and the SocData instance is not already set.
     * @returns {SocData} The singleton instance of SocData.
     */
    constructor(soc) {
        if (!soc && !SocData.instance) {
            throw new Error("Soc must be provided");
        }

        // If SocData instance is not set, load the SOC data from the file system
        if (!SocData.instance) {
            const pathIpClk = path.join(__dirname, "..", "..", "soc_data", soc, "soc_ip_clock_list.json");
            const pathClkData = path.join(__dirname, "..", "..", "soc_data", soc, "soc_clk_data.json");
            const pathClkDataMux = path.join(__dirname, "..", "..", "soc_data", soc, "soc_clk_data_mux.json");
            const pathClkParents = path.join(__dirname, "..", "..", "soc_data", soc, "soc_clk_data_mux_parents.json");
            const pathClkDataDiv = path.join(__dirname, "..", "..", "soc_data", soc, "soc_clk_data_div.json");
            const pathPllData = path.join(__dirname, "..", "..", "soc_data", soc, "soc_pll_data.json");
            const pathPscData = path.join(__dirname, "..", "..", "soc_data", soc, "soc_psc_data.json");

            this.socIPClk = getJsonObject(pathIpClk);
            this.socClkData = getJsonObject(pathClkData);
            this.socClkDataMux = getJsonObject(pathClkDataMux);
            this.socClkDataParent = getJsonObject(pathClkParents);
            this.socClkDataDiv = getJsonObject(pathClkDataDiv);
            this.socPllData = getJsonObject(pathPllData);
            this.socPscData = getJsonObject(pathPscData);

            SocData.instance = this;
        }

        return SocData.instance;
    }
}

module.exports = SocData;
