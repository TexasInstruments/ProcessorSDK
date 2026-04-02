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



/* Common test utilities */
#include "../unittest.h"

/* Request a buffer descriptor with the required buffer size */
Cppi_HostDesc*  testCommonGetBuffer(tFramework_t *tf, Int size)
{

    int32_t QNum;
	Cppi_HostDesc *hd;
    
    
    if (size <= TF_HOST_LINKED_BUF_Q1_BUF_SIZE)
    {
        QNum = tf->QHostLinkedBuf1;
    }
    else if (size <= TF_HOST_LINKED_BUF_Q2_BUF_SIZE)
    {
        QNum = tf->QHostLinkedBuf2;
    }
    else if (size <= TF_HOST_LINKED_BUF_Q3_BUF_SIZE)
    {
        QNum = tf->QHostLinkedBuf3;
    }
    else if (size <= TF_HOST_LINKED_BUF_Q4_BUF_SIZE)
    {
        QNum = tf->QHostLinkedBuf4;
    }
    else
    {
        return (NULL);
    }

	hd =  (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QNum)) & ~15);

    if (hd)
    {
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
        hd->buffPtr = hd->origBuffPtr;
        hd->buffLen = hd->origBufferLen;
        hd->nextBDPtr = (uint32_t)NULL;
    }
    
    return (hd);
}

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/* Request stats from the PA */
int testCommonRequestPaStats (char *fname, tFramework_t *tf, Bool reset, Int32 QSource, Int32 QRecycle,  paCmdReply_t *reply)
{
	Cppi_HostDesc *hd;
	paReturn_t     paret;
	UInt16         cmdSize;
	Int			   cmdDest;
	UInt32		   pktcmd = PASAHO_PACFG_CMD;
	
	/* Pop a descriptor with an associated buffer to use for the command request */
	hd = (Cppi_HostDesc *)(((UInt32)Qmss_queuePop (QSource)) & ~15);
	if (hd == NULL)  {
		SALog ("%s (%s:%d): Failed to pop descriptor from queue %d\n", fname, __FILE__, __LINE__, QSource);
        System_flush ();
		return (-1);
	}
	
	/* Make sure there is a buffer attached and get the size */
	if (hd->buffPtr == (uint32_t)NULL)  {
		SALog ("%s (%s:%d): Descriptor from queue %d had no associated buffer\n", fname, __FILE__, __LINE__, QSource);
		System_flush ();
        return (-1);
	}
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen; 
        hd->buffPtr = hd->origBuffPtr;
    }
	
	paret = Pa_requestStats (tf->passHandle, reset, (paCmd_t)hd->buffPtr, &cmdSize, reply, &cmdDest);

	
	if (paret != pa_OK)  {
		SALog ("%s (%s:%d): Pa_requestStats returned error code %d\n", fname, __FILE__, __LINE__, paret);
		System_flush ();
        Qmss_queuePushDesc (QSource, (Ptr)hd);
		return (-1);
	}
	
	/* Set the packet length and the buffer length. The buffer length MUST NOT
	 * exceed the packet length */
	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);
  	hd->buffLen = cmdSize;
	
	/* Mark the packet as a configuration packet and setup for the descriptor return */
  	Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (UInt8 *)&pktcmd, 4);
  	
	Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
	return (0);

}


/* Compare stats and report any differences */
Int testCommonCompareStats (char *fname, paSysStats_t *expected, paSysStats_t *actual)
{
	Int retval = 0;
	
    if (!nssGblCfgParams.layout.fNssGen2)
    {
	    if (expected->classify1.nPackets != actual->classify1.nPackets)  {
		    SALog ("%s: Stat classify1.nPackets expected %d, found %d\n", fname, expected->classify1.nPackets, actual->classify1.nPackets);
		    retval = 1;
	    }
    }
    
  	if (expected->classify1.nIpv4Packets != actual->classify1.nIpv4Packets)  {
  		SALog ("%s: Stat classify1.nIpv4Packets expected %d, found %d\n", fname, expected->classify1.nIpv4Packets, actual->classify1.nIpv4Packets);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nIpv6Packets != actual->classify1.nIpv6Packets)  {
  		SALog ("%s: Stat classify1.nIpv6Packets expected %d, found %d\n", fname, expected->classify1.nIpv6Packets, actual->classify1.nIpv6Packets);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nCustomPackets != actual->classify1.nCustomPackets)  {
  		SALog ("%s: Stat classify1.nCustomPackets expected %d, found %d\n", fname, expected->classify1.nCustomPackets, actual->classify1.nCustomPackets);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nLlcSnapFail != actual->classify1.nLlcSnapFail)  {
  		SALog ("%s: Stat classify1.nLlcSnapFail expected %d, found %d\n", fname, expected->classify1.nLlcSnapFail, actual->classify1.nLlcSnapFail);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nTableMatch != actual->classify1.nTableMatch)  {
  		SALog ("%s: Stat classify1.nTableMatch expected %d, found %d\n", fname, expected->classify1.nTableMatch, actual->classify1.nTableMatch);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nNoTableMatch != actual->classify1.nNoTableMatch)  {
  		SALog ("%s: Stat classify1.nNoTableMatch expected %d, found %d\n", fname, expected->classify1.nNoTableMatch, actual->classify1.nNoTableMatch);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nIpFrag != actual->classify1.nIpFrag)  {
  		SALog ("%s: Stat classify1.nIpFrag expected %d, found %d\n", fname, expected->classify1.nIpFrag, actual->classify1.nIpFrag);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nIpDepthOverflow != actual->classify1.nIpDepthOverflow)  {
  		SALog ("%s: Stat classify1.nIpDepthOverflow expected %d, found %d\n", fname, expected->classify1.nIpDepthOverflow, actual->classify1.nIpDepthOverflow);
  		retval = 1;
  	}
  	
 	if (expected->classify1.nVlanDepthOverflow != actual->classify1.nVlanDepthOverflow)  {
 		SALog ("%s: Stat classify1.nVlanDepthOverflow expected %d, found %d\n", fname, expected->classify1.nVlanDepthOverflow, actual->classify1.nVlanDepthOverflow);
 		retval = 1;
 	}
 	
  	if (expected->classify1.nGreDepthOverflow != actual->classify1.nGreDepthOverflow)  {
  		SALog ("%s: Stat classify1.nGreDepthOverflow expected %d, found %d\n", fname, expected->classify1.nGreDepthOverflow, actual->classify1.nGreDepthOverflow);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nMplsPackets != actual->classify1.nMplsPackets)  {
  		SALog ("%s: Stat classify1.nMplsPackets expected %d, found %d\n", fname, expected->classify1.nMplsPackets, actual->classify1.nMplsPackets);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nParseFail != actual->classify1.nParseFail)  {
  		SALog ("%s: Stat classify1.nParseFail expected %d, found %d\n", fname, expected->classify1.nParseFail, actual->classify1.nParseFail);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nInvalidIPv6Opt != actual->classify1.nInvalidIPv6Opt)  {
  		SALog ("%s: Stat classify1.nInvalidIPv6Opt expected %d, found %d\n", fname, expected->classify1.nInvalidIPv6Opt, actual->classify1.nInvalidIPv6Opt);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nTxIpFrag != actual->classify1.nTxIpFrag)  {
  		SALog ("%s: Stat classify1.nnTxIpFrag expected %d, found %d\n", fname, expected->classify1.nTxIpFrag, actual->classify1.nTxIpFrag);
  		retval =1 ;
  	}
  	
  	if (expected->classify1.nSilentDiscard != actual->classify1.nSilentDiscard)  {
  		SALog ("%s: Stat classify1.nSilentDiscard expected %d, found %d\n", fname, expected->classify1.nSilentDiscard, actual->classify1.nSilentDiscard);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nInvalidControl != actual->classify1.nInvalidControl)  {
  		SALog ("%s: Stat classify1.nInvalidControl expected %d, found %d\n", fname, expected->classify1.nInvalidControl, actual->classify1.nInvalidControl);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nInvalidState != actual->classify1.nInvalidState)  {
  		SALog ("%s: Stat classify1.nInvalidState expected %d, found %d\n", fname, expected->classify1.nInvalidState, actual->classify1.nInvalidState);
  		retval = 1;
  	}
  	
  	if (expected->classify1.nSystemFail != actual->classify1.nSystemFail)  {
  		SALog ("%s: Stat classify1.nSystemFail expected %d, found %d\n", fname, expected->classify1.nSystemFail, actual->classify1.nSystemFail);
  		retval = 1;
  	}
  	
  	System_flush ();
  	
  	if (expected->classify2.nPackets != actual->classify2.nPackets)  {
  		SALog ("%s: Stat classify2.nPackets expected %d, found %d\n", fname, expected->classify2.nPackets, actual->classify2.nPackets);
  		retval = 1;
  	}
  	
  	if (expected->classify2.nUdp != actual->classify2.nUdp)  {
  		SALog ("%s: Stat classify2.nUdp expected %d, found %d\n", fname, expected->classify2.nUdp, actual->classify2.nUdp);
  		retval = 1;
  	}
  	
  	if (expected->classify2.nTcp != actual->classify2.nTcp)  {
  		SALog ("%s: Stat classify2.nTcp expected %d, found %d\n", fname, expected->classify2.nTcp, actual->classify2.nTcp);
  		retval = 1;
  	}
  	
  	if (expected->classify2.nCustom != actual->classify2.nCustom)  {
  		SALog ("%s: Stat classify2.nCustom expected %d, found %d\n", fname, expected->classify2.nCustom, actual->classify2.nCustom);
  		retval = 1;
  	}
  	
  	if (expected->classify2.nSilentDiscard != actual->classify2.nSilentDiscard)  {
  		SALog ("%s: Stat classify2.nSilentDiscard expected %d, found %d\n", fname, expected->classify2.nSilentDiscard, actual->classify2.nSilentDiscard);
  		retval = 1;
  	}
  	
  	if (expected->classify2.nInvalidControl != actual->classify2.nInvalidControl)  {
  		SALog ("%s: Stat classify2.nInvalidControl expected %d, found %d\n", fname, expected->classify2.nInvalidControl, actual->classify2.nInvalidControl);
  		retval = 1;
  	}
  	
  	if (expected->modify.nCommandFail != actual->modify.nCommandFail)  {
  		SALog ("%s: Stat modify.nCommandFail expected %d, found %d\n", fname, expected->modify.nCommandFail, actual->modify.nCommandFail);
  		retval = 1;
  	}
  	
  	System_flush ();
  	return (retval);
}


Bool testCommonReqAndChkPaStats (tFramework_t *tf, saTest_t *pat, paSysStats_t *pExpectedStats)
{

	paSysStats_t   paStats1;
	paSysStats_t  *paStats = &paStats1;
 	Cppi_HostDesc *hd;
 	paCmdReply_t   reply;
    Int            i; 
 	UInt8		  *bp;
 	UInt32		   blen;
    
    if (!nssGblCfgParams.layout.fNssGen2)
    {
	    /* Request stats from the PA */
	    reply.dest    = pa_DEST_HOST;
	    reply.replyId = 0;
	    reply.queue   = tf->QGen[Q_STATS_REPLY];
	    reply.flowId  = tf->tfFlowNum;
    
	    if (testCommonRequestPaStats (pat->name, tf, TRUE, tf->QHostLinkedBuf1, tf->QGen[Q_REQ_STATS_RECYCLE], &reply))  {
		    SALog ("%s (%s:%d): testCommonRequestPaStats failed\n", pat->name, __FILE__, __LINE__);
 		    System_flush ();
            return FALSE;
	    }
	
	    /* Wait for the stats reply */
	    for (i = 0; i < 200; i++)  {
		    utilCycleDelay (1000);
		    if (Qmss_getQueueEntryCount (tf->QGen[Q_STATS_REPLY]) > 0)
			    break;
	    }
	
	    if (i >= 200)  {
		    SALog ("%s (%s:%d): Did not find response from PA to stats request command\n", pat->name, __FILE__, __LINE__);
 		    System_flush ();
            return FALSE;
	    }
	
	    /* Format the stats response and compare to the expected results */
	    hd = (Cppi_HostDesc *)(((UInt32)Qmss_queuePop (tf->QGen[Q_STATS_REPLY])) & ~15);
	    Cppi_getData (Cppi_DescType_HOST, (Cppi_Desc *)hd, &bp, &blen);
	    paStats = (paSysStats_t *)Pa_formatStatsReply (tf->passHandle, (paCmd_t)bp);
    
    }
    else
    {
	  
	    paReturn_t      paret;
    
	    paret = Pa_querySysStats (tf->passHandle, TRUE, paStats);

	
	    if (paret != pa_OK)  {
		    SALog ("%s (%s:%d): Pa_querySysStats returned error code %d\n", pat->name, __FILE__, __LINE__, paret);
		    return (FALSE);
	    }
    
    }
      
    /* Display Error If any */  
    testCommonCompareStats (pat->name, pExpectedStats, paStats);
         
    if (!nssGblCfgParams.layout.fNssGen2)    
    { 
        /* Recycle the descriptor and associated buffer back to queue from which it came */
	    if (testCommonRecycleLBDesc (tf, hd))  {
		    SALog ("%s (%s:%d): Failed to find original free buffer Q for stats response\n", pat->name, __FILE__, __LINE__);
 		    System_flush ();
            return FALSE;
	    }
    }

    return TRUE;
}

/* Compare long packet information */
Int testCommonComparePktInfo (char *tfName, pasahoLongInfo_t *expected, pasahoLongInfo_t *actual)
{
	Int retval = 0;

#if 0	
	if (PASAHO_LINFO_READ_RECLEN(expected) != PASAHO_LINFO_READ_RECLEN(actual))  {
		SALog ("%s (%s:%d): expected record length = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_RECORDLEN(expected), PASAHO_LINFO_READ_RECORDLEN(actual));
		retval = 1;
	}
#endif
	
	if (PASAHO_LINFO_READ_START_OFFSET(expected) != PASAHO_LINFO_READ_START_OFFSET(actual))  {
		SALog ("%s (%s:%d): expected start offset = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_START_OFFSET(expected), PASAHO_LINFO_READ_START_OFFSET(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_END_OFFSET(expected) != PASAHO_LINFO_READ_END_OFFSET(actual))  {
		SALog ("%s (%s:%d): expected end offset = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_END_OFFSET(expected), PASAHO_LINFO_READ_END_OFFSET(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_EIDX(expected) != PASAHO_LINFO_READ_EIDX(actual))  {
		SALog ("%s (%s:%d): expected error index = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_EIDX(expected), PASAHO_LINFO_READ_EIDX(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_PMATCH(expected) != PASAHO_LINFO_READ_PMATCH(actual))  {
		SALog ("%s (%s:%d): expected pmatch = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_PMATCH(expected), PASAHO_LINFO_READ_PMATCH(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_HDR_BITMASK(expected) != PASAHO_LINFO_READ_HDR_BITMASK(actual))  {
		SALog ("%s (%s:%d): expected header bitmask = 0x%012x, found 0x%012x\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_HDR_BITMASK(expected), PASAHO_LINFO_READ_HDR_BITMASK(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_VLAN_COUNT(expected) != PASAHO_LINFO_READ_VLAN_COUNT(actual))  {
		SALog ("%s (%s:%d): expected vlan count = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_VLAN_COUNT(expected), PASAHO_LINFO_READ_VLAN_COUNT(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_IP_COUNT(expected) != PASAHO_LINFO_READ_IP_COUNT(actual))  {
		SALog ("%s (%s:%d): expected IP count = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_IP_COUNT(expected), PASAHO_LINFO_READ_IP_COUNT(actual));
		retval = 1;
	}
	
	if (PASAHO_LINFO_READ_GRE_COUNT(expected) != PASAHO_LINFO_READ_GRE_COUNT(actual))  {
		SALog ("%s (%s:%d): expected GRE count = %d, found %d\n", tfName, __FILE__, __LINE__, PASAHO_LINFO_READ_GRE_COUNT(expected), PASAHO_LINFO_READ_GRE_COUNT(actual));
		retval = 1;
	}
	
	System_flush ();
	
	return (retval);
}

/* Increment expected stats based on an input bit map */
void testCommonIncStats (paStatsBmap_t *map,  paSysStats_t *stats)	
{
	Int i;
	
	/* There are always three maps in the map list because stats can be incremented up to 3 times */
	for (i = 0; i < 3; i++)  {
		
		if (map[i] & (1 << TF_STATS_BM_C1_NUM_PACKETS))
			stats->classify1.nPackets += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_TABLE_MATCH))
			stats->classify1.nTableMatch += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_NO_TABLE_MATCH))
			stats->classify1.nNoTableMatch += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_VLAN_OVERFLOW))
			stats->classify1.nVlanDepthOverflow += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_NUM_MPLS))
			stats->classify1.nMplsPackets += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_PARSE_FAIL))
			stats->classify1.nParseFail += 1;
			
		if (map[i] & (1 << TF_STATS_BM_C1_SILENT_DISCARD))
			stats->classify1.nSilentDiscard += 1;
	}
}

#endif

/* Reset a buffer descriptor with linked buffer and place on the correct free buffer with 
 * descriptor queue */
Int testCommonRecycleLBDesc (tFramework_t *tf, Cppi_HostDesc *hd)
{
	hd->buffLen = hd->origBufferLen;
	hd->buffPtr = hd->origBuffPtr;
	hd->nextBDPtr = (uint32_t)NULL;
    Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
	
#ifdef NSS_LITE
	if (hd->buffLen == TF_LINKED_BUF_Q1_BUF_SIZE)
		Qmss_queuePushDesc (tf->QLinkedBuf1, (Ptr)hd);
	else if (hd->buffLen == TF_LINKED_BUF_Q2_BUF_SIZE)
		Qmss_queuePushDesc (tf->QLinkedBuf2, (Ptr)hd);
	else if (hd->buffLen == TF_LINKED_BUF_Q3_BUF_SIZE)
		Qmss_queuePushDesc (tf->QLinkedBuf3, (Ptr)hd);
	else if (hd->buffLen == TF_LINKED_BUF_Q4_BUF_SIZE)
		Qmss_queuePushDesc (tf->QLinkedBuf4, (Ptr)hd);
	else
		return (-1);
#else
	if (hd->buffLen == TF_LINKED_BUF_Q1_BUF_SIZE)
		Qmss_queuePush (tf->QLinkedBuf1, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
	else if (hd->buffLen == TF_LINKED_BUF_Q2_BUF_SIZE)
		Qmss_queuePush (tf->QLinkedBuf2, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
	else if (hd->buffLen == TF_LINKED_BUF_Q3_BUF_SIZE)
		Qmss_queuePush (tf->QLinkedBuf3, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
	else if (hd->buffLen == TF_LINKED_BUF_Q4_BUF_SIZE)
		Qmss_queuePush (tf->QLinkedBuf4, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
	else
		return (-1);
#endif
		
	return (0);
}

/* Reset a buffer descriptor with Host linked buffer and place on the correct free buffer with 
 * descriptor queue */
Int testCommonRecycleHostLBDesc (tFramework_t *tf, Cppi_HostDesc *hd)
{
	hd->buffLen = hd->origBufferLen;
	hd->buffPtr = hd->origBuffPtr;
	hd->nextBDPtr = (uint32_t)NULL;
    Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);

#ifdef NSS_LITE	
	if (hd->buffLen == TF_HOST_LINKED_BUF_Q1_BUF_SIZE)
		Qmss_queuePushDesc (tf->QHostLinkedBuf1, (Ptr)hd);
	else if (hd->buffLen == TF_HOST_LINKED_BUF_Q2_BUF_SIZE)
		Qmss_queuePushDesc (tf->QHostLinkedBuf2, (Ptr)hd);
	else if (hd->buffLen == TF_HOST_LINKED_BUF_Q3_BUF_SIZE)
		Qmss_queuePushDesc (tf->QHostLinkedBuf3, (Ptr)hd);
	else if (hd->buffLen == TF_HOST_LINKED_BUF_Q4_BUF_SIZE)
		Qmss_queuePushDesc (tf->QHostLinkedBuf4, (Ptr)hd);
	else
		return (-1);
#else
  if (hd->buffLen == TF_HOST_LINKED_BUF_Q1_BUF_SIZE)
    Qmss_queuePush (tf->QHostLinkedBuf1, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
  else if (hd->buffLen == TF_HOST_LINKED_BUF_Q2_BUF_SIZE)
    Qmss_queuePush (tf->QHostLinkedBuf2, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
  else if (hd->buffLen == TF_HOST_LINKED_BUF_Q3_BUF_SIZE)
    Qmss_queuePush (tf->QHostLinkedBuf3, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
  else if (hd->buffLen == TF_HOST_LINKED_BUF_Q4_BUF_SIZE)
    Qmss_queuePush (tf->QHostLinkedBuf4, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
  else
    return (-1);
#endif
	return (0);
}

#ifndef NSS_LITE

/* Look for command replies from PA */
#define  PA_CMD_RESP_OK      0
#define  PA_CMD_RESP_WAIT   -1
#define  PA_CMD_RESP_FAIL   -2 
					     
static Int PaCmdRep (tFramework_t *tf, saTest_t *pat, paHandleL2L3_t handle)
{
	Cppi_HostDesc  *hd;
	uint32_t         *swInfo0;
	uint32_t 		swInfoCmd;
	uint32_t		lid;
	paReturn_t      paret;
	paEntryHandle_t	reth;
	Int				htype;
	Int				cmdDest;
	
	if (Qmss_getQueueEntryCount ((tf->QGen)[Q_CMD_REPLY]) > 0)  {
		
		hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop ((tf->QGen[Q_CMD_REPLY]))) & ~15);
		if (Cppi_getSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t **) &swInfo0) == CPPI_EPIB_NOT_PRESENT)  {
			SALog ("%s (%s:%d): Found descriptor in PA command reply queue without EIPB present, failing\n", pat->name, __FILE__, __LINE__);
            testCommonRecycleLBDesc (tf, hd);  /* Ignore return code */
			return PA_CMD_RESP_FAIL;
		}
		swInfoCmd = (*swInfo0 & 0xffff0000u); 
		/* Verify expected value in swinfo0 16 msbs */
		if ( (swInfoCmd != TF_CMD_SWINFO0_ADD_ID) && (swInfoCmd != TF_CMD_SWINFO0_DEL_ID) && (swInfoCmd != TF_CMD_SWINFO0_CONFIG_ID)) {
			SALog ("%s (%s:%d): Found descriptor in PA command reply queue without command reply swinfo0\n", pat->name, __FILE__, __LINE__);
			testCommonRecycleLBDesc (tf, hd);  /* Ignore return code */
			return PA_CMD_RESP_FAIL;
		}
		
		/* Extract the local instance value */
		lid = *swInfo0 & 0xffffu;
		if (lid >= TF_NUM_LOCAL_HANDLES)  {
			SALog ("%s (%s:%d): Received PA command reply for out of range local handle %d (max %d)\n", pat->name, __FILE__, __LINE__, lid, TF_NUM_LOCAL_HANDLES);
            testCommonRecycleLBDesc (tf, hd);  /* Ignore return code */
			return PA_CMD_RESP_FAIL;
		}
		
		/* Send the reply back to PA to let the driver know the command has been accepted */
		paret = Pa_forwardResult (tf->passHandle, (void *)hd->buffPtr, &reth, &htype, &cmdDest);
		if (paret != pa_OK)  {
			SALog ("%s (%s:%d): paForwardResult returned error %d in response to add command from PA\n", pat->name, __FILE__, __LINE__);
			testCommonRecycleLBDesc (tf, hd);  /* Ignore return code */
			return PA_CMD_RESP_FAIL;  /* No Return */
		}
		
		/* Make sure the handle returned by PA matches the local copy */
		if ((handle) && (handle != reth.l2l3Handle))  {
			SALog ("%s (%s:%d): paForwardResult returned handle (0x%08x) that did match internal table value (0x%08x)\n", pat->name, __FILE__, __LINE__, (uint32_t)(handle), (uint32_t) reth.l2l3Handle);
			testCommonRecycleLBDesc (tf, hd);  /* Ignore return code */
			return PA_CMD_RESP_FAIL;  /* No Return */
		}
		
		/* Recycle the descriptor and buffer */
		if (testCommonRecycleLBDesc (tf, hd))  {
			SALog ("%s: (%s:%d): testCommonRecycleLBDesc failed to return a buffer/descriptor\n", pat->name, __FILE__, __LINE__);
            return PA_CMD_RESP_FAIL;
		}
        
        return PA_CMD_RESP_OK;
		
	}
    
    return PA_CMD_RESP_WAIT;
}

static Bool testWaitCmdResp(tFramework_t *tf, saTest_t *pat, paHandleL2L3_t handle, char* cmd)
{

    Int ret, i;
    
 	/* Wait for a PA reply */
 	for (i = 0; i < 200; i++)  {
 		utilCycleDelay (1000);
 		ret = PaCmdRep (tf, pat, handle);
        
        if (ret == PA_CMD_RESP_OK)
        {
            return TRUE;
        }    
        else if (ret == PA_CMD_RESP_FAIL)
        {
            return FALSE;
        }    
 	}
 	
 	if (i >= 200)  {
 		SALog ("%s (%s:%d): Reply to %s not found\n", pat->name, __FILE__, __LINE__, cmd);
 		System_flush ();
        //return FALSE;
 	}
    
    return TRUE;
}


/* Provide an add mac configuration to the PA sub-system */
Bool testCommonAddMac (tFramework_t *tf, saTest_t *pat, paEthInfo_t *ethInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t *pl2handle, int32_t QCmdMem,  paCmdReply_t *repInfo)
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
    
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonAddMac failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    }    
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
	
	paret = Pa_addMac (tf->passHandle,
                        pa_LUT1_INDEX_NOT_SPECIFIED,
					    ethInfo,
					    matchRoute,
					    nfailRoute,
					    pl2handle,
					    (paCmd_t) hd->buffPtr,
					    &cmdSize,
					    repInfo,
					    &cmdDest);
			
    /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
		Qmss_queuePushDesc (QCmdMem, (Ptr)hd);
 		SALog ("%s (%s:%d): testCommonAddMac failed, PA LLD error code = %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
        return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);
    
    //mdebugHaltPdsp (0);
 	/* Send the command to PA. This will result in 1 packet in classify1 */
#ifndef NSS_LITE
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
 	Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
#endif    
    //salld_sim_halt();
    
    return(testWaitCmdResp(tf, pat, *pl2handle, "addMac"));
}

/* Provide an add mac configuration to the PA sub-system */
Bool testCommonAddIp2 (tFramework_t *tf, saTest_t *pat, int lutInst, paIpInfo_t *ipInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t l2handle, paHandleL2L3_t *pl3handle, int32_t QCmdMem, paCmdReply_t *repInfo)
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonAddIp failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    } 
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
    
	paret = Pa_addIp  (tf->passHandle,
                        lutInst,
                        pa_LUT1_INDEX_NOT_SPECIFIED,
					    ipInfo,
                        l2handle,
					    matchRoute,
					    nfailRoute,
					    pl3handle,
					    (paCmd_t) hd->buffPtr,
					    &cmdSize,
					    repInfo,
					    &cmdDest);
			
    /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
#ifndef NSS_LITE
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
		Qmss_queuePushDesc (QCmdMem, (Ptr)hd);
#endif
 		SALog ("%s (%s:%d): testCommonAddIp2 failed, PA LLD error code = %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
        return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif
    
  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);

#ifndef NSS_LITE
    Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
 	  /* Send the command to PA. This will result in 1 packet in classify1 */
 	  Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
#endif    
    return(testWaitCmdResp(tf, pat, *pl3handle, "addIp"));
  	
}

Bool testCommonAddIp (tFramework_t *tf, saTest_t *pat, paIpInfo_t *ipInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                  paHandleL2L3_t l2handle, paHandleL2L3_t *pl3handle, int32_t QCmdMem, paCmdReply_t *repInfo)
{
    return(testCommonAddIp2(tf, pat, pa_LUT_INST_NOT_SPECIFIED, ipInfo, matchRoute, nfailRoute, l2handle, pl3handle, QCmdMem, repInfo));
}

/* Provide an add mac configuration to the PA sub-system */
Bool testCommonAddUdp (tFramework_t *tf, saTest_t *pat, uint16_t port, Bool replace, paRouteInfo_t *routeInfo,  
 	                   paHandleL2L3_t l3handle, paHandleL4_t l4handle, int32_t QCmdMem, paCmdReply_t *repInfo)
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonAddUdp failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    }    
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
		
	paret = Pa_addPort (tf->passHandle,
                        pa_LUT2_PORT_SIZE_16,
					    port,
                        l3handle,
                        replace,
                        pa_PARAMS_NOT_SPECIFIED,
					    routeInfo,
					    l4handle,
					    (paCmd_t) hd->buffPtr,
					    &cmdSize,
					    repInfo,
					    &cmdDest);
			
    /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
#ifndef NSS_LITE
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
		Qmss_queuePushDesc (QCmdMem, (Ptr)hd);
#endif
 		SALog ("%s (%s:%d): testCommonAddUdp failed, PA LLD error code = %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
        return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);

#ifndef NSS_LITE
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
 	/* Send the command to PA. This will result in 1 packet in classify1 */
 	Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
#endif    
    return(testWaitCmdResp(tf, pat, 0, "addUdp"));
}

/* Provide a Global configuration to the PA sub-system */
Bool testCommonGlobalConfig (tFramework_t *tf, saTest_t *pat, paCtrlInfo_t *cfgInfo, int32_t QCmdMem, paCmdReply_t *repInfo)
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonGlobalConfig failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    } 
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
    
	paret = Pa_control (tf->passHandle,
					    cfgInfo,
					    (paCmd_t) hd->buffPtr,
					    &cmdSize,
					    repInfo,
					    &cmdDest);
			
    /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
#ifndef NSS_LITE
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
		Qmss_queuePushDesc (QCmdMem, (Ptr)hd);
#endif
		SALog ("%s (%s:%d): Pa_control returned error code %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
		return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);
#ifndef NSS_LITE
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
 	/* Send the command to PA. This will result in 1 packet in classify1 */
 	Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
#endif    
    return(testWaitCmdResp(tf, pat, NULL, "GlobalConfig"));
}

/* Provide an Exception Route configuration to the PA sub-system */
Bool testCommonConfigExceptionRoute (tFramework_t *tf, saTest_t *pat, int nRoute, int *routeTypes, paRouteInfo_t *eRoutes, 
 	                       		     int32_t QCmdMem, paCmdReply_t *repInfo)
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonConfigExceptionRoute failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    } 
    else
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
	
	paret =  Pa_configExceptionRoute (tf->passHandle,
					                  nRoute,
                                      routeTypes,
					                  eRoutes,
					                  (paCmd_t) hd->buffPtr,
					                  &cmdSize,
					                  repInfo,
					                  &cmdDest);
			
    /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
#ifndef NSS_LITE
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
		Qmss_queuePushDesc (QCmdMem, (Ptr)hd);
#endif
		SALog ("%s (%s:%d): Pa_configExceptionRoute returned error code %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
		return (FALSE);
	}
    
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);

#ifndef NSS_LITE
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
 	/* Send the command to PA. This will result in 1 packet in classify1 */
 	Qmss_queuePushDescSize (tf->QPaTx[cmdDest], (Ptr)hd, TF_SIZE_DESC);
#endif    
    return(testWaitCmdResp(tf, pat, NULL, "ExceptionRoutes"));
}

#endif

void testCommonSendNullPkt(tFramework_t *tf, uint32_t *swInfo)
{
	Cppi_HostDesc *hd;
 	Qmss_Queue      q;
     

	/* Send null packet to SA, it should be discarded by SA */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tf->QfreeDesc)) & ~15);
	if (hd == NULL)  {
		SALog ("testCommonSendNullPkt (%s:%d): Queue pop failed for free descriptor queue (%d)\n",  __FILE__, __LINE__, tf->QfreeDesc);
 		System_flush ();
        return;
	}
		
	/* Setup the return for the descriptor */
  	q.qMgr = 0;
  	q.qNum = tf->QfreeDesc;
  	Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
		
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, 0);
    Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
        
  	Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)swInfo);
#ifndef NSS_LITE
  	Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), 0, TF_SIZE_DESC, Qmss_Location_TAIL);
#else
  	Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#endif

}

#ifndef NSS_LITE

Bool testCommonDelHandle (tFramework_t *tf, saTest_t *pat, paHandleL2L3_t *paHdl, Int QCmdMem, paCmdReply_t *cmdReply) 
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonDelHandle failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
		System_flush ();
        return (FALSE);
    }
    else    
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
		
	paret = Pa_delHandle (tf->passHandle, paHdl, (paCmd_t)hd->buffPtr, &cmdSize, cmdReply, &cmdDest);
	
	 /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
 		SALog ("%s (%s:%d): testCommonDelHandle failed, PA LLD error code = %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
        return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);
    
 	/* Send the command to PA. This will result in 1 packet in classify1 */
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
    
    return(testWaitCmdResp(tf, pat, 0, "delHandle"));
}

Bool testCommonDelL4Handle (tFramework_t *tf, saTest_t *pat, paHandleL4_t paHdl, Int QCmdMem, paCmdReply_t *cmdReply) 
{
	Cppi_HostDesc *hd;
	uint32_t	   psCmd;
    int            cmdDest; 
    uint16_t       cmdSize; 
    paReturn_t     paret; 
	
	/* Pop a descriptor with a linked buffer to create the command */
	hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (QCmdMem)) & ~15);
	if (hd == NULL)
    {
 		SALog ("%s (%s:%d): testCommonDelL4Handle failed due to unavailable free linked buffer descriptor\n", pat->name, __FILE__, __LINE__);
        System_flush ();
        return (FALSE);
    }    
    else    
    {
        cmdSize = hd->buffLen = hd->origBufferLen;
        hd->buffPtr = hd->origBuffPtr;
        Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    }   
	
	paret = Pa_delL4Handle (tf->passHandle, paHdl, (paCmd_t)hd->buffPtr, &cmdSize, cmdReply, &cmdDest);
	
	 /* Restore the descriptor and return it on PA failure */
	if (paret != pa_OK)  {
		hd->buffLen = hd->origBufferLen;
		Qmss_queuePush (QCmdMem, (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
 		SALog ("%s (%s:%d): testCommonDelL4Handle failed, PA LLD error code = %d\n", pat->name, __FILE__, __LINE__, paret);
		System_flush ();
        return (FALSE);
	}
	
#ifdef PA_SIM_BUG_4BYTES
  	cmdSize = (cmdSize+3)& ~3;
#endif

  	/* Mark the packet as a configuration packet */
  	psCmd = PASAHO_PACFG_CMD;
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)&psCmd, 4);
  	
    hd->buffLen = cmdSize;
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, cmdSize);
    
 	/* Send the command to PA. This will result in 1 packet in classify1 */
 	Qmss_queuePush (tf->QPaTx[cmdDest], (Ptr)hd, cmdSize, TF_SIZE_DESC, Qmss_Location_TAIL);
    
    return(testWaitCmdResp(tf, pat, 0, "delL4Handle"));
}


Bool testCommonDelL2L3Connection (tFramework_t *tf, saTest_t *pat,  Int numHandles, t2Handles_t *l2Handles, int *numConnet)
{
	Int	i;
	
	paCmdReply_t cmdReply = {  pa_DEST_HOST,			/* Dest */
 							   0,						/* Reply ID (returned in swinfo0) */
 							   0,						/* Queue */
 							   0 };						/* Flow ID */
 							   
	cmdReply.queue   = (uint16_t) tf->QGen[Q_CMD_REPLY];
    
    *numConnet = 0;

	
	/* Delete all the handles */
	for (i = numHandles - 1; i >= 0; i--)  {
		
        if (l2Handles[i].state == TF_L2_HANDLE_ACTIVE)
        {
            cmdReply.replyId = TF_CMD_SWINFO0_DEL_ID + i;
            if (!testCommonDelHandle(tf, pat, &l2Handles[i].paHandle, tf->QHostLinkedBuf1, &cmdReply))
            {
                return (FALSE);
            }
            
            l2Handles[i].state = TF_L2_HANDLE_DISABLED;
            *numConnet += 1;
        }
        else
        {
            continue;
        }
	}
    
    return TRUE;
}

Bool testCommonDelL4Connection (tFramework_t *tf, saTest_t *pat, Int numHandles, t4Handles_t *l4Handles, int *numConnet)
{
	Int i;
	
	paCmdReply_t cmdReply = {  pa_DEST_HOST,			/* Dest */
 							   0,						/* Reply ID (returned in swinfo0) */
 							   0,						/* Queue */
 							   0 };						/* Flow ID */
 							   
	cmdReply.queue   = (uint16_t) tf->QGen[Q_CMD_REPLY];
    
    *numConnet = 0;
    

	
	/* Delete all the handles */
	for (i = 0; i < numHandles; i++)  {
		
        if (l4Handles[i].state == TF_L2_HANDLE_ACTIVE)
        {
            cmdReply.replyId = TF_CMD_SWINFO0_DEL_ID + i;
            if (!testCommonDelL4Handle(tf, pat, l4Handles[i].paHandle, tf->QHostLinkedBuf1, &cmdReply))
            {
                return (FALSE);
            }
            
            l4Handles[i].state = TF_L2_HANDLE_DISABLED;
            *numConnet += 1;
            
        }
        else
        {
            continue;
        }
	}
    
    return TRUE;
}

#endif

/* Search the receive data packet queue for received data packets. Remain in 
 * this function until all buffers are restored to their respective queues */
void testDispPkts (Cppi_HostDesc *hd)
{
	uint32_t  *swInfo;
    uint32_t  *psInfo;
	uint32_t  infoLen;
    uint8_t   *bufPtr;
    uint32_t  bufLen;
    uint8_t   errFlags;
	int index = 0;
   
   /* Dispaly Software Info */
   Cppi_getSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t **)&swInfo);
   
   salld_sim_print("Software Info: 0x%08x, 0x%08x, 0x%08x\n", swInfo[0], swInfo[1], swInfo[2]);
   
   /* Display PS Info if exists */
   Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&psInfo, &infoLen);
   
   #if !SA_GEN_TEST_VECTOR
   
   while (infoLen >= 4)
   {
    salld_sim_print("PS Info[%d] = 0x%08x\n", index, psInfo[index]);
    index++;
    infoLen -= 4;
   }
   
   if (infoLen)
   {
    salld_sim_print("PS Info[%d] (remaing %d bytes)= 0x%08x\n", index, infoLen, psInfo[index]);
   }
   
   #endif
   
			
   if ((errFlags = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd)))
   {
    salld_sim_print("Packet Error with error code %d\n", errFlags);
   }  
   
   Cppi_getData(Cppi_DescType_HOST, (Cppi_Desc *)hd, &bufPtr, &bufLen);
   
   salld_sim_print("Packet of size %d received at 0x%04x\n", bufLen, bufPtr);
   
   #if SA_GEN_TEST_VECTOR
   utilOutputPkt(&tFramework, swInfo, infoLen, psInfo,
                  bufLen, bufPtr, FALSE);
   #endif

}

#ifndef NSS_LITE

/* Function used for debugging firmware */
#include <ti/csl/cslr_pa_ss.h>
void mdebugHaltPdsp (Int pdspNum)
{
    //CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_NETCP_CFG_REGS;
	//passRegs->PDSP_CTLSTAT[pdspNum].PDSP_CONTROL &= ~(CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK);

}

#endif

  	
		
	

