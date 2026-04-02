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
 *   @file  nvme.c
 *
 *   @brief
 *      This is implements the NVMe APIs.
 *
 */

#include <ti/drv/pcie/example/ssd/src/nvme.h>
#include <ti/osal/osal.h>

#include <stdlib.h>
#include <malloc.h>

#define LAST(k,n) ((k) & ((1<<(n))-1))
#define MID(k,m,n) LAST((k)>>(m),((n)-(m)))
#define MPS 4096

nvmeSubq_t admin_subq __attribute__ ((aligned (MPS)));
nvmeCompq_t admin_compq __attribute__ ((aligned (MPS)));
nvmeSubq_t io_subq __attribute__ ((aligned (MPS)));
nvmeCompq_t io_compq __attribute__ ((aligned (MPS)));

int32_t gAdminsubq_tail = 0;
int32_t gAdmincompq_head = 0;
int32_t gAdminPhaseTag = 0;

int32_t gIOsubq_tail = 0;
int32_t gIOcompq_head = 0;
int32_t gIOPhaseTag = 0;

uint16_t gCMD_id = 1;


void dump_size()
{
    NVME_printf("Size of admin_subq = %d bytes\n", sizeof(admin_subq));
    NVME_printf("Size of command structure is %d bytes\n", sizeof(nvmeCommand_t));

    NVME_printf("Size of admin_compq = %d bytes\n", sizeof(admin_compq));
    NVME_printf("Size of command structure is %d bytes\n", sizeof(nvmeComp_t));
}


void dump_data(uint8_t *data, uint64_t size)
{
    int i;
    for(i=0; i<size; i++)
    {
        NVME_printf("DATA at byte[%d] -->0x%x\n",i,data[i]);
    }
}

void dump_status(uint16_t status)
{
    uint16_t sct, sc;
    uint16_t cmd_spec = 0;

    if(status%2 == 0)
        NVME_printf("Phase Tag: 0\n");
    else
        NVME_printf("Phase Tag: 1\n");

    sct = MID(status,9,11);
    switch(sct)
    {
        case 0: NVME_printf("Genric command status\n");
                break;
        case 1: NVME_printf("Command specific status\n");
                cmd_spec = 1;
                break;
        default : NVME_printf("Check NVME spec for staus code\n");
                break;
    }

    sc = MID(status, 1,8);
    switch(sc)
    {
        case 0:
        if(cmd_spec)
            NVME_printf("Invalid completion queue\n");
        else
            NVME_printf("Command Completed without error\n");
        break;

        case 1:
        if(cmd_spec)
            NVME_printf("Invalid queue id\n");
        else
            NVME_printf("Invalid opcode\n");
        break;

        case 2:
        if(cmd_spec)
            NVME_printf("Invalid queue size\n");
        else
            NVME_printf("Invalid fiend in command\n");
        break;

        case 3:
        if(cmd_spec)
            NVME_printf("Abort command limit exceeded\n");
        else
            NVME_printf("Command id conflict\n");
        break;

        case 4:
        if(cmd_spec)
            NVME_printf("Reserved\n");
        else
            NVME_printf("Data Transfer error\n");
        break;

        case 5:
        if(cmd_spec)
            NVME_printf("Asynchronous Event Request Limit Exceeded\n");
        else
            NVME_printf("Command aborted due to power loss notification\n");
        break;

        case 6:
        if(cmd_spec)
            NVME_printf("Invalid Firmware Slot\n");
        else
            NVME_printf("Internel error\n");
        break;

        case 7:
        if(cmd_spec)
            NVME_printf("Invalid Firmware Image\n");
        else
            NVME_printf("Command Aborted due to coammnd abort request\n");
        break;

        case 8:
        if(cmd_spec)
            NVME_printf("Invalid interrupt vector\n");
        else
            NVME_printf("Command aborted due to submission queue deletion\n");
        break;

        case 9:
        if(cmd_spec)
            NVME_printf("Invalid log page\n");
        else
            NVME_printf("Failed fused command\n");
        break;

        case 10:
        if(cmd_spec)
            NVME_printf("Invalid format\n");
        else
            NVME_printf("Missing fused command\n");
        break;

        default:
            NVME_printf("Check Nvme spec for error code\n");
            break;
    }

}

uint64_t readReg64(uint64_t addr)
{
    uint64_t val1 = 0, val2 = 0, temp=0;
    val1 = (uint64_t)HW_RD_REG32(addr);

    val2 = (uint64_t)HW_RD_REG32(addr+4);

    temp = val2 << 32;
    val2 = val1 | temp;
    return val2;
}

static nvmeComp_t poll_completion_queue(nvmeCompq_t *compq, int32_t qid)
{
    uint32_t timeout = 0;
    uint64_t cap_reg, dstrd;
    int32_t head;

    head = (qid == 0) ?  gAdmincompq_head : gIOcompq_head;

    nvmeComp_t completion;
    memset(&completion, 0, sizeof(nvmeComp_t));

    cap_reg = readReg64(EP_CONFIG_BASE + NVME_REG_CAP);
    dstrd = MID(cap_reg, 32, 35);

    Osal_delay(10);
    do{
        timeout++;
        completion = compq->completion[head];
        if(completion.command_id == gCMD_id)
        {
            head++;
            if(head == QUEUE_DEPTH)
                head = 0;
            HW_WR_REG32(EP_CONFIG_BASE+NVME_REG_DBS+((2*qid + 1)*(1<<(2+dstrd))),head);
            break;
        }
    }while(timeout < 4000);

    if(qid == 0)
    {
        gAdmincompq_head = head;
    }
    else
    {
        gIOcompq_head = head;
    }

    if(timeout >= 4000)
    {
        NVME_printf("Command timeout\n");
        dump_status(completion.status);
    }
    return completion;
}

static inline void lo_hi_writeq(uint64_t val, uint32_t addr)
{
    HW_WR_REG32(addr, val);
    HW_WR_REG32(addr + 4, val >> 32);
}

void init_subqueue(nvmeSubq_t *subq)
{
    memset(subq->command, 0, sizeof(subq->command));
}

void init_Compqueue(nvmeCompq_t *compq)
{
    memset(compq->completion, 0, sizeof(compq->completion));
}

int32_t configure_admin_queues(nvmeSubq_t *subq, nvmeCompq_t *compq)
{
    volatile uint32_t val;

    /* Disable the controller */
    val = HW_RD_REG32(EP_CONFIG_BASE+NVME_REG_CC);
    val = val&0xFFFFFFFE;
    HW_WR_REG32(EP_CONFIG_BASE+NVME_REG_CC, val);

    /* Wait for CSTS.RDY to be zero */
    /* NVME_printf("\nWaiting for CSTS RDY to become 0\n"); */
    do
    {
        val = HW_RD_REG32(EP_CONFIG_BASE+NVME_REG_CSTS);
    }while(val%2 != 0);


    /* Initialize the subQ */
    init_subqueue(subq);

    /* Initialize the compQ */
    init_Compqueue(compq);


    /* Write to AQA reg */
    val = QUEUE_DEPTH - 1;
    val |= val << 16;
    HW_WR_REG32((EP_CONFIG_BASE+NVME_REG_AQA), val);

    /* writing the submission queue base addr */
    lo_hi_writeq((uint64_t)subq,EP_CONFIG_BASE+NVME_REG_ASQ);
    lo_hi_writeq((uint64_t)compq,EP_CONFIG_BASE+NVME_REG_ACQ);

    /* Enabling the controller */
    val = HW_RD_REG32(EP_CONFIG_BASE+NVME_REG_CC);
    val = 0x460001;
    HW_WR_REG32(EP_CONFIG_BASE+NVME_REG_CC, val);

    val = HW_RD_REG32(EP_CONFIG_BASE+NVME_REG_CC);
    /* NVME_printf("CC VAl after enable: 0x%x", val);
       NVME_printf("\nWaiting for CSTS RDY to become 1\n"); */

    do
    {
        val = HW_RD_REG32(EP_CONFIG_BASE+NVME_REG_CSTS);
        NVME_printf("NVME_REG_CSTS Value : 0x%x \n", val);
    }while(val%2 !=1 );

    NVME_printf("\nAdmin queue creation complete\n");

    return NVME_OK;
}

int32_t nvme_submit_cmd(nvmeCommand_t cmd, nvmeSubq_t *subq, int32_t qid)
{
    uint32_t dstrd;
    uint64_t cap_reg;
    int32_t tail;

    if(qid == 0)
        tail = gAdminsubq_tail;
    else
        tail = gIOsubq_tail;

    /*NVME_printf("\n\nSubmititng the command\n");*/

    subq->command[tail] = cmd;
    tail++;
    if (tail == QUEUE_DEPTH)
        tail = 0;

    /* Read the cap register */
    cap_reg = readReg64(EP_CONFIG_BASE + NVME_REG_CAP);
    dstrd = MID(cap_reg, 32, 35);

    /* ring the dorrbell */
    HW_WR_REG32(EP_CONFIG_BASE+NVME_REG_DBS+((2*qid)*(1<<(2+dstrd))),tail);

    if(qid == 0)
        gAdminsubq_tail = tail;
    else
        gIOsubq_tail = tail;
    return NVME_OK;
}

int32_t identify_cmd()
{
    NVME_ADMIN_CONTROLLER_DATA *ctrl_data;
    nvmeCommand_t cmd;

    ctrl_data = (NVME_ADMIN_CONTROLLER_DATA *) memalign(MPS, sizeof(NVME_ADMIN_CONTROLLER_DATA));

    if(!ctrl_data)
    {
        NVME_printf("Memory not allocated for controller data\n");
        NVME_printf("Size of ctrl_data is: %d", sizeof(NVME_ADMIN_CONTROLLER_DATA));
    }
    memset(&cmd,0, sizeof(cmd));
    cmd.identify.opcode = nvme_admin_identify;
    cmd.identify.command_id = ++gCMD_id;
    cmd.identify.prp1 = (uintptr_t) ctrl_data;
    cmd.identify.CNS = 1;
    cmd.identify.nsid = 0;

    nvme_submit_cmd(cmd,&admin_subq,0);
    poll_completion_queue(&admin_compq, 0);

    ctrl_data->sn[19] = 0;
    ctrl_data->mn[39] = 0;
    NVME_printf(" == NVME IDENTIFY CONTROLLER DATA ==\n");
    NVME_printf("    PCI VID   : 0x%x\n", ctrl_data->vid);
    NVME_printf("    PCI SSVID : 0x%x\n", ctrl_data->ssvid);
    NVME_printf("    SN        : %s\n",   (char *)(ctrl_data->sn));
    NVME_printf("    MN        : %s\n",   (char *)(ctrl_data->mn));
    NVME_printf("    RAB       : 0x%x\n", ctrl_data->rab);
    NVME_printf("    AERL      : 0x%x\n", ctrl_data->aerl);
    NVME_printf("    SQES      : 0x%x\n", ctrl_data->sqes);
    NVME_printf("    CQES      : 0x%x\n", ctrl_data->cqes);
    NVME_printf("    NN        : 0x%x\n", ctrl_data->nn);
    NVME_printf("Identified NVMe model %s\n",(char *)ctrl_data->mn);

    return NVME_OK;
}


int32_t reuest_io_queues(int16_t num_subq, int16_t num_compq)
{
    int32_t retval = NVME_OK;
    uint16_t allocated_subq, allocated_compq;
    nvmeCommand_t cmd;
    nvmeComp_t completion;
    memset(&cmd, 0, sizeof(cmd));
    cmd.set_features.set_num_io_q.opcode = nvme_admin_set_features;
    cmd.set_features.set_num_io_q.flags = 0x0;
    cmd.set_features.set_num_io_q.command_id = ++gCMD_id;
    cmd.set_features.set_num_io_q.feature_id = 0x07;
    cmd.set_features.set_num_io_q.num_subq = 0x8;
    cmd.set_features.set_num_io_q.num_compq = 0x8;

    nvme_submit_cmd(cmd,&admin_subq,0);
    completion = poll_completion_queue(&admin_compq, 0);
    if(completion.dword0 == 0)
    {
        NVME_printf("No Queues are allocated by controller\n");
        NVME_printf("Cannot create IO queues\n");
        return -NVME_NO_Q_ALLOCATED;
    }
    else
    {
        allocated_subq = (uint16_t)completion.dword0;
        allocated_compq = (uint16_t)(completion.dword0 >> 16);
        NVME_printf("Maximum IO submission queues allowed by controller: %d\n",allocated_subq+1);
        NVME_printf("Maximum IO completion queues allowed by controller: %d\n",allocated_compq+1);
    }
    return retval;
}

int32_t create_io_queues(nvmeSubq_t *admin_subq, nvmeCompq_t *admin_compq)
{
    nvmeCommand_t cmd;
    nvmeCommand_t cmd2;

    /* request queues */
    reuest_io_queues(8,8);

    /* Initialize the subQ */
    init_subqueue(&io_subq);

    init_Compqueue(&io_compq);

    /* Initialize completion queue command */
    memset(&cmd, 0, sizeof(cmd));
    cmd.create_cq.opcode = nvme_admin_create_cq;
    cmd.create_cq.flags = 0;
    cmd.create_cq.command_id = ++gCMD_id;
    cmd.create_cq.prp1 = (uint64_t)(&io_compq);
    cmd.create_cq.cqid = 1;
    cmd.create_cq.qsize = QUEUE_DEPTH - 1;
    cmd.create_cq.cq_flags = 0x1;
    cmd.create_cq.irq_vector = 0x0;

    nvme_submit_cmd(cmd,admin_subq,0);
    poll_completion_queue(admin_compq,0);

    /* Initialize submission queue command */
    memset(&cmd2, 0, sizeof(cmd2));
    cmd2.create_sq.opcode = nvme_admin_create_sq;
    cmd2.create_sq.flags = 0;
    cmd2.create_sq.command_id = ++gCMD_id;
    cmd2.create_sq.prp1 = (uint64_t)(&io_subq);
    cmd2.create_sq.sqid = 1;
    cmd2.create_sq.cqid = 1;
    cmd2.create_sq.qsize = QUEUE_DEPTH - 1;
    cmd2.create_sq.sq_flags = 0x5;

    nvme_submit_cmd(cmd2,admin_subq,0);
    poll_completion_queue(admin_compq,0);

    return NVME_OK;
}

uint32_t nvme_write(void *src,uint64_t dst, int32_t num_blocks)
{
    nvmeCommand_t write_cmd;
    //uint8_t srcLocal[BLOCK_SIZE] __attribute__ ((aligned (MPS)));
    uint64_t dstAddr;
    dstAddr = (uint64_t)dst;
    num_blocks--;
    memset(&write_cmd,0,sizeof(write_cmd));
    write_cmd.write_ssd.opcode = nvme_io_write;
    write_cmd.write_ssd.flags = 0x0;
    write_cmd.write_ssd.command_id = ++gCMD_id;
    write_cmd.write_ssd.nsid = 1;
    write_cmd.write_ssd.prp1 = (uint64_t)src;
    write_cmd.write_ssd.dst_addr_lo = (uint32_t)dstAddr;
    write_cmd.write_ssd.dst_addr_hi = (uint32_t)(dstAddr>>32);
    write_cmd.write_ssd.num_blocks = num_blocks;
    write_cmd.write_ssd.config = 0x2;
    nvme_submit_cmd(write_cmd,&io_subq,1);
    poll_completion_queue(&io_compq,1);
    return NVME_OK;
}

uint32_t nvme_read(uint64_t src, void *dst, int32_t num_blocks)
{
    nvmeCommand_t read_cmd;
    uint64_t srcAddr;
    srcAddr = (uint64_t)src;
    num_blocks--;
    memset(&read_cmd, 0, sizeof(read_cmd));
    read_cmd.read_ssd.opcode = nvme_io_read;
    read_cmd.read_ssd.flags = 0x0;
    read_cmd.read_ssd.nsid = 1;
    read_cmd.read_ssd.command_id = ++gCMD_id;
    read_cmd.read_ssd.prp1 = (uint64_t)dst;
    read_cmd.read_ssd.src_addr_lo = (uint32_t)srcAddr;
    read_cmd.read_ssd.src_addr_hi = (uint32_t)(srcAddr>>32);
    read_cmd.read_ssd.num_blocks =num_blocks;
    read_cmd.read_ssd.config = 0x2;
    nvme_submit_cmd(read_cmd,&io_subq,1);
    poll_completion_queue(&io_compq,1);
    return NVME_OK;
}

int32_t nvme_init()
{
    int32_t retval;

    dump_size();

    retval = configure_admin_queues(&admin_subq,&admin_compq);
    if(retval)
    {
     NVME_printf("Admin queue creation failed\n");
     return -NVME_ADMIN_QUEUE_ERR;
    }

   /* Identify command to get conroller data */
   identify_cmd();

   retval = create_io_queues(&admin_subq,&admin_compq);
   if(retval)
   {
       NVME_printf("I/O queue creation failed\n");
       return -NVME_IO_Q_FAIL;
   }

   NVME_printf("NVME INIT Complete\n");

   return 0;
}
