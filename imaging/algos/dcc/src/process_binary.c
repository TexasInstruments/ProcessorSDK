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

#include <stdint.h>
#include <stddef.h>
#include <idcc.h>
#include "dcc_defs.h"
#include "parse_data.h"
#include "dcc_comp.h"
#include <TI_aaa_awb.h>

/* ============================================================================
 *                                  INCLUDE FILES
 * ===========================================================================*/

/* ============================================================================
 *                                  GLOBALS
 * ===========================================================================*/


/* =============================================================================
 *                                  Function Declaration
 * =========================================================================== */
int update_driver_ptr(dcc_parser_output_params_t *outPrms);
DCC_GET_STATUS process_binary_dcc(uint8_t *dcc_buf,
                                  uint32_t dcc_bytes,
                                  uint32_t camera_module_id,
                                  dcc_descriptor_id_type dcc_descriptor_id,
                                  dcc_algorithm_vendor_id_type algorithm_vendor_id,
                                  dcc_use_case_id_type use_case_id,
                                  dcc_phspace_vals_t *dim_values,
                                  dcc_ptrs_t         *ret_ptrs,
                                  uint32_t* ret_info);

static dcc_ptrs_t gDccRetPtrs;

/* =============================================================================
 *                                  Function Defination
 * =========================================================================== */
int update_driver_ptr (dcc_parser_output_params_t *outPrms)
{
    tDCCCompList[DCC_ID_AAA_ALG_AWB_TI3].driver_ptr = (void*)&outPrms->awbCalbData;
    tDCCCompList[DCC_ID_IPIPE_RGB_RGB_1].driver_ptr = (void*)&outPrms->ipipeRgb2Rgb1Cfg;
    tDCCCompList[DCC_ID_H3A_MUX_LUTS].driver_ptr = (void*)&outPrms->issH3aMuxLuts;
    tDCCCompList[DCC_ID_H3A_AEWB_CFG].driver_ptr = (void*)&outPrms->ipipeH3A_AEWBCfg;
    tDCCCompList[DCC_ID_NSF4].driver_ptr = (void*)&outPrms->vissNSF4Cfg;
    tDCCCompList[DCC_ID_IPIPE_CFA].driver_ptr = (void*)&outPrms->vissCFACfg;
    tDCCCompList[DCC_ID_VISS_GLBCE].driver_ptr = (void*)&outPrms->vissGlbceCfg;
    tDCCCompList[DCC_ID_MESH_LDC_J7].driver_ptr = (void*)&outPrms->vpacLdcCfg;
    tDCCCompList[DCC_ID_VISS_LSC].driver_ptr = (void*)&outPrms->vissLscCfg;
#if defined(VPAC3)
    tDCCCompList[DCC_ID_VISS_CAC].driver_ptr = (void*)&outPrms->vissCacCfg;
    tDCCCompList[DCC_ID_VISS_RAWHIST].driver_ptr = (void*)&outPrms->vissRawhistCfg;
    tDCCCompList[DCC_ID_VISS_CFAI3_A].driver_ptr = (void*)&outPrms->vissCfai3aCfg;
    tDCCCompList[DCC_ID_VISS_CFAI3_B].driver_ptr = (void*)&outPrms->vissCfai3bCfg;
    tDCCCompList[DCC_ID_VISS_CCMV].driver_ptr = (void*)&outPrms->vissCcMvCfg;
#endif
#if defined(VPAC3L)
    tDCCCompList[DCC_ID_VISS_CAC].driver_ptr = (void*)&outPrms->vissCacCfg;
    tDCCCompList[DCC_ID_VISS_RAWHIST].driver_ptr = (void*)&outPrms->vissRawhistCfg;
    tDCCCompList[DCC_ID_VISS_CFAI3_A].driver_ptr = (void*)&outPrms->vissCfai3aCfg;
    tDCCCompList[DCC_ID_VISS_DPC_EXT].driver_ptr = (void*)&outPrms->vissDpcExtCfg;
    tDCCCompList[DCC_ID_VISS_LSC_EXT].driver_ptr = (void*)&outPrms->vissLscExtCfg;
    tDCCCompList[DCC_ID_VISS_PCID].driver_ptr = (void*)&outPrms->vissPcidCfg;
#endif
    return 0;
}

int dcc_update(dcc_parser_input_params_t * input_params,
               dcc_parser_output_params_t *output_params)
{
    int32_t                           retVal = 0;

    /*-----------------------------------------------*/
    /*These parameters we are not using currently    */
    /*but some apis requires these as input so these */
    /*will be populated with default values          */
    /*-----------------------------------------------*/
    uint32_t                        camera_module_id;
    dcc_descriptor_id_type          dcc_descriptor_id   = DCC_ID_AAA_ALG_AWB_TI3;
    dcc_algorithm_vendor_id_type    algorithm_vendor_id = DCC_ALG_VENDOR_ID_2;
    dcc_use_case_id_type            use_case_id = DCC_USE_CASE_HIGH_SPEED_PREVIEW;
    dcc_phspace_vals_t              dim_vals;
    uint32_t                        ret_info;
    DCC_GET_STATUS                  sts;
    uint32_t                        module;

    Trace_printf(" dcc_update : ISS_DCC_NUM_SUPPORT_MODULES = %d\n", ISS_DCC_NUM_SUPPORT_MODULES);

    /*-----------------------------------------------*/
    /*initialize the driver structure pointers in    */
    /*driver structure                               */
    /*-----------------------------------------------*/
    update_driver_ptr(output_params);

    camera_module_id = input_params->cameraId;

    /*-----------------------------------------------*/
    /*Present we might not be using the Control      */
    /*parameters this is for future usage            */
    /*-----------------------------------------------*/
    /*-----------------------------------------------*/
    /*Initialize the photo space regions to default  */
    /*values                                         */
    /*-----------------------------------------------*/
    dim_vals[DCC_PS_DIM_ID_AG]           = input_params->analog_gain;
    dim_vals[DCC_PS_DIM_ID_ET]           = input_params->exposure_time;
    dim_vals[DCC_PS_DIM_ID_CT]           = input_params->color_temparature;
    dim_vals[DCC_PS_DIM_ID_FLASH]        = 0;
    dim_vals[DCC_PS_DIM_ID_FOCUS]        = 0;
    dim_vals[DCC_PS_DIM_ID_TOTAL_EXP]    = 264;
    dim_vals[DCC_PS_DIM_ID_FACE_DETECT]  = 0;
    dim_vals[DCC_PS_DIM_ID_SCENE_MODE]   = 0;
    dim_vals[DCC_PS_DIM_ID_EFFECTS_MODE] = 0;
    dim_vals[DCC_PS_DIM_ID_RESERVED_1]   = 0;
    dim_vals[DCC_PS_DIM_ID_RESERVED_2]   = 0;
    dim_vals[DCC_PS_DIM_ID_RESERVED_3]   = 0;


    /*-----------------------------------------------*/
    /*Get the number of supporting dcc modules       */
    /*-----------------------------------------------*/
    for (module = 0; module < ISS_DCC_NUM_SUPPORT_MODULES; module++ )
    {
        dcc_descriptor_id = gDccModuleList[module];

        Trace_printf ("--dcc_descriptor_id = %d---\n", dcc_descriptor_id);

        Trace_printf ("Module requested is %s size %d\n",
            tDCCCompList[dcc_descriptor_id].sCompName,
            tDCCCompList[dcc_descriptor_id].struct_size);

        gDccRetPtrs.desc_id = dcc_descriptor_id;

        /*-----------------------------------------------*/
        /*Loop on each supported component and see if it */
        /*has any update for the dcc profile             */
        /*-----------------------------------------------*/

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (0U != tDCCCompList[dcc_descriptor_id].struct_size)
        /* LDRA_JUSTIFY_END */
        {
            /*-----------------------------------------------*/
            /*Get the dcc profile for the requested component*/
            /*in the given buffer. this also looks at the    */
            /*input dimentions                               */
            /*-----------------------------------------------*/
            sts = process_binary_dcc((uint8_t*)input_params->dcc_buf,
                                     input_params->dcc_buf_size,
                                     camera_module_id,      /* camera ID */
                                     dcc_descriptor_id,     /* DCC ID */
                                     algorithm_vendor_id,   /* vendor ID */
                                     use_case_id,           /* usecase ID */
                                     &dim_vals,
                                     &gDccRetPtrs,
                                     &ret_info);

            /*-----------------------------------------------*/
            /*if status is not OK print appropriate error    */
            /*message                                        */
            /*-----------------------------------------------*/
            Trace_printf ("status = %d \n", sts);
            Trace_printf ("gDccRetPtrs.p_gen_data = 0x%p \n", gDccRetPtrs.p_gen_data);
            Trace_printf ("gDccRetPtrs.p_uc_data = 0x%p \n", gDccRetPtrs.p_uc_data);
            Trace_printf ("gDccRetPtrs.p_parpack = 0x%p \n", gDccRetPtrs.p_parpack);

            if (DCC_GET_ERR_NO_SUCH_DCC == sts)
            {
                Trace_printf ("\n********* No DCC component for module ");
                Trace_printf ("%s sensor ID %d, camera ID %d and vendor ID %d*********\n",
                    tDCCCompList[dcc_descriptor_id].sCompName,
                    dcc_descriptor_id,
                    camera_module_id,
                    algorithm_vendor_id);
            }
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The test framework and test apps cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            else if (DCC_GET_ERR_NO_SUCH_USE_CASE == sts)
            {
                /* It should be fine, AWB parameters does not have
                   any usecase */
                Trace_printf("No use case in dcc profile %d \n",
                    use_case_id);
            }
            else if (DCC_GET_ERR_INVALID_DCC == sts)
            {
                printf ("********* A broken DCC header in DCC buff \
                    found - parsing can not proceed. \n");
                printf ("********* srv_process_binary_dcc sets \
                    ret_info to the DCC ID of the last valid DCC\n");
                printf ("********* component found\n");
                printf("********* Last valid DCC found with DCC \
                    ID %d\n", (int)ret_info);
                retVal = -1;
                break;
            }
            else if (DCC_GET_ERR_IMPROPER_DCC == sts)
            {
                printf ("********* DCC component is found it \
                    seems improper or cut. sys_prm or use_case_parm\n");
                printf ("********* or parpack points outside \
                    this DCC component binary\n");
                printf("********* DCC ID %d\n", (int)ret_info);
                retVal = -1;
                break;
            }
            else if (DCC_GET_ERR_INVALID_CAMERA_ID == sts)
            {
                printf(" Invalid Camera Id %d\n", camera_module_id);
                retVal = -1;
                break;
            }
            /* LDRA_JUSTIFY_END */
            else if (DCC_GET_ERR_INVALID_DESC_ID == sts)
            {
                printf(" Invalid Desc Id %d \n", dcc_descriptor_id);
                /*There is no need to return error in this case*/
                /*retVal = -1;*/
                break;
            }
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start> 
            Rationale: The test framework and test apps cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            else if (DCC_GET_OK == sts)
            /* LDRA_JUSTIFY_END */
            {
                Trace_printf ("Updating for the module %s \n",
                    tDCCCompList[dcc_descriptor_id].sCompName);

                /* LDRA_JUSTIFY_START
                <metric start> statement branch <metric end>
                <justification start> 
                Rationale: The test framework and test apps cannot reach this portion.
                The test framework does not support the configuration required to trigger this error scenario.
                Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
                However, due to the stated rationale, this is not tested.
                <justification end> */
                if (NULL == tDCCCompList[dcc_descriptor_id].comp_parse)
                {
                    printf("No parse function for %s module\n",
                      tDCCCompList[dcc_descriptor_id].sCompName);
                    return -1;
                }
                /* LDRA_JUSTIFY_END */
                /*-----------------------------------------------*/
                /*parse the component binary and get the comp    */
                /*structure                                      */
                /*-----------------------------------------------*/
                {
                    ret_info = tDCCCompList[dcc_descriptor_id].comp_parse(
                         &gDccRetPtrs,
                          NULL,
                          NULL,
                          (uint8_t *) output_params);
                }

                Trace_printf("Expected CRC value for %s module is 0x%x \n",
                    tDCCCompList[dcc_descriptor_id].sCompName,
                    gDccRetPtrs.checksum);

                /* LDRA_JUSTIFY_START
                <metric start> statement branch <metric end>
                <justification start> 
                Rationale: The test framework and test apps cannot reach this portion.
                The test framework does not support the configuration required to trigger this error scenario.
                Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
                However, due to the stated rationale, this is not tested.
                <justification end> */
                if(ret_info != 0)
                {
                    printf("Parser return value of dcc parser for %s module is %d\n",
                      tDCCCompList[dcc_descriptor_id].sCompName,
                      ret_info);
                    return -1;
                }
                /* LDRA_JUSTIFY_END */
            }
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The test framework and test apps cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: If the control reaches here, our code base is expected to print the error status.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            else
            {
                Trace_printf ("Unhandled case for the module %s \n",
                    tDCCCompList[dcc_descriptor_id].sCompName);
                break;
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to print the error status.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        else
        {
            Trace_printf ("Module %s parsing not supported\n",
                tDCCCompList[dcc_descriptor_id].sCompName);

        }
        /* LDRA_JUSTIFY_END */
        Trace_printf ("----------------------------------------\n");
    }

    return retVal;
}

/* ========================================================================== */
/**
*  process_binary_dcc();
*  reads the DCC Profile
*  returns ptr to the Use Case Specific General Parameters
*  for the use case ID passed by the fw3A in use_case_id
*  acts as wrapper for find_parpack();
*
*  @param   filename - the name of the input binary file
*
*  @return  nothing.
*/
/* ========================================================================== */

DCC_GET_STATUS process_binary_dcc(uint8_t* dcc_buf,
                                  uint32_t dcc_bytes,
                                  uint32_t camera_module_id,
                                  dcc_descriptor_id_type dcc_descriptor_id,
                                  dcc_algorithm_vendor_id_type algorithm_vendor_id,
                                  dcc_use_case_id_type use_case_id,
                                  dcc_phspace_vals_t *dim_values,
                                  dcc_ptrs_t         *ret_ptrs,
                                  uint32_t* ret_info)
{
    /*-----------------------------------------------*/
    /*Local pointers to the dcc buffer               */
    /*-----------------------------------------------*/
    uint8_t                        *dcc_ptr = NULL;
    uint8_t                        *p_dcc_file = NULL;
    /*-----------------------------------------------*/
    /*Storage for the header data of current and     */
    /* previous                                      */
    /*-----------------------------------------------*/
    dcc_component_header_type     header_data;
    dcc_component_header_type     header_data_last;
    /*-----------------------------------------------*/
    /*Number of use cases                            */
    /*-----------------------------------------------*/
    uint32_t                        uc_count;
    /*-----------------------------------------------*/
    /*pointer to current use case                    */
    /*-----------------------------------------------*/
    uint8_t                        *ptr2curr_use_case;
    /*-----------------------------------------------*/
    /*offset to current usecase in the dcc buffer    */
    /*-----------------------------------------------*/
    uint32_t                        use_case_offst;
    /*-----------------------------------------------*/
    /*pointer to end of dcc buffer                   */
    /*TODO: Need to remove the dependency on this    */
    /*-----------------------------------------------*/
    uint8_t*                        dcc_buff_end = NULL;

    /*-----------------------------------------------*/
    /*Pointer to general module data                 */
    /*This is for future purpose we might not use now*/
    /*-----------------------------------------------*/
    ret_ptrs->p_gen_data = NULL;
    /*-----------------------------------------------*/
    /*Pointer to usecase specific data               */
    /*This is for future purpose we might not use now*/
    /*-----------------------------------------------*/
    ret_ptrs->p_uc_data  = NULL;
    /*-----------------------------------------------*/
    /*Pointer to profile parameters data             */
    /*-----------------------------------------------*/
    ret_ptrs->p_parpack  = NULL;
    uint32_t found = 0, module;

    dcc_ptr              = dcc_buf;
    p_dcc_file           = dcc_buf;

    /*-----------------------------------------------*/
    /*TODO: convert this printf into trace           */
    /*-----------------------------------------------*/

    /*Trace_printf("camera_module_id = %d,\n dcc_descriptor_id_type = %d,\n \
        dcc_algorithm_vendor_id_type = %d\n dcc_use_case_id_type %d\n",
        camera_module_id,
        dcc_descriptor_id,
        algorithm_vendor_id,
        use_case_id);*/

    /*-----------------------------------------------*/
    /*Compute the end of the dcc buffer              */
    /*-----------------------------------------------*/
    dcc_buff_end         = (uint8_t*)dcc_buf + dcc_bytes;

    /*-----------------------------------------------*/
    /*set the last discriptor id in the header       */
    /*-----------------------------------------------*/
    header_data_last.dcc_descriptor_id = (dcc_descriptor_id_type)0xFFFFFFFF;
    *ret_info = 0;

    /*-----------------------------------------------*/
    /*parse the header data to get different sizes   */
    /*From this we get component specific data size  */
    /*use case specific data size                    */
    /*dcc descriptor data size                       */
    /*using these three we can get pointer to        */
    /*dcc descriptor                                 */
    /*-----------------------------------------------*/

    do
    {
        /*-----------------------------------------------*/
        /*parse the header                               */
        /*-----------------------------------------------*/
        Trace_printf("Calling parse_header_data \n");
        parse_header_data(&dcc_ptr, &header_data);
        Trace_printf("dcc_descriptor_id =%d \n", header_data.dcc_descriptor_id);
        Trace_printf("sz_comp_spec_gen_params =%d \n", header_data.sz_comp_spec_gen_params);
        Trace_printf("sz_uc_spec_gen_params =%d \n", header_data.sz_uc_spec_gen_params);
        Trace_printf("sz_x_dcc_descriptor =%d \n", header_data.sz_x_dcc_descriptor);

        found = 0U;
        for (module = 0; module < ISS_DCC_NUM_SUPPORT_MODULES; module++)
        {
            if (header_data.dcc_descriptor_id == gDccModuleList[module])
            {
                found = 1U;
                Trace_printf("found dcc_descriptor_id %d for module %d \n", header_data.dcc_descriptor_id, module);
                break;
            }
        }

        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (0U == found)
        {
            return DCC_GET_ERR_INVALID_DESC_ID;
        }
        /* LDRA_JUSTIFY_END */

        /* If camera Id is not matching with the running usecase, return error */
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (((header_data.camera_module_id != camera_module_id) &&
             (header_data.camera_module_id != 0xFFFF)))
        {
            Trace_printf ("Error : camera Id mismatch \n");
            return DCC_GET_ERR_INVALID_CAMERA_ID;
        }
        /* LDRA_JUSTIFY_END */

        /*-----------------------------------------------*/
        /* 0xFFFF for camera_module_id = fits ANY module */
        /*-----------------------------------------------*/
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
        An else block can be added to catch and print the error status in a future release.
        <justification end> */
        if (((header_data.camera_module_id == camera_module_id)||
             (header_data.camera_module_id == 0xFFFF))        &&
            (header_data.dcc_descriptor_id == dcc_descriptor_id) &&
            (header_data.algorithm_vendor_id == algorithm_vendor_id))
        /* LDRA_JUSTIFY_END */
        {
            Trace_printf ("Found the dcc descriptor for given id \n");
            break;
        }

        /*-----------------------------------------------*/
        /*move to next dcc descriptor                    */
        /*-----------------------------------------------*/
        p_dcc_file += header_data.total_file_sz;
        dcc_ptr = p_dcc_file;

        /*-----------------------------------------------*/
        /* The below if condition will be true if the    */
        /*file end was reached.So we could not find any  */
        /*valid dcc profile                              */
        /*-----------------------------------------------*/
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if((p_dcc_file > dcc_buff_end) || (p_dcc_file < dcc_buf))
        {
            *ret_info = header_data_last.dcc_descriptor_id;
            return DCC_GET_ERR_INVALID_DCC;
        }
        /* LDRA_JUSTIFY_END */

        /*-----------------------------------------------*/
        /*Take the back up of the header data            */
        /*-----------------------------------------------*/
        memcpy(&header_data_last, &header_data, sizeof(header_data_last));
    } while (dcc_ptr < dcc_buff_end);

    /*-----------------------------------------------*/
    /*If no DCC component for that alg,sensor and alg*/
    /* vendor, return error.                         */
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (dcc_ptr >= dcc_buff_end){
        Trace_printf ("no DCC component for the plugin 0x%x, sensor 0x%x and vendor 0x%x \n", 
              dcc_descriptor_id, camera_module_id, algorithm_vendor_id);
        return DCC_GET_ERR_NO_SUCH_DCC;
    }
    /* LDRA_JUSTIFY_END */

    ret_ptrs->checksum = header_data.crc_checksum;

    /*-----------------------------------------------*/
    /*parse the use case table of contents           */
    /*so you can use the offsets to find the address */
    /*of the current use case passed by the fw3A     */
    /*-----------------------------------------------*/
    use_case_offst = find_use_case_block(&dcc_ptr,
                                         use_case_id,
                                         &uc_count);

    /*-----------------------------------------------*/
    /*fill pointer to DCC component specific         */
    /*general data if it is given in the dcc params  */
    /*-----------------------------------------------*/
    if(header_data.sz_comp_spec_gen_params != 0)
    {
        /*-----------------------------------------------*/
        /*if there is component specific general data    */
        /*-----------------------------------------------*/
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if((dcc_ptr > p_dcc_file + header_data.total_file_sz) ||
           (dcc_ptr < p_dcc_file)) {
            *ret_info = header_data.dcc_descriptor_id;
            return DCC_GET_ERR_IMPROPER_DCC;
        }
        /* LDRA_JUSTIFY_END */

        ret_ptrs->p_gen_data = dcc_ptr;
    }

    if (uc_count == 0){
        /*-----------------------------------------------*/
        /*no use cases in this DCC, only general data    */
        /*-----------------------------------------------*/
        return DCC_GET_OK;
    }

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (use_case_offst == 0) {
        /*-----------------------------------------------*/
        /*no such use case even though there are         */
        /*use cases in DCC                               */
        /*-----------------------------------------------*/
        Trace_printf ("no such use case even though there are use cases in DCC  \n");
        return DCC_GET_ERR_NO_SUCH_USE_CASE;
    }
    /* LDRA_JUSTIFY_END */

    /*-----------------------------------------------*/
    /*Go to current use case offset                  */
    /*-----------------------------------------------*/
    ptr2curr_use_case = p_dcc_file + use_case_offst;

    /*-----------------------------------------------*/
    /*If there is any usecase specific general data  */
    /*It will be at beginning of each use case block */
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The test framework and test apps cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(header_data.sz_uc_spec_gen_params != 0){
        if((ptr2curr_use_case > p_dcc_file + header_data.total_file_sz) ||
           (ptr2curr_use_case < p_dcc_file))
        {
            *ret_info = header_data.dcc_descriptor_id;
            Trace_printf ("Improper DCC  \n");
            return DCC_GET_ERR_IMPROPER_DCC;
        }

        ret_ptrs->p_uc_data = ptr2curr_use_case;
    }
    /* LDRA_JUSTIFY_END */

    /*-----------------------------------------------*/
    /*If the dcc descriptor is present               */
    /*-----------------------------------------------*/
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if(header_data.sz_x_dcc_descriptor != 0)
    /* LDRA_JUSTIFY_END */
    {
        /*-----------------------------------------------*/
        /*if there are parameter packages in DCC parse it*/
        /*-----------------------------------------------*/
        uint8_t *ppack;

        //skip usecase params, point to number of PHspace dimensions
        ppack = find_parpack(
            ptr2curr_use_case + header_data.sz_uc_spec_gen_params,
            header_data.sz_x_dcc_descriptor,
            ret_ptrs);

        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The test framework and test apps cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if( (ppack > p_dcc_file + header_data.total_file_sz) ||
            (ppack < p_dcc_file)) {
            *ret_info = header_data.dcc_descriptor_id;
            Trace_printf ("IMPROPER DCC \n");
            return DCC_GET_ERR_IMPROPER_DCC;
        }
        /* LDRA_JUSTIFY_END */
        ret_ptrs->p_parpack = ppack;
    }

    return DCC_GET_OK;

}
