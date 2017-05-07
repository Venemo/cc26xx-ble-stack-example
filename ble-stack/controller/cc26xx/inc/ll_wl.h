/******************************************************************************

 @file  ll_wl.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy White List structures using the CC26xx
        RF Core Firmware Specification.

 Group: WCS, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2009-2016, Texas Instruments Incorporated
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

#ifndef WL_H
#define WL_H

/*******************************************************************************
 * INCLUDES
 */

#include "rf_hal.h"
#include "bcomdef.h"
#include "ll.h"

/*******************************************************************************
 * MACROS
 */

// Advertising White List
// Note: Assumes wlEntryFlags = white list entry's flags.

#define SET_WL_ENTRY_FREE( wlEntryFlags )                                      \
  (wlEntryFlags) &= ~BV(0)

#define SET_WL_ENTRY_BUSY( wlEntryFlags )                                      \
  (wlEntryFlags) |= BV(0)

#define IS_WL_ENTRY_FREE( wlEntryFlags )                                       \
  ((wlEntryFlags) & BV(0)) == 0

#define IS_WL_ENTRY_BUSY( wlEntryFlags )                                       \
  ((wlEntryFlags) & BV(0)) == 1

#define SET_WL_ENTRY_PUBLIC( wlEntryFlags )                                    \
  (wlEntryFlags) &= ~BV(1)

#define SET_WL_ENTRY_RANDOM( wlEntryFlags )                                    \
  (wlEntryFlags) |= BV(1)

#define GET_WL_ENTRY_ADDR_TYPE( wlEntryFlags )                                 \
  (((wlEntryFlags) & BV(1)) >> 1)

#define CLR_WL_ENTRY_IGNORE( wlEntryFlags )                                    \
  (wlEntryFlags) &= ~BV(2)

#define SET_WL_ENTRY_IGNORE( wlEntryFlags )                                    \
  (wlEntryFlags) |= BV(2)

#define SET_WL_ENTRY_INIT_IGNORE( wlEntryFlags )                               \
  (wlEntryFlags) |= BV(4)

#define CLR_WL_ENTRY_INIT_IGNORE( wlEntryFlags )                               \
  (wlEntryFlags) &= ~BV(4)

#define GET_WL_TABLE_POINTER( pWlEntry )                                       \
    ((wlTable_t *)((uint8 *)(pWlEntry) -                                       \
     (sizeof(wlTable_t) - (sizeof(wlEntry_t) * BLE_NUM_WL_ENTRIES))))

/*******************************************************************************
 * CONSTANTS
 */

// API
#define BLE_IGNORE_WL_ENTRY            0
#define BLE_USE_WL_ENTRY               1

// Miscellaneous
#define BLE_BDADDR_SIZE                6
#define BLE_MAX_NUM_WL_ENTRIES         16  // at 8 bytes per WL entry
#define BLE_NO_WL_MATCH_FOUND          0xFF

// White List Size
#if defined(CTRL_V42_CONFIG) && (CTRL_V42_CONFIG & PRIVACY_1_2_CFG)
  #define BLE_NUM_WL_ENTRIES             (BLE_MAX_NUM_WL_ENTRIES + B_RESOLVING_LIST_SIZE + 1)
#else // !PRIVACY_1_2_CFG
  #define BLE_NUM_WL_ENTRIES             (BLE_MAX_NUM_WL_ENTRIES)
#endif // PRIVACY_1_2_CFG

/*******************************************************************************
 * TYPEDEFS
 */

// BLE White List Flags
// | 7..5 |     4     |  3  |        2         |      1       |      0       |
// |  N/A | bIrkValid | N/A | WL Entry Ignored | Address Type | Entry In Use |
//
typedef uint8 wlFlgs_t;

// White List Entry
// Note: The layout of this structure can not be changed.
PACKED_TYPEDEF_STRUCT
{
  uint8    numEntries;                 // W:  number of white list entries
  wlFlgs_t wlFlags;                    // W:  white list flags (RW for bit 2)
  uint8    devAddr[BLE_BDADDR_SIZE];   // W:  BLE address
} wlEntry_t;

// White List Entry Table
// Note: The layout of this structure can be changed as long as wlEntries is
//       aligned on a word boundary.
PACKED_TYPEDEF_STRUCT
{
  uint8     numWlEntries;
  uint8     numBusyWlEntries;
  uint16    reserve;
  wlEntry_t wlEntries[BLE_NUM_WL_ENTRIES];
} wlTable_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

extern wlTable_t wlTable;
extern wlTable_t wlTableScan;

/*******************************************************************************
 * GLOBAL ROUTINES
 */

extern void       WL_Init( wlTable_t * );

extern llStatus_t WL_Clear( wlTable_t * );

extern void       WL_ClearEntry( wlEntry_t * );

extern uint8      WL_GetSize( wlTable_t * );

extern uint8      WL_GetNumFreeEntries(  wlTable_t * );

extern uint8      WL_FindEntry( wlTable_t *, uint8 *, uint8 );

extern llStatus_t WL_AddEntry( wlTable_t *, uint8 *, uint8, uint8 );

extern llStatus_t WL_RemoveEntry( wlTable_t *, uint8 *, uint8 );

extern llStatus_t WL_SetWlIgnore( wlTable_t *, uint8 *, uint8 );

extern llStatus_t WL_ClearIgnoreList( wlTable_t * );

extern wlEntry_t *WL_Alloc( uint8 );

extern void       WL_Free( wlEntry_t * );

extern wlEntry_t *WL_Copy( wlEntry_t *, wlEntry_t * );

/*******************************************************************************
 */

#endif /* WL_H */


