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

/*
 *  ====================== CC2650DK_7ID.c =============================================
 *  This file is responsible for setting up the board specific items for the
 *  SRF06EB with the CC2650EM_7ID board.
 */


/*
 *  ====================== Includes ============================================
 */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC26XX.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/ioc.h>
#include <driverlib/udma.h>

#include "Board.h"

/*
 *  ========================= IO driver initialization =========================
 *  From main, PIN_init(BoardGpioInitTable) should be called to setup safe
 *  settings for this board.
 *  When a pin is allocated and then de-allocated, it will revert to the state
 *  configured in this table.
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(BoardGpioInitTable, ".const:BoardGpioInitTable")
#pragma DATA_SECTION(PINCC26XX_hwAttrs, ".const:PINCC26XX_hwAttrs")
#endif

const PIN_Config BoardGpioInitTable[] = {

    Board_DK_LED1    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off               */
    Board_DK_LED2    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off               */
    Board_DK_LED3    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off               */
    Board_DK_LED4    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW   | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off               */
    Board_KEY_SELECT | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                             /* Button is active low            */
    Board_KEY_UP     | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                             /* Button is active low            */
    Board_KEY_DOWN   | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                             /* Button is active low            */
    Board_KEY_LEFT   | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                             /* Button is active low            */
    Board_KEY_RIGHT  | PIN_INPUT_EN  | PIN_PULLUP | PIN_HYSTERESIS,                             /* Button is active low            */
    Board_3V3_EN     | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW    | PIN_PUSHPULL,                     /* 3V3 domain off initially        */
    Board_LCD_MODE   | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* LCD pin high initially          */
    Board_LCD_RST    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* LCD pin high initially          */
    Board_LCD_CSN    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* LCD CSn deasserted initially    */
    Board_ALS_PWR    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW    | PIN_PUSHPULL,                     /* ALS power off initially         */
    Board_ACC_PWR    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW    | PIN_PUSHPULL,                     /* ACC power off initially         */
    Board_ACC_CSN    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* ACC CSn deasserted initially    */
    Board_SDCARD_CSN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* SDCARD CSn deasserted initially */
    Board_UART_TX    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH   | PIN_PUSHPULL,                     /* UART TX pin at inactive level   */
    PIN_TERMINATE                                                                               /* Terminate list                  */
};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};
/*============================================================================*/

/*
 *  ============================= Power begin ===================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PowerCC26XX_config, ".const:PowerCC26XX_config")
#endif
const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = TRUE,
    .calibrateRCOSC_LF  = TRUE,
    .calibrateRCOSC_HF  = TRUE,
};
/*
 *  ============================= Power end ===================================
 */

/*
 *  ============================= UART begin ===================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartCC26XXHWAttrs, ".const:uartCC26XXHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

/* UART objects */
UARTCC26XX_Object uartCC26XXObjects[CC2650DK_7ID_UARTCOUNT];

/* UART hardware parameter structure, also used to assign UART pins */
const UARTCC26XX_HWAttrsV1 uartCC26XXHWAttrs[CC2650DK_7ID_UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = Board_UART_TX,
        .rxPin          = Board_UART_RX,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED
    }
};

/* UART configuration structure */
const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[0],
        .hwAttrs     = &uartCC26XXHWAttrs[0]
    },
    {NULL, NULL, NULL}
};
/*
 *  ============================= UART end =====================================
 */

/*
 *  ============================= UDMA begin ===================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UDMACC26XX_config, ".const:UDMACC26XX_config")
#pragma DATA_SECTION(udmaHWAttrs, ".const:udmaHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/dma/UDMACC26XX.h>

/* UDMA objects */
UDMACC26XX_Object udmaObjects[CC2650DK_7ID_UDMACOUNT];

/* UDMA configuration structure */
const UDMACC26XX_HWAttrs udmaHWAttrs[CC2650DK_7ID_UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

/* UDMA configuration structure */
const UDMACC26XX_Config UDMACC26XX_config[] = {
    {
         .object  = &udmaObjects[0],
         .hwAttrs = &udmaHWAttrs[0]
    },
    {NULL, NULL}
};
/*
 *  ============================= UDMA end =====================================
 */

/*
 *  ========================== SPI DMA begin ===================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiCC26XXDMAHWAttrs, ".const:spiCC26XXDMAHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/spi/SPICC26XXDMA.h>

/* SPI objects */
SPICC26XXDMA_Object spiCC26XXDMAObjects[CC2650DK_7ID_SPICOUNT];

/* SPI configuration structure, describing which pins are to be used */
const SPICC26XXDMA_HWAttrsV1 spiCC26XXDMAHWAttrs[CC2650DK_7ID_SPICOUNT] = {
    {
        .baseAddr           = SSI0_BASE,
        .intNum             = INT_SSI0_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue  = 0,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin            = Board_SPI0_MOSI,
        .misoPin            = Board_SPI0_MISO,
        .clkPin             = Board_SPI0_CLK,
        .csnPin             = Board_SPI0_CSN
    },
    {
        .baseAddr           = SSI1_BASE,
        .intNum             = INT_SSI1_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI1,
        .defaultTxBufValue  = 0,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI1_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI1_TX,
        .mosiPin            = Board_SPI1_MOSI,
        .misoPin            = Board_SPI1_MISO,
        .clkPin             = Board_SPI1_CLK,
        .csnPin             = Board_SPI1_CSN
    }
};

/* SPI configuration structure */
const SPI_Config SPI_config[] = {
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[0],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[0]
    },
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[1],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[1]
    },
    {NULL, NULL, NULL}
};
/*
 *  ========================== SPI DMA end =====================================
*/


/*
 *  ========================== LCD begin =======================================
*/
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(LCD_config, ".const:LCD_config")
#pragma DATA_SECTION(lcdHWAttrs, ".const:lcdHWAttrs")
#endif

/* Include lcd middleware */
#include <ti/mw/lcd/LCDDogm1286.h>

/* LCD object */
LCD_Object lcdObject;

/* LCD hardware attribute structure */
const LCD_HWAttrs lcdHWAttrs = {
    .LCD_initCmd = &LCD_initCmd,
    .lcdResetPin = Board_LCD_RST,       /* LCD reset pin */
    .lcdModePin  = Board_LCD_MODE,      /* LCD mode pin  */
    .lcdCsnPin   = Board_LCD_CSN,       /* LCD CSn pin   */
    .spiIndex    = Board_SPI0
};

/* LCD configuration structure */
const LCD_Config LCD_config = {
    .object  = &lcdObject,
    .hwAttrs = &lcdHWAttrs
};
/*
 *  ========================== LCD end =========================================
 */

/*
 *  ========================== Crypto begin ====================================
 *  NOTE: The Crypto implementation should be considered experimental
 *        and not validated!
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(CryptoCC26XX_config, ".const:CryptoCC26XX_config")
#pragma DATA_SECTION(cryptoCC26XXHWAttrs, ".const:cryptoCC26XXHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/crypto/CryptoCC26XX.h>

/* Crypto objects */
CryptoCC26XX_Object cryptoCC26XXObjects[CC2650DK_7ID_CRYPTOCOUNT];

/* Crypto configuration structure, describing which pins are to be used */
const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[CC2650DK_7ID_CRYPTOCOUNT] = {
    {
        .baseAddr       = CRYPTO_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_CRYPTO,
        .intNum         = INT_CRYPTO_RESULT_AVAIL_IRQ,
        .intPriority    = ~0,
    }
};

/* Crypto configuration structure */
const CryptoCC26XX_Config CryptoCC26XX_config[] = {
    {
         .object  = &cryptoCC26XXObjects[0],
         .hwAttrs = &cryptoCC26XXHWAttrs[0]
    },
    {NULL, NULL}
};
/*
 *  ========================== Crypto end =========================================
 */


/*
 *  ========================= RF driver begin ==============================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(RFCC26XX_hwAttrs, ".const:RFCC26XX_hwAttrs")
#endif

/* Include drivers */
#include <ti/drivers/rf/RF.h>

/* RF hwi and swi priority */
const RFCC26XX_HWAttrs RFCC26XX_hwAttrs = {
    .hwiCpe0Priority = ~0,
    .hwiHwPriority   = ~0,
    .swiCpe0Priority =  5,
    .swiHwPriority   =  5,
};

/*
 *  ========================== RF driver end =========================================
 */

/*
 *  ========================= Display begin ====================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Display_config, ".const:Display_config")
#pragma DATA_SECTION(displayDogm1286HWattrs, ".const:displayDogm1286HWAttrs")
#pragma DATA_SECTION(displayUartHWAttrs, ".const:displayUartHWAttrs")
#endif

#include <ti/mw/display/Display.h>
#include <ti/mw/display/DisplayDogm1286.h>
#include <ti/mw/display/DisplayUart.h>

/* Structures for UartPlain Blocking */
DisplayUart_Object        displayUartObject;

#ifndef BOARD_DISPLAY_UART_STRBUF_SIZE
#define BOARD_DISPLAY_UART_STRBUF_SIZE    128
#endif
static char uartStringBuf[BOARD_DISPLAY_UART_STRBUF_SIZE];

const DisplayUart_HWAttrs displayUartHWAttrs = {
    .uartIdx      = Board_UART,
    .baudRate     =     115200,
    .mutexTimeout = BIOS_WAIT_FOREVER,
    .strBuf = uartStringBuf,
    .strBufLen = BOARD_DISPLAY_UART_STRBUF_SIZE,
};

/* Structures for DOGM1286 */
DisplayDogm1286_Object        displayDogm1286Object;

const DisplayDogm1286_HWAttrs displayDogm1286HWattrs = {
    .lcdHandle = (LCD_Handle) & LCD_config,
    .powerPin  = Board_3V3_EN
};

/* Array of displays */
const Display_Config Display_config[] = {
#if !defined(BOARD_DISPLAY_EXCLUDE_UART)
    {
        .fxnTablePtr = &DisplayUart_fxnTable,
        .object      = &displayUartObject,
        .hwAttrs     = &displayUartHWAttrs,
    },
#endif
#if !defined(BOARD_DISPLAY_EXCLUDE_LCD)
    {
        .fxnTablePtr = &DisplayDogm1286_fxnTable,
        .object      = &displayDogm1286Object,
        .hwAttrs     = &displayDogm1286HWattrs
    },
#endif
    { NULL, NULL, NULL } // Terminator
};

/*
 *  ========================= Display end ======================================
 */

/*
 *  ============================ GPTimer begin =================================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPTimerCC26XX_config, ".const:GPTimerCC26XX_config")
#pragma DATA_SECTION(gptimerCC26xxHWAttrs, ".const:gptimerCC26xxHWAttrs")
#endif

/* GPTimer hardware attributes, one per timer part (Timer 0A, 0B, 1A, 1B..) */
const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[CC2650DK_7ID_GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1A, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1B, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2A, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2B, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3A, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3B, },
};

/*  GPTimer objects, one per full-width timer (A+B) (Timer 0, Timer 1..) */
GPTimerCC26XX_Object gptimerCC26XXObjects[CC2650DK_7ID_GPTIMERCOUNT];

/* GPTimer configuration (used as GPTimer_Handle by driver and application) */
const GPTimerCC26XX_Config GPTimerCC26XX_config[CC2650DK_7ID_GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[0], &gptimerCC26xxHWAttrs[0], GPT_A },
    { &gptimerCC26XXObjects[0], &gptimerCC26xxHWAttrs[1], GPT_B },
    { &gptimerCC26XXObjects[1], &gptimerCC26xxHWAttrs[2], GPT_A },
    { &gptimerCC26XXObjects[1], &gptimerCC26xxHWAttrs[3], GPT_B },
    { &gptimerCC26XXObjects[2], &gptimerCC26xxHWAttrs[4], GPT_A },
    { &gptimerCC26XXObjects[2], &gptimerCC26xxHWAttrs[5], GPT_B },
    { &gptimerCC26XXObjects[3], &gptimerCC26xxHWAttrs[6], GPT_A },
    { &gptimerCC26XXObjects[3], &gptimerCC26xxHWAttrs[7], GPT_B },
};

/*
 *  ============================ GPTimer end ===================================
 */



/*
 *  ============================= PWM begin ====================================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PWM_config, ".const:PWM_config")
#pragma DATA_SECTION(pwmtimerCC26xxHWAttrs, ".const:pwmtimerCC26xxHWAttrs")
#endif

/* PWM configuration, one per PWM output.   */
PWMTimerCC26XX_HwAttrs pwmtimerCC26xxHWAttrs[CC2650DK_7ID_PWMCOUNT] = {
    { .pwmPin = Board_PWMPIN0, .gpTimerUnit = Board_GPTIMER0A },
    { .pwmPin = Board_PWMPIN1, .gpTimerUnit = Board_GPTIMER0B },
    { .pwmPin = Board_PWMPIN2, .gpTimerUnit = Board_GPTIMER1A },
    { .pwmPin = Board_PWMPIN3, .gpTimerUnit = Board_GPTIMER1B },
    { .pwmPin = Board_PWMPIN4, .gpTimerUnit = Board_GPTIMER2A },
    { .pwmPin = Board_PWMPIN5, .gpTimerUnit = Board_GPTIMER2B },
    { .pwmPin = Board_PWMPIN6, .gpTimerUnit = Board_GPTIMER3A },
    { .pwmPin = Board_PWMPIN7, .gpTimerUnit = Board_GPTIMER3B },
};

/* PWM object, one per PWM output */
PWMTimerCC26XX_Object pwmtimerCC26xxObjects[CC2650DK_7ID_PWMCOUNT];

extern const PWM_FxnTable PWMTimerCC26XX_fxnTable;

/* PWM configuration (used as PWM_Handle by driver and application) */
const PWM_Config PWM_config[CC2650DK_7ID_PWMCOUNT + 1] = {
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[0], &pwmtimerCC26xxHWAttrs[0] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[1], &pwmtimerCC26xxHWAttrs[1] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[2], &pwmtimerCC26xxHWAttrs[2] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[3], &pwmtimerCC26xxHWAttrs[3] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[4], &pwmtimerCC26xxHWAttrs[4] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[5], &pwmtimerCC26xxHWAttrs[5] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[6], &pwmtimerCC26xxHWAttrs[6] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[7], &pwmtimerCC26xxHWAttrs[7] },
    { NULL,                NULL,                 NULL                 }
};


/*
 *  ============================= PWM end ======================================
 */

/*
 *  ========================== ADCBuf begin =========================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(ADCBuf_config, ".const:ADCBuf_config")
#pragma DATA_SECTION(adcBufCC26xxHWAttrs, ".const:adcBufCC26xxHWAttrs")
#pragma DATA_SECTION(ADCBufCC26XX_adcChannelLut, ".const:ADCBufCC26XX_adcChannelLut")
#endif

/* Include drivers */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufCC26XX.h>

/* ADC objects */
ADCBufCC26XX_Object adcBufCC26xxObjects[CC2650DK_7ID_ADCBufCOUNT];

/*
 *  This table converts a virtual adc channel into a dio and internal analogue input signal.
 *  This table is necessary for the functioning of the adcBuf driver.
 *  Comment out unused entries to save flash.
 *  Dio and internal signal pairs are hardwired. Do not remap them in the table. You may reorder entire entries though.
 *  The mapping of dio and internal signals is package dependent.
 */
const ADCBufCC26XX_AdcChannelLutEntry ADCBufCC26XX_adcChannelLut[] = {
    {Board_ALS_OUT,  ADC_COMPB_IN_AUXIO7},
    {PIN_UNASSIGNED, ADC_COMPB_IN_DCOUPL},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VSS},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VDDS}
};

const ADCBufCC26XX_HWAttrs adcBufCC26xxHWAttrs[CC2650DK_7ID_ADCBufCOUNT] = {
    {
        .intPriority = ~0,
        .swiPriority = 0,
        .adcChannelLut = ADCBufCC26XX_adcChannelLut,
        .gpTimerUnit = Board_GPTIMER0A,
        .gptDMAChannelMask = 1 << UDMA_CHAN_TIMER0_A,
    }
};

const ADCBuf_Config ADCBuf_config[] = {
    {&ADCBufCC26XX_fxnTable, &adcBufCC26xxObjects[0], &adcBufCC26xxHWAttrs[0]},
    {NULL, NULL, NULL},
};
/*
 *  ========================== ADCBuf end =========================================
 */

 /*
 *  ========================== ADC begin =========================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(ADC_config, ".const:ADC_config")
#pragma DATA_SECTION(adcCC26xxHWAttrs, ".const:adcCC26xxHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>

/* ADC objects */
ADCCC26XX_Object adcCC26xxObjects[CC2650DK_7ID_ADCCOUNT];


const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[CC2650DK_7ID_ADCCOUNT] = {
   {
        .adcDIO = Board_DIO23_ANALOG,
        .adcCompBInput = ADC_COMPB_IN_AUXIO7,
        .refSource = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource = ADCCC26XX_TRIGGER_MANUAL
    },
    {
        .adcDIO = PIN_UNASSIGNED,
        .adcCompBInput = ADC_COMPB_IN_DCOUPL,
        .refSource = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource = ADCCC26XX_TRIGGER_MANUAL
    },
    {
        .adcDIO = PIN_UNASSIGNED,
        .adcCompBInput = ADC_COMPB_IN_VSS,
        .refSource = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource = ADCCC26XX_TRIGGER_MANUAL
    },
    {
        .adcDIO = PIN_UNASSIGNED,
        .adcCompBInput = ADC_COMPB_IN_VDDS,
        .refSource = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource = ADCCC26XX_TRIGGER_MANUAL
    }
};

const ADC_Config ADC_config[] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[0], &adcCC26xxHWAttrs[0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[1], &adcCC26xxHWAttrs[1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[2], &adcCC26xxHWAttrs[2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[3], &adcCC26xxHWAttrs[3]},
    {NULL, NULL, NULL},
};

/*
 *  ========================== ADC end =========================================
 */

/*
 *  ========================= TRNG begin ====================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(TRNGCC26XX_config, ".const:TRNGCC26XX_config")
#pragma DATA_SECTION(TRNGCC26XXHWAttrs, ".const:TRNGCC26XXHWAttrs")
#endif

/* Include drivers */
#include <TRNGCC26XX.h>

/* TRNG objects */
TRNGCC26XX_Object trngCC26XXObjects[CC2650DK_7ID_TRNGCOUNT];

/* TRNG configuration structure, describing which pins are to be used */
const TRNGCC26XX_HWAttrs TRNGCC26XXHWAttrs[CC2650DK_7ID_TRNGCOUNT] = {
    {
        .powerMngrId    = PowerCC26XX_PERIPH_TRNG,
    }
};

/* TRNG configuration structure */
const TRNGCC26XX_Config TRNGCC26XX_config[] = {
    {
         .object  = &trngCC26XXObjects[0],
         .hwAttrs = &TRNGCC26XXHWAttrs[0]
    },
    {NULL, NULL}
};

/*
 *  ========================= TRNG end ====================================
 */
