/**
 * @file isotp_user.c
 * @author Little Basdart (twitch.tv/LittleBasdart)
 * @brief ISO TP user function implementation
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * @license GPLv3
 */
#include "isotp_user.h"
#include "isotp_defines.h"

#include "drvCan.h"
#include "drvClocks.h"

void isotp_user_debug(const char* message, ...)
{
    (void) message;
    return;
}

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size)
{
    bool result = drvCan_sendMessage(arbitration_id, size, data);
    return result ? ISOTP_PROTOCOL_RESULT_OK : ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW;
}

uint32_t isotp_user_get_ms(void)
{
    return drvClocks_getRuntime();
}
