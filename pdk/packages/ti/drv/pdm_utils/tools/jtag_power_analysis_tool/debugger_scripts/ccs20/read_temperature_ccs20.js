const fsT = require("fs");
const pathT = require("path");

/* ------------------------------------- EDIT FILE PATH TO ABSOLUTE PATH BEFORE RUNNING ------------------------------------- */
const filePathTemperature = "/home/cgt/ti/pdm_utils/tools/jtag_power_analysis_tool/debugger_scripts/ccs20/read_temperature_ccs20.js";

const jsonDescription =
    "This JSON file contains junction temperature data for the SoC. Each object in the list represents\
    the temperature measured by a sensor located at the specified core and vddDomain. \
    The 'tempMiliDegC' field in each object represents the temperature in milli degree celcius. \
    The 'description' field in each object provides a brief description of the location of the sensor in the SoC. \
    The length of the list indicates the number of temperature sensors present in the SoC.";


let debugServer = ds;
let session = debugServer.openSession(/MCU_Cortex_R5_0|WKUP_Cortex_R5_0/);
if (!session.target.isConnected()) {
    session.target.connect();
}

function getJsonObject(filePath) {
    try {
        const data = fsT.readFileSync(filePath, "utf8");
        return JSON.parse(data);
    } catch (err) {
        console.error("Error reading or parsing JSON file:", filePath, err);
        return null;
    }
}

/**
 * Saves an object to the output directory as a JSON file.
 *
 * @param {string} fileName - The name of the file.
 * @param {object} obj - The object to be saved.
 * @return {void}
 */
function saveObjToOutput(fileName, obj) {
    const dir = pathT.dirname(filePathTemperature);
    const filePath = path.join(dir, "..", "..", "output", fileName);

    if (!fs.existsSync(path.dirname(filePath))) {
        fs.mkdirSync(path.dirname(filePath));
    }

    fs.writeFileSync(filePath, JSON.stringify(obj, null, 2), "utf8");
}

const readMmrDirect = (address) => {
    try {
        const value = session.memory.readOne(address);
        return value;
    } catch (error) {
        console.error("Error Reading Memory Address ", address.toString(16));
        return NaN;
    }
};

const writeMmrDirect = (address, value) => {
    session.memory.write(address, value);
};

const readMmrFieldDirect = (address, startBit, endBit) => {
    const value = readMmrDirect(address);
    const mask = BigInt((1 << (endBit - startBit + 1)) - 1) << BigInt(startBit);
    return Number((value & mask) >> BigInt(startBit));
};

class temperature {
    constructor(soc) {
        this.soc = soc;
        const dir = pathT.dirname(filePathTemperature);
        const pathMmr = pathT.join(dir, "..", "..", "soc_data", this.soc, "soc_mmr.json");
        const pathCommonVtmLut = pathT.join(dir, "..", "..", "soc_data", "common", "adc_vtm_lut.json");

        this.socMmr = getJsonObject(pathMmr);
        this.adcVtmLut = getJsonObject(pathCommonVtmLut).adcCodeToMilidegLut;

        if (!this.socMmr) {
            console.error(`Failed to load '${pathMmr}' for '${this.soc}' soc.`);
            this.socMmrVtm = null;
        } else {
            this.socMmrVtm = this.socMmr.vtm;
        }

        if (!this.adcVtmLut) {
            console.error("Failed to load adc_vtm_lut.json. Using default LUT.");
        }

        this.numOfSensor = this.getTempSensorCount();
    }

    /**
     * Retrieves the number of temperature sensors.
     *
     * @return {number} The number of temperature sensors.
     */
    getTempSensorCount() {
        if (!this.socMmrVtm) {
            console.warn("socMmrVtm is not initialized. Cannot determine number of sensors.");
            return 0;
        }

        const reg = parseInt(this.socMmrVtm.VTM_CFG_REG_CFG1, 16) + parseInt(this.socMmrVtm.VTM_DEVINFO_PWR0, 16);
        let numOfSensor = readMmrFieldDirect(
            reg,
            this.socMmrVtm.bitFields.numOfSensor.startBit,
            this.socMmrVtm.bitFields.numOfSensor.endBit
        );

        if (numOfSensor > 8) {
            console.warn("Invalid number of temperature sensors:", numOfSensor);
            numOfSensor = 0;
        }

        return numOfSensor;
    }

    readSocTemperature() {
        let tempMiliDegC = [];
        let initialConfig = [];

        if (!this.socMmrVtm || this.workaround_needed()) {
            console.log("Temperature info is not available for SoC:", this.soc);
            return [];
        }

        /* Configure the VTM temperature sensor */
        for (let i = 0; i < this.numOfSensor; i++) {
            const configReg =
                parseInt(this.socMmrVtm.VTM_CFG_REG_CFG2, 16) +
                parseInt(this.socMmrVtm.TMPSENS_CTRL_OFFSET, 16) +
                i * parseInt(this.socMmrVtm.INSTANCE_OFFSET, 16);

            const configRegVal = readMmrDirect(configReg);
            initialConfig.push(configRegVal);
            writeMmrDirect(configReg, BigInt(configRegVal) | BigInt(this.socMmrVtm.CONFIG_VALUE));
        }

        for (let i = 0; i < this.numOfSensor; i++) {
            const reg =
                parseInt(this.socMmrVtm.VTM_CFG_REG_CFG1, 16) +
                parseInt(this.socMmrVtm.TMPSENS_STAT_OFFSET, 16) +
                i * parseInt(this.socMmrVtm.INSTANCE_OFFSET, 16);

            const adcCode = readMmrFieldDirect(reg, this.socMmrVtm.bitFields.adcCode.startBit, this.socMmrVtm.bitFields.adcCode.endBit);

            tempMiliDegC.push({
                description: this.socMmrVtm.sensorMap[i].description,
                tempMiliDegC: this.adcVtmLut[adcCode]
            });
        }

        /* Restore the initial config */
        for (let i = 0; i < this.numOfSensor; i++) {
            const configReg =
                parseInt(this.socMmrVtm.VTM_CFG_REG_CFG2, 16) +
                parseInt(this.socMmrVtm.TMPSENS_CTRL_OFFSET, 16) +
                i * parseInt(this.socMmrVtm.INSTANCE_OFFSET, 16);

            writeMmrDirect(configReg, initialConfig[i]);
        }
        return tempMiliDegC;
    }

    workaround_needed() {
        /* J721e have some hardware issue, firmware do some workaround */
        if (this.soc != "j721e") {
            return false;
        }

        let reg = readMmrFieldDirect(parseInt(this.socMmrVtm.SDL_VTM_EFUSE_BASE_ADDR, 16), 30, 31);

        if (reg != 0) {
            return true;
        } else {
            return false;
        }
    }
}

function printSocTemperature(socTempData) {
    const header = ["Sensor", "Location", "Temperature (°C)"];
    const rows = [];
    const maxLen = Math.max(header[1].length, ...socTempData.map((item) => item.description.length));
    for (let i = 0; i < socTempData.length; i++) {
        const row = [i, socTempData[i].description, (socTempData[i].tempMiliDegC / 1000).toFixed(2)];
        rows.push(
            row.map((item, index) => {
                const padding = maxLen - item.length + (index === 0 ? 0 : 2);
                return item + " ".repeat(Math.max(0, padding));
            })
        );
    }

    const table = rows.reduce(
        (acc, row) => {
            const line = row.join("\t");
            return acc + line + "\n";
        },
        header.join("\t") + "\n"
    );

    console.log(table);
}

function getSocTemperature(soc) {
    const tempObj = new temperature(soc);
    const socTempData = tempObj.readSocTemperature();
    saveObjToOutput("junction_temp_" + soc + ".json", {description: jsonDescription, temp_data: socTempData});
    printSocTemperature(socTempData);
}

/* Run the script with soc name as argument. e.g. getSocTemperature("j722s"); */
