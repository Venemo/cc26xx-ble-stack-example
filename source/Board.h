/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#include <ti/drivers/PIN.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const PIN_Config BoardGpioInitTable[];

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
///* Generic SPI instance identifiers */
//#define Board_SPI0                  CC2650DK_7ID_SPI0
//#define Board_SPI1                  CC2650DK_7ID_SPI1
///* Generic UART instance identifiers */
//#define Board_UART                  CC2650DK_7ID_UART0
/* Generic Crypto instance identifiers */
#define Board_CRYPTO                CC2650DK_7ID_CRYPTO0
/* Generic GPTimer instance identifiers */
#define Board_GPTIMER0A             CC2650DK_7ID_GPTIMER0A
#define Board_GPTIMER0B             CC2650DK_7ID_GPTIMER0B
#define Board_GPTIMER1A             CC2650DK_7ID_GPTIMER1A
#define Board_GPTIMER1B             CC2650DK_7ID_GPTIMER1B
#define Board_GPTIMER2A             CC2650DK_7ID_GPTIMER2A
#define Board_GPTIMER2B             CC2650DK_7ID_GPTIMER2B
#define Board_GPTIMER3A             CC2650DK_7ID_GPTIMER3A
#define Board_GPTIMER3B             CC2650DK_7ID_GPTIMER3B
///* Generic PWM instance identifiers */
//#define Board_PWM0                  CC2650DK_7ID_PWM0
//#define Board_PWM1                  CC2650DK_7ID_PWM1
//#define Board_PWM2                  CC2650DK_7ID_PWM2
//#define Board_PWM3                  CC2650DK_7ID_PWM3
//#define Board_PWM4                  CC2650DK_7ID_PWM4
//#define Board_PWM5                  CC2650DK_7ID_PWM5
//#define Board_PWM6                  CC2650DK_7ID_PWM6
//#define Board_PWM7                  CC2650DK_7ID_PWM7
/* Generic TRNG instance identiifer */
#define Board_TRNG                  CC2650DK_7ID_TRNG0

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC2650DK_7ID_CryptoName
 *  @brief  Enum of Crypto names on the CC2650 dev board
 */
typedef enum CC2650DK_7ID_CryptoName {
    CC2650DK_7ID_CRYPTO0 = 0,
    CC2650DK_7ID_CRYPTOCOUNT
} CC2650DK_7ID_CryptoName;

///*!
// *  @def    CC2650DK_7ID_SPIName
// *  @brief  Enum of SPI names on the CC2650 dev board
// */
//typedef enum CC2650DK_7ID_SPIName {
//    CC2650DK_7ID_SPI0 = 0,
//    CC2650DK_7ID_SPI1,
//    CC2650DK_7ID_SPICOUNT
//} CC2650DK_7ID_SPIName;

/*!
 *  @def    CC2650DK_7ID_TRNGName
 *  @brief  Enum of TRNG names on the CC2650 dev board
 */
typedef enum CC2650DK_7ID_TRNGName {
    CC2650DK_7ID_TRNG0 = 0,
    CC2650DK_7ID_TRNGCOUNT
} CC2650DK_7ID_TRNGName;

///*!
// *  @def    CC2650DK_7ID_UARTName
// *  @brief  Enum of UARTs on the CC2650 dev board
// */
//typedef enum CC2650DK_7ID_UARTName {
//    CC2650DK_7ID_UART0 = 0,
//    CC2650DK_7ID_UARTCOUNT
//} CC2650DK_7ID_UARTName;

/*!
 *  @def    CC2650DK_7ID_UdmaName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2650DK_7ID_UdmaName {
    CC2650DK_7ID_UDMA0 = 0,
    CC2650DK_7ID_UDMACOUNT
} CC2650DK_7ID_UdmaName;

/*!
 *  @def    CC2650DK_7ID_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC2650DK_7ID_GPTimerName
{
    CC2650DK_7ID_GPTIMER0A = 0,
    CC2650DK_7ID_GPTIMER0B,
    CC2650DK_7ID_GPTIMER1A,
    CC2650DK_7ID_GPTIMER1B,
    CC2650DK_7ID_GPTIMER2A,
    CC2650DK_7ID_GPTIMER2B,
    CC2650DK_7ID_GPTIMER3A,
    CC2650DK_7ID_GPTIMER3B,
    CC2650DK_7ID_GPTIMERPARTSCOUNT
} CC2650DK_7ID_GPTimerName;

/*!
 *  @def    CC2650DK_7ID_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC2650DK_7ID_GPTimers
{
    CC2650DK_7ID_GPTIMER0 = 0,
    CC2650DK_7ID_GPTIMER1,
    CC2650DK_7ID_GPTIMER2,
    CC2650DK_7ID_GPTIMER3,
    CC2650DK_7ID_GPTIMERCOUNT
} CC2650DK_7ID_GPTimers;

///*!
// *  @def    CC2650DK_7ID_PWM
// *  @brief  Enum of PWM outputs on the board
// */
//typedef enum CC2650DK_7ID_PWM
//{
//    CC2650DK_7ID_PWM0 = 0,
//    CC2650DK_7ID_PWM1,
//    CC2650DK_7ID_PWM2,
//    CC2650DK_7ID_PWM3,
//    CC2650DK_7ID_PWM4,
//    CC2650DK_7ID_PWM5,
//    CC2650DK_7ID_PWM6,
//    CC2650DK_7ID_PWM7,
//    CC2650DK_7ID_PWMCOUNT
//} CC2650DK_7ID_PWM;

///*!
// *  @def    CC2650DK_7ID_ADCBufName
// *  @brief  Enum of ADCBufs
// */
//typedef enum CC2650DK_7ID_ADCBufName {
//    CC2650DK_7ID_ADCBuf0 = 0,
//    CC2650DK_7ID_ADCBufCOUNT
//} CC2650DK_7ID_ADCBufName;

///*!
// *  @def    CC2650DK_7ID_ADCName
// *  @brief  Enum of ADCs
// */
//typedef enum CC2650DK_7ID_ADCName {
//    CC2650DK_7ID_ADCALS = 0,
//    CC2650DK_7ID_ADCDCOUPL,
//    CC2650DK_7ID_ADCVSS,
//    CC2650DK_7ID_ADCVDDS,
//    CC2650DK_7ID_ADCCOUNT
//} CC2650DK_7ID_ADCName;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
