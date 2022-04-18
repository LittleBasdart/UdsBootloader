/**
 * @file isotp_socket.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief ISO TP socket wrapper for charon UDS
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#ifndef ISOTP_SOCKET_H_
#define ISOTP_SOCKET_H_

#include "Socket/ISocket.h"

/**
 * @brief Callback functions for CHARON UDS Stack.
 * Pass this struct to charon uds during initialization.
 */
extern const ISocket_t isotp_socket;

/**
 * @brief Initializes ISO-TP and CAN peripheral.
 * Call this function upon main init procedure.
 * ISO-TP and CAN driver are initialized by this
 * function.
 */
void isotp_socket_init (void);

/**
 * @brief Cyclic handling of ISO-TP stuff.
 * CAN bus is polled for new messages and
 * ISO-TP is handled in this function.
 * It must be called on a regular basis.
 */
void isotp_socket_task (void);

#endif /* ISOTP_SOCKET_H_ */
