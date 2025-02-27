/**
 ****************************************************************************************************
 * @file        01_led.ino
 * @author      ALIENTEK
 * @brief       LED example
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
 * @brief    Initialize function
 * @param    None
 * @retval   None
 */
void setup() 
{
    led_init();   /* led initialization */
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    LED(0);       /* sets led_pin off */
    delay(500);   /* waits for 500ms */
    LED(1);       /* sets led_pin on */
    delay(500);   /* waits for 500ms */

    /* The effect of the above four lines of code can also be achieved with the following two lines of code
    LED_TOGGLE(); // Toggles it HIGH and LOW
    delay(500);   // waits for 500ms
    */
}
