
#include "charon_interface_clock.h"

#include "drvClocks.h"

uint32_t charon_interface_clock_getTime(void)
{
    return drvClocks_getRuntime();
}

uint32_t charon_interface_clock_getTimeElapsed(uint32_t timestamp)
{
    return drvClocks_getRuntime() - timestamp;
}
