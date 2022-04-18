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
 * @defgroup Common
 * @{
 * @file charon_types.h
 * Provides all needed Types and declarations used by Charon UDS.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_TYPES_H_
#define CHARON_TYPES_H_

/* Includes ******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Define NULL for Compatibility across libraries and Compilers */
#ifndef NULL
#define NULL        ((void *) 0)
#endif

/** Helper Macro to define a Byte Packed struct depending on compiler */
#ifdef __GNUC__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed)) name
#endif
#ifdef __arm__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed)) name
#endif

/* Types *********************************************************************/

typedef enum
{
    charon_messageType_Diagnostics,      //!< charon_messageType_Diagnostics
    charon_messageType_RemoteDiagnostics,//!< charon_messageType_RemoteDiagnostics
} charon_messageType_t;

typedef enum
{
    charon_targetAddressType_Physical,
    charon_targetAddressType_Functional,
} charon_targetAddressType_t;

typedef enum
{
    uds_sid_DiagnosticSessionControl = 0x10u,
    uds_sid_EcuReset = 0x11u,
    uds_sid_ClearDiagnosticInformation = 0x14u,
    uds_sid_ReadDtcInformation = 0x19u,
    uds_sid_ReadDataByIdentifier = 0x22u,
    uds_sid_ReadMemoryByAddress = 0x23u,
    uds_sid_ReadScalingDataByIdentifier = 0x24u,
    uds_sid_SecurityAccess = 0x27u,
    uds_sid_CommunicationControl = 0x28u,
    uds_sid_ReadDataByPeriodicIdentifier = 0x2Au,
    uds_sid_DynamicallyDefineDataIdentifier = 0x2Cu,
    uds_sid_WriteDataByIdentifier = 0x2Eu,
    uds_sid_InputOutputControlByIdentifier = 0x2Fu,
    uds_sid_RoutineControl = 0x31u,
    uds_sid_RequestDownload = 0x34u,
    uds_sid_RequestUpload = 0x35u,
    uds_sid_TransferData = 0x36u,
    uds_sid_RequestTransferExit = 0x37u,
    uds_sid_RequestFileTransfer = 0x38u,
    uds_sid_WriteMemoryByAddress = 0x3Du,
    uds_sid_TesterPresent = 0x3Eu,
    uds_sid_PositiveResponseMask = 0x40u,
    uds_sid_NegativeResponse = 0x7Fu,
    uds_sid_AccessTimingParameter = 0x83u,
    uds_sid_SecuredDataTransmission = 0x84u,
    uds_sid_ControlDtcSetting = 0x85u,
    uds_sid_ResponseOnEvent = 0x86u,
    uds_sid_LinkControl = 0x87u,

    uds_sid_amount
} uds_sid_t;

typedef enum
{
    uds_responseCode_PositiveResponse = 0u,
    uds_responseCode_GeneralReject = 0x10u,
    uds_responseCode_ServiceNotSupported = 0x11u,
    uds_responseCode_SubfunctionNotSupported = 0x12u,
    uds_responseCode_IncorrectMessageLengthOrInvalidFormat = 0x13u,
    uds_responseCode_ResponseTooLong = 0x14u,
    uds_responseCode_BusyRepeatRequest = 0x21u,
    uds_responseCode_ConditionsNotCorrect = 0x22u,
    uds_responseCode_RequestSequenceError = 0x24u,
    uds_responseCode_NoResponseFromSubnetComponent = 0x25u,
    uds_responseCode_FailurePreventsExecutionOfRequestedAction = 0x26u,
    uds_responseCode_RequestOutOfRange = 0x31u,
    uds_responseCode_SecurityAccessDenied = 0x33u,
    uds_responseCode_InvalidKey = 0x35u,
    uds_responseCode_ExceededNumberOfAttempts = 0x36u,
    uds_responseCode_RequiredTimeDelayNotExpired = 0x37u,
    uds_responseCode_UploadDownloadNotAccepted = 0x70u,
    uds_responseCode_TransferDataSuspended = 0x71u,
    uds_responseCode_GeneralProgrammingFailure = 0x72u,
    uds_responseCode_WrongBlockSequenceCounter = 0x73u,
    uds_responseCode_RequestCorrectlyReceived_ResponsePending = 0x78u,
    uds_responseCode_SubfunctionNotSupportedInActiveSession = 0x7Eu,
    uds_responseCode_ServiceNotSupportedInActiveSession = 0x7Fu,
    uds_responseCode_RpmTooHigh = 0x81u,
    uds_responseCode_RpmTooLow = 0x82u,
    uds_responseCode_EngineIsRunning = 0x83u,
    uds_responseCode_EngineIsNotRunning = 0x84u,
    uds_responseCode_EngineRunTimeTooLow = 0x85u,
    uds_responseCode_TemperatureTooHigh = 0x86u,
    uds_responseCode_TemperatureTooLow = 0x87u,
    uds_responseCode_VehicleSpeedTooHigh = 0x88u,
    uds_responseCode_VehicleSpeedTooLow = 0x89u,
    uds_responseCode_ThrottlePedalTooHigh = 0x8Au,
    uds_responseCode_ThrottlePedalTooLow = 0x8Bu,
    uds_responseCode_TransmissionNotInNeutral = 0x8Cu,
    uds_responseCode_TransmissionNotInGear = 0x8Du,
    uds_responseCode_BrakeSwitchesNotClosed = 0x8Fu,
    uds_responseCode_ShifterLeverNotInPark = 0x90u,
    uds_responseCode_TorqueConverterClutchLocked = 0x91u,
    uds_responseCode_VoltageTooHigh = 0x92u,
    uds_responseCode_VoltageTooLow = 0x93u,
} uds_responseCode_t;

/* Interfaces ****************************************************************/

#endif /* CHARON_TYPES_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

