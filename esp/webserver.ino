#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include "fruit_keyboard.h"
#include "index_html.h"

// Web server instance
ESP8266WebServer server(80);

String play_result = "";

void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/song-list", HTTP_GET, handleSongList);
    server.on("/setsong", HTTP_POST, handleSetSong);
    server.on("/listen", HTTP_GET, handleListen);
    server.on("/play", HTTP_GET, handlePlay);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/freeplay", HTTP_POST, handleFreeplay);
    server.on("/return", HTTP_POST, handleReturn);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Web server started\n");
}

void handleRoot() {
    server.send(200, "text/html", index_html);
}

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

String getSongEmojiString(const Song_t& song) {
    String emojiString = "";
    for (uint8_t i = 0; i < song.len; ++i) {
        Note note = song.sequence[i];
        if (note >= 0 && note < NUM_NOTES) {
            emojiString += Notes[note].emoji;
        } else {
            emojiString += " "; // Optional: add space or skip for NoNote
        }
    }
    return emojiString;
}

void handleSetSong() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        Serial.println("Received setsong body: " + body);

        // super basic way without ArduinoJson:
        int colonIndex = body.indexOf(":");
        int value = body.substring(colonIndex + 1).toInt();
        current_song = value;

        Serial.printf("Selected song: %d - %s \n", current_song, Songs[current_song].name);
        Serial.println(Songs[current_song].name);
    }
    server.send(200, "text/plain", "OK");
}

void sendResult(String res) {
    play_result = res;
}

void handleListen() {

    DynamicJsonDocument doc(512);
    doc["name"] = Songs[current_song].name;
    doc["sequence"] = getSongEmojiString(Songs[current_song]);

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);

    menu_selection = MenuSelect::ListenM;
}

void handlePlay() {

    DynamicJsonDocument doc(512);
    doc["name"] = Songs[current_song].name;

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);

    menu_selection = MenuSelect::PlayM;
}

void handleFreeplay() {
    menu_selection = MenuSelect::FreeplayM;
    server.send(200, "text/plain", "Playing song");
}

void handleReturn() {
    menu_selection = MenuSelect::ReturnM;
    server.send(200, "text/plain", "Return to menu");
}

void handleStatus() {

    DynamicJsonDocument doc(512);
    doc["result"] = play_result;

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

void handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}
