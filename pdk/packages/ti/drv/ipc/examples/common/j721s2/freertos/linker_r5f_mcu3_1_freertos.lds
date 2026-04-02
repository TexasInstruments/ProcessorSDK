#include "memory_map_defines.inc"

#define __VECS                  MCU_R5F_TCMA_VECS
#define __BOOT                  MCU_R5F_TCMA
#define __CORE_IPC_DATA         MCU3_1_IPC_DATA
#define __CORE_EXT_DATA_BASE    MCU3_1_EXT_DATA_BASE
#define __CORE_EXT_DATA         MCU3_1_EXT_DATA
#define __CORE_R5F_MEM_TEXT     MCU3_1_MEM_TEXT
#define __CORE_R5F_MEM_DATA     MCU3_1_MEM_DATA
#define __CORE_DDR_SPACE        MCU3_1_DDR_SPACE

#include "linker_r5f_freertos_common.inc"