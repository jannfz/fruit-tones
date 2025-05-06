#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "fruit-keyboard.h"
#include "gpio.h"



GameState currentState = GameState::MenuS;

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



    _delay_ms(2000);

    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espSetNote(0);
    _delay_ms(1000);
    espDone(0);
    


    // gameTick();
}

