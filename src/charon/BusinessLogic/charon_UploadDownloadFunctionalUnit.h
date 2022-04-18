/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2020 Florian Kaup
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/**
 * @addtogroup CharonUDS
 * @{
 * @defgroup BusinessLogic Business Logic
 * @{
 * @file charon_UploadDownloadFunctionalUnit.h
 * Module handles Service Group for Upload Download
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_
#define CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "Common/charon_types.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * Resets all internal Variables and
 * stops all ongoing Services.
 */
void charon_UploadDownloadFunctionalUnit_reset (void);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x34
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestDownload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x35
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestUpload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x36
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_UploadDownloadFunctionalUnit_TransferData (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x37
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestTransferExit (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x38
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestFileTransfer (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#ifdef TEST

void charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress);

void charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength);

void charon_UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection);

void charon_UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter);

uint32_t charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void);

uint32_t charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength (void);

uint8_t charon_UploadDownloadFunctionalUnit_getTransferDirection (void);

uint8_t charon_UploadDownloadFunctionalUnit_getNextSequenceCounter (void);

#endif

#endif /* CHARON_UPLOADDOWNLOADFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
