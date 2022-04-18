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
 * @addtogroup Common
 * @{
 * @file charon_negativeResponse.c
 * This Module handles the Transfer of the negative Response message.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Interfaces ****************************************************************/

#include "charon_negativeResponse.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "ComLogic/charon_ServiceLookupTable.h"
#include "HSDI/charon_interface_debug.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void charon_sendNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid)
{
    CHARON_INFO("Sending error message %s (0x%x) for SID %s (0x%x)", charon_ServiceLookupTable_getNameForReturnCode(ResponseCode), (uint8_t)ResponseCode, charon_ServiceLookupTable_getNameForServiceSid((uint8_t)RequestedSid), (uint8_t)RequestedSid);
    uint8_t transmitBuffer[3] = {(uint8_t)uds_sid_NegativeResponse, (uint8_t)RequestedSid, (uint8_t)ResponseCode};
    charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
