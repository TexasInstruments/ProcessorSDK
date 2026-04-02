const fs = require('fs');
const os = require('os');
const path = require('path');
const { spawn } = require('child_process');
const { execSync } = require('child_process');

/**
 * ProcessKiller Class to kill child process terminals
 * More specifically, the methods of this class are used to locate and kill OpenOCD and Trace32 terminals created by executeT32 function in execute_t32.js
 * for better user experience.
 */
class ProcessKiller {
    /**
     * Gets the running processes using ps -a command, and returns list of processes
     * @returns {Array<String>} -> List of string of type `<PID> <TTY> <TIME> <CMD>`
     */
    static getRunningProcessList() {
        try {
            /* Execute ps -a command and get output */
            const output = execSync('ps -a', { encoding: 'utf-8' });
            return output.trim().split('\n');
        } catch (error) {
            console.error('Error getting process list:', error.message);
            return [];
        }
    }

    /**
     * Given a process name (eg openocd or t32), returns true if that process is running, false otherwise
     * @param {String} processName -> Name of process to check if it is running
     * @returns 
     */
    static isProcessRunning(processName) {
        try {
            const runningProcesses = this.getRunningProcessList();
            const processes = this.parseProcesses(runningProcesses);
            const targetProcesses = this.findTargetProcesses(processes, [processName]);
            return targetProcesses.length > 0;
        } catch (error) {
            console.error(`Error checking if ${processName} is running:`, error.message);
            return false;
        }
    }

    /**
     * Parses process info strings of type `<PID> <TTY> <TIME> <CMD>` and convert them into objects
     * @param {Array<String>} processList 
     * @returns {Array<Object>} 
     */
    static parseProcesses(processList) {
        /*Skip the header line and parse each process line*/
        const processes = processList.slice(1).map(line => {
            const [pid, pts, time, command] = line.trim().split(/\s+/);
            return {
                pid: parseInt(pid),
                pts: pts,
                time: time,
                command: command
            };
        });

        return processes;
    }

    /**
     * Filters the list of running processes for the processes that we want to terminate
     * @param {Array<Object>} processes -> List of running processes
     * @param {Array<String>} killList -> Names of processes to be terminated
     * @returns {Array<Object>} -> filtered list of processes, contains only those processes that need to be terminated
     */
    static findTargetProcesses(processes, killList) {
        return processes.filter(proc => {
            const cmd = proc.command.toLowerCase();
            return killList.some(killItem => cmd.includes(killItem.toLowerCase()));
        });
    }

    /**
     * Given the pts value for a process, e.g. pts/0, it terminated that process and closes the terminal 
     * @param {String} pts -> pts value for a process, e.g. pts/0
     * @returns {void}
     */
    static killTerminal(pts) {
        if (!pts) {
            console.log('No terminal (pts) specified');
            return;
        }

        try {
            /* Try SIGTERM first */
            execSync(`pkill -TERM -t ${pts}`);
        } catch (error) {
            try {
                /* If SIGTERM fails, try SIGKILL */
                execSync(`pkill -KILL -t ${pts}`);
            } catch (killError) {
                console.error(`Failed to kill terminal ${pts}:`, killError.message);
                throw killError;
            }
        }
    }

    /**
     * Terminates each process given in the killList 
     * @param {Array<String>} killList -> names of processes that are needed to be terminated 
     * @returns {void}
     */
    static async killTargetProcesses(killList) {     
        /* Get and parse process list */
        const runningProcesses = this.getRunningProcessList();
        if (runningProcesses.length === 0) {
            console.log('No processes found');
            return;
        }

        const processes = this.parseProcesses(runningProcesses);
        const targetProcesses = this.findTargetProcesses(processes, killList);

        if (targetProcesses.length === 0){
            console.log('No target processes found');
            return;
        }

        /* Get unique terminals to kill */
        const uniqueTerminals = [...new Set(targetProcesses.map(proc => proc.pts))];

        /* Kill each terminal */
        for (const terminal of uniqueTerminals) {
            try {
                this.killTerminal(terminal);
            } catch (error) {
                console.error(`Failed to process terminal ${terminal}:`, error.message);
            }
        }
    }
}


/**
 * Starts OpenOCD instance with given device config file if not already running
 * @param {string} pathToOpenocdCfg 
 * @returns {Promise<{success: boolean, error: any, wasAlreadyRunning: boolean}>} 
 */
function runOpenocd(pathToOpenocdCfg) {
    return new Promise((resolve, reject) => {
        /* Check if OpenOCD is already running */
        if (ProcessKiller.isProcessRunning('openocd')) {
            resolve({
                success: true,
                error: null,
                wasAlreadyRunning: true
            });
            return;
        }

        const command = `openocd -f ${pathToOpenocdCfg}`;
        
        const wrapperCommand = [
            '--',
            'bash',
            '-c',
            `echo "Starting OpenOCD..."; ${command}`
        ];

        const openocdProcess = spawn('gnome-terminal', wrapperCommand, {
            stdio: ['inherit', 'pipe', 'inherit'], 
            detached: false
        });

        openocdProcess.unref();

        openocdProcess.on('error', (error) => {
            console.error('Failed to start OpenOCD:', error);
            resolve({
                success: false,
                error: error,
                wasAlreadyRunning: false
            });
        });

        resolve({
            success: true,
            error: null,
            wasAlreadyRunning: false
        });
    });
}

/**
 * Runs Trace32 session with given device config or reuses existing instance to load script to read memory addresses
 * @param {String} pathToT32Cfg
 * @param {String} soc
 * @param {String} selector
 * @returns {Promise<{success: boolean, error: any, wasAlreadyRunning: boolean}>} 
 */
function runT32(pathToT32Cfg, soc, selector) {
    return new Promise((resolve, reject) => {
        const isT32Running = ProcessKiller.isProcessRunning('t32');
        
        const cmmScriptDir = path.resolve(__dirname, `../../src/trace32_scripts`);
        const addressesDir = path.resolve(__dirname, `../../soc_data/${soc}/mem_addr`);
        const memValuesCsvDir = path.resolve(__dirname, `../../memory_values_csv/${soc}`);
        const t32InstallPath = '/usr/bin/t32';
        const t32remInstallPath = '/opt/t32/bin/pc_linux64/t32rem';
        const port = '20000'; /* Default Trace32 Remote API Port */

        let cmmScript = '';
        if (!isT32Running) {
            cmmScript = `DO ${pathToT32Cfg} \n`;
        }

        if (selector == "all" || selector == "pll" || selector == "clock_tree" || selector == "pet") {
            const pathToPllAddr = path.join(addressesDir, 'soc_pll_data_addr.csv');
            const pathToReadPllCmm = path.join(cmmScriptDir, 'read_pll.cmm');
            cmmScript += `DO ${pathToReadPllCmm} "${pathToPllAddr}" "${memValuesCsvDir}" "soc_pll_data_val.csv" \n`;

            const pathToClkDivAddr = path.join(addressesDir, 'soc_clk_data_div_addr.csv');
            const pathToClkMuxAddr = path.join(addressesDir, 'soc_clk_data_mux_addr.csv');
            const pathToReadClkCmm = path.join(cmmScriptDir, 'read_clk.cmm');
            cmmScript += `DO ${pathToReadClkCmm} "${pathToClkDivAddr}" "${pathToClkMuxAddr}" "${memValuesCsvDir}" "soc_clk_data_div_val.csv" "soc_clk_data_mux_val.csv" \n`;
        }

        if (selector == "all" || selector == "psc" || selector == "pet" || selector == "clock_tree") {
            const pathToPscAddr = path.join(addressesDir, 'soc_psc_data_addr.csv');
            const pathToReadPscCmm = path.join(cmmScriptDir, 'read_psc.cmm');
            cmmScript += `DO ${pathToReadPscCmm} "${pathToPscAddr}" "${memValuesCsvDir}" "soc_psc_data_val.csv" \n`;

        }

        const markerFilePath = path.join(memValuesCsvDir, 'marker.csv');
        cmmScript += `OPEN #10 "${markerFilePath}" /Create \n`
        cmmScript += `CLOSE #10 \n`

        /* Only add QUIT if we started a new instance */
        if (!isT32Running) {
            cmmScript += 'QUIT'
        }

        const tmpDir = os.tmpdir();
        const tmpCmmPath = path.join(tmpDir, 'wrapper.cmm');
        fs.writeFileSync(tmpCmmPath, cmmScript);

        if (isT32Running) {
            /**Utilize existing trace32 instance through remote api */
            const command = `${t32remInstallPath} localhost port=${port} protocol=NETASSIST 'DO ${tmpCmmPath}'`;
            
            const trace32Process = spawn('bash', ['-c', command], {
                stdio: 'inherit',
                detached: false
            });

            trace32Process.on('error', (error) => {
                console.error('Failed to execute script in existing Trace32:', error);
                resolve({
                    success: false,
                    error: error,
                    wasAlreadyRunning: true
                });
            });

            resolve({
                success: true,
                error: null,
                wasAlreadyRunning: true
            });
        } else {
            /*Start new trace32 instance */
            const command = `${t32InstallPath} -m gdb -T '-s ${tmpCmmPath}'`;
            const wrapperCommand = [
                '--',
                'bash',
                '-c',
                `echo "Starting trace32..."; ${command}`
            ];

            const trace32Process = spawn('gnome-terminal', wrapperCommand, {
                stdio: 'inherit',
                detached: false
            });

            trace32Process.on('error', (error) => {
                console.error('Failed to start trace32:', error);
                resolve({
                    success: false,
                    error: error,
                    wasAlreadyRunning: false
                });
            });

            resolve({
                success: true,
                error: null,
                wasAlreadyRunning: false
            });
        }
    });
}

/**
 * Main function to execute OpenOCD and Trace32 operations
 * @param {string} pathToOpenocdCfg 
 * @param {string} pathToT32Startup
 * @param {string} soc
 * @param {string} selector
 * @returns {{success: boolean, error: any}} 
 */
async function executeT32(pathToOpenocdCfg, pathToT32Startup, soc, selector) {
    try {
        const openocdResult = await runOpenocd(pathToOpenocdCfg);
        const t32Result = await runT32(pathToT32Startup, soc, selector);

        const memValuesCsvDir = path.resolve(__dirname, `../../memory_values_csv/${soc}`);
        const markerFilePath = path.join(memValuesCsvDir, 'marker.csv');
        while(!fs.existsSync(markerFilePath)){
            /**Busy Wait */
        }

        /* Only kill processes that we started */
        if (!openocdResult.wasAlreadyRunning) {
            await ProcessKiller.killTargetProcesses(killList=['openocd'])
        }

        if(!t32Result.wasAlreadyRunning){
            await ProcessKiller.killTargetProcesses(killList=['t32'])
        }

        return {
            success: true,
            error: null
        }
        
    } catch (error) {
        console.error('Error in executing OpenOCD+Trace32:', error);
        return {
            success: false,
            error: error
        };
    }
}

async function main() {
    // running from terminal
    const minimist = require("minimist");
    const args = minimist(process.argv.slice(2), {
        string: ["soc", "selector", "arg1", "arg2"],
        alias: {
            s: "soc",
            sel: "selector",
            pathToOpenocdCfg: "arg1",
            pathToT32Startup: "arg2"
        },
        default: {
            selector: "all"
        }
    });
    const {soc, selector, arg1, arg2} = args;
    if (soc && arg1 && arg2) {
       await executeT32(arg1,arg2,soc,selector);
    }
    else {
        throw new Error("Invalid Arguments. Please provide --soc, --selector, --arg1 and --arg2");
    }
    process.exit(0);
    return 0;
}

main();
