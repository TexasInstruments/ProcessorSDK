/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


/**
 *   @file  nvme.h
 *
 *   @brief
 *      This is implements the NVMe API prototypes and data strucutres.
 *
 */


#include <stdint.h>
#include <stdlib.h>
#include <ti/drv/pcie/example/ssd/src/pcie_ssd.h>
#include <ti/csl/hw_types.h>

#define Console_printf printf

/* Enable the below macro to have prints on the IO Console */
#if !defined(SOC_AM574x) && !defined(SOC_AM572x) && !defined(SOC_AM571x) && \
    !defined(SOC_K2G) && \
    !defined(SOC_AM65XX) && \
    !defined(SOC_J721E) && \
    !defined(__ARM_ARCH_7A__)
/* AM57x, AM65x and K2G always use UART for all cores.  Also all ARM use UART */
#define IO_CONSOLE
#endif

#ifndef IO_CONSOLE
#include <ti/drv/uart/UART_stdio.h>
#define NVME_printf  UART_printf
#define NVME_scanf   UART_scanFmt
#else
#define NVME_printf  Console_printf
#define NVME_scanf   Console_scanf
#endif


/* **************************** */
/*   NVME Error Definitions     */
/* **************************** */
typedef int32_t NVME_ERR;

#define NVME_OK 0
#define NVME_READ_ERR 1
#define NVME_WRITE_ERR 2
#define NVME_ADMIN_QUEUE_ERR 3
#define NVME_NO_Q_ALLOCATED 4
#define NVME_IO_Q_FAIL 5


#define QUEUE_DEPTH 8
#define EP_CONFIG_BASE 0x10100000
#define BLOCK_SIZE 512
#define NVME_MODEL_NUMBER_LEN   40
#define NVME_SERIAL_NUMBER_LEN  20

/* ********************* */
/* *        enums        */
/* ********************* */

enum nvme_admin_opcode {
    nvme_admin_delete_sq        = 0x00,
    nvme_admin_create_sq        = 0x01,
    nvme_admin_get_log_page     = 0x02,
    nvme_admin_delete_cq        = 0x04,
    nvme_admin_create_cq        = 0x05,
    nvme_admin_identify         = 0x06,
    nvme_admin_abort_cmd        = 0x08,
    nvme_admin_set_features     = 0x09,
    nvme_admin_get_features     = 0x0a,
    nvme_admin_async_event      = 0x0c,
    nvme_admin_ns_mgmt          = 0x0d,
    nvme_admin_activate_fw      = 0x10,
    nvme_admin_download_fw      = 0x11,
    nvme_admin_dev_self_test    = 0x14,
    nvme_admin_ns_attach        = 0x15,
    nvme_admin_keep_alive       = 0x18,
    nvme_admin_directive_send   = 0x19,
    nvme_admin_directive_recv   = 0x1a,
    nvme_admin_virtual_mgmt     = 0x1c,
    nvme_admin_nvme_mi_send     = 0x1d,
    nvme_admin_nvme_mi_recv     = 0x1e,
    nvme_admin_dbbuf            = 0x7C,
    nvme_admin_format_nvm       = 0x80,
    nvme_admin_security_send    = 0x81,
    nvme_admin_security_recv    = 0x82,
    nvme_admin_sanitize_nvm     = 0x84,
    nvme_admin_get_lba_status   = 0x86,
    nvme_admin_vendor_start     = 0xC0,
};

enum nvme_io_opcode
{
    nvme_io_write               = 0x01,
    nvme_io_read                = 0x02,
};

enum {
    NVME_REG_CAP    = 0x0000,   /* Controller Capabilities */
    NVME_REG_VS     = 0x0008,   /* Version */
    NVME_REG_INTMS  = 0x000c,   /* Interrupt Mask Set */
    NVME_REG_INTMC  = 0x0010,   /* Interrupt Mask Clear */
    NVME_REG_CC     = 0x0014,   /* Controller Configuration */
    NVME_REG_CSTS   = 0x001c,   /* Controller Status */
    NVME_REG_NSSR   = 0x0020,   /* NVM Subsystem Reset */
    NVME_REG_AQA    = 0x0024,   /* Admin Queue Attributes */
    NVME_REG_ASQ    = 0x0028,   /* Admin SQ Base Address */
    NVME_REG_ACQ    = 0x0030,   /* Admin CQ Base Address */
    NVME_REG_CMBLOC = 0x0038,   /* Controller Memory Buffer Location */
    NVME_REG_CMBSZ  = 0x003c,   /* Controller Memory Buffer Size */
    NVME_REG_BPINFO = 0x0040,   /* Boot Partition Information */
    NVME_REG_BPRSEL = 0x0044,   /* Boot Partition Read Select */
    NVME_REG_BPMBL  = 0x0048,   /* Boot Partition Memory Buffer
                     * Location
                     */
    NVME_REG_CMBMSC = 0x0050,   /* Controller Memory Buffer Memory
                     * Space Control
                     */
    NVME_REG_PMRCAP = 0x0e00,   /* Persistent Memory Capabilities */
    NVME_REG_PMRCTL = 0x0e04,   /* Persistent Memory Region Control */
    NVME_REG_PMRSTS = 0x0e08,   /* Persistent Memory Region Status */
    NVME_REG_PMREBS = 0x0e0c,   /* Persistent Memory Region Elasticity
                     * Buffer Size
                     */
    NVME_REG_PMRSWTP = 0x0e10,  /* Persistent Memory Region Sustained
                     * Write Throughput
                     */
    NVME_REG_DBS    = 0x1000,   /* SQ 0 Tail Doorbell */
};

/* **************************** */
/*    NVME Identify structure   */
/* **************************** */

typedef struct {
    uint32_t power_flags; /* MP, MPS and NOPS */
    uint32_t enlat; /* Entry Latency */
    uint32_t exlat; /* Exit Latency */
    uint32_t latency_flags;
    uint8_t  rsvd7[16]; /* Reserved as of Nvm Express 1.1 Spec */
} NVME_PSDESCRIPTOR;


typedef struct {
    /* Controller Capabilities and Features 0-255 */
    uint16_t vid;   /* PCI Vendor ID */
    uint16_t ssvid; /* PCI sub-system vendor ID */
    uint8_t  sn[NVME_SERIAL_NUMBER_LEN];    /* Product serial number */
    uint8_t  mn[NVME_MODEL_NUMBER_LEN]; /* Product model number */
    uint8_t  fr[8]; /* Firmware Revision */
    uint8_t  rab;   /* Recommended Arbitration Burst */
    uint8_t  ieee_oiu[3];   /* Organization Unique Identifier */
    uint8_t  cmic;  /* Multi-interface Capabilities */
    uint8_t  mdts;  /* Maximum Data Transfer Size */
    uint8_t  cntlid[2]; /* Controller ID */
    uint8_t  rsvd1[176];    /* Reserved as of Nvm Express 1.1 Spec */

    /*
        Admin Command Set Attributes
    */
    uint16_t oacs;  /* Optional Admin Command Support */
    uint8_t  acl;   /* Abort Command Limit */
    uint8_t  aerl;  /* Async Event Request Limit */
    uint8_t  frmw;  /* Firmware updates */
    uint8_t  lpa;   /* Log Page Attributes */
    uint8_t  elpe;  /* Error Log Page Entries */
    uint8_t  npss;  /* Number of Power States Support */
    uint8_t  avscc; /* Admin Vendor Specific Command Configuration */
    uint8_t  apsta; /* Autonomous Power State Transition Attributes */
    uint8_t  rsvd2[246];    /* Reserved as of Nvm Express 1.1 Spec */

    /*
     NVM Command Set Attributes
    */
    uint8_t  sqes;  /* Submission Queue Entry Size */
    uint8_t  cqes;  /* Completion Queue Entry Size */
    uint16_t rsvd3; /* Reserved as of Nvm Express 1.1 Spec */
    uint32_t nn;    /* Number of Namespaces */
    uint16_t oncs;  /* Optional NVM Command Support */
    uint16_t fuses; /* Fused Operation Support */
    uint8_t  fna;   /* Format NVM Attributes */
    uint8_t  vwc;   /* Volatile Write Cache */
    uint16_t awun;  /* Atomic Write Unit Normal */
    uint16_t awupf; /* Atomic Write Unit Power Fail */
    uint8_t  nvscc; /* NVM Vendor Specific Command Configuration */
    uint8_t  rsvd4; /* Reserved as of Nvm Express 1.1 Spec */
    uint16_t acwu;  /* Atomic Compare & Write Unit */
    uint16_t rsvd5; /* Reserved as of Nvm Express 1.1 Spec */
    uint32_t sgls;  /* SGL Support  */
    uint8_t  rsvd6[164];    /* Reserved as of Nvm Express 1.1 Spec */

    /*
        I/O Command set Attributes
    */
    uint8_t rsvd7[1344];    /* Reserved as of Nvm Express 1.1 Spec */

    /*
        Power State Descriptors
    */
    NVME_PSDESCRIPTOR ps_descriptor[32];
    uint8_t  vendor_data[1024]; /* Vendor specific data */
} NVME_ADMIN_CONTROLLER_DATA;


/* **************************** */
/*      NVME Commands           */
/* **************************** */


struct nvme_create_cq {
    uint8_t             opcode;
    uint8_t             flags;
    uint16_t            command_id;
    uint32_t            rsvd1[5];
    uint64_t            prp1;
    uint64_t            rsvd8;
    uint16_t            cqid;
    uint16_t            qsize;
    uint16_t            cq_flags;
    uint16_t            irq_vector;
    uint32_t            rsvd12[4];
};

struct nvme_create_sq {
    uint8_t             opcode;
    uint8_t             flags;
    uint16_t            command_id;
    uint32_t            rsvd1[5];
    uint64_t            prp1;
    uint64_t            rsvd8;
    uint16_t            sqid;
    uint16_t            qsize;
    uint16_t            sq_flags;
    uint16_t            cqid;
    uint32_t            rsvd12[4];
};


struct nvme_read_ssd {
    uint8_t             opcode;
    uint8_t             flags;
    uint16_t            command_id;
    uint32_t            nsid;
    uint64_t            rsvd8;
    uint64_t            mptr;
    uint64_t            prp1;
    uint64_t            prp2;
    uint32_t            src_addr_lo;
    uint32_t            src_addr_hi;
    uint16_t            num_blocks;
    uint16_t            config;
    uint8_t             dataset_mgmt;
    uint8_t             rsvd12[3];
    uint32_t            ExptRefTag;
    uint16_t            ExpAppTag;
    uint16_t            ExpAppTagMask;
};

struct nvme_write_ssd {
    uint8_t             opcode;
    uint8_t             flags;
    uint16_t            command_id;
    uint32_t            nsid;
    uint64_t            rsvd8;
    uint64_t            mptr;
    uint64_t            prp1; /* SRC */
    uint64_t            prp2;
    uint32_t            dst_addr_lo;
    uint32_t            dst_addr_hi;
    uint16_t            num_blocks;
    uint16_t            config;
    uint8_t             dataset_mgmt;
    uint8_t             rsvd12;
    uint16_t            Dspec;
    uint32_t            initailRefTag;
    uint16_t            ExpAppTag;
    uint16_t            ExpAppTagMask;
};

struct set_feature_num_queue
{
    uint8_t             opcode;
    uint8_t             flags;
    uint16_t            command_id;
    uint32_t            rsvd1[9];
    uint8_t             feature_id;
    uint8_t             rsvd2[3];
    uint16_t            num_subq;
    uint16_t            num_compq;
    uint32_t            rsvd3[4];
};

struct nvme_identify
{
    uint8_t     opcode;
    uint8_t     flags;
    uint16_t    command_id;
    uint32_t    nsid;
    uint32_t    rsvd20[4];
    uint64_t    prp1;
    uint64_t    prp2;
    uint8_t     CNS;
    uint8_t     revd1;
    uint16_t    CNTID;
    uint16_t    nvmeSetId;
    uint16_t    rsvd2;
    uint32_t    rsvdDW[4];
};


struct nvme_set_features
{
    union
    {
        struct set_feature_num_queue set_num_io_q;
    };
};

typedef struct
{
    uint32_t dword0;
    uint32_t dword1;
    uint16_t sq_head;
    uint16_t sq_id;
    uint16_t command_id;
    uint16_t status; /* Status + Phase */
}nvmeComp_t;

typedef struct
{
    union
    {
        struct nvme_create_cq       create_cq;
        struct nvme_create_sq       create_sq;
        struct nvme_read_ssd        read_ssd;
        struct nvme_write_ssd       write_ssd;
        struct nvme_set_features    set_features;
        struct nvme_identify        identify;
    };
}nvmeCommand_t;

typedef struct
{
    nvmeCommand_t command[QUEUE_DEPTH];
} nvmeSubq_t;

typedef struct
{
    nvmeComp_t completion[QUEUE_DEPTH];
} nvmeCompq_t;


/* **************************** */
/*          NVME APIs           */
/* **************************** */


int32_t nvme_init();
uint32_t nvme_write(void *src,uint64_t dst, int32_t count);
uint32_t nvme_read(uint64_t src, void *dst, int32_t count);
