
#include "charon_interface_NvmDriver.h"

#include "drvFlash.h"
#include <string.h>

bool charon_NvmDriver_checkAddressRange (uint32_t address, uint32_t length)
{
    uint32_t startAddress = (uint32_t)drvFlash_getStartAddress(drvFlash_partition_App);
    uint32_t endAddress = startAddress+ drvFlash_getLength(drvFlash_partition_App);

    bool rangeValid = false;
    if ( (address >= startAddress) && ((address+length)<= endAddress) )
    {
        rangeValid = true;
    }
    return rangeValid;
}

uds_responseCode_t charon_NvmDriver_write (uint32_t address, const uint8_t* data, uint32_t size)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;
    if ((address & 0x7) != 0)
    {
        result = uds_responseCode_GeneralProgrammingFailure;
    }
    else if ((size & 0x7) != 0)
    {
        result = uds_responseCode_GeneralProgrammingFailure;
    }
    else
    {
        for (uint32_t i = 0; i<size; i+=8)
        {
            uint64_t word = data[i+7];
            word = (word<<8) | data[i+6];
            word = (word<<8) | data[i+5];
            word = (word<<8) | data[i+4];
            word = (word<<8) | data[i+3];
            word = (word<<8) | data[i+2];
            word = (word<<8) | data[i+1];
            word = (word<<8) | data[i+0];
            drvFlash_write((const void *)(address+i), word);
        }
    }
    return result;
}

void charon_NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size)
{
    memcpy(data, (const void*)address, size);
}

void charon_NvmDriver_erase (void)
{
    drvFlash_erasePartition(drvFlash_partition_App);
}

