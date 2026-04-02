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
 * @file rigidTransform_d.c
 * @brief Implementation of functions for manipulating double precision rigid transforms
 */

#include <string.h>

#include <perception/base/core.h>
#include <perception/base/rigidTransform.h>

void PTK_RigidTransform_d_makeIdentity(PTK_RigidTransform_d *M)
{
	memset(M->M, 0, sizeof(M->M));

	M->M[0] = 1.0;
	M->M[5] = 1.0;
	M->M[10] = 1.0;
	M->M[15] = 1.0;
}

void PTK_RigidTransform_d_setTranslation(PTK_RigidTransform_d *M, double x, double y, double z)
{
    M->M[3] = x;
    M->M[7] = y;
    M->M[11] = z;
}

void PTK_RigidTransform_d_getTranslation(const PTK_RigidTransform_d *M, double *x, double *y, double *z)
{
    *x = M->M[3];
    *y = M->M[7];
    *z = M->M[11];
}

void PTK_RigidTransform_d_setRotation(PTK_RigidTransform_d *__restrict M, const double *__restrict R)
{
	M->M[0] = R[0];
	M->M[1] = R[1];
	M->M[2] = R[2];
	M->M[4] = R[3];
	M->M[5] = R[4];
	M->M[6] = R[5];
	M->M[8] = R[6];
	M->M[9] = R[7];
	M->M[10] = R[8];
}

void PTK_RigidTransform_d_getRotation(const PTK_RigidTransform_d *__restrict M, double *__restrict R)
{
	R[0] = M->M[0];
	R[1] = M->M[1];
	R[2] = M->M[2];
	R[3] = M->M[4];
	R[4] = M->M[5];
	R[5] = M->M[6];
	R[6] = M->M[8];
	R[7] = M->M[9];
	R[8] = M->M[10];
}

void PTK_RigidTransform_d_compose(PTK_RigidTransform_d *__restrict M31_out,
                                  const PTK_RigidTransform_d *__restrict M32_in,
                                  const PTK_RigidTransform_d *__restrict M21_in)
{
    int row, col;

	// Do the first three rows of a 4x4 matrix multiply
	for (row = 0; row < 3; ++row)
	{
		int offset = 4 * row;

		for (col = 0; col < 4; ++col)
		{
			M31_out->M[offset + col] = M32_in->M[offset] * M21_in->M[col]
			                           + M32_in->M[offset+1] * M21_in->M[col+4]
									   + M32_in->M[offset+2] * M21_in->M[col+8]
									   + M32_in->M[offset+3] * M21_in->M[col+12];
		}
	}

	// Set the last row explicitly
	M31_out->M[12] = 0.0;
	M31_out->M[13] = 0.0;
	M31_out->M[14] = 0.0;
	M31_out->M[15] = 1.0;
}

void PTK_RigidTransform_d_invert(PTK_RigidTransform_d *__restrict M21_out,
                                 const PTK_RigidTransform_d *__restrict M12_in)
{
    // Rinv = R'
    M21_out->M[0] = M12_in->M[0];
    M21_out->M[1] = M12_in->M[4];
    M21_out->M[2] = M12_in->M[8];

    M21_out->M[4] = M12_in->M[1];
    M21_out->M[5] = M12_in->M[5];
    M21_out->M[6] = M12_in->M[9];

    M21_out->M[8] = M12_in->M[2];
    M21_out->M[9] = M12_in->M[6];
    M21_out->M[10] = M12_in->M[10];

    // tinv = -R'*t
    M21_out->M[3] = -M21_out->M[0] * M12_in->M[3]
                    - M21_out->M[1] * M12_in->M[7]
                    - M21_out->M[2] * M12_in->M[11];
    M21_out->M[7] = -M21_out->M[4] * M12_in->M[3]
                    - M21_out->M[5] * M12_in->M[7]
                    - M21_out->M[6] * M12_in->M[11];
    M21_out->M[11] = -M21_out->M[8] * M12_in->M[3]
                    - M21_out->M[9] * M12_in->M[7]
                    - M21_out->M[10] * M12_in->M[11];

	// Fix final row
	M21_out->M[12] = 0.0;
	M21_out->M[13] = 0.0;
	M21_out->M[14] = 0.0;
	M21_out->M[15] = 1.0;
}

void PTK_RigidTransform_d_print(const PTK_RigidTransform_d *M)
{
    PTK_printf("[ %f %f %f %f\n"
               "  %f %f %f %f\n"
               "  %f %f %f %f ]\n",
               M->M[0], M->M[1], M->M[2], M->M[3],
               M->M[4], M->M[5], M->M[6], M->M[7],
               M->M[8], M->M[9], M->M[10], M->M[11]);
}

void PTK_RigidTransform_d_convertToSingle(PTK_RigidTransform *__restrict Mout,
                                          const PTK_RigidTransform_d *__restrict Min)
{
	int i;
	for (i = 0; i < 16; ++i)
	{
		Mout->M[i] = (float) Min->M[i];
	}
}

