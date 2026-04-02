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

/*
 * loadJSFile ("/ti/j7/workarea/vision_apps/tools/scripts/run_ccs.js")
*/

/* Edit this to match your environment */
psdk_rtos_workarea = "/ti/j7/workarea"
load_mpu1=1
load_mcu2_0=0
load_mcu2_1=1
load_c6x_1=1
load_c6x_2=0
load_c7x_1=1

do_run_after_load=1

image_mcu2_0 = psdk_rtos_workarea + "/vision_apps/out/J721E/R5F/FREERTOS/debug/vx_app_rtos_mcu2_0.out"
image_mcu2_1 = psdk_rtos_workarea + "/vision_apps/out/J721E/R5F/FREERTOS/debug/vx_app_rtos_mcu2_1.out"
image_c6x_1 = psdk_rtos_workarea + "/vision_apps/out/J721E/C66/FREERTOS/debug/vx_app_rtos_c6x_1.out"
image_c6x_2 = psdk_rtos_workarea + "/vision_apps/out/J721E/C66/FREERTOS/debug/vx_app_rtos_c6x_2.out"
image_c7x_1 = psdk_rtos_workarea + "/vision_apps/out/J721E/C71/FREERTOS/debug/vx_app_rtos_c7x_1.out"
image_mpu1 = psdk_rtos_workarea + "/vision_apps/out/J721E/A72/FREERTOS/debug/vx_app_rtos_mpu1.out"


// Import the DSS packages into our namespace to save on typing
importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang)
importPackage(java.io);
importPackage(java.lang);

function updateScriptVars()
{
    // Open a debug session
    dsMCU2_0 = debugServer.openSession( ".*MAIN_Cortex_R5_0_0" );
    dsMCU2_1 = debugServer.openSession( ".*MAIN_Cortex_R5_0_1" );
    dsC6x_1 = debugServer.openSession( ".*C66xx_0" );
    dsC6x_2 = debugServer.openSession( ".*C66xx_1" );
    dsC7x_1 = debugServer.openSession( ".*C71X_0" );
    dsMPU_1 = debugServer.openSession( ".*CortexA72_0_0" );
    dsMPU_2 = debugServer.openSession( ".*CortexA72_0_1" );
}

function printVars()
{
    updateScriptVars();
}

function loadExecutable(ds, filename)
{
    print("Loading file ... " + filename);
    print("Connecting to target ...");
    ds.target.connect();

    if(ds == dsC7x_1)
    {
        print("Skipping target reset for C7x ...");
    }
    else
    {
        print("Reseting target ...");
        ds.target.reset();
    }
    print("Loading target ...");
    ds.memory.loadProgram(filename);
    ds.target.halt();
    print("Loading file ... " + filename + " done !!!");

    if( ds == dsMPU_1)
    {
        /* halt A72 core1 when core0 is loaded */
        dsMPU_2.target.connect();
        dsMPU_2.target.reset();
        dsMPU_2.target.halt();
    }
}

function runExecutable(ds, filename)
{
    print("Running file ... " + filename);
    ds.target.runAsynch();
}

function connectTargets()
{
    /* Set timeout of 60 seconds */
    script.setScriptTimeout(600000);
    updateScriptVars();
    if(load_c7x_1)
    {
        loadExecutable(dsC7x_1, image_c7x_1);
    }
    if(load_c6x_1)
    {
        loadExecutable(dsC6x_1, image_c6x_1);
    }
    if(load_c6x_2)
    {
        loadExecutable(dsC6x_2, image_c6x_2);
    }
    if(load_mcu2_0)
    {
        loadExecutable(dsMCU2_0, image_mcu2_0);
    }
    if(load_mcu2_1)
    {
        loadExecutable(dsMCU2_1, image_mcu2_1);
    }
    if(load_mpu1)
    {
        loadExecutable(dsMPU_1, image_mpu1);
    }
    if(do_run_after_load)
    {
        if(load_mpu1)
        {
            runExecutable(dsMPU_1, image_mpu1);
        }
        if(load_c7x_1)
        {
            runExecutable(dsC7x_1, image_c7x_1);
        }
        if(load_c6x_1)
        {
            runExecutable(dsC6x_1, image_c6x_1);
        }
        if(load_c6x_2)
        {
            runExecutable(dsC6x_2, image_c6x_2);
        }
        if(load_mcu2_0)
        {
            runExecutable(dsMCU2_0, image_mcu2_0);
        }
        if(load_mcu2_1)
        {
            runExecutable(dsMCU2_1, image_mcu2_1);
        }
    }
    print("All done !!!");
}

function doEverything()
{
    printVars();
    connectTargets();
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
