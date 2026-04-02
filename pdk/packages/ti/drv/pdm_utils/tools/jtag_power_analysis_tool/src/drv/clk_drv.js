const snakeToCamelCase = require("./common.js").snakeToCamelCase;
const SocData = require("./soc_data.js");
const common = require("./common.js");
const Driver = require("./driver.js");

class ClkDrv extends Driver {
    constructor(clkName) {
        super();
        this.clkName = clkName;
        this.socData = new SocData(common.getSoc());
    }
    /**
     * Retrieves the parent clock and divider for the current clock.
     *
     * @return {Object} An object containing the parent clock and divider.
     * @property {string} div - The divider value.
     * @property {string} parent - The parent clock.
     */
    getParentClock() {
        // ClkDrv can be used to get the parent clock and divider for any clock

        const clkData = this.socData.socClkData[this.clkName];

        if (!clkData) {
            console.error("clkName not found in socClkData: ", this.clkName);
            return null;
        }

        const drvClass = snakeToCamelCase(clkData.drv);
        let clkDriver = Driver.createDriver(drvClass, this.clkName);

        if (!clkDriver) {
            console.error("No Driver for ", this.clkName);
            return null;
        }

        let divAndParent = clkDriver.getParentClock();

        return divAndParent;
    }

    /**
     * Traces the path from the current clock to the root clock,
     * retrieving the parent clocks and their dividers/multipliers at each step.
     *
     * @return {Array} An array of objects, each containing the parent clock and divider/multiplier,
     *                  representing the path from the current clock to the root clock.
     */
    getClockPathUptoRoot() {
        let clkNames = [];
        let nextClkName = this.clkName;

        do {
            const clkData = this.socData.socClkData[nextClkName];
            const drvClass = snakeToCamelCase(clkData.drv);
            let clkDriver = Driver.createDriver(drvClass, nextClkName);

            if (!clkDriver) {
                console.error("no Driver for ", nextClkName);
                break;
            }

            let divAndParent = clkDriver.getParentClock();
            clkNames.push(divAndParent);

            if (divAndParent.parent === "0") {
                //console.warn("Reaching to reserved clock", divAndParent.parent, nextClkName);
                break;
            }

            nextClkName = divAndParent !== null ? divAndParent.parent : null;
        } while (nextClkName != null);

        return clkNames;
    }
}

module.exports = ClkDrv;
