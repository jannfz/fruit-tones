/**
 ********************************************************************************
 * @file    wifi.ino
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 * @brief   WiFi setup
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include "secrets.h"

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/
const char spinChars[] = { '|', '/', '-', '\\' };
int spinIndex = 0;

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/

/* Global functions ----------------------------------------------------------*/

void spin_load();




void connect_to_WiFi() {

    Serial.println("Connect to WiFi");

    WiFi.mode(WIFI_AP_STA);

    for (int i = 0; i < numNetworks; i++) {

        Serial.printf("Connecting to: %-20s -", networks[i].ssid);
        WiFi.begin(networks[i].ssid, networks[i].password);

        spinIndex = 1;
        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
            delay(500);
            spin_load();
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\bCONNECTED!");
            Serial.printf("\nSTA IP address: %s \n", WiFi.localIP().toString().c_str());
            break;
        } else {
            Serial.println("\bFAILED!");
        }
    }

    WiFi.softAP("FruitTones");

    Serial.printf("AP IP address:  %s \n", WiFi.softAPIP().toString().c_str());

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Could not connect to any Wi-Fi network.");
        // Optionally start AP mode here
    }
    if (MDNS.begin("iot")) {
        Serial.println("mDNS responder started\n");
    }
}

/* Static functions ----------------------------------------------------------*/

void spin_load() {
    Serial.print('\b');
    Serial.print(spinChars[spinIndex]);
    spinIndex = (spinIndex + 1) % 4;
    delay(100);
}
