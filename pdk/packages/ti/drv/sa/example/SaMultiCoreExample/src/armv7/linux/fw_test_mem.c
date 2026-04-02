/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
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

#include "../../testconn.h"
#include "fw_mem_allocator.h"
/* Semaphore Includes */
#include <semaphore.h>

/* shared memory includes */
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>


#define CACHE_WAIT 1

//uint8_t *testPktRam;
//uint32_t testPktOffset;

sa_Example_MemBlk_t* shared_ObjsBase;


uint8_t (*salldsimScBuf)[SALLDSIM_SC_BUF_SIZE];
void*     salldSimSegments[5];
uint16_t  salldSimSegUsedSizes[5];
uint16_t  salldSimSegAllocSizes[5];

#define SHM_KEY  100
static int shm_id = 0;
extern void    perror();
sem_t                       mutex;

/**
 *  @b Description
 *  @n
 *      Yields the task using something like sleep() or usleep().
 *
 *  @retval
 *      Not Applicable
 */
static void yield (void)
{
#ifdef __LINUX_USER_SPACE
    sleep(1);
//    sched_yield();
#endif
}


void APP_publishGlobalCfgDone(void)
{
	sa_MCExample_entry_t val;
	val.globalCfgDone = TRUE;
	fw_shmSetEntry(setGlobalCfgDoneCmd, val);
}

void APP_waitGlobalCfgDone(void)
{
	uint32_t globalCfgDone;
	sa_MCExample_entry_t val;

    do {
    	val = fw_shmGetEntry(getGlobalCfgDoneCmd);
    	globalCfgDone = val.globalCfgDone;
		yield();		
    } while(!globalCfgDone);
}

void APP_waitTestStatus(void)
{
	sa_MCExample_entry_t val;

    do {
    	val =  fw_shmGetEntry(getTestCompCntCmd);
		yield();
    } while (val.testCompCnt != sa_MC_EXAMPLE_NUM_CORES);

}

void APP_publishLocalCfgDone(void)
{
	uint32_t* addr;
	sa_Example_Comm_ShObj_t* shared_CommObjs = &shared_ObjsBase->comShObj;

    /* wait for the semaphore */
	sem_wait(&mutex);

    addr = (uint32_t *) &shared_CommObjs->localCfgDone;
	*addr += 1;
	SYS_CACHE_WB ((void *) addr, CACHE_LINESZ, CACHE_WAIT);

    /* Release the semaphore. */
	sem_post(&mutex);
}

void APP_chkExecute(void)
{
	Bool stopExecution;
	sa_MCExample_entry_t val;

   	val = fw_shmGetEntry(getstopExecuteCmd);
   	stopExecution = val.stopExecution;

	if (stopExecution == TRUE)
		while (1); /* halt here */
}

void APP_stopMCExecute(void)
{
	sa_MCExample_entry_t val;

   	val.stopExecution = TRUE;
   	fw_shmSetEntry(setstopExecuteCmd, val);	
	while (1); /* halt here */	
}

	
void APP_waitAllLocalCfgDone(void)
{
	uint32_t localCfgDone;
	sa_MCExample_entry_t val;

    do {
    	val = fw_shmGetEntry(getLocalCfgDoneCmd);
    	localCfgDone = val.localCfgDone;
		yield();		
    } while (localCfgDone != sa_MC_EXAMPLE_NUM_CORES);
}

void APP_waitSlaveTestStatus(void)
{
	sa_MCExample_entry_t val;
	int   slaveCnt;

	slaveCnt = (sa_MC_EXAMPLE_NUM_CORES-1);

	if (slaveCnt < 0)
		slaveCnt = 0;

    do {
    	val =  fw_shmGetEntry(getTestCompCntCmd);
		yield();
    } while (val.testCompCnt != slaveCnt);

}

void APP_publishTestStatus(void)
{
	uint32_t* addr;
	sa_Example_Comm_ShObj_t* shared_CommObjs = &shared_ObjsBase->comShObj;

    /* wait for the semaphore */
	sem_wait(&mutex);

    addr = (uint32_t *) &shared_CommObjs->testCompCnt;
	*addr += 1;
	SYS_CACHE_WB ((void *) addr, CACHE_LINESZ, CACHE_WAIT);

    /* Release the semaphore. */
	sem_post(&mutex);

}

void APP_waitSlaveCleanupCompletion(void)
{
	sa_MCExample_entry_t val;

    do {
    	val =  fw_shmGetEntry(getMcCleanupCntCmd);
		yield();
    } while (val.mcCleanupCnt != (sa_MC_EXAMPLE_NUM_CORES-1));

}

void APP_publishSlaveCleanupDone(void)
{
	uint32_t* addr;
	sa_Example_Comm_ShObj_t* shared_CommObjs = &shared_ObjsBase->comShObj;

    /* wait for the semaphore */
	sem_wait(&mutex);

    addr = (uint32_t *) &shared_CommObjs->mcCleanupCnt;
	*addr += 1;
	SYS_CACHE_WB ((void *) addr, CACHE_LINESZ, CACHE_WAIT);

    /* Release the semaphore. */
	sem_post(&mutex);

}

/* Initialize memory for the test framework */
int fw_test_mem_init(void)
{
	sa_MCExample_entry_t val;

	/* Allocate memory for the Security context buffers */
    salldsimScBuf = (uint8_t (*)[SALLDSIM_SC_BUF_SIZE]) 
    fw_memAlloc((SALLDSIM_NUM_SC_BUF * SALLDSIM_SC_BUF_SIZE), CACHE_LINESZ);
    if(salldsimScBuf == NULL) {
        printf ("fw_test_mem_init: mem alloc failed for salldsimScBuf\n");
        return -1;
    }

	/* Publish the security context memory offset for other process */
	val.saSecCntxOffset = (&salldsimScBuf[0][0] - fw_mem_start);
	fw_shmSetEntry(setgSecContextOffsetCmd, val);
    return 0;
}

/* Initialize memory for the test framework */
int fw_test_mem_init_local (void)
{
	sa_MCExample_entry_t val;

    /*get the security contex buffer virtual address for that process */
	val = fw_shmGetEntry(getgSecContextOffsetCmd);	
	salldsimScBuf =  (uint8_t (*)[SALLDSIM_SC_BUF_SIZE])(fw_mem_start + val.saSecCntxOffset);	
    return 0;
}

void fw_shmAllocL2Handles(t2Handles_t** t2Hnd)
{
	uint32_t     index;
	sa_Example_PA_ShObj_t* shared_PAObjs = &shared_ObjsBase->paShObj;

    /* wait for the semaphore */
	sem_wait(&mutex);

	SYS_CACHE_INV ((void *) &shared_PAObjs->l2HandleIndex, CACHE_LINESZ, CACHE_WAIT);
	index  = shared_PAObjs->l2HandleIndex;

    if (index < SA_TEST_MAX_L2_HANDLES)
    	*t2Hnd = &shared_PAObjs->l2Handles[index];
    else
    	*t2Hnd = NULL;

    if (*t2Hnd != NULL)
    {
    	shared_PAObjs->l2HandleIndex++;
        SYS_CACHE_WB ((void *) &shared_PAObjs->l2HandleIndex, CACHE_LINESZ, CACHE_WAIT);
    }

    /* Release the semaphore. */
	sem_post(&mutex);
}

void fw_shmAllocL3Handles(t2Handles_t** t3Hnd)
{
	uint32_t     index;
	sa_Example_PA_ShObj_t* shared_PAObjs = &shared_ObjsBase->paShObj;

    /* wait for the semaphore */
	sem_wait(&mutex);


	SYS_CACHE_INV ((void *) &shared_PAObjs->l3HandleIndex, CACHE_LINESZ, CACHE_WAIT);
	index  = shared_PAObjs->l3HandleIndex;

    if (index < SA_TEST_MAX_L3_HANDLES)
    	*t3Hnd = &shared_PAObjs->l3Handles[index];
    else
    	*t3Hnd = NULL;

    if (*t3Hnd != NULL)
    {
    	shared_PAObjs->l3HandleIndex++;
        SYS_CACHE_WB ((void *) &shared_PAObjs->l2HandleIndex, CACHE_LINESZ, CACHE_WAIT);
    }
    /* Release the semaphore. */
	sem_post(&mutex);

}

sa_MCExample_entry_t fw_shmGetEntry(sa_MCExample_shmCmd_e shmCmd)
{
	sa_MCExample_entry_t val;
	sa_Example_PA_ShObj_t* shared_PAObjs = &shared_ObjsBase->paShObj;
	sa_Example_SA_ShObj_t* shared_SAObjs = &shared_ObjsBase->saShObj;
	sa_Example_Comm_ShObj_t* shared_CommObjs = &shared_ObjsBase->comShObj;
	sa_Example_PASA_Bufs_t* shared_PASABufs = &shared_ObjsBase->pasaShBufs;

	switch (shmCmd)
	{
    case getCpdmaHndCmd :
        SYS_CACHE_INV ((void *) &shared_CommObjs->gCpdmaHnd, CACHE_LINESZ, CACHE_WAIT);
        val.cppiHandle = shared_CommObjs->gCpdmaHnd;
        break;
    case 	getSAHndCmd:
        SYS_CACHE_INV ((void *) &shared_SAObjs->salld_handle, CACHE_LINESZ, CACHE_WAIT);
        val.saHandle = shared_SAObjs->salld_handle;
    	break;
    case getSaInstBufAddrCmd:
        SYS_CACHE_INV ((void *) &shared_PASABufs->gSAInst, CACHE_LINESZ, CACHE_WAIT);
        val.saInstBufPtr = shared_PASABufs->gSAInst;
    	break;
    case getSaChInstBufAddrCmd:
        SYS_CACHE_INV ((void *) &shared_PASABufs->gSAChInst, CACHE_LINESZ, CACHE_WAIT);
        val.saChInstBufPtr = shared_PASABufs->gSAChInst;
    	break;
	case getPAHndCmd:
        SYS_CACHE_INV ((void *) &shared_PAObjs->gPAHnd, CACHE_LINESZ, CACHE_WAIT);
        val.paHandle =  shared_PAObjs->gPAHnd;
    	break;			
    case getPaL2HndCmd:
        SYS_CACHE_INV ((void *) &shared_PAObjs->l2Handles, CACHE_LINESZ, CACHE_WAIT);
        val.l2Handle =  shared_PAObjs->l2Handles;
    	break;
    case getPaL3HndCmd:
        SYS_CACHE_INV ((void *) &shared_PAObjs->l3Handles, CACHE_LINESZ, CACHE_WAIT);
        val.l3Handle =  shared_PAObjs->l3Handles;
    	break;
    case getPaInstBufAddrCmd:
        SYS_CACHE_INV ((void *) &shared_PASABufs->gPAInst[0], CACHE_LINESZ, CACHE_WAIT);
        val.paInstBufPtr =  shared_PASABufs->gPAInst;
    	break;
    case getPaInstBufSizeCmd:
    	val.paInstSize = sizeof (shared_PASABufs->gPAInst);
    	break;
    case getMemL2RamBufAddrCmd:
        SYS_CACHE_INV ((void *) &shared_PASABufs->gMemL2Ram[0], CACHE_LINESZ, CACHE_WAIT);
        val.paL2BufPtr = shared_PASABufs->gMemL2Ram;
    	break;
	case getMemL2RamBufSizeCmd:
    	val.paL2MemSize = sizeof (shared_PASABufs->gMemL2Ram);
    	break;	
	case getMemL3RamBufSizeCmd:
    	val.paL3MemSize = sizeof (shared_PASABufs->gMemL3Ram);
    	break;			
    case getMemL3RamBufAddrCmd:
        SYS_CACHE_INV ((void *) &shared_PASABufs->gMemL3Ram[0], CACHE_LINESZ, CACHE_WAIT);
        val.paL3BufPtr =  shared_PASABufs->gMemL3Ram;
    	break;
    case getGlobalCfgDoneCmd:
        SYS_CACHE_INV ((void *) &shared_CommObjs->globalCfgDone, CACHE_LINESZ, CACHE_WAIT);
        val.globalCfgDone = shared_CommObjs->globalCfgDone;
    	break;
    case getLocalCfgDoneCmd:
        SYS_CACHE_INV ((void *) &shared_CommObjs->localCfgDone, CACHE_LINESZ, CACHE_WAIT);
        val.localCfgDone = shared_CommObjs->localCfgDone;
    	break;
    case getgIpInHndCmd:
        SYS_CACHE_INV ((void *) &shared_SAObjs->gIpInHdrHandleOffset, CACHE_LINESZ, CACHE_WAIT);
    	val.ipInHandle = shared_SAObjs->gIpInHdrHandleOffset;
    	break;	
    case getTestCompCntCmd:
        SYS_CACHE_INV ((void *) &shared_CommObjs->testCompCnt, CACHE_LINESZ, CACHE_WAIT);
    	val.testCompCnt = shared_CommObjs->testCompCnt;
    	break;
#ifdef __LINUX_USER_SPACE		
	case getgSecContextOffsetCmd:
        SYS_CACHE_INV ((void *) &shared_SAObjs->gSecContextOffset, CACHE_LINESZ, CACHE_WAIT);
    	val.saSecCntxOffset = shared_SAObjs->gSecContextOffset;
    	break;			
#endif		
    case getstopExecuteCmd:
	   	SYS_CACHE_INV ((void *) &shared_CommObjs->stopExecute, CACHE_LINESZ, CACHE_WAIT);
     	val.stopExecution = shared_CommObjs->stopExecute;
	   	break;
    case getMcCleanupCntCmd:		
	   	SYS_CACHE_INV ((void *) &shared_CommObjs->mcCleanupCnt, CACHE_LINESZ, CACHE_WAIT);
     	val.mcCleanupCnt = shared_CommObjs->mcCleanupCnt;
	   	break;
	case getGFlowId:
	   	SYS_CACHE_INV ((void *) &shared_CommObjs->gFlowId, CACHE_LINESZ, CACHE_WAIT);
     	val.flowId = shared_CommObjs->gFlowId;
	   	break;		
    default:
    	System_printf ("Invalid configuration \n");
    	while (1);
	}

	return (val);
}

/* Note that this function needs to be called by Master Core only */
void fw_shmSetEntry(sa_MCExample_shmCmd_e shmCmd, sa_MCExample_entry_t val)
{
	sa_Example_PA_ShObj_t* shared_PAObjs = &shared_ObjsBase->paShObj;
	sa_Example_SA_ShObj_t* shared_SAObjs = &shared_ObjsBase->saShObj;
	sa_Example_Comm_ShObj_t* shared_CommObjs = &shared_ObjsBase->comShObj;

	switch (shmCmd)
	{
	    case setgCpdmaHndCmd:
	    	shared_CommObjs->gCpdmaHnd =  val.cppiHandle;
	    	SYS_CACHE_WB ((void *) &shared_CommObjs->gCpdmaHnd, CACHE_LINESZ, CACHE_WAIT);
	    	break;
	    case setPAHndCmd:
	    	shared_PAObjs->gPAHnd =  val.paHandle;
	    	SYS_CACHE_WB ((void *) &shared_PAObjs->gPAHnd, CACHE_LINESZ, CACHE_WAIT);
	    	break;
	    case setSAHndCmd:
	    	shared_SAObjs->salld_handle =  val.saHandle;
	    	SYS_CACHE_WB ((void *) &shared_SAObjs->salld_handle, CACHE_LINESZ, CACHE_WAIT);
	    	break;
	    case setGlobalCfgDoneCmd:
	    	shared_CommObjs->globalCfgDone = val.globalCfgDone;
	    	SYS_CACHE_WB ((void *) shared_CommObjs->globalCfgDone, CACHE_LINESZ, CACHE_WAIT);
	    	break;
	    case setLocalCfgDoneCmd:
	    	shared_CommObjs->localCfgDone = val.localCfgDone;
	    	SYS_CACHE_WB ((void *) shared_CommObjs->localCfgDone, CACHE_LINESZ, CACHE_WAIT);
	    	break;			
	    case setgIpInHndCmd:
	    	shared_SAObjs->gIpInHdrHandleOffset = val.ipInHandle;
	    	SYS_CACHE_WB ((void *) shared_SAObjs->gIpInHdrHandleOffset, CACHE_LINESZ, CACHE_WAIT);
	    	break;
	    case setTestCompCntCmd:
	    	shared_CommObjs->testCompCnt = val.testCompCnt;
	    	SYS_CACHE_WB ((void *) &shared_CommObjs->testCompCnt, CACHE_LINESZ, CACHE_WAIT);
	    	break;
#ifdef __LINUX_USER_SPACE			
   	    case setgSecContextOffsetCmd:
     	    val.saSecCntxOffset = shared_SAObjs->gSecContextOffset;
            SYS_CACHE_WB ((void *) &shared_SAObjs->gSecContextOffset, CACHE_LINESZ, CACHE_WAIT);			
    	    break;		
#endif	
        case setstopExecuteCmd:
	    	shared_CommObjs->stopExecute = val.stopExecution;
	    	SYS_CACHE_WB ((void *) &shared_CommObjs->stopExecute, CACHE_LINESZ, CACHE_WAIT);
	    	break;	
        case setGFlowId:
         	shared_CommObjs->gFlowId = val.flowId;
    	   	SYS_CACHE_WB ((void *) &shared_CommObjs->gFlowId, CACHE_LINESZ, CACHE_WAIT);			
	   	break;			
	    default:
	    	System_printf ("Invalid Configuration \n");
	    	while (1);
	}

}

/* shared memory create at Master process */
void fw_shmCreate(void)
{
    struct shmid_ds info;
	char * shm;
	int    i;
	
    shm_id = shmget(SHM_KEY, sizeof (sa_Example_MemBlk_t), IPC_CREAT | 0666 );
    if (shm_id < 0) {
        perror("fw_shmCreate: shmget\n");
		shared_ObjsBase = (sa_Example_MemBlk_t *)NULL;
		exit(-1);
    }
	else {
		shm = shmat(shm_id, 0, 0);
		if (shm == (char *) -1) {
			perror ("fw_shmCreate: shmat\n");
		}
		shared_ObjsBase = (sa_Example_MemBlk_t*) shm; 
		shmctl(shm_id, IPC_STAT, &info);
	    memset(shared_ObjsBase, 0, sizeof (sa_Example_MemBlk_t));

		/* embed the shared memory signature */
		for (i = 0; i < CACHE_LINESZ; i ++)
			shared_ObjsBase->shm_magic_id[i] = i;
	}
}

int32_t fw_shmDelete(void)
{
    shm_id = shmget(SHM_KEY, 0, 0666 );
    if (shm_id < 0)
    {
        perror("fw_shmDelete: shmget error\n");
        exit(-1);
    }
    else
    {
        int err=shmctl(shm_id, IPC_RMID, 0);
        if(err<0)
        {
            perror("fw_shmDelete: shmctl failed\n");
        }
			
    }
}

/* Slave Process would open the previously created Shared memory */
void fw_shmOpen(void)
{
    struct shmid_ds info;
	int i;
    shm_id = shmget(SHM_KEY, 0, 0666 );

	if (shm_id < 0)	{
	   perror ("fw_shmOpen");
	   exit(-1);
	}
	else {
		shmctl(shm_id, IPC_STAT, &info);
	    shared_ObjsBase = (sa_Example_MemBlk_t *) shmat(shm_id, 0, 0);
	}

	/* verify the shared memory attached */
	for (i=0; i<CACHE_LINESZ;i++)
	{ 
	   if (i != shared_ObjsBase->shm_magic_id[i])
	   	 break;
	}

	if (i != CACHE_LINESZ) {
	   System_printf ("Failed to verify the shared memory (core: %d)\n", coreNum);
	   exit(-1);
	}
}


void fw_shmClose(void)
{
	shared_ObjsBase = (sa_Example_MemBlk_t *)NULL;

}

/* Linux Semaphore Initialization */
void fw_SemInit(void)
{
	/* create, initialize semaphore */
	if( sem_init(&mutex,1,1) < 0)
	  {
		perror("sem_init:");
		exit(0);
	  }
}

void fw_SemDestroy(void)
{
	if( sem_destroy(&mutex) < 0)
	  {
		perror("sem_destroy");
		exit(0);
	  }

}




