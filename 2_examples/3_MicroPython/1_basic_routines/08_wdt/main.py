"""
 ****************************************************************************************************
 * @file        main.py
 * @author      ALIENTEK
 * @brief       WDT example
 * @license     Copyright (C) 2020-2030, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK DNESP32S3M mini board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  :
 * version      data         notes
 * V1.0         20240430     the first version
 *
 ****************************************************************************************************
"""

from machine import Pin,I2C,WDT
import time


"""
 * @brief       The application entry point
 * @param       None
 * @retval      None
"""
if __name__ == '__main__':
    
    led = Pin(1,Pin.OUT,value = 1)
    time.sleep_ms(1000)
    wdt = WDT(timeout=3000)
    led.value(0)
    
    while True:
        if key.value() == 0:
            
            time.sleep_ms(10)
            
            if key.value() == 0:
                wdt.feed()
                
                while not key.value():
                    pass

        time.sleep_ms(10)
