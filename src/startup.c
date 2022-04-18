/**
 * @file startup.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief startup code, written in c, because why not?
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */


#include "drvClocks.h"
#include <stdint.h>
#include <string.h>


extern uint8_t _estack[];
void Reset_Handler (void);
static void NMI_Handler (void);
static void HardFault_Handler (void);
static void MemManage_Handler (void);
static void UsageFault_Handler (void);
static void SVCall_Handler (void);
static void PendSV_Handler (void);
static void Systick_Handler (void);

extern void __libc_init_array (void);
extern int main (void);
extern void __libc_fini_array (void);

void (*isr_vector[])(void) = {
    (void*)_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVCall_Handler,
    0,
    0,
    PendSV_Handler,
    Systick_Handler,
};


void Reset_Handler (void)
{
    drvClocks_init();

    extern uint8_t _sdata[];
    extern uint8_t _edata[];
    extern uint8_t _sidata[];
    memcpy(_sdata, _sidata, _edata - _sdata);
    extern uint8_t _sbss[];
    extern uint8_t _ebss[];
    memset(_sbss, 0, _ebss - _sbss);

    __libc_init_array();

    main();

    __libc_fini_array();

    while (1);
}

static void NMI_Handler (void)
{
    while(1);
}

static void HardFault_Handler (void)
{
    while(1);
}

static void MemManage_Handler (void)
{
    while(1);
}

static void UsageFault_Handler (void)
{
    while(1);
}

static void SVCall_Handler (void)
{
    while(1);
}

static void PendSV_Handler (void)
{
    while(1);
}

static void Systick_Handler (void)
{
    drvClocks_incrementTick();
}

