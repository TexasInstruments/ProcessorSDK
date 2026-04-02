/*
 * Copyright 2018 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *	\file	xmodem.c
 *
 *	\brief	This file receives header and image from uniflash
 *			using xmodem protocol
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <xmodem.h>
#include <ti/board/utils/uniflash/target/soc/soc.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(am64x_evm)
/* Reduced the delay for am64x evm as the initial handshake is taking longer 
   with higher delay when compared with other platforms, due to lower CPU clock.
 */
uint32_t DELAY = 500000U;
#else
uint32_t DELAY = 0x3FFFFF;
#endif
extern uint32_t uart_baseAddr;
extern UFP_flashConfig UPF_flashFxnPtr[FLASH_DEVICE_MAX];
extern UART_Handle gUfpUartHandle;
uint8_t gDataRxBuff[DATA_BUFF_LEN], gDataCheckBuff[DATA_BUFF_LEN];

static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
	0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
	0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
	0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3dE,
	0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
	0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
	0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
	0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
	0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
	0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
	0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
	0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
	0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
	0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
	0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
	0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
	0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
	0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
	0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
	0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
	0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
	0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
	0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
	0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
	0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
	0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
	0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
	0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
	0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
	0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
	0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *	\brief		This function calculates CRC for each and every packet to
 * 				be transmitted over the Serial interface.
 *
 *	\param		flashAddr   [IN]	Pointer to the received image from uart
 *	\param		checkAddr   [IN]	Pointer to the image read from flash.
 *
 *	\return		int8_t
 *                  0 -	in case of success
 *                 -1 - in case of failure
 *
 */
static int8_t UFP_flashCheck(uint8_t *flashAddr,
                             uint8_t *checkAddr, uint32_t size)
{
    uint32_t i = 0U;
    uint8_t flashValue = 0, checkValue = 0;

    for(i = 0U; i < size; i +=4U)
    {
        flashValue = *(flashAddr+i);
        checkValue = *(checkAddr+i);
        if (flashValue != checkValue)
        {
            return -1;
        }
    }
    return 0;
}

/**
 *	\brief		This function calculates CRC for each and every packet.
 *
 *	\param		buf		[IN]	Pointer where the data have stored
 *	\param		len		[IN]	length of the buffer
 *
 *	\return		uint_8 	-		return calculated CRC
 *
 */
static unsigned short crc16_ccitt(const void *buf, int len)
{
	uint16_t counter;
	Bool crc = UFALSE;
	for(counter = 0U; counter < len; counter++)
	{
	  crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *(char *)buf) & 0x00FF];
	  buf = ((char *)buf) +1;
	}
	return crc;
}

/**
 *	\brief		This function compares the CRC calculated with the received
 *				data and the CRC received in the data packet.
 *
 *	\param		buf		[IN]	Pointer where the data have stored
 *	\param		sz		[IN]	length of the buffer
 *
 *	\return		uint8_t 
 *					1 -	in case of success
 *					0 - in case of failure
 *
 */
static uint8_t check(const unsigned char *buf, int sz)
{
	Bool crc = crc16_ccitt(buf, sz);
	Bool tcrc = (buf[sz] << 8) + buf[sz+1];
	if (crc == tcrc)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 *	\brief		This function generates delay in msec.
 *
 *	\param		delay_val	[IN]	delay value
 *
 */
void delay(uint16_t delay_val)
{
    uint32_t delayVal;
	delayVal = delay_val * 1000;
	while(delayVal--);
}

/**
 *	\brief		This function reads a character from UART
 *				until a specified timeout.
 *
 *	\param		msec	[IN]	timeout
 *
 *	\return		int32_t -
 *				read data	- in case of success
 *				-1			- in case of failure.
 *
 */
int32_t inbyte(uint32_t msec)
{
#if defined(SOC_K2G)
    return(UART_charGetTimeout_v0(uart_baseAddr, msec));
#elif (defined(SOC_TPR12) || defined(SOC_AWR294X))
    uint8_t data;
    if(UART_readPolling(gUfpUartHandle, (void*)&data, 1) > 0)
    {
        return data;
    }
    else
    {
        /* Clears the UART usage in case of read timeout */
        UART_readCancel(gUfpUartHandle);
        return -1;
    }
#else
    return(UARTCharGetTimeout(uart_baseAddr, msec));
#endif
}

/**
 *  \brief		This function writes a character on UART
 *
 *  \param		c		[IN]	Data to write
 *
 */
void outbyte(uint8_t c)
{
#if defined(SOC_K2G)
    UART_charPut_v0(uart_baseAddr, c);
#elif (defined(SOC_TPR12) || defined(SOC_AWR294X))
    UART_writePolling(gUfpUartHandle, (const void*)&c, 1);
#else
    UARTCharPut(uart_baseAddr, c);
#endif
}

/**
 *	\brief		This function receives the image, using xmodem protocol
 *				from uniflash and copies to the specified location of DDR.
 *
 *  \param		offset	[IN]   Offset to flash the receiving image
 *	\param		devType	[IN]   Flash device type to flash image.
 *
 *  \return		
 *
 */
int8_t UFP_xModemFileReceive(uint32_t offset, uint8_t devType)
{
    int32_t c;
	uint16_t len = 0;
	uint16_t i, bufsz;
	uint16_t retry;
    uint32_t retrans = MAXRETRANS + 1;
    int8_t ret;
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
    UFP_fxnTable const *fnPtr = (UPF_flashFxnPtr[devType]).UPF_fxnTablePtr;
    uint32_t maxBaudRate;

    maxBaudRate = UFP_getMaxBaudrate();
    ret = UFP_uartConfig(maxBaudRate);
    if(0 != ret)
    {
        return -1;
    }

	for(;;)
	{
		for(retry = 1; retry < MAXRETRANS; ++retry)
		{
			if(trychar)
			{
				outbyte(trychar);
			}
			if(0 <= (c = inbyte(DELAY)))
			{
				switch(c)
				{
					case XMODEM_CMD_SOH:
						bufsz = 128;
						goto start_recv;
					case XMODEM_CMD_STX:
						bufsz = 1024;
						goto start_recv;
					case XMODEM_CMD_EOT:
                        outbyte(XMODEM_STS_ACK);
                        if (0 != len)
                        {
                            ret = fnPtr->UFP_flashProgram(&gDataRxBuff[0],
                                  &gDataCheckBuff[0], offset, DATA_BUFF_LEN);
                            if (0 != ret)
                            {
                                return -1;
                            }
                            ret = UFP_flashCheck(&gDataRxBuff[0],
                                                 &gDataCheckBuff[0], len);
                            if (0!= ret)
                            {
                                return -1;
                            }
                        }
						return 0; /* normal end */
					case XMODEM_CMD_CAN:
						if(XMODEM_CMD_CAN == (c = inbyte(DELAY)))
						{
							outbyte(XMODEM_STS_ACK);
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		if('C' == trychar)
		{
			trychar = XMODEM_STS_NAK;
			continue;
		}
		outbyte(XMODEM_CMD_CAN);
		outbyte(XMODEM_CMD_CAN);
		outbyte(XMODEM_CMD_CAN); /* sync error */
		return -1;

		start_recv:
		trychar = 0;
		p = xbuff;
		*p++ = c;

		/* bufsz( 1024/128 ) + crc (2 bytes) + 2nd & 3rd bytes of Data Packet
		( Packet Number + 1's complement of Packet Number ) */
		for (i = 0; i < (bufsz + 4); ++i)
		{
			if ((c = inbyte(DELAY)) < 0)
				goto reject;
			*p++ = c;
		}

		if (xbuff[1] == (unsigned char)(~xbuff[2]) && 
			(xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
			check(&xbuff[3], bufsz) )
		{
			if (xbuff[1] == packetno)
			{
                memcpy (&gDataRxBuff[len], &xbuff[3], bufsz); 
                len += bufsz;
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (0 >= --retrans)
			{
				outbyte(XMODEM_CMD_CAN);
				outbyte(XMODEM_CMD_CAN);
				outbyte(XMODEM_CMD_CAN);
				return -1;
			}
            if (DATA_BUFF_LEN <= len)
            {
                ret = fnPtr->UFP_flashProgram(&gDataRxBuff[0], &gDataCheckBuff[0],
                                              offset, DATA_BUFF_LEN);
                if (0 != ret)
                {
                    return -1;
                }
                ret = UFP_flashCheck(&gDataRxBuff[0], &gDataCheckBuff[0],
                                     DATA_BUFF_LEN);
                if (0 != ret)
                {
                    return -1;
                }
                    offset += DATA_BUFF_LEN;
                    len = 0;
            }
			outbyte(XMODEM_STS_ACK);
			continue;
		}
        reject:
            outbyte(XMODEM_STS_NAK);
	}
}

/**
 *	\brief		This function receives the image, using xmodem protocol
 *				from uniflash and copies to the specified location of XIP Flash.
 *
 *	\param		devType	[IN]   Flash device type to flash image.
 *
 *  \return		
 *
 */
int8_t UFP_xModemXipFileReceive(uint8_t devType)
{
    int32_t c;
	uint16_t len = 0;
    uint32_t offset = 0U;
    uint16_t i, bufsz;
	uint16_t retry;
    uint32_t retrans = MAXRETRANS + 1;
    int8_t ret;
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
    UFP_fxnTable const *fnPtr = (UPF_flashFxnPtr[devType]).UPF_fxnTablePtr;
    Bootloader_MetaHeaderStart static mHdrStr;
    Bootloader_MetaHeaderCore  static mHdrCore[BOOTLOADER_MAX_INPUT_FILES];
    Bootloader_RprcFileHeader  static rprcHeader;
    Bootloader_RprcSectionHeader static rprcSection;
    Bootloader_ExpectedSection static expectedSection = META_HEADER;
    uint32_t maxBaudRate;
    uint8_t *imgPtr;
    uint8_t *chkPtr;
    uint32_t wrSize;
    uint8_t  static overflowBuffer[DATA_BUFF_LEN];
    uint32_t static overflowSize = 0;
    uint32_t static sectionOffset;
    uint32_t static sectionRemaining;
    uint32_t static sectionCount;

    maxBaudRate = UFP_getMaxBaudrate();
    ret = UFP_uartConfig(maxBaudRate);
    if(0 != ret)
    {
        return -1;
    }

	for(;;)
	{
		for(retry = 1; retry < MAXRETRANS; ++retry)
		{
			if(trychar)
			{
				outbyte(trychar);
			}
			if(0 <= (c = inbyte(DELAY)))
			{
				switch(c)
				{
					case XMODEM_CMD_SOH:
						bufsz = 128;
						goto start_recv;
					case XMODEM_CMD_STX:
						bufsz = 1024;
						goto start_recv;
					case XMODEM_CMD_EOT:
                        if(overflowSize > 0)
                        {
                            memmove(&gDataRxBuff[overflowSize], &gDataRxBuff[0], overflowSize);
                            memcpy(&gDataRxBuff[0], &overflowBuffer[0], overflowSize);
                            ret = fnPtr->UFP_flashProgram(&gDataRxBuff[0], &gDataCheckBuff[0], sectionOffset, DATA_BUFF_LEN);
                            if (0 != ret)
                            {
                                return -1;
                            }
                            ret = UFP_flashCheck(&gDataRxBuff[0], &gDataCheckBuff[0], overflowSize);
                            if (0 != ret)
                            {
                                return -1;
                            }
                        }
                        outbyte(XMODEM_STS_ACK);
						return 0; /* normal end */
					case XMODEM_CMD_CAN:
						if(XMODEM_CMD_CAN == (c = inbyte(DELAY)))
						{
							outbyte(XMODEM_STS_ACK);
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		if('C' == trychar)
		{
			trychar = XMODEM_STS_NAK;
			continue;
		}
		outbyte(XMODEM_CMD_CAN);
		outbyte(XMODEM_CMD_CAN);
		outbyte(XMODEM_CMD_CAN); /* sync error */
		return -1;

		start_recv:
		trychar = 0;
		p = xbuff;
		*p++ = c;

		/* bufsz( 1024/128 ) + crc (2 bytes) + 2nd & 3rd bytes of Data Packet
		( Packet Number + 1's complement of Packet Number ) */
		for (i = 0; i < (bufsz + 4); ++i)
		{
			if ((c = inbyte(DELAY)) < 0)
				goto reject;
			*p++ = c;
		}

		if (xbuff[1] == (unsigned char)(~xbuff[2]) && 
			(xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
			check(&xbuff[3], bufsz) )
		{
			if (xbuff[1] == packetno)
			{
                memcpy (&gDataRxBuff[len], &xbuff[3], bufsz); 
                len += bufsz;
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (0 >= --retrans)
			{
				outbyte(XMODEM_CMD_CAN);
				outbyte(XMODEM_CMD_CAN);
				outbyte(XMODEM_CMD_CAN);
				return -1;
			}
            if(overflowSize > 0)
            {
                memmove(&gDataRxBuff[overflowSize], &gDataRxBuff[0], bufsz);
                memcpy(&gDataRxBuff[0], &overflowBuffer[0], overflowSize);
            }
            imgPtr = &gDataRxBuff[0];
            chkPtr = &gDataCheckBuff[0];
            parser:
            switch(expectedSection)
            {
                case META_HEADER:     /* Meta Header */
                    memset(&mHdrCore[0], 0xFF, BOOTLOADER_MAX_INPUT_FILES*sizeof(Bootloader_MetaHeaderCore));
                    memcpy(&mHdrStr, imgPtr, sizeof(Bootloader_MetaHeaderStart));
                    imgPtr += sizeof(Bootloader_MetaHeaderStart);
                    offset += sizeof(Bootloader_MetaHeaderStart);
                    if(BOOTLOADER_META_HDR_MAGIC_STR != mHdrStr.magicStr)
                    {
                        ret = -1;
                        if (0 != ret)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        /* Read all the core offset addresses */
                        uint32_t i;

                        for(i = 0U; i < mHdrStr.numFiles; i++)
                        {
                            memcpy(&mHdrCore[i], imgPtr, sizeof(Bootloader_MetaHeaderCore));
                            imgPtr += sizeof(Bootloader_MetaHeaderCore);
                            offset += sizeof(Bootloader_MetaHeaderCore);
                        }
                    }
                    /* Skip Header End */
                    imgPtr += sizeof(Bootloader_MetaHeaderCore);
                    offset += sizeof(Bootloader_MetaHeaderCore);
                    /* Update Expected Section */
                    expectedSection = RPRC_HEADER;
                case RPRC_HEADER: /* RPRC Header */
                    if(offset+sizeof(Bootloader_RprcFileHeader) >= bufsz)
                    {
                        expectedSection = RPRC_HEADER;
                    }
                    else
                    {
                        memcpy(&rprcHeader, imgPtr, sizeof(Bootloader_RprcFileHeader));
                        imgPtr += sizeof(Bootloader_RprcFileHeader);
                        offset += sizeof(Bootloader_RprcFileHeader);
                        if(BOOTLOADER_RPRC_MAGIC_NUMBER != rprcHeader.magic)
                        {
                            ret = -1;
                            if (0 != ret)
                            {
                                return -1;
                            }
                        }
                        expectedSection = SECTION_HEADER;
                        sectionCount = 1;
                    }
                case SECTION_HEADER: /* Section Header */
                    if(offset+sizeof(Bootloader_RprcSectionHeader) >= bufsz)
                    {
                        expectedSection = SECTION_HEADER;
                    }
                    else
                    {
                        memcpy(&rprcSection, imgPtr, sizeof(Bootloader_RprcSectionHeader));
                        imgPtr += sizeof(Bootloader_RprcSectionHeader);
                        offset += sizeof(Bootloader_RprcSectionHeader);
                        sectionOffset = rprcSection.addr;
                        sectionRemaining = rprcSection.size;
                        expectedSection = SECTION_CONTENT;
                    }
                    break;
                case SECTION_CONTENT: /* Section Content */
                    if(sectionRemaining < bufsz)
                    {
                        wrSize = sectionRemaining;
                    }
                    else
                    {
                        wrSize = bufsz;
                    }

                    ret = fnPtr->UFP_flashProgram(imgPtr, chkPtr, sectionOffset, wrSize);
                    if (0 != ret)
                    {
                        return -1;
                    }

                    ret = UFP_flashCheck(imgPtr, chkPtr, wrSize);
                    if (0 != ret)
                    {
                        return -1;
                    }

                    imgPtr += wrSize;
                    offset += wrSize;
                    sectionOffset += wrSize;
                    sectionRemaining -= wrSize;
                    if(sectionOffset >= (rprcSection.addr+rprcSection.size))
                    {
                        sectionCount++;
                        if(sectionCount > rprcHeader.sectionCount)
                        {
                            expectedSection = RPRC_HEADER;
                        }
                        else
                        {
                            expectedSection = SECTION_HEADER;
                        }
                    }
            }
            if(offset < (bufsz+overflowSize)) /* There is still more data */
            {
                if( (bufsz + overflowSize - offset) > (DATA_BUFF_LEN - bufsz))
                {
                    goto parser;
                }
                else
                {
                    overflowSize = bufsz + overflowSize - offset;
                    memset(overflowBuffer, 0xFF, overflowSize);
                    memcpy(overflowBuffer, imgPtr, overflowSize);
                }
            }
            else
            {
                overflowSize = 0;
            }
            offset = 0U;
            len = 0;
			outbyte(XMODEM_STS_ACK);
			continue;
		}
        reject:
            outbyte(XMODEM_STS_NAK);
	}
}

/**
 *	\brief		This function receive the header information of the image
 *				from uniflash using xmodem protocol.
 *
 *  \param		xbuff		[OUT]   Buffer to store the received header.
 *
 *	\return		int8_t
 *					0 	- in case of success
 *				   -1	- in case of failure
 *				   -2	- if it didn't receive header from uniflash
 *						  within timeout.
 *
 */
int8_t UFP_xModemHeaderReceive(unsigned char *xbuff)
{
	int32_t c;
	uint8_t bufsz, i;
    uint32_t retry;
	unsigned char trychar = 'C';
    int8_t ret;

    ret = UFP_uartConfig(UFP_BAUDRATE_115200);
    if(0 != ret)
    {
        return -1;
    }

    for(retry = 1; retry < MAXRETRANS; ++retry)
    {
        if (trychar)
        {
            outbyte(trychar);
        }
        if (0 <= (c = inbyte(DELAY)))
        {
            switch (c)
            {
                case 'S':
                    bufsz = 14;
                    xbuff[0] = c;
                    goto start_recv;
                case XMODEM_CMD_CAN:
                    
                    break;
                default:
                    break;
            }
        }
    }

    UFP_uartClose();

    return -2;
    start_recv:
    for (i = 1; i < bufsz; ++i)
    {
        c = inbyte(DELAY);
        xbuff[i] = c;
    }

    if (!(check(&xbuff[0], 12))) /* 12th & 13th bytes have CRC */
        return -1;

    return 0;
}

/**
 *  \brief        This function receives the image, using xmodem protocol
 *                from uniflash and copies to the specified location of DDR.
 *
 *  \param        dest    [IN]   DDR address to store image.
 *  \param        destsz    [IN]   Maximum size for storing image to DDR.
 *
 *  \return       uint32_t
 *                  size of the image    - in case of success
 *                  0                    - in case of failure.
 *
 */
uint32_t UFP_xModemFirmwareReceive(unsigned char *dest, uint32_t destsz)
{
    uint32_t len = 0;
    int32_t c;
    uint16_t i, bufsz;
    uint16_t retry;
    uint32_t retrans = MAXRETRANS+1;
    unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
    unsigned char *p;
    unsigned char trychar = 'C';
    unsigned char packetno = 1;

    for(;;)
    {
        for(retry = 1; retry < MAXRETRANS; ++retry)
        {
            if(trychar)
            {
                outbyte(trychar);
            }
            if(0 <= (c = inbyte(DELAY)))
            {
                switch(c)
                {
                    case XMODEM_CMD_SOH:
                        bufsz = 128;
                        goto start_recv;
                    case XMODEM_CMD_STX:
                        bufsz = 1024;
                        goto start_recv;
                    case XMODEM_CMD_EOT:
                        outbyte(XMODEM_STS_ACK);
                        return len; /* normal end */
                    case XMODEM_CMD_CAN:
                        if(XMODEM_CMD_CAN == (c = inbyte(DELAY)))
                        {
                            outbyte(XMODEM_STS_ACK);
                            return 0; /* canceled by remote */
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if('C' == trychar)
        {
            trychar = XMODEM_STS_NAK;
            continue;
        }
        outbyte(XMODEM_CMD_CAN);
        outbyte(XMODEM_CMD_CAN);
        outbyte(XMODEM_CMD_CAN); /* sync error */
        return 0;

        start_recv:
        trychar = 0;
        p = xbuff;
        *p++ = c;

        /* bufsz( 1024/128 ) + crc (2 bytes) + 2nd & 3rd bytes of Data Packet
        ( Packet Number + 1's complement of Packet Number ) */
        for (i = 0; i < (bufsz + 4); ++i)
        {
            if ((c = inbyte(DELAY)) < 0)
                goto reject;
            *p++ = c;
        }

        if (xbuff[1] == (unsigned char)(~xbuff[2]) &&
            (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
            check(&xbuff[3], bufsz) )
        {
            if (xbuff[1] == packetno)
            {
                register uint32_t count = destsz - len;
                if (count > bufsz)
                {
                    /* Remaining bytes/space (count) in destination is more than
                       bytes received from host. Write the bytes received (bufsz) 
                       to the destination.
                     */
                    count = bufsz;
                }
                if (count > 0)
                {
                    memcpy (&dest[len], &xbuff[3], count);
                    len += count;
                }
                ++packetno;
                retrans = MAXRETRANS+1;
            }
            if (0 >= --retrans)
            {
                outbyte(XMODEM_CMD_CAN);
                outbyte(XMODEM_CMD_CAN);
                outbyte(XMODEM_CMD_CAN);
                break;
            }
            outbyte(XMODEM_STS_ACK);
            continue;
        }
    reject:
        outbyte(XMODEM_STS_NAK);
    }

    return 0;
}
