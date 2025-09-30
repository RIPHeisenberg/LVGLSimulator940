#include <unistd.h>
#include "Configuration.h"

static Configuration_SettingsDescriptor_t settingsConfigurationFlash =
#include "Configuration.inc"    //lint !e9019

void HALDelay(void)
{
    usleep(5 * 100);
}


bool ConfigurationHandler_SettingsDescriptor_get(Configuration_SettingsDescriptor_t **descriptor)
{
    *descriptor = &settingsConfigurationFlash;
    return true;
}
