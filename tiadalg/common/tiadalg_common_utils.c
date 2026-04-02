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

#include <../common/tiadalg_common_utils.h>
#include <../common/ti_file_io.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _TMS320C6600
static void tiadalg_set_mar();
#endif

static char* tiadalg_line_string_parse(char* in_str, char delim, char* out_str);

int32_t tiadalg_read_f32_data_from_file(float* dest, int32_t num_elements, const char * filename)
{
  TI_FILE* fp = FOPEN(filename, "r");
  char line_str[2048];
  char token[2048];
  char* pline_str = line_str;
  int32_t num_elements_read = 0;

  if(fp==NULL)
  {
      printf("# ERROR: Unable to open config file [%s]\n", filename);
      exit(0);
  }

  while((FGETS(line_str, sizeof(line_str), fp)!=NULL) && (num_elements_read < num_elements))
  {
    pline_str = line_str;
    pline_str[strlen(line_str)] = '\0';
    pline_str = tiadalg_line_string_parse(pline_str, ',', token);

    while((token != NULL) && (token[0] != '\r') && (token[0] != '\0')){
      *dest++ = (float)atof(token);
      num_elements_read++;
      pline_str = tiadalg_line_string_parse(pline_str, ',', token);
    }
  }

  return(num_elements_read);

}

int32_t tiadalg_read_s16_data_from_file(int16_t* dest, int32_t num_elements, const char * filename)
{
  TI_FILE* fp = FOPEN(filename, "r");
  char line_str[1024];
  char token[1024];
  char* pline_str = line_str;
  int32_t num_elements_read = 0;

  if(fp==NULL)
  {
      printf("# ERROR: Unable to open config file [%s]\n", filename);
      exit(0);
  }

  while((FGETS(line_str, sizeof(line_str), fp)!=NULL) && (num_elements_read < num_elements))
  {
    pline_str = line_str;
    pline_str[strlen(line_str)] = '\0';
    pline_str = tiadalg_line_string_parse(pline_str, ',', token);

    while((token != NULL) && (token[0] != '\r') && (token[0] != '\0')){
      *dest++ = (int16_t)atoi(token);
      num_elements_read++;
      pline_str = tiadalg_line_string_parse(pline_str, ',', token);
    }
  }

  return(num_elements_read);
}
int32_t tiadalg_read_s8_data_from_file(int8_t* dest, int32_t num_elements, const char * filename)
{
  TI_FILE* fp = FOPEN(filename, "r");
  char line_str[1024];
  char token[1024];
  char* pline_str = line_str;
  int32_t num_elements_read = 0;

  if(fp==NULL)
  {
      printf("# ERROR: Unable to open config file [%s]\n", filename);
      exit(0);
  }

  while((FGETS(line_str, sizeof(line_str), fp)!=NULL) && (num_elements_read < num_elements))
  {
    pline_str = line_str;
    pline_str[strlen(line_str)] = '\0';
    pline_str = tiadalg_line_string_parse(pline_str, ',', token);

    while((token != NULL) && (token[0] != '\r') && (token[0] != '\0')){
      *dest++ = (int8_t)atoi(token);
      num_elements_read++;
      pline_str = tiadalg_line_string_parse(pline_str, ',', token);
    }
  }

  return(num_elements_read);
}

int32_t tiadalg_comp_f32_data(float* src, float* dst, int32_t num_elements, float tolerance)
{
  int32_t is_pass= 1;
  int32_t i,j;

  for(i=0; i < num_elements; i++){
    if(fabs(src[i] - dst[i]) > tolerance){
      is_pass = 0;

      for(j=0; j < num_elements; j++){
        printf("src value %f, and dest value %f \n", src[j], dst[j]);
      }
      break;
    }
  }

  return(is_pass);
}

int32_t tiadalg_comp_s32_data(int32_t* src, int32_t* dst, int32_t num_elements, int32_t tolerance)
{
  int32_t is_pass= 1;
  int32_t i,j;

  for(i=0; i < num_elements; i++){
    if(abs(src[i] - dst[i]) > tolerance){
      is_pass = 0;

      for(j=0; j < num_elements; j++){
        printf("src value %d, and dest value %d \n", src[j], dst[j]);
      }

      break;
    }
  }

  return(is_pass);
}

int32_t tiadalg_comp_u16_data(uint16_t* src, uint16_t* dst, int32_t num_elements, uint16_t tolerance)
{
  int32_t is_pass= 1;
  int32_t i,j;

  for(i=0; i < num_elements; i++){
    if(abs(src[i] - dst[i]) > tolerance){
      is_pass = 0;

      for(j=0; j < num_elements; j++){
        printf("src value %d, and dest value %d \n", src[j], dst[j]);
      }

      break;
    }
  }

  return(is_pass);
}

int32_t tiadalg_comp_s16_data(int16_t* src, int16_t* dst, int32_t num_elements, int16_t tolerance)
{
  int32_t is_pass= 1;
  int32_t i,j;

  for(i=0; i < num_elements; i++){
    if(abs(src[i] - dst[i]) > tolerance){
      is_pass = 0;

      for(j=0; j < num_elements; j++){
        printf("src value %d, and dest value %d \n", src[j], dst[j]);
      }

      break;
    }
  }

  return(is_pass);
}
#ifdef _TMS320C6600
static void tiadalg_set_mar()
{

  uint32_t i ;
  /* MAR Reg  Add         Cache able Region
  * MAR 128 (0x01848200) 8000 0000h - 80FF FFFFh
  * MAR 133 (0x01848214) 8500 0000h - 85FF FFFFh
  * MAR 134 (0x01848218) 8600 0000h - 86FF FFFFh
  * MAR 144 (0x01848240) 9000 0000h - 90FF FFFFh
  * MAR 159 (0x0184827c) 9F00 0000h - 9FFF FFFFh
  * MAR 160 (0x01848280) A000 0000h - A0FF FFFFh
  */
  uint32_t* MAR = (uint32_t*)0x01848000 ;

  for(i = 128 ;i <= 134 ; i++) /*8000 0000h - 86FF FFFFh*/
  {
    MAR[i] = 0x1;
  }

  for(i = 135 ;i <= 143 ; i++) /*8700 0000h - 8FFF FFFFh*/
  {
    MAR[i] = 0x0;
  }

  for(i = 144 ;i <= 159 ; i++) /*9000 0000h - 9FFF FFFFh*/
  {
    MAR[i] = 0x1;
  }

  return ;
}
#endif
void tiadalg_init_cache()
{
#ifdef _TMS320C6600
  /*--------------------------------------------------------------------------*/
  /* 7-> full,6 -> 1024KB,5 -> 512KB,4-> 256KB,3 -> 128KB,2-> 64KB,0-->0      */
  /* IMPORTANT ::Currently 160 KB of L2 is assumed to be SRAM, hence in final */
  /* application any amount L2 can be configured as SRAM, but DMVAL will not  */
  /* use that.                                                                */
  /*--------------------------------------------------------------------------*/

  *L2CFG  = 3;

  /*-------------------------------------------------------------------------*/
  /* Full L1P is configured as 32KB of cache                                 */
  /* Nothing in L1P is assumed as SRAM, so no dependency from final applicati*/
  /* -on configuration. Only thing if full L1P is configured as cache then   */
  /* standalone and application performance numbers will match.              */
  /*-------------------------------------------------------------------------*/
  *L1PCFG = 4;

  /*--------------------------------------------------------------------------*/
  /* 1 -> 4 KB, 2-> 8KB, 3 ->16 KB, 4 -> 32KB as cache                       */
  /* Currently 16KB as cache has been allocated for L1D, Rest 16 KB is configu*/
  /* red as SRAM. Any change in cache configuration should be done in conjuca*/
  /* of linker command file modification in test application                 */
  /* IMPORTANT :: Here 16 KB of L1 D is assumed as SRAM hence there is depend*/
  /* -dency from final application configuration of L1D. If atleast 16 KB of */
  /* L1D is not configured as SRAM in final application then that will be a  */
  /* functional issue.                                                       */
  /*-------------------------------------------------------------------------*/
  *L1DCFG = 4;

  tiadalg_set_mar() ;
#endif
}

/*C66 compiler strtok was not working*/
static char*  tiadalg_line_string_parse(char* in_str, char delim, char* out_str){
  while((*in_str != '\0') && (*in_str != delim) && (*in_str != ' ') && (*in_str != '\r')&& (*in_str != '\n')){
    *out_str++ = *in_str++;
  }
  *out_str = '\0';
  in_str++;
  return in_str;
}
//#define DISABLE_IO
int32_t tiadalg_read_bin_file(void* dest, int32_t num_bytes, char * file_name)
{
  int32_t bytesRead;
  TI_FILE * fp;
  int32_t size = num_bytes;

  fp = FOPEN(file_name, "rb");

  if(fp == NULL)
  {
    printf("%s file could not be opened \n",file_name);
    return (0);
  }

  if(size == -1){ /*in this scenario read complete file*/
    FSEEK(fp,0L,SEEK_END);
    size = FTELL(fp);
    FSEEK(fp,0L,SEEK_SET);
  }
#ifndef DISABLE_IO
  if (dest != NULL){ /*if destination pointer is NULL then this API can be used to get the file size in bytes*/
    bytesRead = FREAD(dest, 1, size, fp);

    if(bytesRead < size)
    {
      printf("Number of bytes read is not same as requested \n");
      return (0);
    }
  }
#endif
  FCLOSE(fp);

  return (size);
}

int32_t tiadalg_write_bin_file(void* dest, int32_t num_bytes, char * file_name)
{
  int32_t bytesWritten;
  TI_FILE * fp;

  fp = FOPEN(file_name, "wb");

  if(fp == NULL)
  {
    printf("%s file could not be opened \n",file_name);
    return (0);
  }

#ifndef DISABLE_IO
  if (dest != NULL){ /*if destination pointer is NULL then this API can be used to get the file size in bytes*/

    bytesWritten = FWRITE(dest, 1, num_bytes, fp);

    if(bytesWritten != num_bytes)
    {
      printf("Number of bytes written is not same as requested \n");
      return (0);
    }
  }
#endif
  FCLOSE(fp);

  return (bytesWritten);
}

int32_t tiadalg_fill_buffer_random(uint8_t* buf, int32_t size){
  int32_t i;

  for(i = 0; i < size; i++){
    buf[i] = gen_rand(0,255);
  }

  return(size);
}

int32_t tiadalg_fill_buffer_range_random_16u(uint16_t* buf, int32_t size, uint16_t low, uint16_t high){
  int32_t i;

  for(i = 0; i < size; i++){
    buf[i] = gen_rand(low,high);
  }

  return(size);
}

int32_t tiadalg_fill_buffer_range_random_8u(uint8_t* buf, int32_t size, uint8_t low, uint8_t high){
  int32_t i;

  for(i = 0; i < size; i++){
    buf[i] = gen_rand(low,high);
  }

  return(size);
}
#if defined(__C7100__) || defined(__C7120__)
#define ALIGN_DIMS (64 - 1)
#define ALIGN_ADDR (64 - 1)
static char eleType[16][20] =
{
  "8BIT",
  "16BIT",
  "32BIT",
  "64BIT",
  "NA",
  "NA",
  "NA",
  "NA",
  "8BIT_CMPLX_NOSWAP",
  "16BIT_CMPLX_NOSWAP",
  "32BIT_CMPLX_NOSWAP",
  "64BIT_CMPLX_NOSWAP",
  "8BIT_CMPLX_SWAP",
  "16BIT_CMPLX_SWAP",
  "32BIT_CMPLX_SWAP",
  "64BIT_CMPLX_SWAP"
};

static char transposeType[7][10] =
{
 "OFF",
 "8BIT",
 "16BIT",
 "32BIT",
 "64BIT",
 "128BIT",
 "256BIT"
};

static char promoteType[8][15] =
{
 "OFF",
 "2X_ZEROEXT",
 "4X_ZEROEXT",
 "8X_ZEROEXT",
 "NA",
 "2X_SIGNEXT",
 "4X_SIGNEXT",
 "8X_SIGNEXT"
};

static char veclenType[7][10] =
{
 "1ELEM",
 "2ELEMS",
 "4ELEMS",
 "8ELEMS",
 "16ELEMS",
 "32ELEMS",
 "64ELEMS"
};

static char dimfmtType[6][10] =
{
  "1D",
  "2D",
  "3D",
  "4D",
  "5D",
  "6D"
};

static char eldupType[7][10] =
{
  "OFF",
  "2X",
  "4X",
  "8X",
  "16X",
  "32X",
  "64X"
};

static char grpdupType[2][10] =
{
  "OFF",
  "ON"
};

static char decimType[3][10] =
{
  "OFF",
  "2:1",
  "4:1"
};

static char dirType[2][10] =
{
 "INC",
 "DEC"
};

static char decdimType[6][10] =
{
 "DECDIM0",
 "DECDIM1",
 "DECDIM2",
 "DECDIM3",
 "DECDIM4",
 "DECDIM5"
};

static char lezrType[7][15] =
{
  "SE_LEZR_OFF",
  "SE_LEZR_ICNT0",
  "SE_LEZR_ICNT1",
  "SE_LEZR_ICNT2",
  "SE_LEZR_ICNT3",
  "SE_LEZR_ICNT4",
  "SE_LEZR_ICNT5"
};

static char alignType[2][10] =
{
  "NA",
  "A"
};

#ifndef HOST_EMULATION
void displaySEParam(void *baseAddr, __SE_TEMPLATE_v1 seTemplate)
{
    uint32_t baseAddrAlign = 1;
    uint32_t dim1Align = 1;
    uint32_t dim2Align = 1;
    uint32_t dim3Align = 1;
    uint32_t dim4Align = 1;
    uint32_t dim5Align = 1;

    uint32_t ICNT0 = seTemplate.ICNT0;
    uint32_t ICNT1 = seTemplate.ICNT1;
    uint32_t ICNT2 = seTemplate.ICNT2;
    uint32_t ICNT3 = seTemplate.ICNT3;
    uint32_t ICNT4 = seTemplate.ICNT4;
    uint32_t ICNT5 = seTemplate.ICNT5;

    int32_t DIM1 = seTemplate.DIM1;
    int32_t DIM2 = seTemplate.DIM2;
    int32_t DIM3 = seTemplate.DIM3;
    int32_t DIM4 = seTemplate.DIM4;
    int32_t DIM5 = seTemplate.DIM5;

    if(DIM1 & ALIGN_DIMS) { dim1Align = 0; }
    if(DIM2 & ALIGN_DIMS) { dim2Align = 0; }
    if(DIM3 & ALIGN_DIMS) { dim3Align = 0; }
    if(DIM4 & ALIGN_DIMS) { dim4Align = 0; }
    if(DIM5 & ALIGN_DIMS) { dim5Align = 0; }

    if((uint64_t)baseAddr & ALIGN_DIMS) { baseAddrAlign = 0; }

    dim1Align *= baseAddrAlign;
    dim2Align *= baseAddrAlign;
    dim3Align *= baseAddrAlign;
    dim4Align *= baseAddrAlign;
    dim5Align *= baseAddrAlign;

    if((uint64_t)baseAddr & ALIGN_ADDR) { baseAddrAlign = 0; }

    int32_t DECDIM1_WIDTH = seTemplate.DECDIM1_WIDTH;
    int32_t DECDIM2_WIDTH = seTemplate.DECDIM2_WIDTH;
    int32_t LEZR_CNT      = seTemplate.LEZR_CNT;

    printf("-----------------------------------------------------------------------------\n");
    printf("SE open at 0x%016lX - %s(64), (A - aligned, NA - not aligned)\n", (uint64_t)baseAddr, alignType[baseAddrAlign]);
    printf("-----------------------------------------------------------------------------\n");
    printf("DECDIM1   = %-25s | DECDIM1_WIDTH = %d \n", decdimType[seTemplate.DECDIM1], DECDIM1_WIDTH);
    printf("DECDIM2   = %-25s | DECDIM2_WIDTH = %d \n", decdimType[seTemplate.DECDIM2], DECDIM2_WIDTH);
    printf("LEZR      = %-25s | LEZR_CNT = %d \n", lezrType[seTemplate.LEZR], LEZR_CNT);
    printf("DIMFMT    = %-25s | \n", dimfmtType[seTemplate.DIMFMT]);
    printf("ELETYPE   = %-25s | ICNT0 = %-5d | \n", eleType[seTemplate.ELETYPE], ICNT0);
    printf("TRANSPOSE = %-25s | ICNT1 = %-5d | DIM1 = %-5d - %-2s(64)\n", transposeType[seTemplate.TRANSPOSE], ICNT1, DIM1, alignType[dim1Align]);
    printf("PROMOTE   = %-25s | ICNT2 = %-5d | DIM2 = %-5d - %-2s(64)\n", promoteType[seTemplate.PROMOTE], ICNT2, DIM2, alignType[dim2Align]);
    printf("VECLEN    = %-25s | ICNT3 = %-5d | DIM3 = %-5d - %-2s(64)\n", veclenType[seTemplate.VECLEN], ICNT3, DIM3, alignType[dim3Align]);
    printf("ELEDUP    = %-25s | ICNT4 = %-5d | DIM4 = %-5d - %-2s(64)\n", eldupType[seTemplate.ELEDUP], ICNT4, DIM4, alignType[dim4Align]);
    printf("GRPDUP    = %-25s | ICNT5 = %-5d | DIM5 = %-5d - %-2s(64)\n", grpdupType[seTemplate.GRPDUP], ICNT5, DIM5, alignType[dim5Align]);
    printf("DECIM     = %-25s |               |\n", decimType[seTemplate.DECIM]);
    printf("DIR       = %-25s |               |\n", dirType[seTemplate.DIR]);
    printf("-----------------------------------------------------------------------------\n");

    return;
}
#endif
#endif
