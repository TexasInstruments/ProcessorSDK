/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021-2023
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

/* FreeRTOS Header files */
#include <ti/osal/osal.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/HwiP.h>

/* CSL Header files */
#include <ti/csl/soc.h>
#include <ti/csl/arch/r5/csl_arm_r5.h>
#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include <ti/csl/csl_gpio.h>
#include <ti/csl/hw_types.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* BOARD Header files */
#include <ti/board/board.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2))) && !(defined (NO_SCISERVER))
#include <ti/drv/sciclient/sciserver.h>
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

#ifndef UART_ENABLED

#ifdef BUILD_MCU2_0
#define UART0_ADDR ((int)0x02800000)
#else
#define UART0_ADDR  ((int)0x40A00000)
#endif

#define UART_RHR    ((int)0x00U)
#define UART_LSR    ((int)0x14U)
#define UART_RHR_REG_ADDR   ((volatile unsigned int *)(UART0_ADDR + UART_RHR))
#define UART_LSR_REG_ADDR   ((volatile unsigned int *)(UART0_ADDR + UART_LSR))

#ifdef BUILD_MCU2_0
__attribute__((section("sbl_mcu_2_0_resetvector"))) void sbl_putc(unsigned char c);
__attribute__((section("sbl_mcu_2_0_resetvector"))) void sbl_puts(char *str);
#elif BUILD_MCU1_0
__attribute__((section("sbl_mcu_1_0_resetvector"))) void sbl_putc(unsigned char c);
__attribute__((section("sbl_mcu_1_0_resetvector"))) void sbl_puts(char *str);
#endif

void sbl_putc(unsigned char c)
{
    while((*UART_LSR_REG_ADDR & 0x20) == 0);
    *UART_RHR_REG_ADDR = c;
}
void sbl_puts(char *str)
{
    for (; *str != '\0'; str++ )
        sbl_putc(*str);
}
#endif
#include <ti/drv/uart/UART_stdio.h>
static void AppUtils_Printf (const char *pcString, ...);
Board_STATUS Board_uartStdioInit(void);


uint32_t AppUtils_getElapsedTimeInUsec(uint32_t startTime);
uint32_t AppUtils_getCurTimeInUsec(void);

void memoryBenchmarking_initSciclient();

#define APP_TSK_STACK_MAIN (32U * 1024U)
#define APP_SCISERVER_INIT_TSK_STACK (32U * 1024U)

#if defined(BUILD_MCU1_0)
    static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN];
    static uint8_t  gSciserverInitTskStack[APP_SCISERVER_INIT_TSK_STACK];
#endif

/**< Initialize SCI Server, to process RM/PM Requests by other cores */

#define NUM_TASK 16
#define NUM_TEST 10
#define BUF_SIZE 2048
//#define MEM_CPY_OPER 2400000
#define MEM_CPY_OPER 500 // min > 10 msec
#define BUFFER_IN_USE 2
#define TASK_STACK_SIZE 0x1000


// Types_FreqHz freq1;

/* task_calls is the number of random calls to the slave tasks for different
 * test cases. This can be used to controll the runtime of the code
*/
uint32_t task_calls = 200;

/* For each test case the  size of the memcpy buffer can be defined
 * individually. Lesser the value more is the number of cache misses per
 * second but reduced run time as the number of instructions executed
 * reduces and the memcpy instructions still remain in the cache */
uint32_t memcopy_size_arr[NUM_TEST] = {0, 50, 100, 200, 500, 750, 1000, 1250, \
                                        1500, BUF_SIZE};

/* Variable to pick up value from the memcopy_size_arr for each test*/
uint32_t memcopy_size = 0;

/* Number of times each slave task must repeat the operation.
 * This can be used the control the execution time of the code without much
 * impact on the cache misses
*/
uint32_t iter = 1;

/* Counter for the number of sysbios task switches that occur during the
 * execution of the code
*/
uint32_t num_switches = 0;

QueueP_Handle myQ[NUM_TASK];
TaskP_Handle main_task[NUM_TASK];

/* Array to hold all the semaphores */
void *gSemaphorePHandle[NUM_TASK];
void *gSemaphorePHandle_task;

/* Array to hold the address of all the source buffers*/
uint32_t *buf[NUM_TASK];

#if defined(BUILD_MCU1_0)

void memoryBenchmarking_setupSciServer(void *arg0, void *arg1)
{

    Sciserver_TirtosCfgPrms_t appPrms;
    int32_t ret = CSL_PASS;

    ret = Sciserver_tirtosInitPrms_Init(&appPrms);

    appPrms.taskPriority[SCISERVER_TASK_USER_LO] = 4;
    appPrms.taskPriority[SCISERVER_TASK_USER_HI] = 5;

    if (ret == CSL_PASS)
    {
        ret = Sciserver_tirtosInit(&appPrms);
    }

    if (ret != CSL_PASS)
    {
        AppUtils_Printf("Starting Sciserver..... FAILED\n");
    }

    return;
}

#endif

/* Source buffers for all the memcpy operations. They can lie in the OCMC
 * or the same memory as the code like flash. The location can be changed
 * from the linker file
*/

uint32_t buf_0[BUF_SIZE] __attribute__((section(".buf_0"))) ;
uint32_t buf_1[BUF_SIZE] __attribute__((section(".buf_1"))) ;
uint32_t buf_2[BUF_SIZE] __attribute__((section(".buf_2"))) ;
uint32_t buf_3[BUF_SIZE] __attribute__((section(".buf_3"))) ;
uint32_t buf_4[BUF_SIZE] __attribute__((section(".buf_4"))) ;
uint32_t buf_5[BUF_SIZE] __attribute__((section(".buf_5"))) ;
uint32_t buf_6[BUF_SIZE] __attribute__((section(".buf_6"))) ;
uint32_t buf_7[BUF_SIZE] __attribute__((section(".buf_7"))) ;
uint32_t buf_8[BUF_SIZE] __attribute__((section(".buf_8"))) ;
uint32_t buf_9[BUF_SIZE] __attribute__((section(".buf_9"))) ;
uint32_t buf_10[BUF_SIZE] __attribute__((section(".buf_10"))) ;
uint32_t buf_11[BUF_SIZE] __attribute__((section(".buf_11"))) ;
uint32_t buf_12[BUF_SIZE] __attribute__((section(".buf_12"))) ;
uint32_t buf_13[BUF_SIZE] __attribute__((section(".buf_13"))) ;
uint32_t buf_14[BUF_SIZE] __attribute__((section(".buf_14"))) ;
uint32_t buf_15[BUF_SIZE] __attribute__((section(".buf_15"))) ;

/* The target buffer for all the memcpy operations */
uint32_t buf_ocmc[BUF_SIZE] __attribute__((section(".buf_cpy")));

/* Slave Task Function Definition. All the tasks are same
 * functionally. The only differnce is their location in the memory. They
 * are placed in the memory such that all of them occupy the same cache entry
 * in the 4-way cahce. The sections are defined in the linker cmd file*/

void SlaveTaskFxn_0(void * a0, void * a1)  __attribute__((section(".task_0")));
void SlaveTaskFxn_1(void * a0, void * a1)  __attribute__((section(".task_1")));
void SlaveTaskFxn_2(void * a0, void * a1)  __attribute__((section(".task_2")));
void SlaveTaskFxn_3(void * a0, void * a1)  __attribute__((section(".task_3")));
void SlaveTaskFxn_4(void * a0, void * a1)  __attribute__((section(".task_4")));
void SlaveTaskFxn_5(void * a0, void * a1)  __attribute__((section(".task_5")));
void SlaveTaskFxn_6(void * a0, void * a1)  __attribute__((section(".task_6")));
void SlaveTaskFxn_7(void * a0, void * a1)  __attribute__((section(".task_7")));
void SlaveTaskFxn_8(void * a0, void * a1)  __attribute__((section(".task_8")));
void SlaveTaskFxn_9(void * a0, void * a1)  __attribute__((section(".task_9")));
void SlaveTaskFxn_10(void * a0, void * a1)  __attribute__((section(".task_10")));
void SlaveTaskFxn_11(void * a0, void * a1)  __attribute__((section(".task_11")));
void SlaveTaskFxn_12(void * a0, void * a1)  __attribute__((section(".task_12")));
void SlaveTaskFxn_13(void * a0, void * a1)  __attribute__((section(".task_13")));
void SlaveTaskFxn_14(void * a0, void * a1)  __attribute__((section(".task_14")));
void SlaveTaskFxn_15(void * a0, void * a1)  __attribute__((section(".task_15")));


static uint8_t MainApp_TaskStack[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_0[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_1[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_2[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_3[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_4[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_5[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_6[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_7[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_8[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_9[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_10[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_11[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_12[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_13[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_14[TASK_STACK_SIZE] __attribute__((aligned(32)));
static uint8_t SlaveTaskStack_15[TASK_STACK_SIZE] __attribute__((aligned(32)));


typedef struct
{
    QueueP_Elem elem;
    uint32_t task_call_number;
}msg;

#ifdef BUILD_MCU2_0
#if (defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined (SOC_J742S2))
void _system_post_cinit(void) __attribute__((section(".ratConf")));
void _system_post_cinit(void)
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = true;
    osalArch_Init(&cfg);

    /* Setup RAT to load data into MCU2_0 OCM RAM for MCU1_0 */
    /* This is mapping the OCM RAM for MCU2_0 (a 40 bit address) Main domain to 0xD0000000 */
#define RAT_BASE (0x0FF90000)
#define REGION_ID (0x0)
    *(unsigned int *)(RAT_BASE + 0x44 + (REGION_ID*0x10)) = 0xD0000000; //IN ADDRESS
    *(unsigned int *)(RAT_BASE + 0x48 + (REGION_ID*0x10)) = 0x02000000;
    *(unsigned int *)(RAT_BASE + 0x4C + (REGION_ID*0x10)) = 0x0000004F; //Upper 16 bits of the real physical address.
    *(unsigned int *)(RAT_BASE + 0x40 + (REGION_ID*0x10)) = 0x80000013;
}
#endif
#endif

void multicore_wait() __attribute__ ((optnone))
{
/* Hold MCU1_0 for some time to avoid race condition b/w MCU1_0 and MCU2_0 while reading/writing to GPIO pins */
#if defined(BUILD_MCU1_0) && defined(MULTICORE)
    Osal_delay(500);
#endif
    
/* Using a GPIO pin for syncronization */
#ifdef MULTICORE
    uint32_t gpio_pin_num = 2U;
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, gpio_pin_num, GPIO_DIRECTION_OUTPUT);
#ifdef BUILD_MCU1_0
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, gpio_pin_num, 1);
    while (GPIOPinRead_v0(CSL_WKUP_GPIO0_BASE, gpio_pin_num))
    {}
#else
    while (!GPIOPinRead_v0(CSL_WKUP_GPIO0_BASE, gpio_pin_num))
    {}
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, gpio_pin_num, 0);
#endif
#endif
}

#define CMPLX_FNCT_1 \
                denominator = 2 / buf_ocmc[33] + x1; \
                sinx = sinx * 1.23454 - buf_ocmc[34]; \
                buf_ocmc[55] = buf_ocmc[55] / buf_ocmc[43]; \
                buf_ocmc[33] = buf_ocmc[38] / buf_ocmc[23]; \
                buf_ocmc[44] = buf_ocmc[26] / buf_ocmc[28] / buf_ocmc[56] * 1.235; \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \
                denominator = 2 / buf_ocmc[33] + x1; \
                sinx = sinx * 1.23454 - buf_ocmc[34]; \
                buf_ocmc[55] = buf_ocmc[55] / buf_ocmc[43]; \
                buf_ocmc[33] = buf_ocmc[38] / buf_ocmc[23]; \
                buf_ocmc[44] = buf_ocmc[26] / buf_ocmc[28] / buf_ocmc[56] * 1.235; \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \
                denominator = 2 / buf_ocmc[33] + x1; \
                sinx = sinx * 1.23454 - buf_ocmc[34]; \
                buf_ocmc[55] = buf_ocmc[55] / buf_ocmc[43]; \
                buf_ocmc[33] = buf_ocmc[38] / buf_ocmc[23]; \
                buf_ocmc[44] = buf_ocmc[26] / buf_ocmc[28] / buf_ocmc[56] * 1.235; \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \
                denominator = 2 / buf_ocmc[33] + x1; \
                sinx = sinx * 1.23454 - buf_ocmc[34]; \

#define CMPLX_FNCT_2 \
                buf_ocmc[53] = buf_ocmc[53] + 1; \
                buf_ocmc[54] = buf_ocmc[54] + 1; \
                buf_ocmc[55] = buf_ocmc[55] + 1; \
                buf_ocmc[56] = buf_ocmc[56] + 1; \
                buf_ocmc[57] = buf_ocmc[57] + 1; \
                buf_ocmc[58] = buf_ocmc[58] + 1; \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \

#define CMPLX_FNCT_3 \
                buf_ocmc[53] = buf_ocmc[53] + 1; \
                buf_ocmc[54] = buf_ocmc[54] + 1; \
                buf_ocmc[55] = buf_ocmc[55] + 1; \
                buf_ocmc[56] = buf_ocmc[56] + 1; \
                buf_ocmc[57] = buf_ocmc[57] + 1; \
                buf_ocmc[58] = buf_ocmc[58] + 1; \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \

#define CMPLX_FNCT_4 \
                buf_ocmc[43] = buf_ocmc[53] + 1; \
                buf_ocmc[44] = buf_ocmc[54] + 1; \
                buf_ocmc[45] = buf_ocmc[55] + 1; \
                buf_ocmc[46] = buf_ocmc[56] + 1; \
                buf_ocmc[47] = buf_ocmc[57] + 1; \
                buf_ocmc[48] = buf_ocmc[58] + 1; \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \

/* The fucntion definition for all the tasks.  */
#define TSKFN \
    msg* rp; \
    int i,j, arr_number,m; \
    while(1) \
    { \
        SemaphoreP_pend((SemaphoreP_Handle)gSemaphorePHandle[(uint32_t)a0], osal_WAIT_FOREVER); \
        while (QueueP_EMPTY != QueueP_isEmpty(myQ[(uint32_t)a0])) \
        { \
            rp = QueueP_get(myQ[(uint32_t)a0]); \
            for (i = 0; i < iter; ++i) \
            { \
                arr_number = get_rand() % BUFFER_IN_USE; \
                for (j = 0; j < memcopy_size; ++j) \
                { \
                    buf_ocmc[j] = buf[arr_number][j]; \
                } \
                /*sin calculation*/ \
                float denominator, sinx;  \
                float n = 30;        \
                n = n * (3.142 / 180.0);   \
                float x1 = n;  \
                sinx = n;           \
                m = 1;  \
                denominator = 2 * m * (2 * m + 1);  \
                x1 = -x1 * n * n / denominator;  \
                sinx = sinx + x1;  \
                m =  + 1;  \
                buf_ocmc[23] = buf_ocmc[23] + 1; \
                buf_ocmc[24] = buf_ocmc[24] + 1; \
                buf_ocmc[25] = buf_ocmc[25] + 1; \
                buf_ocmc[26] = buf_ocmc[26] + 1; \
                buf_ocmc[27] = buf_ocmc[27] + 1; \
                buf_ocmc[28] = buf_ocmc[28] + 1; \
                buf_ocmc[29] = buf_ocmc[29] + 1; \
                buf_ocmc[30] = buf_ocmc[30] + 1; \
                buf_ocmc[31] = buf_ocmc[31] + 1; \
                buf_ocmc[32] = buf_ocmc[32] + 1; \
                buf_ocmc[33] = buf_ocmc[33] + 1; \
                buf_ocmc[34] = buf_ocmc[34] + 1; \
                buf_ocmc[35] = buf_ocmc[35] + 1; \
                buf_ocmc[36] = buf_ocmc[36] + 1; \
                buf_ocmc[37] = buf_ocmc[37] + 1; \
                buf_ocmc[38] = buf_ocmc[38] + 1; \
                buf_ocmc[39] = buf_ocmc[39] + 1; \
                buf_ocmc[40] = buf_ocmc[40] + 1; \
                buf_ocmc[41] = buf_ocmc[41] + 1; \
                buf_ocmc[42] = buf_ocmc[42] + 1; \
                buf_ocmc[43] = buf_ocmc[43] + 1; \
                buf_ocmc[44] = buf_ocmc[44] + 1; \
                buf_ocmc[45] = buf_ocmc[45] + 1; \
                buf_ocmc[46] = buf_ocmc[46] + 1; \
                buf_ocmc[47] = buf_ocmc[47] + 1; \
                buf_ocmc[48] = buf_ocmc[48] + 1; \
                buf_ocmc[49] = buf_ocmc[49] + 1; \
                buf_ocmc[50] = buf_ocmc[50] + 1; \
                buf_ocmc[51] = buf_ocmc[51] + 1; \
                buf_ocmc[52] = buf_ocmc[52] + 1; \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_4 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_4 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_4 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_2 \
                CMPLX_FNCT_1 \
                CMPLX_FNCT_3 \
                CMPLX_FNCT_3 \
            } \
            rp->task_call_number = rp->task_call_number + 1; \
        } \
        SemaphoreP_post((SemaphoreP_Handle)gSemaphorePHandle[(uint32_t)a0]); \
        TaskP_yield(); \
    }

/* The hook function defiend to be called at every task switch to count the
 * number of task switches per test  case
*/
// void myswitchFxn(TaskP_Handle prev, TaskP_Handle next)
// {
//     num_switches++;
// }

/* Get a random number depending upon the number of tasks*/
uint32_t get_rand()
{
    return (rand()%NUM_TASK);
}

uint32_t            hrs, mins, secs, durationInSecs, usecs;
uint32_t            startTime, elapsedTime;

/* Master task which will call the slave tasks randomly and */
void MasterTask(void * a0, void * a1)
{
    AppUtils_Printf("\n\rmaster_task\n\r");
    uint32_t icm, ica, icnt;
    msg r[NUM_TASK];
    int i, j;
    uint32_t set = 0, way = 0;


    for (i = 0; i < NUM_TASK; ++i)
    {
        myQ[i] = QueueP_create(NULL);
    }

    AppUtils_Printf("\n\rmaster_task -- start sending\n\r");

    uint32_t count = 0;
    /* this loop is for NUM_TEST with the specified task calls and memcpy_size*/
    for (i = 0; i < NUM_TEST; ++i)
    {
        /* counter to track the task calls already made*/
        count = 0;
        /*reset the value of task switch for that test*/
        num_switches = 0;
        /* number of times each task should repeat its operation*/
        iter = 1;

        /* size of the memcpy to be performed by each task*/
        memcopy_size = memcopy_size_arr[i];
        /*invalidate all the cache to get fresh and reliable data*/
        uint32_t numSets = CSL_armR5CacheGetNumSets();
        uint32_t numWays = CSL_armR5CacheGetNumWays();
	    for (set = 0; set < numSets; set ++)
        {
            for (way = 0; way < numWays; way++)
            {
                CSL_armR5CacheCleanInvalidateDcacheSetWay(set, way);
            }
        }



        CSL_armR5CacheInvalidateAllIcache();

        /* reset the PMU counters to get relevant data */
        CSL_armR5PmuResetCntrs();
        /* Wait for the cores to sync up (empty function if MULTICORE is not defined) */
        multicore_wait();
        /* Hold MCU1_0 for some time to avoid race condition b/w MCU1_0 and MCU2_0 while reading/writing to GPIO pins */
        #if defined(BUILD_MCU1_0) && defined(MULTICORE)
            Osal_delay(500);
        #endif
        startTime = AppUtils_getCurTimeInUsec();

        /*start sending signals and messages to tasks*/
        while(count < task_calls)
        {
            /*invalidate all the cache to get fresh and reliable data*/
            numSets = CSL_armR5CacheGetNumSets();
            numWays = CSL_armR5CacheGetNumWays();
            for (set = 0; set < numSets; set ++)
            {
                for (way = 0; way < numWays; way++)
                {
                    CSL_armR5CacheCleanInvalidateDcacheSetWay(set, way);
                }
            }
            CSL_armR5CacheInvalidateAllIcache();
            /* Get a random task number*/
            j = get_rand();
            r[j].task_call_number = count++;
            /* Add the message to the queue of the task */
            QueueP_put(myQ[j], &(r[j].elem));
            /* Signal the task to start executing */
            SemaphoreP_post((SemaphoreP_Handle)gSemaphorePHandle[j]);
            /* Yield the CPU for the other task to execute*/
            TaskP_yield();
            /* Wait for the task to complete */
            SemaphoreP_pend((SemaphoreP_Handle)gSemaphorePHandle[j], osal_WAIT_FOREVER);
        }

        elapsedTime = AppUtils_getElapsedTimeInUsec(startTime);
        durationInSecs = ((elapsedTime) / 1000U);
        hrs  = durationInSecs / (60U * 60U);
        mins = (durationInSecs / 60U) - (hrs * 60U);
        secs = durationInSecs - (hrs * 60U * 60U) - (mins * 60U);
        usecs = elapsedTime - (((hrs * 60U * 60U) + (mins * 60U) + secs) * 1000000U);

        /* Read the value of the PMU counteres and print the result */
        icm = CSL_armR5PmuReadCntr(0);
        ica = CSL_armR5PmuReadCntr(2);
        // dcm = CSL_armR5PmuReadCntr(1);
	    icnt = CSL_armR5PmuReadCntr(1);
        AppUtils_Printf("\nMem Cpy Size    => %d\n", (unsigned int) memcopy_size);
        AppUtils_Printf("Start Time in Usec => %d\n", (unsigned int) startTime);
        AppUtils_Printf("Exec Time in Usec => %d\n", (unsigned int) elapsedTime);
        AppUtils_Printf("Iter            => %d\n", (unsigned int) iter);
        AppUtils_Printf("Task Calls      => %d\n",  task_calls);
        AppUtils_Printf("Inst Cache Miss => %d\n",  icm);
        AppUtils_Printf("Inst Cache Acc  => %d\n",  ica);
        AppUtils_Printf("Num Instr Exec  => %d\n",  icnt);

        /* I can't say that I understand this hack, but something happened in CLANG that broke the prints for unsigned prints - this does the trick. */
        AppUtils_Printf("ICM/sec         => %u\n", (unsigned int) (1000000 * ((float) (icm*1.0)/(elapsedTime*1.0))));  // (unsigned long)  ((uint64_t)icm*1000000)/((uint64_t)elapsedTime));
        printf("ICM/sec         => %u\n", (unsigned int) (1000000 * ((float) (icm*1.0)/(elapsedTime*1.0))));  // (unsigned long)  ((uint64_t)icm*1000000)/((uint64_t)elapsedTime));
        AppUtils_Printf("INST/sec        => %u\n", (unsigned int) (1000000 * ((float) (icnt*1.0)/(elapsedTime*1.0))));
        printf("INST/sec        => %u\n", (unsigned int) (1000000 * ((float) (icnt*1.0)/(elapsedTime*1.0))));  // (unsigned long)  ((uint64_t)icm*1000000)/((uint64_t)elapsedTime));
    }

    AppUtils_Printf("\nAll tests have passed\n");
    OS_stop();
}

int do_main(void)
{

    /* refer to r5 csl for PMU API references*/
    CSL_armR5PmuCfg(0, 0 ,1);
    CSL_armR5PmuEnableAllCntrs(1);

    CSL_armR5PmuCfgCntr(0, CSL_ARM_R5_PMU_EVENT_TYPE_ICACHE_MISS);
    CSL_armR5PmuCfgCntr(1, CSL_ARM_R5_PMU_EVENT_TYPE_I_X);
    CSL_armR5PmuCfgCntr(2, CSL_ARM_R5_PMU_EVENT_TYPE_ICACHE_ACCESS);

    CSL_armR5PmuEnableCntrOverflowIntr(0, 0);
    CSL_armR5PmuEnableCntrOverflowIntr(1, 0);
    CSL_armR5PmuEnableCntrOverflowIntr(2, 0);

    CSL_armR5PmuResetCntrs();

    CSL_armR5PmuEnableCntr(0, 1);
    CSL_armR5PmuEnableCntr(1, 1);
    CSL_armR5PmuEnableCntr(2, 1);

    int i,j;


    Board_STATUS boardInitStatus = 0;

#if defined(UART_ENABLED) && defined(BUILD_MCU1_0) && defined(MULTICORE)
    Board_initCfg cfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG_MCU | BOARD_INIT_MODULE_CLOCK_MCU;
#ifdef MULTICORE
    cfg = BOARD_INIT_UNLOCK_MMR | BOARD_INIT_UART_STDIO | 
               BOARD_INIT_MODULE_CLOCK | BOARD_INIT_PINMUX_CONFIG;
#endif
    /* Use below when trying to use CCS to boot MCU2_0 from DDR or MSMC */
    boardInitStatus = Board_init(cfg);
    //Board_initCfg cfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK;
#else
 boardInitStatus = Board_uartStdioInit();
#endif

#if defined(BUILD_OCMC)
    AppUtils_Printf("Calculating Benchmarks for OCMC memory ");
#elif defined(BUILD_MSMC)
    AppUtils_Printf("Calculating Benchmarks for MSMC memory ");
#elif defined(BUILD_DDR)
    AppUtils_Printf("Calculating Benchmarks for DDR memory ");
#elif defined(BUILD_XIP)
    AppUtils_Printf("Calculating Benchmarks for XIP memory ");
#else
    AppUtils_Printf("Unknown Memory Configuration \n");
#endif

#if defined(BUILD_MCU1_0)
    AppUtils_Printf("on MCU1_0 ");
#elif defined(BUILD_MCU2_0)
    AppUtils_Printf("on MCU2_0 ");
#else
    AppUtils_Printf("Unknown Core Configuration \n");
#endif

#if defined(MULTICORE)
    AppUtils_Printf("(multicore) \n");
#else
    AppUtils_Printf("(single core) \n");
#endif

    if (boardInitStatus != BOARD_SOK)
    {
        AppUtils_Printf("\nBoard_init failure\n");
        return(0);
    }
    AppUtils_Printf("\nBoard_init success\n");

    // Countdown for board init to be fully done
    unsigned long long cnt = 0;
    AppUtils_Printf("Countdown...\n");
    while (cnt++ < 5000000)
    {
        if ( (cnt%1000000) == 0 )
        {
            AppUtils_Printf("%d/5\n", (int)(cnt/1000000));
        }
    }

    /*task Function pointer array*/
    TaskP_Fxn tasks[NUM_TASK];
    tasks[0] = SlaveTaskFxn_0;
    tasks[1] = SlaveTaskFxn_1;
    tasks[2] = SlaveTaskFxn_2;
    tasks[3] = SlaveTaskFxn_3;
    tasks[4] = SlaveTaskFxn_4;
    tasks[5] = SlaveTaskFxn_5;
    tasks[6] = SlaveTaskFxn_6;
    tasks[7] = SlaveTaskFxn_7;
    tasks[8] = SlaveTaskFxn_8;
    tasks[9] = SlaveTaskFxn_9;
    tasks[10] = SlaveTaskFxn_10;
    tasks[11] = SlaveTaskFxn_11;
    tasks[12] = SlaveTaskFxn_12;
    tasks[13] = SlaveTaskFxn_13;
    tasks[14] = SlaveTaskFxn_14;
    tasks[15] = SlaveTaskFxn_15;

    void * taskStacks[NUM_TASK];
    taskStacks[0]  = SlaveTaskStack_0;
    taskStacks[1]  = SlaveTaskStack_1;
    taskStacks[2]  = SlaveTaskStack_2;
    taskStacks[3]  = SlaveTaskStack_3;
    taskStacks[4]  = SlaveTaskStack_4;
    taskStacks[5]  = SlaveTaskStack_5;
    taskStacks[6]  = SlaveTaskStack_6;
    taskStacks[7]  = SlaveTaskStack_7;
    taskStacks[8]  = SlaveTaskStack_8;
    taskStacks[9]  = SlaveTaskStack_9;
    taskStacks[10] = SlaveTaskStack_10;
    taskStacks[11] = SlaveTaskStack_11;
    taskStacks[12] = SlaveTaskStack_12;
    taskStacks[13] = SlaveTaskStack_13;
    taskStacks[14] = SlaveTaskStack_14;
    taskStacks[15] = SlaveTaskStack_15;

    /* array of buffers to be populated */
    buf[0] = buf_0;
    buf[1] = buf_1;
    buf[2] = buf_2;
    buf[3] = buf_3;
    buf[4] = buf_4;
    buf[5] = buf_5;
    buf[6] = buf_6;
    buf[7] = buf_7;
    buf[8] = buf_8;
    buf[9] = buf_9;
    buf[10] = buf_10;
    buf[11] = buf_11;
    buf[12] = buf_12;
    buf[13] = buf_13;
    buf[14] = buf_14;
    buf[15] = buf_15;


    /* Filling up the buffers with if they do not lie in the flash */
    AppUtils_Printf("Filling up the buffers\n");
    for (i = 0; i < NUM_TASK; ++i)
    {
        for (j = 0; j < BUF_SIZE; ++j)
        {
            buf[i][j] = j;
        }
    }

    /* Creating a task parameter */
    TaskP_Params taskParams;
    /* populating it with default values*/
    TaskP_Params_init(&taskParams);
    /* all tasks will have same priority*/
    taskParams.priority = 1;
    taskParams.stack = MainApp_TaskStack;
    taskParams.stacksize = sizeof(MainApp_TaskStack);

    /* creating master and slave tasks*/
    main_task[0] = TaskP_create(&MasterTask, &taskParams);
    for (i = 0; i < NUM_TASK; ++i)
    {
        taskParams.arg0 = (void *) i;
        taskParams.stack = taskStacks[i];
        main_task[i] = TaskP_create(tasks[i], &taskParams);
    }
    /* initializing the semaphores*/
    SemaphoreP_Params semParams;
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    for (i = 0; i < NUM_TASK; ++i)
    {
        gSemaphorePHandle[i] = ((void *)SemaphoreP_create(0, &semParams));
    }

    CSL_armR5PmuResetCntrs();
    uint32_t Val0 = CSL_armR5PmuReadCntr(0);
    uint32_t Val2 = CSL_armR5PmuReadCntr(2);
    uint32_t Val1 = CSL_armR5PmuReadCntr(1);

    /* Sanity checking for PMU counters*/
    AppUtils_Printf("Inst Cache Miss: %u\n", Val0);
    AppUtils_Printf("Inst Cache Access: %u\n", Val2);
    AppUtils_Printf("Data Cache Miss: %u\n", Val1);

    #if !defined(BUILD_MCU1_0)
        /* Start the BIOS tasks*/
        OS_start ();
    #endif

    return 0;
}

#if defined(BUILD_MCU1_0)
static void taskFxn(void* a0, void* a1)
{
    Board_initCfg boardCfg;

    /* Initialize SCI Client - It must be called before board init */
    memoryBenchmarking_initSciclient();

    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;

    Board_init(boardCfg);

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)))
    TaskP_Handle sciserverInitTask;
    TaskP_Params sciserverInitTaskParams;

    /* Initialize SCI Client Server */
    TaskP_Params_init(&sciserverInitTaskParams);
    sciserverInitTaskParams.priority     = 6;
    sciserverInitTaskParams.stack        = gSciserverInitTskStack;
    sciserverInitTaskParams.stacksize    = sizeof (gSciserverInitTskStack);

    sciserverInitTask = TaskP_create(&memoryBenchmarking_setupSciServer, &sciserverInitTaskParams);
    if(NULL == sciserverInitTask)
    {
        OS_stop();
    }
#endif

do_main();

}
#endif

#if defined(BUILD_MCU2_0) && defined(BUILD_OCMC)
/* giving main a section in this case avoids _system_post_cinit
   becoming a trampoline from OCMRAM that isn't yet mapped by RAT translation
 */
int main(void) __attribute__((section(".main_text")));
#endif

int main(void)
{
#if defined(BUILD_MCU1_0)
    TaskP_Handle task;
    TaskP_Params taskParams;
#endif

    OS_init();

    #if defined(BUILD_MCU1_0)
        /* Initialize the task params */
        TaskP_Params_init(&taskParams);
        /* Set the task priority higher than the default priority (1) */
        taskParams.priority = 2;
        taskParams.stack        = gAppTskStackMain;
        taskParams.stacksize    = sizeof (gAppTskStackMain);

        task = TaskP_create(&taskFxn, &taskParams);
        if(NULL == task)
        {
            OS_stop();
        }
        OS_start();    /* does not return */
    #else
        do_main();
    #endif

}

void SlaveTaskFxn_0(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_1(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_2(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_3(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_4(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_5(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_6(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_7(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_8(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_9(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_10(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_11(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_12(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_13(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_14(void * a0, void * a1)
{TSKFN}

void SlaveTaskFxn_15(void * a0, void * a1)
{TSKFN}


/**
 *  \brief Printf utility
 *
 */
#define APP_UTILS_PRINT_MAX_STRING_SIZE (2000U)
void AppUtils_Printf (const char *pcString, ...)
{
    static char printBuffer[APP_UTILS_PRINT_MAX_STRING_SIZE];
    va_list arguments;

    /* Start the varargs processing. */
    va_start(arguments, pcString);
    vsnprintf(printBuffer, sizeof(printBuffer), pcString, arguments);

    {
/* UART Prints do not work in XIP mode */
#ifdef UART_ENABLED
        UART_printf("%s",printBuffer);
#elif defined(CCS)
        printf("%s", printBuffer);
#else
        sbl_puts("\r");
        sbl_puts(printBuffer);
        sbl_puts("\n");
#endif
    }
    /* End the varargs processing. */
    va_end(arguments);

    return;
}

#if defined(BUILD_MCU1_0)

void memoryBenchmarking_initSciclient()
{
    int32_t ret = CSL_PASS;
    Sciclient_ConfigPrms_t        config;

    /* Now reinitialize it as default parameter */
    ret = Sciclient_configPrmsInit(&config);
    if (ret != CSL_PASS)
    {
        AppUtils_Printf("Sciclient_configPrmsInit Failed\n");
    }

#if (defined (BUILD_MCU1_0) && (defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)))
    if (ret == CSL_PASS)
    {
        ret = Sciclient_boardCfgParseHeader(
            (uint8_t *)SCISERVER_COMMON_X509_HEADER_ADDR,
            &config.inPmPrms, &config.inRmPrms);
        if (ret != CSL_PASS)
        {
            AppUtils_Printf("Sciclient_boardCfgParseHeader Failed\n");
        }
    }
#endif

    if (ret == CSL_PASS)
    {
        ret = Sciclient_init(&config);
        if (ret != CSL_PASS)
        {
            AppUtils_Printf("Sciclient_init Failed\n");
        }
    }
}

#endif
    
uint32_t AppUtils_getCurTimeInUsec(void)
{
    uint64_t curTimeUsec = 0;

    curTimeUsec = TimerP_getTimeInUsecs();
    return ((uint32_t) curTimeUsec);
}

uint32_t AppUtils_getElapsedTimeInUsec(uint32_t startTime)
{
    uint32_t     elapsedTimeUsec = 0U, currTime;

    currTime = AppUtils_getCurTimeInUsec();
    if (currTime < startTime)
    {
        /* Counter overflow occured */
        elapsedTimeUsec = (0xFFFFFFFFU - startTime) + currTime + 1U;
    }
    else
    {
        elapsedTimeUsec = currTime - startTime;
    }

    return (elapsedTimeUsec);
}


