#include "memory_map_defines.inc"

#define __BOOT                  C66X1_TEXT
#define __CORE_IPC_DATA         C66X1_IPC_D
#define __CORE_EXT_DATA_BASE    C66x1_EXT_DATA_BASE
#define __CORE_EXT_DATA         C66X1_EXT_D
#define __CORE_C66_MEM_TEXT     C66X1_TEXT
#define __CORE_C66_MEM_DATA     C66X1_DATA
#define __CORE_DDR_SPACE        C66X1_DDR_SPACE

#include "linker_c66_freertos_common.inc"