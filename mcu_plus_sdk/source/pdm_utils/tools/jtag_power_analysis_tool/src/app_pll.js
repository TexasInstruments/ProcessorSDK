const drv = require("./drv/index.js");
const Table = require("cli-table3");

const jsonDescription =
    "This JSON file contains clock configuration data for a PLL (Phase-Locked Loop) device. \
     Each object in the file represents a specific clock configuration, with properties such as \
    'isEnable', 'numHsdiv', 'freqOut', 'hsdivOut', and 'settings'.\
    The 'isEnable' property indicates whether the clock is enabled or not. \
    The 'numHsdiv' property specifies the number of high-speed dividers.\
    The 'freqOut' property contains the output frequency of the PLL, with \
    'foutVco' and 'foutPostDiv' representing the VCO (Voltage-Controlled Oscillator) frequency and \
    post-divider frequency, respectively. The 'hsdivOut' property is an array of objects, each representing a high-speed divider output,\
    with properties such as 'idx', 'isEnable', 'freqOut', and 'hsDiv'. The 'settings' property contains additional configuration settings,\
    such as 'postDiv1', 'postDiv2', 'vcoFreqMul', 'M', 'fracM', and 'isPostDivEn'.";

/**
 * Retrieves the PLL settings for all pllList in the system.
 *
 * @return {Object} An object containing the PLL settings for all pllList.
 */
function getPllSettings(soc) {
    const socData = new drv.SocData(soc);
    let pllStatusList = {};

    // for each PLL create a PllDrv object and get the PLL status
    for (const pllName of Object.keys(socData.socPllData)) {
        const pllStatus = new drv.pll.PllDrv(pllName).getPllStatus(socData.socPllData[pllName]);

        pllStatusList[pllName.replace("clk_data_","").toUpperCase()] = pllStatus;
    }

    // add json description
    const pllStatusJson = {
        description: jsonDescription,
        pllStatus: pllStatusList
    };

    return pllStatusJson;
}

/**
 * Prints the PLL data in a formatted table.
 *
 * @param {Object} pllList - The list of PLL data.
 * @param {number} [level=0] - The level of the data to print.
 * @return {void}
 */
function printPllDataLevel(pllList, level = 0) {
    let head = [];
    let colWidths = [];

    if (level === 0) {
        head = ["Name", "PLL Status", "VCO Freq(MHz)"];
        colWidths = [12, 12, 18];
    } else if (level == 1) {
        head = head = ["Name", "PLL Status", "VCO Freq(MHz)", "HSDIV", "Status", "Output Freq(MHz)"];
        colWidths = [12, 12, 18, 10, 12, 18];
    } else {
        head = ["Name", "PLL Status", "Config", "PostDiv", "VCO Freq(MHz)", "Num of HSDIVs", "HSDIV", "Status", "Output Freq(MHz)"];
        colWidths = [12, 12, 24, 18, 15, 15, 10, 12, 18];
    }

    const table = new Table({
        head: [],
        colWidths: colWidths,
        style: {
            head: [], //disable colors in header cells
            border: [] //disable colors for the border
        },
        wordWrap: true,
        wrapOnWordBoundary: false
    });

    table.push([{colSpan: head.length, content: "PLL Configuration Table", hAlign: "center"}]);
    table.push(head);

    for (const [pllName, pllStatus] of Object.entries(pllList)) {
        const pllNameTrim = pllName.split("_").slice(-2).join("_");

        let row = {};
        head.forEach((column) => {
            row[column] = ""; // initialize with empty string
        });

        // Add elements to the row

        row["Name"] = pllNameTrim;
         row["PLL Status"] = pllStatus.isEnable==1 ? "ENABLED" : "DISABLED";
        row["VCO Freq(MHz)"] = pllStatus.freqOut.foutVco;
        if (level > 1) {
            row["Config"] = "M=" + pllStatus.settings.M + ", FracM=" + pllStatus.settings.fracM;
            row["PostDiv"] = pllStatus.settings.isPostDivEn ==1 ? "ENABLED" + ", M2=" + pllStatus.settings.postDiv1 : "DISABLED";
            row["Num of HSDIVs"] = pllStatus.numHsdiv.toString();
        }

        if (level > 0) {
            for (let j = 0; j < pllStatus.numHsdiv; j += 1) {
                if (j != 0) {
                    row = {};
                    head.forEach((column) => {
                        row[column] = "";
                    });
                }
                const hsdiv = pllStatus.hsdivOut[j];
                row["HSDIV"] = `HSDIV${hsdiv.idx}`;
                row["Status"] = hsdiv.isEnable ==1 ? "ENABLED" : "DISABLED";
                row["Output Freq(MHz)"] = hsdiv.freqOut;
                table.push(Object.values(row));
            }
            if (pllStatus.numHsdiv == 0) {
                table.push(Object.values(row));
            }
        } else {
            table.push(Object.values(row));
        }
    }

    console.log("\n");
    console.log(table.toString());
}

/**
 * Prints the PLL data in a table format.
 *
 * @param {Object} pllList - The list of PLL data to print.
 */
function printPllData(pllList, level) {
    // sort the pllList by name
    const sortedKeys = Object.keys(pllList).sort((a, b) => {
        const aType = a.split("_").slice(-2, -1)[0];
        const bType = b.split("_").slice(-2, -1)[0];
        if (aType == "MCU" && bType != "MCU") return 100;
        if (aType != "MCU" && bType == "MCU") return -1;
        const aNum = parseInt(a.split("_").pop());
        const bNum = parseInt(b.split("_").pop());
        return aNum - bNum;
    });

    const sortedPllList = {};
    for (const key of sortedKeys) {
        sortedPllList[key] = pllList[key];
    }

    pllList = sortedPllList;

    printPllDataLevel(pllList, level);
}

module.exports = {
    getPllSettings,
    printPllData
};
