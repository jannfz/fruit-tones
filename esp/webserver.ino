#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "secrets.h"
#include "fruit_keyboard.h"
#include "index_html.h"

// Web server instance
ESP8266WebServer server(80);


void connectToWiFi() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi!");

  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {
    Serial.println("mDNS responder started");
  }
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/setsong", HTTP_POST, handleSetSong);
  // server.on("/setmode", HTTP_POST, handleSetMode);
  server.on("/listen", HTTP_POST, handleListen);
  server.on("/play", HTTP_POST, handlePlay);
  server.on("/return", HTTP_POST, handleReturn);
  // server.on("/start", HTTP_POST, handleStart);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Server started");
}

void handleRoot() {
  server.send(200, "text/html", index_html);
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

void handleStatus() {
  String emoji     = Notes[current_note].emoji;
  String name      = Notes[current_note].name;
  String songName  = Songs[current_song].name;
  String songSeq   = getSongEmojiString(Songs[current_song]);

  // Escape any double quotes (unlikely in emoji, but safe for names)
  name.replace("\"", "\\\"");
  songName.replace("\"", "\\\"");
  songSeq.replace("\"", "\\\"");

  String json = "{";
  json += "\"emoji\": \""       + emoji + "\", ";
  json += "\"name\": \""        + name + "\", ";
  json += "\"songName\": \""    + songName + "\", ";
  json += "\"songSeq\": \""     + songSeq + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void handleSetSong() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    Serial.println("Received setsong body: " + body);

    // super basic way without ArduinoJson:
    int colonIndex = body.indexOf(":");
    int value = body.substring(colonIndex + 1).toInt();
    current_song = value;

    Serial.print("Selected song: ");
    Serial.println(Songs[current_song].name);
  }
  server.send(200, "text/plain", "OK");
}

// void handleSetMode() {
//   if (server.hasArg("plain")) {
//     String body = server.arg("plain");
//     Serial.println("Received setmode body: " + body);
// 
//     int colonIndex = body.indexOf(":");
//     int value = body.substring(colonIndex + 1).toInt();
//     current_mode = value;
// 
//     Serial.print("Selected mode: ");
//     Serial.println(current_mode);
//   }
//   server.send(200, "text/plain", "OK");
// }

// void handleStart() {
//   Serial.println("Starting game...");
//   state = GameState::GameOnS;
//   server.send(200, "text/plain", "Game started");
// }

void handleListen() {
  menu_selection = MenuSelect::ListenM;
  server.send(200, "text/plain", "Listening to song");
}

void handlePlay() {
  menu_selection = MenuSelect::PlayM;
  server.send(200, "text/plain", "Playing song");
}

void handleReturn() {
  menu_selection = MenuSelect::ReturnM;
  server.send(200, "text/plain", "Return to menu");
}

