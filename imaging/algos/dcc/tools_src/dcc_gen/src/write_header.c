/*****************************************************************
 * tools.c
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>


/* ========================================================================= */
/*                    variables                                              */
/* ========================================================================= */
FILE* fout_txt;
static char type_name[TYPE_FIELD_LENGTH];

//3 dims x (10characters + []) + terminating 0 + 5 just in case
static char arr[3*(10+2)+1+5];

extern const char* BASIC_TYPES_NAMES[8];

void type_to_string_in_h(struct tree_node_t *  elem,char * name, char * arr)
{

	char elements_count[11];
    uint8 arr_dims = 0;
    uint32 dim_l;
    uint32 dim_m;
    uint32 dim_r;
//	uint8 temp_arr_dims = 0;

	elements_count[0] = '\0';
	strcpy(name,"ERROR: UNKNOWN NAME");
	arr_dims = (uint8)((elem->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);
    dim_l = elem->arr_dim_l;
    dim_m = elem->arr_dim_m;
    dim_r = elem->arr_dim_r;

    if(arr_dims) {
        /*
        if ( (dim_l == 0) && (elem->next) ) {
            printf("Error: flexible array member \"%s.%s\" not at end of structure\n",elem->parrent->name,elem->name);
            dtp_gen_abort(1);
        }
        */
        if( (arr_dims == 2) && (dim_m == 0) ) {
            printf("Error: in flexible array member \"%s.%s\" can only have flexible number of elements for primary array index.\n",elem->parrent->name,elem->name);
            dtp_gen_abort(1);
        }
        if( (arr_dims == 3) && ((dim_r == 0) || (dim_m == 0)) ) {
            printf("Error: in flexible array member \"%s.%s\" can only have flexible number of elements for primary array index.\n",elem->parrent->name,elem->name);
            dtp_gen_abort(1);
        }
    }

	/* When we have structure with known arr_dims and sizes, the actual structure is presented in following way in the tree
	 Example: test[2][3]

			test
			[2]
			/	\
		test[3] test[3]
	   /|\     /|\

	 Therefore we must go down the tree and check whether the name is the same as the root node,
	 whether it is an array and get its elements count
    */

/*
this code is buggy - why did it exist at all ???

	if (((elem->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT) && dim_l )
	{

		if (elem->child)
		{
			if (strcmp(elem->name,elem->child->name) == 0)	// checking if the name of the child is the same as the root element
			{
				temp_arr_dims = (uint8)((elem->child->type & BASIC_TYPE_ARRAY)
                                        >> BASIC_TYPE_ARRAY_DIMS_OFST);	// checking whether the child is an array
				if (temp_arr_dims)
				{
					++arr_dims;																// increasing the dimensions of the root element
					dim_m = elem->child->arr_dim_l;		// setting second dimension counter = elements of the child
				}
			}
			if (elem->child->child)
			{
				if (strcmp(elem->name,elem->child->child->name) == 0)
				{
					temp_arr_dims = (uint8)((elem->child->child->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);
					if (temp_arr_dims)
					{
						++arr_dims;
						dim_r = elem->child->child->arr_dim_l;
					}
				}
			}
		}
	}
*/

	if ((elem->type & BASIC_TYPE_MASK) > (sizeof(BASIC_TYPES_NAMES) / sizeof(BASIC_TYPES_NAMES[0])))
	{
		printf("ERROR: Could not resolve the string from type");
		return;
	}

    if ((elem->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
    {
        strcpy(name, elem->name);
    } else {
        strcpy(name, BASIC_TYPES_NAMES[elem->type & BASIC_TYPE_MASK]);
    }

	strcpy(arr,"");

    //if this type is an arry
    //array dimension 3
    if (arr_dims == 3) {
		if (dim_l){
			sprintf(elements_count,"%d",dim_l);
            strcat(arr,"[");
            strcat(arr,elements_count);
            strcat(arr,"]");
		}
        if (dim_m){
			sprintf(elements_count,"%d",dim_m);
        }
		strcat(arr,"[");
		strcat(arr,elements_count);
		strcat(arr,"]");
        if (dim_r){
			sprintf(elements_count,"%d",dim_r);
        }
		strcat(arr,"[");
		strcat(arr,elements_count);
		strcat(arr,"]");
    }
    //array dimension 2
    else if (arr_dims == 2) {
		if (dim_l){
			sprintf(elements_count,"%d",dim_l);
            strcat(arr,"[");
            strcat(arr,elements_count);
            strcat(arr,"]");
		}
		if (dim_m)
			sprintf(elements_count,"%d",dim_m);
		strcat(arr,"[");
		strcat(arr,elements_count);
		strcat(arr,"]");
    }
    //array dimension 1
    else if (arr_dims == 1) {
		if (dim_l){
			sprintf(elements_count,"%d",dim_l);
            strcat(arr,"[");
            strcat(arr,elements_count);
            strcat(arr,"]");
		}
	}
}

int write_content(struct tree_node_t * header, int indent)
{

    char *name;
    char temp[NAME_FIELD_LENGTH];
	struct tree_node_t * srtruct_field;

    type_to_string_in_h(header,(char*)&type_name,(char*)&arr);

    if(strcmp(type_name, "struct") == 0)
    {
        //this is a struct of NOT-predefined type
    	write_indent(fout_txt, indent);
	    fprintf(fout_txt, "struct {\n");
	    srtruct_field = header->child;
	    while (srtruct_field)
	    {
		    write_content(srtruct_field, indent+1);
		    srtruct_field = srtruct_field->next;
	    }
        write_indent(fout_txt, indent);
	    fprintf(fout_txt,"} %s;",(char*)header->val);
	    fprintf(fout_txt, "\n");
        return 0;
    }

    if ((header->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
    {
        name = (char*)header->val;

    } else {
	    name = header->name;
    }

    if(header->type & BASIC_TYPE_ARRAY_UNDEF){
        //If undefined array it have to be written in the H file like:
        //uint16 (*arr_name)[2];
        //instead of
        //uint16 arr_name[][2];
        //Otherwize C compiler reports error : "undefined size array not in the end of structure"
        strcpy(temp,name);
        fprintf(fout_txt, "    uint32 %s%s;\n",name,UNDEF_ARRAY_SIZE_POSTFIX);
        write_indent(fout_txt, indent);
        fprintf(fout_txt, "%s (*%s)%s;\n",type_name,name,arr);
    } else {
	    write_indent(fout_txt, indent);
        fprintf(fout_txt, "%s %s%s;\n",type_name,name,arr);
    }

	return 0;
}

int write_header(struct tree_node_t * header,int indent)
{
	struct tree_node_t * current = NULL;
	if (( header->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT )
	{
		fprintf(fout_txt, "typedef struct {\n");
		current = header->child;
		while (current)
		{
			write_content(current, 1);
			current = current->next;
		}
		fprintf(fout_txt,"} %s;",(char*)header->val);
		fprintf(fout_txt, "\n\n");
	}
	else
	{
		printf("Error while composing H file\n");
		return -1;
	}
	return 0;
}
/* ============================================================================
* write_header_file()
*
* Writes DCC values and structure in a text file
* ========================================================================== */
int write_header_file(dtp_gen_ctx_t* dtp_gen_ctx)
{
    char* out_fname;
	struct known_types_t* node;
    dtp_gen_ctx_t* ctx = dtp_gen_ctx;
    char void_t[] = "void";
//    char* sys_gen_t = NULL;
//    char* uc_gen_t = NULL;
//    char* ppclass_t = NULL;
    struct tree_node_t* uc_gen = NULL;
    struct tree_node_t* ppclass = NULL;
    char* dcc_descriptor_id_type;
    char dcc_descriptor_id_type_unckown[] = "UNKNOWN";

    dcc_descriptor_id_type = ((ctx->xml_header.dcc_descriptor_id) >= (sizeof(dcc_id_names)/sizeof(dcc_id_names[0]))) ?
                            dcc_descriptor_id_type_unckown :
                            (char*)dcc_id_names[ctx->xml_header.dcc_descriptor_id];

    //allocate memory for name, path, extension and terminating 0
    out_fname = malloc(1 + 4 + strlen(ctx->dcc_name) + strlen(ctx->xml_fdir));
    if(out_fname == NULL){
        printf("Can allocate memory\n");
        return (-1);
    }
    out_fname[0] = 0;
    if(strcmp(ctx->xml_fdir,STR_NOT_PRESENT) != 0){
        //path to XML is present in DCCgen arguments
        strcpy (out_fname, ctx->xml_fdir);
    }
    strcat (out_fname, ctx->dcc_name);
    change_file_ext(out_fname, ".h");

    fout_txt = fopen(out_fname,"wt");

    if(fout_txt == NULL) {
        printf("Can not open output file %s\n", out_fname);
        free(out_fname);
        return (-1);
    }

    fprintf(fout_txt,"/******************************************************************************\n");
    fprintf(fout_txt,"Automatically generated header for DCC data\n");
    fprintf(fout_txt,"Generated by DCC generator ver %d.%d.%d\n", DCC_GEN_VERSION/10000, (DCC_GEN_VERSION/100)%100, DCC_GEN_VERSION%100 );
    fprintf(fout_txt,"for dcc_descriptor_id_type = %s\n", dcc_descriptor_id_type);
    fprintf(fout_txt,"vendor ID %ld\n", ctx->xml_header.algorithm_vendor_id);
    fprintf(fout_txt,"******************************************************************************/\n");
    fprintf(fout_txt,"\n");

	to_upper_without_ext(out_fname);
	fprintf(fout_txt,"#ifndef __%s_H__ \n#define __%s_H__\n\n",out_fname,out_fname);
	fprintf(fout_txt,"#include \"%s\"\n\n",
        (ctx->fw2dcc_fname)?ctx->fw2dcc_fname:EXTERNAL_CTYPES_INCLUDE);
//	fprintf(fout_txt,"#define CAMERA\t\t%d\n", ctx->xml_header.camera_module_id);
//	fprintf(fout_txt,"#define ALGO\t\t%d\n", ctx->xml_header.dcc_descriptor_id);
//	fprintf(fout_txt,"#define VENDOR\t\t%d\n", ctx->xml_header.algorithm_vendor_id);
//	fprintf(fout_txt,"#define TUNNING\t\t%d\n\n", ctx->xml_header.tuning_tool_version);

    free(out_fname);

	node = ctx->known_types;
	while (node)
	{
		if (node->type_tree)
			write_header (node->type_tree, 0);
		else
		{
			printf("Error: Known type with unknown type tree\n");
			fclose(fout_txt);
			dtp_gen_abort(1);
		}
		node = node->next;

	}

    if(ctx->use_cases)
    {
        struct use_case_t*          use_case_cur;
        use_case_cur = ctx->use_cases;
        while (use_case_cur && ((uc_gen==NULL)||(ppclass==NULL)) )
        {
            if (use_case_cur->use_case_parameter){
                uc_gen = use_case_cur->use_case_parameter;
            }
            if(use_case_cur->par_pack){
                struct par_package_t* pp;
                pp = use_case_cur->par_pack;
                while (pp){
                    if(pp->package == NULL) {
                        printf ("!!! parpack node without content !!!\n");
                        return (-1);
                    }
                    ppclass = pp->package;
                    pp = pp->next;
                }
            }
            use_case_cur = use_case_cur->next;
        }
    }
/*
    //determine the type names of DCC data. Some of the data may be missing.
    //Such data is denoted as void in dcc_bin_parse() and is not filled in it.
    if(ctx->tree_gen){
        sys_gen_t = ctx->tree_gen->name;
    } else {
        sys_gen_t = void_t;
    }
    if(uc_gen){
        uc_gen_t = uc_gen->name;
    } else {
        uc_gen_t = void_t;
    }
    if(ppclass){
        ppclass_t = ppclass->name;
    } else {
        ppclass_t = void_t;
    }
*/

    fprintf(fout_txt,"int %s_dcc_bin_parse(uint8* b_sys_prm, uint8* b_uc_prm, uint8* b_parpack,\n",ctx->dcc_name);
//    fprintf(fout_txt,"                  %s* sys_prm, %s* uc_prm, %s* parpack,\n", sys_gen_t, uc_gen_t, ppclass_t);
    fprintf(fout_txt,"                  void* sys_prm, void* uc_prm, void* parpack,\n");
    fprintf(fout_txt,"                  uint32 crc);\n");

//    fprintf(fout_txt,"void %s_dcc_bin_free(%s* sys_prm, %s* uc_prm, %s* parpack);\n\n",ctx->dcc_name, sys_gen_t, uc_gen_t, ppclass_t);
    fprintf(fout_txt,"void %s_dcc_bin_free(void* sys_prm, void* uc_prm, void* parpack);\n\n",ctx->dcc_name);

	fprintf(fout_txt,"#endif\n\n");

    fclose(fout_txt);
    return 0;
}
