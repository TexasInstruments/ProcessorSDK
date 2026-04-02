const path = require("path");
const fs = require("fs");
const os = require('os');

let soc = "undefined";
let memValues = null;

/**
 * Returns the current session.
 *
 * @return {Object} The current session.
 */
function getMemValues() {
    return memValues;
}

/**
 * Sets the system on a chip (SoC)
 *
 * @param {string} socVal - The SoC identifier.
 * @return {void}
 */
function setSoc(soc_) {
    soc = soc_;
}

/**
 * Returns the current system on a chip (SoC).
 *
 * @return {string} The current SoC.
 */
function getSoc() {
    return soc;
}

/**
 * Sets the session.
 *
 * @param {Object} memValues_ - The session object.
 * @return {void}
 */
function setMemValues(memValues_) {
    memValues = memValues_;
}

/* Parse the JSON text */
function getJsonObject(jsonFile) {
    const jsonText = fs.readFileSync(jsonFile, "utf8");
    return JSON.parse(jsonText);
}

/**
 * Reads mem_values.csv files containing address-value pairs and returns as an Object
 * @param {String} filePath
 * @returns {Object} {"address": value,}
 */
function getObjectFromCsv(filePath){
    let fileContent;
    try {
        fileContent = fs.readFileSync(filePath, 'utf-8');
    } catch (readErr) {
        throw new Error(`Error reading file ${filePath}: ${readErr.message}`)
    }

    /* Parse CSV content */
    const lines = fileContent.split('\n').filter(line => line.trim());
    const jsonObject = {};

    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        /* Allow for values containing commas by splitting only on the first comma */
        const commaIndex = line.indexOf(',');
        if (commaIndex === -1) {
            console.warn(`Skipping malformed line (no comma): "${line}"`);
            continue;
        }
        const key = line.slice(0, commaIndex).trim();
        const value = line.slice(commaIndex + 1).trim();

        if (!key) {
            console.warn(`Skipping line with empty key: "${line}"`);
            continue;
        }
        /* Convert value to number if it's numeric */
        const processedValue = value !== '' && !isNaN(value) ? Number(value) : value;
        jsonObject[key] = processedValue;
    }
    return jsonObject;
}

/**
 * Combines data from provided mem_values.csv files and returns as Object
 * @param {Array<String>} -> Array of paths of mem_values.csv files to read
 * @returns {Object}
 */
function getCombinedObjectFromCsv(files){
    const mergedResult = files.reduce((combined, file) => {
        const jsonData = getObjectFromCsv(file);
        return {...combined, ...jsonData};
    }, {})

    return mergedResult;
}

/**
 * Reads a specific field from a memory-mapped register.
 *
 * @param {string} reg - The name of the register.
 * @param {number} bitStart - The starting bit of the field.
 * @param {number} bitEnd - The ending bit of the field.
 * @return {number} The value of the field.
 */
function readMmrField(reg, bitStart, bitEnd) {
    let retVal = NaN;

    if (bitStart > bitEnd) {
        console.error("start bit is greater than end bit");
        return {status: -1, value: NaN};
    }
    const mmrResult = readMmr(reg); //read a word

    if (mmrResult.status >= 0) {
        let reg_val_shifted = BigInt(mmrResult.value) >> BigInt(bitStart);
    let mask = (BigInt(1) << BigInt(bitEnd - bitStart + 1)) - BigInt(1);
        retVal = Number(reg_val_shifted & mask);
    }

    return {status: mmrResult.status, value: retVal};
}

/**
 * Reads a memory-mapped register from mem_values.csv.
 *
 * @param {string} reg - The name of the register.
 * @return {{status: number, value: number}} The status and value of the register.
 * If the value is not available or returns -1, the status will be -1 indicating memory read failure.
 */
function readMmr(reg) {
    let status =-1;
    let value = NaN;
    if (memValues == null) {
        console.error("Memory Data is not initialized");
        process.exit(1);
        return {status: -1, value: NaN};
    }
    else {
        let address = "0x"+reg.toString(16);
        if(address.slice(2, 4) == "0x")
            address = address.slice(2);
        value = memValues[address];

        if(value === undefined || value == NaN || value ==-1) {
            console.error("Memory not found", reg, address);
            status = -1;
            value = NaN;
        }
        else{
            status = 0;
        }

        }
    return {status:status, value: value};
}

/**
 * Saves an object to the output directory as a JSON file.
 *
 * @param {string} fileName - The name of the file.
 * @param {object} obj - The object to be saved.
 * @return {void}
 */
function saveObjToOutput(fileName, obj) {
    const filePath = path.join(__dirname, "..", "..", "output", fileName);

    if (!fs.existsSync(path.dirname(filePath))) {
        fs.mkdirSync(path.dirname(filePath));
    }

    fs.writeFileSync(filePath, JSON.stringify(obj, null, 2), "utf8");
}

/**
 * Converts snake_case to CamelCase.
 *
 * @param {string} str - The string to convert.
 * @return {string} The converted string.
 */
function snakeToCamelCase(str) {
    let ret = str.replace(/_\w/g, (match) => match[1].toUpperCase());
    ret = ret.charAt(0).toUpperCase() + ret.slice(1);
    return ret;
}

module.exports = {
    getJsonObject,
    readMmr,
    readMmrField,
    saveObjToOutput,
    snakeToCamelCase,
    getSoc,
    getMemValues,
    setMemValues,
    setSoc,
    getCombinedObjectFromCsv,
    getObjectFromCsv,
};