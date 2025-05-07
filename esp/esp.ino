/**
 ********************************************************************************
 * @file    esp.ino
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 * @brief   Enterence file with game state machine
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include <ESP8266WebServer.h>
#include "fruit_keyboard.h"

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/
extern ESP8266WebServer server;

uint8_t current_song = 0;
uint8_t current_mode = 0;
uint8_t current_note = 0;

GameState current_state = GameState::MenuS;
MenuSelect menu_selection = MenuSelect::NoneM;

Command_t received_command;

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/

/* Global functions ----------------------------------------------------------*/


void setup() {
    Serial.begin(115200);

    unoSetup(9600);

    setupDisplay();

    delay(10);

    Serial.println("\n======== Fruit Tones ========\n");

    connect_to_WiFi();
    setupWebServer();

}

void loop() {

    server.handleClient();

    switch (current_state) {
        case GameState::MenuS:

            switch (menu_selection) {
                case MenuSelect::NoneM:
                    break;

                case MenuSelect::ListenM:
                    Serial.println("State: Listen");
                    unoStartListen(current_song);
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::ListenS;
                    unoFlush();
                    break;

                case MenuSelect::PlayM:
                    Serial.println("State: Play");
                    unoStartPlay(current_song);
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::PlayS;
                    unoFlush();
                    break;

                case MenuSelect::FreeplayM:
                    Serial.println("State: Freeplay");
                    unoSendCommand(CMD_FREEPLAY);
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::FreeplayS;
                    unoFlush();
                    break;
            }
            break;

        case GameState::ListenS:
            if (unoGetData(&received_command) == Error::Ok) {

                if (received_command.cmd == CMD_DONE) {
                    Serial.println("State: Menu");
                    Serial.println("DONE!");
                    current_state = GameState::MenuS;
                    break;
                } else if (received_command.cmd == CMD_NOTE) {
                    current_note = received_command.param;
                    Serial.print("Note: ");
                    Serial.println(Notes[current_note].name);
                    displayFruit(&Notes[current_note]);
                    break;
                }
            }
            if (menu_selection == MenuSelect::ReturnM) {
                Serial.println("Returning to menu...");
                menu_selection = MenuSelect::NoneM;
                break;
            }
            break;

        case GameState::PlayS:
            if (unoGetData(&received_command) == Error::Ok) {
                if (received_command.cmd == CMD_RESULT) {
                    if (received_command.param == 1) {
                        sendResult("✅ Correct!");
                        Serial.println("✅ Correct!");
                    } else {
                        sendResult("❌ incorrect!");
                        Serial.println("❌ incorrect!");
                    }
                    break;
                } else if (received_command.cmd == CMD_DONE) {
                    Serial.println("DONE!");
                    Serial.println("State: Menu");
                    current_state = GameState::MenuS;
                } else if (received_command.cmd == CMD_NOTE) {
                    current_note = received_command.param;
                    Serial.print("Note: ");
                    Serial.println(Notes[current_note].name);
                    displayFruit(&Notes[current_note]);
                    break;
                }
            }
            if (menu_selection == MenuSelect::ReturnM) {
                Serial.println("Returning to menu...");
                sendResult(" ");
                menu_selection = MenuSelect::NoneM;
                break;
            }
            break;
        case GameState::FreeplayS:
            if (unoGetData(&received_command) == Error::Ok) {
                if (received_command.cmd == CMD_NOTE) {
                    current_note = received_command.param;
                    Serial.print("Note: ");
                    Serial.println(Notes[current_note].name);
                    displayFruit(&Notes[current_note]);
                    break;
                }
            }
            if (menu_selection == MenuSelect::ReturnM) {
                Serial.println("Returning to menu...");
                menu_selection = MenuSelect::NoneM;
                current_state = GameState::MenuS;
                unoSendCommand(CMD_RETURN);
                break;
            }
            break;
    }
}

/* Static functions ----------------------------------------------------------*/
