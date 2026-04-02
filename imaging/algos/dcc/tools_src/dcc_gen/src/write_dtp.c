/*****************************************************************
 * tools.c
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ctypes.h"
#include "private.h"
#include "tools.h"

#include "write_dtp.h"

/* ========================================================================= */
/*                    DEFINES                                                */
/* ========================================================================= */
#define _CRTDBG_MAP_ALLOC
const char* USE_CASE_NAMES[] = {
    "STILL CAPTURE, HIGH QUALITY"       ,
    "STILL CAPTURE, HIGH PERFORMANCE"
};


//This define is used to limit TUNER mode to writing binary data only and no TXT.
#ifndef DEVEL
#ifndef DCCREAD
    #define FPRINTF_DEV fprintf_null
#endif
#endif

#ifndef FPRINTF_DEV
    #define FPRINTF_DEV fprintf
#endif

//Used if we are in tuner mode. This function will take all arguments and not use them.
int fprintf_null (FILE * fin, const char * form, ...)
{
    return 0;
}

/* ========================================================================= */
/*                    variables                                              */
/* ========================================================================= */

FILE* fout_bin;
FILE* fout_txt;
static char type_name[100];
#ifdef DCCREAD
dccread_t dccinfo;
#endif


/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */


/* ========================================================================== */
/**
*  void put_u8(dtp_file_t* file,uint8 *ptr, uint8 val) puts 1 byte in dtp
*
*  @param   ptr - pointer to dtp data
*
*  @param
*
*  @return  nothing.
*/
/* ========================================================================== */
void put_u8(void* in, uint8 val)
{
    dtp_file_t* file = in;
    file->dcc_size += sizeof(uint8);
    if(file->dcc_size > DTP_MAX_SIZE){
		printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		dtp_gen_abort(1);
        return;
    }

    *(file->dtp) = val;
    file->dtp++;
}

/* ========================================================================== */
/**
*  void put_u16_little_endian(uint8 *ptr,uint16 val) puts 2 bytes in dtp
*
*  @param   ptr - pointer to dtp data
*
*  @param
*
*  @return  nothing.
*/
/* ========================================================================== */
void put_u16_little_endian(void *in, uint16 val)
{
    dtp_file_t* file = in;

    file->dcc_size += sizeof(uint16);
    if(file->dcc_size > DTP_MAX_SIZE){
		printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		dtp_gen_abort(1);
        return;
    }
    *(file->dtp) = (uint8)(val & 0x00FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 8) & 0x00FF);
    file->dtp++;
}

/* ========================================================================== */
/**
*  void put_u32_little_endian(uint8 *ptr,uint32 val) puts 4 bytes in dtp little endian
*
*  @param   ptr - pointer to dtp data
*
*  @param
*
*  @return  nothing.
*/
/* ========================================================================== */
void put_u32_little_endian(void *in, uint32 val)
{
    dtp_file_t* file = in;

    file->dcc_size += sizeof(uint32);
    if(file->dcc_size > DTP_MAX_SIZE){
		printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		dtp_gen_abort(1);
        return;
    }
    *(file->dtp) = (uint8)(val & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 8) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 16) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 24) & 0x000000FF);
    file->dtp++;
}

/* ========================================================================== */
/**
*  void put_u16_big_endian(uint8 *ptr,uint16 val) put 2 bytes in dtp in big endian
*
*  @param   ptr - pointer to dtp data
*
*  @param
*
*  @return  nothing.
*/
/* ========================================================================== */
void put_u16_big_endian(void *in, uint16 val)
{
    dtp_file_t* file = in;

    file->dcc_size += sizeof(uint16);
    if(file->dcc_size > DTP_MAX_SIZE){
		printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		dtp_gen_abort(1);
        return;
    }
    *(file->dtp) = (uint8)((val >> 8) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)(val & 0x000000FF);
    file->dtp++;
}

/* ========================================================================== */
/**
*  void put_u32_big_endian(uint8 *ptr, uint32 val) puts 4 bytes in dtp in big endian
*
*  @param   ptr - pointer to dtp data
*
*  @param
*
*  @return  nothing.
*/
/* ========================================================================== */
void put_u32_big_endian(void *in, uint32 val)
{
    dtp_file_t* file = in;

    file->dcc_size += sizeof(uint32);
    if(file->dcc_size > DTP_MAX_SIZE){
		printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		dtp_gen_abort(1);
        return;
    }
    *(file->dtp) = (uint8)((val >> 24) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 16) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)((val >> 8) & 0x000000FF);
    file->dtp++;
    *(file->dtp) = (uint8)(val & 0x000000FF);
    file->dtp++;
}

#ifdef DCCREAD
static uint8 get_uint8(void* in){
    dtp_file_t* file = in;

    uint8 byte = *(file->dtp++);
    file->dcc_size += sizeof(uint8);
    return byte;
}

static uint16 get_uint16(void* in){
    dtp_file_t* file = in;
    uint16 byte2;

    byte2 = *(file->dtp++);
    byte2 |= (uint16)(*(file->dtp++)) << 8;

    file->dcc_size += sizeof(uint16);
    return byte2;
}

static uint32 get_uint32(void* in){
    dtp_file_t* file = in;

    uint32 byte4;
    byte4 = *(file->dtp++);
    byte4 |= (uint32)(*(file->dtp++)) << 8;
    byte4 |= (uint32)(*(file->dtp++)) << 16;
    byte4 |= (uint32)(*(file->dtp++)) << 24;

    file->dcc_size += sizeof(uint32);
    return byte4;
}
#endif

void dtp_align(uint8 **dtp, uint32 *size)
{
    uint8 bytes_to_add, i;

    bytes_to_add = (uint8)(4 - ((*size) % 4));
    for (i = 0; i < bytes_to_add; i++) {
        put_u8(dtp, DTP_FILE_HEADER_RESERVED_BYTE);
    }
    *size += bytes_to_add;
}

int dtp_write(dtp_file_t *file, char *outfilename)
{
    uint8 *dtp;
    //uint32 i;
    FILE* dtp_file;

    if (file->dtp_file == NULL) {
        printf("ERROR: no BIN file contents to write\n");
        return (-1);
    }

    dtp = file->dtp_file;

    // write dtp file
    dtp_file = fopen(outfilename,"wb");
    if (dtp_file == NULL) {
        printf("ERROR: Could not open %s for write\n", outfilename);
        return (-1);
    }

    if( fwrite(file->dtp_file,file->dcc_size,1,dtp_file) != 1){
        printf("ERROR: writing %s failed\n", outfilename);
        fclose(dtp_file);
        return (-1);
    }
    fclose(dtp_file);

    //release dtp file memory
    if (file->dtp_file != NULL) {
        osal_free(file->dtp_file);
    }
    return (0);
}

/* ============================================================================
* write_number ()
*
* A number field has no children. Its value is in val[], which is a pointer to a single number in this case
* ========================================================================== */
int write_number(dtp_file_t* file, struct tree_node_t* tree, int indent)
{
    type_to_string(tree->type, tree->arr_dim_l, tree->arr_dim_m, tree->arr_dim_r, (char*)&type_name);
    write_indent(fout_txt,indent);
    if(tree->val != NULL){
#ifndef DCCREAD
		FPRINTF_DEV(fout_txt, "%ld,  // %s %s\n", *tree->val, type_name, tree->name );
#endif
		if(file){
            switch(tree->type & BASIC_TYPE_MASK)
            {
                case BASIC_TYPE_INT8:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%d,  // %s %s\n", (int8)file->g_ehdl.get_u8(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u8(file, (uint8)(*tree->val));
#endif
                    break;


                case BASIC_TYPE_UINT8:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%d,  // %s %s\n", file->g_ehdl.get_u8(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u8(file, (uint8)(*tree->val));
#endif
                    break;

                case BASIC_TYPE_INT16:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%d,  // %s %s\n", (int16)file->g_ehdl.get_u16(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u16(file, (uint16)(*tree->val));
#endif
                    break;

                case BASIC_TYPE_UINT16:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%d,  // %s %s\n", file->g_ehdl.get_u16(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u16(file, (uint16)(*tree->val));
#endif
                    break;
                case BASIC_TYPE_INT32:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%ld,  // %s %s\n", (int32)file->g_ehdl.get_u32(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u32(file, (uint32)(*tree->val));
#endif
                    break;

                case BASIC_TYPE_UINT32:
#ifdef DCCREAD
                    FPRINTF_DEV(fout_txt, "%ld,  // %s %s\n", file->g_ehdl.get_u32(file), type_name, tree->name );
#else
                    file->p_ehdl.put_u32(file, (uint32)(*tree->val));
#endif
                    break;
                default:
                    printf("ERROR Writing number in binary file.No simple type!\n");
                    FPRINTF_DEV(fout_txt, "ERROR Writing number in binary file.No simple type!\n");
                    return (-1);
            }
		}
    } else {
        //no value set
        FPRINTF_DEV(fout_txt, " ,  // %s %s\n", type_name, tree->name );
    }
    return 0;
}

/* ============================================================================
* write_struct ()
*
* A struct must have children. Their values are in their val[] fields
* ========================================================================== */
int write_struct(dtp_file_t* file,struct tree_node_t* tree, int indent)
{
    write_indent(fout_txt,indent);

    FPRINTF_DEV(fout_txt, "struct %s = {\n", tree->name);

    if(tree->child == NULL){
        //The structure has no fields
        printf("ERROR: this struct has no children\n");
        FPRINTF_DEV(fout_txt, "ERROR: this struct has no children\n");
        return (-1);
	}

    write_tree(file,tree->child, (indent+1));

    write_indent(fout_txt,indent);
    FPRINTF_DEV(fout_txt, "}  // %s\n", tree->name);
    return 0;
}

/* ============================================================================
* write_array()
*
* Array can be any simple type (number or a struct)
* If its type is a number, it has no children, the number type is included in the array tree 'type' field and all the values are in val[] field
* If its type is a struct, the array tree has identical children of struct type with values in their corresponding val[] fields.
* ========================================================================== */
int write_array (dtp_file_t* file,struct tree_node_t* tree, int indent)
{
    uint32 dim_l = 0;
    uint32 dim_m = 0;
    uint32 dim_r = 0;
    uint32 dim_right = 0;
    uint32 dim_mid = 0;
    uint32 dim_left = 0;
    uint8 dims;
	uint32 i_right = 0, i_mid = 0, i_left = 0;
    int arr_of_structs;
    struct tree_node_t* list_1d;
    struct tree_node_t* list_2d;
#ifndef DCCREAD
    uint32 tmp = 0;
#endif

    if ((tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
        if(tree->child == NULL){
            printf("ERROR: this tree is array of structs and has no children\n");
            FPRINTF_DEV(fout_txt, "ERROR: this tree is array of structs and has no children\n");
            return (-1);
        }
    }

    type_to_string(tree->type, tree->arr_dim_l, tree->arr_dim_m, tree->arr_dim_r, (char*)&type_name);
    write_indent(fout_txt, indent);
    FPRINTF_DEV(fout_txt, "%s %s = {\n", type_name, tree->name );

    dim_l = tree->arr_dim_l;
    dim_m = tree->arr_dim_m;
    dim_r = tree->arr_dim_r;

    dims = (uint8)((tree->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);

#ifdef DCCREAD
    if(tree->type & BASIC_TYPE_ARRAY_UNDEF){
        if(file){
            dim_l = file->g_ehdl.get_u32(file);
        }
        FPRINTF_DEV(fout_txt, "UNDEF Arr count: %d\n",dim_l);
    }
#endif
    switch (dims) {
        case 3:
            dim_left  = dim_l;
            dim_mid   = dim_m;
            dim_right = dim_r;
            break;
        case 2:
            dim_left  = 0;
            dim_mid   = dim_l;
            dim_right = dim_m;
            break;
        case 1:
        default:
            dim_left  = 0;
            dim_mid   = 0;
            dim_right = dim_l;
    }
#ifndef DCCREAD
    if(tree->type & BASIC_TYPE_ARRAY_UNDEF){
        FPRINTF_DEV(fout_txt, "UNDEF Arr count: %d\n",dim_l);
        if(file){
            file->p_ehdl.put_u32(file, (uint32)dim_l);
        }
    }
#endif
    arr_of_structs = 0;
    if((tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT) {
        arr_of_structs = 1;
    }

    indent++;
    //fill values
    list_2d = tree;
    if (arr_of_structs && dim_left){
        list_2d = tree->child;
    }
    do { //}while (i_left < dim_left);
        if(list_2d == NULL){
            FPRINTF_DEV(fout_txt, "Not enough elements in multi-dimentional array of structs\n");
            printf("Not enough elements in multi-dimentional array of structs\n");
            return(-1);
        }
        if (dim_left){
            write_indent(fout_txt, indent);
            FPRINTF_DEV(fout_txt, "{\n");
            indent++;
        }

        list_1d = list_2d;
        if (arr_of_structs && dim_mid){
            list_1d = list_2d->child;
        }
		i_mid = 0;
        do { //}while (i_mid < dim_mid);
            if (dim_mid){
                write_indent(fout_txt, indent);
                FPRINTF_DEV(fout_txt, "{\n");
                indent++;
            }


            if (arr_of_structs){
                //This is an array of structs or arrays
                struct tree_node_t* next_elem;

                if(list_1d == NULL){
                    FPRINTF_DEV(fout_txt, "Not enough elements in multi-dimentional array of structs\n");
                    printf("Not enough elements in multi-dimentional array of structs\n");
                    return(-1);
                }
                next_elem = list_1d->child;
                for(i_right=0; i_right<dim_right; i_right++)
                {
                    if(next_elem == NULL) {
                        FPRINTF_DEV(fout_txt, "Too few elements in this array\n");
                        printf("Too few elements in this array\n");
                        return (-1);
                    }
                    if(next_elem->type & BASIC_TYPE_ARRAY){
                        if( write_array(file, next_elem, indent) ){
                            return (-1);
                        }
                    } else {
                        if( write_struct(file, next_elem, indent) ){
                            return (-1);
                        }
                    }
                    next_elem = next_elem->next;
                }
            } else {
                //simple type - the values are in tree->val
                write_indent(fout_txt, indent);
                for(i_right=0; i_right<dim_right; i_right++) {
                    if(tree->val) {
#ifndef DCCREAD
                        tmp = *(tree->val + (dim_mid*dim_right)*i_left +
                                                   dim_right *i_mid  +
                                                              i_right);
                        FPRINTF_DEV(fout_txt, "%ld, ",tmp);
#endif
                        if(file){
                            switch(tree->type & BASIC_TYPE_MASK)
                            {
                                case BASIC_TYPE_INT8:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%d, ",(int8)file->g_ehdl.get_u8(file));
#else
                                    file->p_ehdl.put_u8(file, (uint8)tmp);
#endif
                                    break;

                                case BASIC_TYPE_UINT8:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%d, ",file->g_ehdl.get_u8(file));
#else
                                    file->p_ehdl.put_u8(file, (uint8)tmp);
#endif
                                    break;

                                case BASIC_TYPE_INT16:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%d, ",(int16)file->g_ehdl.get_u16(file));
#else
                                    file->p_ehdl.put_u16(file, (uint16)tmp);
#endif
                                    break;

                                case BASIC_TYPE_UINT16:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%d, ",file->g_ehdl.get_u16(file));
#else
                                    file->p_ehdl.put_u16(file, (uint16)tmp);
#endif
                                    break;
                                case BASIC_TYPE_INT32:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%ld, ",(int32)file->g_ehdl.get_u32(file));
#else
                                    file->p_ehdl.put_u32(file, (uint32)tmp);
#endif
                                    break;

                                case BASIC_TYPE_UINT32:
#ifdef DCCREAD
                                    FPRINTF_DEV(fout_txt,"%ld, ",file->g_ehdl.get_u32(file));
#else
                                    file->p_ehdl.put_u32(file, (uint32)tmp);
#endif
                                    break;
                                default:
                                    FPRINTF_DEV(fout_txt, "ERROR Writing array in binary file.No simple type!\n");
                                    printf("ERROR Writing array in binary file.No simple type!\n");
                                    return (-1);
                            }
                        }
                    } else {
                        //values are not set
                        FPRINTF_DEV(fout_txt, " , ");
                    }
                }
                FPRINTF_DEV(fout_txt, "\n");
            }

            if (dim_mid){
                indent--;
                write_indent(fout_txt, indent);
                FPRINTF_DEV(fout_txt, "},\n");
            }

            i_mid++;
            if(arr_of_structs){
                list_1d = list_1d->next;
            }
        } while (i_mid < dim_mid);

        if (dim_left){
            indent--;
            write_indent(fout_txt, indent);
            FPRINTF_DEV(fout_txt, "},\n");
        }
        i_left++;
        if(arr_of_structs){
            list_2d = list_2d->next;
        }
    } while (i_left < dim_left);

    indent--;
    write_indent(fout_txt, indent);
    FPRINTF_DEV(fout_txt, "},\n");
    return 0;
}

/* ============================================================================
* write_tree ()
*
* ========================================================================== */
int write_tree (dtp_file_t* file,struct tree_node_t* tree, int indent)
{
	struct tree_node_t * next_elem = tree;

	while (next_elem != NULL)
    {
        //This tree is a field of a parrent type, there are other fields in a list.
        //They are at same depth (indent level)
        if(next_elem->type & BASIC_TYPE_ARRAY){
            if( write_array(file,next_elem, indent) ){
                return (-1);
            }
        } else if ((next_elem->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
            if( write_struct(file,next_elem, indent) ){
                return (-1);
            }
        } else {
            if( write_number(file,next_elem, indent) ){
                return (-1);
            }
        }
        next_elem = next_elem->next;
    }
	return 0;
}


void write_bin_header(dtp_file_t* file, dtp_gen_ctx_t* ctx)
{
    // TODO: CHECK if we need this!
    /*
    // put dtp start marker
    // 1th byte
    file->ehdl.put_u8(file, DTP_FILE_HEADER_ENDIAN);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_IDENT_BYTE_1);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_IDENT_BYTE_2);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_IDENT_BYTE_3);

    // 5th byte, put ascii version for hex viewing
    file->ehdl.put_u8(file, DTP_FILE_HEADER_VERSION_BYTE1);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_VERSION_BYTE2);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_VERSION_BYTE3);
    file->ehdl.put_u8(file, DTP_FILE_HEADER_VERSION_BYTE4);
    //*/

    // save the file size value - the header and everything else is already
    //included. Subsequens put_...() will increase it.
    ctx->xml_header.total_file_sz = file->dcc_size;
    ctx->xml_header.dcc_reserved_0 = DCC_GEN_VERSION;
    file->p_ehdl.put_u32(file,ctx->xml_header.camera_module_id);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_descriptor_id);
    file->p_ehdl.put_u32(file,ctx->xml_header.algorithm_vendor_id);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_tuning_tool_version); //0xFFFF for no tool used
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_profile_time_stamp);
    file->p_ehdl.put_u32(file,ctx->xml_header.crc_checksum);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_0);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_1);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_2);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_3);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_4);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_5);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_6);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_7);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_8);
    file->p_ehdl.put_u32(file,ctx->xml_header.dcc_reserved_9);
    file->p_ehdl.put_u32(file,ctx->xml_header.sz_comp_spec_gen_params);
    file->p_ehdl.put_u32(file,ctx->xml_header.sz_uc_spec_gen_params);
    file->p_ehdl.put_u32(file,ctx->xml_header.sz_x_dcc_descriptor);

/* old : wrong. Total file size should be taken from dcc_size after filling
the last parpack - header is included in it

    //calculate the toatal filesize;
    ctx->xml_header.total_file_sz = 0;
    ctx->xml_header.total_file_sz += sizeof(uint32); //USECASE COUNT filed
    //USECASE TABLE
    ctx->xml_header.total_file_sz += ctx->use_case_count * 3 * sizeof(uint32);
    ctx->xml_header.total_file_sz += sizeof(uint32);//THE SIZE FIELD ITSELF
    ctx->xml_header.total_file_sz += file->dcc_size;
*/
    file->p_ehdl.put_u32(file,ctx->xml_header.total_file_sz);
}

#ifdef DCCREAD

uint8* read_dcc(char* filename,uint32* filesize )
{
    FILE* in_file;
    uint8 buffer[80];
    dtp_file_t file;

    in_file = fopen(filename,"rb");
    if(!in_file){
        printf("ERROR opening input file %s\n",filename);
        return NULL;
    }
    memset(buffer,0,80);
    if( fread(buffer,80,1,in_file) != 80 )
    {
        //printf("Error reading header\n");
    }
    fclose(in_file);

    file.dtp = (uint8*)(buffer+76);

    *filesize = get_uint32(&file);

    file.dtp = (uint8*) osal_malloc(*filesize);

    in_file = fopen(filename,"rb");
    if(!in_file){
        osal_free(file.dtp);
        printf("ERROR opening input file %s\n",filename);
        return NULL;
    }
    memset(file.dtp,0,*filesize);
    if( fread(file.dtp,*filesize,1,in_file)!= *filesize)
    {
        //printf("Error reading dcc file\n");
    }
    fclose(in_file);

    return file.dtp;
}

void fill_header_data(dcc_component_header_type* xml_header,dtp_file_t * file)
{

    xml_header->camera_module_id = file->g_ehdl.get_u32(file);
    xml_header->dcc_descriptor_id = file->g_ehdl.get_u32(file);
    xml_header->algorithm_vendor_id = file->g_ehdl.get_u32(file);
    xml_header->dcc_tuning_tool_version = file->g_ehdl.get_u32(file); //0xFFFF for no tool used
    xml_header->dcc_profile_time_stamp = file->g_ehdl.get_u32(file);
    xml_header->crc_checksum = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_0 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_1 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_2 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_3 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_4 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_5 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_6 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_7 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_8 = file->g_ehdl.get_u32(file);
    xml_header->dcc_reserved_9 = file->g_ehdl.get_u32(file);
    xml_header->sz_comp_spec_gen_params = file->g_ehdl.get_u32(file);
    xml_header->sz_uc_spec_gen_params = file->g_ehdl.get_u32(file);
    xml_header->sz_x_dcc_descriptor = file->g_ehdl.get_u32(file);
    xml_header->total_file_sz = file->g_ehdl.get_u32(file);
}

void write_usecase(dtp_file_t* file, dtp_gen_ctx_t* ctx)
{

    uint32 i,c,d;
    uint32 regions_count;
    uint32 *min_dims;
    uint32 *max_dims;
    uint32 *ph_dims;

    //Pointers to any valid tree structure
    struct use_case_t* current_usecase;
    struct par_package_t* current_parpak;

    usecase_entry_t* uc_table=NULL;

    if( dccinfo.usecase_count == 0 ){
        return;
    }

    current_usecase = ctx->use_cases;

    ctx->current_usecase = ctx->use_cases;
    while(ctx->current_usecase){
        if(ctx->current_usecase->parpak_count){
            current_parpak = ctx->current_usecase->par_pack;
            break;
        }
        ctx->current_usecase = ctx->current_usecase->next;
    }

    uc_table = (usecase_entry_t*) osal_malloc(sizeof(usecase_entry_t) * dccinfo.usecase_count);

    //point to start of UC table
    //start+ size of binary header + UC count
    file->dtp = file->dtp_file + 84;
    for(i = 0;i<dccinfo.usecase_count;i++){
        uc_table[i].id    = get_uint32(file);
        uc_table[i].start = get_uint32(file);
        uc_table[i].size  = get_uint32(file);


        //USECASE GEN PARAMS
        printf("UC:\nid = %ld\nstart = %ld\nsize = %ld\n",
               uc_table[i].id,uc_table[i].start,uc_table[i].size);
    }
    file->dtp = file->dtp_file + uc_table[0].start;

    for(i = 0;i<dccinfo.usecase_count;i++){

        FPRINTF_DEV(fout_txt, "===================================================================================\n");
        FPRINTF_DEV(fout_txt, "Use cases : 0x%lX\n", uc_table[i].id);
        FPRINTF_DEV(fout_txt, "===================================================================================\n\n");

        if(dccinfo.dcc_header.sz_uc_spec_gen_params){
            uc_table[i].buffer = file->dtp_file + uc_table[i].start;
            file->dtp += dccinfo.dcc_header.sz_uc_spec_gen_params;
        } else {
            uc_table[i].buffer = NULL;
        }

        //USECASE PHOTOSPACE
        // Get the photospace dimensions count and IDs
        uc_table[i].ph_dims_count = get_uint32(file);
        ph_dims = osal_malloc(uc_table[i].ph_dims_count*sizeof(uint32));
        for(c=0;c<uc_table[i].ph_dims_count;c++){
            ph_dims[c] = get_uint32(file);
        }
        regions_count = get_uint32(file);
        uc_table[i].parpak_count = get_uint32(file);

        for(c=0;c<regions_count;c++)
        {
            FPRINTF_DEV(fout_txt,"region region%d par_pak N %ld:" , c ,get_uint32(file));

            min_dims = osal_malloc(uc_table[i].ph_dims_count*sizeof(uint32));
            max_dims = osal_malloc(uc_table[i].ph_dims_count*sizeof(uint32));

            for(d=0;d<uc_table[i].ph_dims_count;d++){
                min_dims[d] = get_uint32(file);
            }
            for(d=0;d<uc_table[i].ph_dims_count;d++){
                max_dims[d] = get_uint32(file);
            }

            for(d=0;d<uc_table[i].ph_dims_count;d++){
                FPRINTF_DEV(fout_txt,"dimension %ld : %ld ... %ld\n", ph_dims[d],
                                                        min_dims[d],
                                                        max_dims[d]);
            }

            osal_free(min_dims);
            osal_free(max_dims);
        }
        osal_free(ph_dims);
        //USECASE PARPACKS

        if(uc_table[i].parpak_count){
            uc_table[i].parpak_buffer = osal_malloc(uc_table[i].parpak_count * sizeof(uint8*));
            for(d=0;d<uc_table[i].parpak_count;d++){
                uc_table[i].parpak_buffer[d] = file->dtp;
                file->dtp += dccinfo.dcc_header.sz_x_dcc_descriptor;
            }
        } else {
            uc_table[i].parpak_buffer = NULL;
        }
    }

    for(i = 0;i<dccinfo.usecase_count;i++){
        
        FPRINTF_DEV(fout_txt, "===================================================================================\n");
        FPRINTF_DEV(fout_txt, "Use cases : 0x%lX\n", uc_table[i].id);
        FPRINTF_DEV(fout_txt, "===================================================================================\n\n");

        current_usecase = ctx->use_cases;
        for(d=0;d<i;d++){
            current_usecase = current_usecase->next;
        }

        if(dccinfo.dcc_header.sz_uc_spec_gen_params) {
            file->dtp = uc_table[i].buffer;
            write_tree(file,current_usecase->use_case_parameter,0);
        }
        if(uc_table[i].parpak_count){
            FPRINTF_DEV(fout_txt, "---- par packs ------------\n");
            current_parpak = current_usecase->par_pack;
            for(d=0;d<uc_table[i].parpak_count;d++){
                FPRINTF_DEV(fout_txt, "par pack (class) N %d\n",d);
                file->dtp = uc_table[i].parpak_buffer[d];
                write_tree(file,current_parpak->package,0);
                if(current_parpak->next){
                    current_parpak = current_parpak->next;
                }
            }
        }
        if(uc_table[i].parpak_buffer){
            osal_free(uc_table[i].parpak_buffer);
        }
    }
    osal_free(uc_table);
}
#else

void write_usecase(dtp_file_t* file, dtp_gen_ctx_t* ctx)
{
    struct par_package_t* current_parpak = NULL;
    dtp_file_t tmp_buffer;
    struct photo_region_t* phreg;
    struct space_t* current_limit;

    if( !ctx->use_cases ){
        return;
    }

    tmp_buffer.dcc_size = 0;
    tmp_buffer.p_ehdl.put_u32 = put_u32_little_endian;
    tmp_buffer.p_ehdl.put_u16 = put_u16_little_endian;
    tmp_buffer.p_ehdl.put_u8  = put_u8;
    tmp_buffer.dtp = osal_malloc(DTP_MAX_SIZE);
    if(!tmp_buffer.dtp){
        printf("Not enough memory for writing usecases to binary.\n");
        dtp_gen_abort(1);
    }
    tmp_buffer.dtp_file = tmp_buffer.dtp;

    ctx->current_usecase = ctx->use_cases;
    //Prepare all parpacks and general data inside each usecase
    while(ctx->current_usecase){

        ctx->current_usecase->parpak_count = 0;
        if(ctx->current_usecase->use_case_parameter) {
            write_tree(&tmp_buffer,ctx->current_usecase->use_case_parameter,0);
            ctx->current_usecase->buffer_size = tmp_buffer.dcc_size;
            ctx->current_usecase->write_buffer = osal_malloc(ctx->current_usecase->buffer_size);
            if(!ctx->current_usecase->write_buffer){
                printf("Not enough memory for writing usecases to binary.\n");
                dtp_gen_abort(1);
            }
            memcpy(ctx->current_usecase->write_buffer,
                   tmp_buffer.dtp_file,
                   ctx->current_usecase->buffer_size);
            if(ctx->usecase_gen_bin_size < ctx->current_usecase->buffer_size){
                ctx->usecase_gen_bin_size = ctx->current_usecase->buffer_size;
            }
        }
        tmp_buffer.dtp = tmp_buffer.dtp_file;
        tmp_buffer.dcc_size = 0;
        current_parpak = ctx->current_usecase->par_pack;

        while(current_parpak){
            ctx->current_usecase->parpak_count++;
            write_tree(&tmp_buffer,current_parpak->package,0);
            current_parpak->buffer_size = tmp_buffer.dcc_size;
            current_parpak->write_buffer = osal_malloc(current_parpak->buffer_size);
            if(!current_parpak->write_buffer){
                printf("Not enough memory for writing usecases to binary.\n");
                dtp_gen_abort(1);
            }
            memcpy(current_parpak->write_buffer,
                   tmp_buffer.dtp_file,
                   current_parpak->buffer_size);
            if(ctx->par_pack_bin_size < current_parpak->buffer_size){
                ctx->par_pack_bin_size = current_parpak->buffer_size;
            }
            tmp_buffer.dtp = tmp_buffer.dtp_file;
            tmp_buffer.dcc_size = 0;
            current_parpak = current_parpak->next;
        }
        ctx->current_usecase = ctx->current_usecase->next;
    }
    osal_free(tmp_buffer.dtp);
    tmp_buffer.dcc_size = 0;
    tmp_buffer.dtp = tmp_buffer.dtp_file = NULL;

    ctx->xml_header.sz_x_dcc_descriptor = ctx->par_pack_bin_size;
    ctx->xml_header.sz_uc_spec_gen_params = ctx->usecase_gen_bin_size;
    ctx->current_usecase = ctx->use_cases;
    while(ctx->current_usecase){
        FPRINTF_DEV(fout_txt, "===================================================================================\n");
        FPRINTF_DEV(fout_txt, "Use cases : 0x%X\n", ctx->current_usecase->mask);
        FPRINTF_DEV(fout_txt, "===================================================================================\n\n");

        ctx->current_usecase->start_in_dcc_file = file->dcc_size;
        //write_bin_general(file,ctx->current_usecase->use_case_parameter);
        if(ctx->current_usecase->use_case_parameter){
            FPRINTF_DEV(fout_txt, "Use cases parameters:\n\n");

            file->dcc_size += ctx->usecase_gen_bin_size;
            if(file->dcc_size > DTP_MAX_SIZE){
		        printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		        dtp_gen_abort(1);
                return;
            }
            memcpy(file->dtp,
                   ctx->current_usecase->write_buffer,
                   ctx->current_usecase->buffer_size);

            file->dtp+=ctx->current_usecase->buffer_size;
            if(ctx->current_usecase->buffer_size < ctx->usecase_gen_bin_size){
                memset(file->dtp,
                       0x00,
                       ( ctx->usecase_gen_bin_size - ctx->current_usecase->buffer_size) );
                file->dtp += ( ctx->usecase_gen_bin_size - ctx->current_usecase->buffer_size);
            }
            osal_free( ctx->current_usecase->write_buffer );
            ctx->current_usecase->write_buffer = NULL;
            ctx->current_usecase->buffer_size = 0;

            //printf("%ld count\n",ctx->current_usecase->photospace_count);

            //write_tree(NULL,ctx->current_usecase->use_case_parameter,1);
        }
        if(ctx->current_usecase->photospaces){
            //TODO: Write photospaces in bin file.
            phreg = ctx->current_usecase->photospaces;
            //write the number of dimensions.. it should be the same everywhere
            //and that should already be checked at parsing.
            file->p_ehdl.put_u32(file,phreg->limits_count);
            //write the dimension IDs in order. We should have already checked that
            //all the IDs are present and in each region.
            current_limit = phreg->limits;
            while( current_limit ){
                file->p_ehdl.put_u32(file,current_limit->id);
                current_limit = current_limit->next;
            }
            //write number of regions
            file->p_ehdl.put_u32(file,ctx->current_usecase->photospace_count);
            file->p_ehdl.put_u32(file,ctx->current_usecase->parpak_count);
            while (phreg) {

                //printf("%d limits\n",phreg->limits_count);
                FPRINTF_DEV(fout_txt, "region %s par_pak N %ld:\n", phreg->name, phreg->par_pack);

                file->p_ehdl.put_u32(file,phreg->par_pack);
                current_limit = phreg->limits;
                while (current_limit){
                    FPRINTF_DEV(fout_txt, "%s : %s=%ld %ld..%ld\n",
                                                        current_limit->name,
                                                        XML_NAME_ID_TYPE,
                                                        current_limit->id,
                                                        current_limit->start,
                                                        current_limit->end);
                    file->p_ehdl.put_u32(file,current_limit->start);
                    current_limit = current_limit->next;
                }
                current_limit = phreg->limits;
                while (current_limit){
                    FPRINTF_DEV(fout_txt, "%s : %s=%ld %ld..%ld\n",
                                                        current_limit->name,
                                                        XML_NAME_ID_TYPE,
                                                        current_limit->id,
                                                        current_limit->start,
                                                        current_limit->end);
                    file->p_ehdl.put_u32(file,current_limit->end);
                    current_limit = current_limit->next;
                }

                phreg = phreg->next;
            }
        }
        else{
            file->p_ehdl.put_u32(file,0); //no photospace
            file->p_ehdl.put_u32(file,0); //no limits
            file->p_ehdl.put_u32(file,0); //no parpacks
        }
        //TODO: write_photospaces
        FPRINTF_DEV(fout_txt, "---- par packs ------------\n");
        if(ctx->current_usecase->par_pack){
            struct par_package_t* pp;
            int counter = 0;

            pp = ctx->current_usecase->par_pack;
            while (pp){
                FPRINTF_DEV(fout_txt, "par pack (class) N %d\n", counter);

                file->dcc_size += ctx->par_pack_bin_size;
                if(file->dcc_size > DTP_MAX_SIZE){
		            printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
		            dtp_gen_abort(1);
                    return;
                }
                memcpy(file->dtp,pp->write_buffer,pp->buffer_size);
                file->dtp+=pp->buffer_size;
                if(pp->buffer_size < ctx->par_pack_bin_size){
                    memset(file->dtp,
                           0x00,
                           ( ctx->par_pack_bin_size - pp->buffer_size) );
                    file->dtp += (ctx->par_pack_bin_size - pp->buffer_size);
                }
                osal_free( pp->write_buffer );
                pp->write_buffer = NULL;
                pp->buffer_size = 0;

                //write_tree(NULL,pp->package,0);
                pp = pp->next;
                counter ++;
            }
        }

        ctx->current_usecase->size_in_dcc_file =
                   file->dcc_size - ctx->current_usecase->start_in_dcc_file;
        ctx->current_usecase = ctx->current_usecase->next;
    }
}
#endif

void write_usecase_table(dtp_file_t* file, dtp_gen_ctx_t* ctx)
{
    if( !ctx->use_cases ){
        return;
    }
    ctx->current_usecase = ctx->use_cases;
    while(ctx->current_usecase){
        file->p_ehdl.put_u32(file,ctx->current_usecase->mask);
        file->p_ehdl.put_u32(file,ctx->current_usecase->start_in_dcc_file);
        file->p_ehdl.put_u32(file,ctx->current_usecase->size_in_dcc_file);
        ctx->current_usecase = ctx->current_usecase->next;
    }

}

void write_header_txt ( dcc_component_header_type* xml_header  ){
    FPRINTF_DEV(fout_txt, "===========================\nHEADER INFORMATION\n===========================\n" );
    FPRINTF_DEV(fout_txt, "camera_module_id: %ld\n", xml_header->camera_module_id );
    FPRINTF_DEV(fout_txt, "dcc_descriptor_id: %ld\n", xml_header->dcc_descriptor_id );
    FPRINTF_DEV(fout_txt, "algorithm_vendor_id: %ld\n", xml_header->algorithm_vendor_id );
    FPRINTF_DEV(fout_txt, "dcc_tuning_tool_version: %ld\n", xml_header->dcc_tuning_tool_version );
    FPRINTF_DEV(fout_txt, "dcc_profile_time_stamp: %ld\n", xml_header->dcc_profile_time_stamp );
    FPRINTF_DEV(fout_txt, "crc_checksum: %lX\n", xml_header->crc_checksum );
    FPRINTF_DEV(fout_txt, "dcc_reserved_0: %ld\n", xml_header->dcc_reserved_0 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_1: %ld\n", xml_header->dcc_reserved_1 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_2: %ld\n", xml_header->dcc_reserved_2 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_3: %ld\n", xml_header->dcc_reserved_3 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_4: %ld\n", xml_header->dcc_reserved_4 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_5: %ld\n", xml_header->dcc_reserved_5 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_6: %ld\n", xml_header->dcc_reserved_6 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_7: %ld\n", xml_header->dcc_reserved_7 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_8: %ld\n", xml_header->dcc_reserved_8 );
    FPRINTF_DEV(fout_txt, "dcc_reserved_9: %ld\n", xml_header->dcc_reserved_9 );
    FPRINTF_DEV(fout_txt, "sz_comp_spec_gen_params: %ld\n", xml_header->sz_comp_spec_gen_params );
    FPRINTF_DEV(fout_txt, "sz_uc_spec_gen_params: %ld\n", xml_header->sz_uc_spec_gen_params );
    FPRINTF_DEV(fout_txt, "sz_x_dcc_descriptor: %ld\n", xml_header->sz_x_dcc_descriptor );
    FPRINTF_DEV(fout_txt, "total_file_sz: %ld\n", xml_header->total_file_sz );
    FPRINTF_DEV(fout_txt, "===========================\n");
}

int write_dtp(dtp_gen_ctx_t* ctx)
{
    dtp_file_t   file;
    char out_fname[MAX_FILENAME_LENGTH];

#ifdef DCCREAD
    char in_fname[MAX_FILENAME_LENGTH];
#else
    uint32 sys_params_ofst;
#endif

#ifdef DEVEL

    strcpy (out_fname, ctx->xml_fname);
    change_file_ext(out_fname, ".txt");

    fout_txt = fopen(out_fname,"wt");
    if(fout_txt == NULL) {
        printf("Can not open output file %s\n", out_fname);
        return (-1);
    }
#endif

    //Fill the DCC binary buffer with 0. This will guarantee the padding of
    //usecase datas and parpacks will be with 0s.
    memset(&file, 0, sizeof(dtp_file_t));

#ifndef DCCREAD

    file.dtp_file = (uint8 *) osal_malloc(DTP_MAX_SIZE);
    if (file.dtp_file == NULL) {
        printf("ERROR: can not allocate memory for DCC binary\n");
        return(-1);
    }
    memset(file.dtp_file, 0, DTP_MAX_SIZE);
    file.dtp = file.dtp_file;
    file.dcc_size = 0;

    file.p_ehdl.put_u32 = put_u32_little_endian;
    file.p_ehdl.put_u16 = put_u16_little_endian;
    file.p_ehdl.put_u8  = put_u8;

    //calc system parameters offset - they start after heade, UCcount and US tabler
    sys_params_ofst = (sizeof(dcc_component_header_type) + sizeof(uint32) +
                    ctx->use_case_count * 3 * sizeof(uint32) ) ;
    //skip space for header writing:
    file.dtp += sys_params_ofst ;
    file.dcc_size += sys_params_ofst;
    if(file.dcc_size > DTP_MAX_SIZE){
        printf("ERROR: DCC binary file appers larger than %ld bytes\n",(uint32)DTP_MAX_SIZE);
        fclose(fout_txt);
        return (-1);
    }

#else //DCCREAD
    strcpy (out_fname, ctx->xml_fname);
    change_file_ext(out_fname, "_read.txt");

    fout_txt = fopen(out_fname,"wt");
    if(fout_txt == NULL) {
        printf("Can not open output file %s\n", out_fname);
        return (-1);
    }

    strcpy (in_fname, ctx->xml_fname);
    change_file_ext(in_fname, ".bin");
    file.dtp_file = read_dcc(in_fname,&(file.dcc_size));
    if(!file.dtp_file){
        fclose(fout_txt);
        return (-1);
    }


    file.dtp = file.dtp_file;
    file.dcc_size = 0;
    file.g_ehdl.get_u32 = get_uint32;
    file.g_ehdl.get_u16 = get_uint16;
    file.g_ehdl.get_u8  = get_uint8;

    fill_header_data( &(dccinfo.dcc_header),&file);
    if(ctx->xml_header.crc_checksum != dccinfo.dcc_header.crc_checksum) {
        printf("CRC checksum missmatch: XML: %lX != BIN: %lX",
                                                ctx->xml_header.crc_checksum,
                                                dccinfo.dcc_header.crc_checksum );
        printf("CRC mismatch means the structure in XML file differs that the stucture in the binary\n");
        fclose(fout_txt);
        osal_free(file.dtp_file);
        return (-1);
    }

    dccinfo.usecase_count = file.g_ehdl.get_u32(&file);
    dccinfo.parpak_count = (uint32*) osal_malloc(dccinfo.usecase_count * sizeof(uint32));

    //point to start of system parameters
    //start+ size of binary header + UC count + size of UC table
    file.dtp = file.dtp_file + 84 + dccinfo.usecase_count*12;

#endif //DCCREAD

    //Versions and other DCC header data is
    //written as defines in the header file
    //write general data tree
    FPRINTF_DEV(fout_txt, "===========================================\n");
    FPRINTF_DEV(fout_txt, "System parameters structure and values\n");
    FPRINTF_DEV(fout_txt, " for use case : \n");
    FPRINTF_DEV(fout_txt, "%s\n", USE_CASE_NAMES[0]);
    FPRINTF_DEV(fout_txt, "===========================================\n");

    //Write or read system_parameters to/from bin depending on the current build
    write_tree(&file,ctx->tree_gen,0 );
#ifndef DCCREAD
    //calc size of system_params
    ctx->xml_header.sz_comp_spec_gen_params = file.dcc_size - sys_params_ofst;
#endif

    FPRINTF_DEV(fout_txt, "===========================================\n");
    FPRINTF_DEV(fout_txt, "Usecase structure and values\n");
    FPRINTF_DEV(fout_txt, " for use case : \n");
    FPRINTF_DEV(fout_txt, "%s\n", USE_CASE_NAMES[0]);
    FPRINTF_DEV(fout_txt, "===========================================\n");
    //Write or read usecase_parameters to/from bin depending on the current build
    write_usecase(&file, ctx);

#ifndef DCCREAD
    //reset the position to the start in order to write the header and
    //table of contents
    file.dtp = file.dtp_file;
    write_bin_header(&file, ctx);
    write_header_txt(&(ctx->xml_header));
    file.p_ehdl.put_u32(&file,ctx->use_case_count);
    write_usecase_table(&file, ctx);

    //file.dcc_size is updated at every write. It is used to check for the
    //maximum DCC size reached. However the header, UC table and UC count
    //should be written last - after determining the start offset of each UC.
    //That is why file.dcc_size is initialized to the start of sys_params and
    //then the data are written. After all header, UCtable and UCcount are
    //written and increment file.dcc_size again, so file.dcc_size is not
    //relevant at this point (the lines after this comment)
    file.dcc_size = ctx->xml_header.total_file_sz;
#endif

#ifdef DEVEL
    fclose(fout_txt);
#else
    #ifdef DCCREAD
        osal_free(dccinfo.parpak_count);
        osal_free(file.dtp_file);
        write_header_txt(&(dccinfo.dcc_header));
        fclose(fout_txt);
    #endif
#endif

#ifndef DCCREAD

    //find last delimiter the file name satrts after it
    if(strcmp(ctx->xml_fdir,STR_NOT_PRESENT) != 0)
    {
        char* last;
        last = strrchr(ctx->xml_fname, DIR_DELIMITER);
        last++;
        sprintf (out_fname,"%scid%d_%s", ctx->xml_fdir, ctx->xml_header.camera_module_id, last);
    } else {
        sprintf (out_fname,"cid%d_%s", ctx->xml_header.camera_module_id, ctx->xml_fname);
    }

    change_file_ext(out_fname, ".bin");

    if( dtp_write(&file, out_fname) ){
        return (-1);
    }
#endif
    return 0;
}
