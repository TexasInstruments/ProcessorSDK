var ds;
var debugServer;
var script;

/*------------------------------------- EDIT FILE PATH TO ABSOLUTE PATH BEFORE RUNNING ------------------------------------- */
var filePath = "/home/cgt/ti/pdm_utils/tools/jtag_power_analysis_tool/debugger_scripts/ccs12/pm_data_generator_ccs12.js";

function pmDataGenerator(soc, selector) {
    try {
        var dsMCU1_0 = debugServer.openSession(".*MCU_Cortex_R5_0|.*WKUP_Cortex_R5_0");
        dsMCU1_0.target.connect();

        if (!new java.io.File(filePath).exists()) {
            throw new Error("pm_data_generator_ccs12: file " + filePath + " not found, please check the path in script");
        }

        var dirPath = new java.io.File(filePath).getParent();
        var addressDir = new java.io.File(dirPath, "../../soc_data/" + soc + "/mem_addr").getCanonicalPath();

        /* If input CSV directory does not exist, throw error */
        if (!new java.io.File(addressDir).exists() || !new java.io.File(addressDir).isDirectory()) {
            throw new Error("Address csv directory Not Found for SoC: " + soc);
        }

        /* Based on the selector, read memory addresses and create output files */
        var memValuesDir = new java.io.File(dirPath, "../../memory_values_csv/" + soc).getCanonicalPath();

        var paths = {
            pll: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv"],
            psc: ["soc_psc_data_addr.csv"],
            clock_tree: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_clk_data_mux_addr.csv"],
            pet: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_psc_data_addr.csv", "soc_clk_data_mux_addr.csv"],
            all: ["soc_pll_data_addr.csv", "soc_clk_data_div_addr.csv", "soc_psc_data_addr.csv", "soc_clk_data_mux_addr.csv"]
        };

        for (var key in paths) {
            if (selector === key) {
                for (var i = 0; i < paths[key].length; i++) {
                    var file = paths[key][i];
                    var pathToAddr = new java.io.File(addressDir, file).getCanonicalPath();
                    try {
                        var addrFile = new java.io.File(pathToAddr);
                        var addrReader = new java.io.BufferedReader(new java.io.FileReader(addrFile));
                        var addrs = [];
                        var addr;
                        while ((addr = addrReader.readLine()) !== null) {
                            var statusData = dsMCU1_0.memory.readWord(0, addr);
                            addrs.push(addr + "," + statusData);
                        }
                        addrReader.close();
                        var outputFile = file.replace(".csv", "_val.csv");
                        saveCSVFile(new java.io.File(memValuesDir, outputFile).getCanonicalPath(), addrs.join("\n"));
                        print("File generated: " + new java.io.File(memValuesDir, outputFile).getCanonicalPath());
                    } catch (e) {
                        print("Error reading memory: " + e.message);
                    }
                }
            }
        }
    } catch (e) {
        print("Error: " + e.message);
    }

    return 0;
}

function saveCSVFile(filePath, data) {
    try {
        var dirPath = new java.io.File(filePath).getParent();
        var dir = new java.io.File(dirPath);

        if (!dir.exists()) {
            dir.mkdirs();
        }

        var csvContent = data;
        var file = new java.io.FileWriter(filePath);
        file.write(csvContent);
        file.flush();
        file.close();
    } catch (e) {
        print("Error writing to file: " + e.message);
    }
}

/* Check to see if running from within CCS12 Scripting Console */
var withinCCS = ds !== undefined;

/* Create scripting environment and get debug server if running standalone */
if (!withinCCS) {
    /* Import the DSS packages into our namespace to save on typing */
    importPackage(Packages.com.ti.debug.engine.scripting);
    importPackage(Packages.com.ti.ccstudio.scripting.environment);
    importPackage(Packages.java.lang);

    script = ScriptingEnvironment.instance();

    debugServer = script.getServer("DebugServer.1");
} else {
    /* otherwise leverage existing scripting environment and debug server */
    debugServer = ds;
    script = env;
}

/* Run this script with the following command: pmDataGenerator("soc_name", selector="all");
   e.g. pmDataGenerator("j722s", selector="all"); */
