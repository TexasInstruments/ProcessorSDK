/*****************************************************************
 * tools.h
 */

#ifndef __PRIVATE_H__
#define __PRIVATE_H__

#include "ctypes.h"
#include "dcc_defs.h"
#include <stdio.h>

/* ========================================================================= */
/*                    DEFINES                                                */
/* ========================================================================= */
#define DCC_GEN_VERSION  (0*10000 + 1*100 + 11)

//#define DEVEL       //This turns on the developer option
//#define DCCREAD     //This turns on reverse reading option: from bin to txt
//#define DEBUG_PRINT //this lets you build the parser file with debugging prints

#define HEADER_LOCATION             ""
#define EXTERNAL_CTYPES_INCLUDE     "fw_to_dcc.h"

#define AUTO_PARSER_RETURN_ERROR_CRC            (-1)
#define AUTO_PARSER_RETURN_ERROR_ALLOC          (-2)

//Debug string which is given as first arguments in printf_dbg(...)
//This is for compatibility with fprintf and mmsdbg
//#define DEBUG_STR                   "DL_ERROR, MODULE "
#define DEBUG_STR                   ""

#define XML_DEFAYLT_FILE_NAME		"not_used"
#define DATA_STR_LEN_CHUNK          (8192) //Initial size for some strings
#define NAME_FIELD_LENGTH           (80)
#define TYPE_FIELD_LENGTH           (80)
#define MAX_FILENAME_LENGTH         (8193)
#define MAX_DCC_FILE_NAME_LEN       (80)

#define XML_STAGE_DCC_NAME          "dcc_name"
#define XML_STAGE_NAME_HEADER       "dcc_header"
#define XML_STAGE_NAME_TYPEDEF      "typedef"
#define XML_STAGE_NAME_GENERAL_DATA	"system_parameters"
#define XML_STAGE_NAME_USECASE      "use_case"
#define XML_STAGE_NAME_NSPACE       "nspace"
#define XML_STAGE_NAME_VALUES       "values"

#define XML_HEADER_NAME_CAMERA      "camera_module_id"
#define XML_HEADER_NAME_ALGO        "dcc_descriptor_id"
#define XML_HEADER_NAME_VENDOR      "algorithm_vendor_id"
#define XML_HEADER_NAME_TUNE        "tunning_tool_version"

#define XML_USECASE_NAME_GENERAL    "usecase_general"
#define XML_USECASE_NAME_PHOTOSPACE "n-space"
#define XML_USECASE_NAME_PARPACK    "parameter_package"

#define XML_NAME_TYPE        		"type"
#define XML_NAME_MAIN_TYPE   		"main"
#define XML_NAME_CLASS_TYPE         "class"
#define XML_NAME_ID_TYPE            "val"

#define XML_NAME_START              "min"
#define XML_NAME_END                "max"

#define XML_INCLUDE_FILE_STRING     "#include"
#define MAX_INCLUDE_FILE_SIZE       (4000000000) //this number is just an eaxample

#define STR_NOT_PRESENT             "[NOT_PRESENT]"

//This is the postfix to fields definig the size of an unknown size array that
//must be present in the binary file for reading.
#define UNDEF_ARRAY_SIZE_POSTFIX    "_uarr_size"

#ifdef __LINUX__
#define DIR_DELIMITER '/'
#else
#define DIR_DELIMITER '\\'
#endif

/* ========================================================================= */

typedef enum {
    DCCGEN_STAGE_INIT,
    DCCGEN_STAGE_HEADER,
    DCCGEN_STAGE_TYPEDEF,
	DCCGEN_STAGE_GENERAL_DATA,
    DCCGEN_STAGE_USECASE,
    DCCGEN_STAGE_COUNT
} dcc_gen_stage_t;


#define BASIC_IDENTIFIERS_STRUCT		"struct"
#define BASIC_IDENTIFIERS_uint8			"uint8"
#define BASIC_IDENTIFIERS_int8			"int8"
#define BASIC_IDENTIFIERS_uint16		"uint16"
#define BASIC_IDENTIFIERS_int16			"int16"
#define BASIC_IDENTIFIERS_uint32		"uint32"
#define BASIC_IDENTIFIERS_int32			"int32"
#define BASIC_IDENTIFIERS_uint64		"uint64"
#define BASIC_IDENTIFIERS_int64			"int64"


#define MAIN_TYPE_IDENTIFIERS_GENERAL	"general"
#define MAIN_TYPE_IDENTIFIERS_USECASE	"general"
#define MAIN_TYPE_IDENTIFIERS_PARPACK	"general"

// CHECK FOR THIS AS WELL
#define BASIC_IDENTIFIERS_enum			"enum"


#define BASIC_TYPE_ARRAY_DIMS_OFST   (8)
#define BASIC_TYPE_ARRAY_UNDEF_OFST  (12)
//#define BASIC_TYPE_ARRAY_DIM_1_OFST  (16)
//#define BASIC_TYPE_ARRAY_DIM_2_OFST  (32)
//#define BASIC_TYPE_ARRAY_DIM_3_OFST  (48)


#ifdef DEVEL
    #ifdef DCCREAD
        #error Options DEVEL and DCCREAD in Makefile are incompatible.
    #endif
#endif

#ifdef DEBUG_PRINT
    #ifndef DEVEL
        #undef DEBUG_PRINT
    #endif
#endif

typedef enum {
    // 4 LSB are simple type
    //bits 4,5 (BASIC_TYPE_ARRAY) are array dimensions size (1,2,3), 0 if no array
    //bits 15..8 are not used
    //bits 16..31 are array dimension 1 (left [])
    //bits 32..47 are array dimension 2 (middle [])
    //bits 48..63 are array dimension 3 (righ [])
    //2-dimensional array has dimensions only 1 and 2
    //1-dimensional array has only dimension 1
    //Ony the most left dimention may be 0, which means pointer
    BASIC_TYPE_NO,     //uninitialised or not-a-basic type
    BASIC_TYPE_UINT8,
    BASIC_TYPE_INT8,
    BASIC_TYPE_UINT16,
    BASIC_TYPE_INT16,
    BASIC_TYPE_UINT32,
    BASIC_TYPE_INT32,
    BASIC_TYPE_STRUCT,
    BASIC_TYPE_COUNT,
    BASIC_TYPE_MASK  = 0x0F,
//TODO: add bit marking variable size array or pointer. It will be filled
//during "typedef" and will be used at
//C-parser generation, telling the parser that before the rata for this array,
//there are 4 bytes with its actual size. Thisway, there is no need for
//dedicated field in the stricture holding array size - array size is
//explicitely present only in the binary file.
    BASIC_TYPE_ARRAY = (3<<BASIC_TYPE_ARRAY_DIMS_OFST),
    BASIC_TYPE_ARRAY_UNDEF = (1<<BASIC_TYPE_ARRAY_UNDEF_OFST)
} basic_type_t;

typedef enum {
    MAIN_TYPE_NO,
	MAIN_TYPE_GENERAL,
	MAIN_TYPE_USECASE,
	MAIN_TYPE_PARPACK,
	MAIN_TYPE_UNKNOWN,
	MAIN_TYPE_COUNT
} main_type_t;

typedef enum {
    USECASE_STAGE_INIT,
    USECASE_STAGE_GENERAL,
    USECASE_STAGE_PHOTOSPACE,
    USECASE_STAGE_PARPACK,
    USECASE_STAGE_COUNT
} usecase_stage_t;

typedef enum {
    PHOTOSPACE_INIT,
    PHOTOSPACE_REGION,
    PHOTOSPACE_PARPAK,
} photospace_stage_t;

struct tree_node_t {
    struct tree_node_t* parrent;
    struct tree_node_t* prev;
    struct tree_node_t* next;
    struct tree_node_t* child;
    /*This holds the name of the variable if it is simple type covered in the
    type bitmask, or the structure type name if this node is a structure*/
    char*        name;

    /*Bitmask of the tipe this variable is. Look at basic_type_t;*/
    uint16       type;
    //array dimensions
    //2-dimensional array has dimensions only l and m
    //1-dimensional array has only dimension l
    //Only the l dimention may be 0, which means pointer
    uint32       arr_dim_r; //right
    uint32       arr_dim_m; //middle
    uint32       arr_dim_l; //left

    /* Variable used to store pointer to the value an array that is stored in
    this node, OR if the node is a structure this holds the name of the variable
    defined by this structure */
    uint32*      val;
};

struct stack_node_t {
	struct stack_node_t * ptr;
	char * name;
};

struct known_types_t {
    char*        type_name;
    struct tree_node_t* type_tree;
    struct known_types_t* prev;
    struct known_types_t* next;
};

struct space_t {
    uint32 start;
    uint32 end;
    uint32 id;
    char    name[NAME_FIELD_LENGTH];
    struct space_t* next;
    struct space_t* prev;
};

struct photo_region_t {
    char    name[NAME_FIELD_LENGTH];
    uint8   limits_count;
    struct  space_t* limits;
    uint32   par_pack;
    struct  photo_region_t* next;
    struct  photo_region_t* prev;
};

struct par_package_t {
    struct tree_node_t* package;
    uint8* write_buffer;
    uint32 buffer_size;
    struct par_package_t* next;
    struct par_package_t* prev;
};

struct use_case_t {
    uint16 mask;
    uint32 start_in_dcc_file;
    uint32 size_in_dcc_file;
    uint8* write_buffer;
    uint32 buffer_size;
    uint32 photospace_count;
    uint32 parpak_count;
    struct tree_node_t* use_case_parameter;
    struct photo_region_t* photospaces;
    struct photo_region_t* current_photospace;
    struct par_package_t* par_pack;

    struct known_types_t* known_type;			//when initializing the data, points to the begining of the list of complex nodes
	struct known_types_t* cur_known_type;		//points to current node of the list of complex nodes when initializing

    struct use_case_t* next;
    struct use_case_t* prev;
};

struct free_list_t {
    char* free_var;
    struct free_list_t* next;
    struct free_list_t* prev;
};

typedef struct {
    char* xml_fname;

    char  xml_fdir[FILENAME_MAX];
    char*  parser_free_str;
    uint32 parser_free_str_len_max;
    char* dcc_name;
    FILE* xml_file_in;

    dcc_component_header_type xml_header;

    int     xml_depth;
    char*   data_str;
    uint32  data_str_len;
    uint32  data_str_len_max;
    uint32  par_pack_bin_size;
    uint32  usecase_gen_bin_size;
    int     xml_data_pending;
    dcc_gen_stage_t     dcc_gen_stage;
    usecase_stage_t     usecase_stage;
    photospace_stage_t  photospace_stage;
    char region_name[NAME_FIELD_LENGTH];
	uint32	xml_line;

    //use_case_present is a bit mask. Each bit corresponds to a use case according to use_case_t
    uint16   use_case_present;
    uint32   use_case_count;

    struct known_types_t*		known_types;				//points to list of nodes when defininf new types
    struct known_types_t*		known_types_ptr;			//points to current node when defining new types

	struct known_types_t*			ptr_to_known_type;			//when initializing the data, points to the begining of the list of complex nodes
	struct known_types_t*			cur_ptr_to_known_type;		//points to current node of the list of complex nodes when initializing

    struct tree_node_t*			curr_node;					// points to current node in the tree

    struct tree_node_t*			tree_gen;					// points to the root node of the general data tree

    struct use_case_t*          use_cases;
    struct use_case_t*          current_usecase;

    struct free_list_t* prs_free_list;
    FILE* prs_fout_txt;
    
    char* fw2dcc_fname;

}dtp_gen_ctx_t;

/* ========================================================================= */
/*                    EXTERNS                                                */
/* ========================================================================= */

#endif //__PRIVATE_H__

/*
VERSIONS and bug-fixes
0.0.2:
DONE 1.#include files: if the file name includes full path, do not append the path to XML file.

DONE 3. If array dimension is > 32768 => error (1-dimentional array has known second and third dimensions) (should be 65535)

DONE 5. DCCgen to output CRC or 0(if error)

DONE 7. when writing auto parser: Check which of get_...() funcs are needed and define only them

DONE 11. remove ..\inc\ in auto C files

DONE 12. include senID in bin file name idxxx_xmlname.bin

DONE 13. write output files in the dir of XML, not of DCCgen exe

DONE 14. read_DCC : if there is par packs and system_params - first reads sys_params, then usecase descriptor => error

DONE 15 write header in H files

0.0.3:
- auto parser functions: function parameters defined as void* to unify the interface of all DCC components
- bug fixed: if the last character of a predefined type is a number - DCCgen reports error
1.1.
- take fw_to_dcc.h file from command arguments
- fix auto C and H file names  if calling DCCgen not from the XML folder and using relative name to XML file
1.2.
- DCCread mode: print use cases within parpacks prints
- 0-padding at the end of BIN files removed
- bug fixed : auto parser - sometimes necessary gat_xxx fucs are missing
1.3
- Fix DCCgen crach and writing rubbish before header in bin file when long path
  to XML used - file name strings increased to 8193 butes
1.4
- fixed - when spaces exist in the path to included files in XMLs
1.5
- fixed bug when unknown DCC ID in the XML - (used to crash)
1.6
fixed bug in auto-H file when 2D or 3D fully defined array of structs
1.7
maintain 32bit array dimensions sizes. max array elements = 0xffffffff
1.8
maintain NULL for undefined-sized arrays
fix bug in using 32bit array sizes (auto parser may not define gen_u32())
1.9
CRC for XMLs containing undef-arrays changed - to reflect the difference in
auto-parsers, just for protection agains converting such XML with older version
producing 2 byte array sizes in BIN.
1.10
-';' in XMLs recognized as a separator
-bug fix - stack overwrite when some array dim size > 99999 (type_to_string():elements_count[6] overflow)
-bug fix - use case general data did not maintain predefined arrays of simple type due to missing "return 0" in proc_usecase_start() - fixed.
-report errors if out BIN file write failed
1.11
-';' in XMLs recognized as a separator
-bug fix - Include file size was being limited to 4MB.  Increased it to 4GB due to LDC LUT include files being larger than 4MB.
===============================================================================
================== BUGs and TODO ==============================================
===============================================================================

2. size of undef arrays of type : int[][..]
originally found in CAF. DCC gen calkulates array size as dim2*dim1, not as dim2 only. 
This is used in auto free() to free the whole array at once, however if it is used by 
the algFW the size is not meaningfull (algFW expects it to be dim2)
check what happens with struct[][..] - if there are fields which are undef arrays, 
how the loop for freeing those is implemented?

?6. maintain enums - defined in the H file?(OK) or coming from outside?!(not agree)

8.copy comments in H files

9.parpacs do not maintain main=... attribute => remove this attribute from others, consider last defned structure as main

?10.maintain defines (numbers only). Include #define-ed values in CRC if they define sizes arrays

16. free funk is not correct when >=3 nested undef arrays
typedef struct {
    uint32 s1_uarr_size;
    struct s1[] {
        bbb;
        s2_uarr_size;
        struct s2[] {
            aaa;
            large_arr_uarr_size;
            large_arr[];
        }
    };
    uint16 a1;
    uint16 a2;
} us_gen_t;
should be freed
us_gen.s1[0..s1_uarr_size].s2[0..s2_uarr_size].large_arr[]
us_gen.s1[0..s1_uarr_size].s2[]
us_gen.s1[]

actually freed:
!!! us_gen.s1[0..s1_uarr_size].s2[s2_uarr_size].large_arr[] !!!
us_gen.s1[0..s1_uarr_size].s2[]
us_gen.s1[]
========================================
 free funk is not correct when 2D and 3D array of stucts - struct contains undef array

18.Check whether all use cases are present

19.check photospace for coverage

20.reading 0xFFFFFFFF in regions definition results in 0x7FFFFFFF due to atoi() clipping

*/