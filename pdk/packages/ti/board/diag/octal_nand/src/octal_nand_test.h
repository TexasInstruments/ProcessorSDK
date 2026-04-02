/*
 * Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   octal_nand_test.h
 *
 *  \brief  Octal NAND diagnostic test header file.
 *
 */

#ifndef _OCTAL_NAND_TEST_H_
#define _OCTAL_NAND_TEST_H_

#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/board/src/flash/include/board_flash.h>
#include <ti/board/src/flash/nand/ospi/nand_ospi.h>
#include "board.h"
#include "board_cfg.h"
#include "diag_common_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_OCTAL_NAND_INSTANCE                          (0U)  /* Need to move this macro to board_cfg.h */

/* Hyper Flash verification offsets */
#define BOARD_DIAG_OCTAL_NAND_FIRST_VERIFY_ADDR            (0x0U)
#define BOARD_DIAG_OCTAL_NAND_LAST_VERIFY_ADDR             (0x3FFFC00U)
#define BOARD_DIAG_OCTAL_NAND_TEST_BYTES                   (NAND_PAGE_SIZE)

#define BOARD_DIAG_OSPI_NAND_BUS_SEL_PIN                   (0U)

/**
 * \brief   The function performs the Hyperbus Diagnostic test.
 *
 * \return  int8_t  
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_octalNandTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OCTAL_NAND_TEST_H_ */
