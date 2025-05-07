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
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include "fruit_keyboard.h"

/* Macros and defines --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Static variables ----------------------------------------------------------*/
TFT_eSPI tft = TFT_eSPI();

const int cw = tft.width()/2;
const int ch = tft.height()/2;
const int s = min(cw/4,ch/4);

/* Global variables ----------------------------------------------------------*/

/* Static function prototypes-------------------------------------------------*/

/* Global functions ----------------------------------------------------------*/

void setupDisplay(void) {  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
}

void displayFruit(const Note_t *fruit) {
    tft.pushImage(60,60, 120, 120, fruit->bitmap);
}

/* Static functions ----------------------------------------------------------*/
