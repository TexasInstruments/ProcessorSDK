/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <unity.h>
#include <drivers/adc.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <../source/drivers/epwm.h>
#include <../source/kernel/dpl/CycleCounterP.h>

#include <../drivers/menu/menu.h>
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Testcases */
static void ADC_setPrescalerApiCheck(void *args);

static void ADC_single_ended_conversion(void *args);
static void ADC_differential_conversion_with_prescaled_clock(void *args);
static void ADC_software_triggering(void *args);
static void ADC_triggering_from_timer(void *args);
static void ADC_inputxbar_trigger(void *args);
static void ADC_epwm_soc_trigger(void *args);
static void ADC_all_adcs_working_together(void *args);
static void ADC_force_multiple_socs_simultaneously(void *args);
static void ADC_enable_ppb_adcevt_cycle_by_cycle_mode(void *args);
static void ADC_clear_adcintovf_status_register(void *args);
static void ADC_ppb_error_calculation(void *args);
static void ADC_trigger_to_sample_delay_capture(void *args);
static void ADC_asynchronous_operation(void *args);
static void ADC_back_to_back_conversions_after_receiving_trigger(void *args);
static void ADC_ppbs_to_trip_epwms(void *args);
static void ADC_periodic_triggering_of_adc_conversion_from_epwm(void *args);
static void ADC_adc_read_latency(void *args);
static void ADC_adc_latch_to_r5_response_latency(void *args);
static void ADC_r5_rw_latencies_to_adc(void *args);
static void ADC_dma_latency(void *args);

int32_t test_adc_cases(uint8_t in);

static HwiP_Object  gEpwmHwiObject1;
static HwiP_Object  gEpwmHwiObject2;

static HwiP_Object  gAdcHwiObject1;
static HwiP_Object  gAdcHwiObject2;

volatile uint32_t gISR_latency;
volatile uint32_t gISR_complete;

static int32_t      gAdc_ISR1_count = 0;
static int32_t      gEpwm_ISR2_count = 0;

static SemaphoreP_Object  gEpwmSyncSemObject;
static SemaphoreP_Object  gEpwmIsrSemObject;

/* TA-DUT UART communincation related definitions, macros, variables */

#define CMD_TX_BUFSIZE      (200U)
#define CMD_RX_BUFSIZE      (200U)

#define CMD_SIZE            (32U)
#define RSP_SIZE            (32U)

uint8_t gCmdTxBuffer[CMD_TX_BUFSIZE];
uint8_t gCmdRxBuffer[CMD_RX_BUFSIZE];
// volatile uint32_t gNumBytesRead = 0U, gNumBytesWritten = 0U;
int32_t          transferOK;
UART_Transaction trans;

void tester_init(void)
{
        UART_Transaction_init(&trans);

        gNumBytesWritten = 0U;
        trans.buf   = &gCmdTxBuffer[0U];
        strncpy(trans.buf,"123456780000000000000000ABCDEFEF", CMD_SIZE);
        trans.count = CMD_SIZE;           //CMD_SIZE expected. Send CMD_SIZE-1 only to sync. Unexpected 1.
        transferOK = UART_write(gUartHandle[TESTER_UART], &trans);
}

void tester_command(char *str)
{
    DebugP_log("\r\nSending command!!\r\n ");

    for(int i=0; i<= CMD_SIZE; i++)
    {
        gCmdTxBuffer[i] = 0;
    }
    /* Send command*/
    gNumBytesWritten = 0U;
    trans.buf   = &gCmdTxBuffer[0U];
    strncpy(trans.buf,str, strlen(str));
    trans.count = CMD_SIZE;
    transferOK = UART_write(gUartHandle[TESTER_UART], &trans);


    /* Quickly wait to read ack*/
    gNumBytesRead = 0U;
    trans.buf   = &gCmdRxBuffer[0U];
    trans.count = RSP_SIZE;
    transferOK = UART_read(gUartHandle[TESTER_UART], &trans);

    DebugP_log("Received response!! (%d %d) : ", transferOK, trans.status);
    uint8_t ind;
    for(ind=0;ind<RSP_SIZE;ind++)
    {
        DebugP_log("%c", gCmdRxBuffer[ind]);
    }
    DebugP_log("\r\n");
}



/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */

void util_ADC_init(uint32_t base,
                   ADC_ClkPrescale prescalar,
                   ADC_SignalMode signal_mode,
                   ADC_PriorityMode adc_priority_mode);

void util_ADC_setup_and_enable_trigger_source(
    uint32_t base,
    ADC_SOCNumber soc_number,
    ADC_Trigger trigger,
    ADC_IntNumber int_number,
    ADC_Channel channel,
    uint32_t sampleWindow);

void util_ADC_fire_soc_trigger(
    uint32_t base,
    ADC_SOCNumber soc_number,
    ADC_Trigger trigger,
    ADC_IntNumber int_number);

int util_ADC_wait_for_adc_interrupt(uint32_t base, ADC_IntNumber int_number);

uint16_t util_ADC_check_result(uint32_t base, ADC_SOCNumber soc_number);

void util_ADC_deinit(uint32_t base);

int test_trigger(
    uint32_t base,
    ADC_Trigger trigger,
    ADC_Trigger false_trigger,
    ADC_IntNumber int_number,
    ADC_Channel channel,
    uint32_t sample_window);

int test_adc_operational_mode(uint32_t base, int mode);

void enable_all_epwms(void);

void disable_all_epwms(void);

void epwms_interrupt_isr_setup(void);

void util_EPWM_init(uint32_t epwm_base);

void util_EPWM_deinit(uint32_t epwm_base);

void util_EPWM_setup_adc_trigger(uint32_t epwm_base, EPWM_ADCStartOfConversionType adc_soc_type);

void util_EPWM_start_counter(uint32_t epwm_instance);

void util_EPWM_stop_counter(uint32_t epwm_base);

void RTI_INT_ISR(void);

void EPWM_INT_ISR(void *args);

void ADC_INT_ISR_for_periodic_triggering_from_epwm(void *args);

void EPWM_INT_ISR_for_periodic_triggering_from_epwm(void *args);

void ADC_ISR_for_ADC_TTR_0002(void* args);
/* ========================================================================== */



#define LENGTH_OF_TEST_LIST 21


void test_main(void *args)
{
    /* Open drivers */
    Drivers_open();
    Board_driversOpen();

    UNITY_BEGIN();

    tester_init();

    ClockP_sleep(3);

    char list_of_tests[LENGTH_OF_TEST_LIST+1][100] = {
        "ADC_setPrescalerApiCheck",
        "ADC_single_ended_conversion",
        "ADC_differential_conversion_with_prescaled_clock",
        "ADC_software_triggering",
        "ADC_triggering_from_timer",
        "ADC_inputxbar_trigger",
        "ADC_epwm_soc_trigger",
        "ADC_all_adcs_working_together",
        "ADC_force_multiple_socs_simultaneously",
        "ADC_enable_ppb_adcevt_cycle_by_cycle_mode",
        "ADC_clear_adcintovf_status_register",
        "ADC_ppb_error_calculation",
        "ADC_trigger_to_sample_delay_capture",
        "ADC_asynchronous_operation",
        "ADC_back_to_back_conversions_after_receiving_trigger",
        "ADC_ppbs_to_trip_epwms",
        "ADC_periodic_triggering_of_adc_conversion_from_epwm",
        "ADC_adc_read_latency",
        "ADC_adc_latch_to_r5_response_latency",
        "ADC_r5_rw_latencies_to_adc",
        "ADC_dma_latency",
        "Quit"
    };


    while(1)
    {
        int option = menu(list_of_tests, LENGTH_OF_TEST_LIST+1, CONFIG_UART0);
        ClockP_usleep(100);

        int32_t int_time_out_request = time_out_request(CONFIG_UART0);
        if(int_time_out_request == 0)
        {
            /* no wait*/
            /* yet to implement*/
        }
        else
        {
            if(int_time_out_request == 1)
            {
                /* finite timeout*/
                /* yet to implement*/
            }
            else
            {
                /* infinite timeout*/
                /* yet to implement*/
            }
        }
        if(option == (LENGTH_OF_TEST_LIST+1))
        {
            /* end tests*/
            break;
        }
        else
        {
            switch (option)
            {
                case 1:
                    RUN_TEST(ADC_setPrescalerApiCheck, 1, NULL);
                    break;
                case 2:
                    RUN_TEST(ADC_single_ended_conversion, 3155, NULL);
                    break;
                case 3:
                    RUN_TEST(ADC_differential_conversion_with_prescaled_clock, 3156, NULL);
                    break;
                case 4:
                    RUN_TEST(ADC_software_triggering, 3157, NULL);
                    break;
                case 5:
                    RUN_TEST(ADC_triggering_from_timer , 3158, NULL);
                    break;
                case 6:
                    RUN_TEST(ADC_inputxbar_trigger , 3159, NULL);
                    break;
                case 7:
                    RUN_TEST(ADC_epwm_soc_trigger , 3160, NULL);
                    break;
                case 8:
                    RUN_TEST(ADC_all_adcs_working_together , 3161, NULL);
                    break;
                case 9:
                    RUN_TEST(ADC_force_multiple_socs_simultaneously , 3162, NULL);
                    break;
                case 10:
                    RUN_TEST(ADC_enable_ppb_adcevt_cycle_by_cycle_mode , 3163, NULL);
                    break;
                case 11:
                    RUN_TEST(ADC_clear_adcintovf_status_register , 3164, NULL);
                    break;
                case 12:
                    RUN_TEST(ADC_ppb_error_calculation , 3165, NULL);
                    break;
                case 13:
                    RUN_TEST(ADC_trigger_to_sample_delay_capture , 3166, NULL);
                    break;
                case 14:
                    RUN_TEST(ADC_asynchronous_operation , 3167, NULL);
                    break;
                case 15:
                    RUN_TEST(ADC_back_to_back_conversions_after_receiving_trigger , 3168, NULL);
                    break;
                case 16:
                    RUN_TEST(ADC_ppbs_to_trip_epwms , 3169, NULL);
                    break;
                case 17:
                    RUN_TEST(ADC_periodic_triggering_of_adc_conversion_from_epwm , 3170, NULL);
                    break;
                case 18:
                    RUN_TEST(ADC_adc_read_latency , 3171, NULL);
                    break;
                case 19:
                    RUN_TEST(ADC_adc_latch_to_r5_response_latency , 3172, NULL);
                    break;
                // case 20:
                    // RUN_TEST(ADC_r5_rw_latencies_to_adc , 3173, NULL);
                    // break;
                // case 21:
                    // RUN_TEST(ADC_dma_latency , 3174, NULL);
                    // break;
                default :
                    RUN_TEST(ADC_setPrescalerApiCheck, 1, NULL);
                    RUN_TEST(ADC_single_ended_conversion, 3155, NULL);
                    RUN_TEST(ADC_differential_conversion_with_prescaled_clock, 3156, NULL);
                    RUN_TEST(ADC_software_triggering, 3157, NULL);
                    RUN_TEST(ADC_triggering_from_timer , 3158, NULL);
                    RUN_TEST(ADC_inputxbar_trigger , 3159, NULL);
                    RUN_TEST(ADC_epwm_soc_trigger , 3160, NULL);
                    RUN_TEST(ADC_all_adcs_working_together , 3161, NULL);
                    RUN_TEST(ADC_force_multiple_socs_simultaneously , 3162, NULL);
                    RUN_TEST(ADC_enable_ppb_adcevt_cycle_by_cycle_mode , 3163, NULL);
                    RUN_TEST(ADC_clear_adcintovf_status_register , 3164, NULL);
                    RUN_TEST(ADC_ppb_error_calculation , 3165, NULL);
                    RUN_TEST(ADC_trigger_to_sample_delay_capture , 3166, NULL);
                    RUN_TEST(ADC_asynchronous_operation , 3167, NULL);
                    RUN_TEST(ADC_back_to_back_conversions_after_receiving_trigger , 3168, NULL);
                    RUN_TEST(ADC_ppbs_to_trip_epwms , 3169, NULL);
                    RUN_TEST(ADC_periodic_triggering_of_adc_conversion_from_epwm , 3170, NULL);
                    RUN_TEST(ADC_adc_read_latency , 3171, NULL);
                    break;
            }
        }
    }
    UNITY_END();

    /* Close drivers */
    Board_driversClose();
    Drivers_close();

    return;
}

/* Unity framework required information */
void setUp(void)
{
}

void tearDown(void)
{
}


/* Testcase 1 - Check the ADC_setPrescaler API */
static void ADC_setPrescalerApiCheck(void *args)
{
    /* Call the ADC_setPrescaler API */
    ADC_setPrescaler(CSL_CONTROLSS_ADC0_U_BASE, ADC_CLK_DIV_7_5);

    /* Check if the value was written correctly */
    TEST_ASSERT_EQUAL_INT32((HW_RD_REG16(CSL_CONTROLSS_ADC0_U_BASE + CSL_ADC_ADCCTL2) & CSL_ADC_ADCCTL2_PRESCALE_MASK), ADC_CLK_DIV_7_5);
}

static void ADC_single_ended_conversion(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(1), 0);
}
static void ADC_differential_conversion_with_prescaled_clock(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(2), 0);
}
static void ADC_software_triggering(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(3), 0);
}
static void ADC_triggering_from_timer(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(4), 0);
}
static void ADC_inputxbar_trigger(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(5), 0);
}
static void ADC_epwm_soc_trigger(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(6), 0);
}
static void ADC_all_adcs_working_together(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(7), 0);
}
static void ADC_force_multiple_socs_simultaneously(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(8), 0);
}
static void ADC_enable_ppb_adcevt_cycle_by_cycle_mode(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(9), 0);
}
static void ADC_clear_adcintovf_status_register(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(10), 0);
}
static void ADC_ppb_error_calculation(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(11), 0);
}
static void ADC_trigger_to_sample_delay_capture(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(12), 0);
}
static void ADC_asynchronous_operation(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(13), 0);
}
static void ADC_back_to_back_conversions_after_receiving_trigger(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(14), 0);
}
static void ADC_ppbs_to_trip_epwms(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(15), 0);
}
static void ADC_periodic_triggering_of_adc_conversion_from_epwm(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(16), 0);
}
static void ADC_adc_read_latency(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(17), 0);
}
static void ADC_adc_latch_to_r5_response_latency(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(18), 0);
}
static void ADC_r5_rw_latencies_to_adc(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(19), 0);
}
static void ADC_dma_latency(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_adc_cases(20), 0);
}

void util_ADC_init(uint32_t base,
                   ADC_ClkPrescale prescalar,
                   ADC_SignalMode signal_mode,
                   ADC_PriorityMode adc_priority_mode)
{
    SOC_enableAdcReference((base&0x0000F000)>>12);
    /*
     * this util function is meant to..
     * Set Prescalar to prescalar
     * Set Mode to signal_mode
     * Set interrupt Pulse Mode to interrupt_pulse_mode
     */
    ADC_setPrescaler(base, prescalar);
    ADC_setMode(base, ADC_RESOLUTION_12BIT, signal_mode);
    ADC_setInterruptPulseMode(base, ADC_PULSE_END_OF_CONV);
    ADC_setSOCPriority(base, ADC_PRI_ALL_ROUND_ROBIN);
    ADC_enableConverter(base);
}

void util_ADC_deinit(uint32_t base)
{
    /*
     * this function reset ADC
     * and disables the ADC converter
     */

    /* resetting the ADC*/
    SOC_generateAdcReset((base&0x0000F000)>>12);

    ADC_disableConverter(base);
}

void util_ADC_setup_and_enable_trigger_source(
    uint32_t base,
    ADC_SOCNumber soc_number,
    ADC_Trigger trigger,
    ADC_IntNumber int_number,
    ADC_Channel channel,
    uint32_t sample_window)
{
    ADC_disableInterrupt(base, int_number);
    if (trigger == ADC_TRIGGER_SW_ONLY)
    {
        /* No additional setup requried */
    }
    else if ((trigger >= ADC_TRIGGER_RTI0) && (trigger <= ADC_TRIGGER_RTI3))
    {
        /*
         * RTIx setup is configured through the syscfg.
         * No additional setup would be required
         *
         * Steps to setup in syscfg:
         *      Add RTI instances,
         *      enable counter 0/1,
         *      enable the compare event 0 and configure it for counter 0/1
         *      enable interrupt
         *      add the Interrupt ISR Callback
         */
    }
    else if (trigger == ADC_TRIGGER_INPUT_XBAR_OUT5)
    {
        /* GPIOx --> GPIOy loop back, GPIOy in INPUT_XBAR_OUT5 setup */
        /*
         * the required setup is done in the util_ADC_fire_soc_trigger
         * hence, no extra setup is required here.
         */
    }
    else if ((trigger >= ADC_TRIGGER_EPWM0_SOCA) && (trigger <= ADC_TRIGGER_EPWM31_SOCB))
    {
        /* setup the given EPWM */

        /*
         * setup given EPWM TB Counter in stop and freeze mode.
         * setup TB PRD to be largest (12000)
         * setup compare A to say 100
         * setup compare B to say 200
         *      fire trigger() shall change counter mode to up-down count mode.
         * set EPWM interrupt to be at counter == compare B
         * set EPWM INT to change the counter mode to stop and freeze.
         */

        uint8_t epwm_instance   = (uint8_t) (trigger - 8)/2 ;
        uint32_t epwm_base      = (0x00001000)*( (uint32_t) epwm_instance)
                                   + CSL_CONTROLSS_G0_EPWM0_U_BASE;

        util_EPWM_setup_adc_trigger(epwm_base, (trigger%2));
        // DebugP_log("%x epwm base, %d trigger\r\n", epwm_base, trigger%2);


    }
    // else if ( trigger is and ECAP evt trigger)


    /* As per TRM, the sample_window (ACQPS) should be >= 16 sysclks*/
    sample_window = (sample_window > 16)? sample_window : 16;

    ADC_setupSOC(
        base,
        soc_number,
        trigger,
        channel,
        sample_window);

    /* enabling the interrupt*/
    ADC_enableInterrupt(base, int_number);
    /* Setting up the current soc_number (eoc) to trigger ADC_INT*/
    ADC_setInterruptSource(base, int_number, soc_number);
}

void util_ADC_fire_soc_trigger(
    uint32_t base,
    ADC_SOCNumber soc_number,
    ADC_Trigger trigger,
    ADC_IntNumber int_number)
{
    /* Enabling and clearing the interrupt, overflow flags if any*/
    ADC_enableInterrupt(base, int_number);
    ADC_clearInterruptStatus(base, int_number);
    if (ADC_getInterruptOverflowStatus(base, int_number))
    {
        ADC_clearInterruptOverflowStatus(base, int_number);
    }

    if (trigger == ADC_TRIGGER_SW_ONLY)
    {
        ADC_forceSOC(base, soc_number);
    }
    else if ((trigger >= ADC_TRIGGER_RTI0) && (trigger <= ADC_TRIGGER_RTI3))
    {
        /* RTIx trigger is requried */
        /*
         * Enabling the counter should be able to trigger,
         * the interrupt will call its ISR, RTI_INT_ISR()
         *      this ISR will only disable counter, so that there wouldn't be
         *      repeated interrupts from the given RTIx.
         */
        switch (trigger)
        {
        case ADC_TRIGGER_RTI1:
            RTI_counterEnable(CONFIG_RTI1_BASE_ADDR, CONFIG_RTI1_COMP0_SRC);
            break;
        case ADC_TRIGGER_RTI2:
            RTI_counterEnable(CONFIG_RTI2_BASE_ADDR, CONFIG_RTI2_COMP0_SRC);
            break;
        case ADC_TRIGGER_RTI3:
            RTI_counterEnable(CONFIG_RTI3_BASE_ADDR, CONFIG_RTI3_COMP0_SRC);
            break;
        default:
            /* RTI0 is not available.
             * so by default do nothing.
             * This should be able to fail wait and should ultimately cause the
             * test to fail*/
            break;
        }
    }
    else if (trigger == ADC_TRIGGER_INPUT_XBAR_OUT5)
    {
        /* GPIOx --> GPIOy loop back, GPIOy in INPUT_XBAR_OUT5 setup */
        /* Using GPIO7(Input PIN) and GPIO8(Output PIN) as for loopback*/
        GPIO_setDirMode(INPUT_PIN_BASE_ADDR,
                        INPUT_PIN_PIN,
                        INPUT_PIN_DIR);

        GPIO_setDirMode(OUTPUT_PIN_BASE_ADDR,
                        OUTPUT_PIN_PIN,
                        OUTPUT_PIN_DIR);

        GPIO_pinWriteHigh(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);

        /* Adding a delay*/
        ClockP_usleep(1000);

        GPIO_pinWriteLow(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);
    }
    else if ((trigger >= ADC_TRIGGER_EPWM0_SOCA) &&
             (trigger <= ADC_TRIGGER_EPWM31_SOCB))
    {
        /* trigger the given EPWM */
        uint8_t epwm_instance   = (uint8_t) (trigger - 8)/2 ;
        EPWM_clearADCTriggerFlag(epwm_instance, (trigger%2));
        util_EPWM_start_counter(epwm_instance);

        SemaphoreP_pend(&gEpwmSyncSemObject, SystemP_WAIT_FOREVER);


        /* the counter starts running
         * generates the epwm_adc_soc_x trigger and then
         * generates the epwm interrupt, fires, ISR,
         * ISR clears the interrupt and stops the counter */
    }
}

/*
 * triggers the false trigger,
 * waits for the ADC INT to happen. wait should fail,
 * triggers the trigger,
 * waits for the ADC INT to happen, wait should succeed.
 * returns if there is error.
 */
int test_trigger(
    uint32_t base,
    ADC_Trigger trigger,
    ADC_Trigger false_trigger,
    ADC_IntNumber int_number,
    ADC_Channel channel,
    uint32_t sample_window)
{

    int wait = 0;
    int errors = 0;
    ADC_SOCNumber soc_number;
    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_DIFFERENTIAL,
        ADC_PRI_ALL_ROUND_ROBIN);

    for (soc_number = ADC_SOC_NUMBER0;
         soc_number <= ADC_SOC_NUMBER15;
         soc_number++)
    {
        util_ADC_setup_and_enable_trigger_source(
            base,
            soc_number,
            trigger,
            int_number,
            channel,
            sample_window);
        /* firing false trigger*/
        /* Using the GPIO loopback for generating a InputXbar.out[5] trigger */
        util_ADC_fire_soc_trigger(
            base,
            soc_number,
            false_trigger,
            int_number);
        wait = util_ADC_wait_for_adc_interrupt(base, int_number);
        if (wait == 1)
        {
            /* wait successful. Conversion happened */
            /* false trigger triggered the SOC*/
            errors++;
        }
        wait = 0;
        util_ADC_fire_soc_trigger(
            base,
            soc_number,
            trigger,
            int_number);

        uint32_t epwm_instance = (trigger - 8)/2;
        uint32_t epwm_base = (epwm_instance * 0x00001000) +  CSL_CONTROLSS_G0_EPWM0_U_BASE;

        if(EPWM_getADCTriggerFlagStatus(epwm_base, trigger%2) != 1)
        {
            errors++;
            DebugP_log("no adc_soca/b flag\r\n");
        }
        else
        {
            EPWM_clearADCTriggerFlag(epwm_base, trigger%2);
        }
        wait = util_ADC_wait_for_adc_interrupt(base, int_number);
        if (wait == 0)
        {
            /* wait unsuccessful. Conversion did not happen*/
            /* trigger did not trigger the SOC*/
            errors++;
        }
        wait = 0;

    }
    DebugP_log("%d is errors value for %d trigger\r\n", errors, trigger);
    return errors;
}

/*
 * Waits until the interrupt flag is set,
 * Clears the interrupt flag and clears interrupt overflow flag, if set.
 */
int util_ADC_wait_for_adc_interrupt(
    uint32_t base,
    ADC_IntNumber int_number)
{
    /* Need to include a better wait condition*/
    int count_value = 100000;

    while (ADC_getInterruptStatus(base, int_number) != 1)
    {
        if (count_value <= 0)
        {
            /* wait time out. unsuccessful*/
            return 0;
        }
        /* reducing a count and making a timeout*/
        count_value--;
    }
    ADC_clearInterruptStatus(base, int_number);
    if (ADC_getInterruptOverflowStatus(base, int_number))
    {
        ADC_clearInterruptOverflowStatus(base, int_number);
    }
    return 1;
}

/* Reads the ADC result register for given SOC*/
uint16_t util_ADC_check_result(
    uint32_t base,
    ADC_SOCNumber soc_number)
{
    uint32_t result_base = base - (uint32_t) 0x001c0000;
    return ADC_readResult(result_base, soc_number);
}

void enable_all_epwms(void)
{
    for(int i = 0; i<=31 ; i++)
    {
        SOC_setEpwmTbClk(i, TRUE);
        SOC_setEpwmGroup(i, 0);
    }
}

void epwms_interrupt_isr_setup(void)
{
    /* this is to setup the interrupt xbar,
     * setup ISR for all the epwms. */

    /* sets up all the epwm interrupts to the given interrupt xbar instance*/
    /* here, INTXbar.out[1] is used for all the ewpm istances*/
    SOC_xbarSelectInterruptXBarInputSource(
        CSL_CONTROLSS_INTXBAR_U_BASE,
        1,                     // instance number 1
        0xFFFFFFFF,            // group zero all signals i.e. all epwm ints.
        0, 0, 0, 0, 0, 0);     // no other group signals

    int32_t  status;

    /* Initialising a Interrupt parameter
     * setting up the interrupt, callbacks.*/
    HwiP_Params  hwiPrms;
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_1;
    hwiPrms.callback    = &EPWM_INT_ISR;
    // hwiPrms.priority    = 0;
    hwiPrms.isPulse     = 1;
    status              = HwiP_construct(&gEpwmHwiObject1, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

}

void util_EPWM_init(uint32_t epwm_base)
{
/*
 * Initialized give epwm with TBPRD = 12000
 * counter = 0 and counter mode = stop and freeze
 * Compare A value = 100
 * Compare B value = 200
 * Action qualifer :
 *      to set high if counter = compare A counting up
 *      to set low if  counter = compare B counting up.
 * Event trigger   :
 *      EPWM interrupt to be triggered if the counter = compare A counting down
 *
 * the values choosen to configure a pulse and an interrupt, among many
 * combinations
 */
    uint16_t period = 12000;
    uint16_t compare_a = 100;
    uint16_t compare_b = 200;

    int epwm_instance = (epwm_base&0x0001f000)>>12;
    DebugP_log("%x is epwm_base\r\n", epwm_base);

    SOC_generateEpwmReset(epwm_instance);

    EPWM_setClockPrescaler(
        epwm_base,
        EPWM_CLOCK_DIVIDER_128,
        EPWM_HSCLOCK_DIVIDER_14);

    /* counter is set to stop and freeze*/
    EPWM_setTimeBaseCounterMode( epwm_base, EPWM_COUNTER_MODE_STOP_FREEZE);

    /* setting up the time epwm_base period*/
    EPWM_setTimeBasePeriod( epwm_base, period);

    /* Default configurations */
    EPWM_disablePhaseShiftLoad(epwm_base);
    EPWM_setPhaseShift(epwm_base, 0U);
    EPWM_setTimeBaseCounter(epwm_base, 0);

    /* Setup shadow register load on ZERO*/
    EPWM_setCounterCompareShadowLoadMode(
        epwm_base,
        EPWM_COUNTER_COMPARE_A,
        EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(
        epwm_base,
        EPWM_COUNTER_COMPARE_B,
        EPWM_COMP_LOAD_ON_CNTR_ZERO);

    /* Setting up Compare values */

    EPWM_setCounterCompareValue(
        epwm_base,
        EPWM_COUNTER_COMPARE_A,
        compare_a);

    EPWM_setCounterCompareValue(
        epwm_base,
        EPWM_COUNTER_COMPARE_B,
        compare_b);

    EPWM_setActionQualifierAction(
        epwm_base,
        EPWM_AQ_OUTPUT_A,
        EPWM_AQ_OUTPUT_HIGH,
        EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    EPWM_setActionQualifierAction(
        epwm_base,
        EPWM_AQ_OUTPUT_A,
        EPWM_AQ_OUTPUT_LOW,
        EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    EPWM_setActionQualifierAction(
        epwm_base,
        EPWM_AQ_OUTPUT_B,
        EPWM_AQ_OUTPUT_HIGH,
        EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    EPWM_setActionQualifierAction(
        epwm_base,
        EPWM_AQ_OUTPUT_B,
        EPWM_AQ_OUTPUT_LOW,
        EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    EPWM_enableInterrupt(epwm_base);
    EPWM_setInterruptEventCount(epwm_base, 1);

    EPWM_setInterruptSource(
        epwm_base,
        EPWM_INT_TBCTR_D_CMPA,
        EPWM_INT_TBCTR_D_CMPA);

    EPWM_clearEventTriggerInterruptFlag(epwm_base);

}

void util_EPWM_setup_adc_trigger(uint32_t epwm_base, EPWM_ADCStartOfConversionType adc_soc_type)
{
    /* enabling the ADC soc trigger from EPWM */
    EPWM_enableADCTrigger( epwm_base, adc_soc_type);

    EPWM_setADCTriggerEventPrescale(epwm_base, adc_soc_type, 1);
    /* ADC trigger is set at counter == compare A while incrementing*/
    EPWM_setADCTriggerSource(
        epwm_base,
        adc_soc_type,
        EPWM_SOC_TBCTR_U_CMPA,
        EPWM_SOC_TBCTR_U_CMPA);
}

void util_EPWM_deinit(uint32_t epwm_base)
{
    int epwm_instance = (epwm_base&0x0001f000)>>12;
    /*reset the EPWM*/
    SOC_generateEpwmReset(epwm_instance);

    /* disable interrupt if is still enabled*/
    EPWM_disableInterrupt(epwm_base);

    /* time base counter = 0 and mode = stop and freeze */
    EPWM_setTimeBaseCounterMode( epwm_base, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounter(epwm_base, 0);
}

void util_EPWM_start_counter(uint32_t epwm_instance)
{
    uint32_t epwm_base = (epwm_instance*0x00001000) + CSL_CONTROLSS_G0_EPWM0_U_BASE;
    EPWM_setTimeBaseCounterMode(epwm_base, EPWM_COUNTER_MODE_UP_DOWN);
}

void util_EPWM_stop_counter(uint32_t epwm_base)
{
    EPWM_setTimeBaseCounterMode( epwm_base, EPWM_COUNTER_MODE_STOP_FREEZE);
}

void disable_all_epwms(void)
{
    for (int i = 0; i <= 31; i++)
    {
        SOC_setEpwmTbClk(i, FALSE);
    }
}



/* ISR for all the RTIx (x = 1-3)*/
void RTI_INT_ISR(void)
{
    /* Disabling all the counters whenever this ISR is called.*/
    RTI_counterDisable(CONFIG_RTI1_BASE_ADDR, CONFIG_RTI1_COMP0_SRC);
    RTI_counterDisable(CONFIG_RTI2_BASE_ADDR, CONFIG_RTI2_COMP0_SRC);
    RTI_counterDisable(CONFIG_RTI3_BASE_ADDR, CONFIG_RTI3_COMP0_SRC);
}

void EPWM_INT_ISR(void *args)
{
    /* stop counters of all the epwms*/
    int i = 0;
    for (i = 0; i<=31 ; i++)
    {
        uint32_t epwm_base = CSL_CONTROLSS_G0_EPWM0_U_BASE + 0x1000*i;
        util_EPWM_stop_counter(epwm_base);
        EPWM_clearEventTriggerInterruptFlag(epwm_base);
    }
    SemaphoreP_post(&gEpwmSyncSemObject);
}

void ADC_INT_ISR_for_periodic_triggering_from_epwm(void *args)
{
    gAdc_ISR1_count++;
}

void EPWM_INT_ISR_for_periodic_triggering_from_epwm(void *args)
{
    gEpwm_ISR2_count++;
    if(gEpwm_ISR2_count >= 10)
    {
        SemaphoreP_post(&gEpwmIsrSemObject);
    }
    EPWM_clearEventTriggerInterruptFlag(CSL_CONTROLSS_G0_EPWM0_U_BASE);
}
int test_adc_operational_mode(uint32_t base, int mode)
{
    int wait = 0;
    int errors = 0;
    int non_zero_values = 0;

    ADC_SOCNumber soc_number;
    ADC_Trigger trigger;
    ADC_Channel channel;
    uint32_t sample_window;
    uint16_t adc_result;

    ADC_IntNumber int_number;

    int_number = ADC_INT_NUMBER1;
    trigger = ADC_TRIGGER_SW_ONLY;
    channel = ADC_CH_ADCIN2;

    /*
     * ADC_clock_cycle = (ADC_CLK_DIV_X_Y/2 +1)*input_clock_cycle
     * adding 1 at the end to make it >= actual adc_clock_cycle
     */
    sample_window = ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);
    if(mode == 0)
    {
        util_ADC_init(
            base,
            ADC_CLK_DIV_4_0,
            ADC_MODE_SINGLE_ENDED,
            ADC_PRI_ALL_ROUND_ROBIN);
    }
    else if(mode == 1)
    {
        util_ADC_init(
            base,
            ADC_CLK_DIV_4_0,
            ADC_MODE_DIFFERENTIAL,
            ADC_PRI_ALL_ROUND_ROBIN);
    }

    for (soc_number = ADC_SOC_NUMBER0;
         soc_number <= ADC_SOC_NUMBER15;
         soc_number++)
    {
        util_ADC_setup_and_enable_trigger_source(
            base,
            soc_number,
            trigger,
            int_number,
            channel,
            sample_window);
        util_ADC_fire_soc_trigger(
            base,
            soc_number,
            trigger,
            int_number);

        wait = util_ADC_wait_for_adc_interrupt(base, int_number);
        if (wait == 0)
        {
            /* wait successful. Conversion happened*/
            errors++;
        }

        adc_result = util_ADC_check_result(base, soc_number);

        if (adc_result > 0)
        {
            non_zero_values++;
        }
    }

    ADC_disableInterrupt(base, int_number);

    if (non_zero_values < 15)
    {
        errors++;
    }

    DebugP_log("%d errors for base %x, %d non zero values\r\n", errors, base, non_zero_values);

    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    return errors;
}

/*
 * AM263_ADC_BTR_0001	 Single ended conversion
 */
int32_t AM263x_ADC_BTR_001(uint32_t base)
{

    int errors = 0;

    /*expecting all to be non-zero values*/
    /*all the odd channels are connected a voltage divider
      arbitarily set at a positive voltage between 3.3 and 0*/
    tester_command("gen dac voltage 1.000v");
    ClockP_sleep(1);
    /* passing 0 for single ended conversion*/
    errors = test_adc_operational_mode(base, 0);

    if (errors > 0)
    {
        // printf("ADCBTR001 has failed\n");
        return 1;
    }
    else
    {
        // printf("ADCBTR001 has passed\n");
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0002	 Double ended conversion
 */
int32_t AM263x_ADC_BTR_002(uint32_t base)
{
    int errors;

    /*expecting all to be non-zero values*/
    /*all the odd channels are connected a voltage divider
      arbitarily set at a positive voltage between 3.3 and 0*/
    /*even channels are not equal to odd channels */
    tester_command("gen dac voltage 1.000v");
    ClockP_sleep(1);

    /* passing 1 for differential mode of conversion*/
    errors = test_adc_operational_mode(base, 1);
    if (errors > 0)
    {
        // printf("ADCBTR001 has failed\n");
        return 1;
    }
    else
    {
        // printf("ADCBTR001 has passed\n");
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0003	 Software triggering
 */
int32_t AM263x_ADC_BTR_003(uint32_t base)
{
    int errors = 0;

    ADC_Trigger trigger, false_trigger;
    ADC_Channel channel;
    uint32_t sample_window;

    ADC_IntNumber int_number;

    int_number = ADC_INT_NUMBER1;
    trigger = ADC_TRIGGER_SW_ONLY;
    false_trigger = ADC_TRIGGER_INPUT_XBAR_OUT5;
    channel = ADC_CH_ADCIN2;

    /*
     * ADC_clock_cycle = (ADC_CLK_DIV_X_Y/2 +1)*input_clock_cycle
     * adding 1 at the end to make it >= actual adc_clock_cycle
     */
    sample_window = ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);

    errors += test_trigger(
        base,
        trigger,
        false_trigger,
        int_number,
        channel,
        sample_window);

    ADC_disableInterrupt(base, int_number);
    DebugP_log("%d errors for base %x\r\n", errors, base);

    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    if (errors > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0004	 triggering from timer
 */
int32_t AM263x_ADC_BTR_004(uint32_t base)
{
    int errors = 0;

    ADC_Trigger trigger, false_trigger;
    ADC_Channel channel;
    uint32_t sample_window;
    ADC_IntNumber int_number;

    int_number = ADC_INT_NUMBER1;
    false_trigger = ADC_TRIGGER_INPUT_XBAR_OUT5;
    channel = ADC_CH_ADCIN2;

    /*
     * ADC_clock_cycle = (ADC_CLK_DIV_X_Y/2 +1)*input_clock_cycle
     * adding 1 at the end to make it >= actual adc_clock_cycle
     */
    sample_window = ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);

    for (trigger = ADC_TRIGGER_RTI1;
         trigger <= ADC_TRIGGER_RTI3;
         trigger++)
    {
        /* loops through all the SOCs in the ADC*/
        errors += test_trigger(
            base,
            trigger,
            false_trigger,
            int_number,
            channel,
            sample_window);
        ADC_disableInterrupt(base, int_number);
    }
    DebugP_log("%d errors for base %x\r\n", errors, base);

    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    if(errors > 0)  /* Fail criteria */
    {
        return 1 ;
    }
    else            /* Pass Criteria*/
    {
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0005	 InputXBar Trigger
 */
int32_t AM263x_ADC_BTR_005(uint32_t base)
{
    int errors = 0;

    ADC_Trigger trigger, false_trigger;
    ADC_Channel channel;
    uint32_t sample_window;
    ADC_IntNumber int_number;

    int_number = ADC_INT_NUMBER1;
    trigger = ADC_TRIGGER_INPUT_XBAR_OUT5;
    false_trigger = ADC_TRIGGER_RTI0;
    channel = ADC_CH_ADCIN2;

    /*
     * ADC_clock_cycle = (ADC_CLK_DIV_X_Y/2 +1)*input_clock_cycle
     * adding 1 at the end to make it >= actual adc_clock_cycle
     */
    sample_window = ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);

    /* loops through all the SOCs in the ADC*/
    errors += test_trigger(
        base,
        trigger,
        false_trigger,
        int_number,
        channel,
        sample_window);
    ADC_disableInterrupt(base, int_number);

    DebugP_log("%d errors for base %x\r\n", errors, base);

    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    if (errors > 0)
    {
        return 1;
    }
    else
    {
        return 0; // will turn back to 0 once the test is tested
    }
}

/*
 * AM263_ADC_BTR_0006	 ePWM SOC Trigger
 */
int32_t AM263x_ADC_BTR_006(uint32_t base)
{
    int errors = 0;

    ADC_Trigger trigger, false_trigger;
    ADC_Channel channel;
    uint32_t sample_window;
    ADC_IntNumber int_number;

    int_number = ADC_INT_NUMBER1;
    false_trigger = ADC_TRIGGER_INPUT_XBAR_OUT5;
    channel = ADC_CH_ADCIN2;

    /*
     * ADC_clock_cycle = (ADC_CLK_DIV_X_Y/2 +1)*input_clock_cycle
     * adding 1 at the end to make it >= actual adc_clock_cycle
     */
    sample_window = ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);

    uint32_t status = SemaphoreP_constructBinary(&gEpwmSyncSemObject, 0);
    DebugP_assert(SystemP_SUCCESS == status);
    /* enabling all the epwms*/
    enable_all_epwms();

    /* setting up the isr for the epwms*/
    epwms_interrupt_isr_setup();

    for (trigger = ADC_TRIGGER_EPWM0_SOCA;
         trigger <= ADC_TRIGGER_EPWM15_SOCB;
         trigger++)
    {

        int epwm_instance = (trigger - 8)/2;
        uint32_t epwm_base = (epwm_instance*0x00001000) + CSL_CONTROLSS_G0_EPWM0_U_BASE;

        util_EPWM_init(epwm_base);

        /* loops through all the SOCs in the ADC*/
        errors += test_trigger(
            base,
            trigger,
            false_trigger,
            int_number,
            channel,
            sample_window);
        ADC_disableInterrupt(base, int_number);

        util_EPWM_deinit(epwm_base);
    }

    disable_all_epwms();
    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    if(errors > 0)  /* Fail criteria */
    {
        return 1 ;
    }
    else            /* Pass Criteria*/
    {
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0007	 All ADCs working together
 */
int32_t AM263x_ADC_BTR_007(uint32_t base)
{
    int errors  = 0;
    int wait    = 0;

    ADC_SOCNumber   soc_number;
    ADC_Trigger     trigger;
    ADC_Channel     channel;
    uint32_t        sample_window;

    uint32_t adc_offset = 0;
    channel         =   ADC_CH_ADCIN2;

    trigger         =   ADC_TRIGGER_RTI1;
    sample_window   =   ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);
    for (adc_offset = 0; adc_offset <= 0x4000; adc_offset = adc_offset + 0x1000)
    {

        base = CSL_CONTROLSS_ADC0_U_BASE + adc_offset;
        /*
         * Initialising each ADC in prescalar = ADC_CLK_DIV_4_0,
         * single ended mode and round robin mode
         */
        util_ADC_init(
            base,
            ADC_CLK_DIV_4_0,
            ADC_MODE_SINGLE_ENDED,
            ADC_PRI_ALL_ROUND_ROBIN
            );
            /* Disabling all the interrupts*/
            ADC_disableInterrupt(base, ADC_INT_NUMBER1);
            ADC_disableInterrupt(base, ADC_INT_NUMBER2);
            ADC_disableInterrupt(base, ADC_INT_NUMBER3);
            ADC_disableInterrupt(base, ADC_INT_NUMBER4);

            /* Enabling the interrupt*/
            ADC_enableInterrupt(base, ADC_INT_NUMBER1);

        for (soc_number = ADC_SOC_NUMBER0;
             soc_number <= ADC_SOC_NUMBER15;
             soc_number ++)
            {
                /*
                    * Setting up the SOCs to be triggered by the same trigger
                    */

                ADC_setupSOC(
                    base,
                    soc_number,
                    trigger,
                    channel,
                    sample_window
                    );
                if(soc_number == ADC_SOC_NUMBER15)
                {
                    /* for the last SOC in the round robin priority,
                        * adding the interrupt to be triggered at the end of
                        * conversion. this will be used for checking EOC*/
                    ADC_setInterruptSource(base, ADC_INT_NUMBER1, soc_number);
                }
            }
        /* Clearing interrupts of all the ADCs*/
        ADC_clearInterruptStatus(base, ADC_INT_NUMBER1);
    }

    /*
     * All the SOCs of all the ADCs are setup for same trigger. Now, the trigger
     * has to be fired.
     *
     * RTI interrupt ISR shall disable the RTI counter. so there is only one
     * trigger occurred.
     */
    RTI_counterEnable(CONFIG_RTI1_BASE_ADDR, CONFIG_RTI1_COMP0_SRC);

    /*
     * Check for all the ADC_INT for all the ADCs.
     * this should result if All the ADCs-SOCs have been triggered, and
     * completed the conversions.
     */
    for (adc_offset = 0; adc_offset <= 0x4000; adc_offset = adc_offset + 0x1000)
    {
        base = CSL_CONTROLSS_ADC0_U_BASE + adc_offset;
        wait = util_ADC_wait_for_adc_interrupt(base, ADC_INT_NUMBER1);
        if(wait == 0)
        {
            /* wait unsuccessful. so error occurred*/
            errors++;
            wait = 0;
        }
    }

    for (adc_offset = 0; adc_offset <= 0x4000; adc_offset = adc_offset + 0x1000)
    {
        base = CSL_CONTROLSS_ADC0_U_BASE + adc_offset;

        /* resetting and disabling the ADC Converters*/
        util_ADC_deinit(base);
    }


    if(errors > 0)  /* Fail criteria */
    {
        return 1 ;
    }
    else            /* Pass Criteria*/
    {
        return 0;
    }
}

/*
 * AM263_ADC_BTR_0008	 Force multiple SOCs simultaneously
 */
int32_t AM263x_ADC_BTR_008(uint32_t base)
{
    int errors = 0;
    int wait   = 0;

    ADC_SOCNumber   soc_number;
    ADC_IntNumber   int_number;
    ADC_Trigger     trigger;
    ADC_Channel     channel;
    uint32_t        sample_window;

    channel         =   ADC_CH_ADCIN2;
    int_number      =   ADC_INT_NUMBER1;
    trigger         =   ADC_TRIGGER_SW_ONLY;
    sample_window   =   ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);


    /*
     * multiple trigger will be used to select the bits corresponding to the
     * SOCs.
     * mask will be used to identify the last SOC in the round roubin.
     */

    uint16_t multiple_trigger;
    uint16_t mask = 0x8000;
    int max_value = 0;

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_SINGLE_ENDED,
        ADC_PRI_ALL_ROUND_ROBIN
        );

    ADC_disableInterrupt(base, int_number);

    for( multiple_trigger = 0xff00;
         multiple_trigger <= 0xffff;
         multiple_trigger++)
        {
            ADC_enableInterrupt(base, int_number);
            ADC_clearInterruptStatus(base, int_number);
            if (ADC_getInterruptOverflowStatus(base, int_number))
            {
                ADC_clearInterruptOverflowStatus(base, int_number);
            }

            mask =  0x8000;
            max_value = 0;
            /* setting up SOCs based on the bits set in the multiple trigger*/
            for(soc_number = 15; soc_number>= 0; soc_number--)
            {
                if((mask & multiple_trigger)!= 0)
                {
                    max_value =(max_value < soc_number)? soc_number : max_value;
                        ADC_setupSOC(
                            base,
                            soc_number,
                            trigger,
                            channel,
                            sample_window);
                }
                mask = mask >> 1;
                if(mask == 0)break;
            }

            /*
             * Setting up the last SOC in the selected multiple triggers
             * to trigger ADC_INT at its EOC.
             */
            soc_number = max_value;
            ADC_setInterruptSource(base, int_number, soc_number);

            /* Forcing multiple SOCs*/
            ADC_forceMultipleSOC(base, multiple_trigger);

            /*
             * waiting for ADC_INT generated at the end of last SOC in
             * the round robin
             */
            wait = util_ADC_wait_for_adc_interrupt(base, int_number);
            if(wait == 0)
            {
                /* wait unsuccessful. conversions didnt happen as intended.*/
                errors++;
            }
            ADC_disableInterrupt(base, int_number);
            if(multiple_trigger == 0xffff) break;
        }

    /* resetting and disabling the ADC Converter*/
    util_ADC_deinit(base);

    DebugP_log("%d errors for base %x\r\n", errors, base);
    if(errors > 0)  /* Fail criteria */
    {
        return 1 ;
    }
    else            /* Pass Criteria*/
    {
        return 0;
    }

}

/*
 * AM263x_ADC_BTR_0009    Enable PPB ADCEVT cycle by cycle mode
 */
int32_t AM263x_ADC_BTR_009(uint32_t base)
{
    /*
     * PPB ADCEVT CBC clear mode functionality:
     *      the PPB ADCEVT flag will be cleared if there is no event and
     *      the next PPBxResult is updated.
     */

    int errors = 0;

    ADC_SOCNumber   soc_number = ADC_SOC_NUMBER0;

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_SINGLE_ENDED,
        ADC_PRI_ALL_ROUND_ROBIN
        );

    for(ADC_PPBNumber ppb_number = ADC_PPB_NUMBER1;
                      ppb_number <=ADC_PPB_NUMBER4;
                      ppb_number++)
        {
            ADC_setupPPB(base, ppb_number, soc_number);
            ADC_disablePPBEventCBCClear(base, ppb_number);

            ADC_enablePPBEvent(base, ppb_number, ADC_EVT_TRIPHI);
            /* setting high limit to be 0 so that there can be an event*/
                tester_command("gen dac voltage 1.000v");
            ADC_setPPBTripLimits(base, ppb_number, 0, 0);
            /* this is to make sure the PPB result is updated for each soc*/
            ADC_setPPBReferenceOffset(base, ppb_number, 1);
            ADC_forceSOC(base, soc_number);
            /* there should be a PPB event flag set.*/
            ADC_getPPBEventStatus(base, ppb_number)?:errors++;
            /* setting high trip value to 4095,
             * expecting the input to be < 4095
             * this will be made sure through tester application (wip) */
                tester_command("gen dac voltage 1.000v");

            ADC_setPPBTripLimits(base, ppb_number, 4095, 0);
            ADC_forceSOC(base, soc_number);
            /* since the flag hasn't been cleared, there will be event flag
             * set, even if there is not event.*/
            ADC_getPPBEventStatus(base, ppb_number)?:errors++;
            ADC_clearPPBEventStatus(base, ppb_number, ADC_EVT_TRIPHI);
            ADC_getPPBEventStatus(base, ppb_number)?:errors++;

            DebugP_log("%d errors before cbc enable\r\n", errors);
            /*-----------enabling cbc clear--------------------*/

            ADC_enablePPBEventCBCClear(base, ppb_number);
            /* setting high limit to be 0 so that there can be an event*/

                tester_command("gen dac voltage 1.000v");

            ADC_setPPBTripLimits(base, ppb_number, 0, 0);
            ADC_forceSOC(base, soc_number);
            /* there should be a PPB event flag set.*/
            ADC_getPPBEventStatus(base, ppb_number)?:errors++;
            /* setting high trip value to 4095,
             * expecting the input to be < 4095
             * this will be made sure through tester application (wip) */
                tester_command("gen dac voltage 1.000v");

            ADC_setPPBTripLimits(base, ppb_number, 4095, 0);
            ADC_forceSOC(base, soc_number);
            /* now, CBC clear is enabled. so if there is no event,
             * and ppb result register is updated, the event flag should be
             * cleared */
            if(ADC_getPPBEventStatus(base, ppb_number) ==1)
            {
                errors++;
                ADC_clearPPBEventStatus(base, ppb_number, ADC_EVT_TRIPHI);
            }

            ADC_disablePPBEventCBCClear(base, ppb_number);
            ADC_disablePPBEvent(base, ppb_number, ADC_EVT_TRIPHI);
            // DebugP_log("%d errors before adn after cbc enable for %d ppb_number\r\n", errors,ppb_number);
        }

    util_ADC_deinit(base);

    if(errors > 0 )
    {
        /* fail criteria*/
        return 1;
    }
    else
    {
        /* Pass Criteria*/
        return 0;
    }

}


/*
 * AM263_ADC_BTR_0010	 clear ADCINTOVF status register
 */
int32_t AM263x_ADC_BTR_010(uint32_t base)
{
    int errors = 0;

    ADC_SOCNumber   soc_number = ADC_SOC_NUMBER0;
    ADC_IntNumber   int_number;
    ADC_Trigger     trigger;
    ADC_Channel     channel;
    uint32_t        sample_window;

    channel         =   ADC_CH_ADCIN2;
    trigger         =   ADC_TRIGGER_SW_ONLY;
    sample_window   =   ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_DIFFERENTIAL,
        ADC_PRI_ALL_ROUND_ROBIN);

    for (int_number = ADC_INT_NUMBER1;
         int_number <= ADC_INT_NUMBER4;
         int_number++)
         {
            /*for each interrupt checking the interrupt overflow status*/

            util_ADC_setup_and_enable_trigger_source(
                base,
                soc_number,
                trigger,
                int_number,
                channel,
                sample_window);
            /* clearing the interrupt status and overflow status*/
            ADC_clearInterruptStatus(base, int_number);
            ADC_clearInterruptOverflowStatus(base, int_number);

            /* trigger ADC conversion*/
            ADC_forceSOC(base, soc_number);
            int count = 10000;
            while(count > 0)
            {
                if(ADC_getInterruptStatus(base, int_number) != 1)
                {
                    count--;
                }
                else
                {
                    break;
                }
            }
            if(count == 0)
            {
                /* wait failed interrupt never occurred*/
                errors++;
                break;
            }
            /* */
            if(ADC_getInterruptOverflowStatus(base, int_number) == 1)errors++;

            /* Not clearing the interrupt. forcing trigger again. this should
             * set the interrupt overflow flag */
            ADC_forceSOC(base, soc_number);
            count = 1000;
            while(count > 0)
            {
                if(ADC_getInterruptOverflowStatus(base, int_number) != 1)
                {
                    count--;
                }
                else
                {
                    break;
                }
            }
            if(count == 0)
            {
                /* wait failed interrupt never occurred*/
                errors++;
                break;
            }
            /* Interrupt Overflow Occurred. now clear it*/
            ADC_clearInterruptOverflowStatus(base, int_number);
            if(ADC_getInterruptOverflowStatus(base, int_number) == 1) errors++;

            /* diabling interrupt source*/
            ADC_disableInterrupt(base, int_number);
         }
        util_ADC_deinit(base);

        if(errors > 0)
        {
            /* fail criteria*/
            return 1;
        }
        else
        {
            /* pass criteria*/
            return 0;
        }
}

/*
 * AM263_ADC_BTR_0011	 PPB error calculation
 */
int32_t AM263x_ADC_BTR_011(uint32_t base)
{
    int errors = 0;
    /* PPB referecne offset -->
                    PPB_result = ADC_result - reference offset*/
    /* PPB calibration offset -->
                    ADC_result = ADC_result - calibration offset*/

    /* By default PPBs are setup towards soc_number_0. this can interfere in
     * the test. so setting up the PPB towards the soc_number_1*/

    ADC_SOCNumber   soc_number = ADC_SOC_NUMBER1;
    ADC_IntNumber   int_number = ADC_INT_NUMBER1;
    ADC_Trigger     trigger    = ADC_TRIGGER_SW_ONLY;
    ADC_Channel     channel    = ADC_CH_ADCIN2;
    uint32_t        sample_window   =   ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);
    uint32_t        result_base = base - 0x001c0000;
    util_ADC_setup_and_enable_trigger_source(
                base,
                soc_number,
                trigger,
                int_number,
                channel,
                sample_window);

    ADC_PPBNumber   ppb_number;
    for(ppb_number = ADC_PPB_NUMBER1;
        ppb_number <= ADC_PPB_NUMBER4;
        ppb_number++)
        {

                ADC_setupPPB(base, ppb_number, soc_number);
                int16_t offset;
                /* ASSUMPTION : the ADC intput voltage is constant
                 * at say, 2112*/
                    tester_command("gen dac voltage 1.6500v");

                for(offset = -512; offset <= 511; offset++)
                {
                    uint16_t result_without_cal_offset = 0;
                    uint16_t result_without_ref_offset = 0;
                    uint16_t result_with_cal_offset = 0;
                    uint16_t result_with_ref_offset = 0;


                    for(int twos_complement_flag = 0;
                        twos_complement_flag <= 1;
                        twos_complement_flag++)
                        {
                            if(twos_complement_flag == 0)
                            {
                                ADC_disablePPBTwosComplement(base, ppb_number);
                            }
                            else
                            {
                                ADC_enablePPBTwosComplement(base, ppb_number);
                            }
                            ADC_setPPBCalibrationOffset(base, ppb_number, 0);
                            ADC_setPPBReferenceOffset(base, ppb_number, 0);

                            ADC_forceSOC(base, soc_number);
                            while(ADC_getInterruptStatus(base, int_number) != 1);
                            ClockP_usleep(10);
                            result_without_cal_offset = util_ADC_check_result(base, soc_number);
                            result_without_ref_offset = ADC_readPPBResult(result_base, ppb_number);
                            (void)result_without_ref_offset; /* Presently set but not used. Suppress warning */

                            ADC_setPPBCalibrationOffset(base, ppb_number, offset);
                            ADC_setPPBReferenceOffset(base, ppb_number, offset);

                            ADC_forceSOC(base, soc_number);
                            while(ADC_getInterruptStatus(base, int_number) != 1);
                            ClockP_usleep(10);
                            result_with_cal_offset = util_ADC_check_result(base, soc_number);
                            result_with_ref_offset = ADC_readPPBResult(result_base, ppb_number);

                            if((int16_t) result_with_cal_offset != (int16_t)(result_without_cal_offset - offset))
                            {   if((result_with_cal_offset != 0) && (result_with_cal_offset != 0xffff))
                                {
                                    errors++;
                                    DebugP_log(" cal offset fail for offset %d\r\n",offset);
                                }
                            }
                            if(twos_complement_flag == 0)
                            {
                                if((int16_t)result_with_ref_offset !=(int16_t) (result_with_cal_offset - offset))
                                {
                                    errors++;
                                }
                            }
                            else
                            {
                                if((int16_t)result_with_ref_offset !=(int16_t) (offset - result_with_cal_offset))
                                {
                                    errors++;
                                }
                            }
                        }


                }

        }

    util_ADC_deinit(base);

    if(errors > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* AM263_ADC_BTR_0012	 Trigger to sample delay Capture*/
int32_t AM263x_ADC_BTR_012(uint32_t base)
{
    int errors = 0;

    ADC_SOCNumber   soc_number;
    ADC_PPBNumber   ppb_number = ADC_PPB_NUMBER1;
    ADC_IntNumber   int_number = ADC_INT_NUMBER1;
    ADC_Trigger     trigger    = ADC_TRIGGER_INPUT_XBAR_OUT5;
    ADC_Channel     channel    = ADC_CH_ADCIN2;

    int ppb_1_delay_stamp = 0;
    int ppb_2_delay_stamp = 0;
    int ppb_3_delay_stamp = 0;
    int ppb_4_delay_stamp = 0;


    uint32_t        sample_window   =   ((ADC_CLK_DIV_4_0 / 2 + 1) + 1);


    if(sample_window < 16)
    {
        sample_window = 16;
    }
    /* setting up all the SOC in roundrobin mode. enabling ADC instance*/
    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_SINGLE_ENDED,
        ADC_PRI_ALL_ROUND_ROBIN
        );

    /* enabling and clearing the ADC interrupt 1*/
    ADC_enableInterrupt(base, int_number);
    ADC_clearInterruptStatus(base, int_number);

    int_number = ADC_INT_NUMBER1;

    for(soc_number = ADC_SOC_NUMBER0;
        soc_number <=ADC_SOC_NUMBER15;
        soc_number++)
        {
            ADC_setupSOC(
                base,
                soc_number,
                trigger,
                channel,
                sample_window);

            /* setting soc_0, soc_5, soc_10, soc_15 to be configured
             * for PPBs  1 to 4 respectively */
            if((soc_number%5) == 0)
            {
                ADC_setupPPB(base, ppb_number, soc_number);
                ppb_number++;
            }
            if(soc_number == ADC_SOC_NUMBER15)
            {
                /* last soc to trigger interrupt*/
                ADC_setInterruptSource(base, int_number, soc_number);
            }
        }
    /* triggers all the SOCs*/
        GPIO_setDirMode(INPUT_PIN_BASE_ADDR,
                        INPUT_PIN_PIN,
                        INPUT_PIN_DIR);

        GPIO_setDirMode(OUTPUT_PIN_BASE_ADDR,
                        OUTPUT_PIN_PIN,
                        OUTPUT_PIN_DIR);

        GPIO_pinWriteHigh(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);

        /* Adding a delay*/
        ClockP_usleep(1000);

        GPIO_pinWriteLow(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);

    /* wait for last SOC to complete conversion and trigger ADC_interrupt*/
    while(ADC_getInterruptStatus(base, int_number) != 1);

    ppb_1_delay_stamp = ADC_getPPBDelayTimeStamp(base, ADC_PPB_NUMBER1);
    ppb_2_delay_stamp = ADC_getPPBDelayTimeStamp(base, ADC_PPB_NUMBER2);
    ppb_3_delay_stamp = ADC_getPPBDelayTimeStamp(base, ADC_PPB_NUMBER3);
    ppb_4_delay_stamp = ADC_getPPBDelayTimeStamp(base, ADC_PPB_NUMBER4);

    DebugP_log("%d is ppb_1_delaystamp for soc0\r\n", ppb_1_delay_stamp );
    DebugP_log("%d is ppb_2_delaystamp for soc5\r\n", ppb_2_delay_stamp );
    DebugP_log("%d is ppb_3_delaystamp for soc10\r\n", ppb_3_delay_stamp);
    DebugP_log("%d is ppb_4_delaystamp for soc15\r\n", ppb_4_delay_stamp);

    if((ppb_4_delay_stamp <= ppb_3_delay_stamp) ||
       (ppb_3_delay_stamp <= ppb_2_delay_stamp) ||
       (ppb_2_delay_stamp <= ppb_1_delay_stamp) ||
       (ppb_1_delay_stamp)!= 0)
       {
        errors++;
       }
    ADC_disableInterrupt(base, int_number);

    util_ADC_deinit(base);

    if(errors > 0)
    {
        /* fail criteria*/
        return 1;
    }
    else
    {
        /* pass criteria*/
        return 0;
    }

}

/*
 * AM263_ADC_BTR_0013	 Asynchronous operation
 */
int32_t AM263x_ADC_BTR_013(uint32_t base)
{
    int errors = 0;
    ADC_SOCNumber   soc_number;
    uint32_t        sample_window   =   16;

    /* INPUTXbar to be trigger for all SOCs of all ADCs*/
    ADC_Trigger     trigger    = ADC_TRIGGER_INPUT_XBAR_OUT5;
    ADC_Channel     channel    = ADC_CH_ADCIN2;
    ADC_IntNumber   int_number = ADC_INT_NUMBER1;

    uint32_t        base0 = base;
    uint32_t        adc_offset = 0;
    for(adc_offset = 0x0000;
        adc_offset <= 0x4000;
        adc_offset = adc_offset + 0x1000)
        {
            base = adc_offset + base0;
            util_ADC_init(
                base,
                ADC_CLK_DIV_4_0,
                ADC_MODE_SINGLE_ENDED,
                ADC_PRI_ALL_ROUND_ROBIN);

            ADC_enableInterrupt(base,int_number);
            ADC_clearInterruptStatus(base, int_number);
            for(soc_number = ADC_SOC_NUMBER0;
                soc_number <= ADC_SOC_NUMBER15;
                soc_number++ )
                {
                    ADC_setupSOC(
                        base,
                        soc_number,
                        trigger,
                        channel,
                        sample_window);
                    if(soc_number == 15)
                    {
                        ADC_setInterruptSource(base, int_number, soc_number);
                    }
                }
            sample_window = sample_window + 3;
            /* sample_windows for all ADCs will be 16, 19, 22, 25, 28 */
        }

    /* trigger through the inputxbar :: GPIO Loopback*/
    GPIO_setDirMode(INPUT_PIN_BASE_ADDR,
                    INPUT_PIN_PIN,
                    INPUT_PIN_DIR);

    GPIO_setDirMode(OUTPUT_PIN_BASE_ADDR,
                    OUTPUT_PIN_PIN,
                    OUTPUT_PIN_DIR);

    GPIO_pinWriteHigh(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);

    /* Adding a delay*/
    ClockP_usleep(1000);

    GPIO_pinWriteLow(OUTPUT_PIN_BASE_ADDR, OUTPUT_PIN_PIN);
    /* All SOCs of all ADCs are triggered.*/

    /* SOC15 of ADC4 should be the last one to be done
        * Hence its interrupt is last to be triggered */

    /* base is now base0 = 0x4000*/
    while(ADC_getInterruptStatus(base, int_number) != 1);

    /* errors if other ADCs haven't completed */
    base = base0;
    for(adc_offset = 0x0000;
        adc_offset <=0x4000;
        adc_offset = adc_offset + 0x1000)
        {
            if(ADC_getInterruptStatus(base, int_number) != 1)
            {
                errors++;
            }
        }

    base = base0;
    for(adc_offset = 0x0000;
        adc_offset <= 0x4000;
        adc_offset = adc_offset + 0x1000)
        {
            ADC_disableInterrupt(base,int_number);
            util_ADC_deinit(base);
        }

    if(errors > 0)
    {
        /* fail criteria*/
        return 1;
    }
    else
    {
        return 0;
    }
}

/* AM263_ADC_ITR_0001	 Back to back conversions */
int32_t AM263_ADC_ITR_0001(uint32_t base)
{
    /* Burst Mode test. single trigger to SOC and all the soc's should work on
    single channel, triggered by previous EOC*/
    int errors = 0;
    ADC_PriorityMode priority;
    ADC_SOCNumber   soc_number, last_soc_in_high_priority;
    int sample_window = 16;

    for(priority = ADC_PRI_SOC0_HIPRI;
        priority  <= ADC_PRI_ALL_HIPRI;
        priority++)
        {
            util_ADC_init(
                base,
                ADC_CLK_DIV_4_0,
                ADC_MODE_SINGLE_ENDED,
                priority);

            /* Always the first soc to be triggered is soc0*/
            ADC_setupSOC(
                base,
                ADC_SOC_NUMBER0,
                ADC_TRIGGER_SW_ONLY,
                ADC_CH_ADCIN0,
                sample_window);

            /* enabling and setting up interrupt for the last soc
            in the high priority */
            ADC_enableInterrupt(base,ADC_INT_NUMBER1);
            last_soc_in_high_priority = priority - 1;
            ADC_setInterruptSource(
                base,
                ADC_INT_NUMBER1,
                last_soc_in_high_priority);

            /*triggering and waiting for the conversions to occur*/
            ADC_forceSOC(base,ADC_SOC_NUMBER0);
            while(ADC_getInterruptStatus(base,ADC_INT_NUMBER1) != 1);

            for(soc_number = ADC_SOC_NUMBER0;
                soc_number <= last_soc_in_high_priority;
                soc_number++)
                {
                    int result = util_ADC_check_result(base, soc_number);
                    if(result <= 0 )
                    {
                        /* expecting non zero results*/
                        errors++;
                    }
                }

            if(last_soc_in_high_priority != ADC_SOC_NUMBER15)
            {
            for(soc_number = last_soc_in_high_priority+1;
                soc_number <= ADC_SOC_NUMBER15;
                soc_number++)
                {
                    int result = util_ADC_check_result(base, soc_number);
                    if(result > 0 )
                    {
                        /* expecting zero results*/
                        errors++;
                    }
                }
            }

            ADC_clearInterruptStatus(base,ADC_INT_NUMBER1);
            if(ADC_getInterruptOverflowStatus(base,ADC_INT_NUMBER1))
            {
                ADC_clearInterruptOverflowStatus(base, ADC_INT_NUMBER1);
            }

            ADC_disableInterrupt(base,ADC_INT_NUMBER1);

            util_ADC_deinit(base);
        }

    if(errors > 0)
    {
        /* Fail criteria*/
        return 1;
    }
    else
    {
        /* Pass criteria*/
        return 0;
    }

}

/* AM263_ADC_ITR_0002	 PPBs to trip ePWMs */
int32_t AM263_ADC_ITR_0002(uint32_t base)
{
    int errors = 0;
    ADC_PPBNumber ppb_number;
    uint32_t triphi_value, triplo_value;
    triphi_value = 2112;
    triplo_value = 0;

    uint16_t evtflg = 0x0001;    //trip high event

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_SINGLE_ENDED,
        ADC_PRI_ALL_ROUND_ROBIN);

    enable_all_epwms();
    /* selecting all the adc evt to output[0] of PWM xbar */
    SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, 0, 0, 0, 0,
        (EPWM_XBAR_ADC0_EVT1 |
        EPWM_XBAR_ADC0_EVT2 |
        EPWM_XBAR_ADC0_EVT3 |
        EPWM_XBAR_ADC0_EVT4 |
        EPWM_XBAR_ADC1_EVT1 |
        EPWM_XBAR_ADC1_EVT2 |
        EPWM_XBAR_ADC1_EVT3 |
        EPWM_XBAR_ADC1_EVT4 |
        EPWM_XBAR_ADC2_EVT1 |
        EPWM_XBAR_ADC2_EVT2 |
        EPWM_XBAR_ADC2_EVT3 |
        EPWM_XBAR_ADC2_EVT4 |
        EPWM_XBAR_ADC3_EVT1 |
        EPWM_XBAR_ADC3_EVT2 |
        EPWM_XBAR_ADC3_EVT3 |
        EPWM_XBAR_ADC3_EVT4 |
        EPWM_XBAR_ADC4_EVT1 |
        EPWM_XBAR_ADC4_EVT2 |
        EPWM_XBAR_ADC4_EVT3 |
        EPWM_XBAR_ADC4_EVT4 ), 0, 0, 0, 0, 0);

    for(ppb_number = ADC_PPB_NUMBER1;
        ppb_number <=ADC_PPB_NUMBER4;
        ppb_number++)
        {
            ADC_setupSOC(base, ADC_SOC_NUMBER0,ADC_TRIGGER_SW_ONLY,ADC_CH_ADCIN2,16);
            ADC_setupPPB(base, ppb_number,ADC_SOC_NUMBER0);
            /* for each ppb_numbers, assign trip functionality */
            ADC_setPPBTripLimits(base, ppb_number, triphi_value, triplo_value);
            ADC_enablePPBEvent(base, ppb_number, evtflg);
            ADC_clearPPBEventStatus(base, ppb_number,evtflg);

            /* get adc input to be higher than the trip value and small wait*/
                tester_command("gen dac voltage 3.000v");

            if( (SOC_xbarGetPWMXBarOutputSignalStatus(CSL_CONTROLSS_PWMXBAR_U_BASE) != 1)
                ||
                (ADC_getPPBEventStatus(base, ppb_number) != 1))
                {
                    errors++;
                }

            ADC_clearPPBEventStatus(base, ppb_number, evtflg);

            if((SOC_xbarGetPWMXBarOutputSignalStatus(CSL_CONTROLSS_PWMXBAR_U_BASE) == 1))
                {
                errors++;
                }
            ADC_disablePPBEvent(base, ppb_number, evtflg);

                tester_command("gen dac voltage 1.000v");
        }

    disable_all_epwms();
    util_ADC_deinit(base);

    if(errors > 0)
    {
        /*fail criteria*/
        return 1;
    }
    else
    {
        /*pass criteria*/
        return 0;
    }

}

/* AM263_ADC_ITR_0003	 Periodic Triggering of ADC Conversion from ePWM */
int32_t AM263_ADC_ITR_0003(uint32_t base)
{
    /*---------------variables-----------------------------------------------*/
    int errors = 0;
    ADC_SOCNumber   soc_number      = ADC_SOC_NUMBER0;
    ADC_Trigger     trigger         = ADC_TRIGGER_EPWM0_SOCA;
    ADC_Channel     channel         = ADC_CH_ADCIN0;
    uint32_t        sample_window   = 16;

    uint32_t status = SemaphoreP_constructBinary(&gEpwmIsrSemObject, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    /* -------------disabling previous interrupt and adding new ones---------*/
    /* disabling previous interrupt for pwm*/
    HwiP_destruct(&gEpwmHwiObject1);

    /* setting up epwm interrupt to route through int xbar 1*/
    SOC_xbarSelectInterruptXBarInputSource(
        CSL_CONTROLSS_INTXBAR_U_BASE,
        0,                                      //instance
        ( INT_XBAR_EPWM0_INT ),                 //epwm 0 interrupt
        0, 0, 0, 0, 0, 0);                      //no other group

    /* Initialising a Interrupt parameter
     * setting up the interrupt, callbacks.*/
    HwiP_Params  hwiPrms;
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_0;
    hwiPrms.callback    = &EPWM_INT_ISR_for_periodic_triggering_from_epwm;
    hwiPrms.isPulse     = 1;
    status              = HwiP_construct(&gEpwmHwiObject2, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    /* setting up ADCINT1s to route through int xbar 2*/
    SOC_xbarSelectInterruptXBarInputSource(
        CSL_CONTROLSS_INTXBAR_U_BASE,
        1, 0, 0,
        (INT_XBAR_ADC0_INT1 | INT_XBAR_ADC1_INT1 | INT_XBAR_ADC2_INT1 | INT_XBAR_ADC3_INT1 | INT_XBAR_ADC4_INT1),
        0, 0, 0, 0);

    /* Initialising a Interrupt parameter
     * setting up the interrupt, callbacks.*/
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_1;
    hwiPrms.callback    = &ADC_INT_ISR_for_periodic_triggering_from_epwm;
    hwiPrms.isPulse     = 1;
    status              = HwiP_construct(&gAdcHwiObject1, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    /*----------------------test setup---------------------------------*/
    enable_all_epwms();
    /* initialising epwm but the counter is frozen*/
    util_EPWM_init(CSL_CONTROLSS_G0_EPWM0_U_BASE);

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_DIFFERENTIAL,
        ADC_PRI_ALL_ROUND_ROBIN);

    ADC_setupSOC(
        base,
        soc_number,
        trigger,
        channel,
        sample_window);

    ADC_enableInterrupt(base,ADC_INT_NUMBER1);
    ADC_enableContinuousMode(base, ADC_INT_NUMBER1);

    ADC_setInterruptSource(base, ADC_INT_NUMBER1, soc_number);

    util_EPWM_setup_adc_trigger(CSL_CONTROLSS_G0_EPWM0_U_BASE, EPWM_SOC_A);

    gAdc_ISR1_count     = 0;
    gEpwm_ISR2_count    = 0;

    util_EPWM_start_counter(0); //starting epwm0 counter

    // ClockP_sleep(1);
    SemaphoreP_pend(&gEpwmIsrSemObject, SystemP_WAIT_FOREVER);
    util_EPWM_stop_counter(CSL_CONTROLSS_G0_EPWM0_U_BASE);

    HwiP_destruct(&gEpwmHwiObject2);
    HwiP_destruct(&gAdcHwiObject1);

    int difference = gAdc_ISR1_count - gEpwm_ISR2_count;

    if((difference < -1) || (difference > 1))
    {
        errors++;
    }

    ADC_disableInterrupt(base,ADC_INT_NUMBER1);

    util_ADC_deinit(base);
    util_EPWM_deinit(CSL_CONTROLSS_G0_EPWM0_U_BASE);

    disable_all_epwms();

    if(errors > 0 )
    {
        /* fail criteria*/
        return 1;
    }
    else
    {
        /* pass criteria*/
        return 0;
    }
}

/* AM263_ADC_TTR_0001	 ADC Read latency */
int32_t AM263_ADC_TTR_0001(uint32_t base)
{
    int errors = 0;

    uint64_t counter_start_stop_read_latency = 0;
    uint64_t adc_result_read_latency = 0;

    uint16_t result_dummy = 0;

    CycleCounterP_reset();
    counter_start_stop_read_latency = CycleCounterP_getCount32();


    for(int soc_number = 0; soc_number <= 15; soc_number++)
    {
        uint32_t result_addr = (base - 0x001c0000) + CSL_ADC_RESULT_ADCRESULT0;
        result_addr += soc_number*ADC_RESULT_ADCRESULTx_STEP;
        CycleCounterP_reset();
        result_dummy = HW_RD_REG16(result_addr);
        (void)result_dummy; /* Presently set but not used. Suppress warning */
        adc_result_read_latency = CycleCounterP_getCount32();
        adc_result_read_latency -= counter_start_stop_read_latency;
        DebugP_log("%d is adc_result_read_latency\r\n",adc_result_read_latency);

        /* observation 19 or 20 cycles*/
        if((adc_result_read_latency < 17) || (adc_result_read_latency > 22))
        {
            errors++;
        }
    }
    if(errors > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void ADC_ISR_for_ADC_TTR_0002(void* args)
{
    gISR_latency = CycleCounterP_getCount32();
    gISR_complete++;
    // ClockP_usleep(1000);
}

/* AM263_ADC_TTR_0002	 ADC Latch to R5F response latency */
int32_t AM263_ADC_TTR_0002(base)
{
    int errors = 0;
    uint32_t trigger_API_latency;
    uint32_t counter_start_read_latency;

    gISR_latency = 0;
    gISR_complete = 0;

    uint32_t status;

    /* setting up interrupt route for all adc instance*/
    /* for Interrupt xbar instance 1*/
    SOC_xbarSelectInterruptXBarInputSource(
        CSL_CONTROLSS_INTXBAR_U_BASE,
        1, 0, 0,
        0x01FFFFFF,
        0, 0, 0, 0);

    HwiP_Params  hwiPrms;
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_1;
    hwiPrms.callback    = &ADC_ISR_for_ADC_TTR_0002;
    hwiPrms.isPulse     = 1;
    status              = HwiP_construct(&gAdcHwiObject2, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    util_ADC_init(
        base,
        ADC_CLK_DIV_4_0,
        ADC_MODE_DIFFERENTIAL,
        ADC_PRI_ALL_ROUND_ROBIN);

    ADC_setupSOC(
        base,
        ADC_SOC_NUMBER0,
        ADC_TRIGGER_SW_ONLY,
        ADC_CH_ADCIN2,
        16);

    ADC_disableInterrupt(base, ADC_INT_NUMBER1);
    ADC_setInterruptSource(base, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_setInterruptPulseMode(base, ADC_PULSE_END_OF_ACQ_WIN);



    CycleCounterP_reset();
    counter_start_read_latency = CycleCounterP_getCount32();

    for(ADC_IntNumber int_number = ADC_INT_NUMBER1;
                      int_number <= ADC_INT_NUMBER4;
                      int_number++)
        {
            gISR_latency = 0;



            CycleCounterP_reset();
            ADC_forceSOC(base,ADC_SOC_NUMBER0);
            trigger_API_latency = CycleCounterP_getCount32();

            gISR_complete = 0;
            ADC_enableInterrupt(base, int_number);
            ADC_enableContinuousMode(base, int_number);

            /* making sure ADC is not busy*/
            while(ADC_isBusy(base));

            CycleCounterP_reset();
            ADC_forceSOC(base,ADC_SOC_NUMBER0);

            while(gISR_complete != 1);

            gISR_latency -= (trigger_API_latency + counter_start_read_latency + 16);

            if(gISR_latency > 240)
            {
                errors++;
            }
            DebugP_log("%d is ADC latch to r5 response\r\n",gISR_latency);
            DebugP_log("\tfor base : %x for interrupt %d\r\n",base,int_number);

            ADC_disableInterrupt(base,int_number);
        }
    if(base == (CSL_CONTROLSS_ADC0_U_BASE+0x4000))
    {
        HwiP_destruct(&gAdcHwiObject2);
    }
    util_ADC_deinit(base);

    if(errors > 0)
    {
        /* fail criteria*/
        return 1;
    }
    else
    {
        /* pass criteria*/
        return 0;
    }
}


int32_t test_adc_cases(uint8_t in)
{
    int32_t failcount = 0;
    uint32_t adc_offset = 0;
    uint32_t base;
    int flag_for_all_adcs_working_together = 0;
    int flag_for_asynchronous_operation    = 0;
    for (adc_offset = 0x0000; adc_offset <= 0x4000; adc_offset = adc_offset + 0x1000)
    {

        base = CSL_CONTROLSS_ADC0_U_BASE + adc_offset;

        switch (in)
        {
        case 1:
            /* AM263_ADC_BTR_0001	 Single ended conversion */
            failcount += AM263x_ADC_BTR_001(base);
            break;
        case 2:
            /* AM263_ADC_BTR_0002	 Differential conversion with prescaled
                                     clock */
            failcount += AM263x_ADC_BTR_002(base);
            break;
        case 3:
            /* AM263_ADC_BTR_0003	 Software triggering */
            failcount += AM263x_ADC_BTR_003(base);
            break;
        case 4:
            /* AM263_ADC_BTR_0004	 Triggering from Timer */
            failcount += AM263x_ADC_BTR_004(base);
            break;
        case 5:
            /* AM263_ADC_BTR_0005	 InputXBar Trigger */
            failcount += AM263x_ADC_BTR_005(base);
            break;
        case 6:
            /* AM263_ADC_BTR_0006	 ePWM SOC Trigger */
            failcount += AM263x_ADC_BTR_006(base);
            break;
        case 7:
            /* AM263_ADC_BTR_0007	 All ADCs working together */
            /*
             * This test will include all the ADCs in one testcase. so there is
             * no need for this to be repeated.
             * Hence the flag_for_all_adcs_working_together is used
             */
            if(flag_for_all_adcs_working_together == 1)
            {
                break;
            }
            failcount += AM263x_ADC_BTR_007(base);
            flag_for_all_adcs_working_together++;
            break;
        case 8:
            /* AM263_ADC_BTR_0008	 Force multiple SOCs simultaneously */
            failcount += AM263x_ADC_BTR_008(base);
            break;
        case 9:
            /* AM263_ADC_BTR_0009	 Enable PPB ADCEVT cycle by cycle mode*/
            failcount += AM263x_ADC_BTR_009(base);
            break;
        case 10:
            /* AM263_ADC_BTR_0010	 clear ADCINTOVF status register */
            failcount += AM263x_ADC_BTR_010(base);
            break;
        case 11:
            /* AM263_ADC_BTR_0011	 PPB error calculation */
            failcount += AM263x_ADC_BTR_011(base);
            break;
        case 12:
            /* AM263_ADC_BTR_0012	 Trigger to sample delay Capture */
            failcount += AM263x_ADC_BTR_012(base);
            break;
        case 13:
            /* AM263_ADC_BTR_0013	 Asynchronous operation */
            if(flag_for_asynchronous_operation == 1)
            {
                break;
            }
            failcount += AM263x_ADC_BTR_013(base);
            flag_for_asynchronous_operation++;
            break;
        case 14:
            /* AM263_ADC_ITR_0001	 Back to back conversions
                                     after receiving trigger */
            failcount += AM263_ADC_ITR_0001(base);
            break;
        case 15:
            /* AM263_ADC_ITR_0002	 PPBs to trip ePWMs */
            failcount += AM263_ADC_ITR_0002(base);
            break;
        case 16:
            /* AM263_ADC_ITR_0003	 Periodic Triggering of ADC Conversion
                                     from ePWM */
            failcount += AM263_ADC_ITR_0003(base);
            break;
        case 17:
            /* AM263_ADC_TTR_0001	 ADC Read latency */
            failcount += AM263_ADC_TTR_0001(base);
            break;
        case 18:
            /* AM263_ADC_TTR_0002	 ADC Latch to R5F response latency */
            failcount += AM263_ADC_TTR_0002(base);
            break;
        // case 19:
        //     /* AM263_ADC_TTR_0003	 R5F R/W latencies to ADC */
        //     failcount += AM263_ADC_TTR_0003(base);
        //     break;
        // case 20:
        //     /* AM263_ADC_TTR_0004	 DMA Latency */
        //     failcount += AM263_ADC_TTR_0004(base);
        //     break;
        }
    }

    // DebugP_log("%d is the failcount\r\n", failcount);
    if (failcount != 0)
    {
        /* test fail criteria*/
        return 1;
    }
    else
    {
        /* test pass criteria*/
        return 0;
    }
}

