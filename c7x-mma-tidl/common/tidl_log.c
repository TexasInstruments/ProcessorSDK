/*
* module name       :TIDL
*
* module descripton :TI Deep learning Library module is TIs CNN/DNN
*                    acceleration on EVE and DSP
*
* Copyright (C) 2016-2017 Texas Instruments Incorporated - http://www.ti.com/
*
* ALL RIGHTS RESERVED
*
*
*/

/**
----------------------------------------------------------------------------
@file    tidl_fileio_tb.c
@brief   This file contains test bench Fast FILE IO
@version 0.1 (Feb 2018) : Initial version [ADK]
----------------------------------------------------------------------------
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <app_log.h>


app_log_shared_mem_t g_app_log_shared_mem
__attribute__ ((section(".bss:app_log_mem")))
__attribute__ ((aligned(4096)));

#if 0
void appLogvPrintf(const char *format, va_list va_args_ptr);
int32_t tidl_printf(const char *format, ...)
{
  int status;
  va_list args;

  va_start(args, format);
  appLogvPrintf(format, args);
  va_end(args);

  return status;
}
int main()
{
  int status, idx = 1;
  app_log_init_prm_t log_init_prm;
  appLogInitPrmSetDefault(&log_init_prm);

  log_init_prm.shared_mem = &g_app_log_shared_mem;
  log_init_prm.self_cpu_index = 1;
  strncpy(log_init_prm.self_cpu_name, "C7x_0" , APP_LOG_MAX_CPU_NAME);
  log_init_prm.log_rd_max_cpus = 2;
  log_init_prm.device_write = NULL;
  status = appLogWrInit(&log_init_prm);
  if(status == -1)
  {
    return -1;
  }

  tidl_printf("Hello From C7x !!!\n");


  while(idx)
  {
    tidl_printf("%d\n",idx++);
  }
  return 0;
}

#else
int tidl_log_init(void)
{
  int status;
  app_log_init_prm_t log_init_prm;
  appLogInitPrmSetDefault(&log_init_prm);

  log_init_prm.shared_mem = &g_app_log_shared_mem;
  log_init_prm.self_cpu_index = 1;
  strncpy(log_init_prm.self_cpu_name, "C7x_0" , APP_LOG_MAX_CPU_NAME);
  log_init_prm.log_rd_max_cpus = 2;
  log_init_prm.device_write = NULL;
  status = appLogWrInit(&log_init_prm);
  if(status == -1)
  {
    return -1;
  }
  return 0;
}
#endif

