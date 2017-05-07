/******************************************************************************

 @file  rom_jt.h

 @brief This file contains redirection to ROM initialization and jump tables
        for each specific ROM image

 Group: WCS, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2016, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 Release Name: ble_sdk_2_02_01_18
 Release Date: 2016-10-26 15:20:04
 *****************************************************************************/

#ifndef ROM_H
#define ROM_H

/*******************************************************************************
 * INCLUDES
 */

#if defined( ROM_BUILD ) || defined ( COMMON_ROM_BUILD )
// If ROM_BUILD or COMMON_ROM_BUILD are defined then all MAP_ prefixed
// functions must be indirectly to any function address through a jump
// table. These indirect mappings are dependent on which ROM image is
// being built and are found in the following headers:
#ifdef R2
  #include "r2/rom_flash_jt.h"
#else // Default to R1
  #include "r1/r2r_flash_jt.h"
  #include "r1/r2f_flash_jt.h"
  #include "r1/r2r_common_flash_jt.h"
#endif

#else
// If this is not a ROM_BUILD then all MAP_ prefixed functions can be
// directly mapped and linked to the given function's address instead
// of a jump table entry. These mappings are not dependent on the ROM
// image and can be found in the following header:
#include "map_direct.h"
#endif // ROM_BUILD || COMMON_ROM_BUILD

/*******************************************************************************
 * EXTERNS
 */

// ROM's C Runtime initialization
extern void ROM_Init( void );
extern void FPB_Init( void );
extern void CommonROM_Init( void );

#endif // ROM_H
