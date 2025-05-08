/**
 ********************************************************************************
 * @file    fruit_keyboard.h
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 * @brief   Definitions and data structures for Fruit Keyboard game logic
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

#ifndef __FRUIT_KEYBOARD_H
#define __FRUIT_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>

// Bitmap assets for TFT display
#include "inc/square_lemon_small.h"
#include "inc/square_apple_small.h"
#include "inc/square_orange_small.h"
#include "inc/square_banana_small.h"
#include "inc/square_grapes_small.h"
#include "inc/square_pear_small.h"
#include "inc/square_lime_small.h"

/* Macros and defines --------------------------------------------------------*/

/* Typedefs ------------------------------------------------------------------*/

// Enum representing high-level game states
typedef enum {
    MenuS = 0,
    ListenS,
    PlayS,
    FreeplayS,
} GameState;

// Enum representing the selected menu option
typedef enum {
    NoneM = 0,
    ListenM,
    PlayM,
    FreeplayM,
    ReturnM,
} MenuSelect;

// Enum used for error handling (e.g. serial communication)
typedef enum {
    Ok  =  0,
    Err = -1,
} Error;

// Structure to define a musical note and associated display/fruit data
typedef struct {
    const char note;        // Letter representation of note
    const uint16_t freq;    // Frequency in Hz
    const uint8_t pin;      // Output pin
    const String name;      // Fruit name
    const String emoji;     // Emoji string
    const uint16_t *bitmap; // Bitmap for display
} Note_t;

// Command types exchanged between ESP and Arduino Uno
typedef enum {
    CMD_LISTEN    = 0x41,
    CMD_PLAY      = 0x42,
    CMD_FREEPLAY  = 0x43,
    CMD_RETURN    = 0x44,
    CMD_DONE      = 0x81,
    CMD_NOTE      = 0x82,
    CMD_RESULT    = 0x83,
} Command;

// Struct representing a command sent between ESP and Uno
typedef struct {
    uint8_t cmd;
    uint8_t param;
} Command_t;

// Struct representing a song as a sequence of notes
typedef struct {
    String name;
    uint8_t len;
    uint8_t sequence[50];
} Song_t;


/* Exported variables --------------------------------------------------------*/

// List of available notes and their associated fruits and visuals
const Note_t Notes[] = {
    { 'C', 261,  4, "Lemon",    "🍋", lemon     },
    { 'D', 293,  5, "Apple",    "🍎", apple     },
    { 'E', 329,  6, "Banana",   "🍌", banana    },
    { 'F', 349,  7, "Pear",     "🍐", pear      },
    { 'G', 392,  8, "Orange",   "🍊", orange    },
    { 'A', 440, 10, "Grapes",   "🍇", grapes2   },
    { 'B', 493, 11, "Lime",     "🥝", lime      },
};

const uint8_t NUM_NOTES = sizeof(Notes) / sizeof(Notes[0]);     // Total number of defined notes

// List of available songs and their note sequences
const Song_t Songs[] = {
    { 
        "Itsy Bitsy Spider",
        13,
        {0, 0, 0, 1, 2, 2, 2, 1, 1, 1, 2, 0, 0}
    },
    { 
        "Twinkle, Twinkle, Little Star",
        42,
        {0, 0, 4, 4, 5, 5, 4, 3, 3, 2, 2, 1, 1, 0, 4, 4, 3, 3, 2, 2, 1, 4, 4,
         3, 3, 2, 2, 1, 0, 0, 4, 4, 5, 5, 4, 3, 3, 2, 2, 1, 1, 0, }
    },
};


const uint8_t NUM_SONGS = sizeof(Songs) / sizeof(Songs[0]);     // Total number of songs defined

/* Global function prototypes ------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __FRUIT_KEYBOARD_H */
