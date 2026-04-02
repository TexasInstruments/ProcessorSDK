/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <perception/base/grid.h>

#define align(x,b) ((x+(b-1))&~(b-1))

#define PTK_MAX_LINE_LEN        (256U)

uint32_t PTK_Grid_getSize(const PTK_GridConfig *config)
{
    uint32_t size, cellSize, blockSize;

	switch (config->type)
	{
		case PTK_GRID_TYPE_USER:
			cellSize = config->dataSize;
			break;
		case PTK_GRID_TYPE_FLOAT:
			cellSize = sizeof(float);
			break;
		case PTK_GRID_TYPE_UINT32:
		case PTK_GRID_TYPE_BITS:
		default:
			cellSize = sizeof(uint32_t);
            break;
	}

	blockSize = cellSize * config->xCells * config->yCells * config->zCells;

    // Make sure we don't overflow 32 bits in the memory request
    // This isn't a perfect check but catching the remaining false negatives is a lot of work
    if (blockSize < config->xCells
        || blockSize < config->yCells
        || blockSize < config->zCells)
    {
        return 0;
    }

    // Base structure size
    size = sizeof(PTK_Grid);
    size = align(size, 8);

    // Memory block for the data after the struct
    size += blockSize;
    size = align(size, 8);

    return size;
}

PTK_Grid *PTK_Grid_init(uint8_t *mem, const PTK_GridConfig *config)
{
    uint32_t offset = 0;
    PTK_Grid *og = (PTK_Grid *) mem;

	og->config = *config;

    offset = sizeof(PTK_Grid);
    offset = align(offset, 8);
    og->dataOffset = offset;
    og->dataCellCount = config->xCells * config->yCells * config->zCells;

    og->xInvCellSize = 1.f / config->xCellSize;
    og->yInvCellSize = 1.f / config->yCellSize;
    og->zInvCellSize = 1.f / config->zCellSize;

    PTK_Grid_clear(og);
    return og;
}

