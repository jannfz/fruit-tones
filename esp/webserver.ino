/**
 ********************************************************************************
 * @file    webserver.ino
 * @brief   Webserver setup and handle functions
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include <ESP8266WebServer.h>      // HTTP server for ESP8266
#include <ESP8266mDNS.h>           // Optional mDNS support
#include <ArduinoJson.h>           // JSON serialization/deserialization
#include "fruit_keyboard.h"        // Shared game types and data
#include "index_html.h"            // Embedded HTML UI

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/

ESP8266WebServer server(80);       // Web server listening on port 80
String play_result = "";           // Holds the most recent game result (for feedback)

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/

void handleRoot();                 // Serve the main HTML page
void handleSongList();             // Serve available song names as JSON
void handleSetSong();              // Receive song selection from client
void handleListen();               // Handle request to enter "Listen" mode
void handlePlay();                 // Handle request to enter "Play" mode
void handleFreeplay();            // Enter Freeplay mode
void handleReturn();              // Return to main menu
void handleStatus();              // Provide current play result
void handleNotFound();            // Handle unknown routes
String getSongEmojiString(const Song_t& song); // Helper to convert note sequence to emojis

/* Global functions ----------------------------------------------------------*/

/**
 * @brief Initializes and configures the web server with route handlers.
 */
void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/song-list", HTTP_GET, handleSongList);
    server.on("/setsong", HTTP_POST, handleSetSong);
    server.on("/listen", HTTP_GET, handleListen);
    server.on("/play", HTTP_GET, handlePlay);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/freeplay", HTTP_POST, handleFreeplay);
    server.on("/return", HTTP_POST, handleReturn);
    server.onNotFound(handleNotFound); // Catch-all handler for 404s

    server.begin(); // Start the HTTP server
    Serial.println("Web server started\n");
}

/**
 * @brief Sets the play result message for feedback display.
 * 
 * @param res A string such as "✅ Correct!" or "❌ Incorrect!"
 */
void sendResult(String res) {
    play_result = res;
}

/* Static functions ----------------------------------------------------------*/

/**
 * @brief Serves the main HTML page.
 */
void handleRoot() {
    server.send(200, "text/html", index_html);
}

/**
 * @brief Returns a JSON array of song names to populate the song menu.
 */
void handleSongList() {
    DynamicJsonDocument doc(512);
    JsonArray arr = doc.to<JsonArray>();

    for (const Song_t &song : Songs) {
        arr.add(song.name);
    }

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

/**
 * @brief Handles song selection from the web client.
 */
void handleSetSong() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        Serial.println("Received setsong body: " + body);

        // Very basic parsing to extract song index
        int colonIndex = body.indexOf(":");
        int value = body.substring(colonIndex + 1).toInt();
        current_song = value;

        Serial.printf("Selected song: %d - %s \n", current_song, Songs[current_song].name);
    }
    server.send(200, "text/plain", "OK");
}

/**
 * @brief Prepares and sends data for Listen mode.
 */
void handleListen() {
    DynamicJsonDocument doc(512);
    doc["name"] = Songs[current_song].name;
    doc["sequence"] = getSongEmojiString(Songs[current_song]);

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);

    menu_selection = MenuSelect::ListenM; // Update game state
}

/**
 * @brief Prepares and sends data for Play mode.
 */
void handlePlay() {
    DynamicJsonDocument doc(512);
    doc["name"] = Songs[current_song].name;

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);

    menu_selection = MenuSelect::PlayM;
}

/**
 * @brief Handles request to enter Freeplay mode.
 */
void handleFreeplay() {
    menu_selection = MenuSelect::FreeplayM;
    server.send(200, "text/plain", "Playing song");
}

/**
 * @brief Handles return-to-menu request from client.
 */
void handleReturn() {
    menu_selection = MenuSelect::ReturnM;
    server.send(200, "text/plain", "Return to menu");
}

/**
 * @brief Returns current game result (e.g. “✅ Correct!”) for UI feedback.
 */
void handleStatus() {
    DynamicJsonDocument doc(512);
    doc["result"] = play_result;

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

/**
 * @brief Fallback for unknown routes (404).
 */
void handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}

/**
 * @brief Helper to convert a song's note sequence to emoji string for UI display.
 * 
 * @param song Reference to the song
 * @return A string of fruit emojis corresponding to the song's note sequence
 */
String getSongEmojiString(const Song_t& song) {
    String emojiString = "";
    for (uint8_t i = 0; i < song.len; ++i) {
        uint8_t note = song.sequence[i];
        if (note >= 0 && note < NUM_NOTES) {
            emojiString += Notes[note].emoji;
        } else {
            emojiString += " "; // Fallback for invalid notes
        }
    }
    return emojiString;
}

