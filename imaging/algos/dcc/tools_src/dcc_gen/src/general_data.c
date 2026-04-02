/*****************************************************************
 * typedef.c
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
/*                    DEFINES                                                */
/* ========================================================================= */

/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */

int proc_general_data_start(dtp_gen_ctx_t* ctx, const char *name, const char **attr)
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

		p_child_tree = search_for_known_type->type_tree;
		if (p_child_tree)
		{
			new_node = create_tree_node(NULL,NULL,NULL,NULL,node_type_name);
			if (!new_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}


            new_node->type = BASIC_TYPE_STRUCT | new_node_type;
            new_node->arr_dim_l = new_node_dim_l ;
            new_node->arr_dim_m = new_node_dim_m ;
            new_node->arr_dim_r = new_node_dim_r ;

			new_known_node = create_known_type_node(new_node_name,new_node,NULL,NULL);
			if (!new_known_node)
			{
				printf("ERROR: could not make new_node\n");
				dtp_gen_abort(1);
			}

			ctx->cur_ptr_to_known_type = new_known_node;


            if( make_tree_with_nodes(&new_node,p_child_tree) ){
    			dtp_gen_abort(1);
            }

			add_node_to_list(&ctx->cur_ptr_to_known_type,&ctx->ptr_to_known_type);	// add  the new node to the existing list of known nodes
		}
		else
		{
			printf("Unknown type of element\n");
			dtp_gen_abort(1);
		}

		if (main_type == MAIN_TYPE_GENERAL)
		{
			if (ctx->tree_gen && ctx->curr_node)
			{
				printf("Error more than one \"general\" main\n");
				dtp_gen_abort(1);
			}
			ctx->tree_gen = ctx->cur_ptr_to_known_type->type_tree;
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

			ctx->cur_ptr_to_known_type = new_known_node;

            // add  the new node to the existing list of known nodes
			add_node_to_list(   &ctx->cur_ptr_to_known_type,
                                &ctx->ptr_to_known_type);

		} else {
			printf("ERROR: A simple integer, a struct or an array of structs of undefined type is defined (%s), Line: %ld\n", name, ctx->xml_line);
			dtp_gen_abort(1);
        }
    }
	return 0;
}

/* ============================================================================
* proc_general_data_end()
*
* Processes end of a new element inside "general data" DTP XML block
* ========================================================================== */
int proc_general_data_end(dtp_gen_ctx_t* ctx, const char *el)
{
    //if (ctx->curr_node == NULL) {
    //    printf("SW ERR: end of a type element, which is not created\n");
    //    return 1;
    //}
    //ctx->curr_node = ctx->curr_node->parrent;

    return 0;
}

void general_data_process(dtp_gen_ctx_t* ctx)
{
    int current = 0;
    if (ctx->cur_ptr_to_known_type)		// we are in a node
    {
        ////Finished with working with the new type
        if( tls_prepare_data_str(ctx) )
        {
            dtp_gen_abort(1);
        }

        if((ctx->cur_ptr_to_known_type->type_tree->type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT){
            //first check whether this tree is non-fully defined array of structs.
            //If so, actual array dimentions should be determined and a new tree
            //will be created. This new tree will replace the old one in
            //ctx->cur_ptr_to_known_type.
            if( edit_tree(&ctx->cur_ptr_to_known_type->type_tree, ctx->data_str) ){
                dtp_gen_abort(1);
            }
        }

        if (write_values_in_nodes(ctx->cur_ptr_to_known_type->type_tree,ctx->ptr_to_known_type,ctx->data_str,&current,ctx))
        {
            printf("Error while writing data into nodes\n");
            dtp_gen_abort(1);
        }
        if ( current != (int)(strlen(ctx->data_str)))
        {
            printf("Error: more elements to initialise in the xml than needed\n");
            dtp_gen_abort(1);
        }
    }
    ctx->cur_ptr_to_known_type = NULL;
}
