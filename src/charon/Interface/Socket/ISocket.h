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
 * @addtogroup Socket
 * @{
 * @file ISocket.h
 * An abstract interface to all kind of hardware interfaces, like uart, usb, spi,
 * even tcp/ip and file access are possible. This way the upper layer software is
 * free from all implementation details and is more versatile.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef ISOCKET_H
#define ISOCKET_H

/* Includes ******************************************************************/

#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

typedef struct ISocket_t_public
{
    int32_t (*numAvailableBytes)(void);
    int32_t (*receive)(uint8_t *buf, uint32_t len);
    int32_t (*transmit)(const uint8_t *buf, uint32_t len);
} ISocket_t;

/* Interfaces ****************************************************************/

#endif /* ISOCKET_H */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
