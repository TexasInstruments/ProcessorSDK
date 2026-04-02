/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *
 * \file    board_flash.c
 *
 * \brief   This contains board flash common APIs.
 *
 ******************************************************************************/

#include "board_cfg.h"
#include "board_flash.h"

/* This structure holds information about the flash devices on the board */
Board_FlashInfo Board_flashInfo[MAX_BOARD_FLASH_INSTANCE_NUM] =
{
    {0, },
};

static uint32_t Board_getFlashIntf(uint32_t deviceId)
{
    uint32_t flashIntf = 0U;

    if((BOARD_FLASH_ID_NORN25Q128          == deviceId) || \
       (BOARD_FLASH_ID_NORN25Q128A13ESF40F == deviceId) || \
       (BOARD_FLASH_ID_W25X32VSFIG         == deviceId))
    {
        flashIntf = BOARD_FLASH_NOR_SPI;
    }
    else if((BOARD_FLASH_ID_MT25QU512ABB == deviceId) || \
            (BOARD_FLASH_ID_S25FL512S    == deviceId) || \
            (BOARD_FLASH_ID_S25FL256S    == deviceId) ||
            (BOARD_FLASH_ID_GD25B16CSAG  == deviceId) ||
            (BOARD_FLASH_ID_GD25B64CW2G  == deviceId) ||
            (BOARD_FLASH_ID_W25Q16FWSF   == deviceId) ||
            (BOARD_FLASH_ID_MX25V1635F   == deviceId))
    {
        flashIntf = BOARD_FLASH_NOR_QSPI;
    }
    else if((BOARD_FLASH_ID_MT29W160EB   == deviceId) || \
            (BOARD_FLASH_ID_MT28EW256ABA == deviceId))
    {
        flashIntf = BOARD_FLASH_NOR_GPMC;
    }
    else if((BOARD_FLASH_ID_MT29F4G08ABAEAWP == deviceId) || \
            (BOARD_FLASH_ID_MT29F4G16ABAFAH  == deviceId) || \
            (BOARD_FLASH_ID_MT29F8G16ABACAWP == deviceId))
    {
        flashIntf = BOARD_FLASH_NAND_GPMC;
    }
    else if((BOARD_FLASH_ID_MT35XU512ABA1G12 == deviceId) || \
            (BOARD_FLASH_ID_MT35XU256ABA1G12 == deviceId) || \
            (BOARD_FLASH_ID_S28HS512T        == deviceId))
    {
        flashIntf = BOARD_FLASH_NOR_OSPI;
    }
    else if(BOARD_FLASH_ID_S71KS512S == deviceId)
    {
        flashIntf = BOARD_FLASH_NOR_HPF;
    }
    else if(BOARD_FLASH_ID_W35N01JWTBAG == deviceId)
    {
        flashIntf = BOARD_FLASH_NAND_OSPI;
    }
    else
    {
        /* Unknown flash type */
        flashIntf = BOARD_FLASH_NOR_INTF_MAX;
    }

    return (flashIntf);
}
/******************************************************************************
 * BOARD_flashOpen
 ******************************************************************************/
Board_flashHandle Board_flashOpen(uint32_t deviceId, uint32_t portNum, void *params)
{
    uint32_t           count;
    Board_FlashInfo   *flashInfo;
    uint32_t           flashIntf;

    for (count = 0U; count < MAX_BOARD_FLASH_INSTANCE_NUM; count++)
    {
        if (0U == Board_flashInfo[count].flashHandle)
        {
            flashInfo = &Board_flashInfo[count];
            break;
        }
    }

    if (MAX_BOARD_FLASH_INSTANCE_NUM == count)
    {
        return (0U);
    }

    flashIntf = Board_getFlashIntf(deviceId);

#if defined (BOARD_NOR_FLASH_IN)
    NOR_HANDLE         flashHandle;
    NOR_Info          *norInfo;

    if (                                                    \
        (BOARD_FLASH_ID_S25FL512S           == deviceId)    || \
        (BOARD_FLASH_ID_S25FL256S           == deviceId)    || \
        (BOARD_FLASH_ID_MT29W160EB          == deviceId)    || \
        (BOARD_FLASH_ID_W25X32VSFIG         == deviceId)    || \
        (BOARD_FLASH_ID_NORN25Q128          == deviceId)    || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == deviceId)    || \
        (BOARD_FLASH_ID_MT25QU512ABB        == deviceId)    || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == deviceId)    || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == deviceId)    || \
        (BOARD_FLASH_ID_S71KS512S           == deviceId)    || \
        (BOARD_FLASH_ID_S28HS512T           == deviceId)    || \
        (BOARD_FLASH_ID_GD25B16CSAG         == deviceId)    || \
        (BOARD_FLASH_ID_GD25B64CW2G         == deviceId)    || \
        (BOARD_FLASH_ID_W25Q16FWSF          == deviceId)    || \
        (BOARD_FLASH_ID_MX25V1635F          == deviceId)    || \
        (BOARD_FLASH_ID_MT28EW256ABA        == deviceId)
       )
    {
        /* Open the NOR flash */
        flashHandle = NOR_open(flashIntf, portNum, params);
        if (!flashHandle)
        {
            return 0;
        }

        norInfo = (NOR_Info *)flashHandle;
        if (deviceId != norInfo->deviceId)
        {

            NOR_close(flashHandle);
            return 0;
        }

        flashInfo->flashHandle     = flashHandle;
        flashInfo->manufacturer_id = norInfo->manufacturerId;
        flashInfo->device_id       = norInfo->deviceId;
        flashInfo->type            = BOARD_FLASH_NOR;
        flashInfo->block_count     = norInfo->blockCnt;
        flashInfo->page_count      = norInfo->pageCnt;
        flashInfo->page_size       = norInfo->pageSize;
        flashInfo->sector_size     = norInfo->sectorSize;
        flashInfo->blkErase_flag   = BTRUE; /* Default set to Block Erase */

        return (Board_flashHandle)&(Board_flashInfo);
    }
#endif
#if defined (BOARD_NAND_FLASH_IN)
    NAND_HANDLE        nandFlashHandle;
    NAND_Info         *nandInfo;

    if ((BOARD_FLASH_ID_MT29F4G08ABAEAWP == deviceId) || \
        (BOARD_FLASH_ID_MT29F4G16ABAFAH  == deviceId) || \
        (BOARD_FLASH_ID_MT29F8G16ABACAWP == deviceId) || \
        (BOARD_FLASH_ID_W35N01JWTBAG     == deviceId))
    {
        /* Open the Nand flash */
        nandFlashHandle = NAND_open(flashIntf, portNum, params);
        if (!nandFlashHandle)
        {
            return 0;
        }
        nandInfo = (NAND_Info *)nandFlashHandle;

        if (deviceId != nandInfo->deviceId)
        {
            NAND_close(nandFlashHandle);
            return 0;
        }

        flashInfo->flashHandle     = nandFlashHandle;
        flashInfo->manufacturer_id = nandInfo->manufacturerId;
        flashInfo->device_id       = nandInfo->deviceId;
        flashInfo->type            = BOARD_FLASH_NAND;
        flashInfo->block_count     = nandInfo->blockCnt;
        flashInfo->page_count      = nandInfo->pageCnt;
        flashInfo->page_size       = nandInfo->pageSize;
        flashInfo->spare_size      = nandInfo->spareSize;
        flashInfo->bboffset        = nandInfo->bbOffset;
        flashInfo->bblist          = nandInfo->bbList;

        return (Board_flashHandle)(flashInfo);
    }
#endif

    return 0;
}

/******************************************************************************
 * BOARD_flashClose
 ******************************************************************************/
Board_flash_STATUS Board_flashClose(Board_flashHandle handle)
{
    Board_FlashInfo *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

#if defined(BOARD_NOR_FLASH_IN)
    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)  	 || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)     || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)		 || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)
       )
    {
        NOR_close(flashInfo->flashHandle);
    }
#endif
#if defined (BOARD_NAND_FLASH_IN)
    if ((BOARD_FLASH_ID_MT29F4G08ABAEAWP == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29F4G16ABAFAH  == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29F8G16ABACAWP == flashInfo->device_id) || \
        (BOARD_FLASH_ID_W35N01JWTBAG     == flashInfo->device_id))
    {
        NAND_close(flashInfo->flashHandle);
    }
#endif

    flashInfo->flashHandle = 0;

    return BOARD_FLASH_EOK;
}

/******************************************************************************
 * BOARD_flashRead
 ******************************************************************************/
Board_flash_STATUS Board_flashRead(Board_flashHandle  handle,
                                   uint32_t           offset,
                                   uint8_t           *buf,
                                   uint32_t           len,
                                   void              *params)
{
    Board_FlashInfo   *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

    if (0U == len)
    {
        return BOARD_FLASH_EINVALID;
    }

#if defined(BOARD_NOR_FLASH_IN)
    uint32_t readMode = 0U;

    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)	  || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)
       )
    {
        if (params)
        {
            readMode = *((uint32_t *)params);
        }

        if (NOR_PASS != NOR_read(flashInfo->flashHandle, offset, len, buf, readMode))
        {
            return (BOARD_FLASH_EFAIL);
        }
        return BOARD_FLASH_EOK;
    }
#endif
#if defined (BOARD_NAND_FLASH_IN)
    if ((BOARD_FLASH_ID_MT29F4G08ABAEAWP == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29F4G16ABAFAH  == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29F8G16ABACAWP == flashInfo->device_id) || \
        (BOARD_FLASH_ID_W35N01JWTBAG     == flashInfo->device_id))
    {
        if (NAND_PASS != NAND_read(flashInfo->flashHandle, offset, len, buf))
        {
            return (BOARD_FLASH_EFAIL);
        }
        return BOARD_FLASH_EOK;
    }
#endif

    return BOARD_FLASH_EUNSUPPORTED;
}

 /**
 *  \brief		This function computes the sector and page based on an offset
 *              This function also sets the blkErase_flag to BFALSE to support
 *              Sector erase. This flag is used by the Board_flashEraseBlk()
 *              to determine the erase type
 *
 *	\param		handle		            [IN]   	Pointer to Board_FlashHandle.
 *              offset		          [IN]   	Flash Offset to be computed to Sector
 *              sector              [IN]    Computed Sector number
 *              page                [IN]    Computed page number
 *              hybridSector_flag   [IN]   	Flag to enable Hybrid Sector Erase
 *
 *  \return		Returns Board_flash_STATUS
 *
 */
Board_flash_STATUS Board_flashOffsetToSectorPage(Board_flashHandle  handle,
                                              uint32_t           offset,
                                              uint32_t          *sector,
                                              uint32_t          *page,
                                              bool              hybridSector_flag)
{
    uint32_t leftover, block_count, sector_count, page_size, page_count;
    Board_FlashInfo *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }
    if ((BOARD_FLASH_ID_S25FL256S == flashInfo->device_id) && (BFALSE == hybridSector_flag))
    {
        /* In case of S25FL256S device if hybridSector_flag is not set indicating
        uniform sector erase of 64KB, which is handled by Block erase */
        return Board_flashOffsetToBlkPage(handle, offset, sector, page);
    }

    if (                                                                \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id) || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id) || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id) || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT29F4G08ABAEAWP    == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id) || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id) || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)
       )
    {
        block_count = flashInfo->block_count;
        page_size	= flashInfo->page_size;
        page_count 	= flashInfo->page_count;
	}
    else
    {
        return BOARD_FLASH_EUNSUPPORTED;
    }

    sector_count = (page_count * page_size * block_count) / flashInfo->sector_size;

    *sector   = offset / flashInfo->sector_size;
    leftover  = offset % flashInfo->sector_size;
    *page 	  = leftover / page_size;
    if (leftover % page_size)
    {
        /* All writes must be page aligned for now */
        return BOARD_FLASH_EUNSUPPORTED;
    }
    if (*sector > sector_count)
    {
        return BOARD_FLASH_EINVALID;
    }
    if (*page > page_count)
    {
        return BOARD_FLASH_EINVALID;
    }

    flashInfo->blkErase_flag = BFALSE; /* This flag to be set BFALSE for Sector erase */

    return BOARD_FLASH_EOK;
}

/**
 *  \brief		This function computes the block and page number based on the
 *              offset. This function also sets the blkErase_flag to BTRUE to
 *              support Block erase. This flag is used by the
 *              Board_flashEraseBlk() to determine the erase type.
 *
 *	\param		handle		        [IN]   	Pointer to Board_FlashHandle.
 *              offset		      [IN]   	Flash Offset to be computed to Sector
 *              page            [IN]    Computed Page number
 *              page            [IN]    Computed page number
 *
 *  \return		Returns Board_flash_STATUS
 *
 */
Board_flash_STATUS Board_flashOffsetToBlkPage(Board_flashHandle  handle,
                                              uint32_t           offset,
                                              uint32_t          *block,
                                              uint32_t          *page)
{
    uint32_t leftover, block_size, block_count, page_size, page_count;
    Board_FlashInfo *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT29F4G08ABAEAWP    == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)    || \
        (BOARD_FLASH_ID_W35N01JWTBAG        == flashInfo->device_id)        )
    {
        block_count = flashInfo->block_count;
        page_size	= flashInfo->page_size;
        page_count 	= flashInfo->page_count;
	}
    else
    {
        return BOARD_FLASH_EUNSUPPORTED;
    }

    block_size = (page_count * page_size);

    *block 	  = offset / block_size;
    leftover  = offset % block_size;
    *page 	  = leftover / page_size;
    if (leftover % page_size)
    {
        /* All writes must be page aligned for now */
        return BOARD_FLASH_EUNSUPPORTED;
    }
    if (*block > block_count)
    {
        return BOARD_FLASH_EINVALID;
    }
    if (*page > page_count)
    {
        return BOARD_FLASH_EINVALID;
    }

    flashInfo->blkErase_flag = BTRUE; /* This flag to be set BTRUE for Block erase */
    return BOARD_FLASH_EOK;
}

/******************************************************************************
 * Computes a block and page based on an offset
 ******************************************************************************/
Board_flash_STATUS Board_flashBlkPageToOffset(Board_flashHandle  handle,
                                              uint32_t          *offset,
                                              uint32_t           block,
                                              uint32_t           page)
{
    uint32_t	     block_count, page_size, page_count;
    Board_FlashInfo *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_MT29F4G08ABAEAWP    == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)	    || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)      || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)		  || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)
       )
    {
        block_count = flashInfo->block_count;
        page_size	= flashInfo->page_size;
        page_count 	= flashInfo->page_count;
    }
    else
    {
        return BOARD_FLASH_EUNSUPPORTED;
    }

    if ((block > block_count) || (page > page_count))
    {
        return BOARD_FLASH_EINVALID;
    }

    *offset =   (block * (page_count * page_size)) + (page * page_size);

    return BOARD_FLASH_EOK;
}

/******************************************************************************
 * BOARD_flashWrite
 ******************************************************************************/
Board_flash_STATUS Board_flashWrite(Board_flashHandle  handle,
                                    uint32_t           offset,
                                    uint8_t           *buf,
                                    uint32_t           len,
                                    void              *params)
{
    Board_FlashInfo *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

    if ((NULL == buf) || (0U == len))
    {
        return BOARD_FLASH_EINVALID;
    }

#if defined(BOARD_NOR_FLASH_IN)
    uint32_t writeMode = 0;

    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)	 || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)
       )
    {
        if (params)
        {
            writeMode = *((uint32_t *)params);
        }

        if (NOR_PASS != NOR_write(flashInfo->flashHandle, offset, len, buf, writeMode))
        {
            return (BOARD_FLASH_EFAIL);
        }
        return BOARD_FLASH_EOK;
    }
#endif
#if defined (BOARD_NAND_FLASH_IN)
    if ((BOARD_FLASH_ID_MT29F4G08ABAEAWP == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_MT29F4G16ABAFAH  == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_MT29F8G16ABACAWP == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_W35N01JWTBAG     == flashInfo->device_id))
    {
        if (NAND_PASS != NAND_write(flashInfo->flashHandle, offset, len, buf))
        {
           return (BOARD_FLASH_EFAIL);
        }
        return BOARD_FLASH_EOK;
    }
#endif

    return BOARD_FLASH_EUNSUPPORTED;
}

/**
 *  \brief  platform_device_erase_block
 *
 *  Board_flashEraseBlk supports Block and Sector erase based on the blkErase_flag
 *  This API expects blkErase_flag to be set to the appropriate Boolean value to
 *  determine the type of erase.
 *
 *  blkErase_flag is set in one of the Board_flashOffsetToSectorPage() or
 *  Board_flashOffsetToBlkPage() APIs for sector or block erase respectively.
 *
 *  \param
 *  handle  - Handle to the flash device
 *  blk_num - Block number to be erased when blkErase_flag=BTRUE
 *            Sector number to be erased when blkErase_flag=BFALSE
 *
 *
 ********************************************************************************/
Board_flash_STATUS Board_flashEraseBlk(Board_flashHandle handle,
                                       uint32_t          blk_num)
{
    Board_FlashInfo    *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

    if (flashInfo->blkErase_flag)
    {
        if (blk_num > flashInfo->block_count)
        {
            return BOARD_FLASH_EFAIL;
        }
    }

#if defined(BOARD_NOR_FLASH_IN)
    if (                                                                \
        (BOARD_FLASH_ID_S25FL512S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S25FL256S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT29W160EB          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_W25X32VSFIG         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_NORN25Q128          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_NORN25Q128A13ESF40F == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT25QU512ABB        == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MT35XU512ABA1G12    == flashInfo->device_id)	 || \
        (BOARD_FLASH_ID_MT35XU256ABA1G12    == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S71KS512S           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_S28HS512T           == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_GD25B16CSAG         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_GD25B64CW2G         == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_W25Q16FWSF          == flashInfo->device_id)   || \
        (BOARD_FLASH_ID_MX25V1635F          == flashInfo->device_id)	 || \
        (BOARD_FLASH_ID_MT28EW256ABA        == flashInfo->device_id)
       )
    {
        if (NOR_PASS != NOR_erase (flashInfo->flashHandle, (int32_t)blk_num, flashInfo->blkErase_flag))
        {
            return (BOARD_FLASH_EFAIL);
        }
        return BOARD_FLASH_EOK;
    }
#endif
#if defined (BOARD_NAND_FLASH_IN)
    NAND_STATUS status;

    if ((BOARD_FLASH_ID_MT29F4G08ABAEAWP == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_MT29F4G16ABAFAH  == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_MT29F8G16ABACAWP == flashInfo->device_id)  || \
        (BOARD_FLASH_ID_W35N01JWTBAG     == flashInfo->device_id))
    {
        if (NAND_BAD_BLOCK == flashInfo->bblist[blk_num])
        {
            return (BOARD_FLASH_EFAIL);
        }

        status = NAND_erase(flashInfo->flashHandle, blk_num);
        if (NAND_PASS != status)
        {
            if (NAND_BLOCK_BAD == status)
            {
                flashInfo->bblist[blk_num] = NAND_BAD_BLOCK;
            }
            return (BOARD_FLASH_EFAIL);
        }
        else
        {
            return (BOARD_FLASH_EOK);
        }
    }
#endif

    return BOARD_FLASH_EUNSUPPORTED;
}

/**
 *  @brief       Executes flash control and configurations commands
 *
 *  @param[in]   handle  Flash device handle from the open
 *	@param[in]	 command Flash command to execute
 *  @param[in]   args    Command arguments based on type of the command
 *
 *  @retval      BOARD_FLASH_EOK on Success
 *
 */
Board_flash_STATUS Board_flashControl(Board_flashHandle handle,
                                      uint32_t          command,
                                      void              *args)
{
    Board_FlashInfo    *flashInfo;

    if (!handle)
    {
        return BOARD_FLASH_EFAIL;
    }

    flashInfo = (Board_FlashInfo *)handle;
    if (!flashInfo->flashHandle)
    {
        return BOARD_FLASH_EFAIL;
    }

#if defined (BOARD_NAND_FLASH_IN)
    NAND_STATUS status;

    if ((BOARD_FLASH_ID_W35N01JWTBAG == flashInfo->device_id))
    {
        status = NAND_control(flashInfo->flashHandle, command, args);
        if (NAND_PASS != status)
        {
            return (BOARD_FLASH_EFAIL);
        }
        else
        {
            return (BOARD_FLASH_EOK);
        }
    }
#endif

    return BOARD_FLASH_EUNSUPPORTED;
}


