/*****************************************************************
 * dcc_header.c
 */
#define _CRT_SECURE_NO_DEPRECATE


#include <stdio.h>
#include <string.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"

int proc_header_start(dtp_gen_ctx_t* ctx, const char *name, const char **attr)
{
    //Nothing to do here really. All is done in the end handler.
    return 0;
}

int proc_header_end(dtp_gen_ctx_t* ctx, const char *name)
{
    int err = 0;
    int current = 0;
    int64 value = 0;

    if(!ctx->xml_data_pending){
        printf("No data for this tag: %s\n",name);
        return 1;
    }

    if( tls_prepare_data_str(ctx) )
    {
		return -1;
    }
    if (make_number_from_string(ctx->data_str,&current,(int64*)&value))		//make number from the string
	{
		printf("Could not parse value from string\n");
		return -1;
	}
	if (0 > value || value > 65536)
	{
		printf("Error: node type is uint16, trying to initialise with %lld out of range\n",value);
		return -1;
	}
    if(strcmp(name, XML_HEADER_NAME_CAMERA) == 0) {
        ctx->xml_header.camera_module_id = (uint32)value;
    }
    else if(strcmp( name, XML_HEADER_NAME_ALGO ) == 0){
        ctx->xml_header.dcc_descriptor_id = (uint32)value;
    }
    else if(strcmp( name, XML_HEADER_NAME_VENDOR ) == 0){
        ctx->xml_header.algorithm_vendor_id = (uint32)value;
    }
    else if(strcmp( name, XML_HEADER_NAME_TUNE ) == 0){
        ctx->xml_header.dcc_tuning_tool_version = (uint32)value;
    }
    else {
        err = 1;
    }
    //clear the data that data_handler left for us to deal with
    ctx->xml_data_pending = 0;
    ctx->data_str_len     = 0;
    ctx->data_str[0] = 0;
    return err;
}
