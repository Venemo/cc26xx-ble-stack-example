/******************************************************************************

 @file  osal_snv.h

 @brief This module defines the OSAL simple non-volatile memory functions.

 Group: WCS, LPC, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2009-2016, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: ble_sdk_2_02_01_18
 Release Date: 2016-10-26 15:20:04
 *****************************************************************************/

#ifndef OSAL_SNV_H
#define OSAL_SNV_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "hal_types.h"
  
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */
 
#if defined(OSAL_SNV_UINT16_ID)
  typedef uint16 osalSnvId_t;
  typedef uint16 osalSnvLen_t;
#else
  typedef uint8 osalSnvId_t;
  typedef uint8 osalSnvLen_t;
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      osal_snv_init
 *
 * @brief   Initialize NV service.
 *
 * @param   none
 *
 * @return  SUCCESS if initialization succeeds. FAILURE, otherwise.
 */
extern uint8 osal_snv_init(void);

/*********************************************************************
 * @fn      osal_snv_read
 *
 * @brief   Read data from NV.
 *
 * @param   id   - Valid NV item Id.
 * @param   len  - Length of data to read.
 * @param   *pBuf - Data is read into this buffer.
 *
 * @return  SUCCESS if successful.
 *          Otherwise, NV_OPER_FAILED for failure.
 */
extern uint8 osal_snv_read( osalSnvId_t id, osalSnvLen_t len, void *pBuf);

/*********************************************************************
 * @fn      osal_snv_write
 *
 * @brief   Write a data item to NV.
 *
 * @param   id   - Valid NV item Id.
 * @param   len  - Length of data to write.
 * @param   *pBuf - Data to write.
 *
 * @return  SUCCESS if successful, NV_OPER_FAILED if failed.
 */
extern uint8 osal_snv_write( osalSnvId_t id, osalSnvLen_t len, void *pBuf);

/*********************************************************************
 * @fn      osal_snv_ext_write
 *
 * @brief   Write a data item to NV.
 *
 * @param   id   - Valid NV item Id.
 * @param   len  - Length of data to write.
 * @param   *pBuf - Data to write.
 *
 * @return  SUCCESS if successful, NV_OPER_FAILED if failed.
 */
extern uint8 osal_snv_ext_write( osalSnvId_t id, osalSnvLen_t len, void *pBuf);

/*********************************************************************
 * @fn      osal_snv_compact
 *
 * @brief   Compacts NV if its usage has reached a specific threshold.
 *
 * @param   threshold - compaction threshold
 *
 * @return  SUCCESS if successful,
 *          NV_OPER_FAILED if failed, or
 *          INVALIDPARAMETER if threshold invalid.
 */
extern uint8 osal_snv_compact( uint8 threshold );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_SNV.H */
