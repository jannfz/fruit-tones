#ifndef __FRUIT_KEYBOARD_H
#define __FRUIT_KEYBOARD_H

#include <Arduino.h>

enum GameState {
    MenuS = 0,
    ListenS,
    PlayS,
};

enum MenuSelect {
    NoneM = 0,
    ListenM,
    PlayM,
    ReturnM,
};

enum Error {
    Ok       =  0,
    Err      = -1,
    ComErr   = -2,
    InvParam = -3,
    NotReady = -4,
};

typedef enum Note {
    C = 0,
    D = 1,
    E = 2,
    F = 3,
    G = 4,
    A = 5,
    B = 6,
    NoNote,
} Note;

const uint8_t NUM_MODES = 2;

enum Mode {
    EasyM,
    NormalM,
    HardM,
    FreeplayM,
    ShowSongM,
    PlaySongM,
};

typedef struct {
  const char note;
  const uint16_t freq;
  const uint8_t pin;
  const String name;
  const String emoji;
} Note_t;


typedef struct {
  uint8_t cmd;
  uint8_t param;
} Command_t;

typedef struct {
    String name;
    uint8_t len;
    Note sequence[20];
} Song_t;


const uint8_t NUM_NOTES = 7;

const Note_t Notes[NUM_NOTES+1] = {
  {'C', 261,  4, "Lemon",    "🍋"},      // 0
  {'D', 293,  5, "Apple",    "🍎"},     // 1
  {'E', 329,  6, "Banana",   "🍌"},    // 2
  {'F', 349,  7, "Pear",     "🍐"},      // 3
  {'G', 392,  8, "Orange",   "🍊"},    // 4
  {'A', 440, 10, "Grapes",   "🍇"},   // 5
  {'B', 493, 11, "Lime",     "🥝"},      // 6
  {' ',   0,  0, "NoNote",   "?"},      // 6
};

const uint8_t NUM_SONGS = 2;

const Song_t Songs[NUM_SONGS] = {
    {"Song 1", 14, {C, C, G, G, A, A, G, F, F, E, E, D, D, C, NoNote}},
    {"Song 2", 14, {E, E, E, C, E, G, G, C, G, E, A, B, A, G, NoNote}},
};

#define CMD_LISTEN      0x41
#define CMD_PLAY        0x42
#define CMD_FREEPLAY    0x43
#define CMD_RETURN      0x43

#define CMD_DONE        0x81
#define CMD_NOTE        0x82




#define CMD_NEW_GAME    0x41
#define CMD_RESET       0x42         // RESET
#define CMD_MODE        0x44         // SETMODE:    0x00 - 0x01 (game, freeplay)
#define CMD_SONG        0x84         // SONG:            0x00 - 0x08
#define CMD_NOTE_OLD    0x81         // FRUIT:           0x00 - 0x07 (fruit 0 to 7)
#define CMD_PLAYED      0x82         // CORRECT:         0x00 - 0x07 (fruit 0 to 7)
#define CMD_SCORE       0x83         // SCORE:           0x0000 - 0x7FFF
#define CMD_GAMEOVER    0x85         // GAMEOVER
                                     //


#define CMD_NEW_POS   0
#define CMD_SONG_POS    1
#define CMD_MODE_POS    2




#endif
