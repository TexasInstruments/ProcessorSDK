/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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


/** @file tidl_lidar_preproc
*
*  @brief  This file contains utility functions for TI DL input tensor preperation.
*
*
*  @date   Aug 2021
*
*/

#ifdef BUILD_LIDAR_PREPROC

#include "tidl_lidar_preproc.h"
#define DUMP_IN_BUF_BIN_DATA

int32_t tidl_prepareLidarInput(TI_FILE * fp1, tidl_net_config *params, sTIDL_IOBufDesc_t * ioPrms,
                              sTIDLRT_Tensor_t   *ins[], int32_t frameCount, sTIDL_Network_t * net)
{
  uint16_t numBuffs = 0;
  int32_t nw_max_num_voxels, cur_num_voxels; /*nw_max_num_voxels -> onnx network trained for maximum number of voxels*/
  int32_t max_num_points; /*Maximum number of lidar 3d points present in a given data file in dataset*/
  int32_t cur_num_points;
  int32_t num_feat_per_voxel; /*Number of features to be computed per voxel*/

  int32_t dataIdx;
  int32_t indicesIdx;
  int32_t voxelIdx;

  int32_t i,j;

  {
    /*point pillars input tensor preperation from lidar raw data presnet in binary file*/
    for(j = 0; j < TIDL_NUM_MAX_LAYERS; j++)
    {
      if(net->TIDLLayers[j].layerType == TIDL_ScatterElementsLayer)
      {
        break;
      }
    }

    /*Finding out which input buffer belong to which data*/
    for(i = 0; i < ioPrms->numInputBuf; i++)
    {
      if(strcmp((char*)ins[i]->name,(char*)net->TIDLLayers[j].layerParams.scatterElementsParams.dataTensorName) == 0)
      {
        dataIdx = i;
      }
      else if(strcmp((char*)ins[i]->name,(char*)net->TIDLLayers[j].layerParams.scatterElementsParams.indicesTensorName) == 0)
      {
        indicesIdx = i;
      }
      else
      {
        voxelIdx = i;
      }
    }

    nw_max_num_voxels = ioPrms->inWidth[voxelIdx];
    num_feat_per_voxel= ioPrms->inNumChannels[voxelIdx];
    max_num_points    = 40000; /*imperical value for maximum points in one lidar capture*/

    void* inp_pt_cloud = malloc(4*sizeof(float)*max_num_points);
    voxel_info_t voxel_info;

    for(i = 0; i < TIDL_NUM_MAX_LAYERS; i++)
    {
      if(net->TIDLLayers[i].layerType == TIDL_DetectionOutputLayer)
      {
        break;
      }
    }

    /*Prepearing the voxel info*/
    voxel_info.min_x               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[0][0];
    voxel_info.min_y               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[1][0];
    voxel_info.min_z               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[2][0];
    voxel_info.max_x               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[0][1];
    voxel_info.max_y               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[1][1];
    voxel_info.max_z               = net->TIDLLayers[i].layerParams.detectOutParams.point3dRange[2][1];

    voxel_info.voxel_size_x        = net->TIDLLayers[i].layerParams.detectOutParams.voxelSize[0];
    voxel_info.voxel_size_y        = net->TIDLLayers[i].layerParams.detectOutParams.voxelSize[1];
    voxel_info.voxel_size_z        = net->TIDLLayers[i].layerParams.detectOutParams.voxelSize[2];
    voxel_info.one_by_voxel_size_x = 1.0f/voxel_info.voxel_size_x;
    voxel_info.one_by_voxel_size_y = 1.0f/voxel_info.voxel_size_y;
    voxel_info.num_voxel_x         = (voxel_info.max_x - voxel_info.min_x)/voxel_info.voxel_size_x;
    voxel_info.num_voxel_y         = (voxel_info.max_y - voxel_info.min_y)/voxel_info.voxel_size_y;
    voxel_info.nw_max_num_voxels   = nw_max_num_voxels;
    voxel_info.max_points_per_voxel= net->TIDLLayers[i].layerParams.detectOutParams.maxPointsPerVoxel;

    /*Individual binary file (inFileFormat = 1)  or list of binary files (inFileFormat = 5) are supproted*/
    /*And only float version is supported*/
    int32_t bytes_read;
    /*
      inFileFormat 0 --> compressed jpeg/png images
                   1 --> raw data
                   2 --> list of compressed jpeg/png images
                   4 --> random input
                   5 --> list of raw data
    */
    if (params->inFileFormat == 5)
    {
      /*list of binary files*/
      char dataFileName[500];
      getFileNameAndIdxFromList((char*)params->inData, frameCount, numBuffs, dataFileName);
      fp1 = fopen(dataFileName,"rb");
      if(fp1 == NULL)
      {
        printf("Could not open the file %s \n",dataFileName);
        TIDLTB_ASSERT_EXIT(0);
      }
      /*Read maximum number of points irrespective of file size*/
      bytes_read = FREAD(inp_pt_cloud,1,max_num_points*4*sizeof(float), fp1);
    }
    else
    {
      TIDLTB_ASSERT_EXIT(0);
    }

    cur_num_points          = bytes_read/(4*sizeof(float));

    if(cur_num_points > max_num_points)
    {
      assert(0);
    }

    void* scratch_2         = malloc(cur_num_points*sizeof(uint32_t));
    void* scratch_1         = malloc(cur_num_points*sizeof(uint32_t));
    void* num_points_list   = malloc(nw_max_num_voxels*sizeof(int16_t));
    float* voxel_data       = (float*)malloc(nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel*sizeof(float));

    /*Reseting the voxel data buffer, which is dependent on data type*/
    memset(ins[voxelIdx]->ptr, 0x0,
          nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel*tidltb_getDatElementSize(ins[voxelIdx]->elementType));

    /*temprorary voxel data to get prepeared in float precision*/
    memset(voxel_data, 0x0, nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel*sizeof(float));

    int32_t voxel_data_type;

    if(ins[voxelIdx]->elementType == TIDL_SignedChar)
    {
      voxel_data_type = TIADALG_DATA_TYPE_S08;
    }
    else if (ins[voxelIdx]->elementType == TIDL_SignedShort)
    {
      voxel_data_type = TIADALG_DATA_TYPE_S16;
    }
    else if(ins[voxelIdx]->elementType == TIDL_SinglePrecFloat)
    {
      voxel_data_type = TIADALG_DATA_TYPE_F32;
    }
    else
    {
      printf("not supprted data type for voxel \n");
    }

    voxel_data_type = TIADALG_DATA_TYPE_F32;

    cur_num_voxels =
    tiadalg_voxelization_cn((float*)inp_pt_cloud,
                            cur_num_points,
                            (int32_t*)scratch_1,
                            (int32_t*)scratch_2,
                            &voxel_info,
                            (void*)voxel_data,
                            (int32_t*)ins[indicesIdx]->ptr,
                            (int16_t*)num_points_list,
                            32,
                            voxel_data_type);

  if(cur_num_voxels > nw_max_num_voxels)
  {
    assert(0);
  }
#if 0
  int32_t linePitch = voxel_info.nw_max_num_voxels;
  int32_t channelPitch = voxel_info.max_points_per_voxel * linePitch;

  FILE* fp = fopen("voxel_data.txt", "w");
  for(i = 0; i < cur_num_voxels; i++)
  {
    //for(j = 0; j< ((int16_t*)num_points_list)[i]; j++)
    for(j = 0; j< voxel_info.max_points_per_voxel; j++)
    {
      fprintf(fp," %.2f %.2f %.2f %.2f \n", (float)voxel_data[i + j*linePitch + channelPitch*0],
                                         (float)voxel_data[i + j*linePitch + channelPitch*1],
                                         (float)voxel_data[i + j*linePitch + channelPitch*2],
                                         (float)voxel_data[i + j*linePitch + channelPitch*3]);

    }
  }
  fclose(fp);
#endif

    tiadalg_voxel_feature_compute_cn((void*)voxel_data,
                                     (int32_t*)ins[indicesIdx]->ptr,
                                     (int16_t*)num_points_list,
                                     &voxel_info,
                                     cur_num_voxels,
                                     32,
                                     voxel_data_type);

#if 0
  //int32_t linePitch = voxel_info.nw_max_num_voxels;
  //int32_t channelPitch = voxel_info.max_points_per_voxel * linePitch;

  FILE* fp1 = fopen("voxel_raw_features.txt", "w");
  for(i = 0; i < cur_num_voxels; i++)
  {
    fprintf(fp1,"voxel no %d \n", i);
    //for(j = 0; j< ((int16_t*)num_points_list)[i]; j++)
    for(j = 0; j< voxel_info.max_points_per_voxel; j++)
    {
      fprintf(fp1," %.2f %.2f %.2f %.2f", (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*0]),
                                         (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*1]),
                                         (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*2]),
                                         (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*3]));

      fprintf(fp1," %.2f %.2f %.2f %.2f %.2f \n", (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*4]),
                                                (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*5]),
                                                (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*6]),
                                                (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*7]),
                                                (float)(int32_t)(voxel_data[i + j*linePitch + channelPitch*8]));
    }
  }
  fclose(fp1);
#endif

    if(ins[voxelIdx]->elementType == TIDL_SignedChar)
    {
      for(i = 0; i < nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel; i++)
      {
        ((int8_t*)ins[voxelIdx]->ptr)[i] = (int8_t)voxel_data[i];
      }
    }
    else if (ins[voxelIdx]->elementType == TIDL_SignedShort)
    {
      for(i = 0; i < nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel; i++)
      {
        ((int16_t*)ins[voxelIdx]->ptr)[i] = (int16_t)voxel_data[i];
      }
    }
    else if(ins[voxelIdx]->elementType == TIDL_SinglePrecFloat)
    {
      for(i = 0; i < nw_max_num_voxels*voxel_info.max_points_per_voxel*num_feat_per_voxel; i++)
      {
        ((float*)ins[voxelIdx]->ptr)[i] = (float)(int32_t)voxel_data[i];
      }
    }
    else
    {
      printf("not supported data type for voxel \n");
    }

    memset(ins[dataIdx]->ptr,
          0x0,
          ioPrms->inNumChannels[dataIdx] * ioPrms->inWidth[dataIdx] *
          ioPrms->inHeight[dataIdx]*
          tidltb_getDatElementSize(ins[dataIdx]->elementType));

    /*indices can be either int32 or float32. by default it is generated as int32_t.
      Hence in float scenario just copy the data at same place.
    */
    if(ins[indicesIdx]->elementType == TIDL_SinglePrecFloat)
    {
      int32_t tempIndx;

      for(i = 0; i < ins[indicesIdx]->pitch[0]; i++)
      {
        if(i < cur_num_voxels)
        {
          tempIndx = ((int32_t*)ins[indicesIdx]->ptr)[i];
          ((float*)ins[indicesIdx]->ptr)[i] = (float)tempIndx;
        }
        else
        {
          ((float*)ins[indicesIdx]->ptr)[i] = -1;
        }

      }
    }
    else
    {
      /*Number of valid voxels will be less than equal to current num of voxels, hence setting the invalid index for
        invalid voxels
      */
      memset(((int32_t*)ins[indicesIdx]->ptr) + cur_num_voxels,
              0xff,
              (ins[indicesIdx]->pitch[0] - cur_num_voxels)*tidltb_getDatElementSize(ins[indicesIdx]->elementType));

    }
    // how to get 64 from some parameter
    for(i = 1; i< 64; i++)
    {
      memcpy(((int32_t*)ins[indicesIdx]->ptr) + i*ins[indicesIdx]->pitch[0],
              (int32_t*)ins[indicesIdx]->ptr,
              ins[indicesIdx]->pitch[0]*tidltb_getDatElementSize(ins[indicesIdx]->elementType));
    }

    for(numBuffs = 0; numBuffs < ioPrms->numInputBuf; numBuffs++)
    {
      ins[numBuffs]->scale = gIOParams.inTensorScale[numBuffs];
    }

#ifdef DUMP_IN_BUF_BIN_DATA

    FILE* fp0 = fopen("input0.bin","wb");
    FILE* fp1 = fopen("input1.bin","wb");
    FILE* fp2 = fopen("input2.bin","wb");

    if((fp0 == NULL) || (fp1 == NULL) || (fp2 == NULL) )
    {
      printf("Could not open the file \n");
      exit(0);
    }

    int32_t indiceBufSize = ins[indicesIdx]->pitch[0]*ins[indicesIdx]->dimValues[2]*
                            tidltb_getDatElementSize(ins[indicesIdx]->elementType);
    int32_t dataBufSize = ioPrms->inNumChannels[dataIdx] * ioPrms->inWidth[dataIdx] *
                          ioPrms->inHeight[dataIdx]*
                          tidltb_getDatElementSize(ins[dataIdx]->elementType);
    int32_t voxelBufSize = ioPrms->inNumChannels[voxelIdx] * ioPrms->inWidth[voxelIdx] *
                          ioPrms->inHeight[voxelIdx]*
                          tidltb_getDatElementSize(ins[voxelIdx]->elementType);
    for(numBuffs = 0; numBuffs < ioPrms->numInputBuf; numBuffs++)
    {
      if(numBuffs == voxelIdx)
      {
        fwrite(ins[voxelIdx]->ptr, voxelBufSize, 1,fp0);
      }
      else if(numBuffs == dataIdx)
      {
        fwrite(ins[dataIdx]->ptr, dataBufSize,1,fp1);
      }
      else
      {
        fwrite(ins[indicesIdx]->ptr, indiceBufSize,1,fp2);
      }
    }
    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
#endif

    free(scratch_2);
    free(inp_pt_cloud);
    free(scratch_1);
    free(num_points_list);
    free(voxel_data);

  }

  return 0;

}

#endif
