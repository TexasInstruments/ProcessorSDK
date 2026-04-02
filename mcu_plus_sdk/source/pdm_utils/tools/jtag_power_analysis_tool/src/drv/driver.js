const getDriverClass = require("./driver_registry.js").getDriverClass;
const common = require("./common.js");
const SocData = require("./soc_data.js");

/**
 * Driver base abstract class.
 */
class Driver {
    constructor() {
        if (new.target === Driver) {
            throw new Error("Cannot instantiate abstract class");
        }
        this.socData = new SocData(common.getSoc());
    }

    /**
     * Creates a Driver instance based on the given Driver name and instance name.
     *
     * @param {string} drv - The Driver name.
     * @param {string} name - The instance name.
     * @return {object|null} The Driver instance or null if no Driver is found.
     */
    static createDriver(drv, name) {
        const drvClass = getDriverClass(drv);

        if (drvClass) {
            return new drvClass(name);
        } else {
            console.log("no Driver for ", drv);
            return null;
        }
    }
}

module.exports = Driver;
