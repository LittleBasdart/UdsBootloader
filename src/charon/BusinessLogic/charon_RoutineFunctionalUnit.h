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
 * @file charon_RoutineFunctionalUnit.h
 * Module specifies the services of remote activation of routines, as they shall be implemented in servers and client.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_ROUTINEFUNCTIONALUNIT_H_
#define CHARON_ROUTINEFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "Common/charon_types.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x31
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_RoutineFunctionalUnit_RoutineControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_ROUTINEFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/