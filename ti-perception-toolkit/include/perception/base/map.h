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
#pragma once
#ifndef PTK_MAP_H
#define PTK_MAP_H

/**
 * @file map.h
 * @brief A map is a aggregate of multiple grid structures storing different types of summary info
 *        about the environment, intended to simplify keeping track of all of them together
 */

#include <stdint.h>

#include <perception/base/grid.h>

/**
 * \defgroup group_ptk_map PTK Map
 * \ingroup group_ptk_mapping
  */

/**
 * \brief Maximum number of grids per Map - 1. One of the nodes will be used
 *        as a NULL node.
 */
#define PTK_MAP_MAX_NUM_GRIDS_PER_MAP   32U

#ifdef __cplusplus
extern "C" {
#endif

//! Error codes
/**
 * \brief Return status OK.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_OK                     ((uint32_t)0)

/**
 * \brief Invalid X cell count.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_XCELL_COUNT_INVALID     ((uint32_t)-1)

/**
 * \brief Invalid Y cell count.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_YCELL_COUNT_INVALID     ((uint32_t)-2)

/**
 * \brief Invalid Z cell count.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_ZCELL_COUNT_INVALID     ((uint32_t)-3)

/**
 * \brief Invalid grid ID.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_GRID_ID_INVALID         ((uint32_t)-4)

/**
 * \brief Invalid grid data type.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_GRID_DATA_TYPE_INVALID  ((uint32_t)-5)

/**
 * \brief Invalid grid element data size.
 *
 * \ingroup group_ptk_map
 */
#define PTK_MAP_CELL_DATA_SIZE_INVALID  ((uint32_t)-6)

/**
 * \brief Grid information.
 *
 * These PTK GridInfo structs are collected in an array and supplied as part of
 * the PTK MapConfig. A single PTK Map can have no more than
 * PTK_MAP_MAX_NUM_GRIDS_PER_MAP distinct PTK Grid members, however all
 * nonzero 32-bit integers are valid ids for a PTK Grid.
 *
 * \ingroup group_ptk_map
 */
typedef struct
{
    /** Identifier for the PTK_Grid, used in retrieval. The value 0 has a
     *  special meaning. It will be used as a NULL node.
     */
	uint32_t        id;

    /** Value for the type field to configure this PTK_Grid. */
	PTK_GridType    type;

    /** Value for dataSize field, used only if type is PTK_GRID_TYPE_USER. */
	uint32_t        dataSize;

} PTK_GridInfo;

/**
 * \brief Map configuration.
 *
 * The configuration of a PTK_Map is nearly the same as the configuration for a
 * PTK_Grid, but it has an additional member to specify the instance-specific
 * details of PTK Grids stored within the PTK_Map.
 *
 * \ingroup group_ptk_map
 */
typedef struct
{
    /** Number of cells along the x axis. */
    uint32_t            xCells;

    /** Number of cells along the y axis. */
    uint32_t            yCells;

    /** Number of cells along the z axis. If the z axis is not used, set this
     *  to 1.
     */
    uint32_t            zCells;

    /** Dimension of a cell along x in meters. */
    float               xCellSize;

    /** Dimension of a cell along y in meters. */
    float               yCellSize;

    /** Dimension of a cell along z in meters. If the z axis is not used, any
     *  nonzero value is valid.
     */
    float               zCellSize;

    /** Real x coordinate for start of cell 0. */
    float               xMin;

    /** Real y coordinate for start of cell 0. */
    float               yMin;

    /** Real z coordinate for start of cell 0. If the z axis is not used, any
     *  value is valid.
     */
    float               zMin;

    /** Grid parameters. The element with an 'id' 0 is treated as a NULL
     *  node.
     */
	PTK_GridInfo        grid[PTK_MAP_MAX_NUM_GRIDS_PER_MAP];

} PTK_MapConfig;

/**
 * \brief Map definition.
 *
 * The PTK_Map structure serves to aggregate multiple PTK_Grid structures that
 * describe the same real-world region using the same quantization of space.
 * Each constituent PTK_Grid may have a different data type stored in it, but
 * they are required to use identical coordinate systems. The cell indexing
 * schemes are therefore identical as well. The advantage of using a PTK_Map
 * container is that it provides a single, flexible handle for manipulating
 * multiple PTK_Grids at once, simplifying function signatures for processing.
 *
 * PTK_Map provides a system for optionally selecting which PTK_Grid members to
 * populate as well as querying which are available, in order to detect
 * configuration errors at runtime. The optional selection of PTK_Grids is done
 * to improve memory performance, as in practice each PTK_Grid for a suitable
 * region of space typically spans several MB.
 *
 * \ingroup group_ptk_map
 */
typedef struct
{
    /** Map configuration. */
    PTK_MapConfig   config;

    uint32_t        gridOffsets[PTK_MAP_MAX_NUM_GRIDS_PER_MAP];

} PTK_Map;

/**
 * \brief Compute how much memory is required to store the complete PTK_Map
 *        described by the given configuration.
 *
 * \param [in] config Configuration to determine memory requirements for.
 *
 * \return Amount of memory required, in bytes.
 *
 * \ingroup group_ptk_map
 */
uint32_t PTK_Map_getSize(const PTK_MapConfig *config);

/**
 * \brief Initialize an externally allocated piece of memory to be a valid
 *        PTK_Map. This also initializes all member PTK_Grids, so no further
 *        initialization is required.
 *
 * \param [out] mem Externally allocated memory of at least PTK_Map_getSize()
 *                  bytes.
 *
 * \param [in] config Configuration to guide initialization.
 *
 * \return
 *
 * \ingroup group_ptk_map
 */
PTK_Map * PTK_Map_init(uint8_t *mem, const PTK_MapConfig *config);

/**
 * \brief This clears all PTK_Grids internal to the map. It has the same result
 *        as if PTK_Grid clear were called on each individually.
 *
 *        NOTE: Currently there is no way to clear the user specific data.
 *
 * \param [inout] map The PTK_Map to be cleared.
 *
 * \ingroup group_ptk_map
 */
void PTK_Map_clear(PTK_Map *map);

/**
 * \brief Copy all constituent PTK_Grids from the given source PTK_Map to the
 *        destination PTK_Map. They must have the same PTK_MapConfig or an
 *        assertion failure is generated.
 *
 * \param [out] dst The PTK_Map to copy to.
 *
 * \param [in] src The PTK_Map to copy from.
 *
 * \return
 *         - The requested PTK_Grid, if record with matching Id is found.
 *         - NULL, otherwise
 *
 * \ingroup group_ptk_map
 */
uint32_t PTK_Map_copy(PTK_Map *__restrict dst, const PTK_Map *__restrict src);

/**
 * \brief Test if a PTK_Map contains a PTK_Grid with the specified id.
 *
 * \param [in] map PTK_Map to check.
 *
 * \param [in] id The ID to look for.
 *
 * \return
 *        - 1 if the given PTK_Map contains a PTK_Grid matching the given id
 *        - 0, otherwise
 *
 * \ingroup group_ptk_map
 */
uint32_t PTK_Map_has(const PTK_Map *map, uint32_t id);

/**
 * \brief Obtain a pointer to the PTK_Grid matching the specified id.
 *
 * \param [in] map PTK_Map to check.
 *
 * \param [in] id The ID to look for.
 *
 * \return
 *         - The requested PTK_Grid, if record with matching Id is found.
 *         - NULL, otherwise
 *
 * \ingroup group_ptk_map
 */
PTK_Grid * PTK_Map_get(const PTK_Map *map, uint32_t id);

#ifdef __cplusplus
}
#endif

#endif
