/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
//File Name: launch_j721e.js
//Description:
//   Launch the DMSC firmware and board configuration from R5F.
//
//Usage:
//
//From CCS Scripting console
//  1. loadJSFile "C:\\ti\\launch_j721e.js"
//
//Note:
//  1. Search for "edit this" to look at changes that need to be edited
//     for your usage.
//


//<!!!!!! EDIT THIS !!!!!>
// Set this to 1, if using 'FreeRTOS'
isFreertos = 1;  
// Set this to 1, if using 'SafeRTOS'
isSafertos = 0;  

//PDK path. Edit this
pdkPath = "/ti/j7presi/workarea/pdk";

//path to board config elf
pathSciclient = pdkPath+"/packages/ti/drv/sciclient/tools/ccsLoadDmsc/j721e/"
ccs_init_elf_file = pathSciclient+"sciclient_ccs_init_mcu1_0_release.xer5f";

// Set this to 1, to clear 'Secure Claim' Bit in CLEC register
clearCLECSecureClaimFlag = 1;

loadSciserverFlag = 1;
if(isFreertos == 1)
{
    //Path to FreeRTOS sciserver
    sciserver_elf_file = pathSciclient+"sciserver_testapp_freertos_mcu1_0_release.xer5f";
}
else if(isSafertos == 1)
{
    //Path to SafeRTOS sciserver
    sciserver_elf_file = pathSciclient+"sciserver_testapp_safertos_mcu1_0_release.xer5f";
}

//path to sysfw bin
sysfw_bin = pdkPath+"/packages/ti/drv/sciclient/soc/sysfw/binaries/ti-fs-firmware-j721e-gp.bin"

//<!!!!!! EDIT THIS !!!!!>

// Import the DSS packages into our namespace to save on typing
importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang);
importPackage(Packages.java.io);

function updateScriptVars()
{
    //Open a debug session
    dsMCU1_0 = debugServer.openSession( ".*MCU_Cortex_R5_0" );
    dsDMSC_0 = debugServer.openSession( ".*DMSC_Cortex_M3_0" );
}

function printVars()
{
    updateScriptVars();
}

function connectTargets()
{
    /* Set timeout of 20 seconds */
    script.setScriptTimeout(200000);
    updateScriptVars();
    sysResetVar=dsDMSC_0.target.getResetType(1);
    sysResetVar.issueReset();
    print("Connecting to DMSC_Cortex_M3_0!");
    // Connect targets
    dsDMSC_0.target.connect();
    print("Fill R5F ATCM memory...");
    dsDMSC_0.memory.fill(0x61000000, 0, 0x8000, 0);
    print("Writing While(1) for R5F")
    dsDMSC_0.memory.writeWord(0, 0x61000000, 0xE59FF004); /* ldr        pc, [pc, #4] */
    dsDMSC_0.memory.writeWord(0, 0x61000004, 0x38);       /* Address 0x38 */
    dsDMSC_0.memory.writeWord(0, 0x61000038, 0xEAFFFFFE) /* b          #0x38 */
    print("Loading DMSC Firmware ... " + sysfw_bin);
    // Load the DMSC firmware
    dsDMSC_0.memory.loadRaw(0, 0x40000, sysfw_bin, 32, false);
    print("DMSC Firmware Load Done...");
    // Set Stack pointer and Program Counter
    stackPointer = dsDMSC_0.memory.readWord(0, 0x40000);
    progCounter = dsDMSC_0.memory.readWord(0, 0x40004);
    dsDMSC_0.memory.writeRegister("SP", stackPointer);
    dsDMSC_0.memory.writeRegister("PC", progCounter);
    print( "DMSC Firmware run starting now...");
    // Run the DMSC firmware
    dsDMSC_0.target.runAsynch();
    // Run the DDR Configuration
    print("Running the DDR configuration... Wait till it completes!");
    dsDMSC_0.target.halt();
    dsDMSC_0.expression.evaluate("J7ES_LPDDR4_Config_Late()");
    dsDMSC_0.target.runAsynch();

    print("Connecting to MCU Cortex_R5_0!");

    // Connect the MCU R5F
    dsMCU1_0.target.connect();
    // This is done to support other boot modes. OSPI is the most stable.
    // MMC is not always stable.
    bootMode = dsMCU1_0.memory.readWord(0, 0x43000030) & 0xF8;
    print (" WKUP Boot Mode is " + bootMode);
    mainBootMode = dsMCU1_0.memory.readWord(0, 0x100030) & 0xFF;
    print (" Main Boot Mode is " + mainBootMode);
    if ((bootMode != 0x38) || (mainBootMode != 0x11))
    {
        print("Disable MCU Timer for ROM clean up");
        dsMCU1_0.memory.writeWord(0, 0x40400010, 0x1); /* Write reset to MCU Timer 0. Left running by ROM */
        dsMCU1_0.memory.writeWord(0, 0x40F80430, 0xFFFFFFFF); /* Clear Pending Interrupts */
        dsMCU1_0.memory.writeWord(0, 0x40F80018, 0x0); /* Clear Pending Interrupts */
        // Reset the R5F to be in clean state.
        dsMCU1_0.target.reset();
        // Load the board configuration init file.
        dsMCU1_0.expression.evaluate('GEL_Load("'+ ccs_init_elf_file +'")');
        // Run Asynchronously
        dsMCU1_0.target.runAsynch();
        print ("Running Async");
        // Halt the R5F and re-run.
        dsMCU1_0.target.halt();
    }
    // Reset the R5F to be in clean state.
    dsMCU1_0.target.reset();
    print("Running the board configuration initialization from R5!");
    // Load the board configuration init file.
    dsMCU1_0.memory.loadProgram(ccs_init_elf_file);
    // Halt the R5F and re-run.
    dsMCU1_0.target.halt();
    // Run Synchronously for the executable to finish
    dsMCU1_0.target.run();
}

function disconnectTargets()
{
    updateScriptVars();
    // Reset MCU1_0
    dsMCU1_0.target.reset();
    // Disconnect targets
    dsDMSC_0.target.disconnect();
}

function sampleDDRCheck ()
{
    print("Running DDR Memory Checks....");
    dsMCU1_0.memory.fill (0x80000000, 0, 1024, 0xA5A5A5A5);
    ar = dsMCU1_0.memory.readWord(0, 0x80000000, 1024);
    fail = 0
    for (i = 0; i < ar.length; i++) { 
            x = ar[i]; 
            if (x != 0xA5A5A5A5)
            {
                fail = 1;
            }
        } 
    if (fail == 1)
    {
        print ("0x80000000: DDR memory sample check failed !!");
    }
    dsMCU1_0.memory.fill (0x81000000, 0, 1024, 0x5A5A5A5A);
    ar = dsMCU1_0.memory.readWord(0, 0x81000000, 1024);
    fail = 0
    for (i = 0; i < ar.length; i++) { 
            x = ar[i]; 
            if (x != 0x5a5a5a5a)
            {
                fail = 1;
            }
        } 
    if (fail == 1)
    {
        print ("0x81000000: DDR memory sample check failed !!");
    }
}

function loadSciserver()
{
    updateScriptVars();
    print("######################################################################################");
    print("Loading Sciserver Application on MCU1_0. This will service RM/PM messages");
    print("If you do not want this to be loaded update the launch script to make loadSciserverFlag = 0");
    print("If you want to load and run other cores, please run the MCU1_0 core after Sciserver is loaded. ");
    print("######################################################################################");
    dsMCU1_0.expression.evaluate('GEL_Load("'+ sciserver_elf_file +'")');
}

function clearCLECSecureClaim()
{
    dsC7X1_0 = debugServer.openSession( ".*C71X_0" );

    dsC7X1_0.target.connect();

    c7x_binary = pdkPath+"/packages/ti/drv/sciclient/tools/clearClecSecureClaim/sciclient_clear_clec_secure_claim_c7x_1_release.xe71";

    dsC7X1_0.memory.loadProgram(c7x_binary);
    dsC7X1_0.target.runAsynch();

    // Halt and re-run since the startup code image doesn't have a main function.
    dsC7X1_0.target.halt();
    dsC7X1_0.target.runAsynch();

    dsC7X1_0.target.disconnect();
}

function doEverything()
{
    printVars();
    connectTargets();
    disconnectTargets();
    sampleDDRCheck ();
    if (clearCLECSecureClaimFlag == 1)
    {
        print("Clearing CLEC Secure Claim...");
        clearCLECSecureClaim();
    }
    if (loadSciserverFlag == 1)
    {
        loadSciserver();
    }
    print("Happy Debugging!!");
}

var ds;
var debugServer;
var script;

// Check to see if running from within CCSv4 Scripting Console
var withinCCS = (ds !== undefined);

// Create scripting environment and get debug server if running standalone
if (!withinCCS)
{
    // Import the DSS packages into our namespace to save on typing
    importPackage(Packages.com.ti.debug.engine.scripting);
    importPackage(Packages.com.ti.ccstudio.scripting.environment);
    importPackage(Packages.java.lang);

    // Create our scripting environment object - which is the main entry point into any script and
    // the factory for creating other Scriptable ervers and Sessions
    script = ScriptingEnvironment.instance();

    // Get the Debug Server and start a Debug Session
    debugServer = script.getServer("DebugServer.1");
}
else // otherwise leverage existing scripting environment and debug server
{
    debugServer = ds;
    script = env;
}

doEverything();
