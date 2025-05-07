#include <ESP8266WiFi.h>
#include "secrets.h"

const char spinChars[] = { '|', '/', '-', '\\' };

int spinIndex = 0;

void spin_load() {
    Serial.print('\b');                  // overwrite current line
    Serial.print(spinChars[spinIndex]);
    spinIndex = (spinIndex + 1) % 4;
    delay(100);
}

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

    WiFi.softAP("FruitTones");  // No password = open network

    Serial.printf("AP IP address:  %s \n", WiFi.softAPIP().toString().c_str());

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Could not connect to any Wi-Fi network.");
        // Optionally start AP mode here
    }
    if (MDNS.begin("iot")) {
        Serial.println("mDNS responder started\n");
    }
}
