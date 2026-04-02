/* CC0 (Public domain) - see licenses/CC0 file for details */
#ifndef SHORT_TYPES_H
#define SHORT_TYPES_H
#include <stdint.h>

#ifndef NULL
#ifndef __cplusplus
#define NULL     ((void *) 0)
#else
#define NULL     (nullptr)
#endif
#endif

/**
 * u64/s64/u32/s32/u16/s16/u8/s8 - short names for explicitly-sized types.
 */
typedef uint64_t u64;
typedef int64_t s64;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint8_t u8;
typedef int8_t s8;

#endif /* SHORT_TYPES_H */
