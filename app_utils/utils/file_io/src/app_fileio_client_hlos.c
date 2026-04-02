/*
 *
 * Copyright (c) 2023-2025 Texas Instruments Incorporated
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
#include <stdint.h>
#include <stdarg.h>
#include "app_fileio_priv.h"

/*File I/O Functions Start Here*/
int32_t appFileIOWrInit(app_fileio_init_prm_t * prm)
{
    int32_t status = 0;

    return status;
}

int32_t  appFileIOWrDeInit()
{
    int32_t status = 0;

    return status;
}


/* Open a file */
FILE *appFileIOFopen(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

/* Close a file */
int32_t appFileIOFclose(FILE *stream)
{
    return fclose(stream);
}

/* Read a file */
size_t appFileIOFread(void *ptr, size_t size, size_t count, FILE *stream)
{
    return fread(ptr, size, count, stream);
}

/* Write to a file */
size_t appFileIOFwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    return fwrite(ptr, size, count, stream);
}

int32_t appFileIOFseek(FILE *stream, int32_t offset, int32_t origin)
{
    return fseek(stream, offset, origin);
}

int32_t appFileIOFtell(FILE *stream)
{
    return ftell(stream);
}

int32_t appFileIOFprintf(FILE *stream, const char *format, ...)
{
    int32_t ret;
    va_list args;
    va_start(args, format);
    ret = vfprintf(stream, format, args);
    va_end(args);

    return ret;
}

int32_t appWriteBinToFile(const char * filename, void * ptr, int32_t size, int32_t count)
{
    int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
    FILE * fptr = NULL;
    fptr = fopen((const char *)filename, "wb+");
    if (fptr)
    {
        fwrite(ptr, size, count, fptr);
        fclose(fptr);
    }
    else
    {
        printf("Could not open %s file for writing \n",filename);
        status = (int32_t)APP_FILEIO_STATUS_ERROR;
    }
    return status;
}

int32_t appReadBinFromFile(const char * filename, void * ptr, int32_t size, int32_t count)
{
    int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
    FILE * fptr = NULL;
    fptr = fopen((const char *)filename, "rb");
    if(fptr)
    {
        int res = fread(ptr, size, count, fptr);
        if(res != count)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }
        fclose(fptr);
    }
    else
    {
        printf("Could not open %s file for reading \n",filename);
        status = (int32_t)APP_FILEIO_STATUS_ERROR;
    }
    return status;
}

