const pathCcs = require("path");
const fsCcs = require("fs");

const isRunningFromCcs = typeof ds !== "undefined"; /* ds is defined when running in CCS Scripting Console */

let filePathPm = null;

if (isRunningFromCcs) {
    /* ------------------------------------- EDIT FILE PATH TO ABSOLUTE PATH BEFORE RUNNING ------------------------------------- */
    filePathPm = "/home/cgt/ti/pdm_utils/tools/jtag_power_analysis_tool/debugger_scripts/ccs20/pm_data_generator_ccs20.js";

    if (!fsCcs.existsSync(filePathPm)) {
        throw new Error("jtag_power_analysis_tool: file " + filePathPm + " not found, please check the path");
    }
} else {
    filePathPm = __filename;
}

class CCS {
    static debugServer = null;

    constructor() {
        this.soc = null;
        this.session = null;
        this.selector = null;
    }

    /**
     * Initializes CCS debug server and opens session
     * @param {Object} debugServer
     * @param {String} soc
     * @param {String} selector
     * @returns {void}
     */
    init(debugServer, soc, selector) {
        if (debugServer !== null && soc !== undefined) {
            this.soc = soc;
        } else {
            throw new Error("Invalid Arguments");
        }

        this.selector = selector;
        let session = debugServer.openSession(/MCU_Cortex_R5_0|WKUP_Cortex_R5_0/);
        if (session !== null) {
            this.session = session;
        } else {
            throw new Error("Could not open session");
        }

        console.log("Debug server initialized");
        return 0;
    }

    /**
     * Read memory locations specified in addresses.csv file and returns list of strings containing address-value pairs
     * @param {String} readfilePath -> absolute file path of addresses.csv file to read
     * @returns {Array<String>} -> list of strings containing address-value pairs. Each string in the list acts as a row for mem_values.csv files
     */
    readMemory(readfilePath) {
        if (!fsCcs.existsSync(readfilePath)) {
            throw new Error(`Source file does not exist: ${readfilePath}`);
        }

        let fileContent;
        try {
            fileContent = fsCcs.readFileSync(readfilePath, "utf-8");
        } catch (readErr) {
            throw new Error(`Error reading file ${readfilePath}: ${readErr.message}`);
        }

        let lines = fileContent.split("\n").filter((line) => line.trim());

        let csvData = [];
        for (let i = 0; i < lines.length; i++) {
            const hexAddress = String(lines[i]).slice(0, -1);
            const address = parseInt(lines[i], 16);
            let value = null;
            try {
                value = this.session.memory.readOne(address);
            } catch (error) {
                console.error("Error Reading Memory Address ", hexAddress);
                value = -1;
            }

            csvData.push(`${hexAddress},${value}\n`);
        }

        return csvData;
    }

    /**
     * Save data in a CSV file at filePathPm location
     * @param {String} filePathPm -> where to save the csv file
     * @param {Array<String>} data -> data to be saved in the csv file. Each item in the list is one row of the csv file
     * @returns {void}
     */
    saveCSVFile(filePathPm, data) {
        if (!fsCcs.existsSync(pathCcs.dirname(filePathPm))) {
            fsCcs.mkdirSync(pathCcs.dirname(filePathPm), {recursive: true});
        }

        const csvContent = data.join("");

        fsCcs.writeFileSync(filePathPm, csvContent, "utf8");
    }

    /**
     * Reads all relevant memory address CSV files and create output CSV files containing address-value pairs
     * @returns {void}
     */
    run() {
        const addressesDir = pathCcs.resolve(pathCcs.dirname(filePathPm), `../../soc_data/${this.soc}/mem_addr`);

        /* If addresses directory does not exist, throw error */
        if (!(fsCcs.existsSync(addressesDir) && fsCcs.lstatSync(addressesDir).isDirectory())) {
            throw new Error(`Memory Addresses Directory Not Found for SoC: ${this.soc}`);
        }

        /* Based on the selector, read memory addresses and create output files */
        const selector = this.selector;
        const memValuesDir = pathCcs.resolve(pathCcs.dirname(filePathPm), `../../memory_values_csv/${this.soc}`);

        const paths = {
            pll: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv"],
            psc: ["soc_psc_data_addr.csv"],
            clock_tree: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_clk_data_mux_addr.csv"],
            pet: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_psc_data_addr.csv", "soc_clk_data_mux_addr.csv"],
            all: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_psc_data_addr.csv", "soc_clk_data_mux_addr.csv"]
        };

        for (const key of Object.keys(paths)) {
            if (selector === key) {
                paths[key].forEach((file) => {
                    const pathToAddr = pathCcs.join(addressesDir, file);
                    const statusData = this.readMemory(pathToAddr);
                    const outputFile = file.replace(".csv", "_val.csv");
                    this.saveCSVFile(memValuesDir + "/" + outputFile, statusData);
                    console.log(`File generated: ${memValuesDir}/${outputFile}`);
                });
            }
        }

        return 0;
    }

    /**
     * Connects the target to the Debug Server.
     *
     * @return {void}
     */
    connectTarget() {
        let session = this.session;
        if (session) {
            session.target.connect();
            console.log("Target connected: " + session.target.isConnected());
            return -1;
        }

        return 0;
    }

    /**
     * Disconnects the target from the Debug Server.
     *
     * @return {void}
     */
    disconnectTarget() {
        let session = this.session;

        if (session) {
            session.target.disconnect();
            console.log("Target disconnected: " + !session.target.isConnected());
        } else {
            console.log("Target not connected");
            return -1;
        }

        return 0;
    }
}

const ccs = new CCS();

/**
 * If the script is running in the CCS Scripting Console,
 * it uses the same debug server as the CCS Scripting Console.
 *
 * If the script is not running in the CCS Scripting Console,
 * it initializes a new debug server.
 *
 * This feature allows to run this script on
 * live CCS debugging session.
 */

function pmDataGenerator(soc, selector = "all", pathToCcxml = null) {
    if (!CCS.debugServer) {
        CCS.debugServer = initScripting();
        CCS.debugServer.setScriptingTimeout(10000);
        CCS.debugServer.configure(pathToCcxml);
    }

    ccs.init(CCS.debugServer, soc, selector);

    if (!ccs.session.target.isConnected()) {
        ccs.connectTarget();
    }

    ccs.run();

    return 0;
}

function mainPm() {
    /* running from terminal */
    const minimist = require("minimist");
    const args = minimist(process.argv.slice(2), {
        string: ["soc", "selector", "ccxml"],
        alias: {
            s: "soc",
            c: "ccxml",
            sel: "selector"
        },
        default: {
            selector: "all"
        }
    });
    const {soc, selector, ccxml} = args;
    if (soc && ccxml) {
        pmDataGenerator(soc, selector, ccxml);
    } else {
        throw new Error("Invalid Arguments. Please provide --soc, --selector and --ccxml");
    }

    process.exit(0);

    return 0;
}

if (!isRunningFromCcs) {
    mainPm();
} else {
    CCS.debugServer = ds;
}

/* Example usage for j722s: pmDataGenerator("j722s", "all") */
