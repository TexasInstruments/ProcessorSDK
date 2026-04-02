#include "memory_map_defines.inc"

#define __BOOT                  C66X2_TEXT
#define __CORE_IPC_DATA         C66X2_IPC_D
#define __CORE_EXT_DATA_BASE    C66x2_EXT_DATA_BASE
#define __CORE_EXT_DATA         C66X2_EXT_D
#define __CORE_C66_MEM_TEXT     C66X2_TEXT
#define __CORE_C66_MEM_DATA     C66X2_DATA
#define __CORE_DDR_SPACE        C66X2_DDR_SPACE

#include "linker_c66_freertos_common.inc"