/**
 * @file isotp_socket.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief ISO TP socket wrapper Implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */

#include "isotp_socket.h"

#include "drvCan.h"
#include "isotp.h"

#include <stdint.h>

static int32_t numAvailableBytes(void);
static int32_t receive(uint8_t *buf, uint32_t len);
static int32_t transmit(const uint8_t *buf, uint32_t len);

static IsoTpLink link;
static uint8_t sendbuffer[4096];
static uint8_t receivebuffer[4096];

const ISocket_t isotp_socket = 
{
    .numAvailableBytes = numAvailableBytes,
    .receive = receive,
    .transmit = transmit
};

static int32_t numAvailableBytes(void)
{
    int32_t numBytes = 0;
    if (link.receive_status == ISOTP_RECEIVE_STATUS_FULL)
    {
        numBytes = link.receive_size;
    }
    return numBytes;
}

static int32_t receive(uint8_t *buf, uint32_t len)
{
    uint16_t outSize = 0;
    isotp_receive(&link, buf, len, &outSize);
    return outSize;
}

static int32_t transmit(const uint8_t *buf, uint32_t len)
{
    isotp_send(&link, buf, len);
    return len;
}



void isotp_socket_init (void)
{
    drvCan_init(500000, 0x781);
    isotp_init_link(&link, 0x780,
                    sendbuffer, sizeof(sendbuffer),
                    receivebuffer, sizeof(receivebuffer)
    );
}

void isotp_socket_task (void)
{
    uint8_t data[8];
    uint8_t size = drvCan_getMessage(data);
    if (size != 0)
    {
        isotp_on_can_message(&link, data, size);
    }
    isotp_poll(&link);
}
