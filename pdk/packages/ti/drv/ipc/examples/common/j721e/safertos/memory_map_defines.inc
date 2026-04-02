
#define DDR0_RESERVED_START     0x80000000
#define DDR0_RESERVED_SIZE      0x20000000 /* 512MB */

#define DDR0_ALLOCATED_START    0xA0000000 /* DDR0_RESERVED_START + DDR0_RESERVED_SIZE */

#define SHARED_DDR_SPACE_START  0xAA000000
#define SHARED_DDR_SPACE_SIZE   0x01C00000  /*  28MB */

/*------------------------------------------------*/
/* Size of various Memory Locations for each core */ 
/*------------------------------------------------*/
#define IPC_DATA_SIZE   0x00100000 /*  1MB */
#define EXT_DATA_SIZE   0x00100000 /*  1MB */
#define MEM_TEXT_SIZE   0x00100000 /*  1MB */
#define MEM_DATA_SIZE   0x00100000 /*  1MB */
#define DDR_SPACE_SIZE  0x00C00000 /* 12MB */

#define CORE_TOTAL_SIZE 0x01000000 /* 16MB (IPC_DATA_SIZE + R5F_MEM_TEXT_SIZE + R5F_MEM_DATA_SIZE + DDR_SPACE_SIZE) */

/*-----------------------------*/
/* Start address for each core */ 
/*-----------------------------*/

#define MCU1_0_ALLOCATED_START   DDR0_ALLOCATED_START
#define MCU1_1_ALLOCATED_START   MCU1_0_ALLOCATED_START + CORE_TOTAL_SIZE
#define MCU2_0_ALLOCATED_START   MCU1_1_ALLOCATED_START + CORE_TOTAL_SIZE
#define MCU2_1_ALLOCATED_START   MCU2_0_ALLOCATED_START + CORE_TOTAL_SIZE
#define MCU3_0_ALLOCATED_START   MCU2_1_ALLOCATED_START + CORE_TOTAL_SIZE
#define MCU3_1_ALLOCATED_START   MCU3_0_ALLOCATED_START + CORE_TOTAL_SIZE
#define C66x1_ALLOCATED_START    MCU3_1_ALLOCATED_START + CORE_TOTAL_SIZE
#define C66x2_ALLOCATED_START    C66x1_ALLOCATED_START  + CORE_TOTAL_SIZE
#define C7x_1_ALLOCATED_START    C66x2_ALLOCATED_START  + CORE_TOTAL_SIZE

/*--------------------------- MCU R5FSS0 CORE0 --------------------------*/
#define MCU1_0_IPC_DATA_BASE     MCU1_0_ALLOCATED_START
#define MCU1_0_EXT_DATA_BASE     MCU1_0_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU1_0_MEM_TEXT_BASE     MCU1_0_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU1_0_MEM_DATA_BASE     MCU1_0_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU1_0_DDR_SPACE_BASE    MCU1_0_MEM_DATA_BASE     + MEM_DATA_SIZE
/*--------------------------- MCU R5FSS0 CORE1 --------------------------*/
#define MCU1_1_IPC_DATA_BASE     MCU1_1_ALLOCATED_START
#define MCU1_1_EXT_DATA_BASE     MCU1_1_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU1_1_MEM_TEXT_BASE     MCU1_1_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU1_1_MEM_DATA_BASE     MCU1_1_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU1_1_DDR_SPACE_BASE    MCU1_1_MEM_DATA_BASE     + MEM_DATA_SIZE
/*--------------------------- MAIN R5FSS0 CORE0 -------------------------*/
#define MCU2_0_IPC_DATA_BASE     MCU2_0_ALLOCATED_START
#define MCU2_0_EXT_DATA_BASE     MCU2_0_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU2_0_MEM_TEXT_BASE     MCU2_0_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU2_0_MEM_DATA_BASE     MCU2_0_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU2_0_DDR_SPACE_BASE    MCU2_0_MEM_DATA_BASE     + MEM_DATA_SIZE
/*--------------------------- MAIN R5FSS0 CORE1 -------------------------*/
#define MCU2_1_IPC_DATA_BASE     MCU2_1_ALLOCATED_START
#define MCU2_1_EXT_DATA_BASE     MCU2_1_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU2_1_MEM_TEXT_BASE     MCU2_1_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU2_1_MEM_DATA_BASE     MCU2_1_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU2_1_DDR_SPACE_BASE    MCU2_1_MEM_DATA_BASE     + MEM_DATA_SIZE
/*--------------------------- MAIN R5FSS1 CORE0 -------------------------*/
#define MCU3_0_IPC_DATA_BASE     MCU3_0_ALLOCATED_START
#define MCU3_0_EXT_DATA_BASE     MCU3_0_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU3_0_MEM_TEXT_BASE     MCU3_0_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU3_0_MEM_DATA_BASE     MCU3_0_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU3_0_DDR_SPACE_BASE    MCU3_0_MEM_DATA_BASE     + MEM_DATA_SIZE
/*--------------------------- MAIN R5FSS1 CORE1 -------------------------*/
#define MCU3_1_IPC_DATA_BASE     MCU3_1_ALLOCATED_START
#define MCU3_1_EXT_DATA_BASE     MCU3_1_IPC_DATA_BASE     + IPC_DATA_SIZE
#define MCU3_1_MEM_TEXT_BASE     MCU3_1_EXT_DATA_BASE     + EXT_DATA_SIZE
#define MCU3_1_MEM_DATA_BASE     MCU3_1_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define MCU3_1_DDR_SPACE_BASE    MCU3_1_MEM_DATA_BASE     + MEM_DATA_SIZE
/*------------------------------------------------------------------------/
 * NOTE: C66x Cores IPC_DATA is swapped each other, for proper caching!! *           
 * This is to overcome the limitation - "16 MB" being the minimum cache  *
 * block size in C66x. Requirement is to disable cache for IPC_DATA only * 
 * i.e, the "first 1 MB" allocated for a core.                           *
 * To handle this,                                                       *
 * - C66x_1 marks "C66x2_ALLOCATED_START" + 16 MB as Cache Disabled.     *
 * - Places C66x1_IPC_DATA in "C66x2_ALLOCATED_START" + 1 MB             *
 *   - which is un-cached region from C66x_1's perspective               *
 * - Places rest of the core sections (EXT_DATA/MEM_TEXT/MEM_DATA/DDR)   *
 *   in its own dedicated regions.                                       *
 *   - i.e. starting from C66x1_ALLOCATED_START + 1 MB                   *
 *   - which remains "cached" region from C66x_1's perspective           *
 * Vice versa for C66x_2                                                 *
 * - C66x_2 marks "C66x1_ALLOCATED_START" + 16 MB as Cache Disabled.     *
 * - Places C66x2_IPC_DATA in "C66x1_ALLOCATED_START" + 1 MB             *
 *   - which is un-cached region from C66x_2's perspective               *
 * - Places rest of the core sections (EXT_DATA/MEM_TEXT/MEM_DATA/DDR)   *
 *   in its own dedicated regions.                                       *
 *   - i.e. starting from C66x1_ALLOCATED_START + 1 MB                   *
 *   - which remains "cached" region from C66x_1's perspective           */
/*--------------------------- C66x DSP CORE1 ----------------------------*/
#define C66x1_IPC_DATA_BASE      C66x2_ALLOCATED_START /* Swapped for proper caching */
#define C66x1_EXT_DATA_BASE      C66x1_ALLOCATED_START    + IPC_DATA_SIZE
#define C66x1_MEM_TEXT_BASE      C66x1_EXT_DATA_BASE      + EXT_DATA_SIZE
#define C66x1_MEM_DATA_BASE      C66x1_MEM_TEXT_BASE      + MEM_TEXT_SIZE
#define C66x1_DDR_SPACE_BASE     C66x1_MEM_DATA_BASE      + MEM_DATA_SIZE
/*--------------------------- C66x DSP CORE2 ----------------------------*/
#define C66x2_IPC_DATA_BASE      C66x1_ALLOCATED_START /* Swapped for proper caching */
#define C66x2_EXT_DATA_BASE      C66x2_ALLOCATED_START    + IPC_DATA_SIZE
#define C66x2_MEM_TEXT_BASE      C66x2_EXT_DATA_BASE      + EXT_DATA_SIZE
#define C66x2_MEM_DATA_BASE      C66x2_MEM_TEXT_BASE      + MEM_TEXT_SIZE
#define C66x2_DDR_SPACE_BASE     C66x2_MEM_DATA_BASE      + MEM_DATA_SIZE
/*--------------------------- C7x ---------------------------------------*/
#define C7x_1_IPC_DATA_BASE      C7x_1_ALLOCATED_START
#define C7x_1_EXT_DATA_BASE      C7x_1_IPC_DATA_BASE     + IPC_DATA_SIZE
#define C7x_1_MEM_TEXT_BASE      C7x_1_EXT_DATA_BASE     + EXT_DATA_SIZE
#define C7x_1_MEM_DATA_BASE      C7x_1_MEM_TEXT_BASE     + MEM_TEXT_SIZE
#define C7x_1_DDR_SPACE_BASE     C7x_1_MEM_DATA_BASE     + MEM_DATA_SIZE
