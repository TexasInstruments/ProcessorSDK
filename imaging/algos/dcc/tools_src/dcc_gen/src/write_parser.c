/*****************************************************************
 * tools.c
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"

#define INDENT_STR  "    "


int prs_parse_fields(dtp_gen_ctx_t* ctx, struct tree_node_t* tree, char* prefix, int indent, uint16* used_simple_types);

void free_strcat (dtp_gen_ctx_t* ctx, char* str)
{
    if( (strlen(ctx->parser_free_str) + strlen(str) + 1) > ctx->parser_free_str_len_max ){
        expand_parser_free_str(ctx, strlen(str) + 1);
    }
    strcat(ctx->parser_free_str, str);
}
/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */
int add_to_free_list(dtp_gen_ctx_t* ctx, char* free_var)
{

    //TODO: Fix free function so that it can correctly free allocated memory in
    //nested structures and substructures (adding loops)
    struct free_list_t* last_elem;
    struct free_list_t* new_elem;

    new_elem = osal_malloc(sizeof(struct free_list_t));
    if(new_elem == NULL){
        printf("Can not allocate mamory\n");
        return (-1);
    }
    new_elem->free_var = osal_malloc(strlen(free_var)+1);
    if(new_elem->free_var == NULL){
        printf("Can not allocate mamory\n");
        osal_free(new_elem);
        return (-1);
    }
    strcpy(new_elem->free_var, free_var);
    new_elem->next = NULL;
    new_elem->prev = NULL;

    if(ctx->prs_free_list){
        last_elem = ctx->prs_free_list;
        while (last_elem->next){
            last_elem = last_elem->next;
        }
        last_elem->next = new_elem;
        new_elem->prev = last_elem;
    } else {
        ctx->prs_free_list = new_elem;
    }

    return (0);
}


/* ============================================================================
* prs_print_indent()
*
* writes indeting offset in the parser file
* @param indent : size of indeting offset in terms of single indenting width
* ========================================================================== */
void prs_print_indent(dtp_gen_ctx_t* ctx, int indent)
{
    int i;
    for(i=0; i< indent; i++){
        fprintf(ctx->prs_fout_txt,"%s", INDENT_STR);
    }
}
/* ============================================================================
* prs_parse_array()
*
* write automatic parser for node "tree" which is an array
* @param tree : the node to be parsed
* @param prefixt : preceding part name of the name of that node
*    i.e. tree is a simple integer node named "gain[]", prefix is
*    "sys_prm->fielld1[5].modes"
*    The full variable name accessing this field is
*    sys_prm->fielld1[5].modes.gain[..]
* @param indent : indent offset in the parser C file.
* ========================================================================== */
int prs_parse_array(dtp_gen_ctx_t* ctx, struct tree_node_t* tree, char* prefix, int indent, uint16* used_simple_types)
{
    uint16 arr_dims;
    uint32 dim_l;
    uint32 dim_m;
    uint32 dim_r;
    int free_size = 0, name_size = 0;
    char* undef_arr_size = NULL;
    char field_type_name[TYPE_FIELD_LENGTH];
    char* field_name = NULL;
    char* ind_str;
    int i;
    //%d * %d * %d - 10digits per %d, 5 just in case
    char arr_size_str[3*10 + 6 + 1 + 5];

    //prepare indenting string (spaces in the beginning of each line)
    ind_str = osal_malloc(indent * strlen(INDENT_STR) + 1);
    if(ind_str == NULL){
       printf("Can not allocate memory\n") ;
       return (-1);
    }
    ind_str[0] = '\0';
    for(i=0; i < indent; i++){
        strcat(ind_str, INDENT_STR);
    }

    arr_dims = (uint16)((tree->type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST);
	dim_l = tree->arr_dim_l;
	dim_m = tree->arr_dim_m;
	dim_r = tree->arr_dim_r;
/*allow NULL for undefined arrays
    if(dim_l == 0) {
       printf("Array with undefined size after filling values\n") ;
       osal_free(ind_str);
       return (-1);
    }
*/
    if(tree->type & BASIC_TYPE_ARRAY_UNDEF){
        //The array size is undefined in typedef. dim_l, however, contains the
        //values determined when filling the array - it should be ignored
        dim_l = 1;
    }
    if(dim_m == 0) dim_m = 1;
    if(dim_r == 0) dim_r = 1;

    sprintf(arr_size_str, "%d", dim_l);
    if(arr_dims == 2){
        sprintf(arr_size_str, "%d * %d", dim_l, dim_m);
    }
    if(arr_dims == 3){
        sprintf(arr_size_str, "%d * %d * %d", dim_l, dim_m, dim_r);
    }

    if((tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
    {
        field_name = osal_malloc(strlen(prefix) + strlen((char*)tree->val) + 1);
        if(field_name == NULL){
           printf("Can not allocate memory\n") ;
           osal_free(ind_str);
           return (-1);
        }
        strcpy(field_name, prefix);
        strcat(field_name, (char*)tree->val);
        free_strcat(ctx,(char*)tree->val);
        name_size = strlen((char*)tree->val);
        strcpy(field_type_name, (char*)tree->name);
    } else {
        field_name = osal_malloc(strlen(prefix) + strlen((char*)tree->name) + 1);
        if(field_name == NULL){
           printf("Can not allocate memory\n") ;
           osal_free(ind_str);
           return (-1);
        }
        strcpy(field_name, prefix);
        strcat(field_name, (char*)tree->name);
        free_strcat(ctx,(char*)tree->name);
        name_size = strlen((char*)tree->name);
        strcpy(field_type_name, BASIC_TYPES_NAMES[(tree->type & BASIC_TYPE_MASK)]);
        *used_simple_types |= 1 << (tree->type & BASIC_TYPE_MASK);
    }
    free_size += name_size;

    fprintf(ctx->prs_fout_txt,"%s//extract %s array\n", ind_str, field_name);
    fprintf(ctx->prs_fout_txt,"%s{\n",ind_str);
    fprintf(ctx->prs_fout_txt,"%s    uint32 arr_size;\n",ind_str);
    fprintf(ctx->prs_fout_txt,"%s    uint32 cnt;\n",ind_str);
    fprintf(ctx->prs_fout_txt,"%s    %s* elem_ptr%d;\n",ind_str,field_type_name,indent);
    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"%s    arr_size = %s;\n",ind_str, arr_size_str);

    if(tree->type & BASIC_TYPE_ARRAY_UNDEF)
    {
        char ptr_type_name[TYPE_FIELD_LENGTH];
        char *str;
        strcpy(ptr_type_name, field_type_name);
        str = ptr_type_name + strlen(ptr_type_name);
        if(arr_dims == 1){
            *str++ = '*';
            *str++ = 0;
        } else {
            sprintf(str, "(*)[%d]", dim_m);
            str = str + strlen(str);
            if(arr_dims == 3){
                sprintf(str, "[%d]", dim_r);
                str = str + strlen(str);
            }
        }

        *used_simple_types |= 1 << BASIC_TYPE_UINT32;
        fprintf(ctx->prs_fout_txt,"\n");
        fprintf(ctx->prs_fout_txt,"%s    //if variable array size, undefined array dimension (the left one) is written in the first 2 bytes\n",ind_str);
        fprintf(ctx->prs_fout_txt,"%s    arr_size *= (uint32)get_uint32(&p_bin);\n",ind_str);
        fprintf(ctx->prs_fout_txt,"%s    %s%s = arr_size;\n",ind_str,field_name,UNDEF_ARRAY_SIZE_POSTFIX);
        fprintf(ctx->prs_fout_txt,"%s    if(arr_size == 0){\n",ind_str);
        fprintf(ctx->prs_fout_txt,"%s        %s = NULL;\n",ind_str, field_name);
        fprintf(ctx->prs_fout_txt,"%s    } else {\n",ind_str);
        fprintf(ctx->prs_fout_txt,"%s        elem_ptr%d = (%s*)dcc_prs_malloc(arr_size * sizeof(%s));\n",ind_str, indent, field_type_name,field_type_name);
        fprintf(ctx->prs_fout_txt,"%s        if(elem_ptr%d == NULL){\n",ind_str, indent);
        fprintf(ctx->prs_fout_txt,"%s            return (%d);\n",ind_str,AUTO_PARSER_RETURN_ERROR_ALLOC);
        fprintf(ctx->prs_fout_txt,"%s        }\n",ind_str);
        fprintf(ctx->prs_fout_txt,"%s        %s = (%s)elem_ptr%d;\n",ind_str, field_name, ptr_type_name, indent);
        fprintf(ctx->prs_fout_txt,"%s    }\n",ind_str);
        fprintf(ctx->prs_fout_txt,"\n");

        if( add_to_free_list(ctx, ctx->parser_free_str) ){
            osal_free(ind_str);
            osal_free(field_name);
            return (-1);
        }

        undef_arr_size = osal_malloc(strlen(ctx->parser_free_str) + strlen(UNDEF_ARRAY_SIZE_POSTFIX) + 1);
        if(undef_arr_size == NULL){
            printf("Can not allocate memory\n") ;
            osal_free(ind_str);
            osal_free(field_name);
            return (-1);
        }
        strcpy(undef_arr_size, ctx->parser_free_str);
        strcat(undef_arr_size, UNDEF_ARRAY_SIZE_POSTFIX);

        free_strcat(ctx,"[");
        free_strcat(ctx, undef_arr_size);
        free_strcat(ctx,"]");
        free_size += 2 + strlen(undef_arr_size);

    } else {
        fprintf(ctx->prs_fout_txt,"%s    elem_ptr%d = &%s[0]%s%s;\n",ind_str, indent, field_name, (arr_dims>1)?"[0]":"", (arr_dims>2)?"[0]":"");
        free_strcat(ctx, "[");
        free_strcat(ctx,arr_size_str);
        free_strcat(ctx, "]");
        free_size += 2 + strlen(arr_size_str);
    }
#ifdef DEBUG_PRINT
    fprintf(ctx->prs_fout_txt,"%s    printf_dbg(%s \"%s{ //%s[%%ld]\\n\", arr_size);\n",ind_str,DEBUG_STR,ind_str,field_name);
#endif
    fprintf(ctx->prs_fout_txt,"%s    for(cnt = 0; cnt < arr_size; cnt++)\n",ind_str);
    fprintf(ctx->prs_fout_txt,"%s    {\n",ind_str);

    if((tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT) {
        //if tree is e.g. [2][3] array of structs, it has 2 childs, which are
        //[3] arrays, which have 3 chlids each of a struct type, so go into
        //this tree to fild the array element.
        char elem_ptr_prefix[20];
        struct tree_node_t* elem_tree;
        elem_tree = tree;
        sprintf(elem_ptr_prefix, "elem_ptr%d->", indent);
        free_strcat(ctx,".");
        free_size++;
        while (elem_tree->type & BASIC_TYPE_ARRAY){
            elem_tree = elem_tree->child;
        }
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"%s        printf_dbg(%s\"    %s{ //%s \\n\");\n",ind_str,DEBUG_STR,ind_str,ctx->parser_free_str);
#endif
        prs_parse_fields(ctx, elem_tree->child, elem_ptr_prefix, indent+2, used_simple_types);
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"%s        printf_dbg(%s\"    %s} //%s \\n\");\n",ind_str,DEBUG_STR,ind_str,ctx->parser_free_str);
#endif

    } else {
        fprintf(ctx->prs_fout_txt,"%s        *elem_ptr%d = get_%s(&p_bin);\n",ind_str, indent, BASIC_TYPES_NAMES[(tree->type & BASIC_TYPE_MASK)]);
        *used_simple_types |= 1 << (tree->type & BASIC_TYPE_MASK);

#ifdef DEBUG_PRINT
        if( (strcmp(BASIC_TYPES_NAMES[(tree->type & BASIC_TYPE_MASK)], "uint32") == 0) ||
            (strcmp(BASIC_TYPES_NAMES[(tree->type & BASIC_TYPE_MASK)], "int32") == 0) ){
            fprintf(ctx->prs_fout_txt,"%s        printf_dbg(%s\"    %s%%ld, \\n\",*elem_ptr%d);\n",ind_str,DEBUG_STR,ind_str,indent);
        } else {
            fprintf(ctx->prs_fout_txt,"%s        printf_dbg(%s\"    %s%%d, \\n\",*elem_ptr%d);\n",ind_str,DEBUG_STR,ind_str,indent);
        }
#endif
    }

    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"%s        elem_ptr%d++;\n",ind_str, indent);
    fprintf(ctx->prs_fout_txt,"%s    }\n",ind_str);
#ifdef DEBUG_PRINT
    fprintf(ctx->prs_fout_txt,"%s    printf_dbg(%s\"%s}\\n\");\n",ind_str,DEBUG_STR,ind_str);
#endif
    fprintf(ctx->prs_fout_txt,"%s}\n",ind_str);

    memset(&ctx->parser_free_str[strlen(ctx->parser_free_str)-free_size],0,free_size);

    osal_free(field_name);
    osal_free(ind_str);
    if(undef_arr_size){
        osal_free(undef_arr_size);
    }
    return 0;
}

/* ============================================================================
* prs_parse_fields()
*
* write automatic parser for node "tree"
* @param tree : the node to be parsed
* @param prefixt : preceding part name of the name of that node
*    i.e. tree is a simple integer node named "gain", prefix is
*    "sys_prm->fielld1[5].modes"
*    The full variable name accessing this field is
*    sys_prm->fielld1[5].modes.gain
* @param indent : indent offset in the parser C file.
* ========================================================================== */
int prs_parse_fields(dtp_gen_ctx_t* ctx, struct tree_node_t* tree, char* prefix, int indent, uint16* used_simple_types)
{
    struct tree_node_t* curr;

    curr = tree;
    while (curr)
    {
        if(curr->type & BASIC_TYPE_ARRAY) {
            //this field is a structure
            if( prs_parse_array(ctx, curr, prefix, indent, used_simple_types) ){
                return (-1);
            }

//        } else if( ((curr->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT) &&
//                    (curr->type & BASIC_TYPE_ARRAY_UNDEF) ){
//            //this field is a structure array with undefined count
//            if( prs_parse_array(ctx, curr, prefix, indent, used_simple_types) ){
//                return (-1);
//            }
//

        } else if((curr->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
        {
            //this field is a structure
            char* prefix_new;

            if(curr->child == NULL){
               printf("Structure has not childs!!!\n") ;
               return (-1);
            }
            //allocated mem for newprefix = old_prefix+field)name+'.'
            prefix_new = osal_malloc(strlen(prefix) + strlen((char*)curr->val) + 2);
            if(prefix_new == NULL){
               printf("Can not allocate memory\n") ;
               return (-1);
            }
            strcpy(prefix_new, prefix);
            strcat(prefix_new, (char*)curr->val);
#ifdef DEBUG_PRINT
            prs_print_indent(ctx, indent);
            fprintf(ctx->prs_fout_txt,"printf_dbg(%s\"",DEBUG_STR);
            prs_print_indent(ctx, indent-1);
            fprintf(ctx->prs_fout_txt,"{ //%s\\n\");\n",prefix_new);
#endif
            strcat(prefix_new, ".");
            //We need to add the name of the structure that we're entering to the
            //free string in order to be able to correctly free any memory when
            //needed
            free_strcat(ctx,(char*)curr->val);
            free_strcat(ctx, ".");

            if( prs_parse_fields(ctx, curr->child, prefix_new, indent, used_simple_types) ){
                osal_free(prefix_new);
               return (-1);
            }
#ifdef DEBUG_PRINT
            prs_print_indent(ctx, indent);
            fprintf(ctx->prs_fout_txt,"printf_dbg(%s\"",DEBUG_STR);
            prs_print_indent(ctx, indent-1);
            fprintf(ctx->prs_fout_txt,"} //%s\\n\");\n",prefix_new);
#endif
            //Clear the name of the structure from the free string,
            //since we're finished with parsing it. +1 is for the "."
            memset(&ctx->parser_free_str[strlen(ctx->parser_free_str)-
                                        (strlen((char*)curr->val)+1)],
                    0,
                    (strlen((char*)curr->val) + 1));
            osal_free(prefix_new);
        } else {//simple number
            prs_print_indent(ctx, indent);
            fprintf(ctx->prs_fout_txt,"%s%s = get_%s(&p_bin);\n", prefix, curr->name, BASIC_TYPES_NAMES[(curr->type & BASIC_TYPE_MASK)]);
            *used_simple_types |= 1 << (curr->type & BASIC_TYPE_MASK);

#ifdef DEBUG_PRINT
            if( (strcmp(BASIC_TYPES_NAMES[(curr->type & BASIC_TYPE_MASK)], "uint32") == 0) ||
                (strcmp(BASIC_TYPES_NAMES[(curr->type & BASIC_TYPE_MASK)], "int32") == 0) ){
                prs_print_indent(ctx, indent);
                fprintf(ctx->prs_fout_txt,"printf_dbg(%s\"",DEBUG_STR);
                prs_print_indent(ctx, indent);
                fprintf(ctx->prs_fout_txt,"%%ld, //%s%s \\n\",%s%s);\n",prefix, curr->name,prefix, curr->name);
            } else {
                prs_print_indent(ctx, indent);
                fprintf(ctx->prs_fout_txt,"printf_dbg(%s\"",DEBUG_STR);
                prs_print_indent(ctx, indent);
                fprintf(ctx->prs_fout_txt,"%%d, //%s%s \\n\",%s%s);\n",prefix, curr->name,prefix, curr->name);
            }
#endif
        }
        curr = curr->next;
    }

    return 0;
}

int replace_in_str(char* str,char find,char replace)
{
    uint16 x = 0;

    while (str[x]) {

        if ( str[x] == find ) {
            str[x] = replace;
        }

        x++;
    }
    if(x){
        return 0;
    } else {
        return 1;
    }
}

void write_free_string(FILE* fout_txt, char* free_var,int indent)
{
    int c           = 0,
        start_count = 0,
        end_count   = 0,
        in_count    = 0,
        count       = 0,
        indent_cnt  = 0;

    char    *start      = NULL,
            *end        = NULL,
            *count_str  = NULL,
            *indent_str = NULL,
            *var_str    = NULL;

    char write_counter[15];
    char ind_num[10];

    sprintf(ind_num,"%d",indent);
    strcpy(write_counter,"count");
    strcat(write_counter,ind_num);

    indent_cnt = indent * strlen(INDENT_STR);
    indent_str = osal_malloc(indent_cnt + 1);
    memset(indent_str,0,indent_cnt);
    for(c=0;c<indent;c++){
        strcat(indent_str,INDENT_STR);
    }

    start = strchr(free_var, '[');

    if(start){
        //We are looking at array
        start++;
        end = strchr(free_var, ']');

        start_count = strlen(start);
        end_count = strlen(end);

        count = start_count - end_count;

        count_str = osal_malloc(start_count + 1);
        strcpy(count_str, start);
        memset(&count_str[count],0,end_count);

        var_str = osal_malloc(strlen(free_var)+strlen(write_counter)+strlen(end)+2);

        strcpy(var_str,free_var);
        in_count = strlen(var_str);
        memset(&var_str[in_count-(start_count+1)],0,(start_count+1));

        strcat(var_str,"[");
        strcat(var_str,write_counter);
        strcat(var_str,end);

        fprintf(fout_txt,"%s{\n",indent_str);
        fprintf(fout_txt,"%s    uint32 %s;\n",indent_str,write_counter);
        fprintf(fout_txt,"%s    for(%s = 0; %s < %s; %s++){\n",indent_str,
                                                            write_counter,
                                                            write_counter,
                                                            count_str,
                                                            write_counter);

        replace_in_str(var_str,'[','{');
        replace_in_str(var_str,']','}');

        write_free_string(fout_txt, var_str, indent+2);

        fprintf(fout_txt,"%s    }\n",indent_str);
        fprintf(fout_txt,"%s}\n",indent_str);
        fprintf(fout_txt,"\n");
        osal_free(count_str);
        osal_free(var_str);

    } else {
        //simple free
        replace_in_str(free_var,'{','[');
        replace_in_str(free_var,'}',']');
        fprintf(fout_txt,"%sif(%s){\n",indent_str,free_var);
        fprintf(fout_txt,"%s    dcc_prs_free(%s);\n",indent_str,free_var);
        fprintf(fout_txt,"%s}\n",indent_str);
        fprintf(fout_txt,"\n");
    }
    osal_free(indent_str);
}

/* ============================================================================
* write_parser()
*
* Writes DCC values and structure in a text file
* ========================================================================== */
int write_parser(dtp_gen_ctx_t* ctx)
{
    char* sys_gen_t;
    char* uc_gen_t;
    char* ppclass_t;
    char* dcc_descriptor_id_type;
    char void_t[] = "void";
    struct tree_node_t* sys_gen;
    struct tree_node_t* uc_gen;
    struct tree_node_t* ppclass;
    char* out_fname;
    char h_fname[MAX_FILENAME_LENGTH];
    struct free_list_t* last_elem;
    uint16 used_simple_types = 0;
    uint8 sys_p_defined = 0;
    uint8 uc_p_defined = 0;
    uint8 parpack_p_defined = 0;
    char dcc_descriptor_id_type_unckown[] = "UNKNOWN";

    dcc_descriptor_id_type = ((ctx->xml_header.dcc_descriptor_id) >= (sizeof(dcc_id_names)/sizeof(dcc_id_names[0]))) ?
                            dcc_descriptor_id_type_unckown :
                            (char*)dcc_id_names[ctx->xml_header.dcc_descriptor_id];

    //Find sys params, use_case params and parpack trees
    sys_gen = ctx->tree_gen;
    uc_gen = NULL;
    ppclass = NULL;
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

    //determine the type names of DCC data. Some of the data may be missing.
    //Such data is denoted as void in dcc_bin_parse() and is not filled in it.
    if(sys_gen){
        sys_gen_t = sys_gen->name;
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

    //Init variables
    ctx->prs_free_list = NULL;
    ctx->prs_fout_txt = NULL;


    //open output file
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
    change_file_ext(out_fname, ".c");

    ctx->prs_fout_txt = fopen(out_fname,"wt");

    if(ctx->prs_fout_txt == NULL) {
        printf("Can not open output file %s\n", out_fname);
        free(out_fname);
        return (-1);
    }
    free(out_fname);

    //generate the name of the H file. It has to be included in the C file
    strcpy (h_fname, ctx->dcc_name);
    change_file_ext(h_fname, ".h");


    //print header
    fprintf(ctx->prs_fout_txt,"/******************************************************************************\n");
    fprintf(ctx->prs_fout_txt,"Automatically generated parser for DCC data\n");
    fprintf(ctx->prs_fout_txt,"Generated by DCC generator ver %d.%d.%d\n", DCC_GEN_VERSION/10000, (DCC_GEN_VERSION/100)%100, DCC_GEN_VERSION%100 );
    fprintf(ctx->prs_fout_txt,"for dcc_descriptor_id_type = %s\n", dcc_descriptor_id_type);
    fprintf(ctx->prs_fout_txt,"vendor ID %ld\n", ctx->xml_header.algorithm_vendor_id);
    fprintf(ctx->prs_fout_txt,"******************************************************************************/\n");
    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"#include \"%s\"\n", 
        (ctx->fw2dcc_fname)?ctx->fw2dcc_fname:EXTERNAL_CTYPES_INCLUDE);
    fprintf(ctx->prs_fout_txt,"#include \"%s%s\"\n",HEADER_LOCATION, h_fname);
    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"\n");

    //declare get_... functions which are needed
    fprintf(ctx->prs_fout_txt,"static uint8 get_uint8(uint8** p_bin);\n");
    fprintf(ctx->prs_fout_txt,"#define get_int8(a)  (int8)get_uint8(a)\n");
    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"static uint16 get_uint16(uint8** p_bin);\n");
    fprintf(ctx->prs_fout_txt,"#define get_int16(a)  (int16)get_uint16(a)\n");
    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"static uint32 get_uint32(uint8** p_bin);\n");
    fprintf(ctx->prs_fout_txt,"#define get_int32(a)  (int32)get_uint32(a)\n");
    fprintf(ctx->prs_fout_txt,"\n");

    fprintf(ctx->prs_fout_txt,"\n");
    fprintf(ctx->prs_fout_txt,"/* ============================================================================ \n");
    fprintf(ctx->prs_fout_txt,"* dcc_bin_parse()\n");
    fprintf(ctx->prs_fout_txt,"* \n");
    fprintf(ctx->prs_fout_txt,"* @param b_sys_prm : pointer to binary data with system parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param b_uc_prm  : pointer to binary data with use case parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param b_parpack : pointer to binary data with parameter packet\n");
    fprintf(ctx->prs_fout_txt,"* @param sys_prm : pointer output structure with system parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param uc_prm  : pointer output structure with use case parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param parpack : pointer output structure with parameter packet\n");
    fprintf(ctx->prs_fout_txt,"* @return : 0 if success, !0 if error\n");
    fprintf(ctx->prs_fout_txt,"* ========================================================================== */\n");
    fprintf(ctx->prs_fout_txt,"int %s_dcc_bin_parse(uint8* b_sys_prm, uint8* b_uc_prm, uint8* b_parpack,\n",ctx->dcc_name);
    fprintf(ctx->prs_fout_txt,"                  void* sys_prm, void* uc_prm, void* parpack,\n");
    fprintf(ctx->prs_fout_txt,"                  uint32 crc)\n");
    fprintf(ctx->prs_fout_txt,"{\n");
    fprintf(ctx->prs_fout_txt,"    uint8* p_bin;\n");


    fprintf(ctx->prs_fout_txt,"    if(crc != 0x%lX){\n",ctx->xml_header.crc_checksum);
    fprintf(ctx->prs_fout_txt,"        return (%d);//Use this error code to detect different CRC\n", AUTO_PARSER_RETURN_ERROR_CRC);
    fprintf(ctx->prs_fout_txt,"    }\n");

    if(sys_gen)
    {
        fprintf(ctx->prs_fout_txt,"\n");
        fprintf(ctx->prs_fout_txt,"    //parse system parameters\n");
        //skip sys_prms if sys_prm passed to dcc_bin_parse() is NULL;
        fprintf(ctx->prs_fout_txt,"    if( sys_prm ){\n");
        fprintf(ctx->prs_fout_txt,"        %s* sys_p = (%s*)sys_prm;\n",sys_gen_t,sys_gen_t);
        fprintf(ctx->prs_fout_txt,"\n");
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"{ //sys_prms\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"        p_bin = b_sys_prm;\n");
        fprintf(ctx->prs_fout_txt,"\n");

        if(sys_gen->child == NULL){
            printf("general data does not have child\n");
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        free_strcat(ctx,"sys_p->");
        if( prs_parse_fields(ctx, sys_gen->child, "sys_p->", 2, &used_simple_types) ){
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        memset(ctx->parser_free_str,0,ctx->parser_free_str_len_max);
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"} //sys_prms\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"    }\n");
    }

    if(uc_gen)
    {
        fprintf(ctx->prs_fout_txt,"\n");
        fprintf(ctx->prs_fout_txt,"    //parse use case parameters\n");
        //skip uc_prm if uc_prms passed to dcc_bin_parse() is NULL;
        fprintf(ctx->prs_fout_txt,"    if(uc_prm){\n");
        fprintf(ctx->prs_fout_txt,"        %s* uc_p = (%s*)uc_prm;\n", uc_gen_t, uc_gen_t);
        fprintf(ctx->prs_fout_txt,"\n");
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"{ //uc_prms\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"        p_bin = b_uc_prm;\n");

        if(uc_gen->child == NULL){
            printf("usecase data does not have child\n");
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        free_strcat(ctx,"uc_p->");
        if( prs_parse_fields(ctx, uc_gen->child, "uc_p->", 2, &used_simple_types) ){
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        memset(ctx->parser_free_str,0,ctx->parser_free_str_len_max);
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"} //uc_prms\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"    }\n");
    }

    if(ppclass)
    {
        fprintf(ctx->prs_fout_txt,"\n");
        fprintf(ctx->prs_fout_txt,"    //parse parameter packets\n");
        //skip parpacks if parpack ppclass to dcc_bin_parse() is NULL;
        fprintf(ctx->prs_fout_txt,"    if(parpack){\n");
        fprintf(ctx->prs_fout_txt,"        %s* parpack_p = (%s*)parpack;\n", ppclass_t, ppclass_t);
        fprintf(ctx->prs_fout_txt,"\n");
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"{ //parpak\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"        p_bin = b_parpack;\n");

        if(ppclass->child == NULL){
            printf("usecase data does not have child\n");
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        free_strcat(ctx,"parpack_p->");
        if( prs_parse_fields(ctx, ppclass->child, "parpack_p->", 2, &used_simple_types) ){
            fclose(ctx->prs_fout_txt);
            return (-1);
        }
        memset(ctx->parser_free_str,0,ctx->parser_free_str_len_max);
#ifdef DEBUG_PRINT
        fprintf(ctx->prs_fout_txt,"        printf_dbg(%s\"} //parpak\\n\");\n",DEBUG_STR);
#endif
        fprintf(ctx->prs_fout_txt,"    }\n");
    }
    fprintf(ctx->prs_fout_txt,"    return 0;\n");
    fprintf(ctx->prs_fout_txt,"}\n");

    fprintf(ctx->prs_fout_txt,"/* ============================================================================ \n");
    fprintf(ctx->prs_fout_txt,"* dcc_bin_free()\n");
    fprintf(ctx->prs_fout_txt,"*\n");
    fprintf(ctx->prs_fout_txt,"* Frees any memory that was allocated in dcc_bin_parse()\n");
    fprintf(ctx->prs_fout_txt,"* \n");
    fprintf(ctx->prs_fout_txt,"* @param sys_prm : pointer output structure with system parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param uc_prm  : pointer output structure with use case parameters\n");
    fprintf(ctx->prs_fout_txt,"* @param parpack : pointer output structure with parameter packet\n");
    fprintf(ctx->prs_fout_txt,"* @return : 0 if success, !0 if error\n");
    fprintf(ctx->prs_fout_txt,"* ========================================================================== */\n");
    fprintf(ctx->prs_fout_txt,"void %s_dcc_bin_free(void* sys_prm, void* uc_prm, void* parpack)\n",ctx->dcc_name);
    fprintf(ctx->prs_fout_txt,"{\n");
    fprintf(ctx->prs_fout_txt,"\n");

    if(ctx->prs_free_list){
        //sweep the free list to end, free-ing must start from the most
        //inner arrays.
        //Meanwhile define local pinter to DCC parsed structures - the parsed
        //structs are defined as void* in the fuction header to unify the
        //interfaces of parsing functions.
        last_elem = ctx->prs_free_list;
        while (last_elem)
        {
            //if the frww_var starts with "sys_p", the define local variable
            //for sys_prm and so on for UC and parpack
            if( (strstr(last_elem->free_var, "sys_p") == last_elem->free_var) &&
                (sys_p_defined == 0)
            ){
                fprintf(ctx->prs_fout_txt,"    %s* sys_p = (%s*)sys_prm;\n",sys_gen_t,sys_gen_t);
                sys_p_defined = 1;
            }
            if( (strstr(last_elem->free_var, "uc_p") == last_elem->free_var) &&
                (uc_p_defined == 0)
            ){
                fprintf(ctx->prs_fout_txt,"    %s* uc_p = (%s*)uc_prm;\n", uc_gen_t, uc_gen_t);
                uc_p_defined = 1;
            }
            if( (strstr(last_elem->free_var, "parpack_p") == last_elem->free_var) &&
                (parpack_p_defined == 0)
            ){
                fprintf(ctx->prs_fout_txt,"    %s* parpack_p = (%s*)parpack;\n", ppclass_t, ppclass_t);
                parpack_p_defined = 1;
            }

            if(last_elem->next == NULL){
                break;
            }
            last_elem = last_elem->next;
        }

        while (last_elem){
            write_free_string(ctx->prs_fout_txt,last_elem->free_var , 1);
            last_elem = last_elem->prev;
        }
    }
    fprintf(ctx->prs_fout_txt,"}\n\n");

    //define get_... functions which are needed
    if(used_simple_types & ((1<<BASIC_TYPE_UINT8) | (1<<BASIC_TYPE_INT8)))
    {
        fprintf(ctx->prs_fout_txt,"/* ============================================================================ \n");
        fprintf(ctx->prs_fout_txt,"* get_uint8(),get_int8(),get_uint16(),get_int16(),get_uint32(),get_int32()\n");
        fprintf(ctx->prs_fout_txt,"* \n");
        fprintf(ctx->prs_fout_txt,"* reads from binary buffer, assembles 1B,2B and 4B data and updates the buffer\n");
        fprintf(ctx->prs_fout_txt,"* pointer\n");
        fprintf(ctx->prs_fout_txt,"* ========================================================================== */\n");
        fprintf(ctx->prs_fout_txt,"static uint8 get_uint8(uint8** p_bin){\n");
        fprintf(ctx->prs_fout_txt,"    uint8 byte;\n");
        fprintf(ctx->prs_fout_txt,"    uint8 *p = *p_bin;\n");
        fprintf(ctx->prs_fout_txt,"    byte = *p++;\n");
        fprintf(ctx->prs_fout_txt,"    *p_bin = p;\n");
        fprintf(ctx->prs_fout_txt,"    return byte;\n");
        fprintf(ctx->prs_fout_txt,"}\n\n");
    }


    if(used_simple_types & ((1<<BASIC_TYPE_UINT16) | (1<<BASIC_TYPE_INT16)))
    {
        fprintf(ctx->prs_fout_txt,"static uint16 get_uint16(uint8** p_bin){\n");
        fprintf(ctx->prs_fout_txt,"    uint16 byte2;\n");
        fprintf(ctx->prs_fout_txt,"    uint8 *p = *p_bin;\n");
        fprintf(ctx->prs_fout_txt,"    byte2 = *p++;\n");
        fprintf(ctx->prs_fout_txt,"    byte2 |= (uint16)(*p++) << 8;\n");
        fprintf(ctx->prs_fout_txt,"    *p_bin = p;\n");
        fprintf(ctx->prs_fout_txt,"    return byte2;\n");
        fprintf(ctx->prs_fout_txt,"}\n\n");
    }

    if(used_simple_types & ((1<<BASIC_TYPE_UINT32) | (1<<BASIC_TYPE_INT32)))
    {
        fprintf(ctx->prs_fout_txt,"static uint32 get_uint32(uint8** p_bin){\n");
        fprintf(ctx->prs_fout_txt,"    uint32 byte4;\n");
        fprintf(ctx->prs_fout_txt,"    uint8 *p = *p_bin;\n");
        fprintf(ctx->prs_fout_txt,"    byte4 = *p++;\n");
        fprintf(ctx->prs_fout_txt,"    byte4 |= (uint32)(*p++) << 8;\n");
        fprintf(ctx->prs_fout_txt,"    byte4 |= (uint32)(*p++) << 16;\n");
        fprintf(ctx->prs_fout_txt,"    byte4 |= (uint32)(*p++) << 24;\n");
        fprintf(ctx->prs_fout_txt,"    *p_bin = p;\n");
        fprintf(ctx->prs_fout_txt,"    return byte4;\n");
        fprintf(ctx->prs_fout_txt,"}\n\n");
    }


    fclose(ctx->prs_fout_txt);
    return 0;
}
