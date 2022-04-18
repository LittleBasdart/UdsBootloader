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
 * @addtogroup HSDI
 * @{
 * @file charon_interface_NvmDriver.h
 * This Module handles all functions to use Nvm
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_INTERFACE_NVMDRIVER_H_
#define CHARON_INTERFACE_NVMDRIVER_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "Common/charon_types.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Interfaces  ***************************************************************/

/**
 * Checks given memory address for validity.
 * This function checks the address range, if it is available in
 * memory map for reading or writing.
 * @param address   The start address of the memory range
 * @param length    The length of the range
 * @retval  true    Memory range is valid
 * @retval  false   Memory range is not valid
 */
bool charon_NvmDriver_checkAddressRange (uint32_t address, uint32_t length);

/**
 * Writes data to non volatile memory.
 * This function writes given data to the desired memory location.
 * @param address   The start address of write operation
 * @param data      The data to write
 * @param size      The amount of bytes to write
 * @retval  charon_responseCode_PositiveResponse   Memory is written successfully
 * @retval  charon_responseCode_GeneralProgrammingFailure  Error while programming
 * @retval  charon_responseCode_VoltageTooHigh     Voltage is too high for programming
 * @retval  charon_responseCode_VoltageTooLow      Voltage is too low for programming
 */
uds_responseCode_t charon_NvmDriver_write (uint32_t address, const uint8_t* data, uint32_t size);

/**
 * Reads data from non volatile memory.
 * This function reads data to given buffer from desired memory location.
 * @param address   The start address of read operation
 * @param data      The buffer to fill with data
 * @param size      The number of bytes to read
 */
void charon_NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size);

/**
 * Erases non volatile memory.
 */
void charon_NvmDriver_erase (void);

#endif /* CHARON_INTERFACE_FLASHDRIVER_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/