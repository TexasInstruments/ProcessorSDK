/*****************************************************************
 * dcc.c
 */

#define _CRT_SECURE_NO_DEPRECATE

#include "ctypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef __LINUX__
#include <conio.h>
#endif

#include "expat.h"
#include "tools.h"
#include "private.h"

#if defined(__amigaos__) && defined(__USE_INLINE__)
#include <proto/expat.h>
#endif

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

/* ========================================================================= */
/*                    DEFINES                                                */
/* ========================================================================= */
#define READ_FILE_BUFFSIZE        8192

/* ========================================================================= */
/*                    globals                                                */
/* ========================================================================= */
char read_file_buff[READ_FILE_BUFFSIZE];

dtp_gen_ctx_t dtp_gen_ctx;
XML_Parser xml_parser;

/* ========================================================================= */
/*                    routines                                               */
/* ========================================================================= */
void dtp_gen_abort(int err);
void expand_data_str(dtp_gen_ctx_t* ctx);

/* ============================================================================
* print_indent()
*
* ========================================================================== */
void print_indent (int depth)
{
    int i;
    for (i = 0; i < depth; i++)
        printf("  ");
}
/* ============================================================================
* comment_handler()
*
* ========================================================================== */
static void XMLCALL comment_handler(void *userData,
                              const XML_Char *s)
{
    char* comment_str = (char*)s;
    int len = strlen(comment_str);
    //dtp_gen_ctx_t* ctx = (dtp_gen_ctx_t*)userData;

    tls_clear_starting_white_spaces(&comment_str, &len);
/*
    if (len > 0) {
    ///////////////////////////////////////
        print_indent(ctx->xml_depth);
        printf(" //%s", comment_str);
        printf("\n");
    ///////////////////////////////////////
    }
*/
}
/* ============================================================================
* data_handler()
*
* this handler may be called many times within one block of data. The data is
* accumulated in ctx->data_str and all of it is processed in end_handler by
* data_proc function.
* ========================================================================== */
static void XMLCALL data_handler(void *userData,
                                    const XML_Char *s,
                                    int len)
{
    char* data_str = (char*)s;
    dtp_gen_ctx_t* ctx = (dtp_gen_ctx_t*)userData;

//    tls_clear_starting_white_spaces(&data_str, &len);
    if (len <= 0){
        return;
    }

    //accumulate the new chunk of data into ctx->data_str
    if((ctx->data_str_len + len +1) > ctx->data_str_len_max)
    {
        //data string buffer is full, alloc mem for larger data string
        // the buffer is considered full when size-2 is reached to leave space for 1 ' ' and a terminating 0
        expand_data_str(ctx);
    }
    //append the new data to currently accumulated
    memcpy(&(ctx->data_str[ctx->data_str_len]), data_str, len);

    ctx->data_str_len += len;
    ctx->data_str[ctx->data_str_len] = 0; //add terminating 0

    //mark there is data to process in data_str
    ctx->xml_data_pending = 1;

}

/* ============================================================================
* data_proc()
*
* ========================================================================== */
static void XMLCALL data_proc(void *userData)
{
    dtp_gen_ctx_t* ctx = (dtp_gen_ctx_t*)userData;
/*
    ///////////////////////////////////////
    print_indent(ctx->xml_depth);
    printf("%s",ctx->data_str);
    printf("\n");
    ///////////////////////////////////////
*/
	if (ctx->dcc_gen_stage == DCCGEN_STAGE_GENERAL_DATA)
	{
		general_data_process(ctx);
			}
	else if (ctx->dcc_gen_stage == DCCGEN_STAGE_USECASE) {
        usecase_data_process(ctx);
	}
	if(ctx->dcc_gen_stage != DCCGEN_STAGE_HEADER){

    ctx->xml_data_pending = 0;
    ctx->data_str_len     = 0;
    ctx->data_str[0] = 0;
	}

}

/* ============================================================================
* start_handler()
*
* ========================================================================== */
static void XMLCALL start_handler(void *userData, const char *name, const char **attr)
{
    dtp_gen_ctx_t* ctx = (dtp_gen_ctx_t*)userData;
    int err = 0 , c = 0, i = 0;
    char* last;

	ctx->xml_line = XML_GetCurrentLineNumber(xml_parser);
    if (ctx->xml_data_pending)
    {
        data_proc(userData);
    }
    if (strcmp(name, XML_STAGE_DCC_NAME) == 0 ) {
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }

    } else if( strcmp(name, XML_STAGE_NAME_HEADER) == 0 ) {
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_HEADER;
    }
	else if( strcmp(name, XML_STAGE_NAME_TYPEDEF ) == 0 )  {
        //DCC XML block "typedef"
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_TYPEDEF;
    }
    else if( strcmp(name, XML_STAGE_NAME_GENERAL_DATA ) == 0 )  {
        //DCC XML block "general_data"
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_GENERAL_DATA;
    }
    else if(strcmp(name, XML_STAGE_NAME_USECASE ) == 0 )  {
        //DCC XML block "usecase"
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ////Check usecase name - is it meaningfull?
        //if(ctx->curr_usecase >= DCC_USECASE_COUNT){
        //    printf ("ERROR at line %d : invalid usecase defined \n",
        //             XML_GetCurrentLineNumber(xml_parser));
        //    dtp_gen_abort(1);
        //}

        //Update usecases bitmask and set pointers to the new usecase.
        //ctx->use_case_present |= ...
        if(!ctx->use_cases){
            ctx->use_cases = (struct use_case_t*) osal_malloc(sizeof(struct use_case_t));
            if(!ctx->use_cases){
                printf("ERROR malloc failed in usecase allocation!\n");
                dtp_gen_abort(1);
            }
            ctx->current_usecase = ctx->use_cases;
            ctx->current_usecase->prev=NULL;
            ctx->current_usecase->next=NULL;
            ctx->current_usecase->cur_known_type = NULL;
            ctx->current_usecase->known_type = NULL;
            ctx->current_usecase->current_photospace = NULL;
            ctx->current_usecase->par_pack = NULL;
            ctx->current_usecase->photospaces = NULL;
            ctx->current_usecase->use_case_parameter = NULL;
            ctx->current_usecase->write_buffer = NULL;
            ctx->current_usecase->buffer_size = 0;
            ctx->current_usecase->start_in_dcc_file = 0;
            ctx->current_usecase->size_in_dcc_file = 0;
            ctx->current_usecase->photospace_count = 0;
            if(strcmp(*attr, XML_NAME_ID_TYPE ) == 0 ){
                ctx->current_usecase->mask = atoi(*(++attr));
            } else {
                printf("ERROR: No ID given to usecase. Must use \"%s\" type!\n",
                       XML_NAME_ID_TYPE);
                dtp_gen_abort(1);
            }
        } else {
            ctx->current_usecase = ctx->use_cases;
            while(ctx->current_usecase->next){
                ctx->current_usecase = ctx->current_usecase->next;
            }
            ctx->current_usecase->next = (struct use_case_t*) osal_malloc (sizeof(struct use_case_t));
            if(!ctx->current_usecase->next){
                printf("ERROR malloc failed in usecase allocation!\n");
                dtp_gen_abort(1);
            }
            ctx->current_usecase->next->prev = ctx->current_usecase;
            ctx->current_usecase = ctx->current_usecase->next;
            ctx->current_usecase->next=NULL;
            ctx->current_usecase->cur_known_type = NULL;
            ctx->current_usecase->known_type = NULL;
            ctx->current_usecase->current_photospace = NULL;
            ctx->current_usecase->par_pack = NULL;
            ctx->current_usecase->photospaces = NULL;
            ctx->current_usecase->use_case_parameter = NULL;
            ctx->current_usecase->write_buffer = NULL;
            ctx->current_usecase->buffer_size = 0;
            ctx->current_usecase->start_in_dcc_file = 0;
            ctx->current_usecase->size_in_dcc_file = 0;
            ctx->current_usecase->photospace_count = 0;
            if(strcmp(*attr, XML_NAME_ID_TYPE ) == 0 ){
                ctx->current_usecase->mask = atoi(*(++attr));
            } else {
                printf("ERROR: No ID given to usecase. Must use \"%s\" type!\n",
                       XML_NAME_ID_TYPE);
                dtp_gen_abort(1);
            }

        }
        ctx->use_case_count ++;
        ctx->dcc_gen_stage = DCCGEN_STAGE_USECASE;
	}
    else {
        //this is not a start of a DTP XML block
        switch (ctx->dcc_gen_stage){
        case DCCGEN_STAGE_HEADER:
            {
                err = proc_header_start(userData, name, attr);
                if (err)
					dtp_gen_abort(1);
                break;
            }
        case DCCGEN_STAGE_TYPEDEF:
			{
				err = proc_typedef_start(userData, name, attr);
				if (err)
					dtp_gen_abort(1);
				break;
			}
        case DCCGEN_STAGE_GENERAL_DATA:
			{
				err = proc_general_data_start(userData,name, attr);
				if (err)
					dtp_gen_abort(1);
				break;
			}
		case DCCGEN_STAGE_USECASE:
			{
				err = proc_usecase_start(userData, name, attr);
				if (err)
                    dtp_gen_abort(1);
				break;
			}
		case DCCGEN_STAGE_INIT:
			{
			    //Check if we're looking at the xml descriptor itself.
			    //its arguments should start with "xmlns"
                if(*attr){
                    last = strrchr( *attr , ':');
                    if (last) {
                        last++;
                        i = strlen(last);
                        c = strlen(*attr);
                        last = (char*)*attr;
                        memset(&last[c-(i+1)],0,i);
                    } else {
                        last = (char*)*attr;
                    }
                    if(strcmp(last, "xmlns")==0) {
                        break;
                    }
                }

                printf("Unknown tag at root level of DCC:");
                printf("(L:%ld) %s\n",ctx->xml_line , name);
                //TODO: is this a stopping error or should we just continue parsing?
                dtp_gen_abort(1);

				break;
			}
        default:
			printf("Error - Could not find right gen stage %s at line: %ld\n",
                            name,ctx->xml_line);
			dtp_gen_abort(1);
			break;
		}
	}

    /////////////////////////////////
    /*
	printf("%s",name);
    print_indent(ctx->xml_depth);

    for (i = 0; attr[i]; i += 2) {
        printf(" %s='%s'", attr[i], attr[i + 1]);
    }
    printf("\n");
    */
    /////////////////////////////////

    ctx->xml_depth++;
}

/* ============================================================================
* end_handler()
*
* ========================================================================== */
static void XMLCALL end_handler(void *userData, const char *name)
{
    dtp_gen_ctx_t* ctx = (dtp_gen_ctx_t*)userData;
    int err = 0;

    if( strcmp(name, XML_STAGE_DCC_NAME) == 0 ) {
        //This XML element contains file name used for H and C files
        //generation. It does not change dcc_gen_stage from INIT stage

        if(ctx->dcc_gen_stage != DCCGEN_STAGE_INIT){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }

        if( extract_dcc_name(&ctx->dcc_name, ctx->data_str) ){
            dtp_gen_abort(1);
        }

        ctx->xml_depth--;
        return;
    }

    if (ctx->xml_data_pending)
    {
        data_proc(userData);
    }

    if( strcmp(name, XML_STAGE_NAME_HEADER) == 0 ) {
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_HEADER){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_INIT;
    }
    else if( strcmp(name, XML_STAGE_NAME_TYPEDEF ) == 0 )  {
        //End of XML block "typedef"
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_TYPEDEF){
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_INIT;
    }
	else if( strcmp(name, XML_STAGE_NAME_GENERAL_DATA ) == 0 ) {
        //End of XML block "general data". Next "usecase" or end may follow - update usecase counter and go to INIT Sytage
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_GENERAL_DATA) {
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_INIT;
    }
    else if( strcmp(name, XML_STAGE_NAME_USECASE ) == 0 ) {
        //End of XML block "usecase". Next "usecase" or end may follow - update usecase counter and go to INIT Sytage
        if(ctx->dcc_gen_stage != DCCGEN_STAGE_USECASE) {
            printf ("ERROR at line %d : '%s' should not appear in another block neither to be nested \n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
            dtp_gen_abort(1);
        }
        ctx->dcc_gen_stage = DCCGEN_STAGE_INIT;
        ctx->current_usecase = NULL;
    }
    else {
        //this is not an end of a DTP XML block
        switch (ctx->dcc_gen_stage){
        case DCCGEN_STAGE_HEADER:
            {
                err = proc_header_end(userData, name);
                if (err)
                {
                    dtp_gen_abort(1);
                }
                break;
            }
        case DCCGEN_STAGE_TYPEDEF:
            {
            err = proc_typedef_end(userData, name);
            if (err)
			{
				dtp_gen_abort(1);
			}
            break;
            }
        case DCCGEN_STAGE_GENERAL_DATA:
            {
			err = proc_general_data_end(userData, name);
			if (err)
			{
				dtp_gen_abort(1);
			}
                break;
            }
		case DCCGEN_STAGE_USECASE:
			{
				err = proc_usecase_end(userData, name);
				if (err)
                    dtp_gen_abort(1);
				break;
			}
		case DCCGEN_STAGE_INIT:
			{
				//Skip anything that is not a root tag.
				break;
			}


        default:
			{
			    printf ("ERROR at line %d : unrecognized tag %s\n",
                     (int)XML_GetCurrentLineNumber(xml_parser),name);
                dtp_gen_abort(1);
                break;
			}
		}
    }

    ctx->xml_depth--;
}


/* ============================================================================
* dtp_gen_abort()
*
* Aborst the program and frees the memory allocated
* ========================================================================== */
void dtp_gen_abort(int err)
{
    dtp_gen_ctx_t* ctx = &dtp_gen_ctx;
    struct known_types_t* known_types;
    struct free_list_t* free_list_elem;
	//struct known_types_t* ptr_known_types = ctx->ptr_to_known_type;
    if(err){
#ifdef __LINUX__
        printf("Error at line: %ld\e[73G[FAIL!]\n",ctx->xml_line);
#else
        printf("Error at line: %ld\n",ctx->xml_line);
#endif
    }
    fclose( ctx->xml_file_in );


	osal_free(ctx->xml_fname);
	ctx->xml_fname = NULL;
    //distroy XML parser
    XML_ParserFree(xml_parser);

    if(ctx->dcc_name){
        osal_free(ctx->dcc_name);
        ctx->dcc_name = NULL;
    }
    if(ctx->fw2dcc_fname){
        osal_free(ctx->fw2dcc_fname);
        ctx->fw2dcc_fname = NULL;
    }

    //destroy the known types list
	//destroy all trees of types in the known types list
    known_types = ctx->known_types;
    while (known_types)
	{
		tree_destroy(known_types->type_tree);
		known_types = known_types->next;
	}
	//destroy the known types list itself
	known_types_destroy (ctx->known_types);

	ctx->known_types = NULL;
	ctx->known_types_ptr = NULL;

    //destroy general data predefined values list
    //destroy trees in the list
    known_types = ctx->ptr_to_known_type;
    while (known_types)
	{
		tree_destroy(known_types->type_tree);
		known_types = known_types->next;
	}
    //destroy general data predefined values list itself
    known_types_destroy(ctx->ptr_to_known_type);
    ctx->ptr_to_known_type = NULL;
    ctx->cur_ptr_to_known_type = NULL;
    //tree_gen point to one of the known types. It is already distroyed.
	ctx->tree_gen = NULL;

    if (ctx->use_cases)
    {
        use_cases_destroy(ctx->use_cases);
        ctx->use_cases = NULL;
        ctx->current_usecase = NULL;
    }

    //free parser "free list"
    free_list_elem = ctx->prs_free_list;
    while (free_list_elem)
    {
        struct free_list_t* tmp;
        tmp = free_list_elem;
        free_list_elem = free_list_elem->next;

        if (tmp->free_var) {
            osal_free(tmp->free_var);
            tmp->free_var = NULL;
        }
        osal_free(tmp);
    }
    ctx->prs_free_list = NULL;


	//free context
    if(ctx->data_str){
        osal_free(ctx->data_str);
		ctx->data_str = NULL;
    }
    if(ctx->parser_free_str){
        osal_free(ctx->parser_free_str);
        ctx->parser_free_str = NULL;
    }

#ifdef MEMORY_LEAK_DEBUG
	mem_deb_print();
#endif

/*
#ifndef __LINUX__
    printf("Press any key ...\n");
    _getch();
#endif
*/
    if(err){
        exit(0); //0 means error
    }

    exit(ctx->xml_header.crc_checksum);
}

void init_variables()
{
	dtp_gen_ctx_t* ctx = &dtp_gen_ctx;

	ctx->xml_fname = NULL;
	ctx->xml_file_in = NULL;
	ctx->xml_depth = 0;
	ctx->data_str_len = 0;
	ctx->xml_data_pending = 0;
	ctx->use_case_present = 0;
	ctx->dcc_gen_stage      = DCCGEN_STAGE_INIT;
	ctx->usecase_stage      = USECASE_STAGE_INIT;
	ctx->photospace_stage   = PHOTOSPACE_INIT;
	ctx->use_case_count     = 0;
	ctx->par_pack_bin_size  = 0;
	ctx->usecase_gen_bin_size = 0;
	memset(&ctx->xml_header,0,sizeof(ctx->xml_header));

	ctx->data_str = osal_malloc(sizeof(char[DATA_STR_LEN_CHUNK]));
	if (!ctx->data_str)
	{
		printf("ERROR: Could not allocate %d memory for data_str\n",DATA_STR_LEN_CHUNK);
		dtp_gen_abort(1);
	}
	ctx->data_str_len_max   = DATA_STR_LEN_CHUNK;

    ctx->parser_free_str = osal_malloc(NAME_FIELD_LENGTH);
    if(ctx->parser_free_str == NULL){
		printf("ERROR: Could not allocate %d memory\n",NAME_FIELD_LENGTH);
		dtp_gen_abort(1);
    }
    memset(ctx->parser_free_str, 0, NAME_FIELD_LENGTH);
    ctx->parser_free_str_len_max = NAME_FIELD_LENGTH;


    // known_types will point to the start of the knowntypes structure
	ctx->known_types = NULL;

	ctx->known_types_ptr = NULL;
	ctx->curr_node = NULL;
	ctx->ptr_to_known_type = NULL;
	ctx->cur_ptr_to_known_type = NULL;

	ctx->tree_gen = NULL;
    ctx->dcc_name = NULL;
    ctx->fw2dcc_fname = NULL;
}

/* ============================================================================
* main()
*
* ========================================================================== */
int main(int argc, char *argv[])
{
    int c;
    int i;
    dtp_gen_ctx_t* ctx;
    char* last;

//    struct par_package_t * current_parpak = NULL;
    int usecase_crc;
    int parpak_crc;

    // init vars
    ctx = &dtp_gen_ctx;
    i = 0;
    usecase_crc = 0;
    parpak_crc = 0;

	init_variables();

	if (argc > 1)
	{
		ctx->xml_fname = osal_malloc(strlen(argv[1])+1);
		if ( ctx->xml_fname == NULL ){
			printf("Couldn't allocate memory\n");
            dtp_gen_abort(1);
			exit(0); //0 means error
		}
		strcpy (ctx->xml_fname, argv[1]);
        if(argc > 2){
            //fw-to-dcc inc file passed as argument
		    ctx->fw2dcc_fname = osal_malloc(strlen(argv[2])+1);
		    if ( ctx->fw2dcc_fname == NULL ){
			    printf("Couldn't allocate memory\n");
                dtp_gen_abort(1);
			    exit(0); //0 means error
		    }
		    strcpy (ctx->fw2dcc_fname, argv[2]);
        }
	}
	else
	{
	    /*
		ctx->xml_fname = osal_malloc(strlen(XML_DEFAYLT_FILE_NAME)+1);
		if ( ctx->xml_fname == NULL ){
		 printf("Couldn't allocate memory for parser\n");
			exit(0); //0 means error
		}
		strcpy (ctx->xml_fname, XML_DEFAYLT_FILE_NAME);
        */
        last = strrchr(argv[0], DIR_DELIMITER);

        if (last) {
            last++;
        } else {
            last = argv[0];
        }

		printf("Usage: %s [input_file.xml] [fw_to_dcc.h name and path]\n",last);
		printf("input_file.xml -  the XML file to be converted\n");
#ifdef DEVEL
		printf("fw_to_dcc.h - external .H file to be included in auto parser and header (developer mode)\n");
#endif
		return 0;
    }

    //We need to remember the directory that the file we are reading is located
    //so that we can use it later when we see an #include directive in parsing
    last = strrchr(argv[1], DIR_DELIMITER);
    if (last) {
        last++;
        i = strlen(last);
        c = strlen(argv[1]);
        strcpy( ctx->xml_fdir, argv[1]);
        memset(&ctx->xml_fdir[c-i],0,i);
    } else {
        strcpy( ctx->xml_fdir, STR_NOT_PRESENT);
    }

    ctx->xml_file_in = fopen(ctx->xml_fname,"r");
    if(ctx->xml_file_in == NULL) {
        printf("Can not open input file %s\n", ctx->xml_fname);
        exit(0); //0 means error
    }


    //init/start parser

    xml_parser = XML_ParserCreate(NULL);
    if (! xml_parser) {
        fprintf(stderr, "Couldn't allocate memory for parser\n");
        exit(0); //0 means error
    }

    XML_SetUserData(xml_parser, &dtp_gen_ctx);
    XML_SetElementHandler(xml_parser, start_handler, end_handler);
    XML_SetCommentHandler(xml_parser, comment_handler);
    XML_SetCharacterDataHandler(xml_parser, data_handler);

    last = strrchr(ctx->xml_fname, DIR_DELIMITER);

    if (last) {
        last++;
    } else {
        last = ctx->xml_fname;
    }

    printf("Parsing:\t[%s]\n",last);
    for (;;) {
        int done;
        int len;

        len = (int)fread(read_file_buff, 1, READ_FILE_BUFFSIZE, ctx->xml_file_in);
        if (ferror(stdin)) {
            fprintf(stderr, "Read error\n");
            dtp_gen_abort(1);
            exit(0); //0 means error
        }
        done = feof(stdin);

        if (XML_Parse(xml_parser, read_file_buff, len, done) == XML_STATUS_ERROR) {
            fprintf(stderr, "Parse error at line %" XML_FMT_INT_MOD "u:\n%s\n",
                  XML_GetCurrentLineNumber(xml_parser),
                  XML_ErrorString(XML_GetErrorCode(xml_parser)));
            dtp_gen_abort(1);
            exit(0); //0 means error
        }

        if (!len)
            break;
    }
#ifdef __LINUX__
    printf("\e[1A\e[75G[OK!]\n");
#endif
    //CRC generation:
    printf("Generating CRC: \n");
    if(ctx->tree_gen){
        generate_crc(&ctx->xml_header.crc_checksum, ctx->tree_gen->child);
    }
    ctx->current_usecase = ctx->use_cases;
    while(ctx->current_usecase){
        if( ctx->current_usecase->use_case_parameter && !usecase_crc ){
            generate_crc(   &ctx->xml_header.crc_checksum,
                            ctx->current_usecase->use_case_parameter->child);
            usecase_crc = 1;
        }
        if( ctx->current_usecase->par_pack && !parpak_crc ) {
            generate_crc(   &ctx->xml_header.crc_checksum,
                            ctx->current_usecase->par_pack->package->child);
            parpak_crc = 1;
        }
        if( usecase_crc && parpak_crc ){
            break;
        } else {
            ctx->current_usecase = ctx->current_usecase->next;
        }
    }
    #ifdef __LINUX__
    printf("\e[1A\e[17G[%lX]\e[75G[OK!]\n",ctx->xml_header.crc_checksum);
    #else
    printf("[0x%lX]\n",ctx->xml_header.crc_checksum);
    #endif
    if(ctx->xml_header.crc_checksum == 0){
        printf("ERROR: No useful data collected. No CRC\n");
        dtp_gen_abort(1);
    }
    if(ctx->dcc_name == NULL){
        printf("DCC name not defined in %s file\n",ctx->xml_fname);
        dtp_gen_abort(1);
    }
#ifdef DCCREAD
    printf("Reading BIN file and generating TXT... \n");
#else
    printf("Generating BIN file... \n");
#endif
    if( write_dtp(ctx)) {
        dtp_gen_abort(1);
    } else {
        #ifdef __LINUX__
        printf("\e[1A\e[75G[OK!]\n");
        #endif
    }
#ifdef DEVEL
    //Write header file:
    printf("Generating HEADER file...\n");
	if (write_header_file(ctx)) {
        dtp_gen_abort(1);
    } else {
        #ifdef __LINUX__
        printf("\e[1A\e[75G[OK!]\n");
        #endif
    }

    printf("Generating PARSER file...\n");
    if( write_parser(ctx) )
	{
		dtp_gen_abort(1);
	} else {
        #ifdef __LINUX__
        printf("\e[1A\e[75G[OK!]\n");
        #endif
    }
#endif
    dtp_gen_abort(0);

    printf("DCCgen finished succesfully\n");
	return (ctx->xml_header.crc_checksum);
}
