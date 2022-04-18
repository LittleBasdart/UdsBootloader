/**
 * @file drvFlash.h
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief Driver for Flash Memory
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#ifndef DRVFLASH_H_
#define DRVFLASH_H_

#include <stdint.h>

/**
 * @brief List of all available flash partitions.
 */
typedef enum 
{
    drvFlash_partition_EarlyLoader,
    drvFlash_partition_Bootloader1,
    drvFlash_partition_Bootloader2,
    drvFlash_partition_App,
    drvFlash_partition_Log,
    drvFlash_partition_Parameters,

    drvFlash_partition_count
} drvFlash_partition_t;

/**
 * @brief Get start address of flash partition.
 * Physical start address of selected flash partiton.
 * @param partition The selected partition.
 * @return Physical start address.
 */
const void * drvFlash_getStartAddress(drvFlash_partition_t partition);

/**
 * @brief Get lengh of flash partition.
 * Lengh in bytes of selected flash partiton.
 * @param partition The selected partition.
 * @return Lengh in bytes.
 */
uint32_t drvFlash_getLength(drvFlash_partition_t partition);

/**
 * @brief Perform page erase.
 * The page, which contains given address, is erased
 * and prepared for new data.
 * @param address An address in selected page.
 */
void drvFlash_erasePageByAddress(const void* address);

/**
 * @brief Perform partition erase.
 * Selected partition is erased and prepared for new
 * data.
 * @param partition The partition to erase.
 */
void drvFlash_erasePartition(drvFlash_partition_t partition);

/**
 * @brief Write data to flash memory.
 * Data is written to non volatile flash memory.
 * @param address The address to write to, must be 64bit aligned.
 * @param data The data to write.
 */
void drvFlash_write(const void * address, uint64_t data);

#endif /* DRVFLASH_H_ */
