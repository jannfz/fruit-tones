#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "FruitKeyboard.h"
#include "gpio.h"



const int sequence_length_1 = sizeof(level1_sequence) / sizeof(level1_sequence[0]); // length of sequence for Level 1
const int sequence_length_2 = sizeof(level2_sequence) / sizeof(level2_sequence[0]); // length of sequence for Level 2

enum state {
    RecieveData,
    ShowSong,
    Play_song,
    Started,
    Stopped
};

state currentState = recieve_data;

int currentLevel = 1; // Starting with Level 1


void setup() {

    Serial.begin(115200);
    espSetup(9600);

    for (int i = 0; i < NUM_NOTES; i++) {
        pinMode(Notes[i].pin, INPUT_PULLUP);
    }

    pinMode(BUZZER, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

}

void loop() {
    gameTick();
}

