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

extern ESP8266WebServer server;             // Web server instance from another webserver.ino

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

// Variables for current configuration
uint8_t current_song = 0;                       // ID of the selected song
uint8_t current_note = 0;                       // Last note received

GameState current_state = GameState::MenuS;     // Current state of Game
MenuSelect menu_selection = MenuSelect::NoneM;  // Current menu selection

Command_t received_command;                     // Command from Uno

/* Static function prototypes-------------------------------------------------*/

/* Global functions ----------------------------------------------------------*/

/**
 * @brief  Sends a command to the Arduino Uno via serial.
 * 
 * @param  cmd  Command ID to send (defined in protocol, e.g., CMD_PLAY)
 * 
 * @retval void
 * 
 * @details
 * This function writes a single byte to the Uno to initiate a game mode or action.
 * It assumes the Uno is already initialized via `unoSetup()`.
 */

/**
 * @brief Setup function that runs once on ESP boot.
 */
void setup() {
    Serial.begin(115200);           // Start serial communication with debug baudrate
    Serial.println("\n======== Fruit Tones ========\n");  // Welcome message

    unoSetup(9600);                 // Initialize serial connection to Arduino Uno

    setupDisplay();                 // Initialize OLED or LCD display

    connect_to_WiFi();              // Connect ESP to WiFi network

    setupWebServer();               // Initialize HTTP server routes and start server
}

/**
 * @brief Main loop that runs repeatedly.
 *        Implements the state machine for the game logic.
 */
void loop() {
    server.handleClient();          // Handle incoming HTTP requests

    switch (current_state) {

        /* ================= MENU STATE ================= */
        case GameState::MenuS:
            switch (menu_selection) {
                case MenuSelect::NoneM:
                    break;  // Do nothing until a menu option is selected

                case MenuSelect::ListenM:
                    Serial.println("State: Listen");
                    unoStartListen(current_song);        // Tell Uno to begin listen mode
                    menu_selection = MenuSelect::NoneM;  // Clear selection
                    current_state = GameState::ListenS;  // Transition to Listen state
                    unoFlush();                          // Clear Uno command buffer
                    break;

                case MenuSelect::PlayM:
                    Serial.println("State: Play");
                    unoStartPlay(current_song);          // Tell Uno to begin play mode
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::PlayS;
                    unoFlush();
                    break;

                case MenuSelect::FreeplayM:
                    Serial.println("State: Freeplay");
                    unoSendCommand(CMD_FREEPLAY);        // Start freeplay mode on Uno
                    menu_selection = MenuSelect::NoneM;
                    current_state = GameState::FreeplayS;
                    unoFlush();
                    break;
            }
            break;

        /* ================= LISTEN STATE ================= */
        case GameState::ListenS:
            if (unoGetData(&received_command) == Error::Ok) {
                if (received_command.cmd == CMD_DONE) {
                    Serial.println("State: Menu");
                    Serial.println("DONE!");
                    current_state = GameState::MenuS;    // Back to menu after sequence
                    break;
                } else if (received_command.cmd == CMD_NOTE) {
                    current_note = received_command.param;
                    Serial.print("Note: ");
                    Serial.println(Notes[current_note].name);  // Show note name
                    displayFruit(&Notes[current_note]);        // Show corresponding fruit
                    break;
                }
            }
            if (menu_selection == MenuSelect::ReturnM) {
                Serial.println("Returning to menu...");
                menu_selection = MenuSelect::NoneM;
                break;  // Return to menu without notifying Uno (passive return)
            }
            break;

        /* ================= PLAY STATE ================= */
        case GameState::PlayS:
            if (unoGetData(&received_command) == Error::Ok) {
                if (received_command.cmd == CMD_RESULT) {
                    // Feedback on player's note match
                    if (received_command.param == 1) {
                        sendResult("✅ Correct!");
                        Serial.println("Correct!");
                    } else {
                        sendResult("❌ Incorrect!");
                        Serial.println("Incorrect!");
                    }
                    break;
                } else if (received_command.cmd == CMD_DONE) {
                    Serial.println("DONE!");
                    Serial.println("State: Menu");
                    current_state = GameState::MenuS;    // End of play mode
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
                sendResult(" ");                          // Clear feedback on screen
                menu_selection = MenuSelect::NoneM;
                break;
            }
            break;

        /* ================= FREEPLAY STATE ================= */
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
                unoSendCommand(CMD_RETURN);              // Notify Uno to stop freeplay
                break;
            }
            break;
    }
}

/* Static functions ----------------------------------------------------------*/
