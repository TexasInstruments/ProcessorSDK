#include "memory_map_defines.inc"

#define __VECS                  MCU_R5F_TCMA_VECS
#define __BOOT                  MCU_R5F_TCMA
#define __CORE_IPC_DATA         MCU1_0_IPC_DATA
#define __CORE_EXT_DATA_BASE    MCU1_0_EXT_DATA_BASE
#define __CORE_EXT_DATA         MCU1_0_EXT_DATA
#define __CORE_R5F_MEM_TEXT     MCU1_0_MEM_TEXT
#define __CORE_R5F_MEM_DATA     MCU1_0_MEM_DATA
#define __CORE_DDR_SPACE        MCU1_0_DDR_SPACE

#include "linker_r5f_safertos_common.inc"