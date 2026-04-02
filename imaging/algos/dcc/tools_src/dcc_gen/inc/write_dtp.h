#ifndef WRITE_DTP_H__
#define WRITE_DTP_H__

#define DTP_FILE_HEADER_RESERVED_BYTE       ( 0 )
#define DTP_MAX_SIZE                        (0x1000000)

/* DTP FILE HEADER DEFINES */
#define DTP_BIG_ENDIAN                  (0)
#define DTP_LITTLE_ENDIAN               (1)

#define DTP_FILE_HEADER_ENDIAN              (DTP_LITTLE_ENDIAN)
#define DTP_FILE_HEADER_IDENT_BYTE_1        ('D')
#define DTP_FILE_HEADER_IDENT_BYTE_2        ('C')
#define DTP_FILE_HEADER_IDENT_BYTE_3        ('C')
#define DTP_FILE_HEADER_RESERVED_BYTE       ( 0 )

#define DTP_FILE_HEADER_VERSION_BYTE1       ('0')
#define DTP_FILE_HEADER_VERSION_BYTE2       ('1')
#define DTP_FILE_HEADER_VERSION_BYTE3       ('0')
#define DTP_FILE_HEADER_VERSION_BYTE4       ('0')

typedef struct {
    void (*put_u32)  (void* file, uint32 val);  /*pointer to funtion for reading 4 bytes from header depening of endian */
    void (*put_u16)  (void* file, uint16 val);  /*pointer to funtion for reading 2 bytes from header depening of endian */
    void  (*put_u8)  (void* file, uint8  val);  /*pointer to funtion for reading 1 bytes from header depening of endian */
} put_endian_handler_t;

typedef struct {
    uint32 (*get_u32)  (void* file);  /*pointer to funtion for reading 4 bytes from header depening of endian */
    uint16 (*get_u16)  (void* file);  /*pointer to funtion for reading 2 bytes from header depening of endian */
    uint8  (*get_u8)   (void* file);  /*pointer to funtion for reading 1 bytes from header depening of endian */
} get_endian_handler_t;

typedef struct {
    uint8*            dtp_file;
    uint8*            dtp; //temporary pointer used while writing data to the buffer
    uint32            dcc_size;
    put_endian_handler_t  p_ehdl;
    get_endian_handler_t  g_ehdl;
} dtp_file_t;

#ifdef DCCREAD
typedef struct {
    dcc_component_header_type dcc_header;
    uint32 usecase_count;
    //array with size usecase_count containing the number of parpacks in each usecase
    uint32* parpak_count;
} dccread_t;

typedef struct {
    uint32 id;
    uint32 start;
    uint32 size;
    uint8* buffer;
    uint32 parpak_count;
    uint8** parpak_buffer;
    uint32 ph_dims_count;

} usecase_entry_t;

#endif

void dtp_align(uint8 **dtp, uint32 *size);
void put_u8(void *file, uint8 val);
void put_u16_little_endian(void *file,uint16 val);
void put_u32_little_endian(void *file,uint32 val);

int dtp_write(dtp_file_t *file, char *outfilename);
int write_number(dtp_file_t* file, struct tree_node_t* tree, int indent);
int write_struct(dtp_file_t* file, struct tree_node_t* tree, int indent);
int write_array (dtp_file_t* file, struct tree_node_t* tree, int indent);
int write_tree (dtp_file_t* file, struct tree_node_t* tree, int indent);

void write_bin_header(dtp_file_t* file, dtp_gen_ctx_t* ctx);
void write_usecase(dtp_file_t* file, dtp_gen_ctx_t* ctx);
void write_usecase_table(dtp_file_t* file, dtp_gen_ctx_t* ctx);

#endif // WRITE_DTP_H__
