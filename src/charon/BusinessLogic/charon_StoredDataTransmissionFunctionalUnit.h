/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @file charon_StoredDataTransmissionFunctionalUnit.h
 * Module handles Service Group the Stored Data Transmission Unit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_
#define CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

/* Imports *******************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces  ***************************************************************/

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x14
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @param transmitBuffer Payload
 * @param transmitBufferSize Payload Size
 * @return uint32_t
 */
uint32_t charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x19
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @param transmitBuffer Payload
 * @param transmitBufferSize Payload Size
 * @return uint32_t
 */
uint32_t charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/