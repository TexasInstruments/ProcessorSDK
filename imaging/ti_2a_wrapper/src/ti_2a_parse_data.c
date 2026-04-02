/*
TEXAS INSTRUMENTS TEXT FILE LICENSE

Copyright (c) [2018] – [2019] Texas Instruments Incorporated

All rights reserved not granted herein.

Limited License.

Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.

Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution

Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:

*    No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.

*    any redistribution and use are licensed by TI for use only with TI Devices.

*    Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.

If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:

*    any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.

*    any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.

Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.

DISCLAIMER.

THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* =============================================================================
*                                  INCLUDE FILES
* =========================================================================== */
#include <stdio.h>
#include <idcc.h>
#include "parse_data.h"
#include "dcc_defs.h"

/* =============================================================================
*                                  defines
* =========================================================================== */

/* Macro to include flash AWB calibration data */
/* #define DCC_PARSER_INCLUDE_FLASH_CALB */

/* Macro to include Skin image reference data */
/* #define DCC_PARSER_INCLUDE_SKIN_IMG_REF */

/* Macro to include Skin image reference data */
/* #define DCC_PARSER_INCLUDE_WBMODE_DATA */

#define DCC_PARSER_INCLUDE_ASYMMETRY_LUT_CALC

/* Macro for calculating table pitch */
#define DCC_PARSER_MESH_TABLE_PITCH(w, r) ((((((w)/(r))+1) + 15U) & ~15U) * (4U))

static uint8_t get_uint8(uint8_t** p_bin);
#define get_int8(a)  (int8_t)get_uint8(a)

static uint16_t get_uint16(uint8_t** p_bin);
#define get_int16(a)  (int16_t)get_uint16(a)

static uint32_t get_uint32(uint8_t** p_bin);
#define get_int32(a)  (int32_t)get_uint32(a)

/* ========================================================================== */
/**
*  get_uint32()
*  little endian
*
*  @param   ptr - pointer to dcc data
*
*  @param
*
*  @return  parsed data
*/
/* ========================================================================== */
static uint32_t get_uint32(uint8_t **ptr)
{
#if defined(VPAC3L)
    volatile uint32_t res;
    uint8_t *p = *ptr;
    res = *p++;
    res |= (uint32_t)(*p++) << 8;
    res |= (uint32_t)(*p++) << 16;
    res |= (uint32_t)(*p++) << 24;
    *ptr = p;
#else
    uint32_t res;
    uint32_t* p = (uint32_t*)(*ptr);
    res = *p++;
    *ptr = (uint8_t*)p;
#endif
    return res;
}

/* ============================================================================
* get_uint8(),get_int8(),get_uint16(),get_int16(),get_uint32(),get_int32()
*
* reads from binary buffer, assembles 1B,2B and 4B data and updates the buffer
* pointer
* ========================================================================== */
static uint8_t get_uint8(uint8_t** p_bin)
{
    volatile uint8_t byte;
    uint8_t *p = *p_bin;
    byte = *p++;
    *p_bin = p;
    return byte;
}

static uint16_t get_uint16(uint8_t** p_bin)
{
    volatile uint16_t byte2;
    uint8_t *p = *p_bin;
    byte2 = *p++;
    byte2 |= (uint16_t)(*p++) << 8;
    *p_bin = p;
    return byte2;
}


/* ========================================================================== */
/**
*  void parse_header_data(uint8_t **dcc,
*                       void *out,
*                       uint32_t num_use_cases)
*  parses header data and number of use cases from DCC Profile
*
*  @param   dcc - pointer to dcc data
*
*  @param   out - pointer for the parsed data
*
*  @param   num_use_cases - number of use cases
*
*  @return  nothing.
*/
/* ========================================================================== */
void parse_header_data(uint8_t **dcc,
                       dcc_component_header_type* header_data)
{

    header_data->camera_module_id = get_uint32(dcc);
    header_data->dcc_descriptor_id = (dcc_descriptor_id_type)get_uint32(dcc);
    header_data->algorithm_vendor_id = (dcc_algorithm_vendor_id_type)get_uint32(dcc);

    header_data->dcc_tuning_tool_version = get_uint32(dcc);
    header_data->dcc_profile_time_stamp = get_uint32(dcc);
    header_data->crc_checksum = get_uint32(dcc);

    header_data->dcc_reserved_0 = get_uint32(dcc);
    header_data->dcc_reserved_1 = get_uint32(dcc);
    header_data->dcc_reserved_2 = get_uint32(dcc);
    header_data->dcc_reserved_3 = get_uint32(dcc);
    header_data->dcc_reserved_4 = get_uint32(dcc);
    header_data->dcc_reserved_5 = get_uint32(dcc);
    header_data->dcc_reserved_6 = get_uint32(dcc);
    header_data->dcc_reserved_7 = get_uint32(dcc);
    header_data->dcc_reserved_8 = get_uint32(dcc);
    header_data->dcc_reserved_9 = get_uint32(dcc);

    header_data->sz_comp_spec_gen_params = get_uint32(dcc);
    header_data->sz_uc_spec_gen_params = get_uint32(dcc);
    header_data->sz_x_dcc_descriptor = get_uint32(dcc);
    header_data->total_file_sz = get_uint32(dcc);

    Trace_printf ("------TRACE FROM HEADER PARSER START-------\n");
    Trace_printf ("camera_module_id %d \n", header_data->camera_module_id);
    Trace_printf ("dcc_descriptor_id %d \n", header_data->dcc_descriptor_id);
    Trace_printf ("algorithm_vendor_id %d \n", header_data->algorithm_vendor_id);

    Trace_printf ("sz_comp_spec_gen_params %d \n",
        header_data->sz_comp_spec_gen_params);
    Trace_printf ("sz_uc_spec_gen_params %d \n",
        header_data->sz_uc_spec_gen_params);
    Trace_printf ("sz_x_dcc_descriptor %d \n",
        header_data->sz_x_dcc_descriptor);
    Trace_printf ("total_file_sz %d \n", header_data->total_file_sz);
    Trace_printf ("------TRACE FROM HEADER PARSER DONE -------\n");
}

/* ========================================================================== */
/**
*  uint32_t find_use_case_block()
*  parses use case table of contents data in DCC Profile
*
*  @param   dcc - pointer to dcc data
*
*  @param   out - pointer for the parsed data
*
*  @return  nothing.
*/
/* ========================================================================== */
uint32_t find_use_case_block(uint8_t **p_uc_desc,
                         dcc_use_case_id_type use_case_id,
                         uint32_t* num_use_cases)
{
    uint32_t i;
    uint32_t file_offset_location;
    /*-----------------------------------------------*/
    /*find the number of use cases                   */
    /*-----------------------------------------------*/
    *num_use_cases = get_uint32(p_uc_desc);

    /*-----------------------------------------------*/
    /*no use case is present                         */
    /*-----------------------------------------------*/
    if(*num_use_cases == 0){
        Trace_printf ("WARN: no usecases found \n");
        Trace_printf ("------TRACE FROM USECASE PARSER DONE -------\n");
        return 0;
    }
    /*-----------------------------------------------*/
    /*Loop over allthe use cases to find the proper  */
    /*use case                                       */
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The current DCC test framework supports limited number of DCC-enabled testcases.
    Effect on this unit: If the control reaches here, the code is expected to continue execution as expected.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    for(i = 0; i < *num_use_cases; i ++)
    /* LDRA_JUSTIFY_END */
    {
        uint32_t uc_id;
        uint32_t sz_uc_param_data;

        /*-----------------------------------------------*/
        /*Get usecase id, offset and size of data for    */
        /*that usecase                                   */
        /*-----------------------------------------------*/
        uc_id                   = get_uint32(p_uc_desc);
        file_offset_location    = get_uint32(p_uc_desc);
        sz_uc_param_data        = get_uint32(p_uc_desc);

#ifndef __ENABLE_TRACE
        (void)sz_uc_param_data;
#endif
        Trace_printf ("Use case number %d \n", i);
        Trace_printf ("----\n");
        Trace_printf ("uc_id %X \n", uc_id);
        Trace_printf ("file_offset_location %d \n", file_offset_location);
        Trace_printf ("sz_uc_param_data %d \n", sz_uc_param_data);
        Trace_printf ("----\n");

        /*-----------------------------------------------*/
        /*If there is a match break                      */
        /*-----------------------------------------------*/
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this scenario.
        Effect on this unit: The unit is NOT expected to result in an error. This behaviour is part of the application design.
        An error print statement can be added in a future release if required.
        <justification end> */
        if(uc_id & use_case_id) 
        /* LDRA_JUSTIFY_END */
        {
            break;
        }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this scenario.
    Effect on this unit: The unit is NOT expected to result in an error. This behaviour is part of the application design.
    An error print statement can be added in a future release if required.
    <justification end> */
    }
    /* LDRA_JUSTIFY_END */

    /*-----------------------------------------------*/
    /*we could not find the use case in the profile  */
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (i == *num_use_cases){
        Trace_printf ("WARN: no matching usecase found \n");
        Trace_printf ("------TRACE FROM USECASE PARSER DONE -------\n");
        /*-----------------------------------------------*/
        /*no such use_case defined in DCC file           */
        /*-----------------------------------------------*/
        return 0;
    }
    /* LDRA_JUSTIFY_END */

    /*-----------------------------------------------*/
    /*i < *num_use_cases : move the pointer to end of*/
    /* use-case descroptor                           */
    /*-----------------------------------------------*/
    *p_uc_desc += (*num_use_cases - i - 1) * 4 * 3; //4 //bytes in uint32_t, 3 words per use case
    Trace_printf ("Matching usecase found \n");
    Trace_printf ("------TRACE FROM USECASE PARSER DONE -------\n");
    return file_offset_location;
}

/* ========================================================================== */
/**
* find_parpack()
*  finds the index of the proper photospace class based on the passed values in
*  dim_values
*
*  @param   use_case_block - pointer to the current use case data
*
*  @param   dim_values - points to struct containing the current values for all
* possible dimension
*
*  @param   parpack_bytes - size of the parameter packet (get from the dcc
*  header)
*
*  @param   out - pointer to struct with determined pointers during executing of
* find_parpack routine
*
*  @return  nothing.
*/
/* ========================================================================== */
uint8_t* find_parpack(uint8_t *use_case_block, /* ptr to the use case block */
                    uint32_t parpack_bytes,
                    dcc_ptrs_t *ret_ptrs)
{
    uint32_t i, j,dim;
    uint8_t *dcc_ptr;
    uint32_t num_photospace_dim;
    uint32_t num_regions;
    uint32_t num_classes;
    uint32_t dim_id;
    uint32_t min, max;

    /*-----------------------------------------------*/
    /*pointer to the dcc block                       */
    /*-----------------------------------------------*/
    dcc_ptr = use_case_block;

    /*-----------------------------------------------*/
    /*Get the number of photospace regions defined in*/
    /*this dcc block                                 */
    /*-----------------------------------------------*/
    num_photospace_dim = get_uint32(&dcc_ptr);

    Trace_printf ("num_photospace_dim = %d\n", num_photospace_dim);

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(num_photospace_dim == 0){
        /*-----------------------------------------------*/
        /*no photo space regions that means no dcc params*/
        /*-----------------------------------------------*/
        Trace_printf ("No phtospace dimentions found\n");
        Trace_printf ("------TRACE FROM PARAMETER PARSER END-------\n");
        return NULL;
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (num_photospace_dim > DCC_MAX_PHOTO_SPACE)
    {
        /*-----------------------------------------------*/
        /*Only AGain, color Temp and exposure are supported */
        /*-----------------------------------------------*/
        Trace_printf ("number of photospace is more that supported\n");
        Trace_printf ("Increase value of DCC_MAX_PHOTO_SPACE in idcc.h\n");
        Trace_printf ("------TRACE FROM PARAMETER PARSER END-------\n");
        return NULL;
    }
    /* LDRA_JUSTIFY_END */

    ret_ptrs->num_photospace = num_photospace_dim;

    /*-----------------------------------------------*/
    /*PHspace dimension IDs that are used are listed */
    /*in same order as denoted in PHspace region     */
    /*definitions. i.e. in each region definition    */
    /*first start/size couple is for first listed    */
    /*dimension ID. Make an array with indexes       */
    /* corresponding to dimensions position in the   */
    /*tags list and values from input dim_values_fw3A*/
    /* Each dimension ID corresponds to an index in  */
    /*dim_values_fw3A                                */
    /*-----------------------------------------------*/
    for(i = 0; i < num_photospace_dim; i++)
    {
        dim_id = get_uint32(&dcc_ptr);
        ret_ptrs->photospace_id[i] = dim_id;
    }

    /*-----------------------------------------------*/
    /*Get number of regions                          */
    /*-----------------------------------------------*/
    num_regions = get_uint32(&dcc_ptr);

    /*-----------------------------------------------*/
    /*Get number of classes                          */
    /*-----------------------------------------------*/
    num_classes = get_uint32(&dcc_ptr);

    Trace_printf ("number of regions = %d; classes = %d\n",num_regions, num_classes);

    /*-----------------------------------------------*/
    /*If either no regions or no classes then no parm*/
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if((num_regions == 0) || (num_classes == 0))
    {
        /* no regions nor classes defined, no parpacks */
        Trace_printf ("No Regions/classes found\n");
        Trace_printf ("------TRACE FROM PARAMETER PARSER END-------\n");
        return NULL;
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (DCC_MAX_PHOTO_SPACE_INST < num_regions)
    {
        /* no regions nor classes defined, no parpacks */
        Trace_printf ("Numbe of regions are more that supported by dcc lib\n");
        Trace_printf ("Increase macro DCC_MAX_PHOTO_SPACE_DIM_INST in idcc.h\n");
        Trace_printf ("------TRACE FROM PARAMETER PARSER END-------\n");
        return NULL;
    }
    /* LDRA_JUSTIFY_END */

    /* Assumption: Number of regions are same as number of classes */
    ret_ptrs->num_regions = num_regions;
    /* Initialize all photospace dimentions */
    for (i = 0U; i < DCC_MAX_PHOTO_SPACE_INST; i ++)
    {
        for (j = 0U; j < DCC_MAX_PHOTO_SPACE; j ++)
        {
            ret_ptrs->dim_range[i][j].min = 0x0;
            ret_ptrs->dim_range[i][j].max = 0xFFFFFFFF;
        }
    }

    /*-----------------------------------------------*/
    /*search a region that input data fits dependng  */
    /* on the values of input dimentions             */
    /*-----------------------------------------------*/
    for(i = 0; i < num_regions; i++)
    {
#ifdef __ENABLE_TRACE
        uint32_t dims_min[DCC_PS_DIM_ID_COUNT];
        uint32_t dims_max[DCC_PS_DIM_ID_COUNT];
#endif
        /*-----------------------------------------------*/
        /*variable used to determine the region match    */
        /*-----------------------------------------------*/
        get_uint32(&dcc_ptr);
        /*-----------------------------------------------*/
        /*Loop over all the dimentions to get a match for*/
        /* min values of the region                      */
        /*-----------------------------------------------*/
        for(dim = 0; dim < num_photospace_dim; dim++){
            dim_id = ret_ptrs->photospace_id[dim];
            /* read min */
            min = get_uint32(&dcc_ptr);
#ifdef __ENABLE_TRACE
            dims_min[dim] = min;
            Trace_printf ("min value for dim %d = %d\n", dim,dims_min[dim]);
#endif
            ret_ptrs->dim_range[i][dim_id].min = min;
        }
        /*-----------------------------------------------*/
        /*Loop over all the dimentions to get a match for*/
        /* max values of the region                      */
        /*-----------------------------------------------*/
        for(dim = 0; dim < num_photospace_dim; dim++){
            dim_id = ret_ptrs->photospace_id[dim];
            max = get_uint32(&dcc_ptr); /* if the region is defined by begin and end (inclusive) */
            ret_ptrs->dim_range[i][dim_id].max = max;
#ifdef __ENABLE_TRACE
            dims_max[dim] = max;
            Trace_printf ("max value for dim %d = %d\n", dim,dims_max[dim]);
#endif
        }
    }

    /*-----------------------------------------------*/
    /*Get the pointer to the dcc class for these vals*/
    /*-----------------------------------------------*/
    /*i < num_regions : move the pointer to end of   */
    /*PHspase regions descriptor                     */
    /*-----------------------------------------------*/
    dcc_ptr += (num_regions - i) * (num_photospace_dim * 2 + 1) * 4;

    /*-----------------------------------------------*/
    /*return the actual dcc pointer                  */
    /*-----------------------------------------------*/
    return dcc_ptr;
}

static void get_array_int16(int16_t *dst, int32_t arr_size, uint8_t **psrc)
{
    int i;
    get_uint32(psrc); //uarr_size =
    for (i = 0; i < arr_size; i++)
    {
        dst[i] = get_int16(psrc);
    }
}

static void get_array_uint16(uint16_t *dst, int32_t arr_size, uint8_t **psrc)
{
    int i;
    get_uint32(psrc); //uarr_size =
    for (i = 0; i < arr_size; i++)
    {
        dst[i] = get_uint16(psrc);
    }
}

static void get_array_uint8(uint8_t *dst, int32_t arr_size, uint8_t **psrc)
{
    int i;
    get_uint32(psrc); //uarr_size =
    for (i = 0; i < arr_size; i++)
    {
        dst[i] = get_uint8(psrc);
    }
}

static int check_input(dcc_ptrs_t *dcc_ptrs, uint32_t crc, char * func_name)
{
	Trace_printf("%s : dcc_ptrs = 0x%p \n", func_name, dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
	if (NULL == dcc_ptrs)
	{
		return (-1);
	}
    /* LDRA_JUSTIFY_END */

	/* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
	if(dcc_ptrs->checksum != crc)
	{
		Trace_printf("%s : checksum error. Received 0x%x Expected 0x%x\n", func_name, dcc_ptrs->checksum, crc);
		return (-1);
	}
    /* LDRA_JUSTIFY_END */
	Trace_printf("%s : dcc_ptrs->p_parpack = 0x%p \n", func_name, dcc_ptrs->p_parpack);
	return 0;
}

int awb_alg_dcc_tuning_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void* sys_prm, void* uc_prm, void* parpack)
{
    Trace_printf("awb_alg_dcc_tuning_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(dcc_ptrs->checksum != 0x184E603C)
    {
        Trace_printf("awb_alg_dcc_tuning_dcc_bin_parse : checksum error. Received 0x%x Expected 0x184E603C \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != dcc_ptrs->p_gen_data)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        awb_calc_data_t *calbData = &outPrms->awbCalbData;
        uint8_t *p_bin = dcc_ptrs->p_gen_data;

        get_uint16(&p_bin);                    //sys_p->use_calib =
        outPrms->useAwbCalbCfg = 1U;
        calbData->blue_index = get_uint16(&p_bin);
        calbData->red_index = get_uint16(&p_bin);
        calbData->green1_index = get_uint16(&p_bin);
        calbData->green2_index = get_uint16(&p_bin);

        get_uint8(&p_bin);                   //sys_p->enable_opt =
        get_int16(&p_bin);                   //sys_p->max_Cr =
        get_int16(&p_bin);                   //sys_p->max_Cb =
        get_uint16(&p_bin);                  //sys_p->awb_speed =
        calbData->num_of_ref_1 = get_uint8(&p_bin);
        calbData->num_of_ref_2 = get_uint8(&p_bin);
        calbData->num_of_gray = get_uint8(&p_bin);
        calbData->radius = get_uint16(&p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.color_temp_1 array */
        get_array_uint16( calbData->color_temp_1,
                          calbData->num_of_ref_1,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.wbReferenceCb array */
        get_array_int16( (int16_t*) calbData->wbReferenceCb,
                         calbData->num_of_ref_1 * 4,
                         &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.wbReferenceCr array */
        get_array_int16( (int16_t*) calbData->wbReferenceCr,
                         calbData->num_of_ref_1 * 4,
                         &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_R_1 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_R_1,
                          calbData->num_of_ref_1 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_G_1 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_G_1,
                          calbData->num_of_ref_1 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_B_1 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_B_1,
                          calbData->num_of_ref_1 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_index_2 array */
        get_array_uint8( calbData->ref_index_2,
                         calbData->num_of_ref_2,
                         &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.color_temp_2 array */
        get_array_uint16( calbData->color_temp_2,
                          calbData->num_of_ref_2,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.referencesCb_2 array */
        get_array_int16( (int16_t*) calbData->referencesCb_2,
                         calbData->num_of_ref_2 * 4,
                         &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.referencesCr_2 array */
        get_array_int16( (int16_t*) calbData->referencesCr_2,
                         calbData->num_of_ref_2 * 4,
                         &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_R_2 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_R_2,
                          calbData->num_of_ref_2 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_G_2 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_G_2,
                          calbData->num_of_ref_2 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.awb_basic_ref.ref_gray_B_2 array */
        get_array_uint16( (uint16_t*) calbData->ref_gray_B_2,
                          calbData->num_of_ref_2 * 4,
                          &p_bin);

        /* extract sys_p->awb_calc_data.img_ref array */
        get_array_uint16( (uint16_t*) calbData->img_ref,
                          calbData->num_of_ref_2 * 1120,
                          &p_bin);

        calbData->luma_awb_min = get_int32(&p_bin);
        calbData->luma_awb_max = get_int32(&p_bin);
        calbData->low_color_temp_thresh = get_uint16(&p_bin);
        calbData->apply_rgb_adjust = get_uint8(&p_bin);
        calbData->R_adjust = get_int16(&p_bin);
        calbData->B_adjust = get_int16(&p_bin);
        calbData->SB_1 = get_uint16(&p_bin);
        calbData->SB_2 = get_uint16(&p_bin);
        calbData->SB_low_bound = get_uint16(&p_bin);
        calbData->default_T_H = get_uint16(&p_bin);
        calbData->default_T_MH = get_uint16(&p_bin);
        calbData->default_T_ML = get_uint16(&p_bin);
        calbData->default_T_L = get_uint16(&p_bin);
        calbData->default_T_H_index = get_uint8(&p_bin);
        calbData->default_T_MH_index = get_uint8(&p_bin);
        calbData->default_T_ML_index = get_uint8(&p_bin);
        calbData->default_T_L_index = get_uint8(&p_bin);
        calbData->best_gray_index_default = get_uint8(&p_bin);

        /* extract sys_p->awb_calc_data.flash_ref array */
        /* extract sys_p->awb_calc_data.skin_img_ref array */
        /* extract sys_p->awb_data.wbModeData array */
        outPrms->useAwbCalbCfg = 1;

    }
    return 0;
}

int ipipe_rgb2rgb_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void *sys_prm, void *uc_prm, void *parpack)
{
    uint8_t *p_bin = NULL;
    uint32_t regCnt;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

   /* Check for CRC Errors */
   /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(dcc_ptrs->checksum != 0x18F10844)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* parse parameter packets */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL != parpack) && (NULL != dcc_ptrs->p_parpack))
    /* LDRA_JUSTIFY_END */
    {
        uint32_t cnt1, cnt2;
        dcc_parser_output_params_t *outPrms =
            (dcc_parser_output_params_t *)parpack;
        iss_ipipe_rgb2rgb *rgb2RgbCfg = NULL;

        p_bin = dcc_ptrs->p_parpack;

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (DCC_ID_IPIPE_RGB_RGB_1 == dcc_ptrs->desc_id)
        /* LDRA_JUSTIFY_END */
        {
            rgb2RgbCfg = &outPrms->ipipeRgb2Rgb1Cfg[0];
        }
        /* Parse the bin file first */
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (NULL == rgb2RgbCfg)
        {
            Trace_printf("ipipe_rgb2rgb_dcc_bin_parse : rgb2RgbCfg is NULL \n");
            return -1;
        }
        /* LDRA_JUSTIFY_END */

        for (regCnt = 0U; regCnt < dcc_ptrs->num_regions; regCnt ++)
        {
            /* Extract Multiplication Coefficients */
            for (cnt1 = 0; cnt1 < 3; cnt1 ++)
            {
                for (cnt2 = 0; cnt2 < 4; cnt2 ++)
                {
                    rgb2RgbCfg->matrix[cnt1][cnt2] = get_int16(&p_bin);
                }
            }

            /* Extract Offset */
            for (cnt1 = 0; cnt1 < 3; cnt1 ++)
            {
                rgb2RgbCfg->offset[cnt1] = get_int16(&p_bin);
            }

            rgb2RgbCfg++;
        }

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (DCC_ID_IPIPE_RGB_RGB_1 == dcc_ptrs->desc_id)
        /* LDRA_JUSTIFY_END */
        {
            /* Copy all photospace parameters */
            memcpy(outPrms->phPrmsRgb2Rgb1,
                   dcc_ptrs->dim_range,
                   sizeof(dcc_parser_dim_range) *
                    DCC_RGB2RGB1_MAX_PHOTO_SPACE_INST *
                    DCC_MAX_PHOTO_SPACE);

            outPrms->useCcmCfg = 1U;
            outPrms->ipipeNumRgb2Rgb1Inst = dcc_ptrs->num_regions;
        }
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_ipipe_rgb2rgb (void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int viss_nsf4_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_nsf4_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(dcc_ptrs->checksum != 0x41DA95E1)
    {
        Trace_printf("viss_nsf4_dcc_bin_parse : checksum error. Received 0x%x Expected 0x41DA95E1 \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;

        /* Copy all photospace parameters */
        memcpy(outPrms->phPrmsNSF4, dcc_ptrs->dim_range,
               sizeof(dcc_parser_dim_range) * DCC_MAX_PHOTO_SPACE * dcc_ptrs->num_regions);

        memcpy(outPrms->vissNSF4Cfg, dcc_ptrs->p_parpack, sizeof(viss_nsf4) * dcc_ptrs->num_regions);
        outPrms->vissNumNSF4Inst = dcc_ptrs->num_regions;
        outPrms->useNsf4Cfg = 1U;
    }
    return 0;
}

int h3a_mux_luts_bin_parse(
    dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("h3a_mux_luts_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0xC5BE9ED1)
    {
        Trace_printf("h3a_mux_luts_bin_parse : checksum error. Received 0x%x Expected 0xC5BE9ED1 \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("h3a_mux_luts_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        iss_h3a_mux_luts *cfg = (iss_h3a_mux_luts *)(&outPrms->issH3aMuxLuts);
        memcpy(cfg, dcc_ptrs->p_parpack, sizeof(iss_h3a_mux_luts));
        outPrms->useH3aMuxCfg = 1;
    }

    return 0;
}

int iss_rfe_decompand_bin_parse(
    dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("iss_rfe_decompand_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0xDC5E70D)
    {
        Trace_printf("iss_rfe_decompand_bin_parse : checksum error. Received 0x%x Expected 0xDC5E70D \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("iss_ref_decompand_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        iss_rfe_decompand *cfg = (iss_rfe_decompand *)(&outPrms->issRfeDecompand);
        memcpy(cfg, dcc_ptrs->p_parpack, sizeof(iss_rfe_decompand));
        outPrms->useRfeDcmpCfg = 1;
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_iss_rfe_decompand(void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int h3a_aewb_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("h3a_aewb_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0xD0295AD2)
    {
        Trace_printf("h3a_aewb_dcc_bin_parse : checksum error. Received 0x%x Expected 0xD0295AD2 \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("h3a_aewb_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        iss_ipipe_h3a_aewb *h3a_aewb_cfg = (iss_ipipe_h3a_aewb *)(&outPrms->ipipeH3A_AEWBCfg);

        memcpy(h3a_aewb_cfg, dcc_ptrs->p_parpack, sizeof(iss_ipipe_h3a_aewb));
        outPrms->useH3aCfg = 1;
    }

    return 0;
}

int blc_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("blc_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x3C7EA53F)
    {
        Trace_printf("blc_dcc_bin_parse : checksum error. Received 0x%x Expected 0xD0295AD2 \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("blc_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        iss_black_level_subtraction *viss_blc_cfg = (iss_black_level_subtraction *)(&outPrms->vissBLC);

        memcpy(viss_blc_cfg, dcc_ptrs->p_parpack, sizeof(iss_black_level_subtraction));
        outPrms->useBlcCfg = 1;
    }

    return 0;
}

int cfa_dcc_bin_parse(
    dcc_ptrs_t *dcc_ptrs,
    void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("cfa_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x31358E23)
    {
        printf("cfa_dcc_bin_parse : checksum error. Received 0x%x Expected 0x31358E23 \n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("cfa_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        viss_ipipe_cfa_flxd *cfa_flxd_cfg = (viss_ipipe_cfa_flxd *)(&outPrms->vissCFACfg);
        memcpy(cfa_flxd_cfg, dcc_ptrs->p_parpack, sizeof(viss_ipipe_cfa_flxd));
        outPrms->useCfaCfg = 1;
    }
    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_h3a_mux_luts(void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_h3a_aewb(void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_nsf4(void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_blc(void * dcc_data,
                               void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_cfa(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int vpac_ldc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("vpac_ldc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
#if defined(VPAC3) || defined(VPAC3L)
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if ((dcc_ptrs->checksum != 0x8B650052) && (dcc_ptrs->checksum != 0xC5D5EAA1))
    {
        Trace_printf("vpac_ldc_bin_parse : checksum error. Received 0x%x Expected 0x8B650052\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
#else
    if(dcc_ptrs->checksum != 0xC5D5EAA1)
    {
        Trace_printf("vpac_ldc_bin_parse : checksum error. Received 0x%x Expected 0xC5D5EAA1\n", dcc_ptrs->checksum);
        return (-1);
    }
#endif
    Trace_printf("vpac_ldc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;

        vpac_ldc_dcc_cfg_t    *pcfg   = &outPrms->vpacLdcCfg;
        vpac_ldc_dcc_params_t *params = &pcfg->ldc_dcc_params;

        uint8_t *p_bin = dcc_ptrs->p_parpack;

        memcpy(params, p_bin, sizeof(vpac_ldc_dcc_params_t));
        p_bin += sizeof(vpac_ldc_dcc_params_t);

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (params->ldmapen)
        /* LDRA_JUSTIFY_END */
        {
            pcfg->mesh_table = (uint16_t*) p_bin;
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        else
        {
            pcfg->mesh_table = NULL;
        }
        /* LDRA_JUSTIFY_END */
#if defined(VPAC3) || defined(VPAC3L)
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (dcc_ptrs->checksum != 0x8B650052)
        /* LDRA_JUSTIFY_END */
        {
            pcfg->chroma_ctl_en = 0;
            pcfg->chroma_ctl_format = 0;
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        else
        {
            p_bin += params->mesh_table_size * 2;
            pcfg->chroma_ctl_en = * ((uint16_t *)p_bin);
            p_bin += 2;
            pcfg->chroma_ctl_format = * ((uint16_t *)p_bin);
        }
        /* LDRA_JUSTIFY_END */
#endif
        outPrms->useVpacLdcCfg = 1;
    }
    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_vpac_ldc(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int viss_glbce_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_glbce_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x53EFD937)
    {
        Trace_printf("viss_glbce_bin_parse : checksum error. Received 0x%x Expected 0x53EFD937\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_glbce_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        viss_glbce_dcc_cfg_t    *pcfg   = &outPrms->vissGlbceCfg;
        memcpy(pcfg, dcc_ptrs->p_parpack, sizeof(viss_glbce_dcc_cfg_t));
        outPrms->useVissGlbceCfg = 1;
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_glbce(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int viss_lsc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_lsc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x611E6712)
    {
        Trace_printf("viss_lsc_bin_parse : checksum error. Received 0x%x Expected 0x611E6712\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_glbce_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        viss_lsc_dcc_cfg_t         *pcfg    = &outPrms->vissLscCfg;
        viss_lsc_dcc_params_t      *params  = &pcfg->lsc_params;
        uint8_t                    *p_bin   = dcc_ptrs->p_parpack;

        memcpy(params, p_bin, sizeof(viss_lsc_dcc_params_t));
        p_bin += sizeof(viss_lsc_dcc_params_t);

        pcfg->lsc_table = NULL;
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this scenario.
        Effect on this unit: The unit is NOT expected to result in an error. This behaviour is part of the application design.
        An error print statement can be added in a future release if required.
        <justification end> */
        if (params->enable)
        /* LDRA_JUSTIFY_END */
        {
            pcfg->lsc_table = p_bin;
        }

        outPrms->useVissLscCfg = 1;
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_lsc(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */


static int dcc_search_photo_space_regions(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_ranges,
    int dim_to_search,
    int value_to_search,
    int *pk0,
    int *pk1)
{
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications CAN reach this portion.
    No existing test case can reach this portion.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
    A test case will be added to cover this gap in a future release.
    <justification end> */
    if (value_to_search < dim_range[0][dim_to_search].min)
    {
        *pk0 = 0;
        *pk1 = 0;
        return 256;
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications CAN reach this portion.
    No existing test case can reach this portion.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
    A test case will be added to cover this gap in a future release.
    <justification end> */
    if (value_to_search > dim_range[n_ranges-1][dim_to_search].max)
    {
        *pk0 = n_ranges - 1;
        *pk1 = n_ranges - 1;
        return 256;
    }
    /* LDRA_JUSTIFY_END */

    int w0 = 256, k0 = 0, k1 = 0, k;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications CAN reach this portion.
    No existing test case can reach this portion.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
    A test case will be added to cover this gap in a future release.
    <justification end> */
    for (k = 0; k < n_ranges; k++)
    /* LDRA_JUSTIFY_END */
    {
        int low = dim_range[k][dim_to_search].min;
        int hig = dim_range[k][dim_to_search].max;

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (low <= value_to_search && value_to_search <= hig)
        /* LDRA_JUSTIFY_END */
        {
            k0 = k1 = k;
            if (value_to_search * 2 < low + hig)
            {
                /* LDRA_JUSTIFY_START
                <metric start> statement branch <metric end>
                <justification start> 
                Rationale: The component level negative test framework and test applications CAN reach this portion.
                No existing test case can reach this portion.
                Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
                A test case will be added to cover this gap in a future release.
                <justification end> */
                if (k > 0)
                {
                    k0 = k - 1;
                    w0 = 128 - (256 * (value_to_search - low) + (hig - low) / 2) / (hig - low);
                }
                /* LDRA_JUSTIFY_END */
            }
            else
            {
                /* LDRA_JUSTIFY_START
                <metric start> branch <metric end>
                <justification start> 
                Rationale: The component level negative test framework and test applications CAN reach this portion.
                No existing test case can reach this portion.
                Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
                A test case will be added to cover this gap in a future release.
                <justification end> */
                if (k + 1 < n_ranges)
                /* LDRA_JUSTIFY_END */
                {
                    k1 = k + 1;
                    w0 = 128 + (256 * (hig - value_to_search) + (hig - low) / 2) / (hig - low);
                }
            }
            break;
        }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this scenario.
    Effect on this unit: The unit is NOT expected to result in an error. This behaviour is part of the application design.
    An error print statement can be added in a future release if required.
    <justification end> */
    }
    /* LDRA_JUSTIFY_END */

    *pk0 = k0;
    *pk1 = k1;
    return w0;
}

static int dcc_simple_search_photo_space_regions(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_ranges,
    int dim_to_search,
    int value_to_search)
{
    int k;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications CAN reach this portion.
    No existing test case can reach this portion.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
    A test case will be added to cover this gap in a future release.
    <justification end> */
    for (k = 0; k < n_ranges; k++)
    /* LDRA_JUSTIFY_END */
    {
        int low = dim_range[k][dim_to_search].min;
        int hig = dim_range[k][dim_to_search].max;

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        if (low <= value_to_search && value_to_search <= hig)
        /* LDRA_JUSTIFY_END */
        {
            return k;
        }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications CAN reach this portion.
    No existing test case can reach this portion.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
    A test case will be added to cover this gap in a future release.
    <justification end> */
    }

    if (value_to_search < dim_range[0][dim_to_search].min)
    {
        return 0;
    }

    if (value_to_search > dim_range[n_ranges-1][dim_to_search].max)
    {
        return n_ranges - 1;
    }

    return 0;
    /* LDRA_JUSTIFY_END */
}

static void dcc_interpolate_CCM_3x3Q8(
    iss_ipipe_rgb2rgb ccm_in[],
    int k0,
    int k1,
    int w0,
    iss_ipipe_rgb2rgb *ccm_out)
{
    if (k0 == k1)
    {
        *ccm_out = ccm_in[k0];
    }
    else
    {
        int w1 = 256 - w0;
        int cnt1, cnt2;
        for (cnt1 = 0u; cnt1 < 3; cnt1++)
        {
            int sum = 0;
            for (cnt2 = 0u; cnt2 < 3; cnt2++)
            {
                int m0 = ccm_in[k0].matrix[cnt1][cnt2];
                int m1 = ccm_in[k1].matrix[cnt1][cnt2];
                int m = (w0 * m0 + w1 * m1 + 128) >> 8;
                ccm_out->matrix[cnt1][cnt2] = m;
                sum += m;
            }
            ccm_out->matrix[cnt1][3] = 0;
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications CAN reach this portion.
            No existing test case can reach this portion.
            Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
            A test case will be added to cover this gap in a future release.
            <justification end> */
            if(sum != 256)
            {
                ccm_out->matrix[cnt1][cnt1] += (256-sum);
            }
            /* LDRA_JUSTIFY_END */
            ccm_out->offset[cnt1] = 0;
        }
    }
}

int dcc_interp_CCM(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int color_temp,
    iss_ipipe_rgb2rgb ccm_in[],
    iss_ipipe_rgb2rgb *p_ccm_int)
{
    int k0 = 0, k1 = 0;
    /*dim_to_search is hardcoded to 2 to search only on color_temp axis*/
    int w0 = dcc_search_photo_space_regions(dim_range, n_regions, DCC_PHOTOSPACE_CT, color_temp, &k0, &k1);
    dcc_interpolate_CCM_3x3Q8(ccm_in, k0, k1, w0, p_ccm_int);
    return w0;
}

int dcc_search_NSF4(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int analog_gain_in_ev,
    viss_nsf4 nsf4_matrix_in[],
    viss_nsf4 *p_nsf4_out)
{
    int value_to_search;
    /*
        dim_to_search is hardcoded to 0 to search only on analog_gain axis
        analog gain is encoded as 10 * EV
    */
    value_to_search = analog_gain_in_ev;
    int phIndex = dcc_simple_search_photo_space_regions(dim_range, n_regions, DCC_PHOTOSPACE_AG, value_to_search);
    memcpy(p_nsf4_out, &nsf4_matrix_in[phIndex], sizeof(viss_nsf4));
    return 0;
}

int viss_yee_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_yee_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x9A70A925)
    {
        Trace_printf("viss_yee_bin_parse : checksum error. Received 0x%x Expected 0x9A70A925\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_yee_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;

        /* Copy all photospace parameters */
        memcpy(outPrms->phPrmsYee, dcc_ptrs->dim_range,
                sizeof(dcc_parser_dim_range) * DCC_MAX_PHOTO_SPACE * dcc_ptrs->num_regions);

        viss_yee_dcc_cfg_t    *pcfg   = &outPrms->vissYeeCfg[0];
        memcpy(pcfg, dcc_ptrs->p_parpack, sizeof(viss_yee_dcc_cfg_t) * dcc_ptrs->num_regions);
        outPrms->vissNumYeeInst = dcc_ptrs->num_regions;
        outPrms->useVissYeeCfg = 1U;
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_yee(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

static int dcc_search_again_in_dot1EV(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int analog_gain)
{

    // dim_to_search is hardcoded to 0 to search only on analog_gain axis
    // value_to_search for alalog gain must be encoded as 10*log2(analog_gain_linear) as in DCC format
    // analog_gain_linear from AE is in Q10 format (i.e., -10 after log2())
    // analog_gain_linear must be be coverted to alalog gain in DCC format first
    //int analog_gain = (int)(10 * (log(analog_gain_linear) / log(2) - 10) + 0.5);

    int value_to_search = analog_gain;
    int index = dcc_simple_search_photo_space_regions(dim_range, n_regions, DCC_PHOTOSPACE_AG, value_to_search);
    return index;
}

int dcc_search_YEE(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int analog_gain_in_ev)
{
    int index = dcc_search_again_in_dot1EV(dim_range, n_regions, analog_gain_in_ev);
    return index;
}

int viss_dpc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_dpc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0xD1D13CF5)
    {
        Trace_printf("viss_dpc_bin_parse : checksum error. Received 0x%x Expected 0xD1D13CF5\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_dpc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;

        /* Copy all photospace parameters */
        memcpy(outPrms->phPrmsDpc, dcc_ptrs->dim_range,
                sizeof(dcc_parser_dim_range) * DCC_MAX_PHOTO_SPACE * dcc_ptrs->num_regions);

        viss_dpc_dcc_cfg_t    *pcfg   = &outPrms->vissDpcCfg[0];
        memcpy(pcfg, dcc_ptrs->p_parpack, sizeof(viss_dpc_dcc_cfg_t) * dcc_ptrs->num_regions);
        outPrms->vissNumDpcInst = dcc_ptrs->num_regions;
        outPrms->useVissDpcCfg = 1U;
    }

    return 0;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_dpc(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int dcc_search_DPC(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int analog_gain_in_ev)
{
    int index = dcc_search_again_in_dot1EV(dim_range, n_regions, analog_gain_in_ev);
    return index;
}

int viss_rawfe_wb1vs_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_rawfe_wb1vs_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0xF766DB6D)
    {
        Trace_printf("viss_rawfe_wb1vs_dcc_bin_parse : checksum error. Received 0x%x Expected 0xF766DB6D\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_rawfe_wb1vs_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_rawfe_wb1_dcc_cfg_t        * pCfg    = &outPrms->vissRawfeWb1VsCfg;
        viss_rawfe_wb1_dcc_cfg_t        * pSrc    = (viss_rawfe_wb1_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_rawfe_wb1_dcc_cfg_t));
        outPrms->useVissRawfeWb1VsCfg = 1U;
    }

    return 0;
}

int viss_gamma_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
        int status = 0;
        status = check_input(dcc_ptrs, 0xAC9866FF, "viss_gamma_dcc_bin_parse");

        /* Null Pointer check */
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.        <justification end> */
        if ((0 == status) && (NULL != dcc_ptrs->p_parpack))
        /* LDRA_JUSTIFY_END */
        {
                dcc_parser_output_params_t  * outPrms = (dcc_parser_output_params_t *)parpack;

                /* Copy all photospace parameters */
                memcpy(outPrms->phPrmsGamma, dcc_ptrs->dim_range,
                        sizeof(dcc_parser_dim_range) * DCC_MAX_PHOTO_SPACE * dcc_ptrs->num_regions);
                viss_gamma_dcc_cfg_t        * pCfg    = &outPrms->vissGammaCfg[0];
                memcpy(pCfg, dcc_ptrs->p_parpack, sizeof(viss_gamma_dcc_cfg_t) * dcc_ptrs->num_regions);

                outPrms->vissNumGammaInst = dcc_ptrs->num_regions;
                outPrms->useVissGammaCfg = 1U;
        }

        return status;
}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
void dcc_update_viss_gamma(void * dcc_data, void * driver_data)
{
}
/* LDRA_JUSTIFY_END */

int dcc_search_GAMMA(
    dcc_parser_dim_range dim_range[][DCC_MAX_PHOTO_SPACE],
    int n_regions,
    int analog_gain_in_ev)
{
    int index = dcc_search_again_in_dot1EV(dim_range, n_regions, analog_gain_in_ev);
    return index;
    
}

#if defined(VPAC3) || defined(VPAC3L)
int viss_cac_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_cac_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(dcc_ptrs->checksum != 0x5BCB6EB0)
    {
        Trace_printf("viss_cac_dcc_bin_parse : checksum error. Received 0x%x Expected 0x5BCB6EB0\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_cac_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        int k;
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_cac_dcc_cfg_t         * pCfg    = &outPrms->vissCacCfg;
        viss_cac_dcc_cfg_t         * pSrc    = (viss_cac_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        pCfg->enable = pSrc->enable;
        pCfg->color_en = pSrc->color_en;
        pCfg->block_s = pSrc->block_s;
        pCfg->grid_w = pSrc->grid_w;
        pCfg->grid_h = pSrc->grid_h;
        pCfg->lut_size_in_bytes = pSrc->lut_size_in_bytes;

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this scenario.
        Effect on this unit: The unit is NOT expected to result in an error. This behaviour is part of the application design.
        An error print statement can be added in a future release if required.
        <justification end> */
        if (pCfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            for (k = 0; k < pCfg->lut_size_in_bytes; k++)
            {
                pCfg->cac_lut[k] = pSrc->cac_lut[k];
            }
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        A test case will be added to cover this gap in a future release.
        <justification end> */
        else
        {
            for (k = 0; k < pCfg->lut_size_in_bytes; k++)
            {
                pCfg->cac_lut[k] = 0;
            }
        }
        /* LDRA_JUSTIFY_END */

        outPrms->useVissCacCfg = 1U;
    }

    return 0;
}

/* VPAC3 VISS3 RawHist */
/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
<justification end> */
int viss_rawhist_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_rawhist_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }

    /* Check for CRC Error */
    if(dcc_ptrs->checksum != 0xFAB9A65D)
    {
        Trace_printf("viss_rawhist_dcc_bin_parse : checksum error. Received 0x%x Expected 0xFAB9A65D\n", dcc_ptrs->checksum);
        return (-1);
    }
    Trace_printf("viss_rawhist_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    if (NULL != dcc_ptrs->p_parpack)
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_rawhist_dcc_cfg_t     * pCfg    = &outPrms->vissRawhistCfg;
        viss_rawhist_dcc_cfg_t     * pSrc    = (viss_rawhist_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_rawhist_dcc_cfg_t));
        outPrms->useVissRawhistCfg = 1U;
    }

    return 0;
}
/* LDRA_JUSTIFY_END */

int viss_cfai3a_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_cfai3a_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.:
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x8D622260)
    {
        Trace_printf("viss_cfai3a_dcc_bin_parse : checksum error. Received 0x%x Expected 0x8D622260\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_cfai3a_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_cfai3_dcc_cfg_t       * pCfg    = &outPrms->vissCfai3aCfg;
        viss_cfai3_dcc_cfg_t       * pSrc    = (viss_cfai3_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_cfai3_dcc_cfg_t));
        outPrms->useVissCfai3aCfg = 1U;
    }

    return 0;
}
#endif

#if defined(VPAC3)
int viss_cfai3b_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_cfai3b_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x8D622260)
    {
        Trace_printf("viss_cfai3b_dcc_bin_parse : checksum error. Received 0x%x Expected 0x8D622260\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_cfai3b_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_cfai3_dcc_cfg_t       * pCfg    = &outPrms->vissCfai3bCfg;
        viss_cfai3_dcc_cfg_t       * pSrc    = (viss_cfai3_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_cfai3_dcc_cfg_t));
        outPrms->useVissCfai3bCfg = 1U;
    }

    return 0;
}

int viss_ccmv_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    Trace_printf("viss_ccmv_dcc_bin_parse : dcc_ptrs = 0x%p \n", dcc_ptrs);
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario. dcc_ptrs cannot be NULL as it is pre-validated.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (NULL == dcc_ptrs)
    {
        return (-1);
    }
    /* LDRA_JUSTIFY_END */

    /* Check for CRC Error */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if(dcc_ptrs->checksum != 0x90B5F8D6)
    {
        Trace_printf("viss_ccmv_dcc_bin_parse : checksum error. Received 0x%x Expected 0x90B5F8D6\n", dcc_ptrs->checksum);
        return (-1);
    }
    /* LDRA_JUSTIFY_END */
    Trace_printf("viss_ccmv_dcc_bin_parse : dcc_ptrs->p_parpack = 0x%p \n", dcc_ptrs->p_parpack);

    /* Null Pointer check */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.    <justification end> */
    if (NULL != dcc_ptrs->p_parpack)
    /* LDRA_JUSTIFY_END */
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_ccmv_dcc_cfg_t        * pCfg    = &outPrms->vissCcMvCfg;
        viss_ccmv_dcc_cfg_t        * pSrc    = (viss_ccmv_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_ccmv_dcc_cfg_t));
        outPrms->useVissCcMvCfg = 1U;
    }

    return 0;
}
#endif

#if defined(VPAC3L)

static void memcpy_uint8(uint8_t *dst, uint8_t *src, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        dst[i] = src[i];
    }
}

int viss_lsc_ext_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    int status = 0;
    status = check_input(dcc_ptrs, 0xBBD9D71D, "viss_lsc_ext_dcc_bin_parse");

    /* Null Pointer check */
    if ((0 == status) && (NULL != dcc_ptrs->p_parpack))
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_lsc_ext_dcc_cfg_t     * pCfg    = &outPrms->vissLscExtCfg;
        viss_lsc_ext_dcc_cfg_t     * pSrc    = (viss_lsc_ext_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        //memcpy(pCfg, pSrc, sizeof(viss_lsc_ext_dcc_cfg_t));
        memcpy_uint8((uint8_t*)pCfg, (uint8_t*)pSrc, sizeof(viss_lsc_ext_dcc_cfg_t));
        outPrms->useVissLscExtCfg = 1U;
    }

    return status;
}

int viss_dpc_ext_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    int status = 0;
    status = check_input(dcc_ptrs, 0x2BDBAEC8, "viss_dpc_ext_dcc_bin_parse");

    /* Null Pointer check */
    if ((0 == status) && (NULL != dcc_ptrs->p_parpack))
    {
        /* all photospace parameters are available in original DPC part */
        dcc_parser_output_params_t *outPrms = (dcc_parser_output_params_t *)parpack;
        viss_dpc_ext_dcc_cfg_t     * pCfg    = &outPrms->vissDpcExtCfg[0];
        viss_dpc_ext_dcc_cfg_t     * pSrc    = (viss_dpc_ext_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        //memcpy(pCfg, pSrc, sizeof(viss_dpc_ext_dcc_cfg_t) * dcc_ptrs->num_regions);
        memcpy_uint8((uint8_t*)pCfg, (uint8_t*)pSrc, sizeof(viss_dpc_ext_dcc_cfg_t) * dcc_ptrs->num_regions);
        outPrms->useVissDpcExtCfg = 1U;
    }

    return status;
}

int viss_pcid_dcc_bin_parse(dcc_ptrs_t *dcc_ptrs, void *sys_prm, void *uc_prm, void *parpack)
{
    int status = 0;
    status = check_input(dcc_ptrs, 0xA9678802, "viss_pcid_dcc_bin_parse");

    /* Null Pointer check */
    if ((0 == status) && (NULL != dcc_ptrs->p_parpack))
    {
        dcc_parser_output_params_t * outPrms = (dcc_parser_output_params_t *)parpack;
        viss_pcid_dcc_cfg_t        * pCfg    = &outPrms->vissPcidCfg;
        viss_pcid_dcc_cfg_t        * pSrc    = (viss_pcid_dcc_cfg_t * ) dcc_ptrs->p_parpack;
        memcpy(pCfg, pSrc, sizeof(viss_pcid_dcc_cfg_t));
        outPrms->useVissPcidCfg = 1U;
    }

    return status;
}
#endif
