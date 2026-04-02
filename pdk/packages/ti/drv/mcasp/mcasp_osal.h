#ifndef MCASP_OSAL_H
#define MCASP_OSAL_H

#if defined (USE_BIOS)
#include <xdc/std.h>
#endif /* USE BIOS */

#include <ti/osal/osal.h>

#include <ti/osal/osal.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
typedef void *McaspOsal_IntrHandle;
typedef void (*McaspOsal_IntrFuncPtr)(uint32_t arg0);
#define assert(expr)         OSAL_Assert(!(expr))

#endif

#ifdef __cplusplus
}
#endif
