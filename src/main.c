/**
 * @file main.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief main loop Implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */

/*
dinge, die ich ich brauche:
 - Clock treiber ✔
 - CAN-Bus treiber ✔
 - Flash treiber ✔
 - uds stack rufen ✔
 - iso-tp rufen ✔
 - irgendeine form von CRC? ❓
 - Pins ✔
 - irgendeine form um den Bootloader zu starten ✔
 - implementiere charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset ⭕
*/

#include "charon_uds.h"
#include "isotp_socket.h"
#include "drvClocks.h"
#include "drvFlash.h"
#include "drvPins.h"

#include <stdint.h>
#include <stdbool.h>

extern uint32_t bootloaderCommand[];
bool uds_reset_request = false; 

int main (void)
{
    if (bootloaderCommand[0] != 0xB00110AD)
    {
        // start application
        const uint32_t * appStart = drvFlash_getStartAddress(drvFlash_partition_App);
        // todo: check app for validity

        // set stack pointer and jump to entry point
        register uint32_t stackPointer asm ("r13");     // todo: this is gnu extension, build with -std=gnu11 or change this line to inline assembler
        register void(*startAddress)(void) = appStart[1];  // tell compiler to save this in register, as we are messing with stack pointer.
        stackPointer = *appStart;
        startAddress();
        // happy compiler
        (void)stackPointer;
    }

    bootloaderCommand[0] = 0;

    drvPins_init();
    isotp_socket_init();
    charon_init(isotp_socket);

    while (1)
    {
        isotp_socket_task();
        charon_task();

        if (uds_reset_request == true)
        {
            uint32_t entryTime = drvClocks_getRuntime();
            while (drvClocks_getRuntime() - entryTime < 5)
            {
                isotp_socket_task();
                charon_task();
            }
            drvClocks_mcuReset();
        }

    }

    return 0;
}
