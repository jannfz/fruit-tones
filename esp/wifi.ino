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
#include <ESP8266WiFi.h>           // WiFi library for ESP8266
#include "secrets.h"               // WiFi credentials stored securely

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/

// Characters used for spinning animation while waiting for WiFi
const char spinChars[] = { '|', '/', '-', '\\' };
int spinIndex = 0;  // Index into spinChars

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/
void spin_load();  // Animation while connecting

/* Global functions ----------------------------------------------------------*/

/**
 * @brief Connects to the first available WiFi network from the credentials list.
 *        Falls back to soft AP mode ("FruitTones") if all fail.
 */
void connect_to_WiFi() {

    Serial.println("Connect to WiFi");

    // Set ESP8266 to both Station and Access Point mode
    WiFi.mode(WIFI_AP_STA);

    // Try each saved network until one connects
    for (int i = 0; i < numNetworks; i++) {

        Serial.printf("Connecting to: %-20s -", networks[i].ssid);
        WiFi.begin(networks[i].ssid, networks[i].password);

        spinIndex = 1;
        unsigned long start = millis();

        // Wait up to 8 seconds while showing loading animation
        while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
            delay(500);
            spin_load();  // Show spinning loader
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\bCONNECTED!");
            Serial.printf("\nSTA IP address: %s \n", WiFi.localIP().toString().c_str());
            break;  // Stop once successfully connected
        } else {
            Serial.println("\bFAILED!");
        }
    }

    // Start fallback access point regardless of station status
    WiFi.softAP("FruitTones");
    Serial.printf("AP IP address:  %s \n", WiFi.softAPIP().toString().c_str());

    // If still not connected to a station, notify
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Could not connect to any Wi-Fi network.");
        // Optional: Handle fully offline mode here
    }

    // Start mDNS responder for easier local access (http://iot.local)
    if (MDNS.begin("iot")) {
        Serial.println("mDNS responder started\n");
    }
}

/* Static functions ----------------------------------------------------------*/

/**
 * @brief Prints a spinning animation character to indicate WiFi connection progress.
 */
void spin_load() {
    Serial.print('\b');                      // Move back one space in terminal
    Serial.print(spinChars[spinIndex]);      // Print spinner character
    spinIndex = (spinIndex + 1) % 4;         // Advance to next character
    delay(100);                              // Delay for smooth visual feedback
}

