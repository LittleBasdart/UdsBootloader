/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2020 Andreas Hofmann
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
 * @defgroup Socket
 * @{
 * @file DevNullSocket.h
 * Socket for ignoring all data, like "dev/null" on unix systems
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef DEVNULLSOCKET_H
#define DEVNULLSOCKET_H

/* Includes ******************************************************************/

#include "Interface/Socket/ISocket.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

extern const ISocket_t DevNullSocket;

#endif /* DEVNULLSOCKET_H */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

