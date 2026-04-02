/* Use Exception */
var Exception = xdc.useModule('ti.sysbios.family.c64p.Exception');

/* Register the NMI hook */
Exception.nmiHook     = "&watchdogEsmHook";
Exception.enablePrint = false;

