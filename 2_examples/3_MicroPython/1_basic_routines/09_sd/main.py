"""
 ****************************************************************************************************
 * @file        main.py
 * @author      ALIENTEK
 * @brief       SD example
 * @license     Copyright (C) 2020-2030, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK DNESP32S3 Mini board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  :
 * version      data         notes
 * V1.0         20240430     the first version
 *
 ****************************************************************************************************
"""

from machine import Pin,SPI,I2C
from sdcard import SDCard
import st7735
import time
import uos


"""
 * @brief       The application entry point
 * @param       None
 * @retval      None
"""
if __name__ == '__main__':
    
    x = 0
    # Initialize SPI communication with specified parameters and pins
    spi = SPI(2, baudrate=80000000, sck=Pin(12), mosi=Pin(11), miso=Pin(13))
    tft = st7735.atk_tft(spi,40,38,39,41,rotate = 1) #DC, Reset, CS, BL, rotate
    sd = SDCard(spi,Pin(2,Pin.OUT))
    # Display experiment information
    tft.p_string(0,0,"ESP32-S3")
    tft.p_string(0,12,"ATOM@ALIENTEK")
    tft.p_string(0,24,"File Read:")
    # Mount SD card
    uos.mount(sd,'/sd')
    # List directory contents
    print('system directory:{}'.format(uos.listdir()))
    with open("/sd/test.txt", "w") as f:
            f.write(str("Hello ALIENTEK"))

    # Read the content of the test.txt file from the SD card directory
    with open("/sd/test.txt", "r") as f:
        data = f.read()
    
    tft.p_string(60, 24, str(data))
    # Eject SD card
    uos.umount('/sd')
    