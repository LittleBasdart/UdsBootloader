/**
 * @file drvClocks.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief Clock Driver Implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#include "drvClocks.h"
#include "stm32l471xx.h"

static const uint32_t targetClock = 80000000;
static const uint32_t hsiClock = 16000000;

static uint32_t tick;

extern void (*isr_vector[])(void);

void drvClocks_init(void)
{
    RCC->CR |= 0x100;       // turn HSI on

    uint32_t pllR = 0;
    while ((targetClock * (1 << (pllR + 1))) % hsiClock != 0)
    {
        pllR++;
    }
    uint32_t pllM = 0;  // use 1 as divider
    uint32_t pllN = (targetClock * (1 << (pllR + 1))) / hsiClock;

    RCC->PLLCFGR = 0x2uL | (pllM<<4) | (pllN<<8) | (1uL<<24) | (pllR<<25);

    FLASH->ACR |= 0x704;        // enable cache, prefetch, 4 waitstates

    while ((RCC->CR & 0x400) != 0);       // wait for HSI stable
    RCC->CR |= 0x1000000;       // turn PLL on
    while ((RCC->CR & 0x2000000) != 0);       // wait for PLL stable
    RCC->CFGR |= 0x3;               // use PLL as system clock
    while ((RCC->CFGR & 0xF) != 0xF); // wait for clock switch
    RCC->CR &= ~0x1;       // turn MSI off

    // todo: is this enough to enable systick interrupts?
    SCB->VTOR = (uint32_t)isr_vector;
    __enable_irq();
    SysTick->LOAD = (targetClock/8) / 1000;
    SysTick->CTRL = 0x3;
}

uint32_t drvClocks_getRuntime(void)
{
    return tick;
}

uint32_t drvClocks_getSystemClock(void)
{
    return targetClock;
}

void drvClocks_incrementTick(void)
{
    tick++;
}

void drvClocks_mcuReset (void)
{
    __NVIC_SystemReset();
}
