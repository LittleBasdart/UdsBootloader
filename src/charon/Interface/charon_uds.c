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
 * @addtogroup Interface
 * @{
 * @file charon_uds
 * Implementation of Application Interfaces
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_uds.h"
#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "BusinessLogic/charon_UploadDownloadFunctionalUnit.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#if (!(CHARON_CONFIG_DO_NOT_PRINT_WELCOME))
#include "HSDI/charon_interface_debug.h"
#include "charon_logo.h"
#endif

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/**
 * @brief Print Welcome Message to Output
 * As means for Open source just starters and for Demonstration
 * Purposes.
 * Can be disabled.
 */
static void printUnnecessaryLargeWelcome (void);

/* Interfaces  ***************************************************************/


void charon_reset (void)
{
    charon_sscReset();
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_reset();
    charon_UploadDownloadFunctionalUnit_reset();
}

void charon_init (ISocket_t systemCommunicationSocket)
{
    charon_reset();
    charon_sscInit(systemCommunicationSocket);
#if (!(CHARON_CONFIG_DO_NOT_PRINT_WELCOME))
    printUnnecessaryLargeWelcome();
#endif
}

void charon_task (void)
{
    // TODO think about putting charon_sscRcvMessage into charon_sscCyclic

    /* Process Received Data */
    charon_sscRcvMessage();

    /* Process SSC Layer */
    charon_sscCyclic();

    /* Reset library if session has ended */
    if (charon_sscGetSession() == charon_sscType_timedOut)
    {
        charon_reset();
    }

    return;
}

/* Private Function **********************************************************/

static void printUnnecessaryLargeWelcome (void)
{
    PRINT_CHARON_LOGO_ON_LOG_OUT();
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

