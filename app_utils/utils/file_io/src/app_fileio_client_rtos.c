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
#include <string.h>
#include <stdarg.h>
#include "app_fileio_priv.h"
#include <utils/rtos/include/app_rtos.h>

static int32_t fileStatus[APP_FILEIO_MAX_NUM_FILES];

static app_fileio_wr_obj_t g_app_fileio_wr_obj;

static int32_t allocFile();
static void releaseFile(int32_t fileCtr);

static int32_t allocFile()
{
    int32_t ctr = -1;
    uint32_t i;

    for(i = 0; i < APP_FILEIO_MAX_NUM_FILES; i++)
    {
        if(fileStatus[i] == 0)
        {
            fileStatus[i] = 1;
            ctr = i;
            break;
        }
    }

    return ctr;
}

static void releaseFile(int32_t fileCtr)
{
    fileStatus[fileCtr] = 0;
}

static int32_t isValid(int32_t fileCtr)
{
    int32_t ret = 1;

    if((fileCtr < 0) || (fileCtr >= APP_FILEIO_MAX_NUM_FILES))
    {
        ret = 0;
    }

    return ret;
}

static void fileio_send_cmd(void)
{
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;

    cpu_shared_mem->writeIndicatorFlag = 1;
    while(cpu_shared_mem->writeIndicatorFlag == 1)
    {
        appRtosTaskYield();
    };
}

/*File I/O Functions Start Here*/
int32_t appFileIOWrInit(app_fileio_init_prm_t * prm)
{
    int32_t status = 0;
    int32_t ctr;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;

    obj->cpu_shared_mem = NULL;

    appFileIOWrCreateLock(obj);

    if(NULL == prm)
    {
        status = -1;
    }
    else
    {
        if(prm->self_cpu_index >= APP_FILEIO_MAX_CPUS)
        {
            status = -1;
        }
    }
    if(status == 0)
    {
        app_fileio_shared_mem_t *shared_mem = appFileIOMemMap(prm->shared_mem, sizeof(app_fileio_shared_mem_t));
        if(shared_mem!=NULL)
        {
            uint32_t i;

            obj->shared_mem = shared_mem;

            obj->cpu_shared_mem = &shared_mem->cpu_shared_mem[prm->self_cpu_index];

            obj->cpu_shared_mem->fileio_cpu_name[0] = 0;

            for(i=0; i<APP_FILEIO_MAX_CPU_NAME; i++)
            {
                obj->cpu_shared_mem->fileio_cpu_name[i] = prm->self_cpu_name[i];
                for(ctr = 0; ctr < APP_FILEIO_MAX_NUM_FILES; ctr++)
                {
                    fileStatus[ctr] = 0;
                }
            }
        }
        else
        {
            status = -1;
        }
    }
    return status;
}

/* Open a file */
FILE *appFileIOFopen(const char *filename, const char *mode)
{
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    int32_t fileCtr = -1;
    int32_t status = (int32_t )APP_FILEIO_STATUS_OK;
    intptr_t fileCtrCast;
    uint32_t cookie;

    if((!strlen(filename)) || (!strlen(mode)))
    {
        status = (int32_t)APP_FILEIO_STATUS_ERROR;
    }

    cookie = appFileIOWrLock(obj);

    if(status == (int32_t )APP_FILEIO_STATUS_OK)
    {
        fileCtr = allocFile();
        if(fileCtr == -1)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }
    }

    if(status == (int32_t )APP_FILEIO_STATUS_OK)
    {
        cpu_shared_mem->fp[fileCtr] = 0U;

        opStruct->opCode = TI_FILEIO_OPCODE_FOPEN;
        strncpy(opStruct->fileName, filename, APP_FILEIO_FILE_NAME_SIZE-1);
        strncpy(opStruct->mode, mode, APP_FILEIO_MODE_SIZE-1);
        
        cpu_shared_mem->fileCtr = fileCtr;

        fileio_send_cmd();

        if ( APP_FILEIO_STATUS_OK != cpu_shared_mem->retValue )
        {
            /* To propogate error to return value */
            fileCtr = (int32_t)APP_FILEIO_STATUS_ERROR;
        }
    }

    appFileIOWrUnLock(obj, cookie);

    /* Range of fileCtr is [0 .. APP_FILEIO_MAX_NUM_FILES]
     * Since the FILE* of 0 is treated as an error, adding 1 here to shift
     * the value into proper range (error of -1 becomes 0, and valid values
     * become positive
     * Rest of functions shall substract 1 to get the expected value */
    fileCtrCast = (intptr_t)fileCtr+1;

    return (FILE*)fileCtrCast;
}

/* Close a file */
int32_t appFileIOFclose(FILE *stream)
{   
    int8_t r = -1;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint32_t cookie = appFileIOWrLock(obj);

        if(! cpu_shared_mem->fp[fileCtr])
        {
            cpu_shared_mem->retValue = EOF;
            releaseFile(fileCtr);
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            cpu_shared_mem->retValue = 0;
            opStruct->opCode = TI_FILEIO_OPCODE_FCLOSE;
            opStruct->fid    = cpu_shared_mem->fp[fileCtr];

            fileio_send_cmd();

            r = (int32_t)(cpu_shared_mem->retValue);

            releaseFile(fileCtr);
        }

        appFileIOWrUnLock(obj, cookie);
    }

    return r;
}

/* Read a file */
size_t appFileIOFread(void *ptr, size_t size, size_t count, FILE *stream)
{
    size_t   bytestoRead = 0;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    uint8_t * sharedPtr = (uint8_t *)(&cpu_shared_mem->fileio_mem[0]);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint64_t fp = cpu_shared_mem->fp[fileCtr];

        if(!fp)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            /* Copy multiple times if data to copy > shared buffer size */
            int32_t numCopies = 0;
            int32_t totalSize = size * count;
            uint8_t * bufToCopy = (uint8_t *)ptr;
            int32_t i;
            uint32_t cookie;

            numCopies = totalSize / APP_FILEIO_PER_CPU_MEM_SIZE;

            cookie = appFileIOWrLock(obj);

            opStruct->opCode = TI_FILEIO_OPCODE_FREAD;
            opStruct->fid    = fp;
            opStruct->size   = 1;

            for(i = 0; i < numCopies; i++)
            {
                opStruct->count  = APP_FILEIO_PER_CPU_MEM_SIZE;
                fileio_send_cmd();
                if((int32_t)APP_FILEIO_STATUS_OK != cpu_shared_mem->retValue)
                {
                    status = (int32_t)APP_FILEIO_STATUS_ERROR;
                    break;
                }
                memcpy(bufToCopy, sharedPtr, APP_FILEIO_PER_CPU_MEM_SIZE); /* Copy from remote core internal trace data ptr to shared ptr */
                bufToCopy += APP_FILEIO_PER_CPU_MEM_SIZE;
            }
            if( (status == (int32_t )APP_FILEIO_STATUS_OK) &&
                (i == numCopies) &&
                (totalSize % APP_FILEIO_PER_CPU_MEM_SIZE != 0)) /* Remnant data */
            {
                opStruct->count  = totalSize % APP_FILEIO_PER_CPU_MEM_SIZE;
                fileio_send_cmd();
                if((int32_t)APP_FILEIO_STATUS_OK != cpu_shared_mem->retValue)
                {
                    status = (int32_t)APP_FILEIO_STATUS_ERROR;
                }
                else
                {
                    memcpy(bufToCopy, sharedPtr, totalSize % APP_FILEIO_PER_CPU_MEM_SIZE);
                }
            }
            appFileIOWrUnLock(obj, cookie);
        }

        if (status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            bytestoRead = count;
        }
    }
    return bytestoRead;
}

/* Write to a file */
size_t appFileIOFwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    size_t    bytestoWrite = 0;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    uint8_t * sharedPtr = (uint8_t *)(&cpu_shared_mem->fileio_mem[0]);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint64_t fp = cpu_shared_mem->fp[fileCtr];

        if(!fp)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            /* Copy multiple times if data to copy > shared buffer size */
            int32_t numCopies = 0;
            int32_t totalSize = size * count;
            uint8_t * bufToCopy = (uint8_t *)ptr;
            int32_t i;
            uint32_t cookie;

            numCopies = totalSize / APP_FILEIO_PER_CPU_MEM_SIZE;

            cookie = appFileIOWrLock(obj);

            opStruct->opCode = TI_FILEIO_OPCODE_FWRITE;
            opStruct->fid    = fp;
            opStruct->size   = 1;

            for(i = 0; i < numCopies; i++)
            {
                memcpy(sharedPtr, bufToCopy, APP_FILEIO_PER_CPU_MEM_SIZE); /* Copy from remote core internal trace data ptr to shared ptr */
                bufToCopy += APP_FILEIO_PER_CPU_MEM_SIZE;

                opStruct->count  = APP_FILEIO_PER_CPU_MEM_SIZE;
                fileio_send_cmd();
                if((int32_t)APP_FILEIO_STATUS_OK != cpu_shared_mem->retValue)
                {
                    status = (int32_t)APP_FILEIO_STATUS_ERROR;
                    break;
                }
            }
            if((status == (int32_t )APP_FILEIO_STATUS_OK) &&
            (i == numCopies) &&
            (totalSize % APP_FILEIO_PER_CPU_MEM_SIZE != 0)) /* Remnant data */
            {
                memcpy(sharedPtr, bufToCopy, totalSize % APP_FILEIO_PER_CPU_MEM_SIZE);

                opStruct->count  = (int32_t)(totalSize % APP_FILEIO_PER_CPU_MEM_SIZE);
                fileio_send_cmd();
                if((int32_t)APP_FILEIO_STATUS_OK != cpu_shared_mem->retValue)
                {
                    status = (int32_t)APP_FILEIO_STATUS_ERROR;
                }
            }
            appFileIOWrUnLock(obj, cookie);
        }

        if (status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            bytestoWrite = count;
        }
    }

    return bytestoWrite;
}

int32_t appFileIOFseek(FILE *stream, int32_t offset, int32_t origin)
{
    int32_t r = -1;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint64_t fp = cpu_shared_mem->fp[fileCtr];

        if(!fp)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            uint32_t cookie = appFileIOWrLock(obj);

            opStruct->opCode = TI_FILEIO_OPCODE_FSEEK;
            opStruct->fid    = fp;
            opStruct->offset = offset;
            opStruct->count  = origin;

            fileio_send_cmd();

            r = cpu_shared_mem->retValue;

            appFileIOWrUnLock(obj, cookie);
        }
    }
    return r;
}

int32_t appFileIOFtell(FILE *stream)
{
    int32_t   r = -1;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint64_t fp = cpu_shared_mem->fp[fileCtr];

        if(!fp)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            uint32_t cookie = appFileIOWrLock(obj);
            opStruct->opCode = TI_FILEIO_OPCODE_FTELL;
            opStruct->fid    = fp;

            fileio_send_cmd();

            r = cpu_shared_mem->retValue;

            appFileIOWrUnLock(obj, cookie);
        }
    }
    return r;
}

int32_t appFileIOFprintf(FILE *stream, const char *format, ...)
{
    int32_t ret = -1;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    uint8_t * sharedPtr = (uint8_t *)(&cpu_shared_mem->fileio_mem[0]);
    intptr_t fileCtrCast = (intptr_t)stream;
    int32_t fileCtr = (int32_t)fileCtrCast-1;

    if (isValid(fileCtr) == 1)
    {
        int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
        uint64_t fp = cpu_shared_mem->fp[fileCtr];

        if(!fp)
        {
            status = (int32_t)APP_FILEIO_STATUS_ERROR;
        }

        if(status == (int32_t )APP_FILEIO_STATUS_OK)
        {
            /* Copy multiple times if data to copy > shared buffer size */
            int32_t totalSize;
            va_list args;
            uint32_t cookie = appFileIOWrLock(obj);

            va_start(args, format);
            totalSize = vsnprintf((char *)sharedPtr, APP_FILEIO_PER_CPU_MEM_SIZE, format, args);
            va_end(args);

            if(totalSize < APP_FILEIO_PER_CPU_MEM_SIZE)
            {
                opStruct->opCode = TI_FILEIO_OPCODE_FWRITE;
                opStruct->fid    = fp;
                opStruct->size   = 1;
                opStruct->count  = totalSize;

                fileio_send_cmd();

                ret = totalSize;
            }
            appFileIOWrUnLock(obj, cookie);
        }
    }
    return ret;
}


/* The following commented out section of code contains support for more File operations, but it is not validated
    Can be updated and uncommented if required based on more validation */
#if 0
char * appFileIOFgets(char *str, int32_t num, int32_t fileCtr)
{
    char     *ret = NULL;
    int32_t status = (int32_t)APP_FILEIO_STATUS_OK;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    OpStruct_t * opStruct = (OpStruct_t *)(&cpu_shared_mem->opStruct);
    uint64_t fp = cpu_shared_mem->fp[fileCtr];
    uint32_t cookie;

    if(!fp)
    {
        ret = NULL;
        status = (int32_t)APP_FILEIO_STATUS_ERROR;
    }

    if(!str)
    {
        ret = NULL;
        status = (int32_t)APP_FILEIO_STATUS_ERROR;
    }

    cookie = appFileIOWrLock(obj);

    if(status == (int32_t )APP_FILEIO_STATUS_OK)
    {
        opStruct->opCode = TI_FILEIO_OPCODE_FGETS;
        opStruct->fid    = fp;
        opStruct->size   = num;

        fileio_send_cmd();

        ret = str;
    }

    appFileIOWrUnLock(obj, cookie);

    return ret;
}
#endif

int32_t appWriteBinToFile(const char * filename, void * ptr, int32_t size, int32_t count)
{
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    FILE *fptr = NULL;
    int32_t status = 0;

    char fileNameWithPrefix[APP_FILEIO_FILE_NAME_SIZE*3];
    char fileLocalName[APP_FILEIO_FILE_NAME_SIZE], dirName[APP_FILEIO_FILE_NAME_SIZE];
    strncpy(fileLocalName, filename, APP_FILEIO_FILE_NAME_SIZE-1);
    strncpy(dirName, filename, APP_FILEIO_FILE_NAME_SIZE-1);
    fileLocalName[APP_FILEIO_FILE_NAME_SIZE-1] = '\0';
    char * fileNamePtr = getFileNameFromPath(fileLocalName);
    getDirFromPath(dirName);

    sprintf(fileNameWithPrefix, "%s/%s_%s", (char *)dirName, (char *)cpu_shared_mem->fileio_cpu_name, fileNamePtr);

    /* Open file in write mode open afresh and prevent appending traces to existing file */
    #if defined CODE_COVERAGE_ENABLED_FOR_TIDL || defined LDRA_COVERAGE_ENABLED
    fptr = appFileIOFopen(fileNameWithPrefix, "ab");
    #else
    fptr = appFileIOFopen(fileNameWithPrefix, "wb");
    #endif

    if(fptr)
    {
        appFileIOFclose(fptr);
        /* Close and reopen in append mode to do staggered write of entire data */
        fptr = appFileIOFopen(fileNameWithPrefix, "ab");
        if(fptr)
        {
            int32_t bytes = appFileIOFwrite(ptr, size, count, fptr);
            if(bytes != count)
            {
                status = -1;
            }
            appFileIOFclose(fptr);
        }
        else
        {
            status = -1;
        }
    }
    else
    {
        status = -1;
    }

    return status;
}

int32_t appReadBinFromFile(const char * filename, void * ptr, int32_t size, int32_t count)
{
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;
    app_fileio_cpu_shared_mem_t * cpu_shared_mem = obj->cpu_shared_mem;
    FILE *fptr = NULL;
    int32_t status = 0;

    char fileNameWithPrefix[APP_FILEIO_FILE_NAME_SIZE*3];
    char fileLocalName[APP_FILEIO_FILE_NAME_SIZE], dirName[APP_FILEIO_FILE_NAME_SIZE];
    strncpy(fileLocalName, filename, APP_FILEIO_FILE_NAME_SIZE-1);
    strncpy(dirName, filename, APP_FILEIO_FILE_NAME_SIZE-1);
    fileLocalName[APP_FILEIO_FILE_NAME_SIZE-1] = '\0';
    char * fileNamePtr = getFileNameFromPath(fileLocalName);
    getDirFromPath(dirName);

    sprintf(fileNameWithPrefix, "%s/%s_%s", (char *)dirName, (char *)cpu_shared_mem->fileio_cpu_name, fileNamePtr);

    fptr = appFileIOFopen(fileNameWithPrefix, "rb");
    if(fptr)
    {
        int32_t bytes = appFileIOFread(ptr, size, count, fptr);
        if(bytes != count)
        {
            status = -1;
        }
        appFileIOFclose(fptr);
    }
    else
    {
        status = -1;
    }
    return status;
}

int32_t  appFileIOWrDeInit()
{
    int32_t status = 0;
    app_fileio_wr_obj_t *obj = &g_app_fileio_wr_obj;

    appFileIOWrDestroyLock(obj);

    return status;
}
