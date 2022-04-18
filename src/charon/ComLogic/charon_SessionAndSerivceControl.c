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
 * @file charon_SessionAndServiceControl.c
 * Implementation of the Service and Session Control Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stdbool.h>
#include <string.h>
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "ComLogic/charon_ServiceLookupTable.h"
#include "Common/charon_negativeResponse.h"
#include "HSDI/charon_interface_clock.h"
#include "HSDI/charon_interface_debug.h"
#include "Interface/Socket/ISocket.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Size of the UDS Receive Buffer in byte */
#define CHARON_RCV_BUFFER_SIZE      ((uint32_t) 4095u)
/** Size of the UDS Transmit Buffer in byte */
#define CHARON_TX_BUFFER_SIZE      ((uint32_t) 4095u)

/**
 * Default Timing Parameter for the UDS Server Communication and Session Control
 * @{
 * */
#define DEFAULT_P2_SERVER            ((uint32_t) 50u)       /**< Default P2 Server Time (see ISO Requirements) */
#define DEFAULT_P2_STAR_SERVER       ((uint32_t) 5000u)     /**< Default P2* Server Time (see ISO Requirements) */
#define DEFAULT_S3_SERVER            ((uint32_t) 5000u)     /**< Default S3 Server Time (see ISO Requirements) */
/**
 * @}
 */

/**
 * Answer Requests are Request + Indication Bit
 */
#define UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK         ((uint8_t) 0x40u)

/* Types *********************************************************************/

/** Struct to store all timing required Data */
typedef struct
{
    //TODO: there are many more, but for now...
    uint32_t        p2Server;
    uint32_t        p2StarServer;
} ComTimeoutLimits_t;

/* Variables *****************************************************************/

/** Stores the Currently Active Diagnostic Session */
static charon_sessionTypes_t s_currentDiagnosticSession = charon_sscType_default;
/** Pointer to last service that was requested pending */
static charon_serviceObject_t * s_currentlyPendingService = NULL;
/** Timestamp for Pending Start */
static uint32_t s_pendingRequestStartTime = 0u;
/** Flag to indicate if the P2 Expired Message for Pending was already handled */
static bool s_p2PendingExceededHandled = false;
/** Timestamp for Diagnostic Session Timing */
static uint32_t s_diagnosticSessionTimestamp = 0u;
/** Adjustable timeouts, initialized with server default values */
static ComTimeoutLimits_t s_ttl =
{
        DEFAULT_P2_SERVER, DEFAULT_P2_STAR_SERVER
};
/** Stores the System given Communication Socket */
static ISocket_t s_systemComSocket = {NULL};

/* Buffers *******************************************************************/

/** Charon Session and Communication Control Receive Buffer */
static uint8_t s_receiveBuffer[CHARON_RCV_BUFFER_SIZE];
/** Charon Session and Communication Control Transmit Buffer */
static uint8_t s_sendBuffer[CHARON_TX_BUFFER_SIZE];


/* Private Function Definitions **********************************************/

/**
 * Process an UDS Telegram and dispatch services
 * Accordingly to session, security and diagnostic requirements.
 *
 * @param pBuffer
 *      Pointer to received Message
 * @param length
 *      Length in Bytes
 */
static void processReceivedMessage (uint8_t const * const pBuffer, uint32_t length);

/**
 * Checks if SID is allowed in session
 *
 * @param currentSession
 *      Currently Active Diagnose Session
 * @param pService
 *      Pointer to Service Object
 *
 * @return Service is allowed or disallowed in Session
 */
static bool isServiceInSession (charon_sessionTypes_t currentSession, const charon_serviceObject_t * pService);

/**
 * Timeout and Session Control for ongoing
 * sessions other than default.
 */
static void handleDiagnosticSession (void);

/**
 * Is called during a RCRP Communication Monitor.
 */
static void handleResponsePending (void);

/**
 * Execute given Service
 *
 * @param pExecutableService
 *      Dispatched Service Entry from Lookup Table
 * @param pUdsMessage
 *      Pointer to UDS Message Buffer
 * @param length
 *      Length of UDS Message in Byte
 *
 * @return @see @ref uds_responseCode_t
 */
static uds_responseCode_t handleService (const charon_serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length);

/**
 * Copy UDS Message to Output Buffer
 * @param pUdsMessage
 *      Pointer to Message
 * @param length
 *      Length of Message in Bytes
 */
static void sendMessage (uint8_t const * const pUdsMessage, uint32_t length);

/* Interfaces  ***************************************************************/

void charon_sscReset (void)
{
    s_currentDiagnosticSession = charon_sscType_default;
    s_currentlyPendingService = NULL;
    s_pendingRequestStartTime = 0u;
    s_p2PendingExceededHandled = false;
    s_diagnosticSessionTimestamp = 0u;
    s_ttl.p2Server = DEFAULT_P2_SERVER;
    s_ttl.p2StarServer = DEFAULT_P2_STAR_SERVER;
}

void charon_sscInit (ISocket_t sscComSocket)
{
    s_systemComSocket = sscComSocket;
}

void charon_sscCyclic (void)
{
    /* Check Diagnostic Session */
    if(charon_sscType_default != s_currentDiagnosticSession)
    {
        handleDiagnosticSession();
    }

    /* Handle NRC Pending */
    if(NULL != s_currentlyPendingService)
    {
        handleResponsePending();
    }
    else
    {
        /* Reset Flag for P2 and P2* distinguish */
        s_p2PendingExceededHandled = false;
    }
}

void charon_sscRcvMessage (void)
{
    int32_t length;

    /* Get Message from Underlying Transport LL */
    length = s_systemComSocket.receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs translation.
     */

    /* Process Received Message if one was gotten and de-crypted */
    if(length > 0)
    {
        processReceivedMessage(s_receiveBuffer, (uint32_t)length);
    }
}

static void processReceivedMessage (uint8_t const * const pBuffer, uint32_t length)
{
    charon_serviceObject_t * pServiceObj = charon_ServiceLookupTable_getServiceObject((uint8_t)pBuffer[0]);   /* Get Service Object */
    uds_responseCode_t retVal;

    CHARON_INFO("Message received, length: %i SID: %s", length, charon_ServiceLookupTable_getNameForServiceSid((uint8_t)pBuffer[0]));

    /* Is a Service Pending, do not execute any other Requests except for Tester Present */
    if((NULL == s_currentlyPendingService)
            || (pServiceObj->sid == uds_sid_TesterPresent))
    {
        retVal = handleService(pServiceObj, pBuffer, length);
        /* Check Return Value of Service Execution and Act accordingly */
        switch(retVal)
        {
        case uds_responseCode_ServiceNotSupportedInActiveSession:
        {
            CHARON_WARNING("Wrong session for requested service, sending error message.");
            /* Answer NRC and Send */
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupportedInActiveSession, pServiceObj->sid);
            break;
        }
        case uds_responseCode_ServiceNotSupported:
        {
            CHARON_WARNING("Service not supported / not implemented, sending error message.");
            /* Construct Answer */
            uds_sid_t castedSid;
            /*
             * At this point the SID is extracted from the first Byte of the payload and the LINT warning is deliberately suppressed
             * since the ISO Requires to answer specifically with the SID that is not supported, even though it can be, that the SID
             * is not in the Enumeration.
             */
            castedSid = (uds_sid_t)pBuffer[0];  //TODO Suppress
            /* Send NRC */
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, castedSid);
            break;
        }
        case uds_responseCode_RequestCorrectlyReceived_ResponsePending:
        {
            CHARON_INFO("Service is executed, response pending.");
            /* Mark Ongoing Service and Begin with Pending Management */
            s_currentlyPendingService = pServiceObj;
            s_pendingRequestStartTime = charon_interface_clock_getTime();
            break;
        }
        default:
        {
            //Do Nothing...
            CHARON_INFO("Service returned %s.", charon_ServiceLookupTable_getNameForReturnCode(retVal));
            break;
        }
        }
        if (retVal != uds_responseCode_RequestCorrectlyReceived_ResponsePending)
        {
            // completion of a service (which is indicated by not being "response pending") updates session timer
            s_diagnosticSessionTimestamp = charon_interface_clock_getTime();
        }
    }
    else
    {
        /* Send Busy */
        CHARON_WARNING("Server is busy, returning error code.");
        charon_sendNegativeResponse(uds_responseCode_BusyRepeatRequest, pServiceObj->sid);
    }
}

void charon_sscTxMessage (uint8_t const * const pBuffer, uint32_t length)
{
    /* Check if a Request is pending */
    if(NULL != s_currentlyPendingService)
    {
        /* Copy First byte and Align to Request ID Matching to check if it was an ongoing Service that is now answered */
        uint8_t responeRequestId = (uint8_t)(((uint8_t)pBuffer[0]) & ((uint8_t)~UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK));
        if(responeRequestId == (uint8_t)s_currentlyPendingService->sid)
        {
            CHARON_INFO("Sending pending response, length %i", length);
            sendMessage(pBuffer, length);
            /* Reset Pending Request */
            s_currentlyPendingService = NULL;
        }
        else
        {
            // todo: how to handle this case?
            CHARON_ERROR("Message SID %s (0x%x) does not match with pending SID %s (0x%x)", charon_ServiceLookupTable_getNameForServiceSid(responeRequestId), responeRequestId, charon_ServiceLookupTable_getNameForServiceSid((uint8_t)s_currentlyPendingService->sid), (uint8_t)s_currentlyPendingService->sid);
        }
    }
    else
    {
        /* Just send Synchronous Message */
        CHARON_INFO("Sending synchronous response, length %i", length);
        sendMessage(pBuffer, length);
    }
}

void charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended)
{
    if(charon_sscType_default != sessionType)
    {
        /* Mark Beginning of an Diagnostic session that is not default session */
        s_diagnosticSessionTimestamp = charon_interface_clock_getTime();
        /* Set new timings */
        s_ttl.p2Server = timeoutP2;
        s_ttl.p2StarServer = timeoutP2extended;
    }
    else
    {
        /* Fall Back to default timings */
        s_ttl.p2Server = DEFAULT_P2_SERVER;
        s_ttl.p2StarServer = DEFAULT_P2_STAR_SERVER;
    }

    /* Assign new Session Type */
    s_currentDiagnosticSession = sessionType;
}

charon_sessionTypes_t charon_sscGetSession (void)
{
    return s_currentDiagnosticSession;
}

void charon_sscTesterPresentHeartbeat (void)
{
    s_diagnosticSessionTimestamp = charon_interface_clock_getTime();
}

/* Private Function **********************************************************/

static bool isServiceInSession (charon_sessionTypes_t currentSession, const charon_serviceObject_t * pService)
{
    bool retval = false;
    uint32_t result = (uint32_t)((uint32_t)(1uL << currentSession) & pService->sessionMask);
    if(result > 0u)
    {
        retval = true;
    }

    return retval;
}

static void handleDiagnosticSession (void)
{
    /* Check if Session Timed Out */
    if(charon_interface_clock_getTimeElapsed(s_diagnosticSessionTimestamp) >= DEFAULT_S3_SERVER)
    {
        CHARON_WARNING("Session timed out, activating default session.");
        /* terminate Session */
        s_currentDiagnosticSession = charon_sscType_timedOut;
    }
}

static void handleResponsePending (void)
{
    uint32_t tmp;

    /* Get all Time since Pending Request is started */
    tmp = charon_interface_clock_getTimeElapsed(s_pendingRequestStartTime);

    /* Check if P2 was exceeded */
    if(!(s_p2PendingExceededHandled))
    {
        if(tmp >= s_ttl.p2Server)
        {
            s_p2PendingExceededHandled = true;
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
    }
    else /* Handle the P2* exceedings */
    {
        if(tmp >= s_ttl.p2StarServer)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
        //TODO as far as i remember there was a maximum amount to do this, but i couldn't find it...
        // remark: you probably mean P4_Server, which is manufacturer specific
    }
}

static uds_responseCode_t handleService (const charon_serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length)
{
    uds_responseCode_t retVal;

    /* Check if Service is supported */
    if (NULL != pExecutableService)
    {
        /* Check if Service is Supported in Current Session */
        if (isServiceInSession(s_currentDiagnosticSession, pExecutableService))
        {
            /* Execute Service */
            retVal = pExecutableService->serviceRunable(pUdsMessage, length);
        }
        else
        {
            /* Send Diag NRC */
            retVal = uds_responseCode_ServiceNotSupportedInActiveSession;
        }
    }
    else
    {
        /* Send not Supported NRC */
        retVal = uds_responseCode_ServiceNotSupported;
    }

    return retVal;
}

static void sendMessage (uint8_t const * const pUdsMessage, uint32_t length)
{
    uint32_t txLength;

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs encoding.
     */

    /* Transmit Message if it fits, otherwise trim */
    if(length >= CHARON_TX_BUFFER_SIZE)      //TODO Error Case?
    {
        txLength = CHARON_TX_BUFFER_SIZE;
    }
    else
    {
        txLength = length;
    }

    /* Copy to Buffer and start transfer */
    memcpy(s_sendBuffer, pUdsMessage, txLength);
    (void)s_systemComSocket.transmit(s_sendBuffer, txLength);
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
