/*****************************************************************
 * tools.h
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "private.h"
#include "ctypes.h"

#define CRC_GENERATOR (0xEDB88320)
#define CRC_STR_LEN   (1024)

//#define MEMORY_LEAK_DEBUG

extern const char* BASIC_TYPES_NAMES[BASIC_TYPE_COUNT];
extern const char* dcc_id_names[DCC_ID_COUNT];

/* ========================================================================== */
/*                    PROTOTYPES                                              */
/* ========================================================================== */

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
                  uint32* dim_l, uint32* dim_m, uint32* dim_r);

/* ============================================================================
* parse_unknown_type()
*
* Recognizes whether the type is an array and fills array dimensions sizes
* if "type_name" is an array or pointer of non-basic type this func changes
* the type_name removing leading '*' and/or trailing []
* ========================================================================== */
void parse_unknown_type(char * type_name,
                        uint16* type,
                        uint32* dim_l, uint32* dim_m, uint32* dim_r);

/* ============================================================================
* parse_main_type()
*
* Parses the sring of main type name from XML to produce name of main_type_t
* ========================================================================== */
int parse_main_type(char* str);

void type_to_string(uint16 type, uint32 dim_l, uint32 dim_m, uint32 dim_r, char* name);


/* ========================================================================== */
/*                    EXTERNS                                                 */
/* ========================================================================== */
/* ========================================================================== */
/*                    variables                                               */
/* ========================================================================== */

/* =============================================================================
* tls_clear_starting_white_spaces()
*
* clears starting white spaces in a non-NUll-terminated string, uses the
* characters defined in SEPARATORS_STR as separators between tokens.
*
* params
*   str : pointer to non-NUll-terminated string
*   len : pointer number of characters in the string
* return
*   *str : start of first token in the string
*   *len : number of chars in the remaining srting
* =========================================================================== */
void tls_clear_starting_white_spaces(   char** str,
                                        int* len);

void tls_clear_ending_white_space(      char** str,
                                        int* len);
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
int tls_prepare_data_str(dtp_gen_ctx_t* ctx);

/* =============================================================================
* tree_copy ()
*
* Coppies existing tree (src_tree) at the destination tree node (dst_node).
* The root of src_tree is coppied on dst_node, i.e. dst_node is the root of the
* new tree. dsp_node must have already filled name.
*
* params
*   dst_node : node, where to copy the root of the tree
*   src_tree : root of the tree to be coppied
* =========================================================================== */
int tree_copy(  struct tree_node_t** dst_node,
                struct tree_node_t* src_tree);

/* =============================================================================
* tree_destroy ()
*
* Frees recursively all allocated memory for tree and its fieltd
*
* params
*   type_tree : tree to free
* =========================================================================== */
void tree_destroy (struct tree_node_t* type_tree);

/* =============================================================================
* known_types_destroy ()
*
* Frees recursively all allocated memory for knwon types list
*
* params
*   type_tree : tree to free
* =========================================================================== */
void known_types_destroy (struct known_types_t* type_list);

int make_number_from_string(char * data,int * current, int64 * result);

struct tree_node_t * create_tree_node(  struct tree_node_t * par,
                                        struct tree_node_t * next,
                                        struct tree_node_t * prev,
                                        struct tree_node_t * child,
                                        char * name);

struct known_types_t* create_known_type_node(   char* type_name,
                                                struct tree_node_t* type_tree,
                                                struct known_types_t *prev,
                                                struct known_types_t *next);

int make_tree_with_nodes(  struct tree_node_t** new_node,
                            const struct tree_node_t * source);

void add_node_to_list(  struct known_types_t ** new_node,
                        struct known_types_t ** list);

void count_elems_in_struct( char * buff,
                            int * dims,
                            const int dimension);

void parse_known_name(  char * buffer,
                        char * data,
                        int * current);

int8 different( const struct tree_node_t * declared_node,
                const struct tree_node_t * known_node);

void print_tree(    struct tree_node_t * source,
                    int depth);

void write_indent(  FILE* fout,
                    int indent);

int tree_copy_help( struct tree_node_t ** dst,
                    struct tree_node_t *src,
                    struct tree_node_t * par,
                    struct tree_node_t * prev);

struct known_types_t * search_knwon_types(  char * name,
                                            struct known_types_t * start);

void change_file_ext(   char* fname,
                        char* ext);

void change_file_ext(   char* fname,
                        char* ext);

void to_upper_without_ext(char* fname);

void use_cases_destroy( struct use_case_t* use_list);

//IMPORTED FROM GENERAL_DATA.C
int write_values_struct_known_node (    struct tree_node_t* add_tree,
                                        struct known_types_t* search_tree,
                                        char * data,
                                        int * current,
                                        dtp_gen_ctx_t* ctx);

int edit_tree( struct tree_node_t ** node,
                char * data);

int write_values_in_nodes ( struct tree_node_t* tree,
                            struct known_types_t* search_tree,
                            char * data,
                            int * current,
                            dtp_gen_ctx_t* ctx);

int write_values_struct(    struct tree_node_t * node,
                            struct known_types_t* search_tree,
                            char * data,
                            int * current,
                            dtp_gen_ctx_t* ctx);

int write_values_number(    struct tree_node_t* node,
                            char * data,
                            int * current);

int write_values_array(     struct tree_node_t * node,
                            char * data,
                            int * current);

//DIFFERENT HANDLERS
int proc_header_start(  dtp_gen_ctx_t* ctx,
                        const char *name,
                        const char **attr);

int proc_header_end(    dtp_gen_ctx_t* ctx,
                        const char *name);

int proc_typedef_start( dtp_gen_ctx_t* ctx,
                        const char *name,
                        const char **attr);

int proc_typedef_end(   dtp_gen_ctx_t* ctx,
                        const char *el);

int proc_general_data_start(    dtp_gen_ctx_t* ctx,
                                const char *name,
                                const char **attr);

int proc_general_data_end(  dtp_gen_ctx_t* ctx,
                            const char *el);

int proc_usecase_start (    dtp_gen_ctx_t* ctx,
                            const char *name,
                            const char **attr);

int proc_usecase_end (      dtp_gen_ctx_t* ctx,
                            const char *name);

void general_data_process(  dtp_gen_ctx_t* ctx);

void usecase_data_process(  dtp_gen_ctx_t* ctx);

int handle_usecase_general (dtp_gen_ctx_t* ctx,
                            const char *name,
                            const char **attr);

int handle_usecase_limits  (    dtp_gen_ctx_t* ctx,
                                        const char *name,
                                        const char **attr);

void generate_crc(uint32* crc, struct tree_node_t* ctx);
//OTHER PROTOTYPES
int write_header_file(dtp_gen_ctx_t* dtp_gen_ctx);
int write_parser(dtp_gen_ctx_t* ctx);
int write_dtp(dtp_gen_ctx_t* dtp_gen_ctx);
void dtp_gen_abort( int err);
int extract_dcc_name(char** dcc_name, char* in_str);

/* ============================================================================
* expand_data_str()
*
* If data_sr buffer can not take next that just came, new larger buffer have
* to be allocated
* ========================================================================== */
void expand_data_str(dtp_gen_ctx_t* ctx);

/* ============================================================================
* expand_parser_free_str()
*
* If parser_free_str buffer can not take next that just came, new larger buffer
* have to be allocated
* ========================================================================== */
void expand_parser_free_str(dtp_gen_ctx_t* ctx, uint32 len);

#endif //__TOOLS_H__



#ifdef MEMORY_LEAK_DEBUG
	void osal_free_debug(void *ptr);
	void *osal_malloc_debug(    int aSize,
                                const char * aFuncName,
                                int aLine);
	#define osal_malloc(aSize)					 osal_malloc_debug((aSize), __FILE__, __LINE__)
	#define osal_calloc(aNum, aSize)			 osal_malloc_debug((aSize)*(aNum), __FILE__, __LINE__)
    #define osal_free(ptr)                       osal_free_debug(ptr)
	void mem_deb_print();
#else
#define osal_malloc				malloc
#define osal_free				free
#endif
