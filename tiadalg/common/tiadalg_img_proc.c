/*
*
* Copyright (c) {2015 - 2018} Texas Instruments Incorporated
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
#include <stdlib.h>
#include <tiadalg_interface.h>
#include <string.h>
#include <./../common/tiadalg_test_interface.h>
#include <./../common/tiadalg_alg_int_interface.h>

/*BMP image read will happen from test application, hence local malloc/free is done*/
/*Read bmp file to bmp context*/
#ifdef HOST_EMULATION
/*
In host emulation test application build , no c6x or c7x file is included. Hence just defining these two macros locally 
*/
#include <stdlib.h>
#define _mem2(ptr)              (*((uint16_t *) (ptr)))
#define _mem4(ptr)              (*((uint32_t *) (ptr)))
#endif

int32_t tiadalg_read_bmp_context(const char * file_name, tiadalg_buffer_context* bmp_context, void* dst, int32_t readImgData)
{
    /*14 bytes of file header. File related information.*/
    /*Next 40/12 bytes for display related information*/

    int32_t size;
    unsigned char * info; /*maximum size of BMP header is 54 bytes*/
    int32_t dib_header_size;
    int32_t data_offset;
    int32_t tot_header_size;

    TI_FILE * fp = FOPEN(file_name, "rb");

    info = (unsigned char *)malloc(54);
    memset(info,0x0,54);

    if (fp == NULL){
      free(info);
      printf("Input BMP file could not be read \n");
      return(TIADALG_IN_PRM_ERR);
    }

    FREAD(info, sizeof(unsigned char), 26, fp); /*intial 26 bytes is common for diferent BMP format*/
    dib_header_size = _mem4(&info[14]);

    /* First two bytes can be checked for either 'BM' or 'BA', 'CI', 'CP', 'IC', 'PT' */
    if (dib_header_size == 12){
      bmp_context->width = _mem2(&info[18]);
      bmp_context->pitch = ((bmp_context->pitch + 3) >> 2) << 2;
      bmp_context->height = _mem2(&info[20]);
      /*BMP assumes BGR interleaved as one plane with each element being 24 bits*/
      bmp_context->num_planes = (_mem2(&info[22])) * 3;
      bmp_context->bytes_per_element = ((_mem2(&info[24])) >> 3)/3;
      tot_header_size = 26;
    }else if(dib_header_size == 40){
      FREAD(&info[26], sizeof(unsigned char), 18, fp);  /*Complete reading of 54 (26+18) bytes by reading remaining 18 bytes*/

      bmp_context->width = _mem4(&info[18]);
      /* Number of plane is assumed to be 3*/
      /*BMP assumes BGR interleaved as one plane with each element being 24 bits*/
      bmp_context->num_planes = (_mem2(&info[26]))*3;
      bmp_context->pitch = ((bmp_context->width*bmp_context->num_planes + 3) >> 2) << 2;
      bmp_context->height = _mem4(&info[22]);
      /* Number of plane is assumed to be 3 and bytes per element is assumed to be 1*/
      bmp_context->bytes_per_element = ((_mem2(&info[28])) >> 3)/3;
      tot_header_size = 54;
    }else{
      /*invalid bmp file encountered*/
      free(info);
      printf("Invalid bmp file encountered \n");
      return(TIADALG_IN_PRM_ERR);
    }


    size = (_mem4(&info[2]) - tot_header_size);

    if(size != (bmp_context->pitch * bmp_context->height * bmp_context->bytes_per_element)){
      free(info);
      printf("Input BMP size is not crrect \n");
      return TIADALG_PROCESS_FAILURE;
    }


    data_offset = _mem4(&info[10]);

    FSEEK(fp, data_offset, SEEK_SET);

    if((dst == NULL) && (readImgData == 1)){
      bmp_context->data = malloc(size); // this malloc will be freed when bmp context is released.
      bmp_context->is_user_provided_data_ptr = 0;
    }else if ((dst == NULL) && (readImgData == 0)){
      bmp_context->data = NULL; // this malloc will be freed when bmp context is released.
      bmp_context->is_user_provided_data_ptr = 0;
    }else{
      bmp_context->data = dst;/*when destination pointer is provided from outside then bmp_context can be local variable*/
      bmp_context->is_user_provided_data_ptr = 1;
    }

    if(bmp_context->data == NULL){
      free(info);
      printf("Input BMP data pointer is NULL \n");
      return(TIADALG_PROCESS_FAILURE);
    }

    if((bmp_context->pitch == bmp_context->width*bmp_context->num_planes) || (bmp_context->is_user_provided_data_ptr == 0)){
      FREAD(bmp_context->data, sizeof(unsigned char), size, fp);
    }else{
      char dummy[4];
      int32_t i;
      // if it is user provided pointer, and pitch is not same as width then dont assume pitch.
      for(i = 0; i < bmp_context->height; i++){
        FREAD(&(((uint8_t*)bmp_context->data)[i*bmp_context->width*bmp_context->num_planes]), sizeof(unsigned char), bmp_context->width*bmp_context->num_planes, fp);
        /*As making row width in bmp file multiple of 4*/
        FREAD(dummy, sizeof(unsigned char), bmp_context->pitch - bmp_context->width*bmp_context->num_planes, fp);
      }
    }

    FCLOSE(fp);

    bmp_context->plane_pitch = 0x1;
    bmp_context->element_pitch = 0x3;
    free(info);
    return(TIADALG_PROCESS_SUCCESS);
}

/*out data is not allocated inside this API, as allocation needs to be done as per device configuration e.g. cached or non cached or L1/L2 etc*/
/*That is why allocation of the real processing buffer has been kept outside of any of these APIs*/
/* Basically copies the data from BMP context to another buffer
*/
int32_t tiadalg_img_load_from_bmp_context(tiadalg_buffer_context* bmp_context,
                                  int32_t start_x, int32_t start_y,
                                  int32_t img_height,
                                  int32_t roi_width, int32_t roi_height,
                                  int32_t num_planes, void* out_data)
{
  int32_t i,j,k;
  int32_t cur_in_offset;
  uint8_t cur_pixel;
  int32_t ii,jj,kk; /*indices in bmp buffer*/


  /*Currently only 8 bit output is supported*/
  /*Currently only de-interleaved output is supported*/
  for(k = 0; k < num_planes; k++){
    for(i = 0; i < roi_height; i++){
      for(j = 0; j < roi_width; j++){

        /* In BMP file data is stored in reverse direction, and it is BGR inteleaved 24 bit per pixel*/
        /* output data is in RGB format*/
        ii = img_height - 1 - (i + start_y);
        jj = (j + start_x);
        kk = num_planes - 1 - k;

        cur_in_offset =  (ii*bmp_context->width + jj) * bmp_context->element_pitch;

        cur_pixel = ((uint8_t*)bmp_context->data)[cur_in_offset + kk*bmp_context->plane_pitch];

        /*All the plane is deinteleaved and seperated by 'roi_width * roi_height' pixels*/
        ((uint8_t*)out_data)[k * roi_width * roi_height + i * roi_width + j] = cur_pixel;
      }
    }
  }

  return TIADALG_PROCESS_SUCCESS;
}

/*in_data is not allocated inside this API, as allocation needs to be done as per device configuration e.g. cached or non cached or L1/L2 etc*/
/*That is why allocation of the real processing buffer has been kept outside of any of these APIs*/
/* Basically copies the data in_data to bmp_context
*/
int32_t tiadalg_img_save_to_bmp_context(tiadalg_buffer_context* bmp_context,
                                  int32_t start_x, int32_t start_y,
                                  int32_t img_width, int32_t img_height,
                                  int32_t roi_width, int32_t roi_height,
                                  int32_t num_planes, void* in_data)
{
  int32_t i,j,k;
  int32_t ii,jj,kk;
  uint8_t cur_pixel;
  int32_t in_data_plane_pitch;

  /*Currently only 8 bit output is supported*/
  /*Currently only de-interleaved output is supported*/

  bmp_context->width = roi_width;
  bmp_context->height = roi_height;
  /*BMP assumes BGR interleaved as one plane with each element being 24 bits. Hence in BMP header one plane is encoded.
    whereas in local bmp context num_planes=3
  */
  bmp_context->num_planes = 1;
  bmp_context->bytes_per_element = 3;
  bmp_context->plane_pitch = 1;
  bmp_context->element_pitch = 3;

  if(num_planes == 1){
    /*this means only one plane of data is given by user so repeat that data in all three planes*/
    in_data_plane_pitch = 0;
  }else{
    /*this means multiple plane data is given by user so use acctual plane pitch*/
    in_data_plane_pitch =  img_width * img_height;
  }

  for(k = 0; k < 3; k++){
    for(i = 0; i < roi_height; i++){
      for(j = 0; j < roi_width; j++){

        /*in BMP file data is stored in BGR format, and flipped form */
        kk = num_planes - 1 - k;
        ii = img_height - 1 - (i + start_y);
        jj = (j + start_x);

        cur_pixel = ((uint8_t*)in_data)[kk * in_data_plane_pitch + ii * img_width + jj];

        ((uint8_t*)bmp_context->data)[(i*roi_width + j)*bmp_context->element_pitch +\
        k*bmp_context->plane_pitch] = cur_pixel;

      }
    }
  }

  return TIADALG_PROCESS_SUCCESS;
}

/*creates a dummy bmp context*/
int32_t tiadalg_dummy_bmp_context(tiadalg_buffer_context* bmp_context, int32_t img_width, int32_t img_height)
{
  int32_t size;

  bmp_context->width = img_width;
  bmp_context->height = img_height;
  /*irrespective of number of plane as 1 or 3 , data buffer has to be allocated for 3 planes.
    as BGR interleave is assumed as one plane for bmp files.
    one element is one BGR pixel, total 3 bytes per element and one plane
  */
  bmp_context->num_planes = 1;

  bmp_context->bytes_per_element = 3;

  size = bmp_context->width * bmp_context->height * bmp_context->num_planes * bmp_context->bytes_per_element;

  bmp_context->data = malloc(size);

  memset(bmp_context->data,0x0,size);

  if(bmp_context->data == NULL){
    printf("BMP data pointer is NULL \n");
    return(TIADALG_PROCESS_FAILURE);
  }

  return(TIADALG_PROCESS_SUCCESS);
}
/*Save bmp context to bmp file*/
int32_t tiadalg_save_bmp_context(const char * file_name, tiadalg_buffer_context* bmp_context, void* src)
{
  uint8_t *info_ddr;

  uint8_t info[54] = {0x42, 0x4D, 0x36, 0x80, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
                            0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x80, 0x0D, 0x00, 0x74, 0x12, 0x00, 0x00, 0x74, 0x12, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  TI_FILE*  fp;
  int32_t size;

  info_ddr = (uint8_t*)malloc(54);

  size = bmp_context->pitch * bmp_context->height * bmp_context->bytes_per_element + 54;

  _mem4(&info[18]) = bmp_context->width;
  _mem4(&info[22]) = bmp_context->height;
  _mem2(&info[26])= (uint16_t)1; /*number of planes coded as only one*/
  _mem2(&info[28])= (uint16_t)(bmp_context->bytes_per_element * 8 * bmp_context->num_planes);
  _mem4(&info[2]) = size;

  fp = FOPEN(file_name,"wb");

  if(fp == NULL){
    printf("BMP file could not be opened \n");
    return(TIADALG_PROCESS_FAILURE);
  }

  /*Fast file io need src/dest needs to be in ddr. Hence created a dummy buffer for that*/

  memcpy(info_ddr,info,54);
  FWRITE(info_ddr,1,54,fp);
  FCLOSE(fp);

  fp = FOPEN(file_name,"ab");

  if(fp == NULL){
    free(info_ddr);
    printf("BMP file could not be opened \n");
    return(TIADALG_PROCESS_FAILURE);
  }

  if((src == NULL) || (bmp_context->pitch == bmp_context->width)){
    FWRITE(bmp_context->data, 1, (size - 54), fp);
  }else{
    char dummy[4] = {0,0,0,0}; // need not to be zero
    int32_t i;
    // if it is user provided pointer, and pitch is not same as width then dont assume pitch.
    for(i = 0; i < bmp_context->height; i++){
      FWRITE(&(((uint8_t*)src)[i*bmp_context->width*bmp_context->num_planes]), sizeof(unsigned char), bmp_context->width*bmp_context->num_planes, fp);
      /*for making row width in bmp file multiple of 4*/
      FWRITE(dummy, sizeof(unsigned char), bmp_context->pitch - bmp_context->width*bmp_context->num_planes, fp);
    }

  }

  FCLOSE(fp);
  free(info_ddr);

  return TIADALG_PROCESS_SUCCESS;
}

int32_t tiadalg_bmp_release_context(tiadalg_buffer_context* bmp_context){

  if (bmp_context == NULL || bmp_context->data == NULL){
    printf("BMP context is NULL \n");
    return(TIADALG_IN_BUF_ERR);
  }else{
    if(bmp_context->is_user_provided_data_ptr == 0){
      free(bmp_context->data);
    }
    return(TIADALG_PROCESS_SUCCESS);
  }

}

int32_t tiadalg_save_bmp_image(int32_t width, int32_t height, int32_t num_planes, uint8_t * data, char* file_name){
  tiadalg_buffer_context bmp_context;

  /*Create bmp context, along with allocate memory for image buffer*/
  tiadalg_dummy_bmp_context(&bmp_context,width,height);

  /* copy data from data pointer to image buffer*/
  tiadalg_img_save_to_bmp_context(&bmp_context,
    0, 0,
    width, height,
    width, height,
    num_planes, data);

  /*File save data pointer buffer to file*/
  tiadalg_save_bmp_context(file_name, &bmp_context, NULL);

  /*Release memory for image buffer*/
  tiadalg_bmp_release_context(&bmp_context);

  return(TIADALG_PROCESS_SUCCESS);

}

