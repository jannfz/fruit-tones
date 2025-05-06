#ifndef __FRUIT_KEYBOARD_H
#define __FRUIT_KEYBOARD_H

#include <Arduino.h>

#define CMD_NEW         0x01
#define CMD_RESET       0x02         // RESET
#define CMD_MODE        0x04         // SETMODE:    0x00 - 0x01 (game, freeplay)
#define CMD_SONG        0x84         // SONG:            0x00 - 0x08
#define CMD_NOTE        0x81         // FRUIT:           0x00 - 0x07 (fruit 0 to 7)
#define CMD_PLAYED      0x82         // CORRECT:         0x00 - 0x07 (fruit 0 to 7)
#define CMD_SCORE       0x83         // SCORE:           0x0000 - 0x7FFF
#define CMD_GAMEOVER    0x85         // GAMEOVER

#define CMD_NEW_POS   0
#define CMD_SONG_POS    1
#define CMD_MODE_POS    2

const uint8_t NUM_MODES = 6;
const uint8_t NUM_NOTES = 7;
const uint8_t NUM_SONGS = 2;


typedef struct {
    char note;
    uint16_t freq;
    uint8_t pin;
    String name;
    String emoji;
} Note_t;


typedef struct {
    String name;
    uint8_t len;
    uint8_t sequence;
} Song_t;


enum Mode {
    Easy,
    Normal,
    Hard,
    Freeplay,
    ShowSong,
    PlaySong,
};


enum State {
    Menu = 0,
    ShowSong,
    PlaySong,
    GameOn,
    GameOver,
};

enum Error {
    Ok       =  0,
    Err      = -1,
    ComErr   = -2,
    InvParam = -3,
    NotReady = -4,
};

typedef enum {
    C = 0,
    D = 1,
    E = 2,
    F = 3,
    G = 4,
    A = 5,
    B = 6,
} Note;


const Note_t Notes[NUM_NOTES] = {
    {'C', 261,  4, "Citron",    "🍋"},     // 0
    {'D', 293,  5, "Banan",     "🍌"},     // 1
    {'E', 329,  6, "Æble",      "🍎"},     // 2
    {'F', 349,  7, "Fersken",   "🍑"},     // 3
    {'G', 392,  8, "Vindrue",   "🍇"},     // 4
    {'A', 440, 10, "Appelsin",  "🍊"},     // 5
    {'B', 493, 11, "Banan",     "🍌"},     // 6
};


const Song_t Songs[NUM_SONGS] = {
    {"Song 1", {C, C, G, G, A, A, G, F, F, E, E, D, D, C}},
    {"Song 2", {E, E, E, C, E, G, G, C, G, E, A, B, A, G}},
};


#endif
