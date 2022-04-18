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
 * @addtogroup ComLogic
 * @{
 * @file charon_ServiceLookupTable.c
 * Implementation of the Service Lookup Table
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "ComLogic/charon_ServiceLookupTable.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "BusinessLogic/charon_UploadDownloadFunctionalUnit.h"
#include "BusinessLogic/charon_InputOutputControlFunctionalUnit.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

#define SESSION_DEFAULT			((uint32_t) 1u << (uint8_t)charon_sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << (uint8_t)charon_sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << (uint8_t)charon_sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << (uint8_t)charon_sscType_secured)

/* Constants *****************************************************************/

static charon_serviceObject_t serviceLookupTable[] =
        /* SID */                                           /* Allowed Sessions */                                                          /* Service Function */                                                                          /* Encryption */
{       {uds_sid_DiagnosticSessionControl,                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl,             0u},
        {uds_sid_EcuReset,                                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset,                             0u},
        {uds_sid_ClearDiagnosticInformation,                (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadDtcInformation,                        (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadDataByIdentifier,                      (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadMemoryByAddress,                       (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadScalingDataByIdentifier,               (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_SecurityAccess,                            (                                        SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess,                       0u},
        {uds_sid_CommunicationControl,                      (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl,                 0u},
        {uds_sid_ReadDataByPeriodicIdentifier,              (                                        SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_DynamicallyDefineDataIdentifier,           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_WriteDataByIdentifier,                     (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_InputOutputControlByIdentifier,            (                                        SESSION_EXTENDED | SESSION_SECURED),   charon_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier,                         0u},
        {uds_sid_RoutineControl,                            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_RoutineFunctionalUnit_RoutineControl,                                                    0u},
        {uds_sid_RequestDownload,                           (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestDownload,                                            0u},
        {uds_sid_RequestUpload,                             (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestUpload,                                              0u},
        {uds_sid_TransferData,                              (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_TransferData,                                               0u},
        {uds_sid_RequestTransferExit,                       (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestTransferExit,                                        0u},
        {uds_sid_RequestFileTransfer,                       (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestFileTransfer,                                        0u},
        {uds_sid_WriteMemoryByAddress,                      (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_TesterPresent,                             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent,                        0u},
        {uds_sid_AccessTimingParameter,                     (                  SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter,                0u},
        {uds_sid_SecuredDataTransmission,                   (                                                           SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission,              0u},
        {uds_sid_ControlDtcSetting,                         (                                        SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting,                    0u},
        {uds_sid_ResponseOnEvent,                           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent,                      0u},
        {uds_sid_LinkControl,                               (                                        SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl,                          0u},
};

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject (uint8_t sid)
{
    const uint32_t TableSize = (sizeof(serviceLookupTable) / sizeof(serviceLookupTable[0]));
    uint32_t numIterations = 32u - (uint32_t)__builtin_clz(TableSize);
    uint32_t iterationSize = TableSize / 2u;
    uint32_t TableIndex = iterationSize;
    charon_serviceObject_t * result = NULL;

    for (;numIterations > 0u;numIterations--)
    {
        iterationSize = (iterationSize+1u) / 2u;

        uint8_t castedSidEnumeration = (uint8_t)serviceLookupTable[TableIndex].sid;// TODO(Check for Misra at this point)
        if (castedSidEnumeration == sid)
        {
            result = &serviceLookupTable[TableIndex];
            break;
        }
        else if (castedSidEnumeration > sid)
        {
            if (TableIndex != 0u)
            {
                TableIndex -= iterationSize;
            }
        }
        else
        {
            TableIndex += iterationSize;
            if (TableIndex > (TableSize-1u) )
            {
                TableIndex = TableSize-1u;
            }
        }
    }
    return result;
}

const char * charon_ServiceLookupTable_getNameForServiceSid (uint8_t sid)
{
    const char * result = "unknown sid";
    uds_sid_t castedSid = (uds_sid_t)sid;
    switch(castedSid)
    {
    case uds_sid_DiagnosticSessionControl:
        result = "DiagnosticSessionControl";
        break;
    case uds_sid_EcuReset:
        result = "EcuReset";
        break;
    case uds_sid_ClearDiagnosticInformation:
        result = "ClearDiagnosticInformation";
        break;
    case uds_sid_ReadDtcInformation:
        result = "ReadDtcInformation";
        break;
    case uds_sid_ReadDataByIdentifier:
        result = "ReadDataByIdentifier";
        break;
    case uds_sid_ReadMemoryByAddress:
        result = "ReadMemoryByAddress";
        break;
    case uds_sid_ReadScalingDataByIdentifier:
        result = "ReadScalingDataByIdentifier";
        break;
    case uds_sid_SecurityAccess:
        result = "SecurityAccess";
        break;
    case uds_sid_CommunicationControl:
        result = "CommunicationControl";
        break;
    case uds_sid_ReadDataByPeriodicIdentifier:
        result = "ReadDataByPeriodicIdentifier";
        break;
    case uds_sid_DynamicallyDefineDataIdentifier:
        result = "DynamicallyDefineDataIdentifier";
        break;
    case uds_sid_WriteDataByIdentifier:
        result = "WriteDataByIdentifier";
        break;
    case uds_sid_InputOutputControlByIdentifier:
        result = "InputOutputControlByIdentifier";
        break;
    case uds_sid_RoutineControl:
        result = "RoutineControl";
        break;
    case uds_sid_RequestDownload:
        result = "RequestDownload";
        break;
    case uds_sid_RequestUpload:
        result = "RequestUpload";
        break;
    case uds_sid_TransferData:
        result = "TransferData";
        break;
    case uds_sid_RequestTransferExit:
        result = "RequestTransferExit";
        break;
    case uds_sid_RequestFileTransfer:
        result = "RequestFileTransfer";
        break;
    case uds_sid_WriteMemoryByAddress:
        result = "WriteMemoryByAddress";
        break;
    case uds_sid_TesterPresent:
        result = "TesterPresent";
        break;
    case uds_sid_PositiveResponseMask:
        result = "PositiveResponseMask";
        break;
    case uds_sid_NegativeResponse:
        result = "NegativeResponse";
        break;
    case uds_sid_AccessTimingParameter:
        result = "AccessTimingParameter";
        break;
    case uds_sid_SecuredDataTransmission:
        result = "SecuredDataTransmission";
        break;
    case uds_sid_ControlDtcSetting:
        result = "ControlDtcSetting";
        break;
    case uds_sid_ResponseOnEvent:
        result = "ResponseOnEvent";
        break;
    case uds_sid_LinkControl:
        result = "LinkControl";
        break;

    default:
        // nothing to do
        break;
    }
    return result;
}

const char * charon_ServiceLookupTable_getNameForReturnCode (uds_responseCode_t returnCode)
{
    const char * result = "unknown return code";
    switch (returnCode)
    {
    case uds_responseCode_PositiveResponse:
        result = "PositiveResponse";
        break;
    case uds_responseCode_GeneralReject:
        result = "GeneralReject";
        break;
    case uds_responseCode_ServiceNotSupported:
        result = "ServiceNotSupported";
        break;
    case uds_responseCode_SubfunctionNotSupported:
        result = "SubfunctionNotSupported";
        break;
    case uds_responseCode_IncorrectMessageLengthOrInvalidFormat:
        result = "IncorrectMessageLengthOrInvalidFormat";
        break;
    case uds_responseCode_ResponseTooLong:
        result = "ResponseTooLong";
        break;
    case uds_responseCode_BusyRepeatRequest:
        result = "BusyRepeatRequest";
        break;
    case uds_responseCode_ConditionsNotCorrect:
        result = "ConditionsNotCorrect";
        break;
    case uds_responseCode_RequestSequenceError:
        result = "RequestSequenceError";
        break;
    case uds_responseCode_NoResponseFromSubnetComponent:
        result = "NoResponseFromSubnetComponent";
        break;
    case uds_responseCode_FailurePreventsExecutionOfRequestedAction:
        result = "FailurePreventsExecutionOfRequestedAction";
        break;
    case uds_responseCode_RequestOutOfRange:
        result = "RequestOutOfRange";
        break;
    case uds_responseCode_SecurityAccessDenied:
        result = "SecurityAccessDenied";
        break;
    case uds_responseCode_InvalidKey:
        result = "InvalidKey";
        break;
    case uds_responseCode_ExceededNumberOfAttempts:
        result = "ExceededNumberOfAttempts";
        break;
    case uds_responseCode_RequiredTimeDelayNotExpired:
        result = "RequiredTimeDelayNotExpired";
        break;
    case uds_responseCode_UploadDownloadNotAccepted:
        result = "UploadDownloadNotAccepted";
        break;
    case uds_responseCode_TransferDataSuspended:
        result = "TransferDataSuspended";
        break;
    case uds_responseCode_GeneralProgrammingFailure:
        result = "GeneralProgrammingFailure";
        break;
    case uds_responseCode_WrongBlockSequenceCounter:
        result = "WrongBlockSequenceCounter";
        break;
    case uds_responseCode_RequestCorrectlyReceived_ResponsePending:
        result = "RequestCorrectlyReceived_ResponsePending";
        break;
    case uds_responseCode_SubfunctionNotSupportedInActiveSession:
        result = "SubfunctionNotSupportedInActiveSession";
        break;
    case uds_responseCode_ServiceNotSupportedInActiveSession:
        result = "ServiceNotSupportedInActiveSession";
        break;
    case uds_responseCode_RpmTooHigh:
        result = "RpmTooHigh";
        break;
    case uds_responseCode_RpmTooLow:
        result = "RpmTooLow";
        break;
    case uds_responseCode_EngineIsRunning:
        result = "EngineIsRunning";
        break;
    case uds_responseCode_EngineIsNotRunning:
        result = "EngineIsNotRunning";
        break;
    case uds_responseCode_EngineRunTimeTooLow:
        result = "EngineRunTimeTooLow";
        break;
    case uds_responseCode_TemperatureTooHigh:
        result = "TemperatureTooHigh";
        break;
    case uds_responseCode_TemperatureTooLow:
        result = "TemperatureTooLow";
        break;
    case uds_responseCode_VehicleSpeedTooHigh:
        result = "VehicleSpeedTooHigh";
        break;
    case uds_responseCode_VehicleSpeedTooLow:
        result = "VehicleSpeedTooLow";
        break;
    case uds_responseCode_ThrottlePedalTooHigh:
        result = "ThrottlePedalTooHigh";
        break;
    case uds_responseCode_ThrottlePedalTooLow:
        result = "ThrottlePedalTooLow";
        break;
    case uds_responseCode_TransmissionNotInNeutral:
        result = "TransmissionNotInNeutral";
        break;
    case uds_responseCode_TransmissionNotInGear:
        result = "TransmissionNotInGear";
        break;
    case uds_responseCode_BrakeSwitchesNotClosed:
        result = "BrakeSwitchesNotClosed";
        break;
    case uds_responseCode_ShifterLeverNotInPark:
        result = "ShifterLeverNotInPark";
        break;
    case uds_responseCode_TorqueConverterClutchLocked:
        result = "TorqueConverterClutchLocked";
        break;
    case uds_responseCode_VoltageTooHigh:
        result = "VoltageTooHigh";
        break;
    case uds_responseCode_VoltageTooLow:
        result = "VoltageTooLow";
        break;

    default:
        // norhing to do
        break;
    }
    return result;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
