/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2020
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

/* ========================================================================== */
/*                         Include Files                                      */
/* ========================================================================== */
#include <stdint.h>
#include <ti/csl/csl.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/soc.h>


/**
 * @brief
 *  Enumeration for address translation lookup table
 *
 * @details
 *  The enumeration describes the direction of address translation. 
 *
 *  \ingroup CSL_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef enum CSL_TranslateAddr_LUT_Type_e 
{
    CSL_TranslateAddr_LUT_Type_MSS     = 0,         /*!< address as seen by MSS */
    CSL_TranslateAddr_LUT_Type_DSS,                 /*!< address as seen by DSS */
    CSL_TranslateAddr_LUT_Type_LAST    = CSL_TranslateAddr_LUT_Type_DSS,  /*!< address as seen by DSS */
    CSL_TranslateAddr_LUT_Type_INVALID = (CSL_TranslateAddr_LUT_Type_LAST + 1) /*!< invalid - defined for invalid translation */
}CSL_TranslateAddr_LUT_Type;

#define  CSL_TRANSLATEADDR_LUT_TYPE_SIZE   (CSL_TranslateAddr_LUT_Type_LAST + 1)    /*!< address range */

typedef enum CSL_TranslateAddr_MemId_Type_e
{
    CSL_TranslateAddr_MEMID_CR5A_TCMA_RAM,
    CSL_TranslateAddr_MEMID_CR5A_TCMB_RAM,
    CSL_TranslateAddr_MEMID_DSS_L2,
    CSL_TranslateAddr_MEMID_DSS_L1D,
    CSL_TranslateAddr_MEMID_DSS_L1P,
    CSL_TranslateAddr_MEMID_MSS_L2,
    CSL_TranslateAddr_MEMID_MSS_LAST = CSL_TranslateAddr_MEMID_MSS_L2,
}CSL_TranslateAddr_MemId_Type;

#define  CSL_TRANSLATEADDR_MEMID_COUNT    (CSL_TranslateAddr_MEMID_MSS_LAST + 1)

static const uint32_t gMemSegmentSizeTbl[] = 
{
    [CSL_TranslateAddr_MEMID_CR5A_TCMA_RAM] = CSL_MSS_TCMA_CR5A_U_SIZE,
    [CSL_TranslateAddr_MEMID_CR5A_TCMB_RAM] = CSL_MSS_TCMB_CR5A_U_SIZE,
    [CSL_TranslateAddr_MEMID_DSS_L1P] =       CSL_DSS_L1P_U_SIZE,
    [CSL_TranslateAddr_MEMID_DSS_L1D] =       CSL_DSS_L1D_U_SIZE,
    [CSL_TranslateAddr_MEMID_DSS_L2]  =       CSL_DSS_L2_U_SIZE,
    [CSL_TranslateAddr_MEMID_MSS_L2] =        CSL_MSS_L2_U_SIZE,
};

/**
 * @brief
 *  SOC Lookup table for address translation
 *
 * @details
 *  The structure is a lookup table for address translation
 *
 *  \ingroup CSL_DRIVER_INTERNAL_FUNCTION
 */
uint32_t CSL_TranslateAddr_LUT[CSL_TRANSLATEADDR_MEMID_COUNT][CSL_TRANSLATEADDR_LUT_TYPE_SIZE] =
{
    [CSL_TranslateAddr_MEMID_CR5A_TCMA_RAM] = 
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_MSS_TCMA_RAM_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_MSS_TCMA_CR5A_U_BASE,
        },
    [CSL_TranslateAddr_MEMID_CR5A_TCMB_RAM] = 
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_MSS_TCMB_RAM_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_MSS_TCMB_CR5A_U_BASE,
        },
    [CSL_TranslateAddr_MEMID_MSS_L2]  =
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_MSS_L2_RAM_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_GLOB_MSS_L2_RAM_BASE,
        },
    [CSL_TranslateAddr_MEMID_DSS_L1P] =
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_DSS_L1P_U_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_DSP_L1P_U_BASE,
        },
    [CSL_TranslateAddr_MEMID_DSS_L1D] =
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_DSS_L1D_U_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_DSP_L1D_U_BASE,
        },
    [CSL_TranslateAddr_MEMID_DSS_L2]  =
        { 
            [CSL_TranslateAddr_LUT_Type_MSS] = CSL_DSS_L2_U_BASE,
            [CSL_TranslateAddr_LUT_Type_DSS] = CSL_DSP_L2_U_BASE,
        },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
uint64_t CSL_locToGlobAddr(uintptr_t locAddr)
{
    uintptr_t globAddr = locAddr;

#ifdef _TMS320C6X
    if (((locAddr>=CSL_DSP_L1P_U_BASE) && (locAddr<(CSL_DSP_L1P_U_BASE + CSL_DSP_L1P_SIZE))) ||
        ((locAddr>=CSL_DSP_L1D_U_BASE) && (locAddr<(CSL_DSP_L1D_U_BASE + CSL_DSP_L1D_SIZE))) ||
        ((locAddr>=CSL_DSP_L2_U_BASE)  && (locAddr<(CSL_DSP_L2_U_BASE  + CSL_DSP_L2_RAM_SIZE))))
    {
        globAddr = (locAddr | 0x80000000U);
    }
#elif (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
    if (locAddr < (CSL_MSS_TCMA_RAM_BASE + CSL_MSS_TCMA_RAM_SIZE))
    {
        /* TCMA: locAddr | 0xC1000000U) */
        globAddr = (locAddr | CSL_GLOB_MSS_TCMA_RAM_BASE);
    }
    else if ((locAddr >= CSL_MSS_TCMB_RAM_BASE) &&
             (locAddr < (CSL_MSS_TCMB_RAM_BASE + CSL_MSS_TCMB_RAM_SIZE)))
    {
        /* TCMB: (locAddr & 0xFFFF) | 0xC1800000 */
        globAddr = ((locAddr & 0xFFFFU) | CSL_GLOB_MSS_TCMB_RAM_BASE);
    }
    else if ((locAddr >= CSL_MSS_L2_RAM_BASE) &&
             (locAddr < (CSL_MSS_L2_RAM_BASE + CSL_MSS_L2_RAM_SIZE)))
    {
        /* L2: (locAddr & 0xFFFFF) | 0xC0200000 */
        globAddr = ((locAddr & 0xFFFFFU) | CSL_GLOB_MSS_L2_RAM_BASE);
    }
#else
#error "CSL_locToGlobAddr: Unsupported core type for awr294x!"
#endif

    return ((uint64_t) globAddr);
}

uintptr_t CSL_globToLocAddr(uint64_t globAddr64)
{
    uintptr_t globAddr = (uintptr_t) globAddr64;
    uint32_t locAddr = globAddr;

#ifdef _TMS320C6X
    if (((globAddr>=CSL_GLOB_DSP_L1P_U_BASE) && (globAddr<(CSL_GLOB_DSP_L1P_U_BASE + CSL_DSP_L1P_SIZE))) ||
        ((globAddr>=CSL_GLOB_DSP_L1D_U_BASE) && (globAddr<(CSL_GLOB_DSP_L1D_U_BASE + CSL_DSP_L1D_SIZE))) ||
        ((globAddr>=CSL_GLOB_DSP_L2_U_BASE)  && (globAddr<(CSL_GLOB_DSP_L2_U_BASE  + CSL_DSP_L2_RAM_SIZE))))
    {
        locAddr =  (globAddr & ~0x80000000U);
    }
#elif (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
    if ((globAddr >= CSL_GLOB_MSS_TCMA_RAM_BASE) &&
        (globAddr < (CSL_GLOB_MSS_TCMA_RAM_BASE + CSL_MSS_TCMA_RAM_SIZE)))
    {
        /* TCMA: (globAddr & 0xFFFF) | 0x00000000U) */
        locAddr = ((globAddr & 0xFFFFU) | CSL_MSS_TCMA_RAM_BASE);
    }
    else if ((globAddr >= CSL_GLOB_MSS_TCMB_RAM_BASE) &&
             (globAddr < (CSL_GLOB_MSS_TCMB_RAM_BASE + CSL_MSS_TCMB_RAM_SIZE)))
    {
        /* TCMB: (globAddr & 0xFFFF) | 0x00080000U) */
        locAddr = ((globAddr & 0xFFFFU) | CSL_MSS_TCMA_RAM_BASE);
    }
    else if ((globAddr >= CSL_GLOB_MSS_L2_RAM_BASE) &&
             (globAddr < (CSL_GLOB_MSS_L2_RAM_BASE + CSL_MSS_L2_RAM_SIZE)))
    {
        /* L2: (globAddr & 0xFFFFF) | 0x10200000 */
        locAddr = ((globAddr & 0xFFFFFU) | CSL_MSS_L2_RAM_BASE);
    }
#else
#error "CSL_globToLocAddr: Unsupported core type for awr294x!"
#endif

    return (locAddr);
}

/* ESM gating operation related definitions */
/* Four bits are used for each ESM event, creating a mask of 0xF */
#define ESM_GATING_MASK   		(0xFU)

/* 4-bit Shift from one ESM event to the next one */
#define ESM_GATING_SHIFT  		(0x4U)

/* Each ESM_GATING register handles 8 ESM events */
#define ESM_NUM_EVTS_PER_GATING_REG  	(0x8U)

/* 4 ESM_GATING registers for group 2, followed by the registers fo group 3 */
#define ESM_NUM_GATING_REGS_PER_GROUP	(CSL_ESM_GROUP2_3_NUM_EVENTS/ESM_NUM_EVTS_PER_GATING_REG)

#define ESM_GATING_REG_OFFSET(group, event)     ((((group) - CSL_ESM_GROUP_2)*ESM_NUM_GATING_REGS_PER_GROUP) + \
                                                 ((event)/ESM_NUM_EVTS_PER_GATING_REG))*(sizeof(uint32_t))

int32_t CSL_esmEventGating(uint8_t groupNumber, uint8_t eventNumber, uint8_t gating)
{
    int32_t retVal = CSL_EFAIL;

    if (((groupNumber >= CSL_ESM_GROUP_2) && (groupNumber <= CSL_ESM_GROUP_MAX)) &&
        (eventNumber < CSL_ESM_GROUP2_3_NUM_EVENTS))
    {
        uint32_t regVal;
        uint32_t regAddr;
        uint32_t mask;

        #if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
        regAddr = CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_ESM_GATING0;
        #elif defined (_TMS320C6X)
        regAddr = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_ESM_GATING0;
        #else
        #error "awr294x ESM: unsupported core"
        #endif

        regAddr += ESM_GATING_REG_OFFSET(groupNumber, eventNumber);
        regVal = CSL_REG_RD((volatile uint32_t *)regAddr);
        mask = ESM_GATING_MASK << (ESM_GATING_SHIFT*(eventNumber % ESM_NUM_EVTS_PER_GATING_REG));
        regVal &= ~mask;
        if (gating != 0U)
        {
            regVal |= mask;
        }
        CSL_REG_WR((volatile uint32_t *)regAddr, regVal);
        retVal = CSL_PASS;
    }
    return (retVal);
}


#ifdef _TMS320C6X
/**
 *  @b Description
 *  @n
 *      This function returns the address domain for input address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup CSL_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - CSL_TranslateAddr_LUT_Type_INVALID
 */
CSL_TranslateAddr_LUT_Type CSL_getInAddr_LUTType(CSL_TranslateAddr_Dir direction)
{
    CSL_TranslateAddr_LUT_Type inAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case CSL_TranslateAddr_Dir_TO_OTHER_CPU:
            inAddrLUTtype= CSL_TranslateAddr_LUT_Type_DSS;
        break;

        case CSL_TranslateAddr_Dir_FROM_OTHER_CPU:
            inAddrLUTtype= CSL_TranslateAddr_LUT_Type_MSS;
        break;

        default:
            inAddrLUTtype = CSL_TranslateAddr_LUT_Type_INVALID;
        break;
    }

    return inAddrLUTtype;
}

/**
 *  @b Description
 *  @n
 *      This function returns the address domain for output address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup CSL_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - CSL_TranslateAddr_LUT_Type_INVALID
 */
CSL_TranslateAddr_LUT_Type CSL_getOutAddr_LUTType(CSL_TranslateAddr_Dir direction)
{
    CSL_TranslateAddr_LUT_Type outAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case CSL_TranslateAddr_Dir_FROM_OTHER_CPU:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_DSS;
        break;

        case CSL_TranslateAddr_Dir_TO_OTHER_CPU:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_MSS;
        break;

        default:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_INVALID;
        break;

    }

    return outAddrLUTtype;
}
#endif

#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
/**
 *  @b Description
 *  @n
 *      This function returns the address domain for input address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - SOC_TranslateAddr_LUT_Type_INVALID
 */
CSL_TranslateAddr_LUT_Type CSL_getInAddr_LUTType(CSL_TranslateAddr_Dir direction)
{
    CSL_TranslateAddr_LUT_Type inAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case CSL_TranslateAddr_Dir_TO_OTHER_CPU:
            inAddrLUTtype= CSL_TranslateAddr_LUT_Type_MSS;
        break;

        case CSL_TranslateAddr_Dir_FROM_OTHER_CPU:
            inAddrLUTtype= CSL_TranslateAddr_LUT_Type_DSS;
        break;

        default:
            inAddrLUTtype = CSL_TranslateAddr_LUT_Type_INVALID;
        break;
    }

    return inAddrLUTtype;
}

/**
 *  @b Description
 *  @n
 *      This function returns the address domain for output address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - SOC_TranslateAddr_LUT_Type_INVALID
 */
CSL_TranslateAddr_LUT_Type CSL_getOutAddr_LUTType(CSL_TranslateAddr_Dir direction)
{
    CSL_TranslateAddr_LUT_Type outAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case CSL_TranslateAddr_Dir_FROM_OTHER_CPU:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_MSS;
        break;

        case CSL_TranslateAddr_Dir_TO_OTHER_CPU:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_DSS;
        break;

        default:
            outAddrLUTtype = CSL_TranslateAddr_LUT_Type_INVALID;
        break;

    }

    return outAddrLUTtype;
}
#endif

/**
 *  @b Description
 *  @n
 *      This function translates the inAddr from one domain to the other
 *
 *  @param[in] inAddr
 *      input address that needs to be translated
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CSL_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid translated address and errCode is set to 0
 *  @retval
 *      Error       - CSL_TRANSLATEADDR_INVALID and errCode is set to CSL_EINVAL
 */
uint32_t CSL_TranslateAddress(uint32_t inAddr, CSL_TranslateAddr_Dir direction, int32_t* errCode)
{
    int32_t         retCode = CSL_PASS;
    uint32_t        outAddr = inAddr;
    uint32_t        idx = 0;
    CSL_TranslateAddr_LUT_Type inAddrLUTtype,outAddrLUTtype;

    /* get the inAddr and outAddr domain */
    inAddrLUTtype = CSL_getInAddr_LUTType(direction);
    outAddrLUTtype = CSL_getOutAddr_LUTType(direction);

    if ((inAddrLUTtype != CSL_TranslateAddr_LUT_Type_INVALID) &&
        (outAddrLUTtype != CSL_TranslateAddr_LUT_Type_INVALID))
    {
        /* loop around the LUT to translate inAddr to outAddr */
        for (idx=0;idx<CSL_TRANSLATEADDR_MEMID_COUNT;idx++)
        {
            /* get the idx where inAddr falls */
            if ((inAddr >= CSL_TranslateAddr_LUT[idx][inAddrLUTtype]) &&
                (inAddr < (CSL_TranslateAddr_LUT[idx][inAddrLUTtype]+
                            gMemSegmentSizeTbl[idx])))
            {
                /* found the entry; get the offset and add it to the outAddrLUTtype start address */
                outAddr = (inAddr - CSL_TranslateAddr_LUT[idx][inAddrLUTtype])+
                           CSL_TranslateAddr_LUT[idx][outAddrLUTtype];
                break;
            }
        }
    }
    else
    {
        retCode = CSL_EFAIL;
    }

    /* default is error condition, so no need to check the exhaustion of for loop here */
    if (errCode != NULL) {
        *errCode = retCode;
    }
    return outAddr;
}

#define CSL_EFUSE_FIELD_EXTRACT(topCtrlReg, fromNum, row, startBit, endBit)  CSL_extract((topCtrlReg)->EFUSE##fromNum##_ROW_##row, endBit, startBit)

#define CSL_EFUSE_FIELD_EXTRACT_PKG_TYPE(topCtrlReg)                CSL_EFUSE_FIELD_EXTRACT(topCtrlReg, 1, 11, 0,   2)

#define RCM_EFUSE_DEVICE_PKG_TYPE_LOP_VAL                           (0x3) /* 0b011 */
#define RCM_EFUSE_DEVICE_PKG_TYPE_ETS_VAL                           (0x2) /* 0b010 */

#define  RCM_DEVICE_PKG_TYPE_DSP_FREQ_LOP       (450000000U)
#define  RCM_DEVICE_PKG_TYPE_R5_FREQ_LOP        (400000000U)
#define  RCM_DEVICE_PKG_TYPE_SYSCLK_FREQ_LOP    (RCM_DEVICE_PKG_TYPE_R5_FREQ_LOP / 2U)

#define  RCM_DEVICE_PKG_TYPE_DSP_FREQ_ETS       (360000000U)
#define  RCM_DEVICE_PKG_TYPE_R5_FREQ_ETS        (300000000U)
#define  RCM_DEVICE_PKG_TYPE_SYSCLK_FREQ_ETS    (RCM_DEVICE_PKG_TYPE_R5_FREQ_ETS / 2U)

typedef struct CSL_DeviceFreqConfig_s
{
    uint32_t           dspFreqHz;
    uint32_t           r5FreqHz;
    uint32_t           sysClkFreqHz;
} CSL_DeviceFreqConfig;

static uint8_t CSL_extract (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint8_t     value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (uint8_t)((reg >> lsb) & mask);
    return value;
}

static void CSL_SocGetDeviceFreq(CSL_DeviceFreqConfig *devFreq)
{
    CSL_top_ctrlRegs* ptrTopCtrlRegs;
    uint8_t pkgTypeEfuseVal;

    ptrTopCtrlRegs = (CSL_top_ctrlRegs*)CSL_TOP_CTRL_U_BASE;

    pkgTypeEfuseVal = CSL_EFUSE_FIELD_EXTRACT_PKG_TYPE(ptrTopCtrlRegs);
    switch(pkgTypeEfuseVal)
    {
        case RCM_EFUSE_DEVICE_PKG_TYPE_ETS_VAL:
        {
            devFreq->r5FreqHz        = RCM_DEVICE_PKG_TYPE_R5_FREQ_ETS;
            devFreq->dspFreqHz       = RCM_DEVICE_PKG_TYPE_DSP_FREQ_ETS;
            devFreq->sysClkFreqHz    = RCM_DEVICE_PKG_TYPE_SYSCLK_FREQ_ETS;
            break;
        }
        case RCM_EFUSE_DEVICE_PKG_TYPE_LOP_VAL:
        {
            devFreq->r5FreqHz        = RCM_DEVICE_PKG_TYPE_R5_FREQ_LOP;
            devFreq->dspFreqHz       = RCM_DEVICE_PKG_TYPE_DSP_FREQ_LOP;
            devFreq->sysClkFreqHz    = RCM_DEVICE_PKG_TYPE_SYSCLK_FREQ_LOP;
            break;
        }
        default:
        {
            devFreq->r5FreqHz        = RCM_DEVICE_PKG_TYPE_R5_FREQ_LOP;
            devFreq->dspFreqHz       = RCM_DEVICE_PKG_TYPE_DSP_FREQ_LOP;
            devFreq->sysClkFreqHz    = RCM_DEVICE_PKG_TYPE_SYSCLK_FREQ_LOP;
            break;
        }
    }
}

#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
uint32_t CSL_SocGetCpuFreq(void)
{
    CSL_DeviceFreqConfig devFreq;
    CSL_SocGetDeviceFreq(&devFreq);
    return devFreq.r5FreqHz;
}
#endif

#if defined (_TMS320C6X)
uint32_t CSL_SocGetCpuFreq(void)
{
    CSL_DeviceFreqConfig devFreq;
    CSL_SocGetDeviceFreq(&devFreq);
    return devFreq.dspFreqHz;
}
#endif

uint32_t CSL_SocGetSysclkFreq(void)
{
    CSL_DeviceFreqConfig devFreq;
    CSL_SocGetDeviceFreq(&devFreq);
    return devFreq.sysClkFreqHz;
}
