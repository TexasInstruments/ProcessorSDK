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
#ifndef PTK_GRID_ITER_H
#define PTK_GRID_ITER_H

#include <stdint.h>
#include <perception/base/grid.h>

/**
 * \defgroup group_ptk_grid_iter PTK Grid Iterator
 * \ingroup group_ptk_mapping
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Grid circle iterators are intended to be mostly opaque structures
 *        which can produce a sequence of (x,y) pairs covering the interior
 *        of a circle using a minimal number of operations aggregated across
 *        the entire iteration. It does not produce any points outside the
 *        circle. Grid circle iterators operates on cell coordinates.
 *
 * \ingroup group_ptk_grid_iter
 */
typedef struct
{
    /** Grid context. */
    const PTK_Grid    * grid;

    /** Region Of Interest parameters. */
    const PTK_GridRoi * roi;

    /** Center x coordinate of the circle. */
    int32_t             cx;

    /** Center y coordinate of the circle. */
    int32_t             cy;

    /** Circle radius in pixels. */
    int32_t             r;

    /** Compute circle radius squared. */
    int32_t             rsq;

    /** Current iterator x coordinate. */
    int32_t             cellX;

    /** Current iterator y coordinate. */
    int32_t             cellY;

    /** Min y value for current column. */
    int32_t             minY;

    /** Max y value for current column. */
    int32_t             maxY;

    /** Is the iterator valid to produce a new result?. */
    int32_t             valid;

} PTK_GridCircleIter;

/**
 * \brief Initializes the grid iterator.
 *
 * \param [out] iter Iterator context.
 *
 * \param [in] grid Grid configuration.
 *
 * \param [in] roi ROI configuration.
 *
 * \param [in] cx Center x coordinate of the circle.
 *
 * \param [in] cy Center y coordinate of the circle.
 *
 * \param [in] r Circle radius in pixels.
 *
 * \ingroup group_ptk_grid_iter
 */
void PTK_GridCircleIterInit(PTK_GridCircleIter *iter,
                            const PTK_Grid     *grid,
                            const PTK_GridRoi  *roi,
                            int32_t             cx,
                            int32_t             cy,
                            uint32_t            r);

/**
 * \brief Returns the next cell inside the region of interest.
 *
 * \param [out] iter Iterator context.
 *
 * \param [out] cellX Center x coordinate of the cell found.
 *
 * \param [out] cellY Center y coordinate of the cell found.
 *
 * \return
 *         - 1, if a new cell is found
 *         - 0, otherwise
 *
 * \ingroup group_ptk_grid_iter
 */

uint32_t PTK_GridCircleIterNext(PTK_GridCircleIter *iter,
                                uint32_t           *cellX,
                                uint32_t           *cellY);


/**
 * \brief Grid box (rectangle) iterators produce a sequence of the cell
 *        coordinates which lie inside a rectangle defined in physical
 *        grid coordinates.
 *
 * \ingroup group_ptk_grid_iter
 */
typedef struct
{
    /** Grid context. */
    const PTK_Grid    * grid;

    /** Box parameters with target coordinates being the grid's physical
     *  coordinate system.
     */
    const PTK_GridRoi * roi;

    /** Current iterator x coordinate. */
    uint32_t            cellX;

    /** Current iterator y coordinate. */
    uint32_t            cellY;

    /** Iteration limits (bounding box in cell coordinates). */
    uint32_t            minCellX;

    /** Iteration limits (bounding box in cell coordinates). */
    uint32_t            minCellY;

    /** Iteration limits (bounding box in cell coordinates). */
    uint32_t            maxCellX;

    /** Iteration limits (bounding box in cell coordinates). */
    uint32_t            maxCellY;

    /** Translation component for taking grid coordinates to normalized box
     *  coordinates.
     */
    float               cx;

    /** Translation component for taking grid coordinates to normalized box
     *  coordinates.
     */
    float               cy;

    /** Rotation component taking grid coordinates to normalized box
     *  coordinates.
     */
    float               utx;

    /** Rotation component taking grid coordinates to normalized box
     *  coordinates.
     */
    float               uty;

    /** Rotation component taking grid coordinates to normalized box
     *  coordinates. */
    float               vtx;

    /** Rotation component taking grid coordinates to normalized box
     *  coordinates.
     */
    float               vty;

    /** Is the iterator valid to produce a new result?. */
    int32_t             valid;

} PTK_GridIter_PhysBox;

/**
 * \brief Initialize the iterator
 *
 * \param[out]  iter   Pointer to iterator
 *
 * \param[in]   grid   Pointer to grid
 *
 * \param[in]   roi    Pointer to ROI defining the box in physical grid coordinates
 *
 * \ingroup group_ptk_grid_iter
 */
void PTK_GridIter_PhysBoxInit(PTK_GridIter_PhysBox *iter,
                              const PTK_Grid       *grid,
                              const PTK_GridRoi    *roi);
/**
 * \brief Get next cell from iterator. When no more cells are available, return 0.
 *
 * \param[inout] iter   Pointer to iterator
 *
 * \param[out] cellX  Pointer to next cell's index in x
 *
 * \param[out] cellY  Pointer to next cell's index in y
 *
 * \return
 *         - 1 if next cell was provided
 *         - 0 if no more cells are available
 *
 * \ingroup group_ptk_grid_iter
 */
uint32_t PTK_GridIter_PhysBoxGetNext(PTK_GridIter_PhysBox *iter,
                                     uint32_t             *cellX,
                                     uint32_t             *cellY);

#ifdef __cplusplus
}
#endif

#endif
