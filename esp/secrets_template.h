#ifndef __SECRETS_H
#define __SECRETS_H

#pragma once

struct WifiCredentials {
    const char* ssid;
    const char* password;
};

WifiCredentials networks[] = {
    { "SSID_1",         "PASSWORD_1" },
    { "SSID_2",         "PASSWORD_2" },
    { "SSID_3",         "PASSWORD_3" },
};

const int numNetworks = sizeof(networks) / sizeof(networks[0]);


#endif
