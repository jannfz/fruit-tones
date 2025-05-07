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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SECRETS_H
#define __SECRETS_H

#pragma once

/* Includes ------------------------------------------------------------------*/

/* Macros and defines --------------------------------------------------------*/

/* Typedefs ------------------------------------------------------------------*/

struct WifiCredentials {
    const char* ssid;
    const char* password;
};

/* Exported variables --------------------------------------------------------*/

WifiCredentials networks[] = {
    { "SSID_1",         "PASSWORD_1" },
    { "SSID_2",         "PASSWORD_2" },
    { "SSID_3",         "PASSWORD_3" },
};

const int numNetworks = sizeof(networks) / sizeof(networks[0]);

/* Global function prototypes ------------------------------------------------*/

#endif /* __SECRETS_H */
