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
 * @file charon_DiagnosticAndCommunicationManagementFunctionalUnit.h
 * Module handles Service Group for DCM
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_
#define CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_

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
void charon_DiagnosticAndCommunicationManagementFunctionalUnit_reset (void);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x10
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x11
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x27
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x28
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x3E
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x83
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x84
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x85
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x86
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x87
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
