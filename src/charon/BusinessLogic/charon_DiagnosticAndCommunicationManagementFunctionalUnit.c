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
 * @addtogroup BusinessLogic Business Logic
 * @{
 * @file charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
 * Implementation of the DCM Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "HSDI/charon_interface_debug.h"
#include "Common/charon_negativeResponse.h"
#include <stdbool.h>

/* Imports *******************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/**
 * Container Type for Timings
 */
typedef struct DefaultSessionTimings_t_private
{
    uint32_t p2;            /**< Response Timeout */
    uint32_t p2star;        /**< Extended Response Timing */
} DefaultSessionTimings_t;

/* Constants *****************************************************************/

/**
 * Default timings for each session.
 * Sorted for their session id. Values are in milliseconds, for both parameters.
 */
// todo: these timing values are example values from iso 14229-2 chapter 7.2 table 4
// change these as necessary
static const DefaultSessionTimings_t defaultTimings[charon_sscType_amount] =
{
        {50,5000},      /* Default */
        {50,5000},      /* Programming */
        {50,5000},      /* Extended */
        {50,5000}       /* Security */
};

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void charon_DiagnosticAndCommunicationManagementFunctionalUnit_reset (void)
{

}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;
    PACKED_STRUCT(anonym)
    {
        uint8_t sid;
        uint8_t session;
        uint16_t p2;
        uint16_t p2star;
    } transmitBuffer;

    if (receiveBufferSize != 2u)
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        uint8_t session = receiveBuffer[1] & 0x7Fu;
        uint8_t responseSuppress = receiveBuffer[1] & 0x80u;
        if ( (session >= (uint8_t)charon_sscType_amount) || (session == (uint8_t)charon_sscType_invalid) )
        {
            CHARON_WARNING("Session 0x%x unknown or invalid!", session);
            result = uds_responseCode_SubfunctionNotSupported;
        }
        else
        {
            CHARON_INFO("Changing Session to 0x%x.", session);

            if (responseSuppress == 0u)
            {
                transmitBuffer.sid = (uint8_t)uds_sid_DiagnosticSessionControl | (uint8_t)uds_sid_PositiveResponseMask;
                transmitBuffer.session = session;
                transmitBuffer.p2 =  (uint16_t)defaultTimings[session].p2;
                transmitBuffer.p2star = (uint16_t)(defaultTimings[session].p2star/10u);
                charon_sscTxMessage((uint8_t*)&transmitBuffer, sizeof(transmitBuffer));
            }
            charon_sscSetSession((charon_sessionTypes_t)session, defaultTimings[session].p2, defaultTimings[session].p2star);
        }
    }
    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_DiagnosticSessionControl);
    }
    return result;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("ECU Reset Service SID:0x11 Triggered");
    extern bool uds_reset_request;
    uds_reset_request = true;
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Security Access SID:0x27 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Com Control Service SID:0x28 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize != 2u)
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (receiveBuffer[1] & 0x7Fu) != 0u)
    {
        CHARON_ERROR("Subfunction is not 0x00 or 0x80.");
        result = uds_responseCode_SubfunctionNotSupported;
    }
    else
    {
        charon_sscTesterPresentHeartbeat();
        if ( (receiveBuffer[1] & 0x80u) == 0u)
        {
            uint8_t transmitBuffer[2] = {(uint8_t)uds_sid_TesterPresent | (uint8_t)uds_sid_PositiveResponseMask, 0u};
            charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
        }
    }

    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_TesterPresent);
    }

    CHARON_INFO("Tester Present Service SID:0x3E Triggered");
    return result;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Access Timing Parameter Service SID:0x83 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Secured Data Transmission Service SID:0x84 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Control DTC Setting Service SID:0x85 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Response On Event Service SID:0x86 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    CHARON_INFO("Link Control Service SID:0x87 Triggered");
    return uds_responseCode_ServiceNotSupported;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
