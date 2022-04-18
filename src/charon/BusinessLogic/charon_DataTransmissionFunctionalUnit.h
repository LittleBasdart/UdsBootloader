/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Andreas Hofmann
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
 * @file charon_DataTransmissionFunctionalUnit.h
 * Implementation of the DTF Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_
#define CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "Common/charon_types.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x22
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x23
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x24
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2A
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2C
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x2E
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x3D
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_DATATRANSMISSIONFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
