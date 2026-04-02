#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")

#include "sfm_ti_alg_int.h"
#include "sfm_ti_device_utils.h"

#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define USE_STACK_MEM_TR   //TODO: Temporary workaround, keeping due to need for urgent release, ideally need to remove it and check if results in any issue
#endif

int32_t TIADALG_initDmaUtils(void*  algHandle)
{
  int32_t status = IALG_EOK;
  int32_t i;
  SFM_TI_Handle  intAlgHandle = (SFM_TI_Handle)algHandle;
  /* DMA context allocation */
  {
    int32_t     retVal = UDMA_SOK;
    int32_t dmaContextSize;
    DmaUtilsAutoInc3d_InitParam dmaUtilsInitParams;
    DmaUtilsAutoInc3d_ChannelInitParam chInitParams[TIADALG_DMA_CHANNEL_MAX];

    dmaContextSize = DmaUtilsAutoInc3d_getContextSize((uint32_t)TIADALG_DMA_CHANNEL_MAX);

    dmaUtilsInitParams.contextSize     = (uint32_t)dmaContextSize;
    dmaUtilsInitParams.DmaUtilsVprintf = NULL;
    dmaUtilsInitParams.numChannels     = TIADALG_DMA_CHANNEL_MAX;
    dmaUtilsInitParams.traceLogLevel   = 0;
    dmaUtilsInitParams.udmaDrvHandle   = (Udma_DrvHandle)intAlgHandle->createParams.udmaDrvObj;

    for ( i = 0; i < TIADALG_DMA_CHANNEL_MAX; i++)
    {
      //:TODO: Need to do queue balancing
      chInitParams[i].dmaQueNo = 0;
      if ( i <= TIADALG_DMA_CHANNEL_MEMCPY )
      {
        chInitParams[i].druOwner = (uint8_t)DMAUTILSAUTOINC3D_DRUOWNER_DIRECT_TR;
      }
      else
      {
        chInitParams[i].druOwner = (uint8_t)DMAUTILSAUTOINC3D_DRUOWNER_UDMA;
      }
    }

    retVal = DmaUtilsAutoInc3d_init( intAlgHandle->dmaUtilsContext, &dmaUtilsInitParams, chInitParams);
    if ( retVal != DMAUTILS_SOK )
    {
      status = IALG_EFAIL;
    }
  }

#ifndef HOST_EMULATION
   volatile uint64_t* queue0CFG = (uint64_t*)(0x6D008000);
   uint32_t gQoS_DRU_Prirority = 7;
   uint32_t gQoS_DRU_OrderID   = 1;
   uint64_t queue0CFG_VAL = 0x0;
   queue0CFG_VAL |= ((uint64_t)gQoS_DRU_OrderID)<<4;
   queue0CFG_VAL |= ((uint64_t)gQoS_DRU_Prirority);
   *queue0CFG = queue0CFG_VAL;// <--- UC VAL //0x0000000020080000;
#endif

  if(status == IALG_EOK)
  {
    TIADALG_prePareDefault3DTr(intAlgHandle->memcpyTr, TIADALG_DMA_CHANNEL_MEMCPY);
    TIADALG_prePareDefault3DTr(intAlgHandle->pingPongDofTr, TIADALG_DMA_CHANNEL_PING_PONG);
    TIADALG_prePareDefault3DTr(intAlgHandle->pingPongCbCrTr, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);
  }
  return status;
}

/**
 * @brief Function is to prepare transfer prooperties
 *
 * @param trMem  : pointer to trasfer memory
 */
void  TIADALG_prePareDefault3DTr(void * trMem, int32_t channelNo)
{
  DmaUtilsAutoInc3d_TrPrepareParam trPrepParams;
  DmaUtilsAutoInc3d_TransferProp      transferProp;
  trPrepParams.channelId = channelNo;
  trPrepParams.numTRs    = 1U;
  trPrepParams.trMem          = (uint8_t*)trMem;
  trPrepParams.trMemSize  = 64U;

  transferProp.dmaDfmt = DMAUTILSAUTOINC3D_DFMT_NONE;

  transferProp.syncType = (uint32_t)DMAUTILSAUTOINC3D_SYNC_2D;

  transferProp.circProp.circDir = (uint8_t)DMAUTILSAUTOINC3D_CIRCDIR_DST;
  transferProp.circProp.circSize1 = 0;
  transferProp.circProp.circSize2 = 0;
  transferProp.circProp.addrModeIcnt0 = (uint8_t)DMAUTILSAUTOINC3D_ADDR_LINEAR;
  transferProp.circProp.addrModeIcnt1 = (uint8_t)DMAUTILSAUTOINC3D_ADDR_LINEAR;
  transferProp.circProp.addrModeIcnt2 = (uint8_t)DMAUTILSAUTOINC3D_ADDR_LINEAR;
  transferProp.circProp.addrModeIcnt3 = (uint8_t)DMAUTILSAUTOINC3D_ADDR_LINEAR;

  transferProp.transferDim.sicnt0 = 1;
  transferProp.transferDim.sicnt1 =1;
  transferProp.transferDim.sicnt2 =1;
  transferProp.transferDim.sicnt3 =1;
  transferProp.transferDim.sdim1= 1;

  transferProp.transferDim.dicnt0 = 1;
  transferProp.transferDim.dicnt1 = 1;
  transferProp.transferDim.dicnt2 =1;
  transferProp.transferDim.dicnt3 =1;
  transferProp.transferDim.ddim1 = 1;

  transferProp.ioPointers.srcPtr = (uint8_t *)NULL;
  transferProp.ioPointers.dstPtr = (uint8_t *)NULL;

  (void)DmaUtilsAutoInc3d_prepareTr( &trPrepParams,&transferProp);
}

/**
 * @brief Memory copy for trigger
 *
 * @param dstPtr : Pointer to dst memory
 * @param srcPtr : Pointer to src memory
 * @param width : Width of the memory copy
 * @param height : Height of the memory copy
 * @param dstStride : Stride of the dst buffer
 * @param srcStride : Stride of the src buffer
 * @param chNo : Channel Number
 * @param dmaUtilsContext : memory for dmaUtilsContext
 * @param trMem : pointer to transfer memory
 */
void  TIADALG_memcpy2D
(
  const void   *dstPtr,
  const void   *srcPtr,
  uint16_t     width,
  uint16_t     height,
  int32_t      dstStride,
  int32_t      srcStride,
  int32_t      chNo,
  void * dmaUtilsContext,
  uint8_t * trMem,
  int32_t immediateWait // 1 --> means wait immediate, 0 --> wait sometime later
  )
{
  if(dmaUtilsContext != NULL)
  {
    DmaUtilsAutoInc3d_TrPrepareParam trPrepParam;
#ifdef USE_STACK_MEM_TR
    uint8_t    trMemStack[64];
    TIADALG_prePareDefault3DTr(trMemStack, chNo);
    trMem     = trMemStack;
#endif
    uint32_t convertMask = DMAUTILSAUTOINC3D_ADDRCONVERTMASK_SRCADDR |
                            DMAUTILSAUTOINC3D_ADDRCONVERTMASK_DSTADDR;
    trPrepParam.channelId = chNo;
    trPrepParam.numTRs    = 1;
    trPrepParam.trMemSize = 64;
    trPrepParam.trMem     = trMem;

    CSL_UdmapTR * tr;
    tr = (CSL_UdmapTR *) trMem;
    tr->addr   = (uintptr_t)srcPtr;
    tr->daddr  = (uintptr_t)dstPtr;
    tr->icnt0  = width;
    tr->icnt1  = height;
    tr->dicnt0 = width;
    tr->dicnt1 = height;
    tr->dim1   = srcStride;
    tr->ddim1  = dstStride;

    DmaUtilsAutoInc3d_convertTrVirtToPhyAddr(dmaUtilsContext, &trPrepParam, convertMask);

    /* Re-use the same contexxt as feat for memcpy */
    (void)DmaUtilsAutoInc3d_configure(dmaUtilsContext, chNo, trMem, 1U);

    (void)DmaUtilsAutoInc3d_trigger(dmaUtilsContext, chNo);

    if(immediateWait)
    {
      DmaUtilsAutoInc3d_wait(dmaUtilsContext, chNo);
    }

  }
  else
  {
    uint32_t rowCnt;

    for (rowCnt = 0; rowCnt < height; rowCnt++)
    {
      memcpy((uint8_t *)dstPtr + rowCnt * dstStride, (uint8_t *)srcPtr + rowCnt * srcStride, width);
    }
  }
}

int32_t TIADALG_updatePingPong3DTr(
  uint8_t* src, uint8_t* dst, uint8_t* inTrMem,
  int32_t width, int32_t height, int32_t inPitch,
  int32_t blockWidth, int32_t blockHeight, int32_t blockPitch,
  int32_t pongOffset,int32_t isSrcPingPong, int32_t elmSize,
  int32_t chNo, void* dmaUtilsContext)
{
  DmaUtilsAutoInc3d_TrPrepareParam trPrepParam;

  uint32_t convertMask = DMAUTILSAUTOINC3D_ADDRCONVERTMASK_SRCADDR |
                          DMAUTILSAUTOINC3D_ADDRCONVERTMASK_DSTADDR;

  trPrepParam.channelId = chNo;
  trPrepParam.numTRs    = 1;
  trPrepParam.trMemSize = 64;
  trPrepParam.trMem     = inTrMem;

  CSL_UdmapTR * tr;
  tr = (CSL_UdmapTR *) inTrMem;

  int32_t numTilesW = (width + blockWidth - 1)/blockWidth;
  int32_t numTilesH = (height + blockHeight - 1)/blockHeight;

  int32_t pingPongNumBlks = ((numTilesW * numTilesH) + 1)>>1;

  if(isSrcPingPong == 0)
  {
    tr->icnt0     = blockWidth*elmSize;
    tr->icnt1     = blockHeight;
    tr->icnt2     = numTilesW;
    tr->icnt3     = numTilesH;
    tr->dim1      = inPitch * elmSize;
    tr->dim2      = blockWidth * elmSize;
    tr->dim3      = blockHeight * inPitch * elmSize;

    tr->dicnt0     = blockWidth * elmSize;
    tr->dicnt1     = blockHeight;
    tr->dicnt2     = 2; // ping pong
    tr->dicnt3     = pingPongNumBlks;
    tr->ddim1      = blockPitch * elmSize;
    tr->ddim2      = pongOffset * elmSize; // go to pong
    tr->ddim3      = 0; // go back to ping
  }
  else
  {
    tr->dicnt0     = blockWidth*elmSize;
    tr->dicnt1     = blockHeight;
    tr->dicnt2     = numTilesW;
    tr->dicnt3     = numTilesH;
    tr->ddim1      = inPitch * elmSize;
    tr->ddim2      = blockWidth * elmSize;
    tr->ddim3      = blockHeight * inPitch * elmSize;

    tr->icnt0     = blockWidth * elmSize;
    tr->icnt1     = blockHeight;
    tr->icnt2     = 2; // ping pong
    tr->icnt3     = pingPongNumBlks;
    tr->dim1      = blockPitch * elmSize;
    tr->dim2      = pongOffset * elmSize; // go to pong
    tr->dim3      = 0; // go back to ping
  }


  tr->addr       = (size_t)src;
  tr->daddr      = (size_t)dst;

  DmaUtilsAutoInc3d_convertTrVirtToPhyAddr(dmaUtilsContext, &trPrepParam, convertMask);

  return 1;
}
