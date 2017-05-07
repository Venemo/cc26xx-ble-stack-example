/******************************************************************************

 @file  osal_memory.h

 @brief This module defines the OSAL memory control functions.

 Group: WCS, LPC, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2004-2016, Texas Instruments Incorporated
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

#ifndef OSAL_MEMORY_H
#define OSAL_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "comdef.h"
 
/*********************************************************************
 * CONSTANTS
 */

#if !defined ( OSALMEM_METRICS )
  #define OSALMEM_METRICS  FALSE
#endif

/*********************************************************************
 * MACROS
 */
  
#define osal_stack_used()  OnBoard_stack_used()

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
 
/*********************************************************************
 * FUNCTIONS
 */

 /*
  * Initialize memory manager.
  */
  void osal_mem_init( void );

 /*
  * Setup efficient search for the first free block of heap.
  */
  void osal_mem_kick( void );

 /*
  * Allocate a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void *osal_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum );
#define osal_mem_alloc(_size ) osal_mem_alloc_dbg(_size, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void *osal_mem_alloc( uint16 size );
#endif /* DPRINTF_OSALHEAPTRACE */
  
 /*
  * Free a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void osal_mem_free_dbg( void *ptr, const char *fname, unsigned lnum );
#define osal_mem_free(_ptr ) osal_mem_free_dbg(_ptr, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void osal_mem_free( void *ptr );
#endif /* DPRINTF_OSALHEAPTRACE */

#if ( OSALMEM_METRICS )
 /*
  * Return the maximum number of blocks ever allocated at once.
  */
  uint16 osal_heap_block_max( void );

 /*
  * Return the current number of blocks now allocated.
  */
  uint16 osal_heap_block_cnt( void );

 /*
  * Return the current number of free blocks.
  */
  uint16 osal_heap_block_free( void );

 /*
  * Return the current number of bytes allocated.
  */
  uint16 osal_heap_mem_used( void );
#endif

#if defined (ZTOOL_P1) || defined (ZTOOL_P2)
 /*
  * Return the highest number of bytes ever used in the heap.
  */
  uint16 osal_heap_high_water( void );
#endif

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OSAL_MEMORY_H */
