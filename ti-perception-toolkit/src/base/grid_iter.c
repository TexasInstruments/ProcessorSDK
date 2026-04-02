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
 * @file grid_iter.c
 * @brief Implementation of the iterators for grids.
 */

#include <string.h>
#include <math.h>
#include <float.h>

#include <perception/base/core.h>
#include <perception/base/grid_iter.h>
#include <perception/base/point.h>

void PTK_GridCircleIterInit(PTK_GridCircleIter *iter,
                            const PTK_Grid     *grid,
                            const PTK_GridRoi  *roi,
                            int32_t             cx,
                            int32_t             cy,
                            uint32_t            r)
{
 	iter->grid = grid;
 	iter->roi  = roi;
 	iter->cx   = cx;
 	iter->cy   = cy;
 	iter->r    = r;
 	iter->rsq  = r*r;

	if ((cx - (int32_t)r) < 0)
    {
		iter->cellX = 0;
    }
	else
    {
		iter->cellX = cx - r;
    }

	iter->cellY = cy;
	iter->minY  = cy;
	iter->maxY  = cy;
	iter->valid = 1;

    return;

} // End of PTK_GridCircleIterInit()

uint32_t PTK_GridCircleIterNext(PTK_GridCircleIter *iter,
                                uint32_t           *cellX,
                                uint32_t           *cellY)
{
	// Do nothing with invalid iterators
	if (iter->valid)
	{
		*cellX = iter->cellX;
		*cellY = iter->cellY;

		iter->cellY += 1;

		// Check if we're moving to the next vertical stripe
		if (iter->cellY > iter->maxY)
		{
			// Compute the bounds for the next stripe
			iter->cellX += 1;

			// This was the last valid stripe so we're done
			if (iter->cellX > iter->cx + iter->r)
			{
				iter->valid = 0;
			}
			else
			{
				int32_t xsq = (iter->cx - iter->cellX);
				xsq = xsq * xsq;

				// Move minY down such that minY is outside r but minY+1 is inside
				while ((iter->minY > 0)
				       && (xsq + (iter->cy - iter->minY)*(iter->cy - iter->minY) <= iter->rsq))
				{
					iter->minY -= 1;
				}

				// Move minY up such that minY is outside r but minY+1 is inside
				while ((iter->minY < iter->cy)
				       && (xsq + (iter->cy - iter->minY - 1)*(iter->cy - iter->minY - 1) > iter->rsq))
				{
					iter->minY += 1;
				}

				// Find the appropriate maxY from minY
				iter->maxY = iter->cy + iter->cy - iter->minY;
				iter->cellY = iter->minY;
			}
		}

		return 1;
	}

	return 0;

} // End of PTK_GridCircleIterNext()

/**
 * Helper function to split two floats into min/max pairs
 * @param[in] a Pair to compare, first value
 * @param[in] b Pair to compare, second value
 * @param[out] min Min of pair written here
 * @param[out] max Max of pair written here
 */
static void splitMinMax(float a, float b, float *min, float *max)
{
    if (a < b)
    {
        *min = a;
        *max = b;
    }
    else
    {
        *min = b;
        *max = a;
    }
}

static void physBoxStep(PTK_GridIter_PhysBox *iter)
{
    float cellU = -1.f;
    float cellV = -1.f;
    float cellRealX, cellRealY;

	while (cellU < 0.0f || cellU > 1.0f || cellV < 0.0f || cellV > 1.0f)
	{
		iter->cellY += 1;

		if (iter->cellY > iter->maxCellY) //move to next X
		{
			iter->cellY = iter->minCellY;
			iter->cellX += 1;
			if (iter->cellX > iter->maxCellX) //no more points
			{
				iter->valid = 0;
				return;
			}
		}

		cellRealX = PTK_Grid_getX(iter->grid, iter->cellX);
		cellRealY = PTK_Grid_getY(iter->grid, iter->cellY);
		cellU = iter->utx * (cellRealX - iter->cx) + iter->uty * (cellRealY - iter->cy);
		cellV = iter->vtx * (cellRealX - iter->cx) + iter->vty * (cellRealY - iter->cy);
	}

	return;
}

void PTK_GridIter_PhysBoxInit(PTK_GridIter_PhysBox *iter,
                              const PTK_Grid       *grid,
                              const PTK_GridRoi    *roi)
{
    PTK_Point aabbMin, aabbMax;
    float mx1, mx2, mx3, mx4;
    float my1, my2, my3, my4;
    float pmin1, pmin2, pmax1, pmax2;
    float invdet;
    float dirNorm;
    float dx, dy;
    float rdx, rdy;
    uint32_t minCellX, minCellY, maxCellX, maxCellY;
    float cellU = -1.f;
    float cellV = -1.f;
    float cellRealX, cellRealY;

    iter->grid = grid;
    iter->roi = roi;

    /* compute box vertices in phys grid coordinates */
    dirNorm = sqrtf(roi->xDir*roi->xDir + roi->yDir*roi->yDir);
    if (0 == dirNorm || roi->xWidth <= 0 || roi->yWidth <= 0)
    {
        PTK_printf("WARNING: Invalid Roi!\n");
        iter->valid = 0;
        return;
    }
    dx = roi->xDir/dirNorm;
    dy= roi->yDir/dirNorm;
    rdx = -dy;
    rdy = dx;

    mx1 = roi->xOffset;
    my1 = roi->yOffset;
    mx2 = mx1 + roi->xWidth * dx;
    my2 = my1 + roi->xWidth * dy;
    mx3 = mx2 + roi->yWidth * rdx;
    my3 = my2 + roi->yWidth * rdy;
    mx4 = mx3 - roi->xWidth * dx;
    my4 = my3 - roi->xWidth * dy;

    iter->cx = mx1;
    iter->cy = my1;

    /* Find the min/max coordinates of each type for the AABB */
    splitMinMax(mx1, mx2, &pmin1, &pmax1);
    splitMinMax(mx3, mx4, &pmin2, &pmax2);
    splitMinMax(pmin1, pmin2, &aabbMin.x, &pmin1);
    splitMinMax(pmax1, pmax2, &pmax1, &aabbMax.x);

    splitMinMax(my1, my2, &pmin1, &pmax1);
    splitMinMax(my3, my4, &pmin2, &pmax2);
    splitMinMax(pmin1, pmin2, &aabbMin.y, &pmin1);
    splitMinMax(pmax1, pmax2, &pmax1, &aabbMax.y);

    /* Construct inverse matrix to solve for the coordinates of a point in the rectangle's system */
    invdet = 1.0f / ((mx2 - mx1)*(my4-my1) - (my2 - my1)*(mx4-mx1));
    iter->utx = (my4-my1) * invdet;
    iter->uty = (mx1-mx4) * invdet;
    iter->vtx = (my1-my2) * invdet;
    iter->vty = (mx2-mx1) * invdet;

    /* Convert AABB coordinates to integer cell coordinates*/
    minCellX = PTK_Grid_getXCell(grid, aabbMin.x);
    minCellY = PTK_Grid_getYCell(grid, aabbMin.y);
    maxCellX = PTK_Grid_getXCell(grid, aabbMax.x);
    maxCellY = PTK_Grid_getYCell(grid, aabbMax.y);

    iter->minCellX = minCellX;
    iter->minCellY = minCellY;
    iter->maxCellX = maxCellX;
    iter->maxCellY = maxCellY;

    /* Truncate cells that not on the grid */
    if (PTK_GRID_CELL_INVALID == minCellX)
    {
        iter->minCellX = 0;
    }
    if (PTK_GRID_CELL_INVALID == minCellY)
    {
        iter->minCellY = 0;
    }
    if (PTK_GRID_CELL_INVALID == maxCellX)
    {
        iter->maxCellX = grid->config.xCells-1;
    }
    if (PTK_GRID_CELL_INVALID == maxCellY)
    {
        iter->maxCellY = grid->config.yCells-1;
    }

    iter->cellX = iter->minCellX;
    iter->cellY = iter->minCellY;

	// Do steps until we find the first valid coordinate
	cellRealX = PTK_Grid_getX(iter->grid, iter->cellX);
	cellRealY = PTK_Grid_getY(iter->grid, iter->cellY);
	cellU = iter->utx * (cellRealX - iter->cx) + iter->uty * (cellRealY - iter->cy);
	cellV = iter->vtx * (cellRealX - iter->cx) + iter->vty * (cellRealY - iter->cy);
	while (cellU < 0.0f || cellU > 1.0f || cellV < 0.0f || cellV > 1.0f)
	{
		iter->cellY += 1;

		if (iter->cellY > iter->maxCellY) //move to next X
		{
			iter->cellY = iter->minCellY;
			iter->cellX += 1;
			if (iter->cellX > iter->maxCellX) //no more points
			{
				iter->valid = 0;
				return;
			}
		}

		cellRealX = PTK_Grid_getX(iter->grid, iter->cellX);
		cellRealY = PTK_Grid_getY(iter->grid, iter->cellY);
		cellU = iter->utx * (cellRealX - iter->cx) + iter->uty * (cellRealY - iter->cy);
		cellV = iter->vtx * (cellRealX - iter->cx) + iter->vty * (cellRealY - iter->cy);
	}

    iter->valid=1;

    return;

} // End of PTK_GridIter_PhysBoxInit()

uint32_t PTK_GridIter_PhysBoxGetNext(PTK_GridIter_PhysBox *iter,
                                      uint32_t            *cellX,
                                      uint32_t            *cellY)
{
    if (iter->valid)
    {
        *cellX = iter->cellX;
        *cellY = iter->cellY;
		physBoxStep(iter);
		return 1;
    }

    return 0;
} // End of PTK_GridIter_PhysBoxGetNext()

