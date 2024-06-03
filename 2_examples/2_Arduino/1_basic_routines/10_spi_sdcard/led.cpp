/**
 ****************************************************************************************************
 * @file        led.cpp
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

#include "led.h"

/**
 * @brief       Configure led pins
 * @param       None
 * @retval      None
 */
void led_init(void) 
{
    pinMode(LED_PIN, OUTPUT);     /* sets LED_PIN as output */
    digitalWrite(LED_PIN, HIGH);  /* Combined with Board_SCH, sets LED_PIN on, LED is off */
}


