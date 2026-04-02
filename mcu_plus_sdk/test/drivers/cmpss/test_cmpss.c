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
#include <stdlib.h>
#include <unity.h>
#include <drivers/cmpss.h>
#include <drivers/epwm.h>
#include <drivers/ecap.h>
#include <drivers/soc.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <kernel/dpl/ClockP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define NUM_TESTS               19
#define SIZE_STRING_TEST_NAME   70
#define TIMEOUT_UART_MENU       (10000)

/* Tester dependent */
#define CMD_TX_BUFSIZE      (200U)
#define CMD_RX_BUFSIZE      (200U)

#define CMD_SIZE            (32U)
#define RSP_SIZE            (32U)

#define TIMEOUT_UART_TESTER     (10000)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Testcases */

static void CMPSS_high_low_comparator_output(void *args);
static void CMPSS_cmpssa_high_comparator_input_source_selection(void *args);
static void CMPSS_cmpssa_low_comparator_input_source_selection(void *args);
static void CMPSS_comparator_hysteresis(void *args);
static void CMPSS_vdda_vdac_selection_for_dac_reference_voltage(void *args);
static void CMPSS_12_bit_dacs_in_cmpssa_cmpssb(void *args);
static void CMPSS_ramp_generation(void *args);
static void CMPSS_behavior_of_ramp_generator_during_emulation_suspend(void *args);
static void CMPSS_digital_filter_configuration(void *args);
static void CMPSS_pin_sharing_between_adc_and_cmpss(void *args);
static void CMPSS_ctriph_and_ctripl_connectivity_to_pwm_xbar(void *args);
static void CMPSS_ctriph_and_ctripl_connectivity_to_ecap_mux(void *args);
static void CMPSS_ctripouth_and_ctripoutl_connectivity_to_output_xbar(void *args);
static void CMPSS_extend_clear_signal_with_epwmblank(void *args);
static void CMPSS_dac_shadow_register_synchronization_with_epwm(void *args);
static void CMPSS_enable_disable_comparator_dac_output(void *args);
static void CMPSS_dac_ramp_generator_synchronization_with_epwm(void *args);
static void CMPSS_diode_emulation_mode_with_pwm(void *args);
static void CMPSS_cmpss_async_trip(void *args);

/*Utility/helper functions*/
int32_t apiCheck(uint32_t base);

int32_t test_cmpss_cases(uint8_t in);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Tester dependent */
uint8_t gCmdTxBuffer[CMD_TX_BUFSIZE];
uint8_t gCmdRxBuffer[CMD_RX_BUFSIZE];
volatile uint32_t gNumBytesRead = 0U, gNumBytesWritten = 0U;
int32_t          transferOK;
UART_Transaction trans;

bool enableLog;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Tester dependent */
void tester_init(void)
{
    UART_Transaction_init(&trans);
    trans.timeout=TIMEOUT_UART_TESTER;

    if(enableLog) DebugP_log("\r\nSending initialization command!!");

    gNumBytesWritten = 0U;
    trans.buf   = &gCmdTxBuffer[0U];
    strncpy(trans.buf,"123456780000000000000000ABCDEFEF", CMD_SIZE);
    trans.count = CMD_SIZE;           //if not pulled up CMD_SIZE-1 expected. Send CMD_SIZE-1 only to sync. Unexpected 1.
    transferOK = UART_write(gUartHandle[TESTER_UART], &trans);
    /* Quickly wait to read ack. No debug prints here*/
    gNumBytesRead = 0U;
    trans.buf   = &gCmdRxBuffer[0U];
    trans.count = RSP_SIZE;
    transferOK = UART_read(gUartHandle[TESTER_UART], &trans);

    if(enableLog) DebugP_log("\r\nReceived response. Tester Initialized!!");

    /*Clear TX buffer for shorter commands*/
    uint8_t ind;
    for(ind=0;ind<CMD_SIZE;ind++)
    {
        gCmdTxBuffer[ind]=0;
    }
}

void tester_command(char *str)
{
    if(enableLog) DebugP_log("\r\nSending command!!");

    /* Send command*/
    gNumBytesWritten = 0U;
    trans.buf   = &gCmdTxBuffer[0U];
    strncpy(trans.buf,str, strlen(str));
    trans.count = CMD_SIZE;
    transferOK = UART_write(gUartHandle[TESTER_UART], &trans);
    /* Quickly wait to read ack. No debug prints here*/
    gNumBytesRead = 0U;
    trans.buf   = &gCmdRxBuffer[0U];
    trans.count = RSP_SIZE;
    transferOK = UART_read(gUartHandle[TESTER_UART], &trans);


    uint8_t ind;
    if(enableLog)
    {
        DebugP_log("\r\nReceived response!! (%d %d) : ", transferOK, trans.status);
        for(ind=0;ind<RSP_SIZE;ind++)
        {
            DebugP_log("%c", gCmdRxBuffer[ind]);
        }
        DebugP_log("\r\n");
    }
    /*Clear TX buffer for shorter commands*/
    for(ind=0;ind<CMD_SIZE;ind++)
    {
        gCmdTxBuffer[ind]=0;
    }
}


void test_main(void *args)
{
    /* Open drivers */
    Drivers_open();
    Board_driversOpen();

    enableLog = 0;

    UNITY_BEGIN();

    if(enableLog) DebugP_log("\r\nAM263: CMPSS tests");

    tester_init();

    char testList[NUM_TESTS+1][SIZE_STRING_TEST_NAME] = {
        "CMPSS_high_low_comparator_output",
        "CMPSS_cmpssa_high_comparator_input_source_selection",
        "CMPSS_cmpssa_low_comparator_input_source_selection",
        "CMPSS_comparator_hysteresis",
        "CMPSS_vdda_vdac_selection_for_dac_reference_voltage",
        "CMPSS_12_bit_dacs_in_cmpssa_cmpssb",
        "CMPSS_ramp_generation",
        "CMPSS_behavior_of_ramp_generator_during_emulation_suspend",
        "CMPSS_digital_filter_configuration",
        "CMPSS_pin_sharing_between_adc_and_cmpss",
        "CMPSS_ctriph_and_ctripl_connectivity_to_pwm_xbar",
        "CMPSS_ctriph_and_ctripl_connectivity_to_ecap_mux",
        "CMPSS_ctripouth_and_ctripoutl_connectivity_to_output_xbar",
        "CMPSS_extend_clear_signal_with_epwmblank",
        "CMPSS_dac_shadow_register_synchronization_with_epwm",
        "CMPSS_enable_disable_comparator_dac_output",
        "CMPSS_dac_ramp_generator_synchronization_with_epwm",
        "CMPSS_diode_emulation_mode_with_pwm",
        "CMPSS_cmpss_async_trip",
        "Run All tests and quit"
    };

    while(1)
    {
        DebugP_log("\r\n********** CMPSS Unit Test Menu **********");

        uint8_t i;
        for( i = 0; i < NUM_TESTS+1; i++)
        {
            DebugP_log("\r\n%02d:%s",i, testList[i]);
        }

        DebugP_log("\r\nEnter option (2 characters): \r\n");

        UART_Transaction trans;
        UART_Transaction_init(&trans);
        uint8_t optionBuffer[3];
        for(i=0;i<3;i++)
        {
            optionBuffer[i]=0;
        }
        trans.buf   = &optionBuffer[0U];
        trans.count = 2;
        trans.timeout = TIMEOUT_UART_MENU;
        transferOK = UART_read(gUartHandle[CONFIG_UART0], &trans);

        int option;
        if((SystemP_SUCCESS != (transferOK)) || (UART_TRANSFER_STATUS_SUCCESS != trans.status))
        {
            DebugP_log("\r\nInvalid input! \r\n Running all tests and exit");
            option = NUM_TESTS; //""Run All tests and quit"
        }
        else
        {
            option = (int) atof((char *)optionBuffer);
            DebugP_log("\r\nEntered option: %02d", option);
        }

        switch (option)
        {
            case 0:
                RUN_TEST(CMPSS_high_low_comparator_output,  3175, NULL);
                break;
            case 1:
                RUN_TEST(CMPSS_cmpssa_high_comparator_input_source_selection, 3176, NULL);
                break;
            case 2:
                RUN_TEST(CMPSS_cmpssa_low_comparator_input_source_selection, 3177, NULL);
                break;
            case 3:
                RUN_TEST(CMPSS_comparator_hysteresis, 3178, NULL);
                break;
            case 4:
                RUN_TEST(CMPSS_vdda_vdac_selection_for_dac_reference_voltage, 3179, NULL);
                break;
            case 5:
                RUN_TEST(CMPSS_12_bit_dacs_in_cmpssa_cmpssb, 3180, NULL);
                break;
            case 6:
                RUN_TEST(CMPSS_ramp_generation, 3181, NULL);
                break;
            case 7:
                RUN_TEST(CMPSS_behavior_of_ramp_generator_during_emulation_suspend, 3182, NULL);
                break;
            case 8:
                RUN_TEST(CMPSS_digital_filter_configuration, 3183, NULL);
                break;
            case 9:
                RUN_TEST(CMPSS_pin_sharing_between_adc_and_cmpss, 3184, NULL);
                break;
            case 10:
                RUN_TEST(CMPSS_ctriph_and_ctripl_connectivity_to_pwm_xbar, 3185, NULL);
                break;
            case 11:
                RUN_TEST(CMPSS_ctriph_and_ctripl_connectivity_to_ecap_mux, 3186, NULL);
                break;
            case 12:
                RUN_TEST(CMPSS_ctripouth_and_ctripoutl_connectivity_to_output_xbar, 3187, NULL);
                break;
            case 13:
                RUN_TEST(CMPSS_extend_clear_signal_with_epwmblank, 3188, NULL);
                break;
            case 14:
                RUN_TEST(CMPSS_dac_shadow_register_synchronization_with_epwm, 3189, NULL);
                break;
            case 15:
                RUN_TEST(CMPSS_enable_disable_comparator_dac_output, 3190, NULL);
                break;
            case 16:
                RUN_TEST(CMPSS_dac_ramp_generator_synchronization_with_epwm, 3191, NULL);
                break;
            case 17:
                RUN_TEST(CMPSS_diode_emulation_mode_with_pwm, 3192, NULL);
                break;
            case 18:
                RUN_TEST(CMPSS_cmpss_async_trip, 3193, NULL);
                break;
            case 19:
            default :
                RUN_TEST(CMPSS_high_low_comparator_output,  3175, NULL);
                RUN_TEST(CMPSS_cmpssa_high_comparator_input_source_selection, 3176, NULL);
                RUN_TEST(CMPSS_cmpssa_low_comparator_input_source_selection, 3177, NULL);
                RUN_TEST(CMPSS_comparator_hysteresis, 3178, NULL);
                RUN_TEST(CMPSS_vdda_vdac_selection_for_dac_reference_voltage, 3179, NULL);
                RUN_TEST(CMPSS_12_bit_dacs_in_cmpssa_cmpssb, 3180, NULL);
                RUN_TEST(CMPSS_ramp_generation, 3181, NULL);
                RUN_TEST(CMPSS_behavior_of_ramp_generator_during_emulation_suspend, 3182, NULL);
                RUN_TEST(CMPSS_digital_filter_configuration, 3183, NULL);
                RUN_TEST(CMPSS_pin_sharing_between_adc_and_cmpss, 3184, NULL);
                RUN_TEST(CMPSS_ctriph_and_ctripl_connectivity_to_pwm_xbar, 3185, NULL);
                RUN_TEST(CMPSS_ctriph_and_ctripl_connectivity_to_ecap_mux, 3186, NULL);
                RUN_TEST(CMPSS_ctripouth_and_ctripoutl_connectivity_to_output_xbar, 3187, NULL);
                RUN_TEST(CMPSS_extend_clear_signal_with_epwmblank, 3188, NULL);
                RUN_TEST(CMPSS_dac_shadow_register_synchronization_with_epwm, 3189, NULL);
                RUN_TEST(CMPSS_enable_disable_comparator_dac_output, 3190, NULL);
                RUN_TEST(CMPSS_dac_ramp_generator_synchronization_with_epwm, 3191, NULL);
                RUN_TEST(CMPSS_diode_emulation_mode_with_pwm, 3192, NULL);
                RUN_TEST(CMPSS_cmpss_async_trip, 3193, NULL);

                UNITY_END();

                /* Close drivers */
                Board_driversClose();
                Drivers_close();

                return;
            case 99:
                if(enableLog == true)
                {
                    DebugP_log("\r\nDisabled DebugP_log");
                    enableLog = false;
                }
                else
                {
                    DebugP_log("\r\nEnabled DebugP_log");
                    enableLog = true;
                }
        }
    }

}

/* Unity framework required information */
void setUp(void)
{
}

void tearDown(void)
{
}


static void CMPSS_high_low_comparator_output(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(1), 0);
}
static void CMPSS_cmpssa_high_comparator_input_source_selection(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(2), 0);
}
static void CMPSS_cmpssa_low_comparator_input_source_selection(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(3), 0);
}
static void CMPSS_comparator_hysteresis(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(4), 0);
}
static void CMPSS_vdda_vdac_selection_for_dac_reference_voltage(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(5), 0);
}
static void CMPSS_12_bit_dacs_in_cmpssa_cmpssb(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(6), 0);
}
static void CMPSS_ramp_generation(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(7), 0);
}
static void CMPSS_behavior_of_ramp_generator_during_emulation_suspend(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(8), 0);
}
static void CMPSS_digital_filter_configuration(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(9), 0);
}
static void CMPSS_pin_sharing_between_adc_and_cmpss(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(10), 0);
}
static void CMPSS_ctriph_and_ctripl_connectivity_to_pwm_xbar(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(11), 0);
}
static void CMPSS_ctriph_and_ctripl_connectivity_to_ecap_mux(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(12), 0);
}
static void CMPSS_ctripouth_and_ctripoutl_connectivity_to_output_xbar(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(13), 0);
}
static void CMPSS_extend_clear_signal_with_epwmblank(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(14), 0);
}
static void CMPSS_dac_shadow_register_synchronization_with_epwm(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(15), 0);
}
static void CMPSS_enable_disable_comparator_dac_output(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(16), 0);
}
static void CMPSS_dac_ramp_generator_synchronization_with_epwm(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(17), 0);
}
static void CMPSS_diode_emulation_mode_with_pwm(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(18), 0);
}
static void CMPSS_cmpss_async_trip(void *args)
{
    TEST_ASSERT_EQUAL_INT32(test_cmpss_cases(19), 0);
}

uint32_t util_CMPSS_getInstanceFromBase(uint32_t base)
{
    switch(base)
    {
    case CSL_CONTROLSS_CMPSSA0_U_BASE:
            return 0;
    case CSL_CONTROLSS_CMPSSA1_U_BASE:
            return 1;
    case CSL_CONTROLSS_CMPSSA2_U_BASE:
            return 2;
    case CSL_CONTROLSS_CMPSSA3_U_BASE:
            return 3;
    case CSL_CONTROLSS_CMPSSA4_U_BASE:
            return 4;
    case CSL_CONTROLSS_CMPSSA5_U_BASE:
            return 5;
    case CSL_CONTROLSS_CMPSSA6_U_BASE:
            return 6;
    case CSL_CONTROLSS_CMPSSA7_U_BASE:
            return 7;
    case CSL_CONTROLSS_CMPSSA8_U_BASE:
            return 8;
    case CSL_CONTROLSS_CMPSSA9_U_BASE:
            return 9;

    case CSL_CONTROLSS_CMPSSB0_U_BASE:
            return 10;
    case CSL_CONTROLSS_CMPSSB1_U_BASE:
            return 11;
    case CSL_CONTROLSS_CMPSSB2_U_BASE:
            return 12;
    case CSL_CONTROLSS_CMPSSB3_U_BASE:
            return 13;
    case CSL_CONTROLSS_CMPSSB4_U_BASE:
            return 14;
    case CSL_CONTROLSS_CMPSSB5_U_BASE:
            return 15;
    case CSL_CONTROLSS_CMPSSB6_U_BASE:
            return 16;
    case CSL_CONTROLSS_CMPSSB7_U_BASE:
            return 17;
    case CSL_CONTROLSS_CMPSSB8_U_BASE:
            return 18;
    case CSL_CONTROLSS_CMPSSB9_U_BASE:
            return 19;

    default:
            return 0xFFFFFFFF;
    }

}

void util_CMPSS_dump(uint32_t base)
{
    if(enableLog)
    {
        DebugP_log("\n COMPCTL         = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPCTL         ));
        DebugP_log("\n COMPHYSCTL      = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPHYSCTL      ));
        DebugP_log("\n COMPSTS         = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPSTS         ));
        DebugP_log("\n COMPSTSCLR      = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPSTSCLR      ));
        DebugP_log("\n COMPDACCTL      = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPDACCTL      ));
        DebugP_log("\n COMPDACCTL2     = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPDACCTL2     ));
        DebugP_log("\n DACHVALS        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACHVALS        ));
        DebugP_log("\n DACHVALA        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACHVALA        ));
        DebugP_log("\n RAMPMAXREFA     = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPMAXREFA     ));
        DebugP_log("\n RAMPMAXREFS     = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPMAXREFS     ));
        DebugP_log("\n RAMPDECVALA     = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPDECVALA     ));
        DebugP_log("\n RAMPDECVALS     = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPDECVALS     ));
        DebugP_log("\n RAMPSTS         = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPSTS         ));
        DebugP_log("\n DACLVALS        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACLVALS        ));
        DebugP_log("\n DACLVALA        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACLVALA        ));
        DebugP_log("\n RAMPDLYA        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPDLYA        ));
        DebugP_log("\n RAMPDLYS        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_RAMPDLYS        ));
        DebugP_log("\n CTRIPLFILCTL    = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_CTRIPLFILCTL    ));
        DebugP_log("\n CTRIPLFILCLKCTL = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_CTRIPLFILCLKCTL ));
        DebugP_log("\n CTRIPHFILCTL    = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_CTRIPHFILCTL    ));
        DebugP_log("\n CTRIPHFILCLKCTL = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_CTRIPHFILCLKCTL ));
        DebugP_log("\n COMPLOCK        = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPLOCK        ));
        DebugP_log("\n DACHVALS2       = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACHVALS2       ));
        DebugP_log("\n DACLVALS2       = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_DACLVALS2       ));
        DebugP_log("\n CONFIG1         = 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_CONFIG1         ));
    }

}

void util_CMPSS_deInit(uint32_t base)
{
    /*Clear all registers*/
    HW_WR_REG16(base + CSL_CMPSSA_COMPCTL         , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_COMPHYSCTL      , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_COMPSTS         , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_COMPSTSCLR      , 0x0202);
    HW_WR_REG16(base + CSL_CMPSSA_COMPDACCTL      , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_COMPDACCTL2     , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACHVALS        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACHVALA        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPMAXREFA     , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPMAXREFS     , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPDECVALA     , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPDECVALS     , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPSTS         , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACLVALS        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACLVALA        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPDLYA        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_RAMPDLYS        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_CTRIPLFILCTL    , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_CTRIPLFILCLKCTL , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_CTRIPHFILCTL    , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_CTRIPHFILCLKCTL , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_COMPLOCK        , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACHVALS2       , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_DACLVALS2       , 0x0000);
    HW_WR_REG16(base + CSL_CMPSSA_CONFIG1         , 0x0000);

    /*Module reset*/

    if(enableLog) DebugP_log("\r\n util_CMPSS_deInit");

    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK0 ,0x01234567);
    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK1 ,0xFEDCBA8);

    uint8_t i = util_CMPSS_getInstanceFromBase(base);

    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_CMPSSA0_RST + (i*4),0x07);
    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_CMPSSA0_RST + (i*4),0x00);


}

void util_EPWM_deInit()
{

    if(enableLog) DebugP_log("\r\n util_EPWM_deInit");

    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK0 ,0x01234567);
    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK1 ,0xFEDCBA8);

    uint8_t i;

    for(i=0;i<32;i++)
    {
        HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_ETPWM0_RST + (i*4),0x07);
        HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_ETPWM0_RST + (i*4),0x00);
    }

}



void util_EPWM_config(uint32_t base, uint16_t period)
{
    if(enableLog) DebugP_log("\r\nEPWM %d configuration", (base-CSL_CONTROLSS_G0_EPWM0_U_BASE)/0x1000);

    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK0 ,0x01234567);
    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_LOCK0_KICK1 ,0xFEDCBA8);

    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_EPWM_CLKSYNC ,0xFFFFFFFF);
    //FIXME: Replace?
//    HW_WR_REG32(CSL_CONTROLSS_CTRL_U_BASE + CSL_CONTROLSS_CTRL_EPWM_GBCLKSYNC ,0x10);


//    uint32_t reg;
//    reg = HW_RD_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS0_CORE0);
//    reg = reg | 0x00070000;
//    HW_WR_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS0_CORE0,reg);
//
//    reg = HW_RD_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS0_CORE1);
//    reg = reg | 0x00070000;
//    HW_WR_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS0_CORE1,reg);
//
//    reg = HW_RD_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS1_CORE0);
//    reg = reg | 0x00070000;
//    HW_WR_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS1_CORE0,reg);
//
//    reg = HW_RD_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS1_CORE1);
//    reg = reg | 0x00070000;
//    HW_WR_REG32(CSL_HSM_SOC_CTRL_U_BASE +CSL_HSM_SOC_CTRL_ISC_CTRL_REG_R5SS1_CORE1,reg);

    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_IO_CFG_KICK0,0x83E70B13);
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_IO_CFG_KICK1,0x95A4F1E0);

    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM0_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM0_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM1_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM1_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM2_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM2_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM3_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM3_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM4_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM4_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM5_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM5_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM6_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM6_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM7_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM7_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM8_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM8_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM9_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM9_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM10_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM10_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM11_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM11_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM12_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM12_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM13_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM13_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM14_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM14_B_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM15_A_CFG_REG, 0x00) ;
    HW_WR_REG32(CSL_IOMUX_U_BASE + CSL_IOMUX_EPWM15_B_CFG_REG, 0x00) ;



    //config 3

    // Configure Time Base counter Clock - Write to CLKDIV and HSPCLKDIV bit
    EPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    ////DebugP_log("\nmain: CTL Configured");


    // Configure Time Base Counter Mode - Write to CTRMODE bit
    EPWM_setTimeBaseCounterMode( base, EPWM_COUNTER_MODE_UP_DOWN);

    // Configure TBPRD value - Write to TBPRD bit
    EPWM_setTimeBasePeriod( base, period);

    //
    // Default Configurations.
    //
    EPWM_disablePhaseShiftLoad(base);

    EPWM_setPhaseShift(base, 0U);

    // Write to TBCTR register
    EPWM_setTimeBaseCounter(base, 0);
    ////DebugP_log("\nmain: CTR Configured");

    //
    // Setup shadow register load on ZERO
    //
    EPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Set Compare values
    //
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A,
                                30);

    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B,
                                30);

//    // Set PWM output to toggle on CTR = PRD
//    // Write to PRD bit of AQCTLA register
//    EPWM_setActionQualifierAction(base,
//                                  EPWM_AQ_OUTPUT_A,
//                                  EPWM_AQ_OUTPUT_LOW,
//                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);

    // Write to PRD bit of AQCTLA register
    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    // Write to PRD bit of AQCTLA register
    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    // Write to PRD bit of AQCTLA register
    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    // Write to PRD bit of AQCTLA register
    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    ////DebugP_log("\nmain: AQ Configured");
    //End of Config 3




}

void util_EPWM_config_trip(uint32_t base)
{



    //
    // Configure to output high on TZB TRIP
    //Writes CSL_EPWM_TZCTL, Fields: 0-1-TZA, 2-3-TZB, 4-5-DCAEVT1, 6-7-DCAEVT2, 8-9-DCBEVT1, 10-11-DCBEVT2
    //Field values: 00-Z, 01-High, 10-Low, 11-Disabled
    // Writes 0b01 (High) to bits 2-3 (TZB)
    // Trip EPWMxB
    EPWM_setTripZoneAction(base, EPWM_TZ_ACTION_EVENT_TZB,
                           EPWM_TZ_ACTION_HIGH);

    //
    // Trigger event when DCBH is high
    //CSL_EPWM_TZDCSEL
    //
    EPWM_setTripZoneDigitalCompareEventCondition(base,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);

    //
    // Configure DCBH to use TRIP4 as an input
    //CSL_EPWM_DCAHTRIPSEL
    //CSL_EPWM_DCTRIPSEL
    //
    EPWM_enableDigitalCompareTripCombinationInput(base,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    //
    // Enable DCB as OST
    //CSL_EPWM_TZSEL, Fields: 0-CBC1, 1-CBC2, 2-CBC3, 3-CBC4, 4-CBC5, 5-CBC6, 6-DCAEVT2, 7-DCBEVT2, 8-OSHT1, 9-OSHT2, 10-OSHT3, 11-OSHT4, 12-OSHT5, 13-OSHT6, 14-DCAEVT1, 15-DCBEVT1
    //Fields: 0-7 - TZ1, TZ2, TZ3, TZ4, TX5, TZ6, DCAEVT2, DCBEVT2 as Cycle by cycle
    //Fields: 8-15 - TZ1, TZ2, TZ3, TZ4, TX5, TZ6, DCAEVT1, DCBEVT1 as One shot
    //Field values: 1 to enable trip source
    // Writes 1 (enables) to bit15(DCBEVT1 as one shot)
    EPWM_enableTripZoneSignals(base, EPWM_TZ_SIGNAL_DCBEVT1);

    //
    // Configure the DCB path to be unfiltered and asynchronous
    //CSL_EPWM_DCACTL
    //
    EPWM_setDigitalCompareEventSource(base,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);

    //
    // Sync the ePWM time base clock
    //
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Configure TRIP4 to be CTRIP1H using the ePWM X-BAR
    //
    //XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX00_CMPSS1_CTRIPH);
    //XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX00);


    //
    // Clear trip flags
    //Writes to CSL_EPWM_TZCLR, Fields: 0-INT, 1-CBC, 2-OST, 3-DCAEVT1, 4-DCAEVT2, 5-DCBEVT1, 6-DCBEVT2, 14-15-CBCPULSE
    //Field values: 1 to clear
    // Writes 0b1 to bit0 and bit2
    EPWM_clearTripZoneFlag(base, EPWM_TZ_INTERRUPT |
                           EPWM_TZ_FLAG_OST);


}

void util_EPWM_config_blank(uint32_t base, uint16_t windowLengthCount)
{

    EPWM_setDigitalCompareBlankingEvent(base, EPWM_DC_WINDOW_START_TBCTR_ZERO, 0);
    EPWM_setDigitalCompareWindowOffset(base, 0);
    EPWM_setDigitalCompareWindowLength(base, windowLengthCount);
    EPWM_enableDigitalCompareBlankingWindow(base);
}

void util_EPWM_deConfig_blank(uint32_t base)
{
    EPWM_disableDigitalCompareBlankingWindow(base);
    EPWM_setDigitalCompareBlankingEvent(base, EPWM_DC_WINDOW_START_TBCTR_PERIOD, 0);
    EPWM_setDigitalCompareWindowOffset(base, 0);
    EPWM_setDigitalCompareWindowLength(base, 0);
}

void util_EPWM_config_upcount(uint32_t base)
{

    EPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBaseCounter(base, 0);
    EPWM_setTimeBasePeriod(base, 0xFFFF);
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B, 0x8000);
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A, 0x8000);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP);

}



//
// util_ECAP_init - Configure eCAP
//
void util_ECAP_init(uint32_t base, uint32_t input)
{
    //
    // Disable ,clear all capture flags and interrupts
    //
    ECAP_disableInterrupt(base,
                          (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                          ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                          ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                          ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                          ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                          ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                          ECAP_ISR_SOURCE_COUNTER_COMPARE));
    ECAP_clearInterrupt(base,
                        (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
                        ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
                        ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
                        ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
                        ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
                        ECAP_ISR_SOURCE_COUNTER_PERIOD   |
                        ECAP_ISR_SOURCE_COUNTER_COMPARE));

    //
    // Disable CAP1-CAP4 register loads
    //
    ECAP_disableTimeStampCapture(base);

    //
    // Configure eCAP
    //    Enable capture mode.
    //    One shot mode, stop capture at event 4.
    //    Set polarity of the events to rising, falling, rising, falling edge.
    //    Set capture in time difference mode.
    //    Select input from XBAR7.
    //    Enable eCAP module.
    //    Enable interrupt.
    //
    ECAP_stopCounter(base);
    ECAP_enableCaptureMode(base);

    ECAP_setCaptureMode(base, ECAP_ONE_SHOT_CAPTURE_MODE, ECAP_EVENT_4);

    ECAP_setEventPolarity(base, ECAP_EVENT_1, ECAP_EVNT_FALLING_EDGE);
    ECAP_setEventPolarity(base, ECAP_EVENT_2, ECAP_EVNT_RISING_EDGE);
    ECAP_setEventPolarity(base, ECAP_EVENT_3, ECAP_EVNT_FALLING_EDGE);
    ECAP_setEventPolarity(base, ECAP_EVENT_4, ECAP_EVNT_RISING_EDGE);

    ECAP_enableCounterResetOnEvent(base, ECAP_EVENT_1);
    ECAP_enableCounterResetOnEvent(base, ECAP_EVENT_2);
    ECAP_enableCounterResetOnEvent(base, ECAP_EVENT_3);
    ECAP_enableCounterResetOnEvent(base, ECAP_EVENT_4);

    //XBAR_setInputPin(INPUTXBAR_BASE, XBAR_INPUT7, 16);
    ECAP_selectECAPInput(base, input);

    ECAP_enableLoadCounter(base);
    ECAP_setSyncOutMode(base, ECAP_SYNC_OUT_SYNCI);
    ECAP_startCounter(base);
    ECAP_enableTimeStampCapture(base);
    ECAP_reArm(base);

    ECAP_enableInterrupt(base, ECAP_ISR_SOURCE_CAPTURE_EVENT_4);
}


int32_t AM263x_CMPSS_BTR_0001(uint32_t base)
{
  // COMP: High/Low Comparator output
  // Enables the comparators and selects the CTRIPH/CTRIPL and CTRIPOUTH/CTRIPOUTL source to asynchronous or synchronous output, with and without inversion
  // Comparator output high if positive input>negative input. Low if otherwise.

  if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0001", util_CMPSS_getInstanceFromBase(base));

  uint32_t error=0;

  tester_command("gen dac voltage 1.000v");


  int testcase;

  for(testcase=0;testcase<4;testcase++)
  {
      CMPSS_enableModule(base);

      if(testcase==0)           /*No inversion, Async */
      {
          CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
          CMPSS_configLowComparator(base, CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: No, Async or with filter: No
          CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
          CMPSS_configOutputsLow(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
          //TBD: Analog verification
      }

      if(testcase==1)           /*Inversion, Async */
      {
          CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No
          CMPSS_configLowComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: Yes, Async or with filter: No
          CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
          CMPSS_configOutputsLow(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
          //TBD: Analog verification
      }

      if(testcase==2)           /*No inversion, Sync */
      {
          CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
          CMPSS_configLowComparator(base, CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: No, Async or with filter: No
          CMPSS_configOutputsHigh(base, CMPSS_TRIP_SYNC_COMP | CMPSS_TRIPOUT_SYNC_COMP);    //TRIP: sync TRIPOUT: sync
          CMPSS_configOutputsLow(base, CMPSS_TRIP_SYNC_COMP | CMPSS_TRIPOUT_SYNC_COMP);    //TRIP: sync TRIPOUT: sync
          //TBD: Analog verification
      }

      if(testcase==3)
      {
          CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No
          CMPSS_configLowComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: Yes, Async or with filter: No
          CMPSS_configOutputsHigh(base, CMPSS_TRIP_SYNC_COMP | CMPSS_TRIPOUT_SYNC_COMP);    //TRIP: sync TRIPOUT: sync
          CMPSS_configOutputsLow(base, CMPSS_TRIP_SYNC_COMP | CMPSS_TRIPOUT_SYNC_COMP);    //TRIP: sync TRIPOUT: sync
          //TBD: Analog verification
      }

      CMPSS_disableModule(base);
  }

  util_CMPSS_deInit(base);

  if(error==0)
  {
      if(enableLog) DebugP_log("\r\nPass");

      return 0;
  }
  else
  {
      if(enableLog) DebugP_log("\r\nFail");

      return 1;
  }
}

int32_t AM263x_CMPSS_BTR_0002(uint32_t base)
{
    // COMP: CMPSSA high comparator input source selection
    // Selects high comparator negative input source as internal DAC/external pin (ADC)
    // Check High comparator output reponds to configured input source only

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0002", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    int testcase;

    for(testcase=0;testcase<2;testcase++)
    {
        CMPSS_enableModule(base);
        if(testcase==0)
        {
            CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
        }
        {
            CMPSS_configHighComparator(base, CMPSS_INSRC_DAC);  //Source: DAC, Inversion: No, Async or with filter: No
        }
        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
        //TBD: Analog verification
        CMPSS_disableModule(base);
    }

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_BTR_0003(uint32_t base)
{
    // COMP: CMPSSA low comparator input source selection
    // Selects low comparator positive input source as the INP(of connected ADC pin)/INM(ADC of connected ADC pin)
    // Check Low comparator output responds to configured input source only

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0003", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    //TBD: Which register to program?

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_BTR_0004(uint32_t base)
{
    // COMP: Comparator hysteresis
    // Sets different amount (1x to 4x) of hysteresis on comparator inputs
    // Comparator output shows configured hysteresis (12 bits to 48 bits)

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0004", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    uint8_t hysteresisinput=1;

    for(hysteresisinput=0;hysteresisinput<7;hysteresisinput++)
    {
        CMPSS_enableModule(base);
        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: N
        CMPSS_setComparatorHighHysteresis(base,hysteresisinput);
        if(hysteresisinput!=((HW_RD_REG16(base + CSL_CMPSSA_CONFIG1)>>CSL_CMPSSA_CONFIG1_COMPHHYS_SHIFT) & CSL_CMPSSA_CONFIG1_COMPHHYS_MASK))
        {
            error++;
        }



        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
        //TBD: Analog verification
        CMPSS_disableModule(base);
    }

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}


int32_t AM263x_CMPSS_BTR_0005(uint32_t base)
{
    // DAC: VDDA/VDAC selection for DAC reference voltage
    // Selects VDDA/VDAC as reference voltage
    // Comparator output reflects according to configured DAC reference voltage

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0005", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    uint8_t testcase=0;

    for(testcase=0;testcase<2;testcase++)
    {
        CMPSS_enableModule(base);
        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: N

        //CSL_CMPSSA_COMPDACCTL_SELREF_MASK = 0 VDDA
        //CSL_CMPSSA_COMPDACCTL_SELREF_MASK = 1 VDAC
        if(testcase==0)
        {
            CMPSS_configDAC(base, (1<<CSL_CMPSSA_COMPDACCTL_SELREF_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_SWLOADSEL_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_DACSOURCE_SHIFT));
        }
        if(testcase==1)
        {
            CMPSS_configDAC(base, (0<<CSL_CMPSSA_COMPDACCTL_SELREF_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_SWLOADSEL_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_DACSOURCE_SHIFT));
        }


        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async
        //TBD: Analog verification
        CMPSS_disableModule(base);
    }

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t test_cmpss_dac_swload(uint32_t base, uint32_t diode_emulation_mode)
{
    uint32_t error=0;

    uint16_t dac_input_index=0;
    uint16_t dac_input_vals[10]={0, 1, 2, 5, 10, 50, 100, 1024, 2048, 0xFFE};
    uint16_t dac_input=0;

    //CSL_CMPSSA_COMPDACCTL_DACSOURCE_MASK      Input value to DAC (DACHVALA) is from DACVALS (0) or ramp generator(1)
    //CSL_CMPSSA_COMPDACCTL_SELREF_SHIFT        (0) VDDA or (1) VDAC as DAC reference voltage
    //CSL_CMPSSA_COMPDACCTL_SWLOADSEL_MASK      DACxVALS is loaded to active upon (0) SYSCLK or (1) EPWMSYNCPER (selected by ?)
    CMPSS_enableModule(base);
    CMPSS_configDAC(base, (0<<CSL_CMPSSA_COMPDACCTL_SELREF_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_SWLOADSEL_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_DACSOURCE_SHIFT));

    if(diode_emulation_mode==1)
    {
        //Enable CMPSS diode emulation logic in CMPSS
        CMPSS_enableDEmode(base);
        CMPSS_selectDEACTIVESource(base, 0);
        //Configure EPWM and DEL
        util_EPWM_config(CSL_CONTROLSS_G0_EPWM0_U_BASE, 49);

        // MODE
        //0 : DEACTIVE flag works in cycle by cycle mode. On every PWMSYNCOUT, set condition of DEACTIVE flag is evaluated. If the set condition is not present the flag is cleared.
        //1 : DEACTIVE flag works in one shot mode (hardware set) and software clear.
        //ENABLE
        //DE function enable
        //0 : Diode Emulation mode functionality is disabled. DEACTIVE flag is not set on a TRIPH_OR_TRIPL event.
        //1 : Diode Emulation mode functionality is enabled. DEACTIVE flag is set on a TRIPH_OR_TRIPL event. Note: ENABLE bit is cleared on a PWMTRIP event. Software has to re-enable this bit after PWMTRIP condition is serviced.

        //EPWM_enableDEL();
        HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE + CSL_EPWM_DECTL, (HW_RD_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE + CSL_EPWM_DECTL) | CSL_EPWM_DECTL_ENABLE_MASK | CSL_EPWM_DECTL_MODE_MASK));

    }

    HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_DEFRC,CSL_EPWM_DEFRC_DEACTIVE_MASK);

    for(dac_input_index=0;dac_input_index<10;dac_input_index++)
    {
        dac_input = dac_input_vals[dac_input_index];

        if(enableLog) DebugP_log("\r\ndac_input=0x%04x", dac_input);
        uint16_t active_dac_value;

        if(diode_emulation_mode != 1)
        {
            CMPSS_setDACValueHigh(base, dac_input);
        }
        else
        {
            CMPSS_setDACValueHigh(base, (dac_input+1));         //Store any other value in CSL_CMPSSA_DACHVALS
            CMPSS_configHighDACShadowValue2(base, dac_input);   //Store input value in CSL_CMPSSA_DACHVALS2
            //Force DEACTIVE
            HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_DEFRC, CSL_EPWM_DEFRC_DEACTIVE_MASK);
        }
        active_dac_value = CMPSS_getDACValueHigh(base);

        if(diode_emulation_mode == 1)
        {
            //delay/read again
            active_dac_value = CMPSS_getDACValueHigh(base);
            active_dac_value = CMPSS_getDACValueHigh(base);
            active_dac_value = CMPSS_getDACValueHigh(base);
            active_dac_value = CMPSS_getDACValueHigh(base);
            active_dac_value = CMPSS_getDACValueHigh(base);
            active_dac_value = CMPSS_getDACValueHigh(base);
        }

        if(active_dac_value != (dac_input & CSL_CMPSSA_DACHVALA_DACVAL_MASK))
        {
            if(enableLog) DebugP_log("\r\n error: CMPSS_getDACValueHigh=0x%04x", active_dac_value);
            error++;
        }

        if(diode_emulation_mode == 1)
        {
            HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_DECLR, CSL_EPWM_DECLR_DEACTIVE_MASK);
        }





        if(diode_emulation_mode != 1)
        {
            CMPSS_setDACValueLow(base, dac_input);
        }
        else
        {
            CMPSS_setDACValueLow(base, (dac_input+1));          //Store any other value in CSL_CMPSSA_DACHVALS
            CMPSS_configLowDACShadowValue2(base, dac_input);    //Store input value in CSL_CMPSSA_DACHVALS2
            //Force DEACTIVE
            HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_DEFRC, CSL_EPWM_DEFRC_DEACTIVE_MASK);
        }
        active_dac_value = CMPSS_getDACValueLow(base);

        if(diode_emulation_mode == 1)
        {
            //delay/read again
            active_dac_value = CMPSS_getDACValueLow(base);
            active_dac_value = CMPSS_getDACValueLow(base);
            active_dac_value = CMPSS_getDACValueLow(base);
            active_dac_value = CMPSS_getDACValueLow(base);
            active_dac_value = CMPSS_getDACValueLow(base);
            active_dac_value = CMPSS_getDACValueLow(base);
        }

        if(active_dac_value != (dac_input & CSL_CMPSSA_DACHVALA_DACVAL_MASK))
        {
            if(enableLog) DebugP_log("\r\n error: CMPSS_getDACValueLow=0x%04x", active_dac_value);
            error++;
        }

        if(diode_emulation_mode == 1)
        {
            HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_DECLR, CSL_EPWM_DECLR_DEACTIVE_MASK);
        }
    }

    if(diode_emulation_mode==1)
    {
        //TBD: deinit EPWM
        HW_WR_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE + CSL_EPWM_DECTL, (HW_RD_REG32(CSL_CONTROLSS_G0_EPWM0_U_BASE + CSL_EPWM_DECTL) & (~(CSL_EPWM_DECTL_ENABLE_MASK | CSL_EPWM_DECTL_MODE_MASK))));
        util_EPWM_deInit();
        CMPSS_disableDEmode(base);
    }

    CMPSS_disableModule(base);

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }

}

int32_t AM263x_CMPSS_BTR_0006(uint32_t base)
{
    // DAC: 12 bit DACs in CMPSSA, 8 bit DACs in CMPSSB
    // Enables 12 bit DACs in CMPSSA and 8 bt DACs in CMPSSB
    // Active DAC value is 12 bit/8 bit as programmed

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0006", util_CMPSS_getInstanceFromBase(base));

    return test_cmpss_dac_swload(base, 0);
}

int32_t test_cmpss_ramp_pwm(uint32_t base)
{
    uint32_t error=0;

    //Configure ramp
    uint16_t maxRampVal = 0xFFFFU;
    uint16_t decrementVal = 0x1;
    uint16_t delayVal = 0;
    uint16_t pwmSyncSrc = 1;
    bool useRampValShdw = 0;
    CMPSS_configRamp(base, maxRampVal, decrementVal, delayVal, pwmSyncSrc, useRampValShdw);


    //Config PWM
    uint16_t tbctr_prd=2000;
    util_EPWM_config(CSL_CONTROLSS_G0_EPWM0_U_BASE, tbctr_prd);
    //Config EPWMSYNCPER
    //HRPCTL default value already configures PWMSYNCSELX as 000 and PWMSYNCSEL as 0. So PWMSYNCSEL is used for EPWMSYNCPER and CTR=PRD is used

    //Configure DAC
    //Sequence dependency: configRamp first and configDac next. Setting DACSOURCE after configRamp loads MAXREFS to MAXREFA
    CMPSS_configDAC(base, CMPSS_DACSRC_RAMP);

    uint32_t loop_count;
    uint16_t rampsts_logs[100];
    uint16_t dacval_logs[100];
    uint16_t pwmtbctr_logs[100];

    uint16_t margin=200;    //Release/debug mode dependent latency margin. Per loop, pwm tbctr/ramp val delta is approx 50 in release mode, approx 100 in debug mode.
    for(loop_count=0;loop_count<100;loop_count++)
    {
        rampsts_logs[loop_count]=HW_RD_REG16(base+CSL_CMPSSA_RAMPSTS);
        dacval_logs[loop_count]=HW_RD_REG16(base+CSL_CMPSSA_DACHVALA);
        pwmtbctr_logs[loop_count]=HW_RD_REG16(CSL_CONTROLSS_G0_EPWM0_U_BASE+CSL_EPWM_TBCTR);
    }

    //Verify RAMPSTS logs
    //Verify if TBCTR is close to EPWMSYNCPER source

    uint32_t stuck_count=0;
    for(loop_count=0;loop_count<100-1;loop_count++)
    {
        ////DebugP_log("\nrampsts = 0x%04x dacval = 0x%04x, pwmtbctr = 0x%04x", rampsts_logs[loop_count], dacval_logs[loop_count], pwmtbctr_logs[loop_count]);

        if(rampsts_logs[loop_count+1] < rampsts_logs[loop_count])
        {
            //OK, Ramp decrementing
        }
        else
        {
            //DebugP_log("\nramp stuck or increasing: rampsts_logs[loop_count+1] = 0x%04x, rampsts_logs[loop_count] = 0x%04x", rampsts_logs[loop_count+1], rampsts_logs[loop_count]);
            if(rampsts_logs[loop_count+1] == rampsts_logs[loop_count])
            {
                //Not ok, ramp stuck
                //DebugP_log("\nramp stuck: stuck_count=%d", stuck_count);
                if( (rampsts_logs[loop_count+1] == maxRampVal) && (rampsts_logs[loop_count] == maxRampVal))
                {
                    //OK, Ramp decrement not started, ramp is at maxrampval
                    stuck_count++;
                }
                else
                {
                    //Error
                    DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count, rampsts_logs[loop_count],loop_count, pwmtbctr_logs[loop_count]);
                    DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count+1, rampsts_logs[loop_count+1],loop_count+1, pwmtbctr_logs[loop_count+1]);

                    DebugP_log("\nerror 1");
                    error++;
                }
            }
            else
            {
                //Not ok, ramp increasing
                //DebugP_log("\nramp increasing");
                if(rampsts_logs[loop_count+1]>=(maxRampVal-margin))
                {
                    //OK, ramp reset
                    if(pwmtbctr_logs[loop_count+1] > (tbctr_prd - margin))
                    {
                        //OK, In up down count mode, TBCTR will be around PRD. Same as EPWMSYNCPER
                        //DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count, rampsts_logs[loop_count],loop_count, pwmtbctr_logs[loop_count]);
                        //DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count+1, rampsts_logs[loop_count+1],loop_count+1, pwmtbctr_logs[loop_count+1]);

                    }
                    else
                    {
                        //Error
                        DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count, rampsts_logs[loop_count],loop_count, pwmtbctr_logs[loop_count]);
                        DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count+1, rampsts_logs[loop_count+1],loop_count+1, pwmtbctr_logs[loop_count+1]);

                        DebugP_log("\nerror 2");
                        error++;
                    }
                }
                else
                {
                    //Error

                    DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count, rampsts_logs[loop_count],loop_count, pwmtbctr_logs[loop_count]);
                    DebugP_log("\nrampsts[%d] = 0x%04x, pwmtbctr[%d] = 0x%04x", loop_count+1, rampsts_logs[loop_count+1],loop_count+1, pwmtbctr_logs[loop_count+1]);

                    DebugP_log("\nerror 3");
                    error++;
                }
            }

        }
    }

    //Verify if emulation suspend stops ramp - DV

    util_CMPSS_deInit(base);

    util_EPWM_deInit();

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_BTR_0007(uint32_t base)
{
    // RAMP: Ramp generation
    // Configures ramp generation to feed to DAC
    // Active DAC value updates as per programmed ramp

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0007", util_CMPSS_getInstanceFromBase(base));

    return test_cmpss_ramp_pwm(base);
}

int32_t AM263x_CMPSS_BTR_0008(uint32_t base)
{
    // RAMP: Behavior of the ramp generator during emulation suspend
    // Configure ramp generator behavior during emulation suspend
    // Ramp generation stops immediately/stops at next EPWMSYNCPER/does not stop

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0008", util_CMPSS_getInstanceFromBase(base));

    //TBD: manual verification during emulation suspend
    return test_cmpss_ramp_pwm(base);
}

int32_t test_cmpss_digital_filter(uint32_t base)
{

    uint32_t error=0;

    CMPSS_enableModule(base);
    CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
    CMPSS_configOutputsHigh(base, CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER);    //TRIP: filter TRIPOUT: filter

    CMPSS_configLowComparator(base, CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: No, Async or with filter: No
    CMPSS_configOutputsLow(base, CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER);    //TRIP: filter TRIPOUT: filter


    CMPSS_configFilterHigh(base, 0x3FF, 32, 31);
    CMPSS_initFilterHigh(base);

    CMPSS_configFilterLow(base, 0x3FF, 32, 31);
    CMPSS_initFilterLow(base);


    //Force output to 1 using inversion
    CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No
    CMPSS_configLowComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: Yes, Async or with filter: No

    //DebugP_log("\nCOMPCTL 0x%04x", HW_RD_REG16(base + CSL_CMPSSB_COMPCTL));
    uint16_t sts=0;
    uint32_t wait_for_status;
    for(wait_for_status=0;wait_for_status<(1024*32*10);wait_for_status++)
    {
        sts=CMPSS_getStatus(base);
    }
    //DebugP_log("\nCMPSS_getStatus returns 0x%04x", sts);

    //other bit pattern to digital filter and check - DV

    if( (sts&(CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) == (CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK))
    {
        //DebugP_log("\nFilter ok");
    }
    else
    {
        //DebugP_log("\nFilter error");
        error++;
    }

    CMPSS_configHighComparator(base, 0);
    CMPSS_clearFilterLatchHigh(base);
    CMPSS_disableModule(base);


    util_CMPSS_deInit(base);


    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_BTR_0009(uint32_t base)
{
    // FILTER: Digital Filter Configuration
    // Enables the comparators, configures the digital filter and selects the CTRIPH/CTRIPL and CTRIPOUTH/CTRIPOUTL source to digital filter or latched digital filter output
    // Digital filter output as per filter configuration


    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_BTR_0009", util_CMPSS_getInstanceFromBase(base));

    return test_cmpss_digital_filter(base);

}

int32_t AM263x_CMPSS_ITR_0001(uint32_t base)
{
    // Pin sharing between ADC and CMPSS
    // Tests the pin sharing: 2 differential pairs of ADC shared with 2 CMPSSA. 1 differential pair of ADC shared with 2 CMPSSB.
    // ADC and CMPSS output reflects according to same input signal

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0001", util_CMPSS_getInstanceFromBase(base));


    uint32_t error=0;

    //Force ADC pins - need TB support?

    //Verify signal availability using waveforms?

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_ITR_0002(uint32_t base)
{
    // COMP: CTRIPH and CTRIPL connectivity to PWM Xbar
    // Configures CMPSS outputs CTRIPH and CTRIPL to feed to PWM Xbar
    // CTRIPH and CTRIPL reaches PWMXbar outputs (EPWM trip/ICSS)

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0002", util_CMPSS_getInstanceFromBase(base));

    //FIXME: repeat for CTRIPL

    uint32_t error=0;

    CMPSS_enableModule(base);
    CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
    CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async

    //Force output to 1 using inversion
    CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No

    //DebugP_log("\r\nCOMPCTL 0x%04x", HW_RD_REG16(base + CSL_CMPSSA_COMPCTL));
    //DebugP_log("\r\nCMPSS_getStatus returns 0x%04x", CMPSS_getStatus(base));

    uint32_t wait_for_status;
    uint16_t sts;
    for(wait_for_status=0;wait_for_status<100;wait_for_status++)
    {
        sts=CMPSS_getStatus(base);
    }
    (void)sts; /* Presently set but not used. Suppress warning */

    //Configure PWMXbar
    //Configure each PWMXbar and check status
    uint8_t out;
    for(out=0;out<30;out++)
    {
        uint8_t shift;
        if((util_CMPSS_getInstanceFromBase(base)>=0)&&(util_CMPSS_getInstanceFromBase(base)<10))
        {
            shift = (util_CMPSS_getInstanceFromBase(base) * 2) + 1;
            SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, out, 0x1<<shift, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        if((util_CMPSS_getInstanceFromBase(base)>=10)&&(util_CMPSS_getInstanceFromBase(base)<20))
        {
            shift = ((util_CMPSS_getInstanceFromBase(base)-10) * 2) + 1;
            SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, out, 0, 0x1<<shift, 0, 0, 0, 0, 0, 0, 0);
        }

        uint32_t status=SOC_xbarGetPWMXBarOutputSignalStatus(CSL_CONTROLSS_PWMXBAR_U_BASE);
        //DebugP_log("\nPWMXbar %d SOC_xbarGetPWMXBarOutputSignalStatus returns 0x%08x", out, status);

        if(status != (0x1<<out))
        {
            DebugP_log("\r\nPWMXbar %d SOC_xbarGetPWMXBarOutputSignalStatus returns 0x%08x", out, status);

            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G0));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G1));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G2));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G3));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G4));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G5));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G6));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G7));
            DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G8));

            uint32_t i;
            for(i=0;i<30;i++)
            {
                //DebugP_log("\r\n0x%08x", HW_RD_REG32(CSL_CONTROLSS_PWMXBAR_U_BASE + out*CSL_CONTROLSS_PWMXBAR_STEP + CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G0 + 0x40*i));
            }


            DebugP_log("\r\nerror");
            error++;
        }

        SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, out, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    CMPSS_configHighComparator(base, 0);
    CMPSS_clearFilterLatchHigh(base);
    CMPSS_disableModule(base);

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}


int32_t AM263x_CMPSS_ITR_0003(uint32_t base)
{
    // COMP: CTRIPH and CTRIPL connectivity to ECAP Mux
    // Configures CMPSS outputs CTRIPH and CTRIPL to feed to ECAP input mux
    // CTRIPH and CTRIPL reaches ECAP

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0003", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    uint32_t ecap_index;
    uint32_t high_low=1;

    //TBD: check CTRIPL also

    for(ecap_index=0;ecap_index<10;ecap_index++)
    {
        //Configure CMPSS
        CMPSS_enableModule(base);
        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async

        //Configure ECAP
        uint32_t cmpss_index = util_CMPSS_getInstanceFromBase(base);
        uint32_t ecap_base = CSL_CONTROLSS_ECAP0_U_BASE + (ecap_index*0x1000);
        util_ECAP_init(ecap_base, ECAP_INPUT_CMPSSA0_CTRIP_LOW + high_low + cmpss_index*2);

        uint32_t toggle;
        uint16_t sts;

        for(toggle=0;toggle<4;toggle++)
        {
            //Force output to 1 using inversion
            CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No

            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);

            //Force output to 0
            CMPSS_configHighComparator(base, 0 | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No

            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);
            sts=CMPSS_getStatus(base);

        }
        (void)sts; /* Presently set but not used. Suppress warning */

        //Check ecap timestamps
        uint32_t timestamp_event1 = ECAP_getEventTimeStamp(ecap_base, ECAP_EVENT_1);
        uint32_t timestamp_event2 = ECAP_getEventTimeStamp(ecap_base, ECAP_EVENT_2);
        uint32_t timestamp_event3 = ECAP_getEventTimeStamp(ecap_base, ECAP_EVENT_3);
        uint32_t timestamp_event4 = ECAP_getEventTimeStamp(ecap_base, ECAP_EVENT_4);
        //DebugP_log("\ntimestamp_event1 = 0x%08x, timestamp_event2 = 0x%08x, timestamp_event3 = 0x%08x, timestamp_event4 = 0x%08x", timestamp_event1, timestamp_event2, timestamp_event3, timestamp_event4);
        //tbd: verify
        if(timestamp_event1==0)
        {
            if(timestamp_event2==0)
            {
                if(timestamp_event3==0)
                {
                    if(timestamp_event4==0)
                    {
                        //DebugP_log("\nerror");
                        error++;
                    }
                }
            }
        }

        CMPSS_configHighComparator(base, 0);
        CMPSS_clearFilterLatchHigh(base);
        CMPSS_disableModule(base);

        util_CMPSS_deInit(base);
    }

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_ITR_0004(uint32_t base)
{
    // COMP: CTRIPOUTH and CTRIPOUTL connectivity to Output Xbar
    // Configures CMPSS outputs CTRIPOUTH and CTRIPOUTL to output Xbar
    // CTRIPOUTH ad CTRIPOUTL reaches Output Xbar outputs (GPIO)

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0004", util_CMPSS_getInstanceFromBase(base));

    //FIXME: repeat for CTRIPOUTL

    uint32_t error=0;

    CMPSS_enableModule(base);
    CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
    CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async

    //Force output to 1 using inversion
    CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No

    //DebugP_log("\nCOMPCTL 0x%04x", HW_RD_REG16(base + CSL_CMPSSB_COMPCTL));
    //DebugP_log("\nCMPSS_getStatus returns 0x%04x", CMPSS_getStatus(base));

    //Configure each Output Xbar and check status
    uint8_t out;
    for(out=0;out<16;out++)
    {
        uint8_t shift;
        if((util_CMPSS_getInstanceFromBase(base)>=0)&&(util_CMPSS_getInstanceFromBase(base)<10))
        {
            shift = (util_CMPSS_getInstanceFromBase(base) * 2) + 1;
            SOC_xbarSelectOutputXBarInputSource(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, out, 0,0,0,0,0,0,0x1<<shift,0,0,0,0);
        }
        if((util_CMPSS_getInstanceFromBase(base)>=10)&&(util_CMPSS_getInstanceFromBase(base)<20))
        {
            shift = ((util_CMPSS_getInstanceFromBase(base)-10) * 2) + 1;
            SOC_xbarSelectOutputXBarInputSource(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, out, 0,0,0,0,0,0,0,0x1<<shift,0,0,0);
        }

        uint32_t status=SOC_xbarGetOutputXBarOutputSignalStatus(CSL_CONTROLSS_OUTPUTXBAR_U_BASE);
        //DebugP_log("\nOutputXbar %d SOC_xbarGetOutputXBarOutputSignalStatus returns 0x%08x", out, status);
        if(status != (0x1<<out))
        {
            //DebugP_log("\nerror");
            error++;
        }

        SOC_xbarSelectOutputXBarInputSource(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, out, 0,0,0,0,0,0,0,0,0,0,0);
    }


    CMPSS_configHighComparator(base, 0);
    CMPSS_clearFilterLatchHigh(base);
    CMPSS_disableModule(base);

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_ITR_0005(uint32_t base)
{
    // DAC: Extend clear signal with EPWMBLANK
    // Enables BLANK feature and select EPWM blank source to reset latched digital filter output
    // Comparator output does not trip during blank window

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0005", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    uint32_t pwmindex=0;

    for(pwmindex=0;pwmindex<32;pwmindex++)
    {
        //Enable and configure CMPSS
        CMPSS_enableModule(base);
        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async

        //Force output to 1 using inversion
        CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No

        //DebugP_log("\nCOMPCTL 0x%04x", HW_RD_REG16(base + CSL_CMPSSB_COMPCTL));
        //DebugP_log("\nCMPSS_getStatus returns 0x%04x", CMPSS_getStatus(base));

//
//
//
//        CMPSS_enableModule(base);
//        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
//        CMPSS_configOutputsHigh(base, CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER);    //TRIP: filter TRIPOUT: filter
//
//        CMPSS_configLowComparator(base, CMPSS_INSRC_PIN_INH);  //Source: Pin, Inversion: No, Async or with filter: No
//        CMPSS_configOutputsLow(base, CMPSS_TRIP_FILTER | CMPSS_TRIPOUT_FILTER);    //TRIP: filter TRIPOUT: filter
//
//
//        CMPSS_configFilterHigh(base, 0x3FF, 32, 31);
//        CMPSS_initFilterHigh(base);
//
//        CMPSS_configFilterLow(base, 0x3FF, 32, 31);
//        CMPSS_initFilterLow(base);
//



        CMPSS_configBlanking(base, (pwmindex+1));

        CMPSS_enableBlanking(base);
        HW_WR_REG16((base+CSL_CMPSSA_COMPSTSCLR), HW_RD_REG16(base+CSL_CMPSSA_COMPSTSCLR)|CSL_CMPSSA_COMPSTSCLR_HSYNCCLREN_MASK|CSL_CMPSSA_COMPSTSCLR_LSYNCCLREN_MASK);

        //Configure PWM
        util_EPWM_config(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000), 49);
        util_EPWM_config_blank(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000), 0xFFFF);

        uint32_t wait_for_status;
        uint16_t sts;
        for(wait_for_status=0;wait_for_status<49*100;wait_for_status++)
        {
            sts=CMPSS_getStatus(base);
        }
        (void)sts; /* Presently set but not used. Suppress warning */

        //Check if latched digital filter output is reset
        uint16_t status_log[100]={}, status_log_index;
        for(status_log_index=0;status_log_index<100;status_log_index++)
        {
            status_log[status_log_index]= CMPSS_getStatus(base);
        }
        for(status_log_index=0;status_log_index<100;status_log_index++)
        {
            ////DebugP_log("\nCMPSS_getStatus returns 0x%04x", status_log[status_log_index]);
        }

        if( (status_log[100-1] & (CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK | CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) == 0x0000 )
        {
            //DebugP_log("\nOk. Latched digital filter output is reset");
        }
        else
        {
            //DebugP_log("\nerror. Latched digital filter output is not reset");
            error++;
        }



        CMPSS_disableBlanking(base);

        util_EPWM_deConfig_blank(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000));

        util_EPWM_deInit();

        util_CMPSS_deInit(base);
    }

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_ITR_0006(uint32_t base)
{
    // DAC: Synchronization with EPWM
    // Configures source of DAC as the shadow register and loaded upon EPWMSYNCPER
    // Comparator output aligns with PWM cycle

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0006", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;
    uint32_t pwmindex=0;

    for(pwmindex=0;pwmindex<32;pwmindex++)
    {
        uint16_t dac_input = 2048;
        //DebugP_log("\ndac_input=0x%04x", dac_input);

        //Enable and configure CMPSS

        CMPSS_enableModule(base);
        CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
        CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async

        CMPSS_setDACValueHigh(base, dac_input);
        CMPSS_setDACValueLow(base, dac_input);

        CMPSS_configDAC(base, CMPSS_DACREF_VDDA | CMPSS_DACVAL_PWMSYNC |
                                         CMPSS_DACSRC_SHDW);

        //Ramp source is the source for DAC synchronization also?
        HW_WR_REG16(base + CSL_CMPSSA_COMPDACCTL, (HW_RD_REG16(base + CSL_CMPSSA_COMPDACCTL) &
                 ~(CSL_CMPSSA_COMPDACCTL_RAMPSOURCE_MASK)) | (pwmindex <<CSL_CMPSSA_COMPDACCTL_RAMPSOURCE_SHIFT));

        //Configure PWM
        util_EPWM_config(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000), 49);
        //EPWMSYNCPER generation
        //HRPCTL default value already configures PWMSYNCSELX as 000 and PWMSYNCSEL as 0. So PWMSYNCSEL is used for EPWMSYNCPER and CTR=PRD is used

        uint32_t wait_for_status;
        uint16_t sts;
        for(wait_for_status=0;wait_for_status<100;wait_for_status++)
        {
            sts=CMPSS_getStatus(base);
        }
        (void)sts; /* Presently set but not used. Suppress warning */

        //Check if DAC active value is updated

        uint16_t active_dac_value;
        active_dac_value = CMPSS_getDACValueHigh(base);

        //uint32_t tbctr=HW_RD_REG16(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000) + CSL_EPWM_TBCTR);
        //uint32_t tbctr2=HW_RD_REG16(CSL_CONTROLSS_G0_EPWM0_U_BASE + (pwmindex*0x1000) + CSL_EPWM_TBCTR);

        //DebugP_log("\n active_dac_value: 0x%04x, pwmindex=%d, TBCTR=0x%04x, 0x%04x", active_dac_value, pwmindex, tbctr, tbctr2);


        if(active_dac_value != (dac_input & CSL_CMPSSA_DACHVALA_DACVAL_MASK))
        {
            //DebugP_log("\n error: CMPSS_getDACValueHigh=0x%04x", active_dac_value);
            error++;
        }

        active_dac_value = CMPSS_getDACValueLow(base);
        if(active_dac_value != (dac_input & CSL_CMPSSA_DACLVALA_DACVAL_MASK))
        {
            //DebugP_log("\n error: CMPSS_getDACValueLow=0x%04x", active_dac_value);
            error++;
        }
        // Check if aligns with PWM cycle



        util_CMPSS_deInit(base);
    }

    util_EPWM_deInit();

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

int32_t AM263x_CMPSS_ITR_0007(uint32_t base)
{
    // DAC: Enable/disable Comparator DAC Output
    // Is this feature available in AM263? Routing Comparator DAC output to an external pin
    // DAC output voltage at external pin

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0007", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    CMPSS_configDAC(base, (0<<CSL_CMPSSA_COMPDACCTL_SELREF_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_SWLOADSEL_SHIFT) | (0<<CSL_CMPSSA_COMPDACCTL_DACSOURCE_SHIFT));
    CMPSS_setDACValueHigh(base, 2048);
    CMPSS_setDACValueLow(base, 2048);

    //Init
//    CMPSS_enableDACHighTest(base);
//    CMPSS_enableDACLowTest(base);
//    //DebugP_log("\nCMPSSx_TEST 0x%04x", HW_RD_REG16(base+CSL_CMPSSA_TEST));
//    CMPSS_configureCmpssCONFIG(base, CSL_CMPSSA_CONFIG_INT_TEST_TO_TESTANA_MASK);
//    //DebugP_log("\nCMPSSx_CONFIG 0x%04x", HW_RD_REG16(base+CSL_CMPSSA_CONFIG));
    //TBD: analog verification

    //Deinit
//    CMPSS_configureCmpssCONFIG(base, 0);
//    CMPSS_disableDACHighTest(base);
//    CMPSS_disableDACLowTest(base);

    util_CMPSS_deInit(base);

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}




int32_t AM263x_CMPSS_ITR_0008(uint32_t base)
{
    // RAMP: Synchronization with EPWM
    // Configures source of DAC as ramp generator and ramp source is EPWMSYNCPER
    // Ramp start aligns with PWM cycle

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_ITR_0008", util_CMPSS_getInstanceFromBase(base));

    return test_cmpss_ramp_pwm(base);
}


int32_t AM263x_CMPSS_STR_0001(uint32_t base)
{
    // DEL: Diode emulation mode with PWM
    // Enables diode emulation mode and select diode emulation logic EPWM source. Setup TRIPH and TRIPL signals to drive the EPWMA and EPWMB outputs directly
    // PWM output driven by CMPSS CTRIPH and CTRIPL upon trip

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_STR_0001", util_CMPSS_getInstanceFromBase(base));

   return test_cmpss_dac_swload(base, 1);
}

int32_t AM263x_CMPSS_STR_0002(uint32_t base)
{
    // CMPSS Async trip
    // CMPSS is configured to generate trip signals to trip the EPWM signals.
    // PWM gets tripped and outputs as high

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] AM263x_CMPSS_STR_0002", util_CMPSS_getInstanceFromBase(base));

    //MCUSDK-6 CMPSS: Example to generate asynchronous trip output

    if(enableLog) DebugP_log("\r\n\nCMPSS[%d] cmpss_ex1_asynch", util_CMPSS_getInstanceFromBase(base));

    uint32_t error=0;

    CMPSS_enableModule(base);
    CMPSS_configHighComparator(base, CMPSS_INSRC_PIN);  //Source: Pin, Inversion: No, Async or with filter: No
    CMPSS_configOutputsHigh(base, CMPSS_TRIP_ASYNC_COMP | CMPSS_TRIPOUT_ASYNC_COMP);    //TRIP: async TRIPOUT: async


    //
    // Set up ePWM0 to take CTRIPH as TRIP4 for its DC trip input
    //
    util_EPWM_config(CSL_CONTROLSS_G0_EPWM0_U_BASE, 60);
    util_EPWM_config_trip(CSL_CONTROLSS_G0_EPWM0_U_BASE);


    uint8_t out;
    for(out=0;out<30;out++)
    {
        SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, out, CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G0_SEL_MASK, CSL_CONTROLSS_PWMXBAR_PWMXBAR0_G1_SEL_MASK, 0, 0, 0, 0, 0, 0, 0);
    }


    //Force CTRIP output to 1 using inversion
    CMPSS_configHighComparator(base, CMPSS_INV_INVERTED | CMPSS_INSRC_PIN);  //Source: Pin, Inversion: Yes, Async or with filter: No

    //DebugP_log("\nCOMPCTL 0x%04x", HW_RD_REG16(base + CSL_CMPSSB_COMPCTL));
    //DebugP_log("\nCMPSS_getStatus returns 0x%04x", CMPSS_getStatus(base));

    //DebugP_log("\nSOC_xbarGetPWMXBarOutputSignalStatus returns 0x%08x", SOC_xbarGetPWMXBarOutputSignalStatus(CSL_CONTROLSS_PWMXBAR_U_BASE));

    //
    // Trip flag is set when CTRIP signal is asserted
    //Reads CSL_EPWM_TZFLG, Fields: 0-INT, 1-CBC, 2-OST, 3-DCAEVT1, 4-DCAEVT2, 5-DCBEVT1, 6-DCBEVT2
    //Field value: 1 - trip event has occurred
    //
    if((EPWM_getTripZoneFlagStatus(CSL_CONTROLSS_G0_EPWM0_U_BASE) &
        EPWM_TZ_FLAG_OST) != 0U)
    {
        //DebugP_log("\nTZ OST set");
    }
    else
    {
        //DebugP_log("\nTZ OST not set");
        error++;
    }

    for(out=0;out<30;out++)
    {
        SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, out, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }


    CMPSS_configHighComparator(base, 0);
    CMPSS_clearFilterLatchHigh(base);
    CMPSS_disableModule(base);

    util_CMPSS_deInit(base);

    util_EPWM_deInit();

    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

void util_set_sts(uint32_t base)
{
    //Generate non-zero comparator digital filter output (sts)
    CMPSS_enableModule(base);
    tester_command("gen dac voltage 1.000");                //Generate 1v at INH
    CMPSS_configHighComparator(base, CMPSS_INSRC_DAC);      //INH vs DACH
    CMPSS_configLowComparator(base, CMPSS_INSRC_PIN_INH);   //INH vs DACL
    CMPSS_setDACValueHigh(base, 0x100);
    CMPSS_setDACValueLow(base, 0x100);
}

void util_clr_sts(uint32_t base)
{
    //Generate zero comparator digital filter output (sts)
    tester_command("gen dac voltage 0.000");                //Generate 0v at INH
}

void util_load_ramp_regs(uint32_t base)
{
    //Set and clear DACSOURCE. DACSOURCE rising edge latches RAMPMAXREFS to RAMPMAXREFA
    HW_WR_REG16((base + CSL_CMPSSA_COMPDACCTL), (HW_RD_REG16(base + CSL_CMPSSA_COMPDACCTL) & 0xFFFE));
    CMPSS_configDAC(base,CMPSS_DACSRC_RAMP);
}

int32_t apiCheck(uint32_t base)
{
    int32_t error=0;
    uint8_t i;

    if(enableLog) DebugP_log("\r\n\r\nCMPSS[%d] API check", util_CMPSS_getInstanceFromBase(base));

    /* CMPSS_enableModule and CMPSS_disableModule API check */
    /* CMPSS_enableBlanking and CMPSS_disableBlanking API check */
    /* CMPSS_enableDEmode and CMPSS_disableDEmode API check */

    CMPSS_enableModule(base);
    if(0x8000U != (0x8000U & HW_RD_REG16(base + 0x0)))
    {
        error++;
        DebugP_log("\r\nCMPSS_enableModule API check error");
    }

    CMPSS_disableModule(base);
    if(0x0000U != (0x8000U & HW_RD_REG16(base + 0x0)))
    {
        error++;
        DebugP_log("\r\nCMPSS_disableModule API check error");
    }

    CMPSS_enableBlanking(base);
    if(0x1000U != (0x1000U & HW_RD_REG16(base + 0x08)))
    {
        error++;
        DebugP_log("\r\nCMPSS_enableBlanking API check error");
    }

    CMPSS_disableBlanking(base);
    if(0x0000U != (0x1000U & HW_RD_REG16(base + 0x08)))
    {
        error++;
        DebugP_log("\r\nCMPSS_disableBlanking API check error");
    }

    CMPSS_enableDEmode(base);
    if(0x0001U != (0x0001U & HW_RD_REG16(base + 0x0A)))
    {
        error++;
        DebugP_log("\r\nCMPSS_enableDEmode API check error");
    }

    CMPSS_disableDEmode(base);
    if(0x0000U != (0x0001U & HW_RD_REG16(base + 0x0A)))
    {
        error++;
        DebugP_log("\r\nCMPSS_disableDEmode API check error");
    }

    util_CMPSS_deInit(base);

    /* CMPSS_configHighComparator and CMPSS_configLowComparator API check */
    uint16_t test_vec_1[8][3] =
    {     //Possible inputs to API                                   //Expected value in register
        { CMPSS_INSRC_DAC,                                                   0x0000,    0x0000 },
        { CMPSS_INSRC_PIN,                                                   0x0100,    0x0001 },
        { CMPSS_INSRC_DAC | CMPSS_INV_INVERTED,                              0x0200,    0x0002 },
        { CMPSS_INSRC_PIN | CMPSS_INV_INVERTED,                              0x0300,    0x0003 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_DAC,                       0x4000,    0x0040 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN,                       0x4100,    0x0041 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_DAC | CMPSS_INV_INVERTED,  0x4200,    0x0042 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN | CMPSS_INV_INVERTED,  0x4300,    0x0043 }
    };

    for( i = 0; i < 8 ; i++ )
    {
        CMPSS_configHighComparator(base, test_vec_1[i][0]);
        if(test_vec_1[i][2] != (0x0043 & HW_RD_REG16(base + 0x0)))
        {
            error++;
            DebugP_log("\r\nCMPSS_configHighComparator API check error");
        }

    }

    uint16_t test_vec_1b[8][3] =
    {     //Possible inputs to API                                   //Expected value in register
        { CMPSS_INSRC_PIN_INL,                                                   0x0000,    0x0000 },
        { CMPSS_INSRC_PIN_INH,                                                   0x0100,    0x0001 },
        { CMPSS_INSRC_PIN_INL | CMPSS_INV_INVERTED,                              0x0200,    0x0002 },
        { CMPSS_INSRC_PIN_INH | CMPSS_INV_INVERTED,                              0x0300,    0x0003 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN_INL,                       0x4000,    0x0040 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN_INH,                       0x4100,    0x0041 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN_INL | CMPSS_INV_INVERTED,  0x4200,    0x0042 },
        { CMPSS_OR_ASYNC_OUT_W_FILT | CMPSS_INSRC_PIN_INH | CMPSS_INV_INVERTED,  0x4300,    0x0043 }
    };

    for( i = 0; i < 8 ; i++ )
    {

        CMPSS_configLowComparator(base, test_vec_1b[i][0]);
        if(test_vec_1b[i][1] != (0x4300 & HW_RD_REG16(base + 0x0)))
        {
            error++;
            DebugP_log("\r\nCMPSS_configLowComparator API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_configOutputsHigh and CMPSS_configOutputsLow API check */
    uint16_t test_vec_2[16][3] =
    {     //Possible inputs to API                             //Expected value in register
        { CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP,     0x0000,    0x0000 },
        { CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_SYNC_COMP,      0x0400,    0x0004 },
        { CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_FILTER,         0x0800,    0x0008 },
        { CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_LATCH,          0x0C00,    0x000C },
        { CMPSS_TRIPOUT_SYNC_COMP | CMPSS_TRIP_ASYNC_COMP,      0x1000,    0x0010 },
        { CMPSS_TRIPOUT_SYNC_COMP | CMPSS_TRIP_SYNC_COMP,       0x1400,    0x0014 },
        { CMPSS_TRIPOUT_SYNC_COMP | CMPSS_TRIP_FILTER,          0x1800,    0x0018 },
        { CMPSS_TRIPOUT_SYNC_COMP | CMPSS_TRIP_LATCH,           0x1C00,    0x001C },
        { CMPSS_TRIPOUT_FILTER | CMPSS_TRIP_ASYNC_COMP,         0x2000,    0x0020 },
        { CMPSS_TRIPOUT_FILTER | CMPSS_TRIP_SYNC_COMP,          0x2400,    0x0024 },
        { CMPSS_TRIPOUT_FILTER | CMPSS_TRIP_FILTER,             0x2800,    0x0028 },
        { CMPSS_TRIPOUT_FILTER | CMPSS_TRIP_LATCH,              0x2C00,    0x002C },
        { CMPSS_TRIPOUT_LATCH | CMPSS_TRIP_ASYNC_COMP,          0x3000,    0x0030 },
        { CMPSS_TRIPOUT_LATCH | CMPSS_TRIP_SYNC_COMP,           0x3400,    0x0034 },
        { CMPSS_TRIPOUT_LATCH | CMPSS_TRIP_FILTER,              0x3800,    0x0038 },
        { CMPSS_TRIPOUT_LATCH | CMPSS_TRIP_LATCH,               0x3C00,    0x003C }
    };

    for( i = 0; i < 16 ; i++ )
    {
        CMPSS_configOutputsHigh(base, test_vec_2[i][0]);
        if(test_vec_2[i][2] != (0x003C & HW_RD_REG16(base + 0x0)))
        {
            error++;
            DebugP_log("\r\nCMPSS_configOutputsHigh API check error");
        }

        CMPSS_configOutputsLow(base, test_vec_2[i][0]);
        if(test_vec_2[i][1] != (0x3C00 & HW_RD_REG16(base + 0x0)))
        {
            error++;
            DebugP_log("\r\nCMPSS_configOutputsLow API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_getStatus API check */

    util_set_sts(base);

    uint16_t sts=0;
    sts=CMPSS_getStatus(base);

    if( (sts&(CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) != (CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK))
    {
        error++;
        DebugP_log("\r\nCMPSS_getStatus API check error sts= 0x%04x", sts);
    }

    util_CMPSS_deInit(base);

    /* CMPSS_configDAC API check */
    uint16_t test_vec_2b[8][2] =
    {     //Possible inputs to API                             //Expected value in register
        { CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW,  0x0000 },
        { CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_RAMP,  0x0001 },
        { CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDAC | CMPSS_DACSRC_SHDW,  0x0020 },
        { CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDAC | CMPSS_DACSRC_RAMP,  0x0021 },
        { CMPSS_DACVAL_PWMSYNC | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW, 0x0080 },
        { CMPSS_DACVAL_PWMSYNC | CMPSS_DACREF_VDDA | CMPSS_DACSRC_RAMP, 0x0081 },
        { CMPSS_DACVAL_PWMSYNC | CMPSS_DACREF_VDAC | CMPSS_DACSRC_SHDW, 0x00A0 },
        { CMPSS_DACVAL_PWMSYNC | CMPSS_DACREF_VDAC | CMPSS_DACSRC_RAMP, 0x00A1 }
    };

    for( i = 0; i < 8 ; i++ )
    {
        CMPSS_configDAC(base, test_vec_2b[i][0]);
        if(test_vec_2b[i][1] != (0x00A1 & HW_RD_REG16(base + 0x08)))
        {
            error++;
            DebugP_log("\r\nCMPSS_configDAC API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_setDACValueHigh and CMPSS_getDACValueHigh API, CMPSS_setDACValueLow and CMPSS_getDACValueLow API check */
    /* CMPSS_configHighDACShadowValue2 and CMPSS_configLowDACShadowValue2 API check */

    uint16_t test_vec_3;

    for( test_vec_3 = 0; test_vec_3 < 0xFFF ; test_vec_3++ )
    {
        CMPSS_setDACValueHigh(base, test_vec_3);
        CMPSS_setDACValueLow(base, test_vec_3);

        if( (HW_RD_REG16(base + 0x0C) != test_vec_3) || (HW_RD_REG16(base + 0x24) != test_vec_3) )
        {
            error++;
            DebugP_log("\r\nCMPSS_setDACValueHigh and CMPSS_setDACValueLow API check error");
        }

        if( (CMPSS_getDACValueHigh(base) != test_vec_3) || (CMPSS_getDACValueLow(base) != test_vec_3) )
        {
            error++;
            DebugP_log("\r\nCMPSS_getDACValueHigh and CMPSS_getDACValueLow API check error");
        }

        CMPSS_configHighDACShadowValue2(base, test_vec_3);
        CMPSS_configLowDACShadowValue2(base, test_vec_3);

        if( (HW_RD_REG16(base + 0x38) != test_vec_3) || (HW_RD_REG16(base + 0x3A) != test_vec_3) )
        {
            error++;
            DebugP_log("\r\nCMPSS_configHighDACShadowValue2 and CMPSS_configLowDACShadowValue2 API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_initFilterHigh and CMPSS_initFilterLow API check */
        //No direct way to check filter samples
        //Check latched digital filter status

    util_set_sts(base);

    CMPSS_initFilterHigh(base);
    CMPSS_initFilterLow(base);

    sts=0;
    sts=CMPSS_getStatus(base);

    if( (sts&(CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) != (CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK))
    {
        error++;
        DebugP_log("\r\nCMPSS_initFilterHigh, CMPSS_initFilterLow API check error");
    }

    util_CMPSS_deInit(base);

    /* CMPSS_clearFilterLatchHigh and CMPSS_clearFilterLatchLow API check */

    util_set_sts(base);
    util_clr_sts(base);

    CMPSS_initFilterHigh(base);
    CMPSS_initFilterLow(base);

    sts=0;
    sts=CMPSS_getStatus(base);

    if( (sts&(CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) != (CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK))
    {
        error++;
        DebugP_log("\r\nFailed to setup test");
    }

    CMPSS_clearFilterLatchHigh(base);
    CMPSS_clearFilterLatchLow(base);

    sts=CMPSS_getStatus(base);

    if( (sts&(CSL_CMPSSA_COMPSTS_COMPHLATCH_MASK|CSL_CMPSSA_COMPSTS_COMPLLATCH_MASK)) != 0x0000)
    {
        error++;
        DebugP_log("\r\nCMPSS_clearFilterLatchHigh, CMPSS_clearFilterLatchLow API check error");
    }

    util_CMPSS_deInit(base);

    /* CMPSS_setMaxRampValue and CMPSS_getMaxRampValue API check */

    CMPSS_enableModule(base);

    uint32_t test_vec_4 = 0;
    uint16_t inp;
    for(test_vec_4 = 0; test_vec_4 < 0x10000 ; test_vec_4++)
    {
        inp = (uint16_t)test_vec_4;

        CMPSS_setMaxRampValue(base, inp);

        util_load_ramp_regs(base);

        if( HW_RD_REG16(base + 0x14) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_setMaxRampValue API check error");
        }

        if( CMPSS_getMaxRampValue(base) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_getMaxRampValue API check error 0x%08x", test_vec_4);
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_setRampDecValue and CMPSS_getRampDecValue API check */

    CMPSS_enableModule(base);

    for(test_vec_4 = 0; test_vec_4 < 0x10000 ; test_vec_4++)
    {
        uint16_t inp = (uint16_t)test_vec_4;

        CMPSS_setRampDecValue(base, inp);

        util_load_ramp_regs(base);

        if( HW_RD_REG16(base + 0x1C) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_setRampDecValue API check error");
        }

        if( CMPSS_getRampDecValue(base) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_getRampDecValue API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_setRampDelayValue and CMPSS_getRampDelayValue API check */

    CMPSS_enableModule(base);

    for(test_vec_4 = 0; test_vec_4 < 0x2000 ; test_vec_4++)
    {
        uint16_t inp = (uint16_t)test_vec_4;

        CMPSS_setRampDelayValue(base, inp);

        util_load_ramp_regs(base);

        if( HW_RD_REG16(base + 0x2A) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_setRampDelayValuee API check error");
        }

        if( CMPSS_getRampDelayValue(base) != inp )
        {
            error++;
            DebugP_log("\r\nCMPSS_getRampDelayValue API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_configBlanking API check */

    uint16_t test_vec_4b[2][3] =
    {   //Possible inputs to API    //Expected value in register
        {1,                         0x0000, 0x0000},
        {32,                        0x0F00, 0x0100}
    };

    for( i = 0; i < 2 ; i++ )
    {
        CMPSS_configBlanking(base, test_vec_4b[i][0]);

        if( ( (HW_RD_REG16(base + 0x08) & 0x0F00) != test_vec_4b[i][1] ) || ((HW_RD_REG16(base + 0x0A) & 0x0100) != test_vec_4b[i][2]))
        {
            error++;
            DebugP_log("\r\nCMPSS_configBlanking API check error");
        }

    }

    util_CMPSS_deInit(base);

    /* CMPSS_selectDEACTIVESource API check */

    uint8_t test_vec_4c;

    for(test_vec_4c = 0 ; test_vec_4c < 32 ; test_vec_4c++)
    {
        CMPSS_selectDEACTIVESource(base, test_vec_4c);

        if( (HW_RD_REG16(base + 0x0A) & 0x003E) != (test_vec_4c<<1) )
        {
            error++;
            DebugP_log("\r\nCMPSS_selectDEACTIVESource API check error");
        }
    }
    util_CMPSS_deInit(base);


    /* CMPSS_setComparatorHighHysteresis and CMPSS_setComparatorLowHysteresis API check */

    uint16_t test_vec_4d[8][3] =
    {   //Possible inputs to API    //Expected value in register
        {0,                        0x0000, 0x0000},
        {1,                        0x0001, 0x0010},
        {2,                        0x0002, 0x0020},
        {3,                        0x0003, 0x0030},
        {4,                        0x0004, 0x0040},
        {5,                        0x0005, 0x0050},
        {6,                        0x0006, 0x0060},
        {7,                        0x0007, 0x0070}
    };

    for( i = 0; i < 8 ; i++ )
    {
        CMPSS_setComparatorHighHysteresis(base, test_vec_4d[i][0]);
        CMPSS_setComparatorLowHysteresis(base, test_vec_4d[i][0]);

        if( ( (HW_RD_REG16(base + 0x3C) & 0x000F) != test_vec_4d[i][1] ) || ((HW_RD_REG16(base + 0x3C) & 0x00F0) != test_vec_4d[i][2]))
        {
            error++;
            DebugP_log("\r\nCMPSS_setComparatorHighHysteresis and CMPSS_setComparatorLowHysteresis API check error, 0x%04x", HW_RD_REG16(base + 0x3C));
        }

    }

    util_CMPSS_deInit(base);

    /* CMPSS_configFilterHigh and CMPSS_configFilterLow API check */

    uint16_t test_vec_4e[8][5] =
    {   //Possible inputs to API                    //Expected value in register
    //samplePrescale, sampleWindow,  threshold
        {0,             1,          1,                         0x0000 | 0x0000, 0},
        {1,             2,          2,                         0x0010 | 0x0200, 1},
        {2,             3,          2,                         0x0020 | 0x0200, 2},
        {10,            5,          3,                         0x0040 | 0x0400, 10},
        {100,           10,         7,                         0x0090 | 0x0C00, 100},
        {1023,          20,         11,                        0x0130 | 0x1400, 1023},
        {1023,          31,         20,                        0x01E0 | 0x2600, 1023},
        {1023,          32,         17,                        0x01F0 | 0x2000, 1023}
    };

    for( i = 0; i < 8 ; i++ )
    {
        CMPSS_configFilterHigh(base, test_vec_4e[i][0], test_vec_4e[i][1], test_vec_4e[i][2]);

        if( ( (HW_RD_REG16(base + 0x30) & 0x3FF0) != test_vec_4e[i][3] ) || ((HW_RD_REG16(base + 0x32) & 0xFFFF) != test_vec_4e[i][4]))
        {
            error++;
            DebugP_log("\r\nCMPSS_configFilterHigh API check error %u 0x%04x 0x%04x", i, HW_RD_REG16(base + 0x2C), HW_RD_REG16(base + 0x2E));
        }

        CMPSS_configFilterLow(base, test_vec_4e[i][0], test_vec_4e[i][1], test_vec_4e[i][2]);

        if( ( (HW_RD_REG16(base + 0x2C) & 0x3FF0) != test_vec_4e[i][3] ) || ((HW_RD_REG16(base + 0x2E) & 0xFFFF) != test_vec_4e[i][4]))
        {
            error++;
            DebugP_log("\r\nCMPSS_configFilterLow API check error");
        }
    }

    util_CMPSS_deInit(base);

    /* CMPSS_configLatchOnPWMSYNC API check */

    uint16_t test_vec_4f[4][3] =
    {   //Possible inputs to API    //Expected value in register
        {0,             0,          0x0000},
        {0,             1,          0x0400},
        {1,             0,          0x0004},
        {1,             1,          0x0404}
    };

    for( i = 0; i < 4 ; i++ )
    {
        CMPSS_configLatchOnPWMSYNC(base, test_vec_4f[i][0], test_vec_4f[i][1]);

        if(  (HW_RD_REG16(base + 0x06) & 0x0404) != test_vec_4f[i][2] )
        {
            error++;
            DebugP_log("\r\nCMPSS_configLatchOnPWMSYNC API check error %u 0x%04x", i, HW_RD_REG16(base + 0x06));
        }

    }

    util_CMPSS_deInit(base);

    /* CMPSS_configRamp API check */
    uint16_t test_vec_5[3][11] =
    {   //Possible inputs to API                                            //Expected value in register
        //maxRampVal decrementVal delayVal pwmSyncSrc useRampValShdw
        {0xFFF1,        0xFFFE,        0x1FFF,      1,          0,                  0x0000, 0x0000, 0x0000, 0xFFF1, 0xFFFE, 0x1FFF},
        {0xFFF1,        0xFFFE,        0x1FFF,      2,          0,                  0x0002, 0x0000, 0x0000, 0xFFF1, 0xFFFE, 0x1FFF},
        {0xFFF1,        0xFFFE,        0x1FFF,      32,         1,                  0x001E, 0x0400, 0x0040, 0xFFF1, 0xFFFE, 0x1FFF}
    };

    for( i = 0; i < 3 ; i++ )
    {
        CMPSS_configRamp(base, test_vec_5[i][0], test_vec_5[i][1], test_vec_5[i][2], test_vec_5[i][3], test_vec_5[i][4]);

        if(
            ((HW_RD_REG16(base + 0x08) & 0x001E) != test_vec_5[i][5]) ||
            ((HW_RD_REG16(base + 0x0A) & 0x0400) != test_vec_5[i][6]) ||
            ((HW_RD_REG16(base + 0x08) & 0x0040) != test_vec_5[i][7]) ||
            ((HW_RD_REG16(base + 0x14) & 0xFFFF) != test_vec_5[i][8]) ||
            ((HW_RD_REG16(base + 0x1C) & 0xFFFF) != test_vec_5[i][9]) ||
            ((HW_RD_REG16(base + 0x2A) & 0x1FFF) != test_vec_5[i][10])
            )
        {
            error++;
            DebugP_log("\r\nCMPSS_configRamp API check error");
        }

    }

    util_CMPSS_deInit(base);



    //DebugP_log("\r\n\nCMPSS[%d] apiCheck", util_CMPSS_getInstanceFromBase(base));

    //Check COMPCTL write for all CMPSSA and CMPSSB
    //DebugP_log("\r\nSetting COMPCTL");
    HW_WR_REG16(base + CSL_CMPSSA_COMPCTL, 0xFFFF);
    if(0xFF7F != HW_RD_REG16(base + CSL_CMPSSA_COMPCTL))
    {
        error++;
    }
    //DebugP_log("\nCOMPCTL = 0x%08x", HW_RD_REG16(base + CSL_CMPSSA_COMPCTL));
    //DebugP_log("\nCOMPSTS = 0x%08x", HW_RD_REG16(base + CSL_CMPSSA_COMPSTS));

    //DebugP_log("\nClearing COMPCTL");
    HW_WR_REG16(base + CSL_CMPSSA_COMPCTL, 0x0000);
    //DebugP_log("\nCOMPCTL = 0x%08x", HW_RD_REG16(base + CSL_CMPSSA_COMPCTL));

    //DebugP_log("\nSetting COMPSTSCLR");
    HW_WR_REG16(base + CSL_CMPSSA_COMPSTSCLR, CSL_CMPSSA_COMPSTSCLR_HLATCHCLR_MASK | CSL_CMPSSA_COMPSTSCLR_LLATCHCLR_MASK);
    //DebugP_log("\nCOMPSTS = 0x%08x", HW_RD_REG16(base + CSL_CMPSSA_COMPSTS));

    util_CMPSS_deInit(base);
    //util_CMPSS_dump(base);


    if(error==0)
    {
        if(enableLog) DebugP_log("\r\nPass");

        return 0;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nFail");

        return 1;
    }
}

uint8_t isTestSupported[20][19] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //Supported tests for CMPSSA0
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //Supported tests for CMPSSA9

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //Supported tests for CMPSSB0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}   //Supported tests for CMPSSB9
};

int32_t test_cmpss_cases(uint8_t in)
{
    int32_t failcount=0;

    uint32_t cmpss_offset=0, cmpss_a_b_offset=0;
    uint32_t base;
    for(cmpss_a_b_offset=0;cmpss_a_b_offset<=0x20000; cmpss_a_b_offset=cmpss_a_b_offset+0x20000)
    {
        for(cmpss_offset=0;cmpss_offset<=0x9000; cmpss_offset=cmpss_offset+0x1000)
        {

            base = CSL_CONTROLSS_CMPSSA0_U_BASE + cmpss_offset + cmpss_a_b_offset;

            if(enableLog) DebugP_log("\r\n\nbase=0x%08x, instance=%d", base, util_CMPSS_getInstanceFromBase(base));

            if(isTestSupported[util_CMPSS_getInstanceFromBase(base)][in-1])
            {
                failcount += apiCheck(base);

                switch(in)
                {
                    case 1:
                        failcount += AM263x_CMPSS_BTR_0001(base);
                        break;
                    case 2:
                        failcount += AM263x_CMPSS_BTR_0002(base);
                        break;
                    case 3:
                        failcount += AM263x_CMPSS_BTR_0003(base);
                        break;
                    case 4:
                        failcount += AM263x_CMPSS_BTR_0004(base);
                        break;
                    case 5:
                        failcount += AM263x_CMPSS_BTR_0005(base);
                        break;
                    case 6:
                        failcount += AM263x_CMPSS_BTR_0006(base);
                        break;
                    case 7:
                        failcount += AM263x_CMPSS_BTR_0007(base);      //ramp sts is ok, check if active dac value is loaded from sts
                        break;
                    case 8:
                        failcount += AM263x_CMPSS_BTR_0008(base);
                        break;
                    case 9:
                        failcount += AM263x_CMPSS_BTR_0009(base);
                        break;
                    case 10:
                        failcount += AM263x_CMPSS_ITR_0001(base);
                        break;
                    case 11:
                        failcount += AM263x_CMPSS_ITR_0002(base);
                        break;
                    case 12:
                        failcount += AM263x_CMPSS_ITR_0003(base);
                        break;
                    case 13:
                        failcount += AM263x_CMPSS_ITR_0004(base);
                        break;
                    case 14:
                        failcount += AM263x_CMPSS_ITR_0005(base);
                        break;
                    case 15:
                        failcount += AM263x_CMPSS_ITR_0006(base);
                        break;
                    case 16:
                        failcount += AM263x_CMPSS_ITR_0007(base);
                        break;
                    case 17:
                        failcount += AM263x_CMPSS_ITR_0008(base);
                        break;
                    case 18:
                        failcount += AM263x_CMPSS_STR_0001(base);
                        break;
                    case 19:
                        failcount += AM263x_CMPSS_STR_0002(base);
                        break;
                }
                if(enableLog) DebugP_log("\r\nfailcount=%d", failcount);
            }
            else
            {
                if(enableLog) DebugP_log("\r\nCMPSS %u, Test %u not supported", util_CMPSS_getInstanceFromBase(base), in-1);
            }
        }
    }


    if(failcount!=0)
    {
        if(enableLog) DebugP_log("\r\nFAIL %d", failcount);
        return 1;
    }
    else
    {
        if(enableLog) DebugP_log("\r\nPass");
        return 0;
    }

}


