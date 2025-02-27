/**
 ****************************************************************************************************
 * @file        led.h
 * @author      ALIENTEK
 * @brief       LED code
 * @license     Copyright (C) 2020-2030, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK DNESP32S3 mini board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  :
 * version      data         notes
 * V1.0         20240430     the first version
 *
 ****************************************************************************************************
 */

#ifndef __LED_H
#define __LED_H

#include "Arduino.h"

/* Pin Definition */
#define LED_PIN       1   

/* LED operate definition */
#define LED(x)        digitalWrite(LED_PIN, x)
#define LED_TOGGLE()  digitalWrite(LED_PIN, !digitalRead(LED_PIN))

/* Function declaration */
void led_init(void);      

#endif
