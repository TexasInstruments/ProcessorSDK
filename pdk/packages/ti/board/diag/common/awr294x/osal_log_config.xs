/* Enable Log */
var Main = xdc.useModule('xdc.runtime.Main');
var Task = xdc.useModule('ti.sysbios.knl.Task');
var Log = xdc.useModule('xdc.runtime.Log');
var Diags = xdc.useModule('xdc.runtime.Diags');
var LoggerBuf = xdc.useModule('xdc.runtime.LoggerBuf');
LoggerBuf.TimestampProxy = xdc.useModule('xdc.runtime.Timestamp');

/* Trace log */
var loggerBufParams = new LoggerBuf.Params();
loggerBufParams.bufType = LoggerBuf.BufType_CIRCULAR; //or BufType_FIXED
loggerBufParams.exitFlush = false;
loggerBufParams.instance.name = "_logInfo";
loggerBufParams.numEntries = 128;
var logger0 = LoggerBuf.create(loggerBufParams);
Main.common$.logger = logger0;

/* Turn on USER1_logs in Main module (all non-module functions) */
Main.common$.diags_USER1 = Diags.RUNTIME_ON;

/* Turn on USER1 logs in Task module */
Task.common$.diags_USER1 = Diags.RUNTIME_ON;




