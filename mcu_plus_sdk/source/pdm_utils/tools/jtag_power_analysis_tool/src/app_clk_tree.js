const drv = require("./drv/index.js");
const Table = require("cli-table3");
const path = require("path");
const fs = require("fs");

const jsonDescription = `
    This json file list all IPs and their clocks.
    For an IP as key it, value contains a list of N clock paths,
    each clock path is a list of {clk, freq_mhz} where clk is the clock name and freq_mhz is the frequency of the clock in the path.
    {
    "IP1":[
    [{clk: "clk0", freq_mhz: 1000}, {clk: "clk1", freq_mhz: 200}, {clk: "clk2", freq_mhz: 100}],
    [{clk: "clk0", freq_mhz: 1000}, {clk: "clk1", freq_mhz: 200}, {clk: "clk2", freq_mhz: 50}],
    ],
    "IP2":[
    [{clk: "clk3", freq_mhz: 2000}, {clk: "clk4", freq_mhz: 400}, {clk: "clk5", freq_mhz: 400}],
    [{clk: "clk3", freq_mhz: 2000}, {clk: "clk4", freq_mhz: 500}, {clk: "clk5", freq_mhz: 250}],
    ]
    }
    `;

/**
 * The function `buildClkTree` builds a clock tree for each IP..
 * Each IP has a list of clocks that it can use, specified in the `socIPClk` object.
 *
 * The clock tree is a nested object that represents the parent-child relationship of the clocks.
 * To build the clock tree, the function find the parent of child clock recursively till it reaches the root.
 *
 * @return {Object} An object containing the clock tree for each IP.
 */
function buildClkTree(soc) {
    var clkIpTree = {}; // stores the clock tree for each IP
    const socData = new drv.SocData(drv.common.getSoc());

    for (const ipName in socData.socIPClk) {
        clkIpTree[ipName] = [];
        const ipClklist = socData.socIPClk[ipName]; // list of clocks that the IP has

        for (const conn_clk in ipClklist) {
            /* a clock and its divider connect to ip*/
            const srcClkInputName = ipClklist[conn_clk].input_name;
            const srcClk = ipClklist[conn_clk].clk_name;
            const div = ipClklist[conn_clk].div;

            const clkData = socData.socClkData[srcClk];
            const drvType = drv.common.snakeToCamelCase(clkData.drv); // get the Driver type

            /* create the Driver and get the root clock */
            const clk_drv = drv.driver.createDriver(drvType, srcClk); //factory return Driver class
            let clkPathToRoot = clk_drv.getClockPathUptoRoot(); // get the root clock

            /* add the input name and source clock to first place and save the path in ip clock tree*/
            clkPathToRoot.unshift({div: div, parent: srcClk});
            clkPathToRoot.unshift({div: 1, parent: srcClkInputName});
            clkIpTree[ipName].push(clkPathToRoot);
        }
    }

    const clkTreeBottomUp = createBottomUpTree(clkIpTree);

    /* add the description of clock tree */
    const clkTreeJson = {
        description: jsonDescription,
        clkTree: clkTreeBottomUp
    };

    return clkTreeJson;
}

function createBottomUpTree(clkIpTree) {
    let clkTreeBottomUp = {};
    // for an ip, for each direct clock as src clock, reverse iterate clkipTree and store the path.
    for (let ip in clkIpTree) {
        clkTreeBottomUp[ip] = [];

        for (let srcClk in clkIpTree[ip]) {
            clkTreeBottomUp[ip][srcClk] = [];
            let topDownPath = clkIpTree[ip][srcClk];
            let pathFreq = 1;

            for (let i = topDownPath.length - 1; i >= 1; i--) {
                const childTuple = topDownPath[i - 1];
                const currTuple = topDownPath[i];
                const currClk = childTuple.parent;

                if (currTuple.div) {
                    pathFreq = Math.trunc((pathFreq * 1000000) / currTuple.div) / 1000000;
                } else {
                    pathFreq = Math.trunc(pathFreq * 1000000 * currTuple.mul) / 1000000;
                }

                clkTreeBottomUp[ip][srcClk].push({
                    clk: currClk,
                    freq_mhz: +pathFreq
                });
            }
        }
    }

    return clkTreeBottomUp;
}

function printClkTree(clkIpTree) {
    printConfigTable(clkIpTree); // print the major IPs clock table

    console.log("\n");
    console.log(
        "=============================================================================================================================================================="
    );
    console.log(
        "                                                                CLOCK TREE                                                                    "
    );
    console.log(
        "=============================================================================================================================================================="
    );

    for (let ip in clkIpTree) {
        let ip_name = ip;
        console.log(`\nDevice IP: ${ip_name}`);
        console.log(`Number of clocks: ${clkIpTree[ip].length}\n`);

        if (clkIpTree[ip].length == 0) {
            console.log("Refer to clock architecture");
        } else {
            /* full path of the clock */
            for (let clks_path of clkIpTree[ip]) {
                if (clks_path[clks_path.length - 1].freq_mhz == 0) {
                    console.log(`  ▶  ${clks_path[clks_path.length - 1].clk} (Refer to clock architecture)`);
                    continue;
                }
                let clksStr = clks_path
                    .map((clk) => {
                        let rest = clk.clk.split("_");
                        rest.splice(0, 2);

                        let freq = clk.freq_mhz == 0 ? "Refer to clock architecture" : clk.freq_mhz + " MHz";

                        let clkName = rest.join("_");
                        let ret = `${clkName} (${freq})`;

                        return ret;
                    })
                    .join(" --> ");

                console.log(`  ▶  ${clksStr}`);
            }
        }

        console.log("─────────────────────────────────────────────────────────────────────────────────────────\n");
    }
}

function printConfigTable(clkIpTree) {
    let petIpListPath = path.join(__dirname, "..", "..", "config", ".meta", drv.common.getSoc(), "pet_ip_mapping.json");

    let pet_ip_list = null;
    if (fs.existsSync(petIpListPath)) {
        pet_ip_list = drv.common.getJsonObject(petIpListPath);
    }

    let custIpListPath = path.join(__dirname, "..", "..", "config", ".meta", drv.common.getSoc(), "custom_ip_mapping.json");
    
    let cust_ip_list = null;
    if (fs.existsSync(custIpListPath)) {
        cust_ip_list = drv.common.getJsonObject(custIpListPath);
    }

    let ip_list = Object.assign({}, pet_ip_list, cust_ip_list);
    
    const head = ["IP Name", "Freq(MHz)", "LPSC", "Status"];
    const table = new Table({
        head: [],
        colWidths: [30, 12, 24, 12],
        wordWrap: true,
        wrapOnWordBoundary: false,
        style: {
            head: [], //disable colors in header cells
            border: [] //disable colors for the border
        }
    });

    table.push([{colSpan: head.length, content: "Frequency of Selected IPs", hAlign: "center"}]);
    table.push(head);

    let psc_status_path = path.join(__dirname, "..", "output", "psc_status_" + drv.common.getSoc() + ".json");
    let psc_status = null;
    if (fs.existsSync(psc_status_path)) {
        psc_status = drv.common.getJsonObject(psc_status_path).pscStatus;
    } else {
        console.log(`ERROR: ${psc_status_path} does not exist`);
    }

    const lpscStatus = generateLpscStatusList(psc_status);

    for (let ip_alias in ip_list) {
        let configParam = ip_list[ip_alias];
        let paths = clkIpTree[configParam.ip_name];
        if (paths == null || paths.length == 0) {
            continue;
        }

        for (let path of paths) {
            let found = false;
            for (let i = path.length - 1; i >= 0; i--) {
                if (path[i].clk === configParam.input_name) {
                    let freq_mhz = path[i].freq_mhz;
                    if (freq_mhz == 0) {
                        freq_mhz = "N/A";
                    }
                    table.push([ip_alias, freq_mhz, configParam.lpsc_name, lpscStatus[configParam.lpsc_name]]);

                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    console.log("\n");
    console.log(table.toString());
}

function generateLpscStatusList(pscStatusJson) {
    const lpscStatusList = {};

    Object.keys(pscStatusJson).forEach((psc) => {
        Object.keys(pscStatusJson[psc]).forEach((pd) => {
            if (pscStatusJson[psc][pd].lpsc_list) {
                Object.keys(pscStatusJson[psc][pd].lpsc_list).forEach((lpsc) => {
                    const lpscInfo = pscStatusJson[psc][pd].lpsc_list[lpsc];
                    lpscStatusList[lpsc] = lpscInfo.status;
                });
            }
        });
    });

    return lpscStatusList;
}

module.exports = {
    buildClkTree,
    printConfigTable,
    printClkTree
};
