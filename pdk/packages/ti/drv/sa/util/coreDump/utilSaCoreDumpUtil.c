/*
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/* Generate the core dump file */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __LINUX_USER_SPACE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#else
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_xmcAux.h>
#include <ti/csl/csl_chip.h>
#endif

/* Sa Include files */
#include <ti/drv/sa/salld.h>

/* CSL files */
#include <ti/csl/cslr_device.h>

#define  SAUTEST_CORE_DUMP_SIZE     sa_CORE_DUMP_BUF_SIZE
#define  CORE_DUMP_FILE_NAME_SIZE   256
uint32_t coreDumpBuf[SAUTEST_CORE_DUMP_SIZE];
#define salld_SIM_MAX_CHANNELS      1  /* Maximum SALLD Channels         */
#define CACHE_LINESZ    64
#define SYS_CACHE_LINE_SIZE CACHE_LINESZ

#define SA_CFG_BASE_ADDR  CSL_NETCP_CFG_SA_CFG_REGS
#define SA_CFG_BLK_SZ (0x40000)

/* Global variables to hold virtual address of various subsystems */
void *fw_saCfgVaddr;

extern void* salld_sim_malloc (uint32_t num_bytes);
extern void salld_sim_free (void* ptr, uint32_t size);

/* Sa lld specific */
int           sizes[sa_N_BUFS];
Sa_Handle     handle;
void*         bases[sa_N_BUFS];
static int    aligns[sa_N_BUFS];

/******************************************************************************
 * FUNCTION PURPOSE: Implements SALLD's debugInfo() API call.
 ******************************************************************************
 * DESCRIPTION: Handles SALLD API calls to debugInfo(). All INFORMATIONAL msgs
 *              are ignored; if the msgType is FATAL_ERROR or undefined,
 *              a call to salldsimFatalError() is made.
 *
 * CALLING FORMAT:
 *   salldsimSalldDebugInfo (void * moduleID,	  - ID and channel of FIU
 *                       uint16_t msgType,		  - FATAL_ERROR (2) or
 *                                                  INFORMATIONAL (0)
 *                       uint16_t messageCode,	  - not used
 *                       uint16_t msgLength,	  - not used
 *                       uint16_t *supportingData)- not used
 *
 * RETURN VALUES:
 *   None
 *
 *****************************************************************************/
void salldsimDebugInfo (void* mID, uint16_t msgType, uint16_t messageCode,
                        uint16_t msgLength,  uint16_t *supportingData)
{


} /* salldsimSalldDebugInfo */


/**
 *  @brief  Callout to externally supplied system to request a new security key. This function may be triggered
 *  by either the sslldChanSendData() or sslldChanReceiveData() APIs. The application should call the
 *  Sa_chanControl() API to pass the new key when it is available.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   keyReq       Pointer to SALLD key Request structure.
 *
 *  @sa Sa_KeyRequest_t
 *
 */
void salldsimChanKeyRequest (Sa_ChanHandle handle, Sa_KeyRequest_t* keyReq)
{

}

/**
 *  @brief  Callout to externally supplied system to allocate the security context with the specified size.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scReqInfo    Pointer to SALLD security context Control Information structure.
 *
 *  @sa Sa_ScReqInfo_t
 *
 */
void salldsimScAlloc (Sa_ChanHandle handle, Sa_ScReqInfo_t* scReqInfo)
{

}

/**
 *  @brief  Callout to externally supplied system to release the security context with the specified ID.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scID         Security Context ID
 *
 */
void salldsimScFree (Sa_ChanHandle handle, uint16_t scID)
{

}

/**
 *  @brief  Callout to externally supplied system to register the security channel with its software
 *  routing information.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
void salldsimChanRegister (Sa_ChanHandle handle, Sa_SWInfo_t* pSwInfo)
{

}

/**
 *  @brief  Callout to externally supplied system to un-register the security channel with its software
 *  routing information.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
void salldsimChanUnRegister (Sa_ChanHandle handle, Sa_SWInfo_t* pSwInfo)
{

}

/**
 *  @brief  Callout to externally supplied system to send an Null packet to the SA sub-system
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   pktInfo      Pointer to the packet info structure.
 *
 *  @sa Sa_PktInfo_t
 *
 */
void salldsimChanSendNullPkt (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo)
{

}


/* Global SALLD call-out function table */
static Sa_CallOutFuncs_t salldsim_calloutFunc = {
    salldsimDebugInfo,               /* Debug function pointer */
    salldsimChanKeyRequest,          /* Key Request function Pointer */
    salldsimScAlloc,                 /* Security Context Allocation function pointer */
    salldsimScFree,                  /* Security Context Free Function pointer */
    salldsimChanRegister,            /* Channel Registration Function pointer */
    salldsimChanUnRegister,          /* Channel UnRegister Function pointer */
    salldsimChanSendNullPkt          /* Channel Send Null Packet function pointer */
};

#ifdef __LINUX_USER_SPACE
/* File descriptor for /dev/mem */
static int dev_mem_fd;

static uint32_t utilgAddr(uint32_t x)
{
  	return (x);
}
#else
uint32_t utilgAddr(uint32_t x)
{
#ifdef _TMS320C6X
	if ((x >= 0x800000) && (x < 0x900000))
	  x = (1 << 28) | (DNUM << 24) | x;
#endif
  	return (x);
}
#endif


#ifdef __LINUX_USER_SPACE
/***************************************************************************************
 * FUNCTION PURPOSE:  Function to map physical address to virtual memory space
 ***************************************************************************************
 * DESCRIPTION:  Function to map physical address to virtual memory space
 ***************************************************************************************/
void *fw_memMap
(
    void        *addr, /* Physical address */
    uint32_t    size   /* Size of block */
)
{
    void            *map_base,*virt_addr,*tmpAddr;
    uint32_t        page_sz;
    long            retval;
    uint32_t        mask = (size-1);
    uint32_t        offset;

    retval = sysconf(_SC_PAGE_SIZE);
    if (retval == -1)
    {
        printf("fw_memMap: Failed to get page size err=%s\n",
               strerror(errno));
        return (void *)0;
    }

    page_sz = (uint32_t)retval;

    if (size%page_sz)
    {
        printf("fw_memMap: error: block size not aligned to page size\n");
        return (void *)0;
    }

    if ((uint32_t)addr%page_sz)
    {
        printf("fw_memMap: error: addr not aligned to page size\n");
        return (void *)0;
    }

    map_base = mmap(0, size, (PROT_READ|PROT_WRITE), MAP_SHARED, dev_mem_fd, (off_t)addr);
    if(map_base == (void *) -1)
    {
        printf("fw_memMap: Failed to mmap \"dev/mem\" err=%s\n",
               strerror(errno));
        return (void *)0;
    }
    virt_addr = map_base;
    return(virt_addr);
}
#endif

/* Error identifier enumerations */
typedef enum {
  salld_SIM_ERR_NONE            = 0x0000, /* SALLD NO ERROR         */
  salld_SIM_ERR_SALLDGETNUMBUF  = 0x1000, /* Sa_chanGetNumBuf()   */
  salld_SIM_ERR_SALLDGETBUFDEF,           /* Sa_chanGetBufDef()   */
  salld_SIM_ERR_SALLDINIT,                /* Sa_chanInit()        */
  salld_SIM_ERR_SALLDOPEN,                /* salldChanOpen()        */
  salld_SIM_ERR_SALLDCLOSE,               /* Sa_chanClose()       */
  salld_SIM_ERR_SALLDCONTROL,             /* Sa_chanControl()     */
  salld_SIM_ERR_SALLDSENDDATA,            /* Sa_chanSendData()    */
  salld_SIM_ERR_SALLDRECEIVEDATA,         /* Sa_chanReceiveData() */
  salld_SIM_ERR_SALLDSTATS,               /* Sa_chanGetStats()    */
                                          /* SALLD SIM API ERRORS   */
  salld_SIM_ERR_INITCHN = 0x2000,         /* salldSim_init_chn()    */
  salld_SIM_ERR_OPENCHN,                  /* salldSim_open_chn()    */
  salld_SIM_ERR_CONFIGCHN,                /* salldSim_config_chn()  */
  salld_SIM_ERR_CLOSECHN,                 /* salldSim_close_chn()   */
  salld_SIM_ERR_NUMBUF,                   /* salld_SIM_MAX_ECOBUFS  */
  salld_SIM_ERR_GETVERSION,               /* salldSim_prnt_version()*/
  salld_SIM_ERR_SYSGETNUMBUF  = 0x3000,   /* Sa_numAlloc()          */
  salld_SIM_ERR_SYSGETBUFDEF,             /* Sa_alloc()             */
  salld_SIM_ERR_SYSINIT,                  /* Sa_init()              */
  salld_SIM_ERR_SYSCLOSE,                 /* Sa_free()              */
  salld_SIM_ERR_PDSPDNLD                  /* salld_download_pdsp_images()      */
} salldSimErr_e;

static int16_t saCollectData(char *fName)
{
  FILE *fp;
  int  i;
  int16_t ret;

  /* Create the sa Handle */
  fp = fopen (fName, "w+");

  if (fp == NULL)
  {
    printf (" Error in writing to SaCoreDump file \n");
  }

  ret = Sa_coreDump(handle, coreDumpBuf, SAUTEST_CORE_DUMP_SIZE);
  if (ret != sa_ERR_OK)
	  return (ret);

  for (i = 0; i < SAUTEST_CORE_DUMP_SIZE; i++)
  {
    fprintf (fp, "0x%08x\n", coreDumpBuf[i]);
  }
  fclose (fp);

  return (sa_ERR_OK);
}

static int16_t initSa(void)
{
  Sa_SizeCfg_t  sizeCfg;
  Sa_Config_t   cfg;

  int i;
  int16_t       ret_code;

  /* Get SALLD buffer requirements and ensure #buffers within limits */
  memset(&sizeCfg, 0, sizeof(Sa_SizeCfg_t));
  memset(&cfg, 0, sizeof(Sa_Config_t));
  sizeCfg.nMaxChan = salld_SIM_MAX_CHANNELS;
  sizeCfg.cacheLineSize = SYS_CACHE_LINE_SIZE;
  sizeCfg.ctrlBitMap = sa_SIZE_CONFIG_CREATE_SHADOW_INST;
#ifdef NSS_GEN2
  sizeCfg.ctrlBitMap |= sa_SIZE_CONFIG_SASS_GEN2;
#endif

#if defined(SOC_AM65XX)||defined(SOC_J721E)
  sizeCfg.ctrlBitMap |= sa_SIZE_CONFIG_SASS_UL_GEN2;
#endif

  ret_code = Sa_getBufferReq (&sizeCfg, sizes, aligns);

  if (ret_code != sa_ERR_OK)
	  return (ret_code);

  /* Allocate buffers */
  /* Allocate SALLD memory buffers & increment salld heap size. Data from the (global)
     variable salldBufs is copied into the (local) variable salldLocalBufs in order to
     correctly set salldLocalBufs' size parameter */
  for (i = 0; i < sa_N_BUFS; i++)
  {
     /*
      * The LLD system buffers may be shared among multiple cores.
      * Therefore global address should be provided.
      */
     bases[i]  =  (void *)utilgAddr((uint32_t)salld_sim_malloc (sizes[i]));
  }

  /* Create SALLD system */
  memset (&cfg, 0, sizeof (Sa_Config_t));
  cfg.ID  = 0xbabebeef;
  cfg.baseAddr = (uint32_t) fw_saCfgVaddr;
  cfg.sizeConfig = &sizeCfg;
  cfg.callTable = &salldsim_calloutFunc;
  cfg.ctrlBitMap |= sa_CONFIG_CTRL_BITMAP_TRIGGER_PKT_INFO_LOG;
  ret_code = Sa_create (&cfg, bases, &handle);
  return (ret_code);
}

/*-----------------------------------------------------------------
 * Function:  salld_sim_close_sa
 *
 * Description: SALLD close.
 *-----------------------------------------------------------------*/
static int16_t closeSa (void)
{
  int16_t ret_code;
  int i;

  /* Close the SALLD */
  ret_code = Sa_close (handle, bases);

  if (ret_code != sa_ERR_OK)
	  return (ret_code);

  /* Free memory allocated for SALLD buffers & adjust salld_heap_size */
  for (i = 0; i < sa_N_BUFS; i++)
  {
     salld_sim_free (bases[i], sizes[i]);
  }

  /* Free memory for SALLD buffer descriptors & adjust aux_heap_size */
  handle = NULL;

  return (ret_code);

}

#ifdef __LINUX_USER_SPACE
int main (int argc, char** argv)
#else
int main(void)
#endif
{
  char fileName[CORE_DUMP_FILE_NAME_SIZE];

#ifdef __LINUX_USER_SPACE
  int   check;
  /* Check if argument is provided or not */
  if (argc >= 2) {
    strncpy(fileName, argv[1], CORE_DUMP_FILE_NAME_SIZE-1);
	fileName[CORE_DUMP_FILE_NAME_SIZE-1] = 0;
    printf ("This utility will snapshot the internal state of the SASS and halt SASS after writing data to %s \n", fileName);
  }
  else {
    printf ("One argument for the file name is expected \n");
	printf ("Usage: saCoreDumpUtil.out <dump_file_name>\n");
	printf ("       Note that SASS would be halted after this utility is run \n");
	return (0);
  }

  printf (" Do you want to continue?\n");
  printf ("\n Press 0 to cancel, any other number to continue \n");

  scanf ("%d", &check);

  if (check == 0)
    return (0);

  if((dev_mem_fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
  {
        printf("fw_memAllocInit: Failed to open \"dev/mem\" err=%s\n",
               strerror(errno));
        return 0;
  }
    /* SA CFG Regs */
    fw_saCfgVaddr = fw_memMap((void*)SA_CFG_BASE_ADDR, SA_CFG_BLK_SZ);
#else
    strcpy(fileName, "dump.txt");
    fw_saCfgVaddr = (void *) SA_CFG_BASE_ADDR;
#endif

    if (!fw_saCfgVaddr)
    {
        printf("ERROR: Failed to map SA CFG registers\n");
        return (-1);
    }

  /* initialize Sa */
  if ((initSa() != sa_ERR_OK)) {
	  printf ("failed to initialize Sa \n");
	  return (-1);
  }

  /* Create the memory dump */
  if ((saCollectData(fileName) != sa_ERR_OK)) {
	  printf ("error in collecting debug data \n");
	  return (-1);
  }

  /* Close Sa */
  if ((closeSa() != sa_ERR_OK)) {
	  printf ("error in closing Sa \n");
	  return (-1);
  }

  printf ("Finished collecting core dump information in %s file \n", fileName);

  return (0);

}
/* nothing past this point */
