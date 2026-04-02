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

/**
 * @file map.c
 * @brief Implementations of map management routines, algorithms are usually implemented elsewhere
 */

#include <stddef.h>
#include <string.h>

#include <perception/base/core.h>
#include <perception/base/map.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))

void copyConfig(const PTK_MapConfig *mapConfig, PTK_GridConfig *gridConfig)
{
    gridConfig->xCells    = mapConfig->xCells;
    gridConfig->yCells    = mapConfig->yCells;
    gridConfig->zCells    = mapConfig->zCells;
    gridConfig->xCellSize = mapConfig->xCellSize;
    gridConfig->yCellSize = mapConfig->yCellSize;
    gridConfig->zCellSize = mapConfig->zCellSize;
    gridConfig->xMin      = mapConfig->xMin;
    gridConfig->yMin      = mapConfig->yMin;
    gridConfig->zMin      = mapConfig->zMin;
}

uint32_t PTK_Map_getSize(const PTK_MapConfig *config)
{
    PTK_GridConfig gridConfig;
    uint32_t mem;
    uint32_t i;

    copyConfig(config, &gridConfig);

    // Base structure size
    mem = sizeof(PTK_Map);
    mem = align(mem, 8);

    // Plus size for each grid specified in configuration
    for (i = 0; i < 32; ++i)
    {
        if (config->grid[i].id)
        {
            gridConfig.type = config->grid[i].type;
            gridConfig.dataSize = config->grid[i].dataSize;
            mem += PTK_Grid_getSize(&gridConfig);
            mem = align(mem, 8);
        }
        else
        {
            break;
        }
    }

    return mem;
}

PTK_Map *PTK_Map_init(uint8_t *mem, const PTK_MapConfig *config)
{
    PTK_GridConfig  gridConfig;
    PTK_Map        *map;
    uint32_t        offset;
    uint32_t        i;

    copyConfig(config, &gridConfig);

    map    = (PTK_Map *) mem;
    offset = sizeof(PTK_Map);
    offset = align(offset, 8);

    map->config = *config;

    for (i = 0; i < 32; ++i)
    {
        if (config->grid[i].id)
        {
            uint8_t *m;

            gridConfig.type     = config->grid[i].type;
            gridConfig.dataSize = config->grid[i].dataSize;
            map->gridOffsets[i] = offset;
            m                   = (uint8_t *)mem + map->gridOffsets[i];

            PTK_Grid_init(m, &gridConfig);
            offset += PTK_Grid_getSize(&gridConfig);
            offset = align(offset, 8);
        }
        else
        {
            break;
        }
    }

    return map;
}

void PTK_Map_clear(PTK_Map *map)
{
    const PTK_GridInfo     *grid;
    const PTK_MapConfig    *config;
    uint32_t                i;

    config = &map->config;
    grid   = config->grid;

    for (i = 0; i < 32; ++i)
    {
        if (grid[i].id)
        {
            uint8_t   *m;
            PTK_Grid  *g;

            m = (uint8_t *)map;
            g = (PTK_Grid *)(m + map->gridOffsets[i]);

            PTK_Grid_clear(g);
        }
        else
        {
            break;
        }
    }
}

uint32_t PTK_Map_copy(PTK_Map *__restrict dst, const PTK_Map *__restrict src)
{
    const PTK_GridInfo     *grid;
    const PTK_MapConfig    *config;
    uint32_t                mapSize;
    uint32_t                i;

    // Verify configuration
    if(src->config.xCells != dst->config.xCells)
    {
        return(PTK_MAP_XCELL_COUNT_INVALID);
    }

    if(src->config.yCells != dst->config.yCells)
    {
        return(PTK_MAP_YCELL_COUNT_INVALID);
    }

    if(src->config.zCells != dst->config.zCells)
    {
        return(PTK_MAP_ZCELL_COUNT_INVALID);
    }

    config = &src->config;
    grid   = config->grid;

    for (i = 0; i < 32; ++i)
    {
        if (grid[i].id)
        {
            if (src->config.grid[i].id != dst->config.grid[i].id)
            {
                return(PTK_MAP_GRID_ID_INVALID);
            }

            if (src->config.grid[i].type != dst->config.grid[i].type)
            {
                return(PTK_MAP_GRID_DATA_TYPE_INVALID);
            }

            if (PTK_GRID_TYPE_USER == src->config.grid[i].type)
            {
                if (src->config.grid[i].dataSize !=
                    dst->config.grid[i].dataSize)
                {
                    return(PTK_MAP_CELL_DATA_SIZE_INVALID);
                }
            }
        }
        else
        {
            break;
        }
    }

    mapSize = PTK_Map_getSize(config);

    memcpy(dst, src, mapSize);

    return(PTK_MAP_OK);
}

uint32_t PTK_Map_has(const PTK_Map *map, uint32_t id)
{
    uint32_t i;

    for (i = 0; i < 32; ++i)
    {
        if (0 == map->config.grid[i].id)
        {
            return 0;
        }
        else if (id == map->config.grid[i].id)
        {
            return 1;
        }
    }

    return 0;
}

PTK_Grid *PTK_Map_get(const PTK_Map *map, uint32_t id)
{
    uint32_t i;

    for (i = 0; i < 32; ++i)
    {
        if (0 == map->config.grid[i].id)
        {
            return 0;
        }
        else if (id == map->config.grid[i].id)
        {
            uint8_t   *m;
            PTK_Grid  *g;

            m = (uint8_t *)map;
            g = (PTK_Grid *)(m + map->gridOffsets[i]);

            return g;
        }
    }

    return 0;
}

void PTK_Map_printDataStructSizes()
{
    PTK_printf("============== BEGIN: MAP OFFSET DETAILS ===============\n");

    PTK_printf("PTK_Map size                                     = %d\n", sizeof(PTK_Map));
    PTK_printf("    OFFSET(PTK_Map.config)                       = %d\n", offsetof(PTK_Map, config));
    PTK_printf("        OFFSET(PTK_Map.config.xCells)            = %d\n", offsetof(PTK_MapConfig, xCells));
    PTK_printf("        OFFSET(PTK_Map.config.yCells)            = %d\n", offsetof(PTK_MapConfig, yCells));
    PTK_printf("        OFFSET(PTK_Map.config.zCells)            = %d\n", offsetof(PTK_MapConfig, zCells));
    PTK_printf("        OFFSET(PTK_Map.config.xCellSize)         = %d\n", offsetof(PTK_MapConfig, xCellSize));
    PTK_printf("        OFFSET(PTK_Map.config.yCellSize)         = %d\n", offsetof(PTK_MapConfig, yCellSize));
    PTK_printf("        OFFSET(PTK_Map.config.zCellSize)         = %d\n", offsetof(PTK_MapConfig, zCellSize));
    PTK_printf("        OFFSET(PTK_Map.config.xMin)              = %d\n", offsetof(PTK_MapConfig, xMin));
    PTK_printf("        OFFSET(PTK_Map.config.yMin)              = %d\n", offsetof(PTK_MapConfig, yMin));
    PTK_printf("        OFFSET(PTK_Map.config.zMin)              = %d\n", offsetof(PTK_MapConfig, zMin));
    PTK_printf("        OFFSET(PTK_Map.config.grid)              = %d [total size = %d]\n",
               offsetof(PTK_MapConfig, grid), sizeof(PTK_GridInfo)*PTK_MAP_MAX_NUM_GRIDS_PER_MAP);
    PTK_printf("            OFFSET(PTK_Map.config.grid.id)       = %d\n", offsetof(PTK_GridInfo, id));
    PTK_printf("            OFFSET(PTK_Map.config.grid.type)     = %d\n", offsetof(PTK_GridInfo, type));
    PTK_printf("            OFFSET(PTK_Map.config.grid.dataSize) = %d\n", offsetof(PTK_GridInfo, dataSize));
    PTK_printf("    OFFSET(PTK_Map.gridOffsets)                  = %d [total size = %d]\n",
               offsetof(PTK_Map, gridOffsets), sizeof(size_t)*PTK_MAP_MAX_NUM_GRIDS_PER_MAP);

    PTK_printf("============== END: MAP OFFSET DETAILS =================\n\n");
}

void PTK_Map_print(const PTK_Map   *map)
{
    uint32_t    i;

    PTK_printf("************** BEGIN: MAP DETAILS ***************\n");

    PTK_printf("PTK_Map.config.xCells                       = %d\n", map->config.xCells);
    PTK_printf("PTK_Map.config.yCells                       = %d\n", map->config.yCells);
    PTK_printf("PTK_Map.config.zCells                       = %d\n", map->config.zCells);
    PTK_printf("PTK_Map.config.xCellSize                    = %f\n", map->config.xCellSize);
    PTK_printf("PTK_Map.config.yCellSize                    = %f\n", map->config.yCellSize);
    PTK_printf("PTK_Map.config.zCellSize                    = %f\n", map->config.zCellSize);
    PTK_printf("PTK_Map.config.xMin                         = %f\n", map->config.xMin);
    PTK_printf("PTK_Map.config.yMin                         = %f\n", map->config.yMin);
    PTK_printf("PTK_Map.config.zMin                         = %f\n", map->config.zMin);

    for (i = 0; i < PTK_MAP_MAX_NUM_GRIDS_PER_MAP; i++)
    {
        if (0 == map->config.grid[i].id)
        {
            break;
        }
        else
        {
            PTK_printf("    PTK_Map.config.grid.id[%d]               = %d\n", i, map->config.grid[i].id);
            PTK_printf("    PTK_Map.config.grid.type[%d]             = %d\n", i, map->config.grid[i].type);
            PTK_printf("    PTK_Map.config.grid.dataSize[%d]         = %d\n", i, map->config.grid[i].dataSize);
            PTK_printf("    PTK_Map.gridOffset[%d]                   = %d\n", i, map->gridOffsets[i]);
            PTK_printf("    --------------------------------\n");
        }
    }

    PTK_printf("************** END: MAP DETAILS *****************\n\n");
}

