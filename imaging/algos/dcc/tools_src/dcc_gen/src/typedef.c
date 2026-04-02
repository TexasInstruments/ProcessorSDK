/*****************************************************************
 * typedef.c
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctypes.h"
#include "private.h"
#include "tools.h"

/* ========================================================================== */
/*                    DEFINES                                                */
/* ========================================================================== */

/* ========================================================================== */
/*                    PROTOTYPES                                             */
/* ========================================================================== */
struct known_types_t*  known_types_add (struct known_types_t*   curr_ptr,
                                        char*   name,
                                        struct tree_node_t*     new_type);

struct tree_node_t* tree_add_child( struct tree_node_t* curr_node,
                                    char* name,
                                    uint16 type,
                                    uint32 dim_l, uint32 dim_m, uint32 dim_r );

struct tree_node_t* tree_add_elem(  struct tree_node_t* curr_node,
                                    char* name,
                                    uint16 type,
                                    uint32 dim_l, uint32 dim_m, uint32 dim_r);


/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */

/* ============================================================================
* proc_typedef_start()
*
* Processes start of a new element inside "typedef" DTP XML block
* ========================================================================== */
int proc_typedef_start(dtp_gen_ctx_t* ctx, const char *name, const char **attr)
{
    uint16 new_node_type = 0;
    uint32 new_node_dim_l = 0;
    uint32 new_node_dim_m = 0;
    uint32 new_node_dim_r = 0;
	struct known_types_t* search_for_known_type = NULL;
    struct tree_node_t* new_node;
	struct tree_node_t * p_child_tree ;

	char new_node_name[NAME_FIELD_LENGTH];
	char node_type_name[TYPE_FIELD_LENGTH];


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
    if(ctx->curr_node !=  NULL)
    {
        if( ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)  &&
            (new_node_type & BASIC_TYPE_ARRAY)                        )
        {
            printf("line: %ld\n", ctx->xml_line);
            printf("DCC generator does not support array of structures of non-defined type like this:\n");
            printf("  <typedef>                               \n");
            printf("      <...>                               \n");
            printf("          ...                             \n");
            printf("      <struct_arr type=\"struct[2]\">       \n");
            printf("          <f1 type=\"uint8\"></f1>          \n");
            printf("          <f2 type=\"uint8\"></f2>          \n");
            printf("      </struct_arr>                       \n");
            printf("          ...                             \n");
            printf("      </...>                              \n");
            printf("  </typedef>                              \n");
            printf("Please predefine the type of the structure, then use the predefined type to define an array like this:\n");
            printf("  <typedef>                               \n");
            printf("      <struct_t type=\"struct\">            \n");
            printf("          <f1 type=\"uint8\"></f1>          \n");
            printf("          <f2 type=\"uint8\"></f2>          \n");
            printf("      </struct_t>                         \n");
            printf("      ...                                 \n");
            printf("      <...>                               \n");
            printf("          ...                             \n");
            printf("          <struct_arr type=\"struct_t[2]\"> \n");
            printf("          </struct_arr>                   \n");
            printf("          ...                             \n");
            printf("      </...>                              \n");
            printf("  </typedef>                              \n");

            return 1;
        }
        //this is a field of a structure
        if((ctx->curr_node->type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) {
            printf("ERROR: Only a structure may have named fields, line: %ld\n", ctx->xml_line);
            return 1;
        }
    } else {
        //ctx->curr_node ==  NULL, this is newly defined type
        if (new_node_type & BASIC_TYPE_ARRAY) {
            //if the new type is an array
            printf("ERROR: defining an array type is not allowed, line: %ld\n", ctx->xml_line);
            return 1;
        }
        if ((new_node_type & BASIC_TYPE_MASK) != BASIC_TYPE_STRUCT) {
            //if the new type is a simple type (redefinition of a simple type)
            printf("ERROR: redefinition of a simple type is not allowed, line: %ld\n", ctx->xml_line);
            return 1;
        }
    }

    //if the new elemnt is not a simple type, check the known types
	p_child_tree = NULL;

    strcpy(new_node_name, name);
    //if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
    //{
    //    //if the field is a struct, its field name will be kept in ->val,
    //    //->name will keep the name of predefined type (if predefined) of
    //    //just "struct"
    //    strcpy(new_node_name, "struct");
    //}

    if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_NO)
    {
		parse_unknown_type(node_type_name,&new_node_type,
                        &new_node_dim_l, &new_node_dim_m, &new_node_dim_r);
		search_for_known_type = search_knwon_types(node_type_name,ctx->known_types);	// known_types points to the beginning of the knowntypes list

        if (search_for_known_type == NULL) {
            printf("ERROR: undefined type of element %s at line %ld\n", node_type_name, ctx->xml_line);
            return 1;
        }
        //if the type is one of the known types - replace new_node->name with
        //the name of predefined type. The actual field name will be kept in
        //val (predefined types are always structs, and ->val field of structs
        //is not used for anything else)
        new_node_type |= BASIC_TYPE_STRUCT;
		p_child_tree = search_for_known_type->type_tree;
		strcpy(new_node_name,search_for_known_type->type_name);
    }

    //create new node
    new_node = tree_add_child(ctx->curr_node, new_node_name, new_node_type,
                new_node_dim_l, new_node_dim_m, new_node_dim_r );
    if (new_node == NULL) {
		printf("ERROR: Could not add the new node to the tree!\n");
        return 1;
    }

    if(p_child_tree != NULL)
	{
        if( make_tree_with_nodes(&new_node,p_child_tree) ){
            return 1;
        }
    }

    if ((new_node_type & BASIC_TYPE_MASK) == BASIC_TYPE_STRUCT)
    {
        char* struct_name;
        //Keep the struct names in ->val, not in ->name
        //If this new node is a field of predefined type, its filed name will
        //get lost, because new_node->name will keep the predefined type name.
	    struct_name = osal_malloc(strlen(name)+1);
        if(!struct_name ){
            printf("Error not enough memory\n");
	        dtp_gen_abort(1);
        }
        strcpy(struct_name,name);
        if (new_node->val) {
            //free the memory for previously set value if any
            osal_free(new_node->val);
        }
        new_node->val = (uint32*)struct_name;
    }

    if(ctx->curr_node ==  NULL)
    {
        //this is definition of a new type. Add it into the known_types list.
        //The names of structs are kept in ->val, not in ->name
        ctx->known_types_ptr = known_types_add (ctx->known_types_ptr, (char*)new_node->val, new_node);

		if (!ctx->known_types)		// the 1st element in the list -> connect the pointer to the beginning
		{
			ctx->known_types = ctx->known_types_ptr;
		}

        if(ctx->known_types_ptr == NULL) {
            return 1;
        }
    }

    //move the current pointer to the newly created node.
    ctx->curr_node = new_node;

    return 0;
}


/* ============================================================================
* proc_typedef_end()
*
* Processes end of a new element inside "typedef" DTP XML block
* ========================================================================== */
int proc_typedef_end(dtp_gen_ctx_t* ctx, const char *el)
{
    if (ctx->curr_node == NULL) {
        printf("SW ERR: end of a type element, which is not created\n");
        return 1;
    }

    ctx->curr_node = ctx->curr_node->parrent;

    return 0;
}
/* ============================================================================
* known_types_add()
*
* Adds a new type in the known types list
* params
*   curr_ptr : current position in the known types list
*   name : name of the new type
*   new_type : pointer to root element of the new type tree
* return
*   pointer to newly created item in the known types list
*   NULL if error
* ========================================================================== */
struct known_types_t*  known_types_add (struct known_types_t*   curr_ptr,
                                        char*   name,
                                        struct tree_node_t*     new_type)
{
    struct known_types_t* new_node;

    if(new_type == NULL){
        printf("SW ERR: NULL new type \n");
        return NULL;
    }


    new_node = osal_malloc(sizeof(struct known_types_t));
    if(new_node ==  NULL) {
        printf("ERROR: can not allocate %d bytes memory\n",
                            sizeof(struct known_types_t));
        return NULL;
    }

	new_node->type_name = (char*)osal_malloc(strlen(name)+1);
	strcpy(new_node->type_name, name);
    new_node->type_tree = new_type;
    new_node->prev = curr_ptr;
    new_node->next = NULL;

	if (!curr_ptr)
	{
		curr_ptr = new_node;
	}
	else
	{
		curr_ptr->next = new_node;
		curr_ptr = new_node;
	}

    return new_node;
}

/* ============================================================================
* tree_add_child()
*
* Adds a child (nested) element of the "curr_node" in a type tree. If the
* curr_node already has a child, adds the new child at the end of child list.
*
* params
*   curr_node : current position in the type tree. May be NULL if a new type tree is to be created
*   name : elemnt name
*   type : element type
* return
*   pointer to newly created node in the type tree
*   NULL if error
* ========================================================================== */
struct tree_node_t* tree_add_child (struct tree_node_t*     curr_node,
                                    char*   name,
                                    uint16  type,
                                    uint32 dim_l, uint32 dim_m, uint32 dim_r )
{

    struct tree_node_t* new_node;

    if ((curr_node != NULL) && (curr_node->child != NULL))
    {
        //this node already has a child.
        //new child have to be added at the end of children list
        struct tree_node_t* last_child = curr_node->child;

        while (last_child->next){
            last_child = last_child->next;
        }
        new_node = tree_add_elem (last_child, name, type,
                                    dim_l, dim_m, dim_r);
        if(new_node ==  NULL) {
            return NULL;
        }

        return new_node;
    }


    //curr_node has no child - this will be the first one

    new_node = osal_malloc(sizeof(struct tree_node_t));
    if(new_node ==  NULL) {
        printf("ERROR: can not allocate %d bytes memory\n",
                                sizeof(struct tree_node_t));
        return NULL;
    }

    new_node->parrent = curr_node;
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->child = NULL;
	new_node->val = NULL;

	new_node->name = osal_malloc(strlen(name) + 1);
	if (!new_node->name)
	{
		printf("Error: can not allocate %d bytes memory for name of the node\n",
                                            sizeof(char[NAME_FIELD_LENGTH]));
		return NULL;
	}
	strcpy(new_node->name,name);
    new_node->type = type;
    new_node->arr_dim_l = dim_l;
    new_node->arr_dim_m = dim_m;
    new_node->arr_dim_r = dim_r;

    if(curr_node != NULL){
        //if new_node is not the root of a type tree
        curr_node->child = new_node;
    }

    return new_node;
}
/* ============================================================================
* tree_add_elem()
*
* Adds a new elemnt (node) in a type tree at the same depth as curr_node
*
* params
*   curr_node : current position in the type tree.
*      !!! curr_node must exist ! current node must have parrent !!!
*   name : elemnt name
*   type : element type
* return
*   pointer to newly created node in the type tree
*   NULL if error
* ========================================================================== */
struct tree_node_t* tree_add_elem(struct tree_node_t* curr_node,
                                  char*     name,
                                  uint16    type,
                                  uint32 dim_l, uint32 dim_m, uint32 dim_r)
{
    struct tree_node_t* new_node;

    if (curr_node == NULL) {
        //!!! curr_node must exist !!!
        printf("SW ERR: Trying to add next element to not existing node\n");
        return NULL;
    }
    if (curr_node->next != NULL)
	{
        //!!! current node must not be an inner element of a list !!!
        printf("SW ERR: Trying to add next element to a type tree node, that already has such\n");
        return NULL;
    }
    if (curr_node->parrent == NULL) {
        //!!! current node must have parrent !!!
        printf("SW ERR: Trying to add next element to a type tree node, that already has such\n");
        return NULL;
    }

    new_node = osal_malloc(sizeof(struct tree_node_t));
    if(new_node ==  NULL) {
        printf("ERROR: can not allocate %d bytes memory\n",
                                sizeof(struct tree_node_t));
        return NULL;
    }

	new_node->name = osal_malloc(strlen(name) + 1);
	if (!new_node->name)
	{
		printf("ERROR: Could not allocate memory for name\n");
		return NULL;
	}
	strcpy(new_node->name,name);

    new_node->parrent = curr_node->parrent;
    new_node->prev = curr_node;
    new_node->next = NULL;
    new_node->child = NULL;
    new_node->type = type;
    new_node->arr_dim_l = dim_l ;
    new_node->arr_dim_m = dim_m ;
    new_node->arr_dim_r = dim_r ;
	new_node->val = NULL;

    curr_node->next = new_node;

    return new_node;
}
