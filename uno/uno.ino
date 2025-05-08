
// Include I2C library for communication with peripherals like OLED display
#include <Wire.h>

// Include core graphics functions
#include <Adafruit_GFX.h>

// Include driver for SSD1306 OLED display
#include <Adafruit_SSD1306.h>

// Include library to allow additional serial ports via software
#include <SoftwareSerial.h>

// Include library to read values from capacitive sensors
#include <CapacitiveSensor.h>

// -----------------------------
// Serial communication pins
// -----------------------------
const byte RX_PIN = 2;                    // Pin to receive data from ESP8266
const byte TX_PIN = 3;                    // Pin to send data to ESP8266
SoftwareSerial espSerial(RX_PIN, TX_PIN); // Create software serial port using RX_PIN and TX_PIN

// ---------------------------------
// Capacitive sensor configuration
// ---------------------------------
#define CAP_BUTTON_COUNT 7               // Number of capacitive "fruit" sensors
int fruitState[CAP_BUTTON_COUNT];         // Array to track press state (0 or 1) for each sensor

// ---------------------------------
// Melody sequences definitions
// ---------------------------------
// Sequence for level 1 melody (Lille Peter Edderkop)
const int level1_sequence[] = {0,0,0,1,2,2,2,1,1,1,2,0,0};

// Sequence for level 2 melody (Twinkle Twinkle Little Star)
const int level2_sequence[] = {0,0,4,4,5,5,4,3,3,2,2,1,1,0, 4,4,3,3,2,2,1, 4,4,3,3,2,2,1, 0,0,4,4,5,5,4,3,3,2,2,1,1,0};


// Calculate lengths of arrays at compile time
const int sequence_length_1 = sizeof(level1_sequence) / sizeof(level1_sequence[0]);
const int sequence_length_2 = sizeof(level2_sequence) / sizeof(level2_sequence[0]);

// -----------------------------
// Command definitions (ESP->Arduino)
// -----------------------------
const char CMD_LISTEN    = 0x41; // ASCII 'A': request to show melody only
const char CMD_PLAY      = 0x42; // ASCII 'B': request to play melody and wait for repeat
const char CMD_FREE_PLAY = 0x43; // ASCII 'C': enter free play mode
const char ESC           = 0x44; // ASCII 'D': exit free play mode

// Placeholder names for fruits 
const char* fruitNames[CAP_BUTTON_COUNT] = {"lemon","apple","orange","banana","grape","pear","lime"};

// --------------------------------
// Pin definitions for buzzer
// --------------------------------
const int buzzer = 9;                                    // PWM pin driving the piezo buzzer

// --------------------------------
// Scales: major (dur) and minor (mol)
// --------------------------------
// Major scale frequencies: C4, D4, E4, F4, G4, A4, B4
const int tonesMajor[CAP_BUTTON_COUNT] = {261,293,329,349,392,440,493};
// Natural minor scale frequencies: C4, D4, Eb4, F4, G4, Ab4, Bb4
const int tonesMinor[CAP_BUTTON_COUNT] = {261,293,311,349,392,415,466};
// Pointer to the currently active scale (start with major)
const int* activeTones = tonesMajor;

// -----------------------------
// Photoresistor setup
// -----------------------------
const int LDR_PIN = A3;          // Analog pin connected to LDR voltage divider
const int LIGHT_THRESH = 40;     // Threshold to decide between major and minor
bool useMinor = false;           // Flag to select minor scale if true

// --------------------------------
// CapacitiveSensor objects using common send pin 14, initialising the fruits as buttons
// --------------------------------
CapacitiveSensor cs_14_15(14,15); // Sensor channel 1 (receive pin 15)
CapacitiveSensor cs_14_16(14,16); // Sensor channel 2 (receive pin 16)
CapacitiveSensor cs_14_5 (14,5 ); // Sensor channel 3 (receive pin 5)
CapacitiveSensor cs_14_18(14,18); // Sensor channel 4 (receive pin 18)
CapacitiveSensor cs_14_19(14,19); // Sensor channel 5 (receive pin 19)
CapacitiveSensor cs_14_6 (14,6 ); // Sensor channel 6 (receive pin 6)
CapacitiveSensor cs_14_7 (14,7 ); // Sensor channel 7 (receive pin 7)

// Variables to hold raw sensor readings
long total1, total2, total3, total4, total5, total6, total7;
// Variables to hold debounced state of each sensor
int fruit1,fruit2,fruit3,fruit4,fruit5,fruit6,fruit7;
// Debounce flags to detect new presses
int var1,var2,var3,var4,var5,var6,var7;

// -----------------------------
// State machine
// -----------------------------
enum state {recieve_data, show_song, play_song, free_play}; // all the states used for the program
state currentstate = recieve_data; // Start by waiting for command
int currentLevel = 1;              // Default to level 1 melody

// --------------------------------
// setup(): runs once at startup
// --------------------------------
void setup() {
  Serial.begin(9600);        // Start USB serial for debug
  espSerial.begin(9600);     // Start software serial to ESP8266
  pinMode(buzzer, OUTPUT);   // Configure buzzer pin as output
}

// --------------------------------
// loop(): main program loop
// --------------------------------
void loop() {
  // 1) Read light sensor to decide scale
  int res_value = analogRead(LDR_PIN);    // Read LDR value 0-1023
  useMinor = (res_value < LIGHT_THRESH);  // Below threshold => minor scale
  activeTones = useMinor ? tonesMinor : tonesMajor; // Switch scale
  

  // 2) Update states of capacitive fruit sensors
  fruitToDigital();

  // 3) State machine handler
  switch (currentstate) {

    case recieve_data:
      // If ESP sent a command, read and dispatch
      if (espSerial.available() >= 2) {
        char cmd = espSerial.read();
        char param = espSerial.read();
        Serial.println(cmd);
        if (cmd == CMD_LISTEN) {
          Serial.println("currentstate = show_song");
          currentLevel = param;
          currentstate = show_song;
        }
        else if (cmd == CMD_PLAY) {
          Serial.println("currentstate = play_song");
          currentLevel = param;
          currentstate = play_song;
        }
        else if (cmd == CMD_FREE_PLAY) {
          Serial.println("currentstate = free_play");
          currentstate = free_play;
        }
      }
      break;

    case show_song:
      // Short delay so user can prepare
      delay(1000);
      // Play melody without requiring user input
      showSequence();
      // Notify ESP that playback finished (0x81)
      espSerial.write(0x81);
      Serial.println("show done");
      // Return to wait-for-command
      currentstate = recieve_data;
      break;

    case play_song: {
      // 1) Select sequence and length based on level
      int seqLen = (currentLevel == 1) ? sequence_length_1 : sequence_length_2;
      const int* seq = (currentLevel == 1) ? level1_sequence : level2_sequence;

      // 2) Prompt user to play
      Serial.println("Play the sequence:");
      int userSeq[seqLen];
      // Collect user presses
      for (int i = 0; i < seqLen; i++) {
        bool press = false;
        while (!press) {
          int button = waitForButtonPress();
          if (button >= 0) {
            userSeq[i] = button;
            press = true;
          }
        }
      }

      // 3) Play correct sequence for comparison
      delay(1000);
      Serial.println("Correct melody:");
      playSequence(seq, seqLen);

      // 4) Play back user's attempt
      delay(1000);
      Serial.println("Your attempt:");
      playSequence(userSeq, seqLen);

      // 5) Check if sequences match
      bool allCorrect = true;
      for (int i = 0; i < seqLen; i++) {
        if (userSeq[i] != seq[i]) { allCorrect = false; break; }
      }

      // 6) Provide feedback and return to waiting state
      if (allCorrect) {
        Serial.println("Correct!");
        espSerial.write(0x83); // start signal
        espSerial.write(0x01); // correct signal
      } else {
        Serial.println("Wrong");
        espSerial.write(0x83); // start signal
        espSerial.write((uint8_t)0x00); // wrong signal
      }
      // Notify ESP that play song finished (0x81)
      espSerial.write(0x81);
      currentstate = recieve_data;
    }
    break;

    case free_play:
      // Keep playing freely until ESC is received
      while (true) {
        // Check for ESC to exit
        if (espSerial.available() && espSerial.read() == ESC) {
          espSerial.read();
          Serial.println("ESC");
          currentstate = recieve_data;
          break;
        }
        // Wait for a fruit press
        int idx = waitForButtonPress();
        if(idx >= 0) {
          espSerial.write(0x82);
          espSerial.write(idx);
        }
      }
      break;
  }
}

// --------------------------------
// showSequence(): plays melody and sends markers
// --------------------------------
void showSequence() {
  const int* seq = (currentLevel == 1) ? level1_sequence : level2_sequence;
  int len = (currentLevel == 1) ? sequence_length_1 : sequence_length_2;
  for (int i = 0; i < len; i++) {
    // marker before each note
    espSerial.write(0x82);
    espSerial.write(seq[i]);
    // play note from active scale
    tone(buzzer, activeTones[seq[i]], 500);
    delay(600);
    noTone(buzzer);
    delay(200);
  }
}

// --------------------------------
// playSequence(): plays arbitrary sequence of notes
// --------------------------------
void playSequence(const int* seq, int len) {
  for (int i = 0; i < len; i++) {
    espSerial.write(0x82);
    espSerial.write(seq[i]);
    tone(buzzer, activeTones[seq[i]], 500);
    delay(600);
    noTone(buzzer);
    delay(200);
  }
}

// --------------------------------
// fruitToDigital(): updates fruitState[] using hysteresis
// --------------------------------
void fruitToDigital() {
  total1 = cs_14_15.capacitiveSensor(33);
  if (total1 > 1500 && !var1) { fruit1 = 1; var1 = 1; }
  else if (total1 < 1200 && var1) { fruit1 = 0; var1 = 0; }

  total2 = cs_14_16.capacitiveSensor(30);
  if (total2 > 1500 && !var2) { fruit2 = 1; var2 = 1; }
  else if (total2 < 1200 && var2) { fruit2 = 0; var2 = 0; }

  total3 = cs_14_5.capacitiveSensor(30);
  if (total3 > 1500 && !var3) { fruit3 = 1; var3 = 1; }
  else if (total3 < 1200 && var3) { fruit3 = 0; var3 = 0; }

  total4 = cs_14_18.capacitiveSensor(30);
  if (total4 > 1500 && !var4) { fruit4 = 1; var4 = 1; }
  else if (total4 < 1200 && var4) { fruit4 = 0; var4 = 0; }

  total5 = cs_14_19.capacitiveSensor(30);
  if (total5 > 1500 && !var5) { fruit5 = 1; var5 = 1; }
  else if (total5 < 1200 && var5) { fruit5 = 0; var5 = 0; }

  total6 = cs_14_6.capacitiveSensor(30);
  if (total6 > 1500 && !var6) { fruit6 = 1; var6 = 1; }
  else if (total6 < 1200 && var6) { fruit6 = 0; var6 = 0; }

  total7 = cs_14_7.capacitiveSensor(30);
  if (total7 > 1500 && !var7) { fruit7 = 1; var7 = 1; }
  else if (total7 < 1200 && var7) { fruit7 = 0; var7 = 0; }

  fruitState[0] = fruit1;
  fruitState[1] = fruit2;
  fruitState[2] = fruit3;
  fruitState[3] = fruit4;
  fruitState[4] = fruit5;
  fruitState[5] = fruit6;
  fruitState[6] = fruit7;
}

// --------------------------------
// waitForButtonPress(): blocks until a sensor press is detected, returns index
// --------------------------------
int waitForButtonPress() {
    fruitToDigital();                   // refresh sensor states
    for (int i = 0; i < CAP_BUTTON_COUNT; i++) {
      if (fruitState[i]) {              // if pressed
        tone(buzzer, activeTones[i], 300); // play feedback tone
        delay(300);
        noTone(buzzer);
        // wait until sensor is released
        while (fruitState[i]) { fruitToDigital(); }
        return i;                       // return touched index
      }
    }
    return -1;
}
