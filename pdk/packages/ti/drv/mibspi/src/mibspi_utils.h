/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

/*!
 *  \file mibspi_utils.h
 *
 *  \brief MIBSPI utility API used internally in mibspi_dma driver.
 */

#ifndef MIBSPI_UTILS_H_
#define MIBSPI_UTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdarg.h>
#include <ti/drv/mibspi/MIBSPI.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(MIBSPI_CFG_ASSERT_ENABLE)
#if defined(__KLOCWORK__)
#define Mibspi_assert(cond)               do { if (!(cond)) abort(); } while (0)
#else
#define Mibspi_assert(cond)                                 \
    (Mibspi_assertLocal((bool)(cond), (const char *)# cond, \
                      (const char *)__FILE__, (int32_t)__LINE__))
#endif /* __KLOCWORK__ */
#else
#define Mibspi_assert(cond) (void)(cond)
#endif

/*! \brief Macro to get container structure from one of its members' addresses */
#define mibspi_container_of(ptr, type, member)               \
    ({                                                       \
         const typeof(((type *)0)->member) * __mptr = (ptr); \
         (type *)((char *)__mptr - offsetof(type, member));  \
     })

/*! \brief Macro to get the size of an array */
#define MIBSPI_UTILS_ARRAYSIZE(x)  (sizeof(x) / sizeof(x[0]))

/*! \brief Get the index of the given element within an array */
#define MIBSPI_UTILS_GETARRAYINDEX(member, array)   (member - &array[0])

/*! \brief Macro to determine if a member is part of an array */
#define MIBSPI_UTILS_ARRAYISMEMBER(member, array)                         \
    (((((uint32)member - (uint32) & array[0]) % sizeof(array[0])) == 0) \
     && (member >= &array[0])                                           \
     && (MIBSPI_UTILS_GETARRAYINDEX(member, array) < MIBSPI_UTILS_ARRAYSIZE(array)))

/*! \brief Compile-time assertion */
#define MIBSPI_UTILS_COMPILETIME_ASSERT(condition)               \
    do {                                                         \
        typedef char ErrorCheck[((condition) == TRUE) ? 1 : -1]; \
        ErrorCheck a = {0};                                      \
        a[0U] = a[0U];                                           \
    } while (0)

/*! \brief Macro to perform round-up division */
#define MIBSPI_UTILS_DIV_ROUNDUP(val, div)  (((val) + (div) - 1) / (div))

/*! \brief Macro to round up a value */
#define MIBSPI_UTILS_ROUNDUP(val, div)      (MIBSPI_UTILS_DIV_ROUNDUP((val), (div)) * (div))

/*! \brief Macro round down a value */
#define MIBSPI_UTILS_ROUNDDOWN(val, div)    (((val) / (div)) * (div))

/*!
 * \brief Generates a pattern of ones between end bit and start bit
 *        M_ONES(7,4) will give 0x000000F0
 */
#define MIBSPI_UTILS_MK_ONES(c_ebit, c_sbit) \
    (((1U << (((c_ebit) - (c_sbit)) + 1U)) - 1U) << (c_sbit))

/*!
 * \brief Generates a pattern of zeros between end bit and start bit
 *        M_MASK(7,4) will give 0xFFFFFF0F
 */
#define MIBSPI_UTILS_MK_MASK(c_ebit, c_sbit) (~(MIBSPI_UTILS_MK_ONES((c_ebit), (c_sbit))))


/* TODO: Generate a warning if extra bit found */
/*!
 * \brief Bounds the value before writing to register M_VAL_BOUND(0x1F, 7, 4)
 *        will remove the extra bit resulting in 0xF
 */
#define MIBSPI_UTILS_VAL_BOUND(val, c_ebit, c_sbit) \
    ((MIBSPI_UTILS_MK_ONES((c_ebit), (c_sbit)) >> (c_sbit)) & (val))

/*! \brief CPSW Ring Acc memory alignment */
#define MIBSPI_UTILS_CACHELINE_SIZE        (128U)

/*! \brief Align a value by performing a round-up operation */
#define MIBSPI_UTILS_ALIGN(x, y)            (((x + (y - 1)) / y) * y)

/*! \brief Macro to determine if an address is aligned to a given size */
#define MIBSPI_UTILS_IS_ALIGNED(addr, alignSize) \
    (((uintptr_t)addr & (alignSize - 1U)) == 0U)

/*! \brief Macro to copy arrays. They must be of the same size */
#define MIBSPI_UTILS_ARRAY_COPY(dst, src)                                  \
    do                                                                   \
    {                                                                    \
        /* dst argument of macro should be array and not pointer.*/      \
        MIBSPI_UTILS_COMPILETIME_ASSERT(sizeof(dst) != sizeof(uintptr_t)); \
        memcpy(dst, src, (MIBSPI_UTILS_ARRAYSIZE(dst) * sizeof(dst[0])));  \
    } while (0)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Prints to shared memory and CCS console
 *
 * This function prints the provided formatted string to shared memory and CCS
 * console.
 *
 * \param format       [IN] Formatted string followed by variable arguments
 */
void MibspiUtils_printf(const char *format,
                      ...);

/*!
 * \brief Logs trace info
 *
 * This function logs the provided formatted string to the provided trace
 * callback function.
 *
 *  \param format       [IN] Formatted string
 *  \param vaArgPtr     [IN] Variadic argument list
 */
void MibspiUtils_trace(const char *format,
                       va_list vaArgPtr);

/*!
 * \brief Initialize utils module
 *
 * Utils module initialization function. Should be only called from the
 * CPSW top level module.
 *
 * \param pUtilsPrms   [IN] Pointer to the initialization parameters
 */
void MibspiUtils_init(void);

/*!
 * \brief De-initialize utils module
 */
void MibspiUtils_deInit(void);

/*!
 * \brief Physical to Virtual Address conversion
 */
void *  MibspiUtils_phyToVirt (uint32_t phyAddr);

/*!
 * \brief Virtual to Physical Address conversion
 */
uint32_t MibspiUtils_virtToPhy (const void *virtAddr);


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

#if defined(MIBSPI_CFG_ASSERT_ENABLE) && !defined(__KLOCWORK__)
static inline void Mibspi_assertLocal(bool condition,
                                    const char *str,
                                    const char *fileName,
                                    int32_t lineNum)
{
    volatile static bool gMibspiAssertWaitInLoop = TRUE;

    if (!(condition))
    {
        MibspiUtils_printf("Assertion @ Line: %d in %s: %s : failed !!!\n",
                         lineNum, fileName, str);
        while (gMibspiAssertWaitInLoop)
        {
        }

        ;
    }

    return;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MIBSPI_UTILS_H_ */
