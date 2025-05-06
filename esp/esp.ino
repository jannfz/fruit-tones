#include <ESP8266WebServer.h>
#include "fruit_keyboard.h"

extern ESP8266WebServer server;

uint8_t current_song = 0;
uint8_t current_mode = 0;
uint8_t current_note = 0;

GameState current_state = GameState::MenuS;
MenuSelect menu_selection = MenuSelect::NoneM;

void setup() {
    Serial.begin(115200);

    unoSetup(9600);

    setupDisplay();

    delay(10);

    connectToWiFi();
    setupWebServer();
}

void loop() {

    server.handleClient();

    int8_t unoData = unoGetData();

    switch (current_state) {
        case GameState::MenuS:

            switch (menu_selection) {
                case MenuSelect::NoneM:
                    break;

                case MenuSelect::ListenM:
                    unoStartListen(current_song);
                    Serial.println("Listening to song...");
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::ListenS;
                    break;

                case MenuSelect::PlayM:
                    Serial.println("Playing song...");
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::PlayS;
                    break;
            }
            break;

        case GameState::ListenS:

            if (unoData == CMD_DONE) {
                Serial.println("Song is done...");
                current_state = GameState::MenuS;
                break;
            } else if (menu_selection == MenuSelect::ReturnM) {
                    Serial.println("Returning to menu...");
                    menu_selection = MenuSelect::NoneM;
            }

            if (unoData >= 0) {
                current_note = unoData;
                Serial.println(Notes[current_note].name);
                displayFruit(&Notes[current_note]);
            }
            break;

        case GameState::PlayS:
            if (current_note == Note::NoNote) {
                Serial.println("Song is done...");
                current_state = GameState::MenuS;
            } else if (menu_selection == MenuSelect::ReturnM) {
                
                    Serial.println("Returning to menu...");
                    menu_selection = MenuSelect::NoneM;
            }
            break;

    }
}

