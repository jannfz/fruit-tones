/**
 ********************************************************************************
 * @file    gpio.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>

/* Macros and defines --------------------------------------------------------*/

#define UNO_RX_PIN D1
#define UNO_TX_PIN D2

/* Typedefs ------------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Global function prototypes ------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
