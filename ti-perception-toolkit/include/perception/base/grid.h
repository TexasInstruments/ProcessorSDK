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
#ifndef PTK_GRID_H
#define PTK_GRID_H

/**
 * @file grid.h
 * @brief This defines the common grid functionality which is then used to construct occupancy grids
 *        that store a variety of data types
 */

/**
 * \defgroup group_ptk_grid PTK Grid
 * \ingroup group_ptk_mapping
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Error codes
/**
 * \brief Return status OK.
 *
 * \ingroup group_ptk_grid
 */
#define PTK_GRID_OK                     ((uint32_t)0)

/**
 * \brief Invalid value used for any computed cell that does nto fit on the map.
 *
 * \ingroup group_ptk_grid
 */
#define PTK_GRID_CELL_INVALID           ((uint32_t)-1)

/**
 * \brief Type specidied in the Gird config is not what is expected.
 *
 * \ingroup group_ptk_grid
 */
#define PTK_GRID_CONFIG_TYPE_INVALID    ((uint32_t)-2)

/**
 * \brief Grid cell count invalid.
 *
 * \ingroup group_ptk_grid
 */
#define PTK_GRID_CELL_COUNT_INVALID     ((uint32_t)-3)

/**
 * \brief Grid data type definition
 *
 * \ingroup group_ptk_grid
 */
typedef enum
{
    /** Values stored in grid cells are unsigned integers. */
    PTK_GRID_TYPE_UINT32,

    /** Values stored in grid cells are single precision floats. */
    PTK_GRID_TYPE_FLOAT,

    /** Values stored in grid cells 32-bit wide bit fields. */
    PTK_GRID_TYPE_BITS,

    /** Values stored in grid cells are user defined data. */
	PTK_GRID_TYPE_USER,

    /** Values to force the size to 32 bits. */
	PTK_GRID_TYPE_USER_FORCE_SIZE = 0xFFFFFFFF

} PTK_GridType;

/**
 * \brief Grid configuration.
 *
 * The configuration structure itself also requires information about the
 * spatial organization of the grid and, if the PTK_GRID_TYPE_USER type is
 * used, the size of the user data to store per cell. A 2D vs 3D grid is
 * differentiated by the value for zCells, where a value of 1 indicates
 * that the PTK_Grid will only be used with the 2D API, while a value > 1
 * indicates that the PTK_Grid will only be used with the 3D API.
 *
 * PTK_Grid structures are configured once based on their dimension and the type
 * of data they store. After allocation and initialization, the type of data
 * stored within a PTK_Grid cannot be modified safely.
 *
 * The configuration structure itself also requires information about the
 * spatial organization of the grid and, if the PTK_GRID_TYPE_USER type is
 * used, the size of the user data to store per cell. A 2D vs 3D grid is
 * differentiated by the value for zCells, where a value of 1 indicates
 * that the PTK_Grid will only be used with the 2D API, while a value > 1
 * indicates that the PTK_Grid will only be used with the 3D API.
 *
 * \ingroup group_ptk_grid
 */
typedef struct
{
    /** The type of grid to allocate, based on the above constants. */
    PTK_GridType        type;

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

    /** Size of memory required to hold a value for a cell if
     *  PTK_GRID_TYPE_USER is specified for type. Otherwise, ignored.
     */
	uint32_t            dataSize;

} PTK_GridConfig;

/**
 * \brief Grid Meta Data
 *
 * \ingroup group_ptk_grid
 */
typedef struct
{
    /** Smallest X grid index updated. */
    uint32_t            minUpdatedCellX;
     
    /** Smallest Y grid index updated. */
    uint32_t            minUpdatedCellY;

    /** Largest X grid index updated. */
    uint32_t            maxUpdatedCellX;

    /** Largest Y grid index updated. */
    uint32_t            maxUpdatedCellY; 

}PTK_GridMetaData;

/**
 * \brief Grid definition
 *
 * \ingroup group_ptk_grid
 */
typedef struct
{
    /** Configuration information describing the grid organization. */
    PTK_GridConfig      config;

    /** 1/xCellSize from config data. */
    float               xInvCellSize;

    /** 1/yCellSize from config data. */
    float               yInvCellSize;

    /** 1/zCellSize from config data. */
    float               zInvCellSize;

    /** Number of cells in the data array. */
    uint32_t            dataCellCount;

    /** Offset of the data field in bytes, relative to the struct. */
    uint32_t            dataOffset;

    /** Holds meta data about grid. */
     PTK_GridMetaData   metaData;

} PTK_Grid;

/**
 * \brief Defines a rectangle in a target coordinate system.
 *        In local coordinate system, the vertices are
 *        (0|0), (xWidth|0), (xWidth|yWidth), (0|yWidth).
 *
 *        For example, if the target coordinate system is the local system,
 *        then xOffset = yOffset = 0 and xDir = 1, yDir = 0
 *
 * \ingroup group_ptk_grid
 */
typedef struct
{
    /** Rectangle's local (0|0) vertex in target coordinates (x-component). */
    float   xOffset;

    /** Rectangle's local (0|0) vertex in target coordinates (y-component). */
    float   yOffset;

    /** Rectangle's width along rectangle's local x-axis. */
    float   xWidth;

    /** Rectangle's width along rectangle's local y-axis. */
    float   yWidth;

    /** Direction of rectangle's x-axis in target coordinates (x-component). */
    float   xDir;

    /** Direction of rectangle's x-axis in target coordinates (y-component). */
    float   yDir;

} PTK_GridRoi;

/**
 * \brief Compute how much memory is required to store the complete PTK Grid
 *        described by the given configuration struct.
 *
 * \param [in] config Configuration to determine memory size for.
 *
 * \return Amount of memory required, in bytes.
 *
 * \ingroup group_ptk_grid
 */
uint32_t PTK_Grid_getSize(const PTK_GridConfig *config);

/**
 * \brief Initialize an externally allocated piece of memory to be a valid PTK
 *        Grid instance.
 *
 * \param [out] mem Externally allocated memory of at PTK_Grid_getSize() bytes.
 *
 * \param [in] config Configuration to initialize for Pointer to newly
 *             initialized PTK Grid instance.
 *
 * \return
 *
 * \ingroup group_ptk_grid
 */
PTK_Grid * PTK_Grid_init(uint8_t *mem, const PTK_GridConfig *config);

/**
 * \brief This clears a PTK_Grid, resetting all cell contents back to 0,
 *        including user data types. If a nonzero default value is required
 *        for user types, PTK_Grid userClear is recommended.
 *
 * \param [inout] og The PTK Grid to clear.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_clear(PTK_Grid *og)
{
    switch (og->config.type)
    {
        default:
        case PTK_GRID_TYPE_UINT32:
        case PTK_GRID_TYPE_BITS:
            {
                uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
                memset(ui32, 0, sizeof(uint32_t) * og->dataCellCount);
            }
            break;
        case PTK_GRID_TYPE_FLOAT:
            {
                float *f32 = (float *)((uint8_t*)og + og->dataOffset);
                memset(f32, 0, sizeof(float) *og->dataCellCount);
            }
            break;
        case PTK_GRID_TYPE_USER:
            {
                uint8_t *user = (uint8_t *)og + og->dataOffset;
                memset(user, 0, og->config.dataSize * og->dataCellCount);
            }
            break;
    }
}


/**
 * \brief This clears a PTK Grid that is storing user data, resetting it to the
 *        default data supplied. The size of the default data is propagated
 *        from the existing definition for user data size already known to the
 *        PTK_Grid. Note that this function may be significantly slower than
 *        PTK_Grid clear due to implementation constraints.
 *
 * \param [inout] og The PTK Grid to clear.
 *
 * \param [out] def The default data to use when resetting each cell.
 *
 * \return
 *         - PTK_GRID_OK, if successful
 *         - PTK_GRID_CONFIG_TYPE_INVALID, otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_userClear(PTK_Grid *og, void *def)
{
    uint8_t   *user;
    uint32_t   i;

    if(PTK_GRID_TYPE_USER != og->config.type)
    {
        return(PTK_GRID_CONFIG_TYPE_INVALID);
    }

    user = (uint8_t *)og + og->dataOffset;

    for (i = 0; i < og->dataCellCount; ++i)
    {
        memcpy(user + (i*og->config.dataSize), def, og->config.dataSize);
    }

    return(PTK_GRID_OK);
}

/**
 * \brief Copy data from the given source PTK Grid to the destination PTK_Grid.
 *        The two must have the same configuration and be of the same type, or
 *        the results will be undefined.
 *
 * \param [out] dst Copy data to this PTK_Grid.
 *
 * \param [in] src Copy data from this PTK_Grid.
 *
 * \return
 *         - PTK_GRID_OK, if successful
 *         - Appropriate error code, otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_copy(PTK_Grid *__restrict dst, const PTK_Grid *__restrict src)
{
    void      *srcUser;
    void      *dstUser;
    uint32_t   size;

    if(src->config.type != dst->config.type)
    {
        return(PTK_GRID_CONFIG_TYPE_INVALID);
    }

    if(src->dataCellCount != dst->dataCellCount)
    {
        return(PTK_GRID_CELL_COUNT_INVALID);
    }

    switch(src->config.type)
    {
        default:
        case PTK_GRID_TYPE_UINT32:
        case PTK_GRID_TYPE_BITS:
            size = sizeof(uint32_t);
            break;
        case PTK_GRID_TYPE_FLOAT:
            size = sizeof(float);
            break;
        case PTK_GRID_TYPE_USER:
            size = src->config.dataSize;
            break;
    }

    srcUser = (uint8_t *)src + src->dataOffset;
    dstUser = (uint8_t *)dst + dst->dataOffset;

    memcpy(dstUser, srcUser, size * src->dataCellCount);

    return(PTK_GRID_OK);
}

static inline uint32_t computeCell(float c, float min, float invsize, uint32_t count)
{
    uint32_t cell = PTK_GRID_CELL_INVALID;

    if (c >= min)
    {
        cell = (uint32_t) ((c - min) * invsize);
    }

    if (cell >= count)
    {
        cell = PTK_GRID_CELL_INVALID;
    }

    return cell;
}

/**
 * \brief This converts a real-valued x coordinate given in meters to an index
 *        for the grid access function. If the x coordinate is invalid (< the
 *        origin x coordinate or  the total number of cells in the x
 *        direction), PTK_GRID_CELL_INVALID is produced instead.
 *
 * \param [in] og PTK_Grid to compute a cell index for.
 *
 * \param [in] x Real x coordinate in meters.
 *
 * \return
 *         - Cell index containing the given coordinate, if success
 *         - PTK_GRID_CELL_INVALID, otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getXCell(const PTK_Grid *og, float x)
{
    return computeCell(x, og->config.xMin, og->xInvCellSize, og->config.xCells);
}

/**
 * \brief This converts a real-valued y coordinate given in meters to an index
 *        for the grid access function. If the y coordinate is invalid (< the
 *        origin y coordinate or  the total number of cells in the y
 *        direction), PTK_GRID_CELL_INVALID is produced instead.
 *
 * \param [in] og PTK_Grid to compute a cell index for.
 *
 * \param [in] y Real y coordinate in meters.
 *
 * \return
 *         - Cell index containing the given coordinate, if success
 *         - PTK_GRID_CELL_INVALID, otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getYCell(const PTK_Grid *og, float y)
{
    return computeCell(y, og->config.yMin, og->yInvCellSize, og->config.yCells);
}

/**
 * \brief This converts a real-valued z coordinate given in meters to an index
 *        for the grid access function. If the z coordinate is invalid (< the
 *        origin z coordinate or  the total number of cells in the z
 *        direction), PTK_GRID_CELL_INVALID is produced instead.
 *
 * \param [in] og PTK_Grid to compute a cell index for.
 *
 * \param [in] z Real z coordinate in meters.
 *
 * \return
 *         - Cell index containing the given coordinate, if success
 *         - PTK_GRID_CELL_INVALID, otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getZCell(const PTK_Grid *og, float z)
{
    return computeCell(z, og->config.zMin, og->zInvCellSize, og->config.zCells);
}

/**
 * \brief This is a mostly-internal function that is used for converting a pair
 *        of integer cell indices into a single linear index that is then used
 *        with the backing storage internal to the PTK_Grid. It handles the
 *        storage order (row-major vs. column-major) as necessary and makes the
 *        assumption that z coordinates are entirely unused.
 *
 * \param [in] og PTK_Grid to compute a linear index for.
 *
 * \param [in] x Integer x index.
 *
 * \param [in] y Integer y index.
 *
 * \return Linear index of the given cell in the backing storage array.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getLinearIndex2d(const PTK_Grid *og, uint32_t x, uint32_t y)
{
    return x + og->config.xCells * y;
}

/**
 * \brief This is a mostly-internal function that is used for converting a
 *        triple of integer cell indices into a single linear index that is
 *        used with the backing storage internal to the PTK_Grid. It handles
 *        the storage order (row-, column-, or tube-major) as necessary.
 *
 * \param [in] og PTK_Grid to compute a linear index for.
 *
 * \param [in] x Integer x index.
 *
 * \param [in] y Integer y index.
 *
 * \param [in] z Integer z index.
 *
 * \return Linear index of the given cell in the backing storage array.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getLinearIndex3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    return x + og->config.xCells * (y + og->config.yCells * z);
}

/**
 * \brief
 *
 * \param [in] og
 *
 * \param [in] x
 *
 *
 * \return
 *
 * \ingroup group_ptk_grid
 */
static inline float PTK_Grid_getX(const PTK_Grid *og, uint32_t x)
{
    return og->config.xMin + (float)x * og->config.xCellSize;
}

/**
 * \brief
 *
 * \param [in] og
 *
 * \param [in] y
 *
 *
 * \return
 *
 * \ingroup group_ptk_grid
 */
static inline float PTK_Grid_getY(const PTK_Grid *og, uint32_t y)
{
    return og->config.yMin + (float)y * og->config.yCellSize;
}

/**
 * \brief
 *
 * \param [in] og
 *
 * \param [in] z
 *
 *
 * \return
 *
 * \ingroup group_ptk_grid
 */
static inline float PTK_Grid_getZ(const PTK_Grid *og, uint32_t z)
{
    return og->config.zMin + (float)z * og->config.zCellSize;
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_FLOAT in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] value The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setf2d(PTK_Grid *og, uint32_t x, uint32_t y, float value)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        f32[cell] = value;
    }
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_UINT32 in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] value The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setu2d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t value)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] = value;
    }
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_BITS in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] bits The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setb2d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] |= bits;
    }
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_FLOAT in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline float PTK_Grid_getf2d(const PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        return f32[cell];
    }

    return FLT_MAX;
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_UINT32 in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getu2d(const PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return ui32[cell];
    }

    return (uint32_t) -1;
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_BITS in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getb2d(const PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return ui32[cell];
    }

    return 0;
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_FLOAT in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \param [in] value The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setf3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, float value)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        f32[cell] = value;
    }
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_UINT32 in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \param [in] value The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setu3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, uint32_t value)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] = value;
    }
}

/**
 * \brief Sets the data of type PTK_GRID_TYPE_BITS in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \param [in] bits The value to be set at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_setb3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] |= bits;
    }
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_FLOAT in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline float PTK_Grid_getf3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        return f32[cell];
    }

    return FLT_MAX;
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_UINT32 in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getu3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return ui32[cell];
    }

    return (uint32_t) -1;
}

/**
 * \brief Returns the data of type PTK_GRID_TYPE_BITS in the specified cell.
 *
 * \param [in] og The PTK_Grid to read.
 *
 * \param [in] x The integer x index of the cell to read.
 *
 * \param [in] y The integer y index of the cell to read.
 *
 * \param [in] z The integer z index of the cell to read.
 *
 * \return The value stored at the given cell indices.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_getb3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return ui32[cell];
    }

    return 0;
}

/**
 * \brief Tests if the flags set for the specified cell contain any of the
 *        given flags values. Returns 1 if any of the flags match.
 *
 * \param [in] og PTK_Grid to check
 *
 * \param [in] x The integer x cell index to check
 *
 * \param [in] y The integer y cell index to check
 *
 * \param [in] bits The flags to check.
 *
 * \return
 *         - 1 if a match was found
 *         - 0 otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_anyb2d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return (ui32[cell] & bits) > 0;
    }

    return 0;
}

/**
 * \brief Tests if the flags set for the specified cell contain any of the
 *        given flags values. Returns 1 if any of the flags match.
 *
 * \param [in] og PTK_Grid to check
 *
 * \param [in] x The integer x cell index to check
 *
 * \param [in] y The integer y cell index to check
 *
 * \param [in] z The integer z cell index to check
 *
 * \param [in] bits The flags to check.
 *
 * \return
 *         - 1 if a match was found
 *         - 0 otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_anyb3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return (ui32[cell] & bits) > 0;
    }

    return 0;
}

/**
 * \brief Test if all of the flags set for the specified cell match all of the
 *        given flags. Returns 0 if any of them are missing.
 *
 * \param [in] og The PTK_Grid to check.
 *
 * \param [in] x The integer x cell index to check.
 *
 * \param [in] y The integer y cell index to check.
 *
 * \param [in] bits The flags to check for.
 *
 * \return
 *         - 1 if all flags match
 *         - 0 otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_allb2d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return (ui32[cell] & bits) == bits;
    }

    return 0;
}

/**
 * \brief Test if all of the flags set for the specified cell match all of the
 *        given flags. Returns 0 if any of them are missing.
 *
 * \param [in] og The PTK_Grid to check.
 *
 * \param [in] x The integer x cell index to check.
 *
 * \param [in] y The integer y cell index to check.
 *
 * \param [in] z The integer z cell index to check.
 *
 * \param [in] bits The flags to check for.
 *
 * \return
 *         - 1 if all flags match
 *         - 0 otherwise
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t PTK_Grid_allb3d(const PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return (ui32[cell] & bits) == bits;
    }

    return 0;
}

/**
 * \brief This removes the specified flags from the specified cell. Any flags
 *        not mentioned in flags are left.
 *
 *        If the linear index matching the specified cell is out of bounds for
 *        this grid.
 *
 * \param [inout] og The PTK_Grid to modify.
 *
 * \param [in] x The integer x cell index to modify.
 *
 * \param [in] y The integer y cell index to modify.
 *
 * \param [in] bits The flags to remove from this cell.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_clearb2d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] &= ~bits;
    }
}

/**
 * \brief This removes the specified flags from the specified cell. Any flags
 *        not mentioned in flags are left.
 *
 *        If the linear index matching the specified cell is out of bounds for
 *        this grid.
 *
 * \param [inout] og The PTK_Grid to modify.
 *
 * \param [in] x The integer x cell index to modify.
 *
 * \param [in] y The integer y cell index to modify.
 *
 * \param [in] z The integer z cell index to modify.
 *
 * \param [in] bits The flags to remove from this cell.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_Grid_clearb3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z, uint32_t bits)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        ui32[cell] &= ~bits;
    }
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_FLOAT in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline float *PTK_Grid_reff2d(PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        return &f32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_FLOAT in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \param [in] z The integer z index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline float *PTK_Grid_reff3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        float *f32 = (float *)((uint8_t*)og + og->dataOffset);
        return &f32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_UINT32 in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t *PTK_Grid_refu2d(PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return &ui32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_UINT32 in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \param [in] z The integer z index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t *PTK_Grid_refu3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return &ui32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_BITS in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t *PTK_Grid_refb2d(PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return &ui32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_BITS in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \param [in] z The integer z index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline uint32_t *PTK_Grid_refb3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
    uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

    if (cell < og->dataCellCount)
    {
        uint32_t *ui32 = (uint32_t *)((uint8_t*)og + og->dataOffset);
        return &ui32[cell];
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_USER in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline void *PTK_Grid_refv2d(PTK_Grid *og, uint32_t x, uint32_t y)
{
    uint32_t cell = PTK_Grid_getLinearIndex2d(og, x, y);

    if (cell < og->dataCellCount)
    {
        uint8_t *user = (uint8_t *)og + og->dataOffset;
        return (void *)(user + (cell * og->config.dataSize));
    }

    return 0;
}

/**
 * \brief Returns a reference to the data of type PTK_GRID_TYPE_USER in the
 *        specified cell.
 *
 * \param [in] og The PTK_Grid to reference.
 *
 * \param [in] x The integer x index of the cell to reference.
 *
 * \param [in] y The integer y index of the cell to reference.
 *
 * \param [in] z The integer z index of the cell to reference.
 *
 * \return
 *         - A pointer to the contents of the cell, if the linear index is
 *           valid.
 *         - NULL, otherwise.
 *
 * \ingroup group_ptk_grid
 */
static inline void *PTK_Grid_refv3d(PTK_Grid *og, uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t cell = PTK_Grid_getLinearIndex3d(og, x, y, z);

	if (cell < og->dataCellCount)
    {
        uint8_t *user = (uint8_t *)og + og->dataOffset;
		return (void *)(user + (cell * og->config.dataSize));
    }

	return 0;
}

/**
 * \brief Sets the default values for the ROI. The values are set as follows:
 *        - xOffset = 0.0f
 *        - yOffset = 0.0f
 *        - xWidth  = 1.0f
 *        - yWidth  = 1.0f
 *        - xDir    = 1.0f
 *        - yDir    = 0.0f
 *
 * \param [out] roi ROI parameter context.
 *
 * \ingroup group_ptk_grid
 */
static inline void PTK_GridRoi_setDefault(PTK_GridRoi * roi)
{
    roi->xOffset = 0.f;
    roi->yOffset = 0.f;
    roi->xWidth  = 1.f;
    roi->yWidth  = 1.f;
    roi->xDir    = 1.f;
    roi->yDir    = 0.f;
}

#ifdef __cplusplus
}
#endif

#endif
