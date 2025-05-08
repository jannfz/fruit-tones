/**
 ********************************************************************************
 * @file    tft.ino
 * @author  FP_3
 *          Niels-Valdemar Dahlgaard
 *          Sven Emil Rasmussen
 *          Sebastian Fiala Mikkelsen
 *          Emil Kornbeck Bøgh
 *          Jann Feilberg Zachariasen
 * @date    2025-05-06
 * @brief   Setup and display functions for TFT display
 *
 * Copyright (c) 2025 FP_3
 * 
 * This software is released under the MIT License.
 * See LICENSE file in the project root for full license information.
 ********************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include <SPI.h>                // SPI communication library
#include <Adafruit_GFX.h>       // Graphics primitives (required by TFT_eSPI)
#include <TFT_eSPI.h>           // Optimized TFT library
#include "fruit_keyboard.h"     // Note definitions and bitmaps

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/
TFT_eSPI tft = TFT_eSPI();      // TFT display object (configured via User_Setup.h)

// Center of screen and scale factor (used for custom graphics)
const int cw = tft.width()/2;      // Center width
const int ch = tft.height()/2;     // Center height
const int s = min(cw/4, ch/4);     // Scaled unit for positioning or sizing

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/

/* Global functions ----------------------------------------------------------*/

/**
 * @brief Initializes the TFT display with orientation and background color.
 */
void setupDisplay(void) {  
  tft.init();                      // Initialize TFT hardware
  tft.setRotation(1);              // Rotate screen to landscape mode
  tft.fillScreen(TFT_WHITE);       // Clear screen with white background
}


/**
 * @brief Displays a fruit bitmap centered on screen.
 * 
 * @param fruit Pointer to Note_t structure containing fruit bitmap
 */
void displayFruit(const Note_t *fruit) {
    // Draw bitmap at (60, 60) with size 120x120 using pointer from fruit
    tft.pushImage(60, 60, 120, 120, fruit->bitmap);
}


/* Static functions ----------------------------------------------------------*/
