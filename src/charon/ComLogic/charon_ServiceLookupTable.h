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
 * @defgroup ComLogic
 * @{
 * @file charon_ServiceLookupTable.c
 * This Module holds the lookup Table to connect all relevant data of
 * allowance and target FU for every service.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_SERVICELOOKUPTABLE_H_
#define CHARON_SERVICELOOKUPTABLE_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include <Common/charon_types.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/**
 * UDS Service Function Signature
 */
typedef uds_responseCode_t (*charonUdsFunctionSignature)(const uint8_t * pData, uint32_t length);

/**
 * Service Object Type.
 * This describes all Attributes needed to check execution rights of
 * a UDS Service
 */
typedef struct
{
    uds_sid_t                   sid;
    uint32_t                    sessionMask;
    charonUdsFunctionSignature  serviceRunable;
    uint32_t                    emcryptionMask;
} charon_serviceObject_t;

/* Interfaces ****************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject (uint8_t sid);

const char * charon_ServiceLookupTable_getNameForServiceSid (uint8_t sid);

const char * charon_ServiceLookupTable_getNameForReturnCode (uds_responseCode_t returnCode);

#endif /* CHARON_SERVICELOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
