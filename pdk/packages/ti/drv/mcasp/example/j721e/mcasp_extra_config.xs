
if (Program.build.target.name == "A53F")
{
    /* Speed up system tick due to lack of granularity in Task_sleep API
     * causing timeouts with faster A72 execution
     */
    var Clock = xdc.module('ti.sysbios.knl.Clock');
    Clock.tickPeriod = 10;
}


