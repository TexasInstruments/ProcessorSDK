/*****************************************************************
 * tools.c
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"

#ifdef MEMORY_LEAK_DEBUG
#define MAX_DEB_ARR_LEN (3000)
int mem_deb_arr[MAX_DEB_ARR_LEN][2];
//char mem_deb_arr_file[MAX_DEB_ARR_LEN][128];
//uint16 mem_deb_arr_line[MAX_DEB_ARR_LEN];
int debug_array_overflow = 0;
int alloc_calls = 0;
int free_calls = 0;
int first_call = 1;
int total_alloc=0;
int total_free=0;

#endif

const char* BASIC_TYPES_NAMES[BASIC_TYPE_COUNT] = {
	""			,
    "uint8"     ,
    "int8"      ,
    "uint16"    ,
    "int16"     ,
    "uint32"    ,
    "int32"     ,
    "struct"
};
const char* dcc_id_names[DCC_ID_COUNT] = {
    "DCC_ID_ISIF_CSC",                    
    "DCC_ID_ISIF_BLACK_CLAMP",            
    "DCC_ID_LSC",                         
    "DCC_ID_H3A_AEWB_CFG",                
    "DCC_ID_IPIPE_DPC_OTF",               
    "DCC_ID_IPIPE_NOISE_FILTER_1",        
    "DCC_ID_IPIPE_NOISE_FILTER_2",        
    "DCC_ID_IPIPE_NOISE_FILTER_LDC",      
    "DCC_ID_IPIPE_GIC",                   
    "DCC_ID_IPIPE_CFA",                   
    "DCC_ID_IPIPE_RGB_RGB_1",             
    "DCC_ID_IPIPE_GAMMA",                 
    "DCC_ID_IPIPE_RGB_RGB_2",             
    "DCC_ID_IPIPE_3D_LUT",                
    "DCC_ID_IPIPE_GBCE",                  
    "DCC_ID_IPIPE_RGB_TO_YUV",            
    "DCC_ID_IPIPE_EE",                    
    "DCC_ID_IPIPE_CAR",                   
    "DCC_ID_IPIPE_CGS",                   
    "DCC_ID_IPIPE_YUV444_YUV422",         
    "DCC_ID_IPIPE_RSZ",                   
    "DCC_ID_NSF_CFG",                     
    "DCC_ID_LDC_ODC",                     
    "DCC_ID_LDC_CAC",                     
    "DCC_ID_LBCE_1",                      
    "DCC_ID_ADJUST_RGB2RGB",              
    "DCC_ID_VNF_CFG", 					
    "DCC_ID_AAA_ALG_AE",                  
    "DCC_ID_AAA_ALG_AWB",                 
    "DCC_ID_AAA_ALG_AF_HLLC",             
    "DCC_ID_AAA_ALG_AF_AFFW",             
    "DCC_ID_AAA_ALG_AF_SAF",              
    "DCC_ID_AAA_ALG_AF_CAF",              
    "DCC_ID_ISS_SCENE_MODES",             
    "DCC_ID_ISS_EFFECT_MODES",            
    "DCC_ID_ISS_GBCE1",                   
    "DCC_ID_ISS_GBCE2",                   
    "DCC_ID_IPIPE_DPC_LUT",               
    "DCC_ID_3D_MMAC_SAC",                 
    "DCC_ID_IPIPE_LSC",                   
    "DCC_ID_AAA_ALG_AWB3"                
};

/* ========================================================================= */
/*                    DEFINES                                                */
/* ========================================================================= */
char WHITE_SPACES_STR[] = {
    ' ', // space
    '\t', // tab
    ';',
    0x0D, // carrage return
    0x0A, // new line
    0 //terminating 0
};
/* ========================================================================= */
char NOT_ALLOWED_CHARS_STR[] = {
    //characters not allowed in file name
    '*',
    '?',
    '"',
    '<',
    '>',
    '|',
    0 //terminating 0
};
char NAME_END_STR[] = {
    '\t', // tab
    0x0D, // carrage return
    0x0A, // new line
    0 //terminating 0
};
/* ========================================================================= */
/*						variables											 */
/* ========================================================================= */


/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */
#define is_white_space(a)  strchr(WHITE_SPACES_STR,(a))
#define is_not_alllowed_char(a) strchr(NOT_ALLOWED_CHARS_STR,(a))
#define is_name_end_char(a) strchr(NAME_END_STR,(a))
int read_file_in_data_str(dtp_gen_ctx_t* ctx, char* file_name, char* ins_location);


/* ============================================================================
* tls_clear_starting_white_spaces()
*
* clears starting white spaces in a non-NUll-terminated string, uses the
* characters defined in WHITE_SPACES_STR as separators between tokens.
*
* params
*   str : pointer to non-NUll-terminated string
*   len : pointer number of characters in the string
* return
*   *str : start of first token in the string
*   *len : number of chars in the remaining srting
* ========================================================================== */
void tls_clear_starting_white_spaces(char** str, int* len)
{
    int i;

    for(i=0; i < *len; i++){
        if( ! is_white_space((*str)[i]) ){
            break;
        }
    }
    *str = &((*str)[i]);
    *len = *len - i;
}

void tls_clear_ending_white_space(char** str, int* len)
{
    int i=0;

    for(i= *len  ; i > 0 ; i--){
        if( ! is_white_space((*str)[i-1]) ){
            break;
        }
    }

    if(i<*len){
        memset(*str+i,0,*len-i);
        *len = i;
    } else return;
}
/* ============================================================================
* tls_prepare_data_str()
*
* Prepare data string for processing:
* - cleans the white space characters
* - remove comments
* - removes ',' in ",}" sequences
* - removes trailing ',' in the end of string
* - includes external files in the string
*
* params
*   ctx : pointer to DCC generator context vars
* ========================================================================== */
int tls_prepare_data_str(dtp_gen_ctx_t* ctx)
{
    //TODO: check for numbers/names separated by white spaces. Do not merge them ! Insert ',' instead
    char* str = ctx->data_str;
    char* p = ctx->data_str;
    int file_included;
    int in_quotes;

    //clear comments
    str = ctx->data_str;
    p = ctx->data_str;
    while (*str) {
        if(*str == '/'){
            if(*(str+1) == '/'){
                // "//" comment
                while (*str && (*str != 0x0D) && (*str != 0x0A)){
            str++;
                }
            } else if (*(str+1) == '*'){
                //clear comments like /* .. */
                str++;
                while (*str && ((*str != '*') || (*(str+1) != '/'))){
                        str++;
                    }
                if (*str) str+= 2;//skip '*/'
                }
                }
                // Clear comments like "//"
        if(*str){
            *p++ = *str++;
        }
    }
    *p = 0;

    //clear starting white spaces
    str = ctx->data_str;
    while ((*str) && is_white_space(*str)) {
        str++;
    }
    //convert white spaces to commas - white spaces are considered separators. Leave the white spaces within quotes.
    p = ctx->data_str;
    in_quotes = 0;
    while (*str){
        if(*str == '"'){
            in_quotes = ! in_quotes;
        }
        //clear white spaces
        if (is_white_space(*str) && !in_quotes){
            *p = ',';
        } else {
            *p = *str;
        }
        p++;
        str++;
    }
    *p = 0;//terminating 0

    //clear double commas
    str = ctx->data_str;
    p = ctx->data_str;
    while (*str) {
        *p = *str;
        str++;
        if(*p == ','){
             while (*str == ',') str++;
        };
        p++;
    }
    *p = 0;//terminating 0

    //fix "... ,} ..."
    p = ctx->data_str;
    str = ctx->data_str;
    while (*str) {
        if((*str == ',') && (*(str+1) == '}')){
            //if ",}" increment str to point to '}', thus ',' will be omitted
                str++;
            }
        *p = *str;
        if((*str == '{') && (*(str+1) == ',')){
            //if "{," increment str to point to ',', thus ',' will be omitted
                str++;
        }
        str++;
        p++;
    }

    //remove triling commas at the end of string
    p--;
    while ( (*p == ',') && ((uint32)p > (uint32)ctx->data_str) ){
        p--;
    }
    p++;

    *p = 0;//terminating 0

    ctx->data_str_len = (uint32)p - (uint32)ctx->data_str + 1;

    //search for including files
    file_included = 0;
    do {
        char file_name[MAX_FILENAME_LENGTH];
        char full_file_name[MAX_FILENAME_LENGTH];
        uint32 i;
        p = strstr(ctx->data_str, XML_INCLUDE_FILE_STRING);
        if(p == NULL){
            break;
        }
        //There is #include in the XML - a file should be inserted in place of "#include <file_name>"
        file_included = 1;
        //fill "#include" with ' ' to be cleared later
        for(i=0; i< strlen(XML_INCLUDE_FILE_STRING); i++){
            *p++ = ' ';
        }
        if(*p == ','){
            //if there were white spaces between #include and file name, they
            //will appear as 1 ',' here : skip it
            p++;
        }
        if(*p != '"'){
            printf("ERROR: File name in\"\" must follow %s\n",XML_INCLUDE_FILE_STRING);
            return (-1);
        }
        //included file found
        *p++ = ' '; //replace opening " with space
        i=0;
        while ((*p != '"') && (*p) && (i<MAX_FILENAME_LENGTH-1))
        {
            if( is_not_alllowed_char(*p) || (*p < 0x20) || (*p > 0x7E) ){
                printf("ERROR: included file name containes no-allowed character: '%c', code %0xX\n", *p, *p);
                return (-1);
            }
            file_name[i] = *p; //save file name
            i++;
            *p++ = ' '; //replace file name with spaces
        }
        file_name[i] = '\0';
        if((i == 0) || (*p == 0) ){
            //no characters between quotation marks or data strings ends before
            //2nd '"' is met
            printf("ERROR: File name in\"\" must follow %s\n",XML_INCLUDE_FILE_STRING);
            return (-1);
        }
        *p++ = ' '; //replace closing " with space
        //check if the file name contains full path
        //If Yes : use it as it is
        //If no : consider it is just file name of relative path (relative to
        //   XML). Insert the path to XML before the include path. If there is
        //   no path to XML, then dcc_gen is called from the folder the XML is
        //   in, then assume included files are in same folder and use the file
        //   name as it is.
        if(
            (strcmp(ctx->xml_fdir,STR_NOT_PRESENT) == 0)
#ifdef __LINUX__
            || (file_name[0] == DIR_DELIMITER)
#else
            || (file_name[1] == ':')
#endif
        ){
            if( read_file_in_data_str(ctx, file_name, p) ){
                return (-1);
            }
        } else {
            //take the included file from the directory where the xml is
            sprintf(full_file_name,"%s%s",ctx->xml_fdir,file_name);
            if( read_file_in_data_str(ctx, full_file_name, p) ){
                return (-1);
            }
        }
    } while (p);

    if (file_included) {
        //call recursively tls_prepare_data_str() to process the data just
        //included from file. IN the files itself, an "#include <file_name>"
        //may exist.
        tls_prepare_data_str(ctx);
    }

    return 0;

}

void type_to_string(uint16 type, uint32 dim_l, uint32 dim_m, uint32 dim_r, char* name)
{
	char * pstr;
	char elements_count[11];
	int strptr;
    uint8 arr_dims;

	strcpy(name,"ERROR: UNKNOWN NAME");
    arr_dims = (uint8)((type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);

    if(arr_dims > 3){
        arr_dims = 3;
    }
    if(arr_dims < 3){
        dim_r = 0;
    }
    if(arr_dims < 2){
        dim_m = 0;
    }
    if(arr_dims < 1){
        dim_l = 0;
    }

	if ((type & BASIC_TYPE_MASK) > (sizeof(BASIC_TYPES_NAMES) / sizeof(BASIC_TYPES_NAMES[0])))
	{
		printf("ERROR: Could not resolve the string from type");
		return;
	}

    strcpy(name, BASIC_TYPES_NAMES[type & BASIC_TYPE_MASK]);

	strptr = (int)name[strlen(name) - 1];
    pstr = (char*)strptr;

    //if this type is an arry
    //array dimension 1
    if (arr_dims >= 1) {
		sprintf(elements_count,"%d",dim_l);
		strcat(name,"[");
		strcat(name,elements_count);
		strcat(name,"]");
	}
    //array dimension 2
    if (arr_dims >= 2) {
		sprintf(elements_count,"%d",dim_m);
		strcat(name,"[");
		strcat(name,elements_count);
		strcat(name,"]");
    }
    //array dimension 3
    if (arr_dims >= 3) {
        sprintf(elements_count,"%d",dim_r);
		strcat(name,"[");
		strcat(name,elements_count);
		strcat(name,"]");
    }
}


/* ============================================================================
* copy_value()
*
* Coppies the value of src tree in the value of dst tree
*
* params
*   dst : node, where to copy
*   src : root of the tree which value to be coppied
* ========================================================================== */
int copy_value (struct tree_node_t* dst, const struct tree_node_t* src)
{
    if(dst->val) {
        printf("ERROR: copy_value() : destination already has a value\n");
	    return (-1);
        //the dst node already has a value
    }
    //copy value
    if(src->val){
        uint32 val_size;
        if((src->type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT){
            uint32 dim_size;
            //If there is a value, then this node is a single type or an array of
            //a single type
            //sizes of not-used dimensions in 2D and 3D arrays must be 0 in
            //src->type. this is done in parse_type_array()
            val_size = sizeof(uint32);
            dim_size = (uint32)(src->arr_dim_l);
            if(dim_size != 0) {
                val_size *= dim_size;
            };
            dim_size = (uint32)(src->arr_dim_m);
            if(dim_size != 0) {
                val_size *= dim_size;
            };
            dim_size = (uint32)(src->arr_dim_r);
            if(dim_size != 0) {
                val_size *= dim_size;
            };
        } else {
            //if 'src' is a struct, its value contains the name of the field
            //because 'name' contains the type of the field if it is predefined.
            val_size = strlen((char*)src->val)+1;
        }

	    dst->val = osal_malloc(val_size);
	    if (!dst->val)
	    {
		    printf("ERROR: Can not allocate memory for dst->val\n");
		    return (-1);
	    }
	    memcpy(dst->val, src->val, val_size);
    }
    return 0;
}

/* ============================================================================
* tree_copy ()
*
* Coppies existing tree (src_tree) at the destination tree node (dst_node).
* The root of src_tree is coppied on dst_node, i.e.
*
* params
*   dst : node, where to copy the root of the tree
*   src : root of the tree to be coppied
* ========================================================================== */
int tree_copy(  struct tree_node_t**    dst,
                struct tree_node_t*     src)
{
	struct tree_node_t * new_node = osal_malloc(sizeof(struct tree_node_t));
    if (!new_node){
        printf("Can not allocate memory\n");
		return (-1);
    }

	new_node->type = src->type;
    new_node->arr_dim_l = src->arr_dim_l;
    new_node->arr_dim_m = src->arr_dim_m;
    new_node->arr_dim_r = src->arr_dim_r;
    new_node->val = NULL;
	new_node->child = NULL;
	new_node->prev = new_node->next = NULL;
	new_node->parrent = NULL;

    //copy name
	new_node->name = osal_malloc(strlen(src->name) +1);
    if (!new_node->name){
        printf("Can not allocate memory\n");
		return (-1);
    }
	strcpy(new_node->name,src->name);

    if(copy_value (new_node, src)){
		return (-1);
    }

    if (tree_copy_help(&new_node->child,src->child, new_node, NULL)){
		return (-1);
    }

	*dst = new_node;
	return 0;
}

/* ============================================================================
* tree_copy_help ()
*
* Help function of tree_copy(). Copies from src to dst, parent and previous
* should be passed as parameters
*
* params
*   dst : node, where to copy the root of the tree
*   src : root of the tree to be coppied
*   par : parent node of dst
*   prev : previous node of dst
* ========================================================================== */
/*------------------------------------------------------------------------------
/
/  NOTE: DON'T SCREW WITH THIS CODE UNLESS YOU REALLY UNDERSTAND IT!
/
/-----------------------------------------------------------------------------*/
int tree_copy_help( struct tree_node_t  **dst,
                    struct tree_node_t  *src,
                    struct tree_node_t  *par,
                    struct tree_node_t  *prev)
{
	struct tree_node_t * new_node = NULL;
	struct tree_node_t * it = NULL;

	if (!src)
	{
		return 0;
	}
	if (*dst)
	{
		return 0;
	}

	new_node = osal_malloc(sizeof(struct tree_node_t));
	if (!new_node)
	{
		printf("ERROR: Can not allocate memory for new_node->name\n");
		return (-1);
	}

    new_node->val = NULL;
	new_node->parrent = par;
	new_node->next = NULL;
	new_node->child = NULL;
	new_node->type = src->type;
    new_node->arr_dim_l = src->arr_dim_l;
    new_node->arr_dim_m = src->arr_dim_m;
    new_node->arr_dim_r = src->arr_dim_r;
    //copy name
	new_node->name = (char *)osal_malloc(strlen(src->name) + 1);
	if (!new_node->name)
	{
		printf("ERROR: Can not allocate memory for new_node->name\n");
		return (-1);
	}
	strcpy(new_node->name,src->name);

    if(copy_value (new_node, src)){
		return (-1);
    }

	new_node->parrent = par;

	if (par)
	{
		if (!par->child)
		{
			par->child = new_node;
		}
	}
	new_node->prev = prev;
	if (prev)
	{
		prev->next = new_node;
	}

    //TODO: destroy *dst tree, then replace it with new_node
//    tree_destroy(*dst);

	*dst = new_node;

	it = src;
	while (it)
	{
		if (tree_copy_help(&new_node->next,it->next,new_node->parrent,new_node))
		{
			return (-1);
		}
		it = it->next;
	}
	if (tree_copy_help(&new_node->child,src->child,new_node,NULL))
	{
		return (-1);
	}

    return 0;
 }

/* ============================================================================
* tree_destroy ()
*
* Frees recursively all allocated memory for tree and its fieltd
*
* params
*   type_tree : tree to free
* ========================================================================== */
void tree_destroy_recursion (struct tree_node_t* type_tree)
{
	if (type_tree)
	{

		if (type_tree->next != NULL) {
			tree_destroy_recursion(type_tree->next);
			type_tree->next = NULL;
		}

		if (type_tree->child != NULL) {
			tree_destroy_recursion(type_tree->child);
			type_tree->child = NULL;

		}
		if (type_tree->name) {
			osal_free(type_tree->name);
			type_tree->name = NULL;
		}
		if (type_tree->val) {
			osal_free(type_tree->val);
			type_tree->val = NULL;
		}
		osal_free(type_tree);
		type_tree = NULL;
	}
}

void tree_destroy (struct tree_node_t* type_tree)
{
//distroy only this tree, not the list the tree is a part of
	if (type_tree)
	{
		if (type_tree->child != NULL) {
			tree_destroy_recursion(type_tree->child);
			type_tree->child = NULL;

		}
		if (type_tree->name) {
			osal_free(type_tree->name);
			type_tree->name = NULL;
		}
		if (type_tree->val) {
			osal_free(type_tree->val);
			type_tree->val = NULL;
		}
		osal_free(type_tree);
		type_tree = NULL;
	}
}

void free_limits(struct space_t *limits)
{
    struct space_t* current;
    struct space_t* next;
    if(!limits){
        return;
    }

    current = limits;

    while(current){
        next = current->next;
        osal_free(current);
        current = next;
    }
}

void free_photospace(struct photo_region_t *photospace)
{
    struct photo_region_t* current;
    struct photo_region_t* next;
    if(!photospace){
        return;
    }

    current = photospace;
    while(current){
        next = current->next;
        if(current->limits){
            free_limits(current->limits);
            current->limits = NULL;
        }
        osal_free(current);
        current = next;
    }
}

void free_parpack(struct par_package_t* parpak)
{
    struct par_package_t* current;
    struct par_package_t* next;
    if(!parpak){
        return;
    }

    current = parpak;
    while(current){
        next = current->next;
        if(current->write_buffer){
            osal_free(current->write_buffer);
        }
        osal_free(current);
        current = next;
    }
}

void use_cases_destroy(struct use_case_t *use_list)
{
    struct use_case_t* current=NULL;
    struct use_case_t* next=NULL;
    if(!use_list){
        return;
    }

    current = use_list;
    while(current){
        next = current->next;
        if(current->known_type)
        {
            //destroy the known types list for that usecase
	        //destroy all trees of types in the known types list
            struct known_types_t* known_types;
            known_types = current->known_type;
            while (known_types)
	        {
		        tree_destroy(known_types->type_tree);
		        known_types = known_types->next;
	        }
	        //destroy the known types list itself
            known_types_destroy(current->known_type);
            current->known_type = NULL;
        }
        //usecase parameters are in the current->known_types list, and were
        //just freed. Update the dedicated pointer to them.
        current->use_case_parameter = NULL;

        if(current->photospaces){
            free_photospace(current->photospaces);
            current->photospaces = NULL;
        }
        if(current->par_pack){
            free_parpack(current->par_pack);
            current->par_pack = NULL;
        }
        if(current->write_buffer){
            osal_free(current->write_buffer);
        }
        osal_free(current);
        current = next;
    }
}

/* ============================================================================
* known_types_destroy ()
*
* Frees recursively all allocated memory for knwon types list
*
* params
*   type_tree : tree to free
* ========================================================================== */
void known_types_destroy (struct known_types_t *type_list)
{
    struct known_types_t* ptr = type_list;

	while (ptr)
	{
		struct known_types_t* temp = ptr;
		ptr = ptr->next;
		if (temp->type_name)
		{
			osal_free(temp->type_name);
			temp->type_name = NULL;
		}
		osal_free(temp);
	}
}

/* ============================================================================
*	create_tree_node ()
*
*	Creating a tree_note_t structure
*
* params:
*	new_node: new node to be added
*	list	: pointer to the begining of the list
* ========================================================================== */
struct tree_node_t * create_tree_node(  struct tree_node_t  *par,
                                        struct tree_node_t  *next,
                                        struct tree_node_t  *prev,
                                        struct tree_node_t  *child,
                                        char    *name)
{
	struct tree_node_t * new_node = osal_malloc(sizeof(struct tree_node_t));
	if (!new_node)
	{
		printf("Could not allocate memory for new_node\n");
		return NULL;
	}
	new_node->parrent = par;
	new_node->next = next;
	new_node->prev = prev;
	new_node->child = child;
	if (name)
	{
		new_node->name = osal_malloc (strlen(name) + 1);
		if (!new_node->name)
		{
			printf("Could not allocate memory for name\n");
			return NULL;
		}
		strcpy(new_node->name,name);
    } else {
		new_node->name = NULL;
    }
	new_node->val = NULL;
	new_node->type = 0;
    new_node->arr_dim_l = 0;
    new_node->arr_dim_m = 0;
    new_node->arr_dim_r = 0;

	return new_node;
}

void change_file_ext(char   *fname,
                     char   *ext)
{
    char* str;
    char* last;
    char* dir;

    //default: append new extension at the end of fname
    str = &fname[strlen(fname)];

    dir = strrchr(fname, DIR_DELIMITER);
    //search for last appearance of '.' (it may never appear)
    last = strrchr(fname, '.');
    if (last && (last > dir)) {
        str = last;
    }

    //append new file extension
    strcpy(str, ext);
}

void to_upper_without_ext(char *fname)
{
    char* str;
    char* last;
    int strptr;

    //default: append new extension at the end of fname
    strptr = (int)fname[strlen(fname)-1];
    str = (char*)strptr;

    //search for last appearance of '.' (it may never appear)
    last = strrchr(fname, '.');
    if (last) {
        str = last;
    }

    //append new file extension
	str[0] = '\0';

	//default: append new extension at the end of fname
    strptr = (int)fname[strlen(fname)-1];
    str = (char*)strptr;
    //search for last appearance of '\'or'/' (it may never appear)
    last = strrchr(fname, DIR_DELIMITER);

    str = fname;
    if (last) {
        str = last+1;
    }
	strcpy(fname,str);

    //convert to upper case
	last = fname;
	while (*last)
	{
		if ((*last) >='a' && (*last)<='z')
		{
			*last +=('A'-'a');
		}
		last++;
	}

}


/* ============================================================================
*	search_knwon_types ()
*
*	Searching by name for node in known types
*
* params:
*	start: pointer to the beginning of the list of known types
*	name	: name to search for
* ========================================================================== */

struct known_types_t * search_knwon_types(  char    *name,
                                            struct known_types_t *start)
{
	while (start)
	{
		if ( strcmp(start->type_name,name) == 0)
		{
			return start;
		}
		start = start->next;
	}
	return NULL;
}

/* =============================================================================
*	make_one_dim_list_with_nodes ()
*
*	Makes N copies of source and connects them in a list. new_node child is
*   pointing to the beginning of this list. N is calculated from the type of
*   the new_node
*
* params:
*	new_node: pointer to the new_node which will be the parent of the new list
*   of copies
*	source	: node to copy from
* =========================================================================== */
int make_one_dim_list_with_nodes(struct tree_node_t    **new_node,
                                  const struct tree_node_t *source)
{
	uint32 dims;
	//int arr_dims = ((*new_node)->type & BASIC_TYPE_ARRAY) >>
    //                                              BASIC_TYPE_ARRAY_DIMS_OFST;

    //make a list with structures = the count of the array
	struct tree_node_t * start = NULL;
	struct tree_node_t * last = NULL;
	struct tree_node_t * cur = NULL;
	
	dims= (*new_node)->arr_dim_l;

    if(((*new_node)->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
        if(copy_value (*new_node, source)){
            return (-1);
        }
    }

	while (dims)
	{
		cur = NULL;
        if( tree_copy(&cur,(struct tree_node_t *)source) ){
            return (-1);
        }

		cur->parrent = *new_node;

		if ((*new_node)->child == NULL)
		{
            //this is first iteration on 'dims'. First element of the array is
            //just created,it is child of (*new_node). Next elements will be
            //added in a list.
			start = cur; //first element of the array
            last  = cur;
			(*new_node)->child = cur;
		}
		else
		{
            //This is not first iteration on 'dims'. Add this element in a list
            //after the previous created.
            last->next = cur;
			cur->prev = last;
			cur->next = NULL;

            last  = cur;
		}

		dims-=1;
	}
	if (start)
	{
        //If there is a list of array elements created (dims!=0 and *new_node
        //had NOT already created list)
	    //make the child of the new node the first element in the created list
		(*new_node)->child = start;
	}
    return 0;
}

/* =============================================================================
*	make_two_dim_list_with_nodes ()
*
*	Like the above one but makes a two floor tree
*
* params:
*	new_node: pointer to the new_node which will be the parent of the new tree
*   of copies
*	source	: node to copy from
* =========================================================================== */
int make_two_dim_list_with_nodes(  struct tree_node_t** new_node,
                                    const struct tree_node_t * source)
{
	uint32 dim_l = (*new_node)->arr_dim_l;		// get the 2 dimensions
	uint32 dim_m = (*new_node)->arr_dim_m;
	uint32 counter = 0;
	int arr_dims = (int)(((*new_node)->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);

	struct tree_node_t* cur = NULL;
	struct tree_node_t* temp = NULL;

	if(((*new_node)->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
        if(copy_value (*new_node, source)){
            return (-1);
        }
    }

	arr_dims--;
	if (dim_l)			// if we know the 2nd dimension size make nodes
	{
		counter = dim_l;
		while (counter)
		{
			temp = create_tree_node((*new_node),NULL,NULL,NULL,source->name);
			temp->type |= BASIC_TYPE_STRUCT;
			temp->type |= (uint16)arr_dims << BASIC_TYPE_ARRAY_DIMS_OFST;
			temp->arr_dim_l = dim_m;
			if ((*new_node)->child == NULL)
			{
				(*new_node)->child = temp;
				cur = temp;
			}
			else
			{
				cur->next = temp;
				temp->prev = cur;
				cur = temp;
			}
            if( make_one_dim_list_with_nodes(&temp,source) ){
                return (-1);
            }
			counter-=1;
		}
	}
	else
	{
        // if we do not know the 1st dimension size make 1D array with 2nd
        //dimension's count temp. Their lists of childs will be filled when
        //values for them are found (corresponding "{...}")
		temp = create_tree_node((*new_node),NULL,NULL,NULL,source->name);
		temp->type |= BASIC_TYPE_STRUCT;
		temp->type |= (uint16)1 << BASIC_TYPE_ARRAY_DIMS_OFST;
		temp->arr_dim_l = dim_m;
		(*new_node)->child = temp;
        if( make_one_dim_list_with_nodes(&temp,source) ){
            return (-1);
        }
	}
    return 0;
}

/* =============================================================================
*	make_three_dim_list_with_nodes ()
*
*	Like the above one but makes a three floor tree
*
* params:
*	new_node: pointer to the new_node which will be the parent of the new tree
*   of copies
*	source	: node to copy from
* =========================================================================== */
int make_three_dim_list_with_nodes(    struct tree_node_t** new_node,
                                        const struct tree_node_t * source)
{
    // get the 3 dimensions
	uint32 dim_l = (*new_node)->arr_dim_l;
	uint32 dim_m = (*new_node)->arr_dim_m;
	uint32 dim_r = (*new_node)->arr_dim_r;
	uint32 counter = 0;
	int arr_dims = (int)(((*new_node)->type & BASIC_TYPE_ARRAY) >>
                                                    BASIC_TYPE_ARRAY_DIMS_OFST);

	struct tree_node_t* cur = NULL;
	struct tree_node_t* temp = NULL;

	if(((*new_node)->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
        if(copy_value (*new_node, source)){
            return (-1);
        }
    }

	--arr_dims;
	if (dim_l)			// if we know the 2nd dimension size make nodes
	{
		counter = dim_l;
		while (counter)
		{
			temp = create_tree_node((*new_node),NULL,NULL,NULL,source->name);
			temp->type |= BASIC_TYPE_STRUCT;
			temp->type |= (uint16) arr_dims << BASIC_TYPE_ARRAY_DIMS_OFST;
			temp->arr_dim_m = dim_r;
			temp->arr_dim_l = dim_m;

			if ((*new_node)->child == NULL)
			{
				(*new_node)->child = temp;
				cur = temp;
			}
			else
			{
				cur->next = temp;
				temp->prev = cur;
				cur = temp;
			}
            if( make_two_dim_list_with_nodes(&temp,source) ){
                return (-1);
            }
			counter-=1;
		}
	}
	else				// if we do not know the 1st dimension size make
	{
		temp = create_tree_node((*new_node),NULL,NULL,NULL,source->name);
		temp->type |= BASIC_TYPE_STRUCT;
		temp->type |= (uint16)1 << BASIC_TYPE_ARRAY_DIMS_OFST;
		temp->arr_dim_l = dim_m;
		(*new_node)->child = temp;
        if( make_one_dim_list_with_nodes(&temp,source) ){
            return (-1);
        }
	}
    return (0);
}

/* =============================================================================
* make_list_with_n_nodes ()
*
* Makes n copies of the source node and arranges them in a tree according to the
* type of the array of the new_node
Ex: [2][3]
		new_node
		|
	 copy	  ->	copy
	/ | \		  /  |  \
copy copy copy  copy copy copy
* params
*   source		: pointer to the original node
*	new_node	: poitner to the parent of the new tree
* =========================================================================== */
int make_tree_with_nodes(  struct tree_node_t** new_node,
                            const struct tree_node_t * source)
{
	uint32 dims[3];
	int arr_dims = (int)(((*new_node)->type & BASIC_TYPE_ARRAY) >>
                                                    BASIC_TYPE_ARRAY_DIMS_OFST);
    // get the 3 dimensions
	dims[0]= (*new_node)->arr_dim_l;
    dims[1]= (*new_node)->arr_dim_m;
    dims[2]= (*new_node)->arr_dim_r;


	if ( (arr_dims == 3) &&
		(!dims[2] || !dims[1]))
	{
		printf("Array is 3 dimensional with unknown 2nd or 1st dim size\n");
		dtp_gen_abort(1);
	}
	if ( (arr_dims == 2) &&
		(dims[2]|| !dims[1]))
	{
		printf("Array is 2 dimensional with known 3rd or unknown 1st dim size\n");
		dtp_gen_abort(1);
	}
	if ( (arr_dims == 1) &&
		(dims[1]|| dims[2]))
	{
		printf("Array is 1 dimensional with known 2nd or 1st dim size\n");
		dtp_gen_abort(1);
	}

    //checking wheter it is one dim array with known size
	if ((arr_dims==1) && dims[0] )
	{
        if( make_one_dim_list_with_nodes(new_node,source) ){
            return (-1);
        }
	}
	else if((arr_dims == 2) &&dims[0] && dims[1])
	{
	    // checking wheter it is two dim array with known sizes
		if( make_two_dim_list_with_nodes(new_node,source) ){
            return (-1);
        }
	}
	else if((arr_dims == 3) && dims[0] && dims[1] && dims[2])
	{
	    // checking wheter it is three dim array with known sizes
		if( make_three_dim_list_with_nodes(new_node,source) ){
            return (-1);
        }
	}
	else
	{
        //The tree to be coppied is not an array. Create new node and copy
        //recursively "source->child" list or tree in it. Then assigh this new
        //node to (*new_node)->child
		struct tree_node_t * cur = NULL;

        copy_value ((*new_node), (struct tree_node_t*)source);

		if( tree_copy_help(&cur,source->child, *new_node, NULL) ){//asd tree_copy_help(&cur,source->child,cur,NULL);
            return (-1);
        }
		(*new_node)->child = cur;
//		cur->parrent = *new_node;
	}
    return 0;
}

void print_tree(    struct tree_node_t * source,
                    int depth)
{
    int i,arr_dims;
    uint32 c, dim,dim_l,dim_m,dim_r;
    uint32 *ix;

    if(!source){
        return;
    }

    dim_l = source->arr_dim_l;
	dim_m = source->arr_dim_m;
	dim_r = source->arr_dim_r;
	arr_dims = (int)((source->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);

    if(depth < 0 || !source){
        printf("ERROR print tree source= NULL");
        return;
    }

    for(i=0; i< depth;i++){
        printf("\t");
    }
    if((source->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
        printf("%s\t%s\n", source->name, (char*)source->val);
    }
    else if(source->val)
    {
        ix = (uint32*)source->val;
        printf("%s = (%lld)", source->name,source->type);
        if(arr_dims == 0){
            printf(" {%ld}\n",ix[0]);
        }
        for(i = 0; i<arr_dims;i++){

            printf(" {");
            if(i == 0)
                dim = dim_l;
            else if(i==1)
                dim = dim_m;
            else if(i==2)
                dim = dim_r;

            for(c = 0;c<dim;c++){
                printf("%ld, ",ix[c]);
            }

            printf("}\n");
        }
    }
    else
        printf("%s\n", source->name);

    if(source->child){
        print_tree(source->child,depth+1);
    }
    if(source->next){
        print_tree(source->next,depth);
    }

    return;
}



/* =============================================================================
* create_known_type_node()
*
* Adds a new type in the known types list
* params
*   type_name:	name of the new node
*   type_tree:	tree of the new node
*   prev:		pointer to previous element
*	next:		pointer to next element
* return
*   pointer to newly created item
*   NULL if error
* =========================================================================== */
struct known_types_t* create_known_type_node(   char* type_name,
                                                struct tree_node_t* type_tree,
                                                struct known_types_t *prev,
                                                struct known_types_t *next)
{
	struct known_types_t * new_node = osal_malloc(sizeof(struct known_types_t));
	if (!new_node)
	{
		printf("Not enough memory for allocating tree_node_t structure\n");
		return NULL;
	}
	if (!type_name)
		new_node->type_name = NULL;
	else
	{
		new_node->type_name = osal_malloc(strlen(type_name) + 1);
		if (!new_node->type_name)
		{
			printf("Not enough memory for allocating type_name\n");
			return NULL;
		}
		strcpy(new_node->type_name,type_name);
	}
	new_node->next = next;
	new_node->prev = prev;
	new_node->type_tree = type_tree;
	return new_node;

}

/* =============================================================================
*	add_node_to_list ()
*
*	Connecting new node to a list at the last position
*
* params:
*	new_node: new node to be added
*	list	: pointer to the begining of the list
* =========================================================================== */
void add_node_to_list(  struct known_types_t**  new_node,
                        struct known_types_t**  list)
{

	if (!*list)		// list is empty
	{
		*list = *new_node;
	}
	else
	{
		struct known_types_t * it = (*list);
		while (it->next != NULL)
			it = it->next;
		it->next = *new_node;
		(*new_node)->prev = it;
		(*new_node)->next = NULL;
	}
}

/* =============================================================================
*	strtoi_dec()
*
*	Makes number from string containing decimal number
* =========================================================================== */
int strtoi_dec(char * string, int64* num)
{
	int64 result = 0;

	while (*string != '\0')
	{
		if (*string <'0' || *string > '9')
		{
			return (-1);
		}
		result *=10;
		result += *string-'0';
		string++;
	}
    *num = result;
    return 0;
}

int strtoi_hex(char * string, int64* num)
{
	int64 result = 0;

	while (*string)
	{
        int digit;
		if ((*string >='0') && (*string <= '9'))
		{
			digit = *string - '0';
        } else if ((*string >='a') && (*string <= 'f')) {
            digit = *string - 'a' + 10;
        } else if ((*string >='A') && (*string <= 'F')) {
            digit = *string - 'A' + 10;
        } else {
			return (-1);
        }
		result *= 16;
		result += digit;

		string++;
	}
    *num = result;
    return 0;
}


int make_number_from_string(char * data,int * current, int64 * result)
{
	char number[21];
	int i;
	int8 negative = 1;
    int sts;

	if (*(data+ *current) == '-')
	{
	    // if first char is '-' then the number will be negative
		negative = -1;
		// advance for the '-' char ( if present )
		*current+=1;
	}

    i = 0;
	while ( *(data + i + *current) != ',' &&
		*(data + i+ *current) != '}' &&
		*(data + i+ *current) &&
        i < (sizeof(number)-1)
        )
    {
        number[i] = *(data + i+ *current);
		i++;
	}
	number[i] = '\0';
	*current += i;
	// atoi returns int32, cannot handle uint32 numbers > 2147483648
//	*result = atoi(number);
    if ( (number[0] == '0') &&
         ((number[1] == 'x') || (number[1] == 'X'))
       ){
           //hex number follows
        sts = strtoi_hex(&number[2], result);
    } else {
        sts = strtoi_dec(number, result);
    }
    if( sts ){
		printf("Unresolvalble number %s\n", number);
		return (-1);
    }
	*result = (*result)*negative;
	return 0;
}


void parse_known_name(  char * buffer,
                        char * data,
                        int * current)
{
	char * ptr = data;
	while ( (*ptr != ',') &&
		(*ptr != '}'))
	{
		*buffer = *ptr;
		buffer++;
		ptr++;
		*current+=1;
	}
	*buffer = '\0';
}

/* =============================================================================
*	count_elems_in_struct_array ()
*
*	Counts the elements in a struct according to its dimension
*
* params:
*	buff:	string from which we will determine the sizes
*	dims	: output parameter to store the dimensions
*	dimension	: dimension of the structure
* =========================================================================== */
/*------------------------------------------------------------------------------
/
/  NOTE: DON'T SCREW WITH THIS CODE UNLESS YOU REALLY UNDERSTAND IT!
/
/-----------------------------------------------------------------------------*/

void count_elems_in_struct_array(     char * buff,
                                uint32* dims,
                                const int dimension)
{
	int bracket = -1;
	int i;
 	char * ptr = buff;

	while (*ptr != '\0')
	{
		if (*ptr == '{')
		{
			bracket++;
		}
		else if (*ptr == '}')
		{
			bracket--;
		}
		if (bracket == 2 && *ptr == '}'&& (dimension>2))
			dims[bracket]+=1;
		else if (bracket == 1 && *ptr == '}'&& (dimension > 1))
			dims[bracket]+=1;
		else if (bracket == 0 && *ptr == '}')
			dims[bracket]+=1;

		if (bracket == -1)
			break;

		ptr++;
	}

	if (dims[2])
	{
		dims[2] = dims[2] / dims[1];
	}
	if (dims[1])
	{
		dims[1] = dims[1] / dims[0];
	}

	for (i = 0;i < 3;i++)
	{
		if (dims[0] == 0)
		{
			dims[0] = dims[1];
			dims[1] = dims[2];
			dims[2] = 0;
		}
	}

}

/* =============================================================================
*	count_elements_in_3_dimensional_array ()
*
*	Counts the elements in an array
*
* params:
*	buff:	string from which we will determine the sizes
*	dims	: output parameter to store the dimensions
* =========================================================================== */
void count_elements_in_3_dimensional_array( char * buff,
                                            uint32 * dims)
{
	char * ptr = buff;
	int dim = -1;
	int i = 0;
	while (*ptr != '\0' )
	{
		if (*ptr == '{')
		{
			dim++;
		}

		if (dim>2 || dim<0)
			break;
		if (*ptr == '}')
		{
			dims[dim]+=1;
			dim--;
		}

		if (*ptr == ',')
		{
			dims[dim]+=1;
		}
		ptr++;
	}

	if (dims[2])
	{
		dims[2] = dims[2] / dims[1];
	}
	if (dims[1])
	{
		dims[1] = dims[1] / dims[0];
	}

	for (i = 0;i < 3;i++)
	{
		if (dims[0] == 0)
		{
			dims[0] = dims[1];
			dims[1] = dims[2];
			dims[2] = 0;
		}
	}
}

/* =============================================================================
*	different ()
*
*	Checks whether declared node is the same as known_node node
*
* params:
*	buff:	string from which we will determine the sizes
*	dims	: output parameter to store the dimensions
* =========================================================================== */
int8 different( const struct tree_node_t * declared_node,
                const struct tree_node_t * known_node)
{
	uint8 arr_dims = 0;
	uint32 dim[3];
	arr_dims = (uint8)((declared_node->type & BASIC_TYPE_ARRAY) >>
                                                    BASIC_TYPE_ARRAY_DIMS_OFST);

	if (arr_dims == 3)
	{
		dim[0] = declared_node->arr_dim_l;
		dim[1] = declared_node->arr_dim_m;
		dim[2] = declared_node->arr_dim_r;

		if (dim[0] &&  (dim[0] != known_node->arr_dim_l))
		{
			return (-1);
		}
		if (dim[1])
		{
			if(!known_node->child)
				return (-1);
			if(dim[1] != known_node->child->arr_dim_l)
				return (-1);
		}
		if (dim[2])
		{
			if (!known_node->child)
				return (-1);
			if (!known_node->child->child)
				return (-1);
			if (dim[2] != known_node->child->child->arr_dim_l)
				return (-1);
		}
	}
	else if (arr_dims == 2)
	{
		dim[0] = declared_node->arr_dim_l;
		dim[1] = declared_node->arr_dim_m;

		if (dim[0] && (dim[0] != known_node->arr_dim_l))
		{
			return (-1);
		}
		if (dim[1])
		{
			if (!known_node->child)
				return (-1);
			if (dim[1] != known_node->child->arr_dim_l)
				return (-1);
		}
	}



	//TODO: !Proper check when arr_dims == 1 !!!
	else if (arr_dims == 1)
	{
		dim[0] = declared_node->arr_dim_l;
		if (dim[0] && (dim[0] != known_node->arr_dim_l))
		{
		    printf("Error: Declared node has different amount of elements in the array Node: %s",
                        declared_node->name);
			return (-1);
		}
		//if (declared_node->child)
		//{
		//	if (!known_node->child)
		//		return (-1);
		//	if (declared_node->child->arr_dim_l != known_node->child->arr_dim_l)
		//		return (-1);

		//	if (declared_node->child->child)
		//	{
		//		if (!known_node->child->child)
		//			return (-1);
		//		if (declared_node->child->child->arr_dim_l != known_node->child->child->arr_dim_l)
		//		return (-1);

		//	}
		//}

	}
	return 0;
}
//IMPORTED TOOLS FROM GENERAL_DATA.C

/* =============================================================================
* write_values_known_struct()
*
* Connects the data in the node from an already known type
* =========================================================================== */
int write_values_known_struct (    struct tree_node_t** tree,
                                        struct known_types_t* search_tree,
                                        char * data,
                                        int * current,
                                        dtp_gen_ctx_t* ctx)
{
	char node_name[NAME_FIELD_LENGTH];
//TODO: when reading typedef,gen_data,use_case_data and parpacks - chech for maximum name length 79 symbols. Make this number to #define
    struct tree_node_t* add_tree = *tree;
	struct known_types_t * searched = NULL;
	struct tree_node_t * new_node = NULL;
	struct tree_node_t * source = NULL;

	parse_known_name((char*)&node_name,(data+*current),current);
	if (add_tree->next)
	{
		*current+=1; //skipthe next ',' or '}'
	}

	searched = search_knwon_types(node_name,search_tree);
	if (!searched || !searched->type_tree)
	{
		printf("Error: could not find predefined data named \"%s\" to fill %s ( after line %ld)\n",
                        node_name, (char*)(*tree)->val, ctx->xml_line);
		return (-1);
	}
    //Check whether the found predefined tree is of the same type as the tree
    //to be filled (add_tree)
    if( 0 != strcmp(add_tree->name, searched->type_tree->name)){
		printf("Error: Predefined data (%s) of type %s can not be used to initialize data of type %s ( after line %ld)\n",
                                                  node_name, searched->type_tree->name, add_tree->name, ctx->xml_line);
		return (-1);

    }


    //Chech whether the dimentions of the found predefined tree ar compatible
    //with the dimentions of tree to be filled (add_tree)
	if (different(add_tree, searched->type_tree))
	{
		{
			printf("Error: declared array (%s) differs from initialising array (%s) ( after line %ld)\n",
                        searched->type_tree->name, add_tree->name, ctx->xml_line);
			return (-1);
		}
	}



    source = searched->type_tree;

    //create new_node and copy source tree into it.
    if(tree_copy(&new_node, source)){
        return (-1);
    }

	new_node->prev = add_tree->prev;
	new_node->next = add_tree->next;
	new_node->parrent = add_tree->parrent;

    //copy the filed name of add_tree in the new node. Since add_tree is a
    //struct, it is held in add_tree->val since the add_tree->name may keep
    //the predefined type
    if(new_node->val){
        //new_node->val may have a content taken from source->val, which is
        //not relevant
        osal_free(new_node->val);
        new_node->val = NULL;
    }
    copy_value(new_node, add_tree);

    //replace add_tree with new_node in the parrent tree/list
	if (add_tree->prev)
		add_tree->prev->next = new_node;
	if (add_tree->next)
		add_tree->next->prev = new_node;
    if (add_tree->parrent){
        if (add_tree->parrent->child == add_tree){
            //TODO whiy is this if? remove?
			add_tree->parrent->child = new_node;
        }
    }
    //copy the undef_array bit from original type definition
    new_node->type |= add_tree->type & BASIC_TYPE_ARRAY_UNDEF;

    //destroy the tree that was in add_tree - it is completely replaced by the
    //new tree in 'new_node'
    tree_destroy(add_tree);

    //replace th ecalling stack with new_node
    *tree = new_node;

	return 0;
}

/* =============================================================================
* write_values_known_array()
*
* Connects the data in the node from an already known type
* =========================================================================== */
int write_values_known_array (    struct tree_node_t** tree,
                                        struct known_types_t* search_tree,
                                        char * data,
                                        int * current,
                                        dtp_gen_ctx_t* ctx)
{
	char node_name[NAME_FIELD_LENGTH];
//TODO: when reading typedef,gen_data,use_case_data and parpacks - chech for maximum name length 79 symbols. Make this number to #define
    struct tree_node_t* add_tree = *tree;
	struct known_types_t * searched = NULL;
	//struct tree_node_t * new_node = NULL;
	//struct tree_node_t * source = NULL;
	uint32 src_arr_dims;
	uint32 src_dim_l    ;
	uint32 src_dim_m    ;
	uint32 src_dim_r    ;

	uint32 dst_arr_dims;
	uint32 dst_dim_l    ;
	uint32 dst_dim_m    ;
	uint32 dst_dim_r    ;


	parse_known_name((char*)&node_name,(data+*current),current);
    if ((add_tree->next) && (*(data+*current) == ','))
	{
		*current+=1; //skipthe next ','
	}

	searched = search_knwon_types(node_name,search_tree);
	if (!searched || !searched->type_tree)
	{
		printf("Error: could not find predefined data named \"%s\" to fill %s array ( after line %ld)\n",
            node_name, (*tree)->name, ctx->xml_line);
		return (-1);
	}
    //Check whether the found predefined tree is of the same type as the tree
    //to be filled (add_tree)
    if((add_tree->type & BASIC_TYPE_MASK) != (searched->type_tree->type & BASIC_TYPE_MASK)){
		printf("Error: Predefined data (%s) of type %s can not be used to initialize data %s of type %s ( after line %ld)\n",
                searched->type_name, BASIC_TYPES_NAMES[(searched->type_tree->type & BASIC_TYPE_MASK)],
                add_tree->name, BASIC_TYPES_NAMES[(add_tree->type & BASIC_TYPE_MASK)],
                ctx->xml_line);
		return (-1);

    }

    src_arr_dims = (searched->type_tree->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST;
	src_dim_l    = searched->type_tree->arr_dim_l;
    src_dim_m    = searched->type_tree->arr_dim_m;
    src_dim_r    = searched->type_tree->arr_dim_r;

    dst_arr_dims = (add_tree->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST;
	dst_dim_l    = add_tree->arr_dim_l;
    dst_dim_m    = add_tree->arr_dim_m;
    dst_dim_r    = add_tree->arr_dim_r;

    if( (src_arr_dims != dst_arr_dims)                    ||
        ((dst_dim_l) && (dst_dim_l != src_dim_l))            ||
        ((dst_arr_dims > 1) && (dst_dim_m != src_dim_m))    ||
        ((dst_arr_dims > 2) && (dst_dim_r != src_dim_r))    )
    {
		printf("Error: declared array (%s) differs from initialising array (%s) ( after line %ld)\n",
                    searched->type_name, add_tree->name, ctx->xml_line);
		return (-1);
    }

    copy_value(add_tree, searched->type_tree);

    //Make sure to reassign the type in order to update it when we have undefined size array
    add_tree->type = searched->type_tree->type;
    add_tree->arr_dim_l = searched->type_tree->arr_dim_l;
    add_tree->arr_dim_m = searched->type_tree->arr_dim_m;
    add_tree->arr_dim_r = searched->type_tree->arr_dim_r;

	return 0;
}

/* =============================================================================
* edit_tree()
*
* Checks if the node is an array of structs with unkown size yet. If the size is
* unknown, determines it and creates as many subtrees equal to the size of
* the array
* =========================================================================== */
int edit_tree( struct tree_node_t ** node,
                char * data)
{
	uint32 dims[3];
	struct tree_node_t * tree = *node;

    // get how many dimensions the array have
	int arr_dims =(int)((tree->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);
	//get dimensions of the array
	uint32 dim_l = tree->arr_dim_l;
    uint32 dim_m = tree->arr_dim_m;
    uint32 dim_r = tree->arr_dim_r;

    if(arr_dims == 0) {
        //this is not an array, nothing to do
        return 0;
    }

	dims[0] = dims[1] = dims[2] = 0;
	count_elems_in_struct_array(data ,dims,arr_dims);			// counts the elements

    if( ((dim_l) && (dim_l != dims[0]))            ||
        ((arr_dims > 1) && (dim_m != dims[1]))    ||
        ((arr_dims > 2) && (dim_r != dims[2]))    )
	{
        char act_type[TYPE_FIELD_LENGTH+7*3+1];
        char tree_type[TYPE_FIELD_LENGTH+7*3+1];
        uint16 act;
        uint32 act_arr_dim_l;
        uint32 act_arr_dim_m;
        uint32 act_arr_dim_r;


        act = (tree->type & BASIC_TYPE_MASK) |
            ((uint16)arr_dims << BASIC_TYPE_ARRAY_DIMS_OFST);

        act_arr_dim_l = dims[0];
        act_arr_dim_m = dims[1];
        act_arr_dim_r = dims[2];
        type_to_string(tree->type, tree->arr_dim_l, tree->arr_dim_m, tree->arr_dim_r, tree_type);
        type_to_string(act, act_arr_dim_l, act_arr_dim_m, act_arr_dim_r, act_type);

		printf("Error: array %s of type %s can not be initialize by array of type (%s)\n",
            tree->name, tree_type, act_type);
		return (-1);
    }


	if (dim_l == 0)
	{
        //If non-fully defined array - now it is fully defined by the values
        //present - build the actual tree
		struct tree_node_t * temp = NULL;
		struct tree_node_t * source = NULL;

        //create root of a new array tree
		temp = create_tree_node(NULL,NULL,NULL,NULL,tree->name);

        //If tree is non-fully defined array, it keeps the tree of one
        //element of this array - use it as a source to copy other elements
        if ( tree_copy(&source,tree) ){
            return (-1);
        }
		source->type = BASIC_TYPE_STRUCT;
        source->arr_dim_l = 0;
        source->arr_dim_m = 0;
        source->arr_dim_r = 0;

        //copy the type of from typedef and update non-defined array dimension (left one)
		temp->type = tree->type;
		temp->arr_dim_l = tree->arr_dim_l;
		temp->arr_dim_m = tree->arr_dim_m;
		temp->arr_dim_r = tree->arr_dim_r;

		temp->arr_dim_l = dims[0];

        //build the array tree
        if( make_tree_with_nodes(&temp,source) ){
            return (-1);
        }

		temp->prev = (*node)->prev;
		temp->next = (*node)->next;
		temp->parrent = (*node)->parrent;

		if ((*node)->prev)
			(*node)->prev->next = temp;
		if ((*node)->next)
			(*node)->next->prev = temp;
		if ((*node)->parrent)
			if ((*node)->parrent->child == *node)
				(*node)->parrent->child = temp;
		temp->parrent = (*node)->parrent;

		*node = temp;

        //now previous (*node) is replaced by temp. Free the memory it uses
        tree_destroy(tree); //keeps previous (*node)
        tree_destroy(source); //lokal copy of previous (*node)
	}
    return 0;
}

/* =============================================================================
* write_values_in_nodes()
*
* Fills in the data in the nodes from the string
* =========================================================================== */
int write_values_in_nodes ( struct tree_node_t* tree,
                            struct known_types_t* search_tree,
                            char * data,
                            int * current,
                            dtp_gen_ctx_t* ctx)
{
    //"tree" is first element (structure filed) of a list (structure) which
    //values have to be filled. The values are in "data" string and are gouped
    //like {...},{...},...
    //
    // determine how dimensional is the array
    int undef_array;
	struct tree_node_t * next_elem = NULL;

    next_elem = tree;
	do {
        undef_array = 0;
        if((next_elem->type & BASIC_TYPE_ARRAY) && (next_elem->type & BASIC_TYPE_ARRAY_UNDEF)){
            undef_array = 1;
        }

        if (undef_array && 
            ( *(data + *current   ) == 'N' ) &&
            ( *(data + *current +1) == 'U' ) &&
            ( *(data + *current +2) == 'L' ) &&
            ( *(data + *current +3) == 'L' )
            ) {
            //undefined array which should not be present in DCC binary
            next_elem->arr_dim_l = 0;
            next_elem->arr_dim_m = 0;
            next_elem->arr_dim_r = 0;
            *current += 4;
            if ((next_elem->next) && (*(data + *current) == ',')) {
		        *current +=1; //skip ,
            }
        }

		else if ( (next_elem->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT )
		{


	        // if next char is '{' -> we have to init the
		    // node with the following data
			if ( *(data + *current) == '{' )
			{
			    //Check if the current node is an array with unknown size and if so,
			    //make changes to its subtree
				if (undef_array)
				{
                    if( edit_tree(&next_elem,( data + *current )) ){
                        return (-1);
                    }
				}

				if (write_values_struct(next_elem,search_tree, data, current,ctx))
				{
//					printf("Error: could not write data to struct ( after line %ld)\n",
//                                                                ctx->xml_line);
					return (-1);
				}
			}
		    else // if not, we have to search through the known types
			{
				if (write_values_known_struct( &next_elem,
                                                    search_tree,
                                                    data,
                                                    current,
                                                    ctx))
				{
//					printf("Error: could not write data to struct ( after line %ld)\n",
//                                                                ctx->xml_line);
					return (-1);
				}
			}
		}
        else if(next_elem->type & BASIC_TYPE_ARRAY)
		{
            //Array of a single type
			if ( *(data + *current) == '{' )
			{
                if (write_values_array(next_elem, data, current))
			    {
//				    printf("Error: could not write data to array ( after line %ld)\n",
//                                                                    ctx->xml_line);
				    return (-1);
			    }
            } else {
				if (write_values_known_array( &next_elem,
                                                    search_tree,
                                                    data,
                                                    current,
                                                    ctx))
				{
//					printf("Error: could not write data to struct ( after line %ld)\n",
//                                                                ctx->xml_line);
					return (-1);
				}
            }
        }
		else
		{
            //Single number
            if (write_values_number(next_elem, data, current))
			{
//				printf("Error: could not write data to number ( after line %ld)\n",
//                                                                ctx->xml_line);
				return (-1);
			}
        }

        next_elem = next_elem->next;

    } while (next_elem != NULL);

	return 0;
}

/* =============================================================================
* write_values_struct()
*
* Fills in the data in a struct from the string
* =========================================================================== */
int write_values_struct(    struct tree_node_t * node,
                            struct known_types_t* search_tree,
                            char * data,
                            int * current,
                            dtp_gen_ctx_t* ctx)
{
	if (node->child == NULL)
	{
		printf("ERROR: Struct with no children\n ( after line %ld)\n",
                                                                ctx->xml_line);
		return (-1);
	}

	if ( *(data + *current) != '{')
	{
        char string[11];
        int i;
        for(i=0; i<sizeof(string); i++){
            string[i] = *(data + *current + i);
        }
        string[10] = 0;
        printf("Error: wrong format when start initialising structure %s:  ( after line %ld) : <%s...> instead of '{'\n",
            node->name, ctx->xml_line, string);
		return (-1);
	}

	*current +=1;//skip {

    if(write_values_in_nodes(node->child,search_tree, data, current,ctx)){
        return (-1);
    }


	if ( *(data + *current) != '}')
	{
        char string[11];
        int i;
        for(i=0; i<sizeof(string); i++){
            string[i] = *(data + *current + i);
        }
        string[10] = 0;
        printf("Error: wrong format when finish initialising structure %s:  ( after line %ld) : <%s...> instead of '}'\n",
            node->name, ctx->xml_line, string);
		return (-1);
	}
	*current +=1; //skip }

    if ((node->next) && (*(data + *current) == ',')) {
		*current +=1; //skip ,
    }

	return 0;
}

/* =============================================================================
* write_values_number()
*
* Fills in the values in the nodes from the string
* =========================================================================== */
int write_values_number(    struct tree_node_t* node,
                            char * data,
                            int * current)
{
	int64 value = 0;
	if (!node)
	{
		printf("Null pointer\n");
		return (-1);
	}
    //make number from the string
	if (make_number_from_string(data,current,(int64*)&value))
	{
//		printf("Could not parse value from string\n");
		return (-1);
	}

	// Check whether the read number can fit the size of the variable
	if (((node->type & BASIC_TYPE_MASK)== BASIC_TYPE_UINT8) && ( 0 > value || value > 255))
	{
		printf("Error: node type is uint8, trying to initialise with %lld out of range\n",value);
		return (-1);
	}
	else if (((node->type & BASIC_TYPE_MASK) == BASIC_TYPE_INT8) && (-128 > value || value > 127))
	{
		printf("Error: node type is int8, trying to initialise with %lld out of range\n",value);
		return (-1);
	}
	else if (((node->type & BASIC_TYPE_MASK) == BASIC_TYPE_UINT16) && (0 > value || value > 65536))
	{
		printf("Error: node type is uint16, trying to initialise with %lld out of range\n",value);
		return (-1);
	}
	else if (((node->type & BASIC_TYPE_MASK) == BASIC_TYPE_INT16) && (-32768 > value && value > 32767))
	{
		printf("Error: node type is int16, trying to initialise with %lld out of range\n",value);
		return (-1);
	}
	else if (((node->type & BASIC_TYPE_MASK) == BASIC_TYPE_UINT32) && (0 > value || value > 4294967296ll))
	{
		printf("Error: node type is uint32, trying to initialise with %lld out of range\n",value);
		return (-1);
	}
	else if (((node->type & BASIC_TYPE_MASK) == BASIC_TYPE_INT32) && ( -2147483647 >= value  || value > 2147483647))
	{
		printf("Error: node type is int32, trying to initialise with %lld out of range\n",value);
		return (-1);
	}

	if (node->next != NULL)
		*current +=1; //skip , between numbers


	node->val = osal_malloc(sizeof(int32));
	if (!node->val)
	{
		printf("Error: Not enough memory\n");
		return (-1);
	}
	*node->val = (int32)value;

	return 0;
}

/* =============================================================================
* write_values_array()
*
* Fills in the values in an array from the string
* =========================================================================== */
int write_values_array( struct tree_node_t * node,
                        char * data,
                        int * current)
{
    // determine how dimensional is the array
	int arr_dims;
	uint32 dim[3];
    int act_arr_dims;
	uint32 act_dim[3];
    uint32 dim_right;
    uint32 dim_mid;
    uint32 dim_left;
	uint32 i_left = 0, i_mid = 0, i_right = 0;

	int64 value = 0;
    // get the 3 dimensions
    arr_dims =(int)((node->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);
	dim[0] = node->arr_dim_l;
    dim[1] = node->arr_dim_m;
    dim[2] = node->arr_dim_r;

	if (!node)
	{
		printf("Null pointer\n");
		return (-1);
	}

	// Check for validity of the arrays

	if ( (arr_dims == 3) &&
		(!dim[2] || !dim[1]))
	{
		printf("Array is 3 dimensional with unknown 2nd or 1st dim size\n");
		return (-1);
	}
	if ( (arr_dims == 2) &&
		(dim[2]|| !dim[1]))
	{
		printf("Array is 2 dimensional with known 3rd or unknown 1st dim size\n");
		return (-1);
	}
	if ( (arr_dims == 1) &&
		(dim[1]|| dim[2]))
	{
		printf("Array is 1 dimensional with known 3rd or 2nd dim size\n");
		return (-1);
	}

	act_dim[0] = act_dim[1] = act_dim[2] = 0;
	//  counting the actual array dimensions
	count_elements_in_3_dimensional_array(data+*current,act_dim);

    act_arr_dims = 0;

    if(act_dim[0]) act_arr_dims++;
    if(act_dim[1]) act_arr_dims++;
    if(act_dim[2]) act_arr_dims++;

    if( (arr_dims != act_arr_dims)                        ||
        ((dim[0]) && (dim[0] != act_dim[0]))              ||
        ((arr_dims > 1) && (dim[1] != act_dim[1]))    ||
        ((arr_dims > 2) && (dim[2] != act_dim[2]))    )
    {
        char act_type[TYPE_FIELD_LENGTH+7*3+1];
        char node_type[TYPE_FIELD_LENGTH+7*3+1];
        uint16 act;
        uint32 act_arr_dim_l;
        uint32 act_arr_dim_m;
        uint32 act_arr_dim_r;

        act = (node->type & BASIC_TYPE_MASK) |
            ((uint16)act_arr_dims << BASIC_TYPE_ARRAY_DIMS_OFST);

        act_arr_dim_l = act_dim[0];
        act_arr_dim_m = act_dim[1];
        act_arr_dim_r = act_dim[2];
        type_to_string(node->type, node->arr_dim_l, node->arr_dim_m, node->arr_dim_r, node_type);
        type_to_string(act, act_arr_dim_l, act_arr_dim_m, act_arr_dim_r, act_type);

		printf("Error: array %s of type %s can not be initialize by array of type (%s)\n",
            node->name, node_type, act_type);
		return (-1);
    }


    // if there is an unknown size count each dimension elements
	if (dim[0] == 0)//|| !dim[1] || !dim[2])
	{
		dim[0] = act_dim[0];
        // set the new sizes after they have been determined
		node->arr_dim_l = dim[0];
	}

    switch (arr_dims){
    case 3: //if (dim[2] && dim[1]&& dim[0])
        //3D array
		node->val = osal_malloc(dim[2]*dim[1]*dim[0]*sizeof(int32));
        dim_left  = dim[0];
        dim_mid   = dim[1];
        dim_right = dim[2];
	    break;
    case 2:  //else if (!dim[2] && dim[1] && dim[0])
        //2D array
		node->val = osal_malloc(dim[1]*dim[0]*sizeof(int32));
        dim_left  = 0;
        dim_mid   = dim[0];
        dim_right = dim[1];
	    break;
    case 1:  //else if (dim[2] == 0 && dim[1] == 0 && dim[0])
        //1D array
		node->val = osal_malloc(dim[0]*sizeof(int32));
        dim_left  = 0;
        dim_mid   = 0;
        dim_right = dim[0];
	    break;
    default :  //else
		printf("Could not resolve array dimensions\n");
		return (-1);
    }

	if (!node->val)
	{
		printf("ERROR: cannot allocate memory for value\n");
		return (-1);
	}

	//fill values

	if (dim_left)
	{
        //3D array
		if ( *(data + *current) != '{')
		{
			printf("Error when initialising array!\n");
			return (-1);
		}
		*current +=1; //skip {
	}

    i_left = 0;
    do { //}while (i_left < dim_left);

        if (dim_mid){
            if ( *(data + *current) != '{')
			{
				printf("Error when initialising array!\n");
				return (-1);
			}
			*current +=1; //skip {
        }
		i_mid = 0;
        do { //}while (i_mid < dim_mid)
            if (dim_right){
                if ( *(data + *current) != '{')
				{
					printf("Error when initialising array!\n");
					return (-1);
				}
				*current +=1; //skip {
            }
			i_right = 0;

			while (i_right < dim_right)
			{
                if (make_number_from_string(data,current,(int64*)&value)){
					return (-1);
                }

				if (i_right != (dim_right - 1))
				{
					*current +=1; // skip , between numbers
				}
				*(node->val + (dim_mid*dim_right)*i_left +
                                       dim_right *i_mid  +
                                                  i_right) = (uint32)value;

				++i_right;
			}

			if (dim_right)
			{
				if ( *(data + *current) != '}')
				{
					printf("Error when initialising array!\n");
					return (-1);
				}
				*current +=1; //skip }
			}

            if (dim_mid){
				if ((i_mid != (dim_mid - 1)) && (*(data + *current) == ','))
				{
					*current +=1; //skip ,
				}
            }

            i_mid++;
        } while (i_mid < dim_mid);

        if (dim_mid)
		{
			if ( *(data + *current) != '}')
			{
				printf("Error when initialising array!\n");
				return (-1);
			}
			*current +=1; //skip }
		}
        if (dim_left)
		{
			if ((i_left != (dim_left - 1)) && (*(data + *current) == ','))
			{
				*current +=1;  //skip ,
			}
		}
        i_left++;
    } while (i_left < dim_left);

	if (dim_left)
	{
		if ( *(data + *current) != '}')
		{
			printf("Error when initialising array!\n");
			return (-1);
		}
		*current +=1; //skip {
	}

    if ((node->next != NULL) && (*(data + *current) == ',')){
		*current +=1; //skip ,
    }

	return 0;

}

/* ============================================================================
* parse_main_type()
*
* Parses the sring of main type name from XML to produce name of main_type_t
* ========================================================================== */
int parse_main_type(char* str)
//TODO : remove "main" attribute from "typedef". The main structure must be anyway marked in fill values
{
	if (strcmp(str,MAIN_TYPE_IDENTIFIERS_GENERAL) == 0)
	{
		return MAIN_TYPE_GENERAL;
	}
	else if(strcmp(str,MAIN_TYPE_IDENTIFIERS_USECASE) == 0)
	{
		return MAIN_TYPE_USECASE;
	}
	else if (strcmp(str,MAIN_TYPE_IDENTIFIERS_PARPACK) == 0)
	{
		return MAIN_TYPE_PARPACK;
	}

	return MAIN_TYPE_UNKNOWN;
}

/* ============================================================================
* parse_type_array()
*
* Parse the size of the array if it is present and its type
* ========================================================================== */
void parse_type_array(const uint16 mask, int8 * array_attrbs,
                        uint16* type,
                        uint32* dim_l, uint32* dim_m, uint32* dim_r)
{
	int8 * ptr = array_attrbs;
	int8 * bracket = NULL;
	char temp[8];

	uint32 dim[3];
	int16 numdims = 0;
	uint32 size = 0;
	int32 size_len = 0;
    uint16 r_type = 0;
    uint32 r_dim_l = 0;
    uint32 r_dim_m = 0;
    uint32 r_dim_r = 0;

	dim[0] = 0;
	dim[1] = 0;
	dim[2] = 0;


	while ( *ptr != '\0')
	{
		if ( *ptr == '[')
		{
			if (!bracket && !size_len ) {
				bracket = ptr;
			}
			else {
				printf("Error: wrong format when declaring array %s\n", array_attrbs);
				dtp_gen_abort(1);
			}
		}
		else if (*ptr == ']')
		{
			if (bracket)
			{
				++bracket;
				strncpy(temp,bracket, size_len);
				temp[size_len] = '\0';
				size = atoi(bracket);
				bracket = NULL;
				size_len = 0;


				if (numdims > 2)
				{
					printf("Error: wrong format when declaring array %s\n", array_attrbs);
					dtp_gen_abort(1);
				}
				dim[numdims] = size;
				++numdims;
			}
			else
			{
				printf("Error: wrong format when declaring array %s\n", array_attrbs);
				dtp_gen_abort(1);
			}
		}
		else if ( *ptr == '*' )
		{
			if (size_len)
			{
				printf("Error: wrong format when declaring array %s\n", array_attrbs);
				dtp_gen_abort(1);
			}
			else
			{
                //TODO: * may be combined with [..][..]. Maintain it. Here only 1D array may be defined using *
				r_type |= BASIC_TYPE_ARRAY_UNDEF;
                r_type |= mask | (1<<BASIC_TYPE_ARRAY_DIMS_OFST);
				++size_len;
			}
		}
		else if ( ('0' <= *ptr) && (*ptr <= '9' ))
		{
			if (bracket)
			{
				++size_len;
			}
			else
			{
				printf("Error: wrong format when declaring array %s\n", array_attrbs);
				dtp_gen_abort(1);
			}
		}
		else
		{
			printf("Error: wrong format when declaring array %s\n", array_attrbs);
			dtp_gen_abort(1);
		}
		++ptr;
	}

	if (r_type)		// we have spotted '*' character
	{
		if (numdims != 0) {		// we have [..]
			printf("Error: wrong format when declaring array %s\n", array_attrbs);
			dtp_gen_abort(1);
		}
	}
	else
	{
		r_type = mask | (numdims << BASIC_TYPE_ARRAY_DIMS_OFST);
        if( numdims && (dim[0] == 0) ){
           r_type |= BASIC_TYPE_ARRAY_UNDEF;
        }
		r_dim_l = dim[0];
		r_dim_m = dim[1];
		r_dim_r = dim[2];

	}

    if( ((numdims > 1) && (dim[1] == 0))  ||
        ((numdims > 2) && (dim[2] == 0))  ){

		printf("Error: wrong format when declaring array %s\n", array_attrbs);
		dtp_gen_abort(1);
    }
    *type  = r_type;
    *dim_l = r_dim_l;
    *dim_m = r_dim_m;
    *dim_r = r_dim_r;
}

/* ============================================================================
* parse_type()
*
* Parses the sring of type name from XML to produce name of basic_type_t
* Also recognizes whether the type is an array and fills array dimensions sizes
* if "type_name" is an array or pointer of non-basic type this func changes
* the type_name removing leading '*' and/or trailing []
* If the type is array the func checks validity of array dimensions - only the
* most-significant could be not-defined (empty [])
* ========================================================================== */
void parse_type(char* type_name,
                  uint16* type,
                  uint32* dim_l, uint32* dim_m, uint32* dim_r)
{

    uint16 r_type = 0;
    uint32 r_dim_l = 0;
    uint32 r_dim_m = 0;
    uint32 r_dim_r = 0;

	if (strncmp(type_name,BASIC_IDENTIFIERS_int8,4) == 0)
	{
		parse_type_array(BASIC_TYPE_INT8,&type_name[4],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[4]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_uint8,5) == 0)
	{
		parse_type_array(BASIC_TYPE_UINT8,&type_name[5],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[5]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_int16,5) == 0)
	{
		parse_type_array(BASIC_TYPE_INT16,&type_name[5],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[5]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_uint16,6) == 0)
	{
		parse_type_array(BASIC_TYPE_UINT16,&type_name[6],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[6]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_int32,5) == 0)
	{
		parse_type_array(BASIC_TYPE_INT32,&type_name[5],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[5]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_uint32,6) == 0)
	{
		parse_type_array(BASIC_TYPE_UINT32,&type_name[6],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[6]='\0';
	}
	else if (strncmp(type_name,BASIC_IDENTIFIERS_STRUCT,6) == 0)
	{
		parse_type_array(BASIC_TYPE_STRUCT,&type_name[6],
                        &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[6]='\0';
	}

    *type  = r_type  ;
    *dim_l = r_dim_l ;
    *dim_m = r_dim_m ;
    *dim_r = r_dim_r ;
}

/* ============================================================================
* parse_unknown_type()
*
* Recognizes whether the type is an array and fills array dimensions sizes
* if "type_name" is an array or pointer of non-basic type this func changes
* the type_name removing leading '*' and/or trailing []
* ========================================================================== */
void parse_unknown_type(char * type_name,
                        uint16* type,
                        uint32* dim_l, uint32* dim_m, uint32* dim_r)
{
	int length = strlen(type_name)-1;
    int in_brackets = 0;
    uint16 r_type = 0;
    uint32 r_dim_l = 0;
    uint32 r_dim_m = 0;
    uint32 r_dim_r = 0;



    while (length >= 0)
    {
        if(type_name[length] =='['){
            in_brackets = 0;
        }
        if(in_brackets){
            if((type_name[length] < '0')  || (type_name[length] > '9')){
                printf("%s : Only digits are allowed to exist within []\n",type_name);
                dtp_gen_abort(1);
            }

        }
        if(type_name[length] ==']'){
            in_brackets = 1;
        }
	    if ((type_name[length] !=']') &&
		    (type_name[length] !='[') &&
    		(type_name[length] !='*') &&
		    (in_brackets == 0)
            ){
            break;
        }
		length--;
    }
    if(length == 0){
        printf("wrong type format : %s\n",type_name);
        dtp_gen_abort(1);
    }

/*
old version : problem = if the typename end with a number(before any * or []) DCC gen reports error

	while (type_name[length] ==']' ||
		type_name[length] =='[' ||
		type_name[length] =='*'
//		(type_name[length] >= '0'  && type_name[length] <= '9') //there may be numbers in a type name
        )
		length--;
*/

	if (length != strlen(type_name) -1)
	{
		parse_type_array(BASIC_TYPE_NO,&type_name[length+1], 
                            &r_type, &r_dim_l, &r_dim_m, &r_dim_r);
		type_name[length+1] = '\0';
	}

    *type  = r_type  ;
    *dim_l = r_dim_l ;
    *dim_m = r_dim_m ;
    *dim_r = r_dim_r ;
}

int extract_dcc_name(char** dcc_name, char* in_str)
{
    char* p = NULL;
    char* name = NULL;
    int len;
    //clear white spaces in the beginning an din the end of in_str. check for not-allowed characters.
    p = in_str;
    //skip starting white spaces
    while (*p){
        if( ! is_white_space(*p) ){
            name = p;
            break;
        }
        p++;
    };

    len = 0;
    while (*p){
        if (is_name_end_char(*p)) {
            *p = 0; //place terminating 0
            break;
        }
        if( is_not_alllowed_char(*p) || (*p < 0x20) || (*p > 0x7E) ){
            printf("DCC name containes no-allowed character: '%c', code 0x%X\n", (char)*p, *p);
            return (-1);
        }
        if(len > MAX_DCC_FILE_NAME_LEN){
            printf("DCC name should be < %d characters\n", MAX_DCC_FILE_NAME_LEN);
            return (-1);
        }
        p++;
        len++;
    };
    len = strlen(name)+1;
    *dcc_name = osal_malloc(len);
    if(*dcc_name == NULL){
        printf("Can not allocate memory\n");
        return (-1);
    }
    strcpy(*dcc_name,name);

    tls_clear_ending_white_space(dcc_name,&len);

    return 0;
}

/* ============================================================================
* read_file_in_data_str()
*
* Inserts contents of a text file in the ctx->data_str startin at position
* ins_location. The rest of data_str is shifted, not replaced.
* Updates ctx->data_str_len.
*
* @param ctx : pointer to DCC generator context vars
* @param file_name : file to be inserted
* @param ins_location : pinter to where to insert the file
* ========================================================================== */
int read_file_in_data_str(dtp_gen_ctx_t* ctx, char* file_name, char* ins_location)
{
    FILE* inc_file;
	long size;
    uint32 ins_offset;
    char* p;
    char* dst;
    int err;

    ins_offset = (uint32)ins_location - (uint32)ctx->data_str;

    inc_file = fopen(file_name,"rb");
    if(inc_file == NULL){
        printf("error reading file %s\n", file_name);
        return (-1);
    }
    //get file size;
    err = 0;
	err |= fseek(inc_file,0,SEEK_END);
	size = ftell(inc_file);
	err |= fseek(inc_file,0,SEEK_SET);
    if(err){
        printf("error reading file %s\n", file_name);
        fclose(inc_file);
        return (-1);
    }
    if(size > MAX_INCLUDE_FILE_SIZE){
        printf("ERROR:Included file %s is too large (%ld bytes)\n", file_name, size);
        printf("      Max allowed size is %d bytes\n", MAX_INCLUDE_FILE_SIZE);
        fclose(inc_file);
        return (-1);
    }

    while((ctx->data_str_len + size +1) > ctx->data_str_len_max)
    {
        //data string buffer is full, alloc mem for larger data string
        // the buffer is considered full when size-2 is reached to leave space for 1 ' ' and a terminating 0
        expand_data_str(ctx);
    }
    ins_location = ctx->data_str + ins_offset;

    //shift the characters on and after ins_location to make room for inserting the file
    p = ctx->data_str + ctx->data_str_len;
    dst = ctx->data_str + ctx->data_str_len + size;
    while ((uint32)p >= (uint32)ins_location)    {
        *dst-- = *p--;
    }
    ctx->data_str_len += size;

    //read the file onto the just freed space
    if( fread(ins_location, 1, size, inc_file) != size){
        printf("error reading file %s\n", file_name);
        fclose(inc_file);
    }

    fclose(inc_file);
    return 0;
}

/* ============================================================================
* expand_data_str()
*
* If data_sr buffer can not take next that just came, new larger buffer have
* to be allocated
* ========================================================================== */
void expand_data_str(dtp_gen_ctx_t* ctx)
{
    char* new_data_str = NULL;

    new_data_str = osal_malloc(ctx->data_str_len_max + DATA_STR_LEN_CHUNK);
    if(new_data_str == NULL) {
        printf("MEM alloc error, size %d\n", DATA_STR_LEN_CHUNK);
        dtp_gen_abort(1);
    }

    if(ctx->data_str){
        //if there is data in data_str, copy it into the new larger buffer
        memcpy( new_data_str, ctx->data_str, ctx->data_str_len_max);
        //release old buffer
        osal_free(ctx->data_str);
		ctx->data_str = NULL;

    }
    ctx->data_str = new_data_str;
    ctx->data_str_len_max += DATA_STR_LEN_CHUNK;
}

void cat_tree_to_str(    struct tree_node_t * source,
                            char **str_in , int *size)
{
    char type[20];
    char* string = *str_in;
    char* new_str = NULL;
    uint64 type_num,type_mask;
    uint16 dim1_mask = 0xFFFF;
    //int err = 0;

    type_mask = 0;

    if(!source || !string){
        return;
    }

    //little patch to assure ssame CRCs of 1.6 and 1.7 (extending array dims to
    //32bit in 1.7). If an array dimension is > 65535, only its 16LSBs will
    //take part in the CRC, but this is secure enough in practice
    type_num = (uint64)source->type |
        ((uint64)(source->arr_dim_l & 0xFFFF) << 16) |
        ((uint64)(source->arr_dim_m & 0xFFFF) << 32) |
        ((uint64)(source->arr_dim_r & 0xFFFF) << 48) ;

    if((int)strlen(string) > (*size - (NAME_FIELD_LENGTH+TYPE_FIELD_LENGTH+20)))
    {
        new_str = (char*) osal_malloc(*size + CRC_STR_LEN);
        if(!new_str){
            printf("Error: while extending memory for crc string.\n");
            osal_free(string);
            dtp_gen_abort(1);
        }
        *size = *size + CRC_STR_LEN;
        strcpy(new_str,string);
        osal_free(string);
        string = new_str;
        *str_in = string;
    }

    if((type_num & BASIC_TYPE_ARRAY_UNDEF)){
        //if undefined size - replace the left dimension size with 0
        type_mask |= (uint64)dim1_mask << 16;
//        type_mask = ~type_mask;
//        type_num &= type_mask;
        //fill undefined size with FFFF to generate difference in CRC
        //DCC Bin files containing undefined arrays, converted with 1.7 and on
        //will be incompatible with older versions due to 4 bytes used for
        //array size (previously 2). Thus CRC must change.
        type_num |= type_mask;
    }
    sprintf(type, "%llu", type_num);

//    strcat(string,source->name);
    strcat(string,type);
    if( (type_num & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT ){
        struct tree_node_t * arr_elem;
        //TODO: skip redundant child arrays in case of multi-dim array of structs
        strcat(string,(char*)source->val);

        arr_elem = source;
        while (arr_elem->type & BASIC_TYPE_ARRAY) {
            if(arr_elem->child == NULL){
                printf("Array of structs has no children!\n");
                return;
            }
            arr_elem = arr_elem->child;
        }
        //If 'source' is an arry of structs, now arr_elem is a structure with
        //   fields in arr_elem->child
        //If 'source' is just a struct, arr_elem == source
        cat_tree_to_str(arr_elem->child,str_in,size);
        //struct also? If size is undefined we don't want to take into account
    } else {
        strcat(string,source->name);
    }

//    if(source->child){
//        cat_tree_to_str(source->child,string,size);
//    }
    if(source->next){
        cat_tree_to_str(source->next,str_in,size);
    }

    return;
}

uint32 calc_crc(int i)
{
    int j;
    uint32 CRC;
    CRC = i;
    for (j=8;j>0;j--)
    {
        if (CRC & 1) {
            CRC = (CRC >> 1) ^ CRC_GENERATOR;
        }
        else{
            CRC >>= 1;
        }
    }
    return CRC;
}


void generate_crc( uint32 *crc_in, struct tree_node_t* tree)
{
    uint32 temp1,temp2;
    int size = CRC_STR_LEN;
    uint32 CRC = *crc_in;
    int len;
    char* string=NULL;
    char* string_buf=NULL;

    string = osal_malloc(size);
    if(!string){
        printf("ERROR with malloc at CRC generation\n");
        dtp_gen_abort(1);
    }
    memset(string,0x00,size);
    *string = '\0';

    cat_tree_to_str(tree,&string,&size);

    len=strlen(string);
    string_buf = string;

    while (len-- != 0)
    {
        temp1 = ( CRC >> 8 ) & 0x00FFFFFF;
        temp2 = calc_crc( ((int)CRC^*string_buf++) &0xff);
        CRC = temp1 ^ temp2;
    }

    osal_free(string);
    *crc_in = CRC;
    //printf("\nCRC: \t0x%lX\n",CRC);
}

void write_indent(FILE* fout, int indent)
{
#ifdef DEVEL
    int i;
    for(i=0; i<indent; i++) {
        fprintf(fout, "    ");
    }
#else
#ifdef DCCREAD
    int i;
    for(i=0; i<indent; i++) {
        fprintf(fout, "    ");
    }

#endif
#endif
}
void expand_parser_free_str(dtp_gen_ctx_t* ctx, uint32 len)
{
    uint32 new_size;
    char* new_str;

    new_size = ctx->parser_free_str_len_max + ((int)(len/NAME_FIELD_LENGTH) + 1) * NAME_FIELD_LENGTH;
    new_str = osal_malloc(new_size);
    if(new_str == NULL){
        printf("Can not allocate %ld bytes mamory\n", new_size);
        dtp_gen_abort(1);
    }

    //copy existing data to the new location
    memcpy(new_str, ctx->parser_free_str, ctx->parser_free_str_len_max);
    //free exicting location
    osal_free(ctx->parser_free_str);

    ctx->parser_free_str_len_max = new_size;
    ctx->parser_free_str = new_str;
}


#ifdef MEMORY_LEAK_DEBUG
void *osal_malloc_debug(    int aSize,
                            const char * aFuncName,
                            int aLine)
{
//	int current_index = 0;
	void *ptr = NULL;
    int i;
//printf("size: %x",aSize);
	if (first_call == 1)
	{
        memset(mem_deb_arr,0, sizeof(mem_deb_arr));
//        memset(mem_deb_arr_file,0, sizeof(mem_deb_arr_file));
//        memset(mem_deb_arr_line,0, sizeof(mem_deb_arr_line));
		first_call = 0;
	}
	ptr = malloc(aSize);
    if(ptr == NULL){
        return NULL;
    }
    total_alloc+=aSize;
	ptr = memset(ptr,0xff,aSize);

    alloc_calls++;

    //search free index
    for(i=0; i<MAX_DEB_ARR_LEN; i++){
        if(mem_deb_arr[i][0] == 0){
	        mem_deb_arr[i][0] = (int)ptr;
	        mem_deb_arr[i][1] = aSize;
//	        strcpy(mem_deb_arr_file[i], aFuncName);
//          mem_deb_arr_line[i] = aLine;
            break;
        }
    }
    if(i==MAX_DEB_ARR_LEN){
        debug_array_overflow = 1;
    }
//	printf("start: %x, end : %x\n",(uint32)ptr,(uint32)((uint8*)ptr+aSize) );
	return ptr;
}

void osal_free_debug(void *ptr)
{
	int i = 0;
    free_calls++;
	for(i = 0; i < MAX_DEB_ARR_LEN; i++)
	{
		if((int)ptr == mem_deb_arr[i][0])
		{
            total_free += mem_deb_arr[i][1];
			mem_deb_arr[i][0] = mem_deb_arr[i][1] = 0;
//			mem_deb_arr_file[i][0] = '\0';
//			mem_deb_arr_line[i] = 0;
            break;
		}
	}
    if(i==MAX_DEB_ARR_LEN){
        if(debug_array_overflow == 0){
            printf("Trying to free not-allocated memory\n");
            //while(1);
        }
#ifndef __LINUX__
        else {
            uint32 size;
            //Windows only : size of allocated block in the heep
            size = *((uint32*)((uint8*)ptr - 0x10));
            total_free += size;
        }
#endif
    }
}

void mem_deb_print()
{
    unsigned memory_size = 0;
	int current_index = 0;
	printf("\n\nMEMORY_LEAK_DEBUG:\n");
	for(current_index = 0; current_index < MAX_DEB_ARR_LEN; current_index++)
	{
		if(mem_deb_arr[current_index][0])
		{
            memory_size += mem_deb_arr[current_index][1];
			printf("mem_deb_arr[%d]=0x%x size=%d",
                        current_index,
                        mem_deb_arr[current_index][0],
                        mem_deb_arr[current_index][1]);
//			printf(", file=%s, line=%d", mem_deb_arr_file[current_index], mem_deb_arr_line[current_index]);
            printf("\n");
		}
	}
	printf("total_alloc: %d\n", total_alloc);
	printf("total_free: %d\n", total_free);
#ifdef __LINUX__
    if(debug_array_overflow){
        printf("[Warning] Memory debug array too small (%d).\n",MAX_DEB_ARR_LEN );
        printf("\t*Increase MAX_DEB_ARR_LEN in order to have accurate debug information.\n");
    }
#endif

	printf("Memory leak size: %d\n", memory_size);
	printf("Memory allocation calls: %d\n", alloc_calls);
	printf("Memory deallocation calls: %d\n", free_calls);
    printf("=============\n");
	first_call = 1;
}
#endif
