/**
 * @file drvCan.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief CAN Driver Implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#include "drvCan.h"
#include "drvClocks.h"
#include "stm32l471xx.h"

void drvCan_init(uint32_t baudrate, uint16_t filter)
{
    RCC->APB1ENR1 |= 1uL<<25;
    uint32_t sysClock = drvClocks_getSystemClock();

    CAN->MCR = 0x8000;  // master reset
    while ((CAN->MSR & 0x2) == 0);    // wait for sleep acknowledge
    CAN->MCR |= 1;      // start init mode
    while ((CAN->MSR & 0x1) == 0);    // wait for init acknowledge

    uint32_t prescaler = sysClock/(baudrate*10) - 1;
    uint32_t bitSegment1 = 7 - 1;   // content of hardware register has an offset. if 0 is written, 1 bitsegment is used
    uint32_t bitSegment2 = 2 - 1;

    CAN->BTR = prescaler | (bitSegment1<<16) | (bitSegment2<<20);

    CAN->sFilterRegister[0].FR1 = (filter<<5) | 0xffe00000;
    CAN->FA1R = 1;

    CAN->MCR = 0x4;      // stop init mode
    while ((CAN->MSR & 0x1) != 0);    // wait for init acknowledge
}

uint8_t drvCan_getMessage(uint8_t *data)
{
    uint8_t length = 0;

    if ((CAN->RF0R & 0x3) != 0)
    {
        length = CAN->sFIFOMailBox[0].RDTR & 0xf;
        uint32_t dataLow = CAN->sFIFOMailBox[0].RDLR;
        uint32_t dataHigh = CAN->sFIFOMailBox[0].RDHR;

        CAN->RF0R = 0x20;       // release fifo

        data[0] = dataLow >> 0;
        data[1] = dataLow >> 8;
        data[2] = dataLow >> 16;
        data[3] = dataLow >> 24;
        data[4] = dataHigh >> 0;
        data[5] = dataHigh >> 8;
        data[6] = dataHigh >> 16;
        data[7] = dataHigh >> 24;
    }
    
    return length;
}

bool drvCan_sendMessage(uint16_t canId, uint8_t dlc, const uint8_t *data)
{
    uint32_t dataLow;
    dataLow = (uint32_t)(data[0]) << 0;
    dataLow = (uint32_t)(data[1]) << 8;
    dataLow = (uint32_t)(data[2]) << 16;
    dataLow = (uint32_t)(data[3]) << 24;
    uint32_t dataHigh;
    dataHigh = (uint32_t)(data[4]) << 0;
    dataHigh = (uint32_t)(data[5]) << 8;
    dataHigh = (uint32_t)(data[6]) << 16;
    dataHigh = (uint32_t)(data[7]) << 24;

    bool tranmitted = false;
    for (uint8_t i = 0; i<3; i++)
    {
        if ((CAN->sTxMailBox[i].TIR & 1) == 0)
        {
            CAN->sTxMailBox[i].TDLR = dataLow;
            CAN->sTxMailBox[i].TDHR = dataHigh;
            CAN->sTxMailBox[i].TDTR = dlc;
            CAN->sTxMailBox[i].TIR = (uint32_t)canId << 21 | 1;
            tranmitted = true;
            break;
        }
    }
    return tranmitted;
}

