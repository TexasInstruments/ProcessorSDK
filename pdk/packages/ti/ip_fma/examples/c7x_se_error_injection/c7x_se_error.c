/*
 *  Copyright (C) 2026 Texas Instruments Incorporated
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
 *  \file c7x_se_error.c
 *
 *  \brief Streaming Engine error injection functions
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "c7x_se_error.h"

#include <c7x.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* optimization has been diabled for this function for injecting exception */
#pragma FUNCTION_OPTIONS(SE_OpeningError_Injection,"--opt_level=off")
void SE_OpeningError_Injection()
{
	UART_printf("Beginning the Streamming Engine Opening Error Injection Test.\n");
	
	/* Initialize Dummy Arrays to injection Streaming Engine Error */
    int16_t arraySize = 128;
    int16_t ArrayA[arraySize], ArrayB[arraySize];

	int16_t i;
    for(i = 0; i < arraySize; i++)
    {
        ArrayA[i] = 0;
        ArrayB[i] = i;
    }

    /* Initialize the Streaming Engine Configuration */
    __SE_TEMPLATE_v1 saParam = __gen_SE_TEMPLATE_v1();
	saParam.DIMFMT = __SE_DIMFMT_3D;
	/* In this changed the element type to __SE_ELETYPE_64BIT from __SE_ELETYPE_32BIT to inject the error */
	saParam.ELETYPE = __SE_ELETYPE_64BIT;
	saParam.VECLEN = __SE_VECLEN_16ELEMS;
	
	saParam.ICNT0 = arraySize;
	saParam.ICNT1 = 1;
	saParam.ICNT2 = 1;
	saParam.DIM1 = 0;
	saParam.DIM2 = 0;

    /* Open an instance to the  Streaming Engines*/
	__SE0_OPEN((int16_t *)ArrayA, saParam);
	__SE1_OPEN((int16_t *)ArrayB ,saParam);

    for(i = 0; i < arraySize; i++)
	{
        short32 vdata;
        /* Read and Advance the Streaming Engine Instance */
        vdata = __SE0ADV(short32) + __SE1ADV(short32);
	}

	__SE0_CLOSE();
	__SE1_CLOSE();
}


/* optimization has been diabled for this function for injecting exception */
#pragma FUNCTION_OPTIONS(SE_SavingError_Injection,"--opt_level=off")
void SE_SavingError_Injection()
{
	UART_printf("Beginning the Streamming Engine Saving Error Injection Test.\n");

    /* Initialize a Dummy Array */
    int16_t arraySize = 128;
	int16_t ArrayA[arraySize];

	int16_t i;
    for(i = 0; i < arraySize; i++)
    {
        ArrayA[i] = i;
    }

    /* Initialize the Streaming Engine Configuration */
    __SE_TEMPLATE_v1 seParam = __gen_SE_TEMPLATE_v1();
	seParam.DIMFMT = __SE_DIMFMT_3D;
	/* In this changed the element type to __SE_ELETYPE_64BIT from __SE_ELETYPE_32BIT to inject the error */
	seParam.ELETYPE = __SE_ELETYPE_64BIT;
    seParam.VECLEN = __SE_VECLEN_16ELEMS;
	
	seParam.ICNT0 = arraySize;
	seParam.ICNT1 = 1;
	seParam.ICNT2 = 1;
	seParam.DIM1 = 0;
	seParam.DIM2 = 0;

    __SE0_OPEN((int16_t *)&ArrayA[0], seParam);

    UART_printf("Starting to Save Array");

	for(i = 0; i < arraySize; i+=32)
	{
        UART_printf("%d\n", i);
	    short32 vdata = __SE0ADV(short32) ;

        __SE0_SAVE(1);
	}

	__SE0_CLOSE();
}

/* optimization has been diabled for this function for injecting exception */
#pragma FUNCTION_OPTIONS(SE_RestoringError_Injection,"--opt_level=off")
void SE_RestoringError_Injection()
{
	UART_printf("Beginning the Streamming Engine Restoring Error Injection Test.\n");

	/* Initialize Dummy Arrays to injection Streaming Engine Error */
    int16_t arraySize = 128;
    int16_t ArrayA[arraySize], ArrayB[arraySize];

	int16_t i;
    for(i = 0; i < arraySize; i++)
    {
        ArrayA[i] = 0;
        ArrayB[i] = i;
    }

    /* Initialize the Streaming Engine Configuration */
    __SE_TEMPLATE_v1 saParam = __gen_SE_TEMPLATE_v1();
	saParam.DIMFMT = __SE_DIMFMT_3D;
	/* In this changed the element type to __SE_ELETYPE_64BIT from __SE_ELETYPE_32BIT to inject the error */
	saParam.ELETYPE = __SE_ELETYPE_64BIT;
	saParam.VECLEN = __SE_VECLEN_16ELEMS;
	
	saParam.ICNT0 = arraySize;
	saParam.ICNT1 = 1;
	saParam.ICNT2 = 1;
	saParam.DIM1 = 0;
	saParam.DIM2 = 0;

	__SE0_OPEN((int16_t *)ArrayA, saParam);
	__SE1_OPEN((int16_t *)ArrayB ,saParam);
	
	/* Restore call while stream is active will trigger an exception */
    __SE0_RESTORE(0,0);
	__SE0_CLOSE();
	__SE1_CLOSE();
}

/* optimization has been diabled for this function for injecting exception */
#pragma FUNCTION_OPTIONS(SE_ResourceAccessError_Injection,"--opt_level=off")
void SE_ResourceAccessError_Injection()
{
	UART_printf("Beginning the Resource Access Error Injection Test.\n");

	/* Streaming engine functional failure */
	__SE_TEMPLATE_v1 saParam = __gen_SE_TEMPLATE_v1();
	saParam.DIMFMT = __SE_DIMFMT_3D;
	/* In this changed the element type to __SE_ELETYPE_64BIT from __SE_ELETYPE_32BIT to inject the error */
	saParam.ELETYPE = __SE_ELETYPE_64BIT; 
	saParam.VECLEN = __SE_VECLEN_16ELEMS;

	saParam.ICNT0 = 5;
	saParam.ICNT1 = 1;
	saParam.ICNT2 = 1;
	saParam.DIM1 = 0;
	saParam.DIM2 = 0;
	int array[100] = {0};
	__SE0_OPEN((void*)array,saParam);
	__SE0_CLOSE();

	/* Access after closing stream should generate exception*/
	int data = __SE0ADV(int);
}

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
