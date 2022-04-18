/**
 * @file drvClocks.h
 * @author Little Basdart (twitch.tv/littlebasdart)
 * @brief Driver for Clock and Time
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#ifndef DRVCLOCKS_H_
#define DRVCLOCKS_H_

#include <stdint.h>

/**
 * @brief Initialization of System Clock and Time Base.
 * This function should be called directly after POR.
 * No need to wait for data and bss initialitation.
 * All system clocks and dependencies are initialized here.
 */
void drvClocks_init(void);

/**
 * @brief Runtime of System.
 * The time in milliseconds since power on reset is returned by this
 * function.
 * @return Time since POR in milliseconds.
 */
uint32_t drvClocks_getRuntime(void);

/**
 * @brief Frequency of Core.
 * This is the frequency, with wich the mcu is running.
 * @return The frequency in Hz.
 */
uint32_t drvClocks_getSystemClock(void);

/**
 * @brief Interrupt Callback for Time Measurement.
 * This function is only called in interrupt context of
 * systick (or any other timer) when 1 millisecond has
 * elapsed.
 */
void drvClocks_incrementTick(void);

/**
 * @brief Perform a software reset.
 * This function does not return.
 */
void drvClocks_mcuReset (void);

#endif /* DRVCLOCKS_H_ */
