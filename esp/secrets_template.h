/**
 ********************************************************************************
 * @file    secrets.h
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 * @brief   WiFI Credentials - secrets.h is ignored by git
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

#ifndef __SECRETS_H
#define __SECRETS_H

#pragma once

/* Includes ------------------------------------------------------------------*/

/* Macros and defines --------------------------------------------------------*/

/* Typedefs ------------------------------------------------------------------*/

// Struct to hold WiFi network credentials
struct WifiCredentials {
    const char* ssid;       // Network name
    const char* password;   // Corresponding password
};

/* Exported variables --------------------------------------------------------*/

// List of known WiFi networks for fallback connection
WifiCredentials networks[] = {
    { "SSID_1",         "PASSWORD_1" },   // Replace with your actual network info
    { "SSID_2",         "PASSWORD_2" },
    { "SSID_3",         "PASSWORD_3" },
};

// Total number of configured networks
const int numNetworks = sizeof(networks) / sizeof(networks[0]);

/* Global function prototypes ------------------------------------------------*/

#endif /* __SECRETS_H */
