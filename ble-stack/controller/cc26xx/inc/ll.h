/******************************************************************************

 @file  ll.h

 @brief This file contains the Link Layer (LL) API for the Bluetooth
        Low Energy (BLE) Controller. It provides the defines, types, and
        functions for all supported Bluetooth Low Energy (BLE) commands
        commands.

        This API is based on the Bluetooth Core Specification,
        V4.1.0, Vol. 6.

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
 PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

#ifndef LL_H
#define LL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */
#include "bcomdef.h"

/*******************************************************************************
 * MACROS
 */

// check if connection parameter ranges for CI (min/max), SL, and LSTO are valid
// TRUE = Invalid
#define LL_INVALID_CONN_TIME_PARAM( ciMin, ciMax, sl, lsto )                   \
  (((ciMin) < LL_CONN_INTERVAL_MIN) ||                                         \
   ((ciMin) > LL_CONN_INTERVAL_MAX) ||                                         \
   ((ciMax) < LL_CONN_INTERVAL_MIN) ||                                         \
   ((ciMax) > LL_CONN_INTERVAL_MAX) ||                                         \
   ((ciMax) < (ciMin))              ||                                         \
   ((sl)    > LL_SLAVE_LATENCY_MAX) ||                                         \
   ((lsto)  < LL_CONN_TIMEOUT_MIN)  ||                                         \
   ((lsto)  > LL_CONN_TIMEOUT_MAX))

// check if the CI/SL/LSTO combination is valid
// based on: LSTO > (1 + Slave Latency) * (Connection Interval * 2)
// Note: The CI * 2 requirement based on ESR05 V1.0, Erratum 3904.
// Note: LSTO time is normalized to units of 1.25ms (i.e. 10ms = 8 * 1.25ms).
// TRUE = Invalid
#define LL_INVALID_CONN_TIME_PARAM_COMBO( ci, sl, lsto )                       \
  ((uint32)((lsto)*8) <= ((uint32)(1+(sl)) * (uint32)((ci)*2)))

// check if the APTO combination is valid
// based on: APTO >= (1 + Slave Latency) * Connection Interval
// Note: All input values are assumed to be normalized to 1.25ms!
// TRUE = Invalid
#define LL_INVALID_APTO_COMBO( ci, sl, apto )                                  \
  (((uint32)(apto)) < ((uint32)(1+(sl))) * ((uint32)(ci)))

/*******************************************************************************
 * CONSTANTS
 */

/*
** LL API Status Codes
**
** Note: These status values map directly to the HCI Error Codes.
**       Per the Bluetooth Core Specification, V4.0.0, Vol. 2, Part D.
*/
#define LL_STATUS_SUCCESS                              0x00 // Success
#define LL_STATUS_ERROR_UNKNOWN_CONN_HANDLE            0x02 // Unknown Connection Identifier
#define LL_STATUS_ERROR_INACTIVE_CONNECTION            0x02 // Unknown Connection Identifier for now; may be needed for multiple connections
#define LL_STATUS_ERROR_HW_FAILURE                     0x03 // Hardware Failture
#define LL_STATUS_ERROR_AUTH_FAILURE                   0x05 // Authentication Failure
#define LL_STATUS_ERROR_PIN_OR_KEY_MISSING             0x06 // Pin or Key Missing
#define LL_STATUS_ERROR_OUT_OF_CONN_RESOURCES          0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_OUT_OF_TX_MEM                  0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_OUT_OF_RX_MEM                  0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_OUT_OF_HEAP                    0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_OUT_OF_RESOLVING_LIST          0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_WL_TABLE_FULL                  0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_TX_DATA_QUEUE_FULL             0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_TX_DATA_QUEUE_EMPTY            0x07 // Memory Capacity Exceeded
#define LL_STATUS_ERROR_CONNECTION_TIMEOUT             0x08 // Connection Timeout
#define LL_STATUS_ERROR_CONNECTION_LIMIT_EXCEEDED      0x09 // Connection Limit Exceeded
#define LL_STATUS_ERROR_COMMAND_DISALLOWED             0x0C // Command Disallowed
#define LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES       0x0D // Command Rejected Due To Limited Resources
#define LL_STATUS_ERROR_DUE_TO_DELAYED_RESOURCES       0x0D // Command Delayed Due To Limited Resources
#define LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED          0x11 // Unsupported Feature or Parameter Value
#define LL_STATUS_ERROR_UNEXPECTED_PARAMETER           0x12 // Invalid HCI Command Parameters
#define LL_STATUS_ERROR_ILLEGAL_PARAM_COMBINATION      0x12 // Invalid HCI Command Parameters
#define LL_STATUS_ERROR_BAD_PARAMETER                  0x12 // Invalid HCI Command Parameters or 0x30: Parameter Out of Mandatory Range?
#define LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE           0x12 // Invalid HCI Command Parameters or 0x30: Parameter Out of Mandatory Range?
#define LL_STATUS_ERROR_PEER_TERM                      0x13 // Remote User Terminated Connection
#define LL_STATUS_ERROR_PEER_DEVICE_TERM_LOW_RESOURCES 0x14 // Remote Device Terminated Connection Due To Low Resources
#define LL_STATUS_ERROR_PEER_DEVICE_TERM_POWER_OFF     0x15 // Remote Device Terminated Connection Due To Power Off
#define LL_STATUS_ERROR_HOST_TERM                      0x16 // Connection Terminated By Local Host
#define LL_STATUS_ERROR_REPEATED_ATTEMPTS              0x17 // Repeated Attempts
#define LL_STATUS_ERROR_UNSUPPORTED_REMOTE_FEATURE     0x1A // Unsupported Remote Feature
#define LL_STATUS_ERROR_INVALID_PARAMS                 0x1E // Invalid Parameters
#define LL_STATUS_ERROR_WL_ENTRY_NOT_FOUND             0x1F // Unspecified Error
#define LL_STATUS_ERROR_WL_TABLE_EMPTY                 0x1F // Unspecified Error
#define LL_STATUS_ERROR_WL_TABLE_FAULT                 0x1F // Unspecified Error
#define LL_STATUS_ERROR_RNG_FAILURE                    0x1F // Unspecified Error
#define LL_STATUS_ERROR_DISCONNECT_IMMEDIATE           0x1F // Unspecified Error
#define LL_STATUS_ERROR_DATA_PACKET_QUEUED             0x1F // Unspecified Error
#define LL_STATUS_ERROR_UNSUPPORTED_PARAM_VAL          0x20 // Unsupported Parameter Value
#define LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE          0x21 // Role Change Not Allowed
#define LL_STATUS_ERROR_LL_TIMEOUT                     0x22 // Link Layer Response Timeout
#define LL_STATUS_ERROR_LL_TIMEOUT_HOST                0x22 // Link Layer Response Timeout
#define LL_STATUS_ERROR_LL_TIMEOUT_PEER                0x22 // Link Layer Response Timeout
#define LL_STATUS_ERROR_TRANSACTION_COLLISION          0x23 // Transaction Collision
#define LL_STATUS_ERROR_INSTANT_PASSED                 0x28 // Instant Passed
#define LL_STATUS_ERROR_INSTANT_PASSED_HOST            0x28 // Instant Passed
#define LL_STATUS_ERROR_INSTANT_PASSED_PEER            0x28 // Instant Passed
#define LL_STATUS_ERROR_KEY_PAIRING_NOT_SUPPORTED      0x29 // Pairing With Unit Key Not Supported
#define LL_STATUS_ERROR_DIFFERENT_TRANS_COLLISION      0x2A // Different Transaction Collision
#define LL_STATUS_ERROR_NO_ADV_CHAN_FOUND              0x30 // Parameter Out Of Mandatory Range
#define LL_STATUS_ERROR_PARAM_OUT_OF_RANGE             0x30 // Parameter Out Of Mandatory Range
#define LL_STATUS_ERROR_UPDATE_CTRL_PROC_PENDING       0x3A // Controller Busy
#define LL_STATUS_ERROR_CTRL_PROC_ALREADY_ACTIVE       0x3A // Controller Busy
#define LL_STATUS_ERROR_VER_INFO_REQ_ALREADY_PENDING   0x3A // Controller Busy
#define LL_STATUS_ERROR_UNACCEPTABLE_CONN_INTERVAL     0x3B // Unacceptable Connection Parameters
#define LL_STATUS_ERROR_UNACCEPTABLE_CONN_PARAMETERS   0x3B // Unacceptable Connection Parameters
#define LL_STATUS_ERROR_DIRECTED_ADV_TIMEOUT           0x3C // Directed Advertising Timeout
#define LL_STATUS_ERROR_CONN_TERM_DUE_TO_MIC_FAILURE   0x3D // Connection Terminated Due To MIC Failure
#define LL_STATUS_ERROR_CONN_FAILED_TO_BE_ESTABLISHED  0x3E // Connection Failed To Be Established
#define LL_STATUS_ERROR_CONN_TIMING_FAILURE            0x3F // MAC Connection Failed
#define LL_STATUS_ERROR_COARSE_CLOCK_ADJUST_REJECTED   0x40 // Coarse Clock Adjustment Rejected
// Internal
#define LL_STATUS_WARNING_TX_DISABLED                  0xFF // only used internally, so value doesn't matter
#define LL_STATUS_WARNING_FLAG_UNCHANGED               0xFF // only used internally, so value doesn't matter

// Encryption Key Request Reason Codes
#define LL_ENC_KEY_REQ_ACCEPTED                        LL_STATUS_SUCCESS
#define LL_ENC_KEY_REQ_REJECTED                        LL_STATUS_ERROR_PIN_OR_KEY_MISSING
#define LL_ENC_KEY_REQ_UNSUPPORTED_FEATURE             LL_STATUS_ERROR_UNSUPPORTED_REMOTE_FEATURE

// Disconnect Reason Codes
#define LL_SUPERVISION_TIMEOUT_TERM                    LL_STATUS_ERROR_CONNECTION_TIMEOUT
#define LL_PEER_REQUESTED_TERM                         LL_STATUS_ERROR_PEER_TERM
#define LL_PEER_REQUESTED_LOW_RESOURCES_TERM           LL_STATUS_ERROR_PEER_DEVICE_TERM_LOW_RESOURCES
#define LL_PEER_REQUESTED_POWER_OFF_TERM               LL_STATUS_ERROR_PEER_DEVICE_TERM_POWER_OFF
#define LL_HOST_REQUESTED_TERM                         LL_STATUS_ERROR_HOST_TERM
#define LL_CTRL_PKT_TIMEOUT_TERM                       LL_STATUS_ERROR_LL_TIMEOUT
#define LL_CTRL_PKT_TIMEOUT_HOST_TERM                  LL_STATUS_ERROR_LL_TIMEOUT_HOST
#define LL_CTRL_PKT_TIMEOUT_PEER_TERM                  LL_STATUS_ERROR_LL_TIMEOUT_PEER
#define LL_CTRL_PKT_INSTANT_PASSED_TERM                LL_STATUS_ERROR_INSTANT_PASSED
#define LL_CTRL_PKT_INSTANT_PASSED_HOST_TERM           LL_STATUS_ERROR_INSTANT_PASSED_HOST
#define LL_CTRL_PKT_INSTANT_PASSED_PEER_TERM           LL_STATUS_ERROR_INSTANT_PASSED_PEER
#define LL_UNACCEPTABLE_CONN_INTERVAL_TERM             LL_STATUS_ERROR_UNACCEPTABLE_CONN_INTERVAL
#define LL_MIC_FAILURE_TERM                            LL_STATUS_ERROR_CONN_TERM_DUE_TO_MIC_FAILURE
#define LL_CONN_ESTABLISHMENT_FAILED_TERM              LL_STATUS_ERROR_CONN_FAILED_TO_BE_ESTABLISHED

// Disconnect API Parameter
#define LL_DISCONNECT_AUTH_FAILURE                     LL_STATUS_ERROR_AUTH_FAILURE
#define LL_DISCONNECT_REMOTE_USER_TERM                 LL_STATUS_ERROR_PEER_TERM
#define LL_DISCONNECT_REMOTE_DEV_LOW_RESOURCES         LL_STATUS_ERROR_PEER_DEVICE_TERM_LOW_RESOURCES
#define LL_DISCONNECT_REMOTE_DEV_POWER_OFF             LL_STATUS_ERROR_PEER_DEVICE_TERM_POWER_OFF
#define LL_DISCONNECT_UNSUPPORTED_REMOTE_FEATURE       LL_STATUS_ERROR_UNSUPPORTED_REMOTE_FEATURE
#define LL_DISCONNECT_KEY_PAIRING_NOT_SUPPORTED        LL_STATUS_ERROR_KEY_PAIRING_NOT_SUPPORTED
#define LL_DISCONNECT_UNACCEPTABLE_CONN_INTERVAL       LL_STATUS_ERROR_UNACCEPTABLE_CONN_INTERVAL

/*
** LL RF Channels
*/
#define LL_FIRST_RF_CHAN                               0
#define LL_LAST_RF_CHAN                                39
#define LL_TOTAL_NUM_RF_CHAN                           (LL_LAST_RF_CHAN+1)
//
#define LL_MAX_NUM_ADV_CHAN                            3    // 37, 38, 39
#define LL_MAX_NUM_DATA_CHAN                           37   // 0 - 36

/*
** LL Command Buffers Supported
*/
#if !defined( CC26XX ) && !defined( CC13XX )
#define LL_MAX_NUM_DATA_BUFFERS                        12
#endif // !CC26XX/!CC13XX
#define LL_MAX_NUM_CMD_BUFFERS                         1

/*
** LL API Parameters
*/

// LL Parameter Limits
#define LL_ADV_CONN_INTERVAL_MIN                       32        // 20ms in 625us
#define LL_ADV_CONN_INTERVAL_MAX                       16384     // 10.24s in 625us
#if defined(CTRL_V50_CONFIG) && (CTRL_V50_CONFIG & HDC_NC_ADV_CFG)
#define LL_ADV_NONCONN_INTERVAL_MIN                    32        // 20ms in 625us
#else // !HDC_NC_ADV_CFG
#define LL_ADV_NONCONN_INTERVAL_MIN                    160       // 100ms in 625us
#endif // HDC_NC_ADV_CFG
#define LL_ADV_NONCONN_INTERVAL_MAX                    16384     // 10.24s in 625us
#define LL_ADV_DELAY_MIN                               0         // in ms
#define LL_ADV_DELAY_MAX                               10        // in ms
#define LL_SCAN_INTERVAL_MIN                           4         // 2.5ms in 625us
#define LL_SCAN_INTERVAL_MAX                           16384     // 10.24s in 625us
#define LL_SCAN_WINDOW_MIN                             4         // 2.5ms in 625us
#define LL_SCAN_WINDOW_MAX                             16384     // 10.24s in 625us
#define LL_CONN_INTERVAL_MIN                           6         // 7.5ms in 1.25ms
#define LL_CONN_INTERVAL_MAX                           3200      // 4s in 1.25ms
#define LL_CONN_TIMEOUT_MIN                            10        // 100ms in 10ms
#define LL_CONN_TIMEOUT_MAX                            3200      // 32s in 10ms
#define LL_SLAVE_LATENCY_MIN                           0
#define LL_SLAVE_LATENCY_MAX                           499
#define LL_HOP_LENGTH_MIN                              5
#define LL_HOP_LENGTH_MAX                              16
#define LL_INSTANT_NUMBER_MIN                          6
//
#define LL_MAX_TIMEOUT_EVENTS                          (((LL_CONN_TIMEOUT_MAX*8) / LL_CONN_INTERVAL_MIN) + 1)

// LL Advertiser Channels
#define LL_ADV_CHAN_37                                 1
#define LL_ADV_CHAN_38                                 2
#define LL_ADV_CHAN_39                                 4
#define LL_ADV_CHAN_ALL                                (LL_ADV_CHAN_37 | LL_ADV_CHAN_38 | LL_ADV_CHAN_39)

// LL Advertiser Events
#define LL_ADV_CONNECTABLE_UNDIRECTED_EVT              0
#define LL_ADV_CONNECTABLE_HDC_DIRECTED_EVT            1  // High Duty Cycle
#define LL_ADV_SCANNABLE_UNDIRECTED_EVT                2
#define LL_ADV_NONCONNECTABLE_UNDIRECTED_EVT           3
#define LL_ADV_CONNECTABLE_LDC_DIRECTED_EVT            4  // Low Duty Cycle

// LL Address Type
#define LL_DEV_ADDR_TYPE_MASK                          0x01
#define LL_DEV_ADDR_TYPE_ID_MASK                       0x02
//
#define LL_DEV_ADDR_TYPE_PUBLIC                        0
#define LL_DEV_ADDR_TYPE_RANDOM                        1
#if defined(CTRL_V42_CONFIG) && (CTRL_V42_CONFIG & PRIVACY_1_2_CFG)
#define LL_DEV_ADDR_TYPE_PUBLIC_ID                     2
#define LL_DEV_ADDR_TYPE_RANDOM_ID                     3
#endif // PRIVACY_1_2_CFG

// Advertiser White List Policy
#define LL_ADV_WL_POLICY_ANY_REQ                       0  // any scan request, any connect request
#define LL_ADV_WL_POLICY_WL_SCAN_REQ                   1  // any connect request, white list scan request
#define LL_ADV_WL_POLICY_WL_CONNECT_REQ                2  // any scan request, white list connect request
#define LL_ADV_WL_POLICY_WL_ALL_REQ                    3  // white list scan request and any connect request

// Scanner White List Policy
#define LL_SCAN_WL_POLICY_ANY_ADV_PKTS                 0
#define LL_SCAN_WL_POLICY_USE_WHITE_LIST               1
#if defined(CTRL_V42_CONFIG) && (CTRL_V42_CONFIG & PRIVACY_1_2_CFG)
#define LL_SCAN_WL_POLICY_ANY_ADV_PKTS_EXT             2
#define LL_SCAN_WL_POLICY_USE_WHITE_LIST_EXT           3
#endif // PRIVACY_1_2_CFG

// Initiator White List Policy
#define LL_INIT_WL_POLICY_USE_PEER_ADDR                0
#define LL_INIT_WL_POLICY_USE_WHITE_LIST               1

// Black List Control
#define LL_SET_BLACKLIST_DISABLE                       0
#define LL_SET_BLACKLIST_ENABLE                        1

// Advertiser Commands
#define LL_ADV_MODE_OFF                                0
#define LL_ADV_MODE_ON                                 1
#define LL_ADV_MODE_RESERVED                           2

// LL Scan Commands
#define LL_SCAN_STOP                                   0
#define LL_SCAN_START                                  1

// LL Scan Filtering
#define LL_FILTER_REPORTS_DISABLE                      0
#define LL_FILTER_REPORTS_ENABLE                       1

// LL Scan Types
#define LL_SCAN_PASSIVE                                0
#define LL_SCAN_ACTIVE                                 1

// LL Tx Power Types
#define LL_READ_CURRENT_TX_POWER_LEVEL                 0
#define LL_READ_MAX_TX_POWER_LEVEL                     1

// Data Fragmentation Flag
#define LL_DATA_FIRST_PKT_HOST_TO_CTRL                 0
#define LL_DATA_CONTINUATION_PKT                       1
#define LL_DATA_FIRST_PKT_CTRL_TO_HOST                 2

// Connection Complete Role
#define LL_LINK_CONNECT_COMPLETE_MASTER                0
#define LL_LINK_CONNECT_COMPLETE_SLAVE                 1

// Encryption Related
#define LL_ENCRYPTION_OFF                              0
#define LL_ENCRYPTION_ON                               1

// Receive Flow Control
#define LL_DISABLE_RX_FLOW_CONTROL                     0
#define LL_ENABLE_RX_FLOW_CONTROL                      1

// Direct Test Mode
#define LL_DIRECT_TEST_PAYLOAD_PRBS9                   0
#define LL_DIRECT_TEST_PAYLOAD_0x0F                    1
#define LL_DIRECT_TEST_PAYLOAD_0x55                    2
#define LL_DIRECT_TEST_PAYLOAD_PRBS15                  3
#define LL_DIRECT_TEST_PAYLOAD_0xFF                    4
#define LL_DIRECT_TEST_PAYLOAD_0x00                    5
#define LL_DIRECT_TEST_PAYLOAD_0xF0                    6
#define LL_DIRECT_TEST_PAYLOAD_0xAA                    7
#define LL_DIRECT_TEST_PAYLOAD_UNDEFINED               0xFF
//
#define LL_DIRECT_TEST_MODE_TX                         0
#define LL_DIRECT_TEST_MODE_RX                         1

// V4.2 - Privacy 1.2
#define LL_DISABLE_ADDR_RESOLUTION                     0
#define LL_ENABLE_ADDR_RESOLUTION                      1

//
// V4.2 - Secure Connections
//
#define LL_SC_P256_KEY_LEN                             64
#define LL_SC_DHKEY_LEN                                32
#define LL_SC_RAND_NUM_LEN                             32
#define LL_SC_PRIVATE_KEY_LEN                          32

// V5.0 - 2 Mbps PHY
#define LL_PHY_USE_PHY_PARAM                           0
#define LL_PHY_USE_ANY_PHY                             1
//
#define LL_PHY_NONE                                    0x00
#define LL_PHY_1_MBPS                                  0x01
#define LL_PHY_2_MBPS                                  0x02
#define LL_PHY_CODED                                   0x04

// V5.0 - LR Enhanced DTM PHY
#define LL_DTM_TX_1_MBPS                               0x01
#define LL_DTM_TX_2_MBPS                               0x02
#define LL_DTM_TX_C8                                   0x03
#define LL_DTM_TX_C2                                   0x04
//
#define LL_DTM_RX_1_MBPS                               0x01
#define LL_DTM_RX_2_MBPS                               0x02
#define LL_DTM_RX_CODED                                0x03

// V5.0 - Direct Test Mode Enhanced
#define LL_DTM_STANDARD_MODULATION_INDEX               0
#define LL_DTM_STABLE_MODULATION_INDEX                 1

//
// Vendor Specific
//
#define LL_EXT_RX_GAIN_STD                             0
#define LL_EXT_RX_GAIN_HIGH                            1
//
#if defined( CC26XX ) || defined( CC13XX )
#define LL_EXT_TX_POWER_MINUS_21_DBM                   0
#define LL_EXT_TX_POWER_MINUS_18_DBM                   1
#define LL_EXT_TX_POWER_MINUS_15_DBM                   2
#define LL_EXT_TX_POWER_MINUS_12_DBM                   3
#define LL_EXT_TX_POWER_MINUS_9_DBM                    4
#define LL_EXT_TX_POWER_MINUS_6_DBM                    5
#define LL_EXT_TX_POWER_MINUS_3_DBM                    6
#define LL_EXT_TX_POWER_0_DBM                          7
#define LL_EXT_TX_POWER_1_DBM                          8
#define LL_EXT_TX_POWER_2_DBM                          9
#define LL_EXT_TX_POWER_3_DBM                          10
#define LL_EXT_TX_POWER_4_DBM                          11
#define LL_EXT_TX_POWER_5_DBM                          12
#else // CC254x
#define LL_EXT_TX_POWER_MINUS_23_DBM                   0
#define LL_EXT_TX_POWER_MINUS_6_DBM                    1
#define LL_EXT_TX_POWER_0_DBM                          2
#define LL_EXT_TX_POWER_4_DBM                          3
#endif // CC26XX/CC13XX

//
#define LL_EXT_DISABLE_ONE_PKT_PER_EVT                 0
#define LL_EXT_ENABLE_ONE_PKT_PER_EVT                  1
//
#define LL_EXT_DISABLE_CLK_DIVIDE_ON_HALT              0
#define LL_EXT_ENABLE_CLK_DIVIDE_ON_HALT               1
//
#define LL_EXT_NV_NOT_IN_USE                           0
#define LL_EXT_NV_IN_USE                               1
//
#define LL_EXT_DISABLE_FAST_TX_RESP_TIME               0
#define LL_EXT_ENABLE_FAST_TX_RESP_TIME                1
//
#define LL_EXT_DISABLE_SL_OVERRIDE                     0
#define LL_EXT_ENABLE_SL_OVERRIDE                      1
//
#define LL_EXT_TX_MODULATED_CARRIER                    0
#define LL_EXT_TX_UNMODULATED_CARRIER                  1
//
#define LL_EXT_SET_FREQ_TUNE_DOWN                      0
#define LL_EXT_SET_FREQ_TUNE_UP                        1
//
#define LL_EXT_PM_IO_PORT_P0                           0
#define LL_EXT_PM_IO_PORT_P1                           1
#define LL_EXT_PM_IO_PORT_P2                           2
#define LL_EXT_PM_IO_PORT_NONE                         0xFF
#define LL_EXT_PM_IO_DISABLE                           LL_EXT_PM_IO_PORT_NONE
//
#define LL_EXT_PM_IO_PORT_PIN0                         0
#define LL_EXT_PM_IO_PORT_PIN1                         1
#define LL_EXT_PM_IO_PORT_PIN2                         2
#define LL_EXT_PM_IO_PORT_PIN3                         3
#define LL_EXT_PM_IO_PORT_PIN4                         4
#define LL_EXT_PM_IO_PORT_PIN5                         5
#define LL_EXT_PM_IO_PORT_PIN6                         6
#define LL_EXT_PM_IO_PORT_PIN7                         7
//
#define LL_EXT_PER_RESET                               0
#define LL_EXT_PER_READ                                1
//
#define LL_EXT_HALT_DURING_RF_DISABLE                  0
#define LL_EXT_HALT_DURING_RF_ENABLE                   1
//
#define LL_EXT_SET_USER_REVISION                       0
#define LL_EXT_READ_BUILD_REVISION                     1

//
#define LL_EXT_RESET_SYSTEM_DELAY                      100   // in ms
#define LL_EXT_RESET_SYSTEM_HARD                       0
#define LL_EXT_RESET_SYSTEM_SOFT                       1
//
#define LL_EXT_DISABLE_OVERLAPPED_PROCESSING           0
#define LL_EXT_ENABLE_OVERLAPPED_PROCESSING            1
//
#define LL_EXT_DISABLE_NUM_COMPL_PKTS_ON_EVENT         0
#define LL_EXT_ENABLE_NUM_COMPL_PKTS_ON_EVENT          1

#define LL_EXT_DISABLE_SCAN_REQUEST_REPORT             0
#define LL_EXT_ENABLE_SCAN_REQUEST_REPORT              1

// Packet Lengths
#define LL_DEVICE_ADDR_LEN                             6
#define LL_MAX_ADV_DATA_LEN                            31
#define LL_MAX_ADV_PAYLOAD_LEN                         (LL_DEVICE_ADDR_LEN + LL_MAX_ADV_DATA_LEN)
#define LL_MAX_SCAN_DATA_LEN                           31
#define LL_MAX_SCAN_PAYLOAD_LEN                        (LL_DEVICE_ADDR_LEN + LL_MAX_SCAN_DATA_LEN)
#define LL_MAX_DISCOVERY_DATA_LEN                      31
//
#define LL_MIN_LINK_DATA_LEN                           27    // in bytes
#define LL_MIN_LINK_DATA_TIME                          328   // in us
#define LL_MAX_LINK_DATA_LEN                           251   // in bytes

#if defined(CTRL_V42_CONFIG) && (CTRL_V42_CONFIG & EXT_DATA_LEN_CFG)
  #if defined(CTRL_V50_CONFIG) && (CTRL_V50_CONFIG & LE_CODED_CFG)
    #define LL_MAX_LINK_DATA_TIME                      17040 // in us
  #else // !LE_CODED_CFG
    #define LL_MAX_LINK_DATA_TIME                      2120  // in us
  #endif // LE_CODED_CFG
#else // !EXT_DATA_LEN_CFG
  #if defined(CTRL_V50_CONFIG) && (CTRL_V50_CONFIG & LE_CODED_CFG)
    #define LL_MAX_LINK_DATA_TIME                      2704  // in us
  #else // !LE_CODED_CFG
    #define LL_MAX_LINK_DATA_TIME                      328   // in us
  #endif // LE_CODED_CFG
#endif // EXT_DATA_LEN_CFG

/*
** Event Parameters
*/

// Advertising Report Data
#define LL_ADV_RPT_ADV_IND                             LL_ADV_CONNECTABLE_UNDIRECTED_EVT
#define LL_ADV_RPT_ADV_DIRECT_IND                      LL_ADV_CONNECTABLE_HDC_DIRECTED_EVT
#define LL_ADV_RPT_ADV_SCANNABLE_IND                   LL_ADV_SCANNABLE_UNDIRECTED_EVT
#define LL_ADV_RPT_ADV_NONCONN_IND                     LL_ADV_NONCONNECTABLE_UNDIRECTED_EVT
#define LL_ADV_RPT_SCAN_RSP                            (LL_ADV_NONCONNECTABLE_UNDIRECTED_EVT + 1)
#define LL_ADV_RPT_INVALID                             0xFF

// Scan Request Report Data
// Vendor Specific - Texas Instructures Inc. Only!
#define LL_ADV_RPT_SCAN_REQ                            0

// RSSI
#define LL_RF_RSSI_INVALID                             0x81  // reported by RF
#define LL_RF_RSSI_UNDEFINED                           0x80  // reported by RF
//
#define LL_RSSI_NOT_AVAILABLE                          0x7F  // report to user

// Sleep Clock Accuracy (SCA)
#define LL_SCA_500_PPM                                 0
#define LL_SCA_250_PPM                                 1
#define LL_SCA_150_PPM                                 2
#define LL_SCA_100_PPM                                 3
#define LL_SCA_75_PPM                                  4
#define LL_SCA_50_PPM                                  5
#define LL_SCA_30_PPM                                  6
#define LL_SCA_20_PPM                                  7

/*******************************************************************************
 * TYPEDEFS
 */

typedef uint8 llStatus_t;

// Packet Error Rate Information By Channel
typedef struct
{
  uint16 numPkts[ LL_MAX_NUM_DATA_CHAN ];
  uint16 numCrcErr[ LL_MAX_NUM_DATA_CHAN ];
} perByChan_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * LL OSAL Functions
 */

/*******************************************************************************
 * @fn          LL_Init
 *
 * @brief       This is the Link Layer task initialization called by OSAL. It
 *              must be called once when the software system is started and
 *              before any other function in the LL API is called.
 *
 * input parameters
 *
 * @param       taskId - Task identifier assigned by OSAL.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_Init( uint8 taskId );


/*******************************************************************************
 * @fn          LL_ProcessEvent
 *
 * @brief       This is the Link Layer process event handler called by OSAL.
 *
 * input parameters
 *
 * @param       taskId - Task identifier assigned by OSAL.
 *              events - Event flags to be processed by this task.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Unprocessed event flags.
 */
extern uint16 LL_ProcessEvent( uint8  task_id,
                               uint16 events );


/*******************************************************************************
 * LL API for HCI
 */

/*******************************************************************************
 * @fn          LL_TX_bm_alloc API
 *
 * @brief       This API is used to allocate memory using buffer management.
 *
 *              Note: This function should never be called by the application.
 *                    It is only used by HCI and L2CAP_bm_alloc.
 *
 * input parameters
 *
 * @param       size - Number of bytes to allocate from the heap.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to buffer, or NULL.
 */
extern void *LL_TX_bm_alloc( uint16 size );


/*******************************************************************************
 * @fn          LL_RX_bm_alloc API
 *
 * @brief       This API is used to allocate memory using buffer management.
 *
 *              Note: This function should never be called by the application.
 *                    It is only used by HCI and L2CAP_bm_alloc.
 *
 * input parameters
 *
 * @param       size - Number of bytes to allocate from the heap.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Pointer to buffer, or NULL.
 */
extern void *LL_RX_bm_alloc( uint16 size );


/*******************************************************************************
 * @fn          LL_Reset API
 *
 * @brief       This function is used by the HCI to reset and initialize the
 *              LL Controller.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_Reset( void );


/*******************************************************************************
 * @fn          LL_ReadBDADDR API
 *
 * @brief       This API is called by the HCI to read the controller's
 *              own public device address.
 *
 *              Note: The device's address is stored in NV memory.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       bdAddr  - A pointer to a buffer to hold this device's address.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadBDADDR( uint8 *bdAddr );


/*******************************************************************************
 *
 * @fn          LL_SetRandomAddress API
 *
 * @brief       This function is used to save this device's random address. It
 *              is provided by the Host for devices that are unable to store a
 *              IEEE assigned public address in NV memory.
 *
 * input parameters
 *
 * @param       devAddr - Pointer to a random address (LSO..MSO).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 *
 */
extern llStatus_t LL_SetRandomAddress( uint8 *devAddr );


/*******************************************************************************
 * @fn          LL_ClearWhiteList API
 *
 * @brief       This API is called by the HCI to clear the White List.
 *
 *              Note: If Scanning is enabled using filtering, and the white
 *                    list policy is "Any", then this command will be
 *                    disallowed.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ClearWhiteList( void );


/*******************************************************************************
 * @fn          LL_AddWhiteListDevice API
 *
 * @brief       This API is called by the HCI to add a device address and its
 *              type to the White List.
 *
 * input parameters
 *
 * @param       devAddr      - Pointer to a 6 byte device address.
 * @param       addrType     - Public or Random device address.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_WL_TABLE_FULL
 */
extern llStatus_t LL_AddWhiteListDevice( uint8 *devAddr,
                                         uint8  addrType );

/*******************************************************************************
 * @fn          LL_RemoveWhiteListDevice API
 *
 * @brief       This API is called by the HCI to remove a device address and
 *              it's type from the White List.
 *
 * input parameters
 *
 * @param       devAddr  - Pointer to a 6 byte device address.
 * @param       addrType - Public or Random device address.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_WL_TABLE_EMPTY,
 *              LL_STATUS_ERROR_WL_ENTRY_NOT_FOUND
 */
extern llStatus_t LL_RemoveWhiteListDevice( uint8 *devAddr,
                                            uint8  addrType );


/*******************************************************************************
 * @fn          LL_ReadWlSize API
 *
 * @brief       This API is called by the HCI to get the total number of white
 *              list entries that can be stored in the Controller.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       *numEntries - Total number of available White List entries.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadWlSize( uint8 *numEntries );


/*******************************************************************************
 * @fn          LL_NumEmptyWlEntries API
 *
 * @brief       This API is called by the HCI to get the number of White List
 *              entries that are empty.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       *numEmptyEntries - number of empty entries in the White List.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_NumEmptyWlEntries( uint8 *numEmptyEntries );


/*******************************************************************************
 * @fn          LL_Encrypt API
 *
 * @brief       This API is called by the HCI to request the LL to encrypt the
 *              data in the command using the key given in the command.
 *
 *              Note: The parameters are byte ordered MSO to LSO.
 *
 * input parameters
 *
 * @param       *key           - A 128 bit key to be used to calculate the
 *                               session key.
 * @param       *plaintextData - A 128 bit block that is to be encrypted.
 *
 * output parameters
 *
 * @param       *encryptedData - A 128 bit block that is encrypted.
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_Encrypt( uint8 *key,
                              uint8 *plaintextData,
                              uint8 *encryptedData );


/*******************************************************************************
 * @fn          LL_Rand API
 *
 * @brief       This API is called by the HCI to request the LL Controller to
 *              provide a data block with random content.
 *
 * input parameters
 *
 * @param       *randData - Pointer to buffer to place a random block of data.
 * @param        dataLen  - The length of the random data block, from 1-255.
 *
 * output parameters
 *
 * @param       *randData - Pointer to buffer containing a block of true
 *                          random data.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_Rand( uint8 *randData,
                           uint8  dataLen );


/*******************************************************************************
 * @fn          LL_PseudoRand API
 *
 * @brief       This API is called by the HCI to request the LL Controller to
 *              provide a data block with pseudo random content.
 *
 * input parameters
 *
 * @param       *randData - Pointer to buffer to place a random block of data.
 * @param        dataLen  - The length of the random data block, from 1-255.
 *
 * output parameters
 *
 * @param       *randData - Pointer to buffer containing a block of pseudo
 *                          random data.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED,
 */
extern llStatus_t LL_PseudoRand( uint8 *randData,
                                 uint8  dataLen );


/*******************************************************************************
 * @fn          LL_ReadSupportedStates API
 *
 * @brief       This function is used to provide the HCI with the Link Layer
 *              supported states and supported state/role combinations.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       *states - Eight byte Bit map of supported states/combos.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadSupportedStates( uint8 *states );


/*******************************************************************************
 * @fn          LL_ReadLocalSupportedFeatures API
 *
 * @brief       This API is called by the HCI to read the controller's
 *              Features Set. The Controller indicates which features it
 *              supports.
 *
 * input parameters
 *
 * @param       featureSet  - A pointer to the Feature Set where each bit:
 *                            0: Feature not supported.
 *                            1: Feature supported by controller.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadLocalSupportedFeatures( uint8 *featureSet );


/*******************************************************************************
 * @fn          LL_ReadLocalVersionInfo API
 *
 * @brief       This API is called by the HCI to read the controller's
 *              Version information.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       verNum    - Version of the Bluetooth Controller specification.
 * @param       comId     - Company identifier of the manufacturer of the
 *                          Bluetooth Controller.
 * @param       subverNum - A unique value for each implementation or revision
 *                          of an implementation of the Bluetooth Controller.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadLocalVersionInfo( uint8  *verNum,
                                           uint16 *comId,
                                           uint16 *subverNum );


/*******************************************************************************
 * @fn          LL_CtrlToHostFlowControl API
 *
 * @brief       This function is used to indicate if the LL enable/disable
 *              receive FIFO processing. This function provides support for
 *              Controller to Host flow control.
 *
 * input parameters
 *
 * @param       mode: LL_ENABLE_RX_FLOW_CONTROL, LL_DISABLE_RX_FLOW_CONTROL
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_CtrlToHostFlowControl( uint8 mode );


/*******************************************************************************
 * @fn          LL_ReadRemoteVersionInfo API
 *
 * @brief       This API is called by the HCI to read the peer controller's
 *              Version Information. If the peer's Version Information has
 *              already been received by its request for our Version
 *              Information, then this data is already cached and can be
 *              directly returned to the Host. If the peer's Version Information
 *              is not already cached, then it will be requested from the peer,
 *              and when received, returned to the Host via the
 *              LL_ReadRemoteVersionInfoCback callback.
 *
 *              Note: Only one Version Indication is allowed for a connection.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_VER_IND_ALREADY_SENT
 */
extern llStatus_t LL_ReadRemoteVersionInfo( uint16 connId );


/*******************************************************************************
 * @fn          LL_ReadTxPowerLevel
 *
 * @brief       This function is used to read a connection's current transmit
 *              power level or the maximum transmit power level.
 *
 * input parameters
 *
 * @param       connId   - The LL connection handle.
 * @param       type     - LL_READ_CURRENT_TX_POWER_LEVEL or
 *                         LL_READ_MAX_TX_POWER_LEVEL
 * @param       *txPower - A signed value from -30..+20, in dBm.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_PARAM_OUT_OF_RANGE,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
llStatus_t LL_ReadTxPowerLevel( uint8  connId,
                                uint8  type,
                                int8  *txPower );


/*******************************************************************************
 * @fn          LL_ReadChanMap API
 *
 * @brief       This API is called by the HCI to read the channel map that the
 *              LL controller is using for the LL connection.
 *
 * input parameters
 *
 * @param       connId  - The LL connection handle.
 *
 * output parameters
 *
 * @param       chanMap - A five byte array containing one bit per data channel
 *                        where a 1 means the channel is "used" and a 0 means
 *                        the channel is "unused".
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_ReadChanMap( uint8  connId,
                                  uint8 *chanMap );


/*******************************************************************************
 * @fn          LL_ReadRssi API
 *
 * @brief       This API is called by the HCI to request RSSI. If there is an
 *              active connection for the given connection ID, then the RSSI of
 *              the last received data packet in the LL will be returned. If a
 *              receiver Modem Test is running, then the RF RSSI for the last
 *              received data will be returned. If no valid RSSI value is
 *              available, then LL_RSSI_NOT_AVAILABLE will be returned.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to read last RSSI.
 *
 * output parameters
 *
 * @param       *lastRssi - The last data RSSI received.
 *                          Range: -127dBm..+20dBm, 127=Not Available.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_ReadRssi( uint16  connId,
                               int8   *lastRssi );


/*******************************************************************************
 * @fn          LL_Disconnect API
 *
 * @brief       This API is called by the HCI to terminate a LL connection.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 * @param       reason - The reason for the Host connection termination.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 *              LL_STATUS_ERROR_CTRL_PROC_ALREADY_ACTIVE
 */
extern llStatus_t LL_Disconnect( uint16 connId,
                                 uint8  reason );


/*******************************************************************************
 * @fn          LL_TxData API
 *
 * @brief       This API is called by the HCI to transmit a buffer of data on a
 *              given LL connection. If fragmentation is supported, the HCI must
 *              also indicate whether this is the first Host packet, or a
 *              continuation Host packet. When fragmentation is not supported,
 *              then a start packet should always specified. If the device is in
 *              a connection as a Master and the current connection ID is the
 *              connection for this data, or is in a connection as a Slave, then
 *              the data is written to the TX FIFO (even if the radio is
 *              currently active). If this is a Slave connection, and Fast TX is
 *              enabled and Slave Latency is being used, then the amount of time
 *              to the next event is checked. If there's at least a connection
 *              interval plus some overhead, then the next event is re-aligned
 *              to the next event boundary. Otherwise, in all cases, the buffer
 *              pointer will be retained for transmission, and the callback
 *              event LL_TxDataCompleteCback will be generated to the HCI when
 *              the buffer pointer is no longer needed by the LL.
 *
 *              Note: If the return status is LL_STATUS_ERROR_OUT_OF_TX_MEM,
 *                    then the HCI must not release the buffer until it receives
 *                    the LL_TxDataCompleteCback callback, which indicates the
 *                    LL has copied the transmit buffer.
 *
 *              Note: The HCI should not call this routine if a buffer is still
 *                    pending from a previous call. This is fatal!
 *
 *              Note: If the connection should be terminated within the LL
 *                    before the Host knows, attempts by the HCI to send more
 *                    data (after receiving a LL_TxDataCompleteCback) will
 *                    fail (LL_STATUS_ERROR_INACTIVE_CONNECTION).
 *
 * input parameters
 *
 * @param       connId   - The LL connection ID on which to send this data.
 * @param       *pBuf    - A pointer to the data buffer to transmit.
 * @param       len      - The number of bytes to transmit on this connection.
 * @param       fragFlag - LL_DATA_FIRST_PKT_HOST_TO_CTRL:
 *                           Indicates buffer is the start of a
 *                           Host-to-Controller packet.
 *                         LL_DATA_CONTINUATION_PKT:
 *                           Indicates buffer is a continuation of a
 *                           Host-to-Controller packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION,
 *              LL_STATUS_ERROR_OUT_OF_TX_MEM,
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER
 */
extern llStatus_t LL_TxData( uint16  connId,
                             uint8  *pBuf,
                             uint8   len,
                             uint8   fragFlag );


/*******************************************************************************
 * @fn          LL_DirectTestTxTest API
 *
 * @brief       This function is used to initiate a BLE PHY level Transmit Test
 *              in Direct Test Mode where the DUT generates test reference
 *              packets at fixed intervals. This test will make use of the
 *              nanoRisc Raw Data Transmit and Receive task.
 *
 *              Note: The BLE device is to transmit at maximum power.
 *              Note: A LL reset should be issued when done using DTM!
 *
 * input parameters
 *
 * @param       txChan      - Tx RF frequency k=0..39, where F=2402+(k*2MHz).
 * @param       payloadLen  - Number of bytes (0..37)in payload for each packet.
 * @param       payloadType - The type of pattern to transmit.
 * @param       txPhy       - LL_PHY_1_MBPS
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_DirectTestTxTest( uint8 txChan,
                                       uint8 payloadLen,
                                       uint8 payloadType,
                                       uint8 txPhy );


/*******************************************************************************
 * @fn          LL_DirectTestRxTest API
 *
 * @brief       This function is used to initiate a BLE PHY level Receive Test
 *              in Direct Test Mode where the DUT receives test reference
 *              packets at fixed intervals. This test will make use of the
 *              nanoRisc Raw Data Transmit and Receive task. The received
 *              packets are verified based on the CRC, and metrics are kept.
 *
 *              Note: A LL reset should be issued when done using DTM!
 *
 * input parameters
 *
 * @param       rxChan - Rx Channel k=0..39, where F=2402+(k*2MHz).
 * @param       rxPhy  - LL_PHY_1_MBPS
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_DirectTestRxTest( uint8 rxChan,
                                       uint8 rxPhy );


/*******************************************************************************
 * @fn          LL_DirectTestEnd API
 *
 * @brief       This function is used to end the Direct Test Transmit or Direct
 *              Test Receive tests executing in Direct Test mode. When the raw
 *              task is ended, the LL_DirectTestEndDoneCback callback is called.
 *              If a Direct Test mode operation is not currently active, an
 *              error is returned.
 *
 *              Note: A LL reset is issued upon completion!
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_DirectTestEnd( void );


/*******************************************************************************
 * @fn          LL_SetAdvParam API
 *
 * @brief       This API is called by the HCI to set the Advertiser's
 *              parameters.
 *
 * input parameters
 * @param       advIntervalMin - The minimum Adv interval.
 * @param       advIntervalMax - The maximum Adv interval.
 * @param       advEvtType     - The type of advertisement event.
 * @param       ownAddrType    - The Adv's address type of public or random.
 * @param       directAddrType - Only used for directed advertising.
 * @param       *directAddr    - Only used for directed advertising (NULL otherwise).
 * @param       advChanMap     - A byte containing 1 bit per advertising
 *                               channel. A bit set to 1 means the channel is
 *                               used. The bit positions define the advertising
 *                               channels as follows:
 *                               Bit 0: 37, Bit 1: 38, Bit 2: 39.
 * @param       advWlPolicy    - The Adv white list filter policy.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_NO_ADV_CHAN_FOUND
 */
extern llStatus_t LL_SetAdvParam( uint16  advIntervalMin,
                                  uint16  advIntervalMax,
                                  uint8   advEvtType,
                                  uint8   ownAddrType,
                                  uint8   directAddrType,
                                  uint8  *directAddr,
                                  uint8   advChanMap,
                                  uint8   advWlPolicy );


/*******************************************************************************
 * @fn          LL_SetAdvData API
 *
 * @brief       This API is called by the HCI to set the Advertiser's data.
 *
 *              Note: If the Advertiser is restarted without intervening calls
 *                    to this routine to make updates, then the previously
 *                    defined data will be reused.
 *
 *              Note: If the data happens to be changed while advertising, then
 *                    the new data will be sent on the next advertising event.
 *
 * input parameters
 *
 * @param       advDataLen - The number of scan response bytes: 0..31.
 * @param       advData    - Pointer to the advertiser data, or NULL.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_SetAdvData( uint8  advDataLen,
                                 uint8 *advData );


/*******************************************************************************
 * @fn          LL_SetScanRspData API
 *
 * @brief       This API is called by the HCI to set the Advertiser's Scan
 *              Response data.
 *
 *              Note: If the Advertiser is restarted without intervening calls
 *                    to this routine to make updates, then the previously
 *                    defined data will be reused.
 *
 * input parameters
 *
 * @param       scanRspLen   - The number of scan response bytes: 0..31.
 * @param       *scanRspData - Pointer to the scan response data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_SetScanRspData( uint8  scanRspLen,
                                     uint8 *scanRspData );


/*******************************************************************************
 * @fn          LL_SetAdvControl API
 *
 * @brief       This API is called by the HCI to request the Controller to start
 *              or stop advertising.
 *
 * input parameters
 *
 * @param       advMode - LL_ADV_MODE_ON or LL_ADV_MODE_OFF.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_UNEXPECTED_PARAMETER,
 *              LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_SetAdvControl( uint8 advMode );


/*******************************************************************************
 * @fn          LL_ReadAdvChanTxPower
 *
 * @brief       This function is used to read the transmit power level used
 *              for BLE advertising channel packets. Currently, only two
 *              settings are possible, a standard setting of 0 dBm, and a
 *              maximum setting of 4 dBm.
 *
 * input parameters
 *
 * @param       *txPower - A non-null pointer.
 *
 * output parameters
 *
 * @param       *txPower - A signed value from -20..+10, in dBm.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_PARAM_OUT_OF_RANGE
 */
extern llStatus_t LL_ReadAdvChanTxPower( int8 *txPower );


/*******************************************************************************
 * @fn          LL_SetScanParam API
 *
 * @brief       This API is called by the HCI to set the Scanner's parameters.
 *
 * input parameters
 *
 * @param       scanType     - Passive or Active scan type.
 * @param       scanInterval - Time between scan events.
 * @param       scanWindow   - Duration of a scan. When the same as the scan
 *                             interval, then scan continuously.
 * @param       ownAddrType  - Address type (Public or Random) to use in the
 *                             SCAN_REQ packet.
 * @param       advWlPolicy  - Either allow all Adv packets, or only those that
 *                             are in the white list.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_SetScanParam( uint8  scanType,
                                   uint16 scanInterval,
                                   uint16 scanWindow,
                                   uint8  ownAddrType,
                                   uint8  advWlPolicy );


/*******************************************************************************
 * @fn          LL_SetScanControl API
 *
 * @brief       This API is called by the HCI to start or stop the Scanner. It
 *              also specifies whether the LL will filter duplicate advertising
 *              reports to the Host, or generate a report for each packet
 *              received.
 *
 * input parameters
 *
 * @param       scanMode      - LL_SCAN_START or LL_SCAN_STOP.
 * @param       filterReports - LL_FILTER_REPORTS_DISABLE or
 *                              LL_FILTER_REPORTS_ENABLE
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER,
 *              LL_STATUS_ERROR_OUT_OF_TX_MEM,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_SetScanControl( uint8 scanMode,
                                     uint8 filterReports );


/*******************************************************************************
 * @fn          LL_EncLtkReply API
 *
 * @brief       This API is called by the HCI to provide the controller with
 *              the Long Term Key (LTK) for encryption. This command is
 *              actually a reply to the link layer's LL_EncLtkReqCback, which
 *              provided the random number and encryption diversifier received
 *              from the Master during an encryption setup.
 *
 *              Note: The key parameter is byte ordered LSO to MSO.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 * @param       *key   - A 128 bit key to be used to calculate the session key.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EncLtkReply( uint16  connId,
                                  uint8  *key );


/*******************************************************************************
 * @fn          LL_EncLtkNegReply API
 *
 * @brief       This API is called by the HCI to indicate to the controller
 *              that the Long Term Key (LTK) for encryption can not be provided.
 *              This command is actually a reply to the link layer's
 *              LL_EncLtkReqCback, which provided the random number and
 *              encryption diversifier received from the Master during an
 *              encryption setup. How the LL responds to the negative reply
 *              depends on whether this is part of a start encryption or a
 *              re-start encryption after a pause. For the former, an
 *              encryption request rejection is sent to the peer device. For
 *              the latter, the connection is terminated.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EncLtkNegReply( uint16 connId );


/*******************************************************************************
 * @fn          LL_CreateConn API
 *
 * @brief       This API is called by the HCI to create a connection.
 *
 * input parameters
 *
 * @param       scanInterval    - The scan interval.
 * @param       scanWindow      - The scan window.
 * @param       initWlPolicy    - Filter Adv address directly or using WL.
 * @param       peerAddrType    - Peer address is Public or Random.
 * @param       *peerAddr       - The Adv address, or NULL for WL policy.
 * @param       ownAddrType     - This device's address is Public or Random.
 * @param       connIntervalMin - Defines minimum connection interval value.
 * @param       connIntervalMax - Defines maximum connection interval value.
 * @param       connLatency     - The connection's Slave Latency.
 * @param       connTimeout     - The connection's Supervision Timeout.
 * @param       minLength       - Info parameter about min length of connection.
 * @param       maxLength       - Info parameter about max length of connection.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE,
 *              LL_STATUS_ERROR_ILLEGAL_PARAM_COMBINATION,
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_CreateConn( uint16  scanInterval,
                                 uint16  scanWindow,
                                 uint8   initWlPolicy,
                                 uint8   peerAddrType,
                                 uint8  *peerAddr,
                                 uint8   ownAddrType,
                                 uint16  connIntervalMin,
                                 uint16  connIntervalMax,
                                 uint16  connLatency,
                                 uint16  connTimeout,
                                 uint16  minLength,
                                 uint16  maxLength );


/*******************************************************************************
 * @fn          LL_CreateConnCancel API
 *
 * @brief       This API is called by the HCI to cancel a previously given LL
 *              connection creation command that is still pending. This command
 *              should only be used after the LL_CreateConn command as been
 *              issued, but before the LL_ConnComplete callback.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_CreateConnCancel( void );


/*******************************************************************************
 * @fn          LL_ConnActive
 *
 * @brief       This API is called by the HCI to check if a connection
 *              given by the connection handle is active.
 *
 * input parameters
 *
 * @param       connId - Connection handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_ConnActive( uint16 connId );


/*******************************************************************************
 * @fn          LL_ConnUpdate API
 *
 * @brief       This API is called by the HCI to update the connection
 *              parameters by initiating a connection update control procedure.
 *
 * input parameters
 *
 * @param       connId          - The connection ID on which to send this data.
 * @param       connIntervalMin - Defines minimum connection interval value.
 * @param       connIntervalMax - Defines maximum connection interval value.
 * @param       connLatency     - The connection's Slave Latency.
 * @param       connTimeout     - The connection's Supervision Timeout.
 * @param       minLength       - Info parameter about min length of connection.
 * @param       maxLength       - Info parameter about max length of connection.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 *              LL_STATUS_ERROR_CTRL_PROC_ALREADY_ACTIVE,
 *              LL_STATUS_ERROR_ILLEGAL_PARAM_COMBINATION
 */
extern llStatus_t LL_ConnUpdate( uint16 connId,
                                 uint16 connIntervalMin,
                                 uint16 connIntervalMax,
                                 uint16 connLatency,
                                 uint16 connTimeout,
                                 uint16 minLength,
                                 uint16 maxLength );


/*******************************************************************************
 * @fn          LL_ChanMapUpdate API
 *
 * @brief       This API is called by the HCI to update the Host data channels
 *              initiating an Update Data Channel control procedure.
 *
 *              Note: While it isn't specified, it is assumed that the Host
 *                    expects an update channel map on all active connections.
 *
 *              Note: This LL currently only supports one connection.
 *
 * input parameters
 *
 * @param       chanMap - A five byte array containing one bit per data channel
 *                        where a 1 means the channel is "used".
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_ILLEGAL_PARAM_COMBINATION
 */
extern llStatus_t LL_ChanMapUpdate( uint8 *chanMap );


/*******************************************************************************
 * @fn          LL_StartEncrypt API
 *
 * @brief       This API is called by the Master HCI to setup encryption and to
 *              update encryption keys in the LL connection. If the connection
 *              is already in encryption mode, then this command will first
 *              pause the encryption before subsequently running the encryption
 *              setup.
 *
 *              Note: The parameters are byte ordered LSO to MSO.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 * @param       *rand  - Random vector used in device identification.
 * @param       *eDiv  - Encrypted diversifier.
 * @param       *key   - A 128 bit key to be used to calculate the session key.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 */
extern llStatus_t LL_StartEncrypt( uint16  connId,
                                   uint8  *rand,
                                   uint8  *eDiv,
                                   uint8  *ltk );


/*******************************************************************************
 * @fn          LL_ReadRemoteUsedFeatures API
 *
 * @brief       This API is called by the Master or Slave HCI to initiate a
 *              Feature Exchange control process.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_ReadRemoteUsedFeatures( uint16 connId );


/*******************************************************************************
 * @fn          LL_ReadAuthPayloadTimeout API
 *
 * @brief       This LL API is used to read the connection's Authenticated
 *              Payload Timeout value.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID to read the APTO value from.
 *
 * output parameters
 *
 * @param       apto   - Pointer to current APTO value, in units of 10ms.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadAuthPayloadTimeout( uint16  connId,
                                             uint16 *apto );


/*******************************************************************************
 * @fn          LL_WriteAuthPayloadTimeout API
 *
 * @brief       This LL API is used to write the connection's Authenticated
 *              Payload Timeout value.
 *
 * input parameters
 *
 * @param       connId     - The LL connection ID to write the APTO value to.
 * @param       aptoValue  - The APTO value, in units of 10ms.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_WriteAuthPayloadTimeout( uint16 connId,
                                              uint16 apto );


/*******************************************************************************
 * @fn          LL_RemoteConnParamReqReply API
 *
 * @brief       This LL API is used to receive the accepted Host connection
 *              parameters in response to the LE Remote Connection Parameter
 *              Request Event.
 *
 * input parameters
 *
 * @param       connHandle       - Connection handle.
 * @param       connIntervalMin  - Minimum allowed connection interval.
 * @param       connIntervalMax  - Maximum allowed connection interval.
 * @param       connLatency      - Number of skipped events (slave latency).
 * @param       connTimeout      - Connection supervision timeout.
 * @param       minLen           - Info parameter about min length of conn.
 * @param       maxLen           - Info parameter about max length of conn.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_RemoteConnParamReqReply( uint16 connHandle,
                                              uint16 connIntervalMin,
                                              uint16 connIntervalMax,
                                              uint16 connLatency,
                                              uint16 connTimeout,
                                              uint16 minLen,
                                              uint16 maxLen );

/*******************************************************************************
 * @fn          LL_RemoteConnParamReqNegReply API
 *
 * @brief       This LL API is used to receive the Host's rejection to the LE
 *              Remote Connection Parameter Request event from the Controller.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 * @param       reason     - Reason connection parameter request was rejected.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_RemoteConnParamReqNegReply( uint16 connHandle,
                                                 uint8  reason );


/*******************************************************************************
 * @fn          LL_GetNumActiveConns API
 *
 * @brief       This LL API is used to provide the HCI with the number of
 *              active Link Layer connections.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       numActiveConns - Number of active Link Layer connections.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_GetNumActiveConns( uint8 *numActiveConns );


// V4.2 - Extended Data Length

/*******************************************************************************
 * @fn          LL_SetDataLen API
 *
 * @brief       This LL API is used to set the maximum transmission packet size
 *              and the maximum packet transmission time for the connection.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 * @param       txOctets   - Maximum number of transmit payload bytes.
 * @param       txTime     - Maximum transmit time.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_SetDataLen( uint16 connHandle,
                                 uint16 txOctets,
                                 uint16 txTime);


/*******************************************************************************
 * @fn          LL_ReadDefaultDataLen API
 *
 * @brief       This LL API is used to read the default maximum trasmit packet
 *              size and the default maximum packet transmit time to be used
 *              for new connections.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       txOctets - Maximum number of transmit payload bytes.
 * @param       txTime   - Maximum transmit time.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadDefaultDataLen( uint16 *txOctets,
                                         uint16 *txTime );


/*******************************************************************************
 * @fn          LL_WriteDefaultDataLen API
 *
 * @brief       This LL API is used to set the default maximum transmission
 *              packet size and the default maximum packet transmission time
 *              for the connection.
 *
 * input parameters
 *
 * @param       txOctets - Maximum number of transmit payload bytes.
 * @param       txTime   - Maximum transmit time.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_WriteDefaultDataLen( uint16 txOctets,
                                          uint16 txTime);


/*******************************************************************************
 * @fn          LL_ReadMaxDataLen API
 *
 * @brief       This LL API is used to read the maximum supported transmit and
 *              receive payload octets and packet duration times.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       maxTxOctets - Maximum supported transmit payload bytes.
 * @param       maxTxTime   - Maximum supported transmit time.
 * @param       maxRxOctets - Maximum supported receive payload bytes.
 * @param       maxRxTime   - Maximum supported receive time.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadMaxDataLen( uint16 *maxTxOctets,
                                     uint16 *maxTxTime,
                                     uint16 *maxRxOctets,
                                     uint16 *maxRxTime );

// V4.2 - Privacy 1.2

/*******************************************************************************
 * @fn          LL_AddDeviceToResolvingList API
 *
 * @brief       This LL is used to add one device to the list of address
 *              translations used to resolve Resolvable Private Addresses in
 *              the Controller.
 *
 * input parameters
 *
 * @param       peerIdAddrType - LL_DEV_ADDR_TYPE_PUBLIC,
 *                               LL_DEV_ADDR_TYPE_RANDOM
 * @param       peerIdAddr     - Peer device Identity Address.
 * @param       peerIRK        - IRK of peer device.
 * @param       localIRK       - IRK for own device.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_AddDeviceToResolvingList( uint8  peerIdAddrType,
                                               uint8 *peerIdAddr,
                                               uint8 *peerIRK,
                                               uint8 *localIRK );

/*******************************************************************************
 * @fn          LL_RemoveDeviceFromResolvingList API
 *
 * @brief       This LL is used to remove one device fromthe list of address
 *              translations used to resolve Resolvable Private Addresses in
 *              the Controller.
 *
 * input parameters
 *
 * @param       peerIdAddrType - LL_DEV_ADDR_TYPE_PUBLIC,
 *                               LL_DEV_ADDR_TYPE_RANDOM
 * @param       peerIdAddr     - Peer device Identity Address.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_RemoveDeviceFromResolvingList( uint8  peerIdAddrType,
                                                    uint8 *peerIdAddr );


/*******************************************************************************
 * @fn          LL_ClearResolvingList API
 *
 * @brief       This LL is used to remove all devices from the list of address
 *              translations used to resolve Resolvable Private addresses in
 *              the Controller.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ClearResolvingList( void );


/*******************************************************************************
 * @fn          LL_ReadResolvingListSize API
 *
 * @brief       This LL is used to read the total number of address translation
 *              entries in the resolving list that can be stored in the
 *              Controller.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       resolvingListSize - Number of entries in the Resolving List.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadResolvingListSize( uint8 *resolvingListSize );


/*******************************************************************************
 * @fn          LL_ReadPeerResolvableAddress API
 *
 * @brief       This LL is used to get the current peer Resolvable Private
 *              Address being used for the corresponding peer Public or
 *              Random (Static) Identity Address.
 *
 *              Note: The peer's Resolvable Private Address being used may
 *                    change after this command is called.
 *
 * input parameters
 *
 * @param       peerIdAddrType - LL_DEV_ADDR_TYPE_PUBLIC,
 *                               LL_DEV_ADDR_TYPE_RANDOM
 * @param       peerIdAddr     - Peer device Identity Address.
 *
 * output parameters
 *
 * @param       peerRPA        - Peer device RPA.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadPeerResolvableAddress( uint8  peerIdAddrType,
                                                uint8 *peerIdAddr,
                                                uint8 *peerRPA );


/*******************************************************************************
 * @fn          LL_ReadLocalResolvableAddress API
 *
 * @brief       This LL is used to get the current local Resolvable Private
 *              Address being used for the corresponding local Public or
 *              Random (Static) Identity Address.
 *
 *              Note: The local Resolvable Private Address being used may
 *                    change after this command is called.
 *
 * input parameters
 *
 * @param       localIdAddrType - LL_DEV_ADDR_TYPE_PUBLIC,
 *                                LL_DEV_ADDR_TYPE_RANDOM
 * @param       localIdAddr     - Local device Identity Address.
 *
 * output parameters
 *
 * @param       localRPA        - Local device RPA.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadLocalResolvableAddress( uint8  localIdAddrType,
                                                 uint8 *localIdAddr,
                                                 uint8 *localRPA );


/*******************************************************************************
 * @fn          LL_SetAddressResolutionEnable API
 *
 * @brief       This LL is used to enable resolution of Resolvable Private
 *              Addresses in the Controller. This causes the Controller to
 *              use the resolving list whenever the Controller receives a
 *              local or peer Resolvable Private Address.
 *
 * input parameters
 *
 * @param       addrResolutionEnable - LL_DISABLE_ADDR_RESOLUTION,
 *                                     LL_ENABLE_ADDR_RESOLUTION
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_SetAddressResolutionEnable( uint8 addrResolutionEnable );


/*******************************************************************************
 * @fn          LL_SetResolvablePrivateAddressTimeout API
 *
 * @brief       This LL is used to set the length of time the Controller uses
 *              a Resolvable Private Address before a new Resolvable Private
 *              Address is generated and starts being used.
 *
 *              Note: This timeout applies to all addresses generated by
 *                    the Controller..
 *
 * input parameters
 *
 * @param       rpaTimeout - RPA timeout (in secs).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_SetResolvablePrivateAddressTimeout( uint16 rpaTimeout );


// V4.2 - Secure Connections

/*******************************************************************************
 * @fn          LL_ReadLocalP256PublicKeyCmd API
 *
 * @brief       This LL API is used to read the local P-256 public key from the
 *              Controller. The Controller shall generate a new P-256 public/
 *              private key pair upon receipt of this command.
 *
 *              Note: Generates LE Read Local P256 Public Key Complete event.
 *
 *              WARNING: THIS ROUTINE WILL TIE UP THE LL FOR ABOUT 160ms!
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      hciStatus_t
 */
extern llStatus_t LL_ReadLocalP256PublicKeyCmd( void );


/*******************************************************************************
 * @fn          LL_GenerateDHKeyCmd API
 *
 * @brief       This LL API is used to initiate the generation of a Diffie-
 *              Hellman key in the Controller for use over the LE transport.
 *              This command takes the remote P-256 public key as input. The
 *              Diffie-Hellman key generation uses the private key generated
 *              by LE_Read_Local_P256_Public_Key command.
 *
 *              Note: Generates LE DHKey Generation Complete event.
 *
 *              WARNING: THIS ROUTINE WILL TIE UP THE LL FOR ABOUT 160ms!
 *
 * input parameters
 *
 * @param       publicKey: The remote P-256 public key (X-Y format).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      hciStatus_t
 */
extern llStatus_t LL_GenerateDHKeyCmd( uint8 *publicKey );


// V5.0 - 2Mbps PHY

/*******************************************************************************
 * @fn          LL_ReadPhy API
 *
 * @brief       This LL API is used to read the current transmitter and receiver
 *              PHY.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 *
 * output parameters
 *
 * @param       txPhy   - Bit field of Host preferred Tx PHY.
 * @param       rxPhy   - Bit field of Host preferred Rx PHY.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_ReadPhy( uint16  connHandle,
                              uint8  *txPhy,
                              uint8  *rxPhy );


/*******************************************************************************
 * @fn          LL_SetDefaultPhy API
 *
 * @brief       This LL API allows the Host to specify its preferred values for
 *              the transmitter and receiver PHY to be used for all subsequent
 *              connections.
 *
 * input parameters
 *
 * @param       allPhys - Host preference on how to handle txPhy and rxPhy.
 * @param       txPhy   - Bit field of Host preferred Tx PHY.
 * @param       rxPhy   - Bit field of Host preferred Rx PHY.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_SetDefaultPhy( uint8 allPhys,
                                    uint8 txPhy,
                                    uint8 rxPhy );


/*******************************************************************************
 * @fn          LL_SetPhy API
 *
 * @brief       This LL API is used to request a change to the transmitter and
 *              receiver PHY for a connection.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 * @param       allPhys    - Host preference on how to handle txPhy and rxPhy.
 * @param       txPhy      - Bit field of Host preferred Tx PHY.
 * @param       rxPhy      - Bit field of Host preferred Rx PHY.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_SetPhy( uint16 connHandle,
                             uint8  allPhys,
                             uint8  txPhy,
                             uint8  rxPhy );


/*******************************************************************************
 * @fn          LL_EnhancedRxTest API
 *
 * @brief       This LL API is used to start a test where the DUT receives
 *              reference packets at a fixed interval. The tester generates
 *              the test reference packets.
 *
 * input parameters
 *
 * @param       rxChan - Rx Channel k=0..39, where F=2402+(k*2MHz).
 * @param       rxPhy  - Rx PHY to use.
 * @param       modIndex - LL_DTM_STANDARD_MODULATION_INDEX,
 *                         LL_DTM_STABLE_MODULATION_INDEX
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_EnhancedRxTest( uint8 rxChan,
                                     uint8 rxPhy,
                                     uint8 modIndex );


/*******************************************************************************
 * @fn          LL_EnhancedTxTest API
 *
 * @brief       This LL API is used to start a test where the DUT generates
 *              test reference packets at a fixed interval. The Controller
 *              shall transmit at maximum power.
 *
 * input parameters
 *
 * @param       txChan      - Tx RF channel k=0..39, where F=2402+(k*2MHz).
 * @param       payloadLen  - Byte length (0..37) in payload for each packet.
 * @param       payloadType - The type of pattern to transmit.
 * @param       txPhy       - Tx PHY to use.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_EnhancedTxTest( uint8 txChan,
                                     uint8 payloadLen,
                                     uint8 payloadType,
                                     uint8 txPhy );

/*
** Vendor Specific Command API
*/

/*******************************************************************************
 * @fn          LL_EXT_SetRxGain Vendor Specific API
 *
 * @brief       This function is used to to set the RF RX gain.
 *
 * input parameters
 *
 * @param       rxGain - LL_EXT_RX_GAIN_STD, LL_EXT_RX_GAIN_HIGH
 *
 * output parameters
 *
 * @param       cmdComplete - Boolean to indicate the command is still pending.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetRxGain( uint8  rxGain,
                                    uint8 *cmdComplete );


/*******************************************************************************
 * @fn          LL_EXT_SetTxPower Vendor Specific API
 *
 * @brief       This function is used to to set the RF TX power.
 *
 * input parameters
 *
 * @param       txPower - For CC254x: LL_EXT_TX_POWER_MINUS_23_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_6_DBM,
 *                                    LL_EXT_TX_POWER_0_DBM,
 *                                    LL_EXT_TX_POWER_4_DBM
 *
 *                        For CC26xx: LL_EXT_TX_POWER_MINUS_21_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_18_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_15_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_12_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_9_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_6_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_3_DBM,
 *                                    LL_EXT_TX_POWER_0_DBM,
 *                                    LL_EXT_TX_POWER_1_DBM,
 *                                    LL_EXT_TX_POWER_2_DBM,
 *                                    LL_EXT_TX_POWER_3_DBM,
 *                                    LL_EXT_TX_POWER_4_DBM,
 *                                    LL_EXT_TX_POWER_5_DBM
 *
 * output parameters
 *
 * @param       cmdComplete - Boolean to indicate the command is still pending.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetTxPower( uint8  txPower,
                                     uint8 *cmdComplete );


/*******************************************************************************
 * @fn          LL_EXT_OnePacketPerEvent Vendor Specific API
 *
 * @brief       This function is used to enable or disable allowing only one
 *              packet per event.
 *
 * input parameters
 *
 * @param       control - LL_EXT_ENABLE_ONE_PKT_PER_EVT,
 *                        LL_EXT_DISABLE_ONE_PKT_PER_EVT
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_OnePacketPerEvent( uint8 control );


/*******************************************************************************
 * @fn          LL_EXT_ClkDivOnHalt Vendor Specific API
 *
 * @brief       This function is used to enable or disable dividing down the
 *              system clock while halted.
 *
 *              Note: This command is disallowed if haltDuringRf is not defined.
 *
 * input parameters
 *
 * @param       control - LL_EXT_ENABLE_CLK_DIVIDE_ON_HALT,
 *                        LL_EXT_DISABLE_CLK_DIVIDE_ON_HALT
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_ClkDivOnHalt( uint8 control );


/*******************************************************************************
 * @fn          LL_EXT_DeclareNvUsage Vendor Specific API
 *
 * @brief       This HCI Extension API is used to indicate to the Controller
 *              whether or not the Host will be using the NV memory during BLE
 *              operations.
 *
 * input parameters
 *
 * @param       mode - HCI_EXT_NV_IN_USE, HCI_EXT_NV_NOT_IN_USE
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_DeclareNvUsage( uint8 mode );


/*******************************************************************************
 * @fn          LL_EXT_Decrypt API
 *
 * @brief       This API is called by the HCI to request the LL to decrypt the
 *              data in the command using the key given in the command.
 *
 *              Note: The parameters are byte ordered MSO to LSO.
 *
 * input parameters
 *
 * @param       *key           - A 128 bit key to be used to calculate the
 *                               session key.
 * @param       *encryptedData - A 128 bit block that is encrypted.
 *
 * output parameters
 *
 * @param       *plaintextData - A 128 bit decrypted block.
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EXT_Decrypt( uint8 *key,
                                  uint8 *encryptedData,
                                  uint8 *plaintextData );


/*******************************************************************************
 * @fn          LL_EXT_SetLocalSupportedFeatures API
 *
 * @brief       This API is called by the HCI to indicate to the Controller
 *              which features can or can not be used.
 *
 *              Note: Not all features indicated by the Host to the Controller
 *                    are valid. If invalid, they shall be ignored.
 *
 * input parameters
 *
 * @param       featureSet  - A pointer to the Feature Set where each bit:
 *                            0: Feature shall not be used.
 *                            1: Feature can be used.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EXT_SetLocalSupportedFeatures( uint8 *featureSet );


/*******************************************************************************
 * @fn          LL_EXT_SetFastTxResponseTime API
 *
 * @brief       This API is used to enable or disable the fast TX response
 *              time feature. This can be helpful when a short connection
 *              interval is used in combination with slave latency. In such
 *              a scenario, the response time for sending the TX data packet
 *              can effectively shorten or eliminate slave latency, thereby
 *              increasing power consumption. By disabling, this feature
 *              trades fast response time for less power consumption.
 *
 * input parameters
 *
 * @param       control - LL_EXT_ENABLE_FAST_TX_RESP_TIME,
 *                        LL_EXT_DISABLE_FAST_TX_RESP_TIME
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED,
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetFastTxResponseTime( uint8 control );


/*******************************************************************************
 * @fn          LL_EXT_SetSlaveLatencyOverride API
 *
 * @brief       This API is used to enable or disable the suspension of slave
 *              latency. This can be helpful when the Slave application knows
 *              it will soon receive something that needs to be handled without
 *              delay.
 *
 * input parameters
 *
 * @param       control - LL_EXT_DISABLE_SL_OVERRIDE,
 *                        LL_EXT_ENABLE_SL_OVERRIDE
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED,
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetSlaveLatencyOverride( uint8 control );


/*******************************************************************************
 * @fn          LL_EXT_ModemTestTx
 *
 * @brief       This API is used start a continuous transmitter modem test,
 *              using either a modulated or unmodulated carrier wave tone, at
 *              the frequency that corresponds to the specified RF channel. Use
 *              LL_EXT_EndModemTest command to end the test.
 *
 *              Note: A LL reset will be issued by LL_EXT_EndModemTest!
 *              Note: The BLE device will transmit at maximum power.
 *              Note: This API can be used to verify this device meets Japan's
 *                    TELEC regulations.
 *
 * input parameters
 *
 * @param       cwMode - LL_EXT_TX_MODULATED_CARRIER,
 *                       LL_EXT_TX_UNMODULATED_CARRIER
 *              rfChan - Transmit RF channel k=0..39, where BLE F=2402+(k*2MHz).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_EXT_ModemTestTx( uint8 cwMode,
                                      uint8 rfChan );


/*******************************************************************************
 * @fn          LL_EXT_ModemHopTestTx
 *
 * @brief       This API is used to start a continuous transmitter direct test
 *              mode test using a modulated carrier wave and transmitting a
 *              37 byte packet of Pseudo-Random 9-bit data. A packet is
 *              transmitted on a different frequency (linearly stepping through
 *              all RF channels 0..39) every 625us. Use LL_EXT_EndModemTest
 *              command to end the test.
 *
 *              Note: A LL reset will be issued by LL_EXT_EndModemTest!
 *              Note: The BLE device will transmit at maximum power.
 *              Note: This API can be used to verify this device meets Japan's
 *                    TELEC regulations.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_EXT_ModemHopTestTx( void );


/*******************************************************************************
 * @fn          LL_EXT_ModemTestRx
 *
 * @brief       This API is used to start a continuous receiver modem test
 *              using a modulated carrier wave tone, at the frequency that
 *              corresponds to the specific RF channel. Any received data is
 *              discarded. Receiver gain may be adjusted using the
 *              LL_EXT_SetRxGain command. RSSI may be read during this test by
 *              using the LL_ReadRssi command. Use LL_EXT_EndModemTest command
 *              to end the test.
 *
 *              Note: A LL reset will be issued by LL_EXT_EndModemTest!
 *              Note: The BLE device will transmit at maximum power.
 *
 * input parameters
 *
 * @param       rxChan - Receiver RF channel k=0..39, where BLE F=2402+(k*2MHz).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_EXT_ModemTestRx( uint8 rxChan );


/*******************************************************************************
 * @fn          LL_EXT_EndModemTest
 *
 * @brief       This API is used to shutdown a modem test. A complete link
 *              layer reset will take place.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_UNEXPECTED_STATE_ROLE
 */
extern llStatus_t LL_EXT_EndModemTest( void );


/*******************************************************************************
 * @fn          LL_EXT_SetBDADDR
 *
 * @brief       This API is used to set this device's BLE address (BDADDR).
 *
 *              Note: This command is only allowed when the device's state is
 *                    Standby.
 *
 * input parameters
 *
 * @param       bdAddr  - A pointer to a buffer to hold this device's address.
 *                        An invalid address (i.e. all FF's) will restore this
 *                        device's address to the address set at initialization.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_SetBDADDR( uint8 *bdAddr );


/*******************************************************************************
 * @fn          LL_EXT_SetSCA
 *
 * @brief       This API is used to set this device's Sleep Clock Accuracy.
 *
 *              Note: For a slave device, this value is directly used, but only
 *                    if power management is enabled. For a master device, this
 *                    value is converted into one of eight ordinal values
 *                    representing a SCA range, as specified in Table 2.2,
 *                    Vol. 6, Part B, Section 2.3.3.1 of the Core specification.
 *
 *              Note: This command is only allowed when the device is not in a
 *                    connection.
 *
 *              Note: The device's SCA value remains unaffected by a HCI_Reset.
 *
 * input parameters
 *
 * @param       scaInPPM - This device's SCA in PPM from 0..500.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_SetSCA( uint16 scaInPPM );


/*******************************************************************************
 * @fn          LL_EXT_SetFreqTune
 *
 * @brief       This API is used to set the Frequency Tuning up or down. If the
 *              current setting is already at the max/min value, then no
 *              update is performed.
 *
 *              Note: This is a Production Test Mode only command!
 *
 * input parameters
 *
 * @param       step - LL_EXT_SET_FREQ_TUNE_UP or LL_EXT_SET_FREQ_TUNE_DOWN
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetFreqTune( uint8 step );


/*******************************************************************************
 * @fn          LL_EXT_SaveFreqTune
 *
 * @brief       This API is used to save the current Frequency Tuning value to
 *              flash memory. It is restored on reboot or wake from sleep.
 *
 *              Note: This is a Production Test Mode only command!
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_SaveFreqTune( void );


/*******************************************************************************
 * @fn          LL_EXT_SetMaxDtmTxPower Vendor Specific API
 *
 * @brief       This function is used to set the max RF TX power to be used
 *              when using Direct Test Mode.
 *
 * input parameters
 *
 * @param       txPower - For CC254x: LL_EXT_TX_POWER_MINUS_23_DBM,
 *                                    LL_EXT_TX_POWER_MINUS_6_DBM,
 *                                    LL_EXT_TX_POWER_0_DBM,
 *                                    LL_EXT_TX_POWER_4_DBM
 *
 *                        For CC26xx: HCI_EXT_TX_POWER_MINUS_21_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_18_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_15_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_12_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_9_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_6_DBM,
 *                                    HCI_EXT_TX_POWER_MINUS_3_DBM,
 *                                    HCI_EXT_TX_POWER_0_DBM,
 *                                    HCI_EXT_TX_POWER_1_DBM,
 *                                    HCI_EXT_TX_POWER_2_DBM,
 *                                    HCI_EXT_TX_POWER_3_DBM,
 *                                    HCI_EXT_TX_POWER_4_DBM,
 *                                    HCI_EXT_TX_POWER_5_DBM
 *
 * output parameters
 *
 * @param       cmdComplete - Boolean to indicate the command is still pending.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_SetMaxDtmTxPower( uint8 txPower );


/*******************************************************************************
 * @fn          LL_EXT_MapPmIoPort Vendor Specific API
 *
 * @brief       This function is used to configure and map a CC254x I/O Port as
 *              a General Purpose I/O (GPIO) output signal that reflects the
 *              Power Management (PM) state of the CC254x device. The GPIO
 *              output will be High on Wake, and Low upon entering Sleep. This
 *              feature can be disabled by specifying LL_EXT_PM_IO_PORT_NONE for
 *              the ioPort (ioPin is then ignored). The system default value
 *              upon hardware reset is disabled. This command can be used to
 *              control an external DC-DC Converter (its actual intent) such has
 *              the TI TPS62730 (or any similar converter that works the same
 *              way). This command should be used with extreme care as it will
 *              override how the Port/Pin was previously configured! This
 *              includes the mapping of Port 0 pins to 32kHz clock output,
 *              Analog I/O, UART, Timers; Port 1 pins to Observables, Digital
 *              Regulator status, UART, Timers; Port 2 pins to an external 32kHz
 *              XOSC. The selected Port/Pin will be configured as an output GPIO
 *              with interrupts masked. Careless use can result in a
 *              reconfiguration that could disrupt the system. It is therefore
 *              the user's responsibility to ensure the selected Port/Pin does
 *              not cause any conflicts in the system.
 *
 *              Note: Only Pins 0, 3 and 4 are valid for Port 2 since Pins 1
 *                    and 2 are mapped to debugger signals DD and DC.
 *
 *              Note: Port/Pin signal change will only occur when Power Savings
 *                    is enabled.
 *
 * input parameters
 *
 * @param       ioPort - LL_EXT_PM_IO_PORT_P0,
 *                       LL_EXT_PM_IO_PORT_P1,
 *                       LL_EXT_PM_IO_PORT_P2,
 *                       LL_EXT_PM_IO_PORT_NONE
 *
 * @param       ioPin  - LL_EXT_PM_IO_PORT_PIN0,
 *                       LL_EXT_PM_IO_PORT_PIN1,
 *                       LL_EXT_PM_IO_PORT_PIN2,
 *                       LL_EXT_PM_IO_PORT_PIN3,
 *                       LL_EXT_PM_IO_PORT_PIN4,
 *                       LL_EXT_PM_IO_PORT_PIN5,
 *                       LL_EXT_PM_IO_PORT_PIN6,
 *                       LL_EXT_PM_IO_PORT_PIN7
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER,
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED
 */
extern llStatus_t LL_EXT_MapPmIoPort( uint8 ioPort,
                                      uint8 ioPin );


/*******************************************************************************
 * @fn          LL_EXT_DisconnectImmed Vendor Specific API
 *
 * @brief       This function is used to disconnect the connection immediately.
 *
 *              Note: The connection (if valid) is immediately terminated
 *                    without notifying the remote device. The Host is still
 *                    notified.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID on which to send this data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_EXT_DisconnectImmed( uint16 connId );


/*******************************************************************************
 * @fn          LL_EXT_PacketErrorRate Vendor Specific API
 *
 * @brief       This function is used to Reset or Read the Packet Error Rate
 *              counters for a connection. When Reset, the counters are cleared;
 *              when Read, the total number of packets received, the number of
 *              packets received with a CRC error, the number of events, and the
 *              number of missed events are returned via a callback.
 *
 *              Note: The counters are only 16 bits. At the shortest connection
 *                    interval, this provides a bit over 8 minutes of data.
 *
 * input parameters
 *
 * @param       connId  - The LL connection ID on which to send this data.
 * @param       command - LL_EXT_PER_RESET, LL_EXT_PER_READ
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_EXT_PacketErrorRate( uint16 connId,
                                          uint8  command );


/*******************************************************************************
 * @fn          LL_EXT_PERbyChan Vendor Specific API
 *
 * @brief       This API is called by the HCI to start or end Packet Error Rate
 *              by Channel counter accumulation for a connection. If the
 *              pointer is not NULL, it is assumed there is sufficient memory
 *              for the PER data, per the type perByChan_t. If NULL, then
 *              the operation is considered disabled.
 *
 *              Note: It is the user's responsibility to make sure there is
 *                    sufficient memory for the data, and that the counters
 *                    are cleared prior to first use.
 *
 *              Note: The counters are only 16 bits. At the shortest connection
 *                    interval, this provides a bit over 8 minutes of data.
 *
 * input parameters
 *
 * @param       connId    - The LL connection ID on which to send this data.
 * @param       perByChan - Pointer to PER by Channel data, or NULL.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
extern llStatus_t LL_EXT_PERbyChan( uint16       connId,
                                    perByChan_t *perByChan );


/*******************************************************************************
 * @fn          LL_EXT_ExtendRfRange Vendor Specific API
 *
 * @brief       This function is used to Extend Rf Range using the TI CC2590
 *              2.4 GHz RF Front End device.
 *
 * input parameters
 *
 * @param       cmdComplete - Pointer to get indication if command is done.
 *
 * output parameters
 *
 * @param       cmdComplete - Boolean to indicate the command is still pending.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EXT_ExtendRfRange( uint8 *cmdComplete );


/*******************************************************************************
 * @fn          LL_EXT_HaltDuringRf Vendor Specific API
 *
 * @brief       This function is used to enable or disable halting the
 *              CPU during RF. The system defaults to enabled.
 *
 * input parameters
 *
 * @param       mode - LL_EXT_HALT_DURING_RF_ENABLE,
 *                     LL_EXT_HALT_DURING_RF_DISABLE
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_COMMAND_DISALLOWED,
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_HaltDuringRf( uint8 mode );


/*******************************************************************************
 * @fn          LL_EXT_AdvEventNotice Vendor Specific API
 *
 * @brief       This API is called to enable or disable a notification to the
 *              specified task using the specified task event whenever a Adv
 *              event ends. A non-zero taskEvent value is taken to be "enable",
 *              while a zero valued taskEvent is taken to be "disable".
 *
 * input parameters
 *
 * @param       taskID    - User's task ID.
 * @param       taskEvent - User's task event.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_AdvEventNotice( uint8  taskID,
                                         uint16 taskEvent );


/*******************************************************************************
 * @fn          LL_EXT_ScanEventNotice Vendor Specific API
 *
 * @brief       This API is called to enable or disable a notification to the
 *              specified task using the specified task event whenever a Scan
 *              event ends. A non-zero taskEvent value is taken to be "enable",
 *              while a zero valued taskEvent is taken to be "disable".
 *
 * input parameters
 *
 * @param       taskID    - User's task ID.
 * @param       taskEvent - User's task event.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_ScanEventNotice( uint8  taskID,
                                          uint16 taskEvent );


/*******************************************************************************
 * @fn          LL_EXT_ConnEventNotice Vendor Specific API
 *
 * @brief       This API is called to enable or disable a notification to the
 *              specified task using the specified task event whenever a
 *              Connection event ends. A non-zero taskEvent value is taken to
 *              be "enable", while a zero valued taskEvent is taken to be
 *              "disable".
 *
 *  Note: This command does not check if the taskID and/or taskEvent are valid!
 *
 * input parameters
 *
 * @param       connId    - The LL connection ID for connection event notice.
 * @param       taskID    - User's task ID.
 * @param       taskEvent - User's task event.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_INACTIVE_CONNECTION,
 *              LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_ConnEventNotice( uint16 connId,
                                          uint8  taskID,
                                          uint16 taskEvent );


/*******************************************************************************
 * @fn          LL_EXT_BuildRevision Vendor Specific API
 *
 * @brief       This API is used to to set a user revision number or read the
 *              build revision number.
 *
 * input parameters
 *
 * @param       mode       - LL_EXT_SET_USER_REVISION |
 *                           LL_EXT_READ_BUILD_REVISION
 * @param       userRevNum - A 16 bit value the user can set as their own
 *                           revision number
 *
 * output parameters
 *
 * @param       buildRev   - Pointer to returned build revision, if any.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_BuildRevision( uint8   mode,
                                        uint16  userRevNum,
                                        uint8  *buildRev );


// ROM WORKAROUND - REMOVE FOR NEXT ROM FREEZE
/*******************************************************************************
 * @fn          LL_EXT_DelaySleep Vendor Specific API
 *
 * @brief       This API is used to to set the sleep delay.
 *
 * input parameters
 *
 * @param       delay - 0 .. 1000, in milliseconds.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_DelaySleep( uint16 delay );


/*******************************************************************************
 * @fn          LL_EXT_ResetSystem Vendor Specific API
 *
 * @brief       This API is used to to issue a soft or hard system reset.
 *
 * input parameters
 *
 * @param       mode - LL_EXT_RESET_SYSTEM_HARD | LL_EXT_RESET_SYSTEM_SOFT
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_BAD_PARAMETER
 */
extern llStatus_t LL_EXT_ResetSystem( uint8 mode );


/*******************************************************************************
 * @fn          LL_EXT_LLTestMode Vendor Specific API
 *
 * @brief       This API is used to send a LL Test Mode test case.
 *
 * input parameters
 *
 * @param       testCase - See list of defines in ll_common.h.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EXT_LLTestMode( uint8 testCase );


/*******************************************************************************
 * @fn          LL_EXT_OverlappedProcessing Vendor Specific API
 *
 * @brief       This API is used to enable or disable overlapped processing.
 *
 * input parameters
 *
 * @param       mode - LL_EXT_ENABLE_OVERLAPPED_PROCESSING |
 *                     LL_EXT_DISABLE_OVERLAPPED_PROCESSING
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS
 */
extern llStatus_t LL_EXT_OverlappedProcessing( uint8 mode );


/*******************************************************************************
 * @fn          LL_EXT_NumComplPktsLimit Vendor Specific API
 *
 * @brief       This API is used to set the minimum number of
 *              completed packets which must be met before a Number of
 *              Completed Packets event is returned. If the limit is not
 *              reach by the end of the connection event, then a Number of
 *              Completed Packets event will be returned (if non-zero) based
 *              on the flushOnEvt flag.
 *
 * input parameters
 *
 * @param       limit      - From 1 to LL_MAX_NUM_DATA_BUFFERS.
 * @param       flushOnEvt - LL_EXT_DISABLE_NUM_COMPL_PKTS_ON_EVENT |
 *                           LL_EXT_ENABLE_NUM_COMPL_PKTS_ON_EVENT
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS, LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS
 */
extern llStatus_t LL_EXT_NumComplPktsLimit( uint8 limit,
                                            uint8 flushOnEvt );


/*******************************************************************************
 * @fn          LL_EXT_GetConnInfo Vendor Specific API
 *
 * @brief       This API is used to get connection related information, which
 *              includes the number of allocated connections, the number of
 *              active connections, and for each active connection, the
 *              connection ID, the connection role (Master or Slave), the peer
 *              address and peer address type. The number of allocated
 *              connections is based on a default build value that can be
 *              changed using MAX_NUM_BLE_CONNS. The number of active
 *              connections refers to active BLE connections.
 *
 * Note: If all the parameters are NULL, then the command is assumed to have
 *       originated from the transport layer. Otherwise, they are assumed to
 *       have originated from a direct call by the Application and any
 *       non-NULL pointer will be directly used.
 *
 * input parameters
 *
 * @param       numAllocConns  - Pointer for number of build time connections.
 * @param       numActiveConns - Pointer for number of active BLE connections.
 * @param       activeConnInfo - Pointer for active connection information.
 *
 * output parameters
 *
 * @param       numAllocConns  - Number of build time connections allowed.
 * @param       numActiveConns - Number of active BLE connections.
 * @param       activeConnInfo - Active connection information.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern llStatus_t LL_EXT_GetConnInfo( uint8 *numAllocConns,
                                      uint8 *numActiveConns,
                                      uint8 *activeConnInfo );


/*******************************************************************************
 * @fn          LL_EXT_SetMaxDataLen API
 *
 * @brief       This LL API is used to set the maximum supported Tx and Rx
 *              Octets (in bytes) and Time (in us).
 *
 *              Note: Any parameter not within a valid range will leave the
 *                    corresponding supportedMax value unchanged.
 *
 * input parameters
 *
 * @param       txOctets - Value of maximum supported Tx octets (in bytes).
 * @param       txTime   - Value of maximum supported Tx time (in us).
 * @param       rxOctets - Value of maximum supported Rx octets (in bytes).
 * @param       rxTime   - Value of maximum supported Rx time (in us).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_EXT_SetMaxDataLen( uint16 txOctets,
                                        uint16 txTime,
                                        uint16 rxOctets,
                                        uint16 rxTime );


/*******************************************************************************
 * @fn          LL_EXT_ScanReqReport API
 *
 * @brief       This LL API is used to enable/disable Scan Request Reports.
 *
 * input parameters
 *
 * @param       mode - LL_EXT_DISABLE_SCAN_REQUEST_REPORT |
 *                     LL_EXT_ENABLE_SCAN_REQUEST_REPORT
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      llStatus_t
 */
extern llStatus_t LL_EXT_ScanReqReport( uint8 mode );


/*
**  LL Callbacks to HCI
*/

/*******************************************************************************
 * @fn          LL_ConnectionCompleteCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              a new connection has been created. For the Slave, this means
 *              a CONNECT_REQ message was received from an Initiator. For the
 *              Master, this means a CONNECT_REQ message was sent in response
 *              to a directed or undirected message addressed to the Initiator.
 *
 * input parameters
 *
 * @param       reasonCode    - LL_STATUS_SUCCESS or ?
 * @param       connId        - The LL connection ID for new connection.
 * @param       role          - LL_LINK_CONNECT_COMPLETE_MASTER or
 *                              LL_LINK_CONNECT_COMPLETE_SLAVE.
 * @param       peerAddrType  - Peer address type (public or random).
 * @param       peerAddr      - Peer address.
 * @param       connInterval  - Connection interval.
 * @param       slaveLatency  - The connection's Slave Latency.
 * @param       connTimeout   - The connection's Supervision Timeout.
 * @param       clockAccuracy - The sleep clock accurracy of the Master. Only
 *                              valid on the Slave. Set to 0x00 for the Master.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_ConnectionCompleteCback( uint8   reasonCode,
                                        uint16  connId,
                                        uint8   role,
                                        uint8   peerAddrType,
                                        uint8  *peerAddr,
                                        uint16  connInterval,
                                        uint16  slaveLatency,
                                        uint16  connTimeout,
                                        uint8   clockAccuracy );


/*******************************************************************************
 * @fn          LL_EnhancedConnectionCompleteCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              a new connection has been created. For the Slave, this means
 *              a CONNECT_REQ message was received from an Initiator. For the
 *              Master, this means a CONNECT_REQ message was sent in response
 *              to a directed or undirected message addressed to the Initiator.
 *              The Enhanced Connection Complete event now also supports
 *              prvoiding the the Host with the Local and Peer RPA, if
 *              applicable, and if supported, replaces LL Connection Complete.
 *
 * input parameters
 *
 * @param       reasonCode    - LL_STATUS_SUCCESS or ?
 * @param       connId        - The LL connection ID for new connection.
 * @param       role          - LL_LINK_CONNECT_COMPLETE_MASTER or
 *                              LL_LINK_CONNECT_COMPLETE_SLAVE.
 * @param       peerAddrType  - Peer address type (public or random).
 * @param       peerAddr      - Peer address.
 * @param       localRPA      - Local RPA.
 * @param       peerRPA       - Peer RPA.
 * @param       connInterval  - Connection interval.
 * @param       slaveLatency  - The connection's Slave Latency.
 * @param       connTimeout   - The connection's Supervision Timeout.
 * @param       clockAccuracy - The sleep clock accurracy of the Master. Only
 *                              valid on the Slave. Set to 0x00 for the Master.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EnhancedConnectionCompleteCback( uint8   reasonCode,
                                                uint16  connId,
                                                uint8   role,
                                                uint8   peerAddrType,
                                                uint8  *peerAddr,
                                                uint8  *localRPA,
                                                uint8  *peerRPA,
                                                uint16  connInterval,
                                                uint16  slaveLatency,
                                                uint16  connTimeout,
                                                uint8   clockAccuracy );


/*******************************************************************************
 * @fn          LL_DisconnectCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              the connection has been terminated. The cause is given by the
 *              reason code.
 *
 * input parameters
 *
 * @param       connId - The LL connection ID.
 * @param       reason - The reason the connection was terminated.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_DisconnectCback( uint16 connId,
                                uint8  reason );


/*******************************************************************************
 * @fn          LL_ConnParamUpdateCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              the update parameters control procedure has completed. It is
 *              always made to the Master's Host when the update request has
 *              been sent. It is only made to the Slave's Host when the update
 *              results in a change to the connection interval, and/or the
 *              connection latency, and/or the connection timeout. It is also
 *              possible to get this event during a Connection Parameters
 *              Request control procedure.
 *
 * input parameters
 *
 * @param       status       - Status of update complete event.
 * @param       connId       - The LL connection ID.
 * @param       connInterval - Connection interval.
 * @param       connLatency  - The connection's Slave Latency.
 * @param       connTimeout  - The connection's Supervision Timeout.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#if defined(CTRL_V41_CONFIG) && (CTRL_V41_CONFIG & CONN_PARAM_REQ_CFG)
extern void LL_ConnParamUpdateCback( llStatus_t status,
#else
extern void LL_ConnParamUpdateCback(
#endif // CTRL_V41_CONFIG=CONN_PARAM_REQ_CFG
                                     uint16     connId,
                                     uint16     connInterval,
                                     uint16     connLatency,
                                     uint16     connTimeout );


/*******************************************************************************
 * @fn          LL_ReadRemoteVersionInfoCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host the
 *              requested peer's Version information.
 *
 * input parameters
 *
 * @param       status    - Status of callback.
 * @param       connId    - The LL connection ID.
 * @param       verNum    - Version of the Bluetooth Controller specification.
 * @param       comId     - Company identifier of the manufacturer of the
 *                          Bluetooth Controller.
 * @param       subverNum - A unique value for each implementation or revision
 *                          of an implementation of the Bluetooth Controller.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_ReadRemoteVersionInfoCback( uint8  status,
                                           uint16 connId,
                                           uint8  verNum,
                                           uint16 comId,
                                           uint16 subverNum );


/*******************************************************************************
 * @fn          LL_EncChangeCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              an encryption change has taken place. This results when
 *              the host performs a LL_StartEncrypt when encryption is not
 *              already enabled.
 *
 *              Note: If the key request was rejected, then encryption will
 *                    remain off.
 *
 * input parameters
 *
 * @param       connId  - The LL connection ID for new connection.
 * @param       reason  - LL_ENC_KEY_REQ_ACCEPTED or LL_ENC_KEY_REQ_REJECTED.
 * @param       encEnab - LL_ENCRYPTION_OFF or LL_ENCRYPTION_ON.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EncChangeCback( uint16 connId,
                               uint8  reason,
                               uint8  encEnab );


/*******************************************************************************
 * @fn          LL_EncKeyRefreshCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              an encryption key change has taken place. This results when
 *              the host performs a LL_StartEncrypt when encryption is already
 *              enabled.
 *
 * input parameters
 *
 * @param       connId  - The LL connection ID for new connection.
 * @param       reason  - LL_ENC_KEY_REQ_ACCEPTED.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EncKeyRefreshCback( uint16 connId,
                                   uint8  reason );


/*******************************************************************************
 * @fn          LL_AdvReportCback Callback
 *
 * @brief       This Callback is used by the LL to provide information about
 *              advertisers from which an advertising packet was received.
 *
 * input parameters
 *
 * @param       eventType   - Type of advertisement packet received by Scanner
 *                            or Initiator, and scan response for Initiator.
 * @param       advAddrType - Advertiser address type (public or random).
 * @param       advAddr     - Advertiser address.
 * @param       dataLen     - Size in bytes of advertisement packet.
 * @param       data        - Advertisement data.
 * @param       rssi        - RSSI value (-127..20dBm), or not available
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_AdvReportCback( uint8  eventType,
                               uint8  advAddrType,
                               uint8 *advAddr,
                               uint8  dataLen,
                               uint8 *data,
                               int8   rssi );


/*******************************************************************************
 * @fn          LL_ReadRemoteUsedFeaturesCompleteCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the Host that
 *              the Read Remote Feature Support command as completed.
 *
 * input parameters
 *
 * @param       status      - SUCCESS or control procedure timeout.
 * @param       connId      - The LL connection ID for new connection.
 * @param       featureSet  - A pointer to the Feature Set.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_ReadRemoteUsedFeaturesCompleteCback( uint8   status,
                                                    uint16  connId,
                                                    uint8  *featureSet );


/*******************************************************************************
 * @fn          LL_EncLtkReqCback Callback
 *
 * @brief       This Callback is used by the LL to provide to the Host the
 *              Master's random number and encryption diversifier, and to
 *              request the Host's Long Term Key (LTK).
 *
 * input parameters
 *
 * @param       connId  - The LL connection ID for new connection.
 * @param       randNum - Random vector used in device identification.
 * @param       encDiv  - Encrypted diversifier.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EncLtkReqCback( uint16  connId,
                               uint8  *randNum,
                               uint8  *encDiv );


/*******************************************************************************
 * @fn          LL_DirectTestEndDone Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the
 *              Direct Test End command has completed.
 *
 *
 * input parameters
 *
 * @param       numPackets - The number of packets received. Zero for transmit.
 * @param       mode       - LL_DIRECT_TEST_MODE_TX or LL_DIRECT_TEST_MODE_RX.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 */
extern void LL_DirectTestEndDoneCback( uint16 numPackets,
                                       uint8  mode );


/*******************************************************************************
 * @fn          LL_TxDataCompleteCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the HCI that
 *              the HCI's buffer is free for its own use again.
 *
 * input parameters
 *
 * @param       connId   - The LL connection ID on which to send this data.
 * @param       *pBuf    - A pointer to the data buffer to transmit, or NULL.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 ******************************************************************************/
extern void LL_TxDataCompleteCback( uint16  connId,
                                    uint8  *pBuf );


/*******************************************************************************
 * @fn          LL_RxDataCompleteCback Callback
 *
 * @brief       This Callback is used by the LL to indicate to the HCI that
 *              data has been received and placed in the buffer provided by
 *              the HCI.
 *
 * input parameters
 *
 * @param       connId   - The LL connection ID on which data was received.
 * @param       *pBuf    - A pointer to the receive data buffer provided by
 *                           the HCI.
 * @param       len      - The number of bytes received on this connection.
 * @param       fragFlag - LL_DATA_FIRST_PKT indicates buffer is the start of
 *                           a Host packet.
 *                         LL_DATA_CONTINUATION_PKT: Indicates buffer is a
 *                           continuation of a Host packet.
 * @param       rssi     - The RSSI of this received packet as a signed byte.
 *                         Range: -127dBm..+20dBm, 127=Not Available.
 *
 * output parameters
 *
 * @param       **pBuf   - A double pointer updated to the next receive data
 *                         buffer, or NULL if no next buffer is available.
 *
 * @return      None.
 */
extern void LL_RxDataCompleteCback( uint16  connId,
                                    uint8  *ppBuf,
                                    uint8   len,
                                    uint8   fragFlag,
                                    int8    rssi );


/*******************************************************************************
 * @fn          LL_RandCback API
 *
 * @brief       This Callback is used by the LL to notify the HCI that the true
 *              random number command has been completed.
 *
 *              Note: The length is always given by B_RANDOM_NUM_SIZE.
 *
 * input parameters
 *
 * @param       *randData - Pointer to buffer to place a random block of data.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_RandCback( uint8 *randData );


/*******************************************************************************
 * @fn          LL_EXT_SetRxGainCback Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the set
 *              RX gain command has been completed.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EXT_SetRxGainCback( void );


/*******************************************************************************
 * @fn          LL_EXT_SetTxPowerCback Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the set
 *              TX power command has been completed.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EXT_SetTxPowerCback( void );


/*******************************************************************************
 * @fn          LL_EXT_PacketErrorRateCback Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the
 *              Packet Error Rate Read command has been completed.
 *
 *              Note: The counters are only 16 bits. At the shortest connection
 *                    interval, this provides a bit over 8 minutes of data.
 *
 * input parameters
 *
 * @param       numPkts   - Number of Packets received.
 * @param       numCrcErr - Number of Packets received with a CRC error.
 * @param       numEvents - Number of Connection Events.
 * @param       numPkts   - Number of Missed Connection Events.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EXT_PacketErrorRateCback( uint16 numPkts,
                                         uint16 numCrcErr,
                                         uint16 numEvents,
                                         uint16 numMissedEvts );


/*******************************************************************************
 * @fn          LL_EXT_ExtendRfRangeCback Callback
 *
 * @brief       This Callback is used by the LL to notify the HCI that the
 *              Extend Rf Range command has been completed.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EXT_ExtendRfRangeCback( void );


/*******************************************************************************
 * @fn          LL_AuthPayloadTimeoutExpiredCback Callback
 *
 * @brief       This LL callback is used to generate an Authenticated Payload
 *              Timeout event when the APTO expires.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_AuthPayloadTimeoutExpiredCback( uint16 connId );


/*******************************************************************************
 * @fn          LL_RemoteConnParamReqCback Callback
 *
 * @brief       This LL callback is used to generate a Remote Connection
 *              Parameter Request meta event to provide to the Host the peer's
 *              connection parameter request parameters (min connection
 *              interval, max connection interval, slave latency, and connection
 *              timeout), and to request the Host's acceptance or rejection of
 *              this parameters.
 *
 * input parameters
 *
 * @param       connHandle   - Connection handle.
 * @param       Interval_Min - Lower limit for connection interval.
 * @param       Interval_Max - Upper limit for connection interval.
 * @param       Latency      - Slave latency.
 * @param       Timeout      - Connection timeout.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_RemoteConnParamReqCback( uint16 connHandle,
                                        uint16 Interval_Min,
                                        uint16 Interval_Max,
                                        uint16 Latency,
                                        uint16 Timeout );


/*******************************************************************************
 * @fn          LL_PhyUpdateCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate that the Controller has
 *              has changed the transmitter or receiver PHY in use or that the
 *              LL_SetPhy command has failed.
 *
 * input parameters
 *
 * @param       status     - Status of LL_SetPhy command.
 * @param       connHandle - Connection handle.
 * @param       txPhy      - Bit map of PHY used for Tx.
 * @param       rxPhy      - Bit map of PHY used for Rx.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_PhyUpdateCompleteEventCback( llStatus_t status,
                                            uint16     connHandle,
                                            uint8      txPhy,
                                            uint8      rxPhy );


/*******************************************************************************
 * @fn          LL_DataLengthChangeEventCback Callback
 *
 * @brief       This LL callback is used to indicate that the Controller has
 *              has changed the either the maximum payload length or the
 *              maximum transmit time of data channel PDUs in either direction.
 *              The values reported are the maximum taht will actually be used
 *              on the connection following the change.
 *
 * input parameters
 *
 * @param       connHandle  - Connection handle.
 * @param       maxTxOctets - Maximum number of transmit payload bytes.
 * @param       maxTxTime   - Maximum transmit time.
 * @param       maxRxOctets - Maximum number of receive payload bytes.
 * @param       maxRxTime   - Maximum receive time.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_DataLengthChangeEventCback( uint16 connHandle,
                                           uint16 maxTxOctets,
                                           uint16 maxTxTime,
                                           uint16 maxRxOctets,
                                           uint16 maxRxTime );


/*******************************************************************************
 * @fn          LL_ReadLocalP256PublicKeyCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate the Controller has
 *              completed the generation of the P256 public key.
 *
 * input parameters
 *
 * @param       status  - Operation status.
 * @param       p256KeyX - P256 public key (first 32 bytes X=0..31).
 * @param       p256KeyY - P256 public key (second 32 bytes Y=32..63).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_ReadLocalP256PublicKeyCompleteEventCback( uint8  status,
                                                         uint8 *p256KeyX,
                                                         uint8 *p256KeyY );

/*******************************************************************************
 * @fn          LL_GenerateDHKeyCompleteEventCback Callback
 *
 * @brief       This LL callback is used to indicate teh Controller has
 *              completed the generation of the Diffie Hellman key.
 *
 * input parameters
 *
 * @param       status - Operation status.
 * @param       dhKey  - Diffie Hellman key (32 bytes).
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_GenerateDHKeyCompleteEventCback( uint8  status,
                                                uint8 *dhKey );

/*******************************************************************************
 * @fn          LL_EXT_ScanReqReportCback Callback
 *
 * @brief       This LL callback is used to generate a vendor specific Scan
 *              Request Report meta event when an Scan Request is received by
 *              an Advertiser.
 *
 * input parameters
 *
 * @param       peerAddrType - Peer address type.
 * @param       peerAddr     - Peer address.
 * @param       ownAddrType  - Own address type.
 * @param       ownAddr      - Own address.
 * @param       chan         - LL_ADV_CHAN_37 | LL_ADV_CHAN_38  | LL_ADV_CHAN_39
 * @param       rssi         - The RSSI of received packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
extern void LL_EXT_ScanReqReportCback( uint8  peerAddrType,
                                       uint8 *peerAddr,
                                       uint8  ownAddrType,
                                       uint8 *ownAddr,
                                       uint8  chan,
                                       int8   rssi );

#ifdef __cplusplus
}
#endif

#endif /* LL_H */

