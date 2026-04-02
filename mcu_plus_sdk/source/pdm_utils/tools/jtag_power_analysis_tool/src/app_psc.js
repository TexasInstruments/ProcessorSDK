const drv = require("./drv/index.js");
const Table = require("cli-table3");

const nameMap = {"j7am_bolt_psc_wrap0":"BOLTON_PSC0"};

const jsonDescription =
    "This JSON file contains configurations for Power and Sleep Controller (PSC) domains within a SoC. The structure of the file is as follows: \
                  The root contains keys that represent the different PSC domains. Each PSC domain contains keys that represent the different power domains. \
                  Each power domain contains a list of LPSCs, each LPSC have its status an a list of IPs it controls";

function getPscData(soc) {
    // from socPscData read the each lpsc status and store in a list
    const lpscData = {};
    const socData = new drv.SocData(soc);

    for (const lpscName in socData.socPscData) {
        const lpscInst = socData.socPscData[lpscName];
        const lpscDrvObj = new drv.lpsc.LpscDrv();
        const data = lpscDrvObj.getLpscData(lpscInst);

        lpscData[lpscName] = {
            gpsc_name: nameMap[lpscInst.gpsc_name] || lpscInst.gpsc_name,
            lpsc_name: lpscInst.lpsc_name,
            lpsc_status: data.mdState === true ? "ENABLED" : "DISABLED",
            pd_name: lpscInst.pd_name,
            pd_status: data.pdState === true ? "ENABLED" : "DISABLED",
            controlled_ip_instances: lpscInst.controlled_ip_instances
        };
    }

    // convert lpscData to to psc hierarchy
    const pscStatus = createPscHierarchyfromLpscData(lpscData);

    const pscStatusJson = {
        description: jsonDescription,
        pscStatus: pscStatus
    };

    return pscStatusJson;
}

function createPscHierarchyfromLpscData(lpscData) {
    // creates psc->pd->lpsc hierarchy
    const hierarchy = {};

    for (const lpscInst of Object.values(lpscData)) {
        if (!hierarchy[lpscInst.gpsc_name]) {
            hierarchy[lpscInst.gpsc_name] = {};
        }
        if (!hierarchy[lpscInst.gpsc_name][lpscInst.pd_name]) {
            hierarchy[lpscInst.gpsc_name][lpscInst.pd_name] = {};
            hierarchy[lpscInst.gpsc_name][lpscInst.pd_name]["lpsc_list"] = {};
        }
        hierarchy[lpscInst.gpsc_name][lpscInst.pd_name]["status"] = lpscInst.pd_status;
        hierarchy[lpscInst.gpsc_name][lpscInst.pd_name]["lpsc_list"][lpscInst.lpsc_name] = {
            status: lpscInst.lpsc_status,
            controlled_ip_instances: lpscInst.controlled_ip_instances
        };
    }

    return hierarchy;
}

function printPscData(pscHierarchy, level = 0) {
    let head = [];
    let colWidths = [];

    if (level === 0) {
        head = ["PSC Name", "PD Name", "PD Status"];
        colWidths = [14, 18, 12];
    } else if (level === 1) {
        head = ["PSC Name", "PD Name", "PD Status", "LPSC Name", "LPSC Status"];
        colWidths = [14, 18, 12, 24, 13];
    } else {
        head = ["PSC Name", "PD Name", "PD Status", "LPSC Name", "LPSC Status", "Controlled IP Instances"];
        colWidths = [14, 18, 12, 24, 13, 36];
    }

    const table = new Table({
        head: [],
        colWidths: colWidths,
        wordWrap: true,
        wrapOnWordBoundary: false,
        style: {
            head: [], //disable colors in header cells
            border: [] //disable colors for the border
        }
    });

    table.push([{colSpan: head.length, content: "PSC Configuration Table", hAlign: "center"}]);
    table.push(head);

    let loopCntPd = 0,
        loopCntLpsc = 0;

    for (const gpscName in pscHierarchy) {
        let row1 = {};

        head.forEach((column) => {
            row1[column] = "";
        });

        row1["PSC Name"] = gpscName;

        const pdList = pscHierarchy[gpscName];
        loopCntPd = 0;

        for (const pdName in pdList) {
            if (loopCntPd > 0) {
                // for 2nd time create a empty low
                head.forEach((column) => {
                    row1[column] = "";
                });
            }
            const pdOn = pscHierarchy[gpscName][pdName]["status"];

            row1["PD Name"] = pdName;
            row1["PD Status"] = pdOn;

            const lpscList = pscHierarchy[gpscName][pdName]["lpsc_list"];
            loopCntLpsc = 0;
            if (level > 0) {
                for (const lpscName in lpscList) {
                    if (loopCntLpsc > 0) {
                        // for 2nd time create a empty low
                        head.forEach((column) => {
                            row1[column] = "";
                        });
                    }

                    const lpscOn = lpscList[lpscName]["status"];
                    const connectedIps = lpscList[lpscName]["controlled_ip_instances"];
                    if (level > 0) {
                        row1["LPSC Name"] = lpscName;
                        row1["LPSC Status"] = lpscOn;
                    }
                    if (level > 1) {
                        row1["Controlled IP Instances"] = connectedIps.length > 0 ? connectedIps[0] : " ";
                    }

                    table.push(Object.values(row1));
                    if (level > 1) {
                        for (let i = 1; i < connectedIps.length; i++) {
                            let row2 = {};
                            head.forEach((column) => {
                                row2[column] = "";
                            });
                            row2["Controlled IP Instances"] = connectedIps[i];
                            table.push(Object.values(row2));
                        }
                    }
                    loopCntLpsc++;
                }

                if (loopCntLpsc == 0) {
                    // no lpsc , so push the row
                    table.push(Object.values(row1));
                }
            } else {
                table.push(Object.values(row1));
            }

            loopCntPd++;
        }

        if (loopCntPd == 0) {
            // no pd , so push the row
            table.push(Object.values(row1));
        }
    }

    console.log("\n");
    console.log(table.toString());
}

module.exports = {
    getPscData,
    createPscHierarchyfromLpscData,
    printPscData
};
