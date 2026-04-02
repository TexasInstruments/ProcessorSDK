/*
 * mss_Utils.h
 *
 *  Created on: Feb 9, 2021
 *      Author: fliu_
 */

#ifndef _MSS_UTILS_H_
#define _MSS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Macro Define */
//#define OFFR_MAX_DMA_TRANSFER_BYTES     (8192U)    /* Maximum DMA transfer bytes */

/* SPI */
extern void MSS_UtilsSPIInit(void);
extern int32_t MSS_UtilsSPIRead(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t* pu8Buffer);
extern int32_t MSS_UtilsSPIWrite(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t* pu8Buffer);
extern int32_t MSS_UtilsSPIReadWrite(const MIBSPI_Handle stHandle, uint32_t u32DataLen, uint8_t* pu8TxBuffer, uint8_t* pu8RxBuffer);

/* SPI Test */
void spiReadWriteTest();
void spiReadTest();
void spiWriteTest();
void spiRandomNumberTest();

#ifdef __cplusplus
}
#endif

#endif /* _MSS_UTILS_H_ */
