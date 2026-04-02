/*
*
* Copyright (c) {2015 - 2019} Texas Instruments Incorporated
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
/** @file fcw_tb.c
*
*  @brief  This file contains test code for forward collision warning algorithm
*
*
*  @date   October 2015
*
*  Description
*    This file contains test code to validate the frame level algorithm TI FCW
*
*/

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>

#include "tiadalg_vl_test.h"
#include "tiadalg_interface.h"
#include "../common/tiadalg_test_interface.h"
#include "../common/profile.h"

#define ENABLE_PRINT_LOG
//#define ENABLE_NMS_PROFILE_DEBUG_PRINT_LOG

#define ENABLE_PROFILE

#if (!HOST_EMULATION)
#if defined(_TMS320C6600)
#include <c6x.h>
#endif
#endif

#define SPARSE_KP_FLOW
//#define DUMP_BIN_KP_DESC


#ifndef SPARSE_KP_FLOW
static int32_t vl_alg_fe_correct(int32_t* in_pt_list, int32_t num_points, float* lens_dist_table, int32_t table_len,
  int32_t* in_pruned_pt_list, float* out_pt_list, int32_t img_width, int32_t img_height,
  int32_t out_img_width, int32_t out_img_height);

static inline uint8_t vl_alg_interpolate(float xPosition, float xRangeLower, float xRangeHigher, float yRangeLower,
    float yRangeHigher, float *yInterpolated);

#endif


#define PRINT_ERRORE_MSG()  printf("Error at line: %5d : in file %22s, of \
                            function : %s \n",__LINE__, __FILE__,__FUNCTION__)

#define DMEM1_SIZE (128*1024)

#define DDRCACHE_SIZE (4*1024*1024)

#ifndef SPARSE_KP_FLOW
#pragma DATA_SECTION (".ddrcacheSect");
int8_t score_buf[768*384];
#pragma DATA_SECTION (".ddrcacheSect");
int8_t desc_buf[192*96*65];
#pragma DATA_SECTION (".ddrcacheSect");
int16_t scratch_buf[16*12*64];
#pragma DATA_SECTION (".ddrcacheSect");
int8_t weights[7*7*64];
#pragma DATA_SECTION (".ddrcacheSect");
int8_t bias[64*2];
#endif

#pragma DATA_SECTION (".dmem1");
uint8_t L2MEM_SCRATCH[DMEM1_SIZE];
TIMemObject memObj_DMEM1;

#pragma DATA_SECTION (".ddrcacheSect");
uint8_t DDRMEM_CACHE[DDRCACHE_SIZE];
TIMemObject memObj_DDRMEMC;

int32_t TestApp_AllocMemRecords(IALG_MemRec * memRec,int32_t numMemRec)
{
  int32_t i;
  TIMemHandle memHdl_DMEM1 = &memObj_DMEM1;
  TIMemHandle memHdl_EXTMEM = &memObj_DDRMEMC;

  for (i = 0; i < numMemRec; i++)
  {
    if(memRec[i].space == IALG_EXTERNAL){
      memRec[i].base = tiadalg_get_memory_chunk(memHdl_EXTMEM, memRec[i].size,
         memRec[i].alignment);

      if(memRec[i].base == NULL) {
        memRec[i].base = (void *) malloc( memRec[i].size);
        memRec[i].space = IALG_EXTERNAL;
        printf("\n External cached memory could not be allocated");
      }
    }
    else if(memRec[i].space == IALG_DARAM1) {
      memRec[i].base = tiadalg_get_memory_chunk(memHdl_DMEM1, memRec[i].size,
         memRec[i].alignment);

      if(memRec[i].base == NULL) {
        memRec[i].base = (void *) malloc( memRec[i].size);
        memRec[i].space = IALG_EXTERNAL;
        printf("\n Internal memory could not be allocated");
      }
    }
    else if(memRec[i].space == IALG_DARAM0) {
      /*L1D SRAM space, currently alg requires 16kb SRAM and 16kb cache.
      */
      printf("\n L1D Internal memory could not be allocated");
    }
    else {
      /* From heap present in external memory
      */
      memRec[i].base = (void *) malloc( memRec[i].size);
    }
    if(memRec[i].base == NULL)
    {
      exit(0);
      return IALG_EFAIL;
    }
  }
  return IALG_EOK;
}

int32_t Inargs_memContamination(TIADALG_el_in_args   *inArgs)
{

  memset(inArgs, (int32_t) 0xFF , sizeof(TIADALG_el_in_args));
  return(0);
}

int32_t TestApp_memContamination(IALG_MemRec * memRec,int32_t numMemRec,
                                 int32_t frmIdx)
{
  int32_t i,j;
  int32_t size;
  float*  src;

  for (i = 0; i < numMemRec; i++)
  {
    if(memRec[i].attrs == IALG_SCRATCH){

      size = memRec[i].size >> 2;
      src  = (float*)memRec[i].base;
      for(j = 0; j < size; j++)
        src[j] = FLT_MAX;
    }
    if((memRec[i].attrs == IALG_PERSIST) && (frmIdx == -1)){

      size = memRec[i].size >> 2;
      src  = (float*)memRec[i].base;
      for(j = 0; j < size; j++)
        src[j] = FLT_MAX;
    }
  }
  return(0);
}

int32_t TestApp_FreeMemRecords(IALG_MemRec * memRec,int32_t numMemRec)
{
  int32_t i;
  for (i = 0; i < numMemRec; i++)
  {
    if(memRec[i].base == NULL)
    {
      return IALG_EFAIL;
    }
    if(memRec[i].space == IALG_EXTERNAL){
      free(memRec[i].base);
    }
  }
  return IALG_EOK;
}


int32_t TestApp_AllocIOBuffers(IVISION_InBufs *inBufs,IVISION_OutBufs *outBufs,
                               sEL_Config * params)
{
  TIMemHandle memHdl_DMEM1 = &memObj_DDRMEMC;
  //TIMemHandle memHdl_DMEM1 = &memObj_DMEM1;

#if 0
  if(params->is_feat_comp_ext == 0){
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->numPlanes                       = 1;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].frameROI.topLeft.x = 0;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].frameROI.topLeft.y = 0;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].width              = params->width;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].height             = params->height;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].frameROI.width     = params->width;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].frameROI.height    = params->height;
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].planeType          = 0;
    /* Allocate the buffer in DDR which is non-cacheable */
    inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].buf =
                                        tiadalg_get_memory_chunk(memHdl_EXTMEM, params->width*params->height, 4);
    if(inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_IMAGE]->bufPlanes[0].buf == NULL)
    {
      PRINT_ERRORE_MSG();
    }
  }
#endif
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->numPlanes                       = 1;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].frameROI.topLeft.x = 0;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].frameROI.topLeft.y = 0;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].width              = sizeof(tiadalg_ext_feat);
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].height             = params->max_frame_feat;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].frameROI.width     = sizeof(tiadalg_ext_feat);
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].frameROI.height    = params->max_frame_feat;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].planeType          = 0;
  /* Allocate the buffer in DDR which is non-cacheable */
  /*In NMS based on row major procesing vs column major processing, inner loop might collect more keypoints than
    confgured, hence allocating more space to avoid illegal mmeory access.
  */
  int32_t max_key_points = params->max_frame_feat +  (params->width >  params->height ? params->width : params->height);

  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf =
                                      tiadalg_get_memory_chunk(memHdl_DMEM1, sizeof(tiadalg_ext_feat)*max_key_points, 8);

  if(inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf == NULL)
  {
    PRINT_ERRORE_MSG();
  }

  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->numPlanes                       = 1;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].frameROI.topLeft.x = 0;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].frameROI.topLeft.y = 0;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].width              = sizeof(tiadalg_feat_desc);
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].height             = params->max_frame_feat;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].frameROI.width     = sizeof(tiadalg_feat_desc);
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].frameROI.height    = params->max_frame_feat;
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].planeType          = 0;
  /* Allocate the buffer in DDR which is non-cacheable */
  inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf =
                                      tiadalg_get_memory_chunk(memHdl_DMEM1, params->max_frame_feat*sizeof(tiadalg_feat_desc), 8);
  if(inBufs->bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf == NULL)
  {
    PRINT_ERRORE_MSG();
  }


  return IALG_EOK;
}

#define TIADALG_LENS_DIST_TBL_SIZE (652)

int32_t elTest(sEL_Config * testcaseParams)
{
  int32_t numMemRec;
  IALG_MemRec *memRec;
  int32_t status;

  FILE *fpOut = NULL;
  char *file = NULL;
  int32_t bytes_read;

#ifdef ENABLE_PROFILE
  long long  t0, t1;
  _TSC_enable();
#endif

#ifndef HOST_EMULATION
    tiadalg_init_cache();
#endif

  /*-----------------------------------------------------------------
  Pointer to algorithm instance
  -----------------------------------------------------------------*/
  IM_Fxns *     handle;

  /*-----------------------------------------------------------------
  Algorithm parameters memory in stack
  -----------------------------------------------------------------*/
  TIADALG_el_create_params  createParams;
  TIADALG_el_in_args        inArgs;
  IVISION_InBufs            inBufs;

  /*-----------------------------------------------------------------
  For test purpose it is allocated in stack. Application may get
  this descriptors from its buffer manager
  -----------------------------------------------------------------*/
  IVISION_BufDesc   inBufDesc[TIADALG_EL_IN_BUFDESC_TOTAL];
  IVISION_BufDesc   *inBufDescList[TIADALG_EL_IN_BUFDESC_TOTAL];

  TIADALG_el_out_args    outArgs;

  float lens_dist_table_ptr[TIADALG_LENS_DIST_TBL_SIZE];

  outArgs.iVisionOutArgs.size = sizeof(TIADALG_el_out_args);
  inArgs.iVisionInArgs.size   = sizeof(TIADALG_el_in_args);

  /*--------------------------------------------------------------------------*/
  /*Create memory handle for L2MEM.                                           */
  /*--------------------------------------------------------------------------*/
  uint8_t *pL2mem;
  TIMemHandle memHdl_L2MEM;

  /*--------------------------------------------------------------------------*/
  /*Create memory handle for DDR NON CACHE MEM                                */
  /*--------------------------------------------------------------------------*/
  uint8_t *pExtCmem;
  TIMemHandle memHdl_EXTMEMC;


  pL2mem = L2MEM_SCRATCH;
  memHdl_L2MEM = &memObj_DMEM1;
  tiadalg_create_memory_handle(memHdl_L2MEM, pL2mem, DMEM1_SIZE);

  pExtCmem = DDRMEM_CACHE;
  memHdl_EXTMEMC = &memObj_DDRMEMC;
  tiadalg_create_memory_handle(memHdl_EXTMEMC, pExtCmem, DDRCACHE_SIZE);

  //for ( testcase = 0 ; testcase < el_numTestCases; testcase++ )
  {
    /*-----------------------------------------------------------------
    Set buffer descriptor pointers array base
    -----------------------------------------------------------------*/
    inBufs.bufDesc  = inBufDescList;
    inBufs.numBufs  = TIADALG_EL_IN_BUFDESC_TOTAL;
    for(int32_t i = 0; i <  TIADALG_EL_IN_BUFDESC_TOTAL; i++){
      inBufDescList[i] = &inBufDesc[i];
    }

    /*-----------------------------------------------------------------
    Set algorithm create time parameters
    -----------------------------------------------------------------*/
    createParams.map_info               = testcaseParams->map_info;
    createParams.max_frame_feat         = testcaseParams->max_frame_feat;
    createParams.max_map_feat       = testcaseParams->max_map_feat;
    createParams.is_feat_comp_ext       = testcaseParams->is_feat_comp_ext;
    createParams.max_feat_match         = 50;

    /*-----------------------------------------------------------------
    Query alogorithm memory requirement using standard IALG interface
    -----------------------------------------------------------------*/
    numMemRec = VL_VISION_FXNS.ialg.algNumAlloc();
    memRec    = (IALG_MemRec *)malloc(numMemRec*sizeof(IALG_MemRec));

    status = VL_VISION_FXNS.ialg.algAlloc((IALG_Params *)(&createParams),
                                                                  NULL, memRec);

    if(status != IALG_EOK)
    {
      PRINT_ERRORE_MSG();
      goto EXIT_LOOP;
    }

    printf("EL algorithm Create Done\n");

    /*-----------------------------------------------------------------
    Allocate memory for all the mem records
    Application has to implement the below function
    Refer the example implementation below in this file
    -----------------------------------------------------------------*/
    status = TestApp_AllocMemRecords(memRec,numMemRec);

    TestApp_memContamination(memRec,numMemRec,-1);

#if !defined(HOST_EMULATION)
#ifdef TIADALG_OS_BUILD
    Cache_WbInvAll();
#endif
#endif

    printf("Reading the voxel data \n");
    bytes_read =
    tiadalg_read_bin_file(memRec[1].base, -1, testcaseParams->map_voxel_file_name);

    if(bytes_read == 0){
      printf("could not open the file %s \n",testcaseParams->map_voxel_file_name);
      exit(0);
    }

    printf("voxel data reading finished\n");

    if(bytes_read != testcaseParams->map_info.num_voxels*sizeof(tiadalg_voxel_info)){
      printf(" bytes_read is %d, and it was expected as %lld\n", bytes_read,testcaseParams->map_info.num_voxels*sizeof(tiadalg_voxel_info));
      exit(0);
    }

    printf("Reading map feature point data \n");
    bytes_read =
    tiadalg_read_bin_file(memRec[2].base, -1,testcaseParams->map_feat_file_name);

    if(bytes_read == 0){
      printf("could not open the file %s \n",testcaseParams->map_feat_file_name);
      exit(0);
    }

    printf("map feature point data reading finished\n");

    if(bytes_read != testcaseParams->map_info.num_map_feat*3*sizeof(float)){
      printf(" bytes_read is %d, and it was expected as %lld\n", bytes_read, testcaseParams->map_info.num_map_feat*3*sizeof(float));
      exit(0);
    }

    printf("Reading map feature descriptor data \n");
    bytes_read =
    tiadalg_read_bin_file(memRec[3].base, -1,testcaseParams->map_desc_file_name);

    if(bytes_read == 0){
      printf("could not open the file %s \n",testcaseParams->map_desc_file_name);
      exit(0);
    }

    if(bytes_read != testcaseParams->map_info.num_map_feat*testcaseParams->map_info.desc_size*sizeof(VL_DESC_DATA_TYPE)){
      printf(" bytes_read is %d, and it was expected as %lld\n",
              bytes_read, testcaseParams->map_info.num_map_feat*testcaseParams->map_info.desc_size*sizeof(VL_DESC_DATA_TYPE));
      exit(0);
    }

    printf("map feature descriptor data reading finished\n");

    if(testcaseParams->is_ip_fe == 1){
      printf("Reading Fish eye disrtortion correction table \n");

      bytes_read =
      tiadalg_read_f32_data_from_file(lens_dist_table_ptr, TIADALG_LENS_DIST_TBL_SIZE, testcaseParams->lens_dist_table_file);

      if(bytes_read == 0){
        printf("could not open the file %s \n",testcaseParams->lens_dist_table_file);
        exit(0);
      }

      if(bytes_read != TIADALG_LENS_DIST_TBL_SIZE){
        printf(" bytes_read is %d, and it was expected as %d\n",
                bytes_read, TIADALG_LENS_DIST_TBL_SIZE);
        exit(0);
      }
#ifdef DUMP_LENS_DIST_TABLE
    FILE * fp = fopen("/user/a0393749/deepak_files/ti/vx-work/tiadalg/data/visual_localization/lens_dist_table.bin","wb");
    fwrite(lens_dist_table_ptr,sizeof(float),TIADALG_LENS_DIST_TBL_SIZE,fp);
    fclose(fp);
#endif

    }

    if(status != IALG_EOK)
    {
      PRINT_ERRORE_MSG();
      goto EXIT_LOOP;
    }

    createParams.map_voxel_ptr = (uint8_t*)memRec[1].base;
    createParams.map_pt_ptr = (uint8_t*)memRec[2].base;
    createParams.map_desc_ptr = (uint8_t*)memRec[3].base;

    /*-----------------------------------------------------------------
    Intialize the algorithm instance with the alocated memory
    and user create parameters
    -----------------------------------------------------------------*/
    status = VL_VISION_FXNS.ialg.algInit((IALG_Handle)(&handle), memRec,
                                          NULL, (IALG_Params *)(&createParams));

    handle = (IM_Fxns *) memRec[0].base;

    if(status != IALG_EOK)
    {
      PRINT_ERRORE_MSG();
      goto EXIT_LOOP;
    }

    printf("EL algorithm Init Done\n");

    inArgs.iVisionInArgs.subFrameInfo = 0;
    inArgs.iVisionInArgs.size         = sizeof(TIADALG_el_in_args);

    for(int32_t i = 0; i <  TIADALG_EL_IN_BUFDESC_TOTAL; i++){
      inBufDesc[i].numPlanes               = 1;
    }

    TestApp_AllocIOBuffers(&inBufs,NULL,testcaseParams);

    FILE* log_file;
    log_file             =  fopen(testcaseParams->output_file_path,"w");

    if(log_file == NULL){
      printf("log file could not be opened \n");
    }
    char file_name[TIADALG_FILE_NAME_SIZE];

#ifndef SPARSE_KP_FLOW
    uint32_t is_scratch_filled =0;
    bytes_read = tiadalg_read_bin_file(weights,-1,testcaseParams->upsampling_weight_file);

    if((bytes_read == 0) || (bytes_read != (7*7*64))){
      printf("could not open the file %s \n",file_name);
      exit(0);
    }

    bytes_read = tiadalg_read_bin_file(bias,-1,testcaseParams->upsampling_bias_file);

    if((bytes_read == 0) || (bytes_read != (2*64))){
      printf("could not open the file %s \n",file_name);
      exit(0);
    }

#endif
    for(int32_t frameIdx = testcaseParams->start_frame; frameIdx < testcaseParams->start_frame + testcaseParams->num_frames*testcaseParams->skip_frame;)
    {
      //printf("%010d \n",frameIdx);
      Inargs_memContamination(&inArgs);


#ifdef SPARSE_KP_FLOW
      /*Reading external key point file to know the number of key points*/
      sprintf(file_name,testcaseParams->ext_feat_file_name,frameIdx);

#ifdef ENABLE_PRINT_LOG
      printf("Reading the file %s \n",file_name);
#endif

      /*** Below portion of the code is to find the number of sparse features availbale in feature file***/
      bytes_read = tiadalg_read_bin_file(0x0,-1,file_name);

      testcaseParams->num_frame_feat = bytes_read/sizeof(tiadalg_ext_feat);

      if(testcaseParams->num_frame_feat > testcaseParams->max_frame_feat){
        printf("Current frame features are higher than maximum allowed, setting to max\n");
        printf("In this scenario if the buffer would  have been provided then it would have overflowed \n");
        testcaseParams->num_frame_feat = testcaseParams->max_frame_feat;
      }

      bytes_read=
      tiadalg_read_bin_file(inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf,
        -1,file_name);

      testcaseParams->num_frame_feat = bytes_read/sizeof(tiadalg_ext_feat);


#ifdef ENABLE_PRINT_LOG
      printf("Total features in current frame is %d \n",testcaseParams->num_frame_feat);
#endif

      if((bytes_read == 0) ||(bytes_read != (testcaseParams->num_frame_feat*sizeof(tiadalg_ext_feat)))){
        printf("could not open the file %s \n",file_name);
        exit(0);
      }

      /*Reading descriptor file*/
      sprintf(file_name,testcaseParams->ext_desc_file_name,frameIdx);
#ifdef ENABLE_PRINT_LOG
      printf("Reading the file %s \n",file_name);
#endif
      bytes_read=
      tiadalg_read_bin_file(inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf,
                           -1,file_name);

      if(bytes_read == 0){
        printf("could not open the file %s \n",file_name);
        exit(0);
      }

      if(bytes_read != testcaseParams->num_frame_feat*testcaseParams->map_info.desc_size*sizeof(VL_DESC_DATA_TYPE)){
        printf(" bytes_read is %d, and it was expected as %d",
                  bytes_read, testcaseParams->num_frame_feat*testcaseParams->map_info.desc_size*sizeof(VL_DESC_DATA_TYPE));
        exit(0);
      }

      inArgs.feat_q_factor                    = 1024;

#else // Dense feature map file is read. And with NMS and sparse upsampling sparse list is prepared.
      int32_t img_height=384;
      int32_t img_width=768;
      int32_t recon_img_width = 2048;
      int32_t recon_img_height = 1024;

      int32_t guard_pixels = 32;
      int32_t shift_amt = -(-0);
      int32_t rnd = 1<<(shift_amt-1);
      int32_t cur_score_th = ((testcaseParams->nms_th + rnd) >> shift_amt);
      tiadalg_ext_feat* key_point_list = (tiadalg_ext_feat*)inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf;
      int32_t num_key_points = 0;

      /*Reading the dense key points score file*/
      sprintf(file_name,testcaseParams->ext_feat_file_name,frameIdx);

#ifdef ENABLE_PRINT_LOG
      printf("Reading the file %s \n",file_name);
#endif

      bytes_read = tiadalg_read_bin_file(score_buf,img_width*img_height,file_name);

      if((bytes_read == 0) || (bytes_read != 768*384)){
        printf("could not open the file %s \n",file_name);
        exit(0);
      }

      /*Reading descriptor file*/
      sprintf(file_name,testcaseParams->ext_desc_file_name,frameIdx);

#ifdef ENABLE_PRINT_LOG
      printf("Reading the file %s \n",file_name);
#endif
      bytes_read=
      tiadalg_read_bin_file(desc_buf,-1,file_name);

      if((bytes_read == 0) || (bytes_read !=((img_width*img_height)>>4)*64)){
        printf("could not open the file %s \n",file_name);
        exit(0);
      }

      int32_t* fe_kp_list = (int32_t*)malloc(testcaseParams->max_frame_feat*sizeof(int32_t)*2);
      int32_t* fe_pruned_kp_list = (int32_t*)malloc(testcaseParams->max_frame_feat*sizeof(int32_t)*2);

      num_key_points = tiadalg_image_recursive_nms_cn(score_buf, img_width, img_width, img_height, cur_score_th,
                                                       testcaseParams->max_frame_feat, guard_pixels,
                                                       (int8_t*)scratch_buf, TIADALG_DATA_TYPE_U08, fe_kp_list);
      if(testcaseParams->is_ip_fe == 1){
        num_key_points = vl_alg_fe_correct(fe_kp_list, num_key_points, lens_dist_table_ptr,
                                            TIADALG_LENS_DIST_TBL_SIZE, fe_pruned_kp_list,
                                            (float*) key_point_list, img_width, img_height,
                                            recon_img_width, recon_img_height);
      }else{
        // in this flow just scale up the key points to orginal image resolution
        float x_scale_fct = ((float)recon_img_width)/img_width;
        float y_scale_fct = ((float)recon_img_height)/img_height;

        for(int32_t i = 0; i < num_key_points; i++)
        {
          ((float*) key_point_list)[2*i]     = ((int32_t*)fe_kp_list)[2*i] * x_scale_fct;
          ((float*) key_point_list)[2*i + 1] = ((int32_t*)fe_kp_list)[2*i + 1] * y_scale_fct;

          ((int32_t*)fe_pruned_kp_list)[2*i]     = ((int32_t*)fe_kp_list)[2*i];
          ((int32_t*)fe_pruned_kp_list)[2*i + 1] = ((int32_t*)fe_kp_list)[2*i + 1];
        }

      }

      /*key points in original resolution of 768x384 needs to be transformed and it has to be in original
        fish eye domain for upsampling purpose
      */

#ifdef ENABLE_PROFILE
  t0 = _TSC_read();
#endif

      tiadalg_sparse_upsampling_c66((int8_t*)&desc_buf[0] ,
                                    (img_width >> 2),
                                    (img_height >> 2)*(img_width >> 2),
                                    64, /*descriptor size in terms of int16_t*/
                                    weights, /*filter coefficients*/
                                    (int16_t*)bias, /*filter biases*/
                                    fe_pruned_kp_list,
                                    num_key_points,
                                    scratch_buf,is_scratch_filled,
                                    0,
                                    8,
                                    8,
                                    0, // for 16 bit desc, output scale is -1, otherwise zero
                                    (VL_DESC_DATA_TYPE*)inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf,
                                    TIADALG_DATA_TYPE_S08,
                                    (sizeof(VL_DESC_DATA_TYPE) == 1 ? TIADALG_DATA_TYPE_U08 : TIADALG_DATA_TYPE_U16));

#ifdef ENABLE_PROFILE
  t1 = _TSC_read();
  printf("up sampling cycle => per key point %lld \n", (t1-t0)/num_key_points/64);
#endif

      free(fe_kp_list);
      free(fe_pruned_kp_list);


      testcaseParams->num_frame_feat = num_key_points;
      inArgs.feat_q_factor                    = 1;
      is_scratch_filled =0x1;

#ifdef DUMP_BIN_KP_DESC
      sprintf(file_name,"%s_%010d.bin",testcaseParams->ext_feat_file_name,frameIdx);
      printf("writing the file %s \n",file_name);
      tiadalg_write_bin_file(inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_FEAT]->bufPlanes[0].buf,
                                  num_key_points*sizeof(tiadalg_ext_feat),file_name);

      sprintf(file_name,"%s_%010d.bin",testcaseParams->ext_desc_file_name,frameIdx);
      printf("writing the file %s \n",file_name);
      tiadalg_write_bin_file(inBufs.bufDesc[TIADALG_EL_IN_BUFDESC_EXTERNAL_DESC]->bufPlanes[0].buf,
                                  num_key_points*sizeof(tiadalg_feat_desc),file_name);
#endif

#endif

      inArgs.p3p_params                       = testcaseParams->p3p_params;
      inArgs.search_range[0]                  = testcaseParams->search_range[0];
      inArgs.search_range[1]                  = testcaseParams->search_range[1];
      inArgs.search_range[2]                  = testcaseParams->search_range[2];
      inArgs.num_cur_feat                     = testcaseParams->num_frame_feat;
      inArgs.en_pose_filtering                = testcaseParams->en_pose_filtering;
      inArgs.est_loc[0]                       = testcaseParams->est_loc[0];
      inArgs.est_loc[1]                       = testcaseParams->est_loc[1];
      inArgs.est_loc[2]                       = testcaseParams->est_loc[2];

      if(frameIdx == testcaseParams->start_frame)
      {
        inArgs.en_reset                         = 1;
      }
      else
      {
        inArgs.en_reset                         = 0;
      }

#ifdef ENABLE_PROFILE
      t0 = _TSC_read();
#endif

#ifdef ENABLE_PRINT_LOG
      printf("  Process call has started \n");
#endif
      status = handle->ivision->algProcess((IVISION_Handle)handle, &inBufs,
               NULL, (IVISION_InArgs *)&inArgs,(IVISION_OutArgs *)&outArgs);

#ifdef ENABLE_PROFILE
      t1 = _TSC_read();
      printf(" Cycles => per frame %lld \n", t1-t0);
#endif

#ifdef ENABLE_PRINT_LOG
      printf("Estimate roation matrix is %f,%f,%f,%f,%f,%f,%f,%f,%f\n",\
                outArgs.rotation[0],outArgs.rotation[1],outArgs.rotation[2],\
                outArgs.rotation[3],outArgs.rotation[4],outArgs.rotation[5],\
                outArgs.rotation[6],outArgs.rotation[7],outArgs.rotation[8]
                );

      printf("Estimate translation matrix is %f,%f,%f\n",\
                outArgs.translation[0],outArgs.translation[1],outArgs.translation[2]
                );
#endif

      if(log_file != NULL){
        sprintf(file_name,"%010d",frameIdx);
        fprintf(log_file,"%s",file_name);
        fprintf(log_file," %f %f %f ",outArgs.angle[0],outArgs.angle[1],outArgs.angle[2]);
        fprintf(log_file,"%f %f %f\n",outArgs.translation[0],outArgs.translation[1],outArgs.translation[2]);
      }

      frameIdx += testcaseParams->skip_frame;

      if(status != IALG_EOK)
      {
        PRINT_ERRORE_MSG();
        goto EXIT_LOOP;
      }
#ifdef ENABLE_PRINT_LOG
      printf("Processing Completed for frame \n");
#endif
    }

    if(log_file != NULL){
      fclose(log_file);
    }

  }

EXIT_LOOP:

  if(file != NULL)
  {
    free(file);
  }

  if(fpOut)
  {
    fclose(fpOut);
  }

  status = handle->ivision->ialg.algFree((IALG_Handle)(handle), memRec);

  if(status != IALG_EOK)
  {
    PRINT_ERRORE_MSG();
  }

  /* Here Free memory for all the mem records */
  //status = TestApp_FreeMemRecords(memRec,numMemRec);

  if(status != IALG_EOK)
  {
    PRINT_ERRORE_MSG();
  }

  if ( memRec != NULL )
  {
    free(memRec);
  }

  return status;
}

int32_t main(int32_t argc, char *argv[])
{
  int32_t status = 0;
  char configFile[TIADALG_FILE_NAME_SIZE]={0};

#if (!HOST_EMULATION)
  tiadalg_init_cache();
#endif

  if (argc > 1)
  {
    strcpy(configFile, argv[1]) ;
  }
  else
  {
    strcpy(configFile,"/ti/j7/workarea/tiadalg/tiadalg_visual_localization/test/vl.cfg");
  }

  status = readparamfile(configFile, &gsTokenMap_EL[0]) ;

  if(status == -1)
  {
    printf("Parser Failed");
    return -1;
  }

#ifdef USE_HOST_FILE_IO
  ti_fileio_init();
#endif

  /* Validate the algorithm */
  status = elTest(&gConfig_EL);

  return status;
}

#ifndef SPARSE_KP_FLOW
static int32_t vl_alg_fe_correct(int32_t* in_pt_list, int32_t num_points, float* lens_dist_table, int32_t table_len,
                                  int32_t* in_pruned_pt_list, float* out_pt_list, int32_t img_width, int32_t img_height,
                                  int32_t recon_img_width, int32_t recon_img_height)
{
  int32_t i;
  int32_t orig_x_fe,orig_y_fe;
  float in_x_fe,in_y_fe,in_x_fec,in_y_fec;
  float cx = (float)(recon_img_width >> 1);
  float cy = (float)(recon_img_height>> 1);
  int32_t num_out_points=0;
  float r_rect_0,r_rect_1,r_fe_0,r_fe_1,r_fe,r_rect;

  for(i = 0; i < num_points; i++){

    orig_x_fe = in_pt_list[2*i];
    orig_y_fe = in_pt_list[2*i + 1];

    in_x_fe = ((float)(orig_x_fe * recon_img_width))/img_width;
    in_y_fe = ((float)(orig_y_fe * recon_img_height))/img_height;

    r_fe     = sqrtf((in_x_fe - cx)*(in_x_fe - cx) + (in_y_fe - cy)*(in_y_fe - cy));

    r_fe_1   = (ceilf(r_fe) < (table_len - 1.0) ? ceilf(r_fe) : (table_len - 1.0));
    r_fe_1   = (0.f > r_fe_1) ? 0.0f : r_fe_1;
    r_fe_0   = (r_fe_1 - 1);
    r_fe_0   = (0.f > r_fe_0) ? 0.0f : r_fe_0;

    r_rect_1   = lens_dist_table[(int32_t)r_fe_1];
    r_rect_0   = lens_dist_table[(int32_t)r_fe_0];

    vl_alg_interpolate(r_fe,r_fe_0,r_fe_1,r_rect_0,r_rect_1,&r_rect);

    in_x_fec  = cx + ((r_rect/r_fe)*(in_x_fe - cx));
    in_y_fec  = cy + ((r_rect/r_fe)*(in_y_fe - cy));
    if((in_x_fec > 0) && (in_x_fec < recon_img_width) && (in_y_fec > 0) && (in_y_fec < recon_img_height)){
      out_pt_list[2*num_out_points] = in_x_fec;
      out_pt_list[2*num_out_points + 1] = in_y_fec;
      in_pruned_pt_list[2*num_out_points] = orig_x_fe;
      in_pruned_pt_list[2*num_out_points + 1] = orig_y_fe;
      num_out_points++;
    }
  }
  return(num_out_points);
}

static inline uint8_t vl_alg_interpolate(float xPosition, float xRangeLower, float xRangeHigher, float yRangeLower,
                                float yRangeHigher, float *yInterpolated)
{
  if (xPosition >= xRangeLower && xPosition <= xRangeHigher) {
    float locXPosition = xPosition;
    float locXRangeLower = xRangeLower;
    float locXRangeHigher = xRangeHigher;
    float locYRangeLower = yRangeLower;
    float locYRangeHigher = yRangeHigher;
    float locYInterpolated;
    float weight1 = 1.0 - (locXPosition - locXRangeLower) / (locXRangeHigher - locXRangeLower);
    float weight2 = 1.0 - (locXRangeHigher - locXPosition) / (locXRangeHigher - locXRangeLower);

    locYInterpolated = weight1 * locYRangeLower + weight2 * locYRangeHigher;
    *yInterpolated = (float)(locYInterpolated);
    return true;
  }
  else {
    return false;
  }
}
#endif
