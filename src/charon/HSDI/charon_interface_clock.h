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
 * @addtogroup HSDI
 * @{
 * @file charon_interface_clock.h
 * This Interface Describes the Functions needed by CHARON UDS for
 * its timing and Clock Requirements.
 * It is strongly recommended to setup a timebase on 1ms/digit frame.
 * Otherwise the timing default parameters and dcm heartbeat parameters
 * need to be adjusted accordingly.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_INTERFACE_CLOCK_H_
#define CHARON_INTERFACE_CLOCK_H_

/* Includes ******************************************************************/

#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * Get Current System Time (preferable as Timestamp in ms)
 *
 * @return System Time
 */
uint32_t charon_interface_clock_getTime(void);

/**
 * Get time difference from given System time to moment of
 * call.
 *
 * @param timestamp
 *      Value given by charon_interface_clock_getTime
 *
 * @return time elapsed to given timestamp (preferable in ms)
 */
uint32_t charon_interface_clock_getTimeElapsed(uint32_t timestamp);

#endif /* CHARON_INTERFACE_CLOCK_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
