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
 * @addtogroup BusinessLogic Business Logic
 * @{
 * @file charon_RoutineFunctionalUnit.c
 * Implementation of the Routine functional Unit Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "HSDI/charon_interface_NvmDriver.h"
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

uds_responseCode_t charon_RoutineFunctionalUnit_RoutineControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize < 4u)
    {
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (receiveBuffer[1] == 0u) || (receiveBuffer[1] > 3u) )
    {
        result = uds_responseCode_SubfunctionNotSupported;
    }
    else
    {
        uint16_t routineIdentifier = ((uint16_t)receiveBuffer[2] << 8) | receiveBuffer[3];
        if ( (routineIdentifier == 0xFF00u) && (receiveBuffer[1] == 1u) )
        {
            charon_NvmDriver_erase();
        }
        else
        {
            result = uds_responseCode_RequestOutOfRange;
        }
    }

    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_RoutineControl);
    }
    else
    {
        uint8_t transmitBuffer[4] = {
                receiveBuffer[0] | (uint8_t)uds_sid_PositiveResponseMask,
                receiveBuffer[1],
                receiveBuffer[2],
                receiveBuffer[3]
        };
        charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
    }
    return result;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
