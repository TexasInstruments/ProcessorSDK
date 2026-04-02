/*****************************************************************
 * usecase.c
 */
#define _CRT_SECURE_NO_DEPRECATE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"
#include "expat.h"

/* ========================================================================= */
/*                    PROTOTYPES                                             */
/* ========================================================================= */
void handle_parpak_data     (dtp_gen_ctx_t* ctx);
int handle_usecase_parpack  (dtp_gen_ctx_t* ctx,
                             const char *name,
                             const char **attr);

int photospace_add          (dtp_gen_ctx_t* ctx,
                             const char *name,
                             const char **attr);

int handle_usecase_general  (dtp_gen_ctx_t* ctx,
                             const char *name,
                             const char **attr);

int handle_usecase_limits   (dtp_gen_ctx_t* ctx,
                             const char *name,
                             const char **attr);

void handle_parpak_data     (dtp_gen_ctx_t* ctx);

/* ========================================================================= */
/*                    FUNCTIONS                                              */
/* ========================================================================= */
int proc_usecase_start (    dtp_gen_ctx_t* ctx,
                            const char *name,
                            const char **attr)
{
    int err=0;
    if(strcmp(name,XML_USECASE_NAME_GENERAL) == 0){
        if(ctx->usecase_stage!=USECASE_STAGE_INIT){

            printf("ERROR Usecase not in its initialized stage\n");
            return 1;
        }
        ctx->usecase_stage = USECASE_STAGE_GENERAL;
    } else if(strcmp(name,XML_USECASE_NAME_PHOTOSPACE) == 0) {
        if(ctx->usecase_stage!=USECASE_STAGE_INIT){

            printf("ERROR Usecase not in its initialized stage\n");
            return 1;
        }
        ctx->usecase_stage = USECASE_STAGE_PHOTOSPACE;

    }
    else if(strcmp(name,XML_USECASE_NAME_PARPACK) == 0) {
        if(ctx->usecase_stage!=USECASE_STAGE_INIT){
            printf("ERROR Usecase not in its initialized stage\n");
            return 1;
        }
        ctx->usecase_stage = USECASE_STAGE_PARPACK;

    } else {
        switch (ctx->usecase_stage) {
        case USECASE_STAGE_GENERAL:
            err = handle_usecase_general(ctx,name,attr);
            break;
        case USECASE_STAGE_PHOTOSPACE:
            if(ctx->photospace_stage == PHOTOSPACE_INIT){
                strcpy(ctx->region_name,name);
                err = photospace_add(ctx,name,attr);
                ctx->photospace_stage = PHOTOSPACE_REGION;
            }
            else if(ctx->photospace_stage == PHOTOSPACE_REGION){
                err = handle_usecase_limits(ctx,name,attr);
            }
            break;
        case USECASE_STAGE_PARPACK:
            err = handle_usecase_parpack(ctx,name,attr);
            //printf("HANDLE parpak\n");
            break;
        case USECASE_STAGE_INIT:
        default:
            //error
            //err = 1;
            printf("ERROR: Undrecognized tag \"%s\" in usecase section. Line: %ld\n", name,ctx->xml_line);
            return 1;
        }
    }
    return err;
}

int proc_usecase_end (  dtp_gen_ctx_t* ctx,
                        const char *name)
{
    int err = 0;
    struct photo_region_t* ph_space;

    if(strcmp(name,XML_USECASE_NAME_GENERAL) == 0){
        if(ctx->usecase_stage!=USECASE_STAGE_GENERAL){
            printf("ERROR Usecase not in its General Params stage!");
            return 1;
        }
        ctx->usecase_stage = USECASE_STAGE_INIT;

    } else if(strcmp(name,XML_USECASE_NAME_PHOTOSPACE) == 0){
        if(ctx->usecase_stage!=USECASE_STAGE_PHOTOSPACE){
            printf("ERROR Usecase not in its Photospace stage!");
            return 1;
        }


        //we need to check if all photospace regions have the same number of
        //limits.
         ph_space = ctx->current_usecase->photospaces;
         while (ph_space->next){
            if (ph_space->limits_count != ph_space->next->limits_count){
                printf("ERROR: The number of limits in this photospace is different in some regions.\n");
                return 1;
            }
            ph_space = ph_space->next;
            //TODO: Add check for the ID of each dimension... or do we need it here?
         }

        ctx->usecase_stage = USECASE_STAGE_INIT;

    }
    else if(strcmp(name,XML_USECASE_NAME_PARPACK) == 0){
        if(ctx->usecase_stage!=USECASE_STAGE_PARPACK){
            printf("ERROR Usecase not in its Parameter Package stage!");
            return 1;
        }
        ctx->usecase_stage = USECASE_STAGE_INIT;
    }
    else {

        switch (ctx->usecase_stage)
        {
        case USECASE_STAGE_PHOTOSPACE:
            if (strcmp(name,ctx->region_name)==0 &&
               (ctx->photospace_stage==PHOTOSPACE_REGION) ){
                ctx->photospace_stage = PHOTOSPACE_INIT;
            }
            break;
        default:
            break;
        }


    }
    return err;
}

void usecase_data_process(dtp_gen_ctx_t* ctx)
{
    int current = 0;

    if( tls_prepare_data_str(ctx) )
    {
		dtp_gen_abort(1);
    }

    if (ctx->current_usecase->cur_known_type)		// we are in a node
    {
        ////Finished with working with the new type
        if((ctx->current_usecase->cur_known_type->type_tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
            //first check whether this tree is non-fully defined array of structs.
            //If so, actual array dimentions should be determined and a new tree
            //will be created. This new tree will replace the old one in
            //ctx->cur_ptr_to_known_type.
            if( edit_tree(&ctx->current_usecase->cur_known_type->type_tree,ctx->data_str) ){
                dtp_gen_abort(1);
            }
        }
        if(write_values_in_nodes(ctx->current_usecase->cur_known_type->type_tree,
                                 ctx->current_usecase->known_type,
                                 ctx->data_str,
                                 &current,
                                 ctx))
        {
            printf("Error while writing data into nodes\n");
            dtp_gen_abort(1);
        }
        if( current != (int)(strlen(ctx->data_str)))
        {
            printf("Error: more elements to initialise in the xml than needed\n");
            dtp_gen_abort(1);
        }
    }

    ctx->current_usecase->cur_known_type = NULL;

}

int handle_usecase_general (dtp_gen_ctx_t* ctx,
                            const char *name,
                            const char **attr)
{
    char new_node_name[NAME_FIELD_LENGTH];
    uint16 new_node_type = 0;
    uint32 new_node_dim_l = 0;
    uint32 new_node_dim_m = 0;
    uint32 new_node_dim_r = 0;
    char  node_type_name[TYPE_FIELD_LENGTH];
    uint32 main_type = 0;
	struct known_types_t* search_for_known_type = NULL;
    struct tree_node_t* new_node = NULL;
	struct tree_node_t * p_child_tree = NULL;
	struct known_types_t * new_known_node = NULL;

    strcpy(new_node_name, name);

    while (*attr){
        if(strcmp(*attr, XML_NAME_TYPE) == 0) {
            attr++;
            strcpy(node_type_name, *attr);
            parse_type(node_type_name, &new_node_type,
                        &new_node_dim_l, &new_node_dim_m, &new_node_dim_r);
            attr++;
        } else if (strcmp(*attr, XML_NAME_MAIN_TYPE) == 0) {
            attr++;
            main_type = parse_main_type((char*)*attr);
            attr++;
        } else {
            printf("ERROR: invalid attribute in typedef section, %s on row %ld\n",
                    *attr, ctx->xml_line);
            return 1;
        }
    }

    if(ctx->curr_node ==  NULL)
    {
        if ( ((new_node_type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) &&
             ((new_node_type & BASIC_TYPE_MASK) != BASIC_TYPE_NO) &&
            !(new_node_type & BASIC_TYPE_ARRAY)) {
            //if the new predefined data is a simple type and is not an array
            printf("ERROR: predefinition of single simple variables is not allowed %ld\n", ctx->xml_line);

            //TODO: check whether this is allowed in practice in DCCgen. single var is same as int[1].
            return 1;
        }
    } else {
        printf("ERROR: Nested XML element (%s) within general data block not allowed. Line: %ld\n", name, ctx->xml_line);
	    dtp_gen_abort(1);
    }

    //if the new elemnt is not a simple type, check the known types
	p_child_tree = NULL;

    if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_NO)
    {
		parse_unknown_type(node_type_name, &new_node_type,
            &new_node_dim_l, &new_node_dim_m, &new_node_dim_r);
        // known_types points to the beginning of the knowntypes structure
		search_for_known_type = search_knwon_types(node_type_name,ctx->known_types);

        if (search_for_known_type == NULL) {
            printf("ERROR: undefined type of element %s at line %ld\n", node_type_name, ctx->xml_line);
            return 1;
        }

        //if the type is one of the known types
		if (ctx->current_usecase->cur_known_type && (!ctx->curr_node))
		{
            //cur_known_type is set to teh just found know type further in this function. 
            //It is reset to 0 after this XML element (predefined data element) is processed in handle_usecase_general.
            //This "if" is entered if a predefined data (named e.g. xxx ) tries to use nested predefined data with same
            //name. I.e. is the structure xxx has predefine dfield with same name xxx. This would create a recursion.
            //With this implementation recursion like ...xxx.yyy.xxx... can not be found, only ...xxx.xxx...
			printf("ERROR: we have already found a known type %lld . Line: %ld\n",
                            ctx->current_usecase->known_type->type_tree->type,
                            ctx->xml_line);
			dtp_gen_abort(1);
		}

		p_child_tree = search_for_known_type->type_tree;
		if (p_child_tree)
		{
			new_node = create_tree_node(NULL,NULL,NULL,NULL,node_type_name);
			if (!new_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}
			new_node->type = BASIC_TYPE_STRUCT;
			new_node->type |=new_node_type;
            new_node->arr_dim_l = new_node_dim_l;
            new_node->arr_dim_m = new_node_dim_m;
            new_node->arr_dim_r = new_node_dim_r;

			new_known_node = create_known_type_node(new_node_name,
                                                    new_node,
                                                    NULL,
                                                    NULL);
			if (!new_known_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}

			ctx->current_usecase->cur_known_type = new_known_node;


            if( make_tree_with_nodes(&new_node,p_child_tree) ){
                return 1;
            }

            // add  the new node to the existing list of known nodes
			add_node_to_list(   &ctx->current_usecase->cur_known_type,
                                &ctx->current_usecase->known_type);
		}
		else
		{
			printf("Unknown type of element\n");
			dtp_gen_abort(1);
		}

		if (main_type == MAIN_TYPE_GENERAL)
		{
			if (ctx->current_usecase->use_case_parameter && ctx->curr_node)
			{
				printf("Error more than one \"general\" main in usecase\n");
				dtp_gen_abort(1);
			}
			ctx->current_usecase->use_case_parameter =
                                ctx->current_usecase->cur_known_type->type_tree;
		}
    } else {
		//The type of that predefined data is not a predefined type. This is allowed only for integer arrays
        if ( ((new_node_type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) && (new_node_type & BASIC_TYPE_ARRAY)) {
			//if it is an array of simple type
	        if (((new_node_type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST) > 3)
	        {
                printf("ERROR: Up to 3D arrays are supported\n");
    			dtp_gen_abort(1);
	        }

			new_node = create_tree_node(NULL,NULL,NULL,NULL,new_node_name);
			if (!new_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}
			new_node->type = new_node_type;
            new_node->arr_dim_l = new_node_dim_l;
            new_node->arr_dim_m = new_node_dim_m;
            new_node->arr_dim_r = new_node_dim_r;

			new_known_node = create_known_type_node(new_node_name,
                                                    new_node,
                                                    NULL,
                                                    NULL);
			if (!new_known_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}

			ctx->current_usecase->cur_known_type = new_known_node;

            // add  the new node to the existing list of known nodes
			add_node_to_list(   &ctx->current_usecase->cur_known_type,
                                &ctx->current_usecase->known_type);

		} else {
			printf("ERROR: A struct or array of structs of undefined type is defined (%s), Line: %ld\n", name, ctx->xml_line);
			dtp_gen_abort(1);
        }

    }

	return 0;
}

int photospace_add       (  dtp_gen_ctx_t* ctx,
                            const char* name,
                            const char** attr)
{
    //char new_node_name[80];
    int32 temp;
    const char **attribs = attr;
    if(!ctx->current_usecase->photospaces){
        ctx->current_usecase->photospaces = (struct photo_region_t*)
                                osal_malloc(sizeof(struct photo_region_t));
        if(!ctx->current_usecase->photospaces){
            printf("ERROR malloc failed in usecase photospace allocation!\n");
            return 1;
        }
        ctx->current_usecase->current_photospace =
                                        ctx->current_usecase->photospaces;
        ctx->current_usecase->current_photospace->prev=NULL;
        ctx->current_usecase->current_photospace->next=NULL;
        ctx->current_usecase->current_photospace->limits = NULL;
        ctx->current_usecase->current_photospace->limits_count = 0;
        ctx->current_usecase->current_photospace->par_pack = 0;
        ctx->current_usecase->photospace_count++;
    } else {
        ctx->current_usecase->current_photospace =
                                        ctx->current_usecase->photospaces;
        while(ctx->current_usecase->current_photospace->next){
            ctx->current_usecase->current_photospace =
                            ctx->current_usecase->current_photospace->next;
        }
        ctx->current_usecase->current_photospace->next = (struct photo_region_t*)
                                osal_malloc (sizeof(struct photo_region_t));
        if(!ctx->current_usecase->current_photospace->next){
            printf("ERROR malloc failed in usecase photospace allocation!\n");
            return 1;
        }
        ctx->current_usecase->current_photospace->next->prev =
                            ctx->current_usecase->current_photospace;
        ctx->current_usecase->current_photospace =
                            ctx->current_usecase->current_photospace->next;
        ctx->current_usecase->current_photospace->next = NULL;
        ctx->current_usecase->current_photospace->limits = NULL;
        ctx->current_usecase->current_photospace->limits_count = 0;
        ctx->current_usecase->current_photospace->par_pack = 0;
        ctx->current_usecase->photospace_count++;
    }

    strcpy(ctx->current_usecase->current_photospace->name,name);

    while (*attribs){
        if(strcmp(*attribs, XML_NAME_CLASS_TYPE) == 0) {
            // HERE WE ALSO KNOW THAT WE ARE LOOKING AT A START OF REGION
            attribs++;
            temp = atoi(*attribs);
            ctx->current_usecase->current_photospace->par_pack = temp;
            attribs++;
        } else {
            printf("ERROR: invalid attribute in usecase photospace section \"%s\" on row %ld\n",
                    *attribs, ctx->xml_line);
            return 1;
        }
    }
    return 0;
}

int handle_usecase_limits   (   dtp_gen_ctx_t* ctx,
                                const char *name,
                                const char **attr)
{
    uint32 temp;
    const char **attribs = attr;
    struct space_t* current_limit;

    if(ctx->current_usecase->current_photospace->limits){
        current_limit = ctx->current_usecase->current_photospace->limits;
        while(current_limit->next){
            current_limit = current_limit->next;
        }
        current_limit->next = (struct space_t*)osal_malloc(sizeof (struct space_t));
        current_limit->next->prev = current_limit;
        current_limit->next->next = NULL;
        current_limit = current_limit->next;
        ctx->current_usecase->current_photospace->limits_count++;

    } else {
        ctx->current_usecase->current_photospace->limits =
                    (struct space_t*)osal_malloc(sizeof ( struct space_t));
        ctx->current_usecase->current_photospace->limits->next =
        ctx->current_usecase->current_photospace->limits->prev = NULL;
        current_limit = ctx->current_usecase->current_photospace->limits;
        ctx->current_usecase->current_photospace->limits_count++;

    }

    strcpy(current_limit->name,name);

    while (*attribs){
        if(strcmp(*attribs, XML_NAME_START) == 0) {
            // HERE WE ALSO KNOW THAT WE ARE LOOKING AT A START OF REGION
            attribs++;
            temp = atoi(*attribs);
            current_limit->start = temp;
            attribs++;
        } else if(strcmp(*attribs,XML_NAME_END) == 0) {
            // HERE WE ALSO KNOW THAT WE ARE LOOKING AT AN END OF REGION
            attribs++;
            temp = atoi(*attribs);
            current_limit->end = temp;
            attribs++;
        } else if(strcmp(*attribs,XML_NAME_ID_TYPE) == 0) {
            // HERE WE ALSO KNOW WHAT THE ID OF THE DIMENSION IS
            attribs++;
            temp = atoi(*attribs);
            current_limit->id = temp;
            attribs++;
        } else {
            printf("ERROR: invalid attribute in usecase photospace limit section \"%s\" on row %ld\n",
                    *attribs, ctx->xml_line);
            return 1;
        }
    }
    return 0;
}

void handle_parpak_data(dtp_gen_ctx_t* ctx)
{
    int current = 0;
    struct tree_node_t tree;

    if(write_values_in_nodes(&tree,
                             ctx->current_usecase->known_type,
                             ctx->data_str,
                             &current,
                             ctx))
    {
        printf("Error while writing data into nodes\n");
        dtp_gen_abort(1);
    }


}
int handle_usecase_parpack(dtp_gen_ctx_t* ctx, const char *name, const char **attr)
{
    char new_node_name[NAME_FIELD_LENGTH];
    uint16 new_node_type = 0;
    uint32 new_node_dim_l = 0;
    uint32 new_node_dim_m = 0;
    uint32 new_node_dim_r = 0;
    char  node_type_name[TYPE_FIELD_LENGTH];
	struct known_types_t* search_for_known_type = NULL;
    struct tree_node_t* new_node = NULL;
	struct tree_node_t * p_child_tree = NULL;
	struct known_types_t * new_known_node = NULL;
	struct par_package_t * current_parpak = NULL;

    strcpy(new_node_name, name);

    while (*attr){
        if(strcmp(*attr, XML_NAME_TYPE) == 0) {
            attr++;
            strcpy(node_type_name, *attr);
            parse_type(node_type_name, &new_node_type,
                        &new_node_dim_l, &new_node_dim_m, &new_node_dim_r);
            attr++;
        } else {
            printf("ERROR: invalid attribute in typedef section, %s on row %ld\n",
                    *attr, ctx->xml_line);
            return 1;
        }
    }

    if(ctx->curr_node ==  NULL)
    {
         //ctx->curr_node ==  NULL, this is newly defined type
        if (new_node_type & BASIC_TYPE_ARRAY) {
            //if the new type is an array
            printf("ERROR: defining an array type is not allowed, line: %ld\n",
                                                                ctx->xml_line);
            return 1;
        }
        if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT) {
            //if the new type is a simple type (redefinition of a simple type)
            printf("ERROR: redefinition of a simple type is not allowed, line: %ld\n",
                                                                ctx->xml_line);
            return 1;
        }
    } else { //check for other constraints
 	//this is a field of a structure
        if((ctx->curr_node->type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) {
            printf("ERROR: Only a structure may have named fields, line: %ld\n",
                                                             ctx->xml_line);
            return 1;
        }
    }

    //if the new elemnt is not a simple type, check the known types
	p_child_tree = NULL;

    if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_NO)
    {
		parse_unknown_type(node_type_name, &new_node_type,
                        &new_node_dim_l, &new_node_dim_m, &new_node_dim_r);
        // known_types points to the beginning of the knowntypes structure
		search_for_known_type = search_knwon_types(node_type_name,ctx->known_types);

        if (search_for_known_type == NULL) {
            printf("ERROR: undefined type of element %s at line %ld\n",
                                            node_type_name, ctx->xml_line);
            return 1;
        }

        //if the type is one of the known types
		if (ctx->current_usecase->cur_known_type && (!ctx->curr_node))
		{
			printf("ERROR: we have already found a known type %lld . Line: %ld\n",
                            ctx->current_usecase->known_type->type_tree->type,
                            ctx->xml_line);
			dtp_gen_abort(1);
		}

		p_child_tree = search_for_known_type->type_tree;
		if (p_child_tree)
		{
			new_node = create_tree_node(NULL,NULL,NULL,NULL,node_type_name);
			if (!new_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}
			new_node->type = BASIC_TYPE_STRUCT;
			new_node->type |=new_node_type;
            new_node->arr_dim_l = new_node_dim_l;
            new_node->arr_dim_m = new_node_dim_m;
            new_node->arr_dim_r = new_node_dim_r;

			new_known_node = create_known_type_node(new_node_name,
                                                    new_node,
                                                    NULL,
                                                    NULL);
			if (!new_known_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}

			ctx->current_usecase->cur_known_type = new_known_node;


            if( make_tree_with_nodes(&new_node,p_child_tree) ){
				dtp_gen_abort(1);
            }

            // add  the new node to the existing list of known nodes
			add_node_to_list(   &ctx->current_usecase->cur_known_type,
                                &ctx->current_usecase->known_type);
		}
		else
		{
			printf("Unknown type of element\n");
			dtp_gen_abort(1);
		}

    } else {
		//The type of that predefined data is not a predefined type. This is allowed only for integer arrays
        if ( ((new_node_type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) && (new_node_type & BASIC_TYPE_ARRAY)) {
			//if it is an array of simple type
	        if (((new_node_type & BASIC_TYPE_ARRAY) >> BASIC_TYPE_ARRAY_DIMS_OFST) > 3)
	        {
                printf("ERROR: Up to 3D arrays are supported\n");
    			dtp_gen_abort(1);
	        }

			new_node = create_tree_node(NULL,NULL,NULL,NULL,new_node_name);
			if (!new_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}
			new_node->type = new_node_type;
            new_node->arr_dim_l = new_node_dim_l;
            new_node->arr_dim_m = new_node_dim_m;
            new_node->arr_dim_r = new_node_dim_r;

			new_known_node = create_known_type_node(new_node_name,
                                                    new_node,
                                                    NULL,
                                                    NULL);
			if (!new_known_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}

			ctx->current_usecase->cur_known_type = new_known_node;

            // add  the new node to the existing list of known nodes
			add_node_to_list(   &ctx->current_usecase->cur_known_type,
                                &ctx->current_usecase->known_type);

		} else {
			printf("ERROR: A struct or array of structs of undefined type is defined (%s), Line: %ld\n", name, ctx->xml_line);
			dtp_gen_abort(1);
        }
    }
    current_parpak = ctx->current_usecase->par_pack;
    if(!current_parpak){
        ctx->current_usecase->par_pack = (struct par_package_t *)
                            osal_malloc(sizeof(struct par_package_t));
        ctx->current_usecase->par_pack->next = NULL;
        ctx->current_usecase->par_pack->prev = NULL;
        ctx->current_usecase->par_pack->write_buffer = NULL;
        ctx->current_usecase->par_pack->buffer_size = 0;
        ctx->current_usecase->par_pack->package =
                            ctx->current_usecase->cur_known_type->type_tree;
    }
    else {
        while( current_parpak->next ){
            current_parpak = current_parpak->next;
        }
        current_parpak->next = (struct par_package_t *)
                            osal_malloc(sizeof(struct par_package_t));
        current_parpak->next->prev = current_parpak;
        current_parpak->next->next = NULL;
        current_parpak->next->write_buffer = NULL;
        current_parpak->next->buffer_size = 0;
        current_parpak->next->package =
                            ctx->current_usecase->cur_known_type->type_tree;
    }
	return 0;
}

