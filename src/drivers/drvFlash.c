/**
 * @file drvFlash.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief Flash Driver Implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#include "drvFlash.h"
#include "stm32l471xx.h"

static const uint32_t pagesize = 2 * 1024;
static const void * flashBase = (const void *)0x08000000;

static const struct
{
    const void * startaddress;
    uint32_t length;
} partitionLookupTable [drvFlash_partition_count] = 
{
    {(const void *)0x08000000, 2 * 1024},
    {(const void *)0x08000800, 2 * 1024},
    {(const void *)0x08001000, 2 * 1024},

    {(const void *)0x08001800, 488 * 1024},

    {(const void *)0x0807C000, 6 * 1024},
    {(const void *)0x0807D800, 10 * 1024},
};


const void * drvFlash_getStartAddress(drvFlash_partition_t partition)
{
    return partitionLookupTable[partition].startaddress;
}

uint32_t drvFlash_getLength(drvFlash_partition_t partition)
{
    return partitionLookupTable[partition].length;
}

void drvFlash_erasePageByAddress(const void* address)
{
    while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
    FLASH->SR = 0xFFFF;                 // clear all error flags

    uint32_t pagenum = ((const uint8_t*)address - (const uint8_t*)flashBase) / pagesize;
    // unlock sequence
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    uint32_t cr = FLASH->CR;
    cr &= 0xFFFF0000;
    cr |= (pagenum << 3);
    cr |= 0x2;
    FLASH->CR = cr;
    FLASH->CR |= 0x10000;

    // lock sequence
    FLASH->CR |= 1uL<<31;

    while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
}

void drvFlash_erasePartition(drvFlash_partition_t partition)
{
    while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
    FLASH->SR = 0xFFFF;                 // clear all error flags

    const uint8_t * address = drvFlash_getStartAddress(partition);
    uint32_t length = drvFlash_getLength(partition);
    while (length > 0u)
    {
        uint32_t pagenum = (address - (const uint8_t*)flashBase) / pagesize;
        // unlock sequence
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;

        uint32_t cr = FLASH->CR;
        cr &= 0xFFFF0000;
        cr |= (pagenum << 3);
        cr |= 0x2;
        FLASH->CR = cr;
        FLASH->CR |= 0x10000;

        // lock sequence
        FLASH->CR |= 1uL<<31;

        while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
        length -= pagesize;
    }
}

void drvFlash_write(const void * address, uint64_t data)
{
    while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
    FLASH->SR = 0xFFFF;                 // clear all error flags

    // unlock sequence
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    FLASH->CR |= 0x1;
    ((uint32_t*)address)[0] = data & 0xFFFFFFFF;
    ((uint32_t*)address)[1] = data >> 32;
    while ((FLASH->SR & 0x1000) != 0);  // wait for busy flag
    FLASH->CR &= ~0x1uL;

    // lock sequence
    FLASH->CR |= 1uL<<31;
}

