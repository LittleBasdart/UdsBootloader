/**
 * @file drvPins.h
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief Driver for GPIO
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#ifndef DRVPINS_H_
#define DRVPINS_H_

#include <stdbool.h>

/**
 * @brief List of all available GPIOs.
 * These identifiers are used to reference
 * user GPIO pins in code. They can be either
 * digital inputs or digital outputs.
 * @note This list must be in sync with ::pinMapping
 */
typedef enum
{
    // just some sample pins
    pinId_LED1,
    pinId_LED2,
    pinId_LED3,
    pinId_LED4,
    pinId_LED5,
    
    pinId_count
} drvPins_pinId_t;


/**
 * @brief Initializes all needed pins of mcu.
 * This function must be called before using any
 * mcu pins, including CAN and other interfaces.
 */
void drvPins_init (void);

/**
 * @brief Set an output Pin to desired state.
 * @note Only takes effect, if pin is configured as an output.
 * @param pin The pin to set.
 * @param state The state to apply.
 */
void drvPins_setPin (drvPins_pinId_t pin, bool state);

/**
 * @brief Get state of input pin.
 * @note Might not be reliable, if pin is configured as output.
 * @param pin The pin to read.
 * @return The physical state of pin. 
 */
bool drvPins_getPin (drvPins_pinId_t pin);


#endif /* DRVPINS_H_ */
