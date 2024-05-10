"""
 ****************************************************************************************************
 * @file        main.py
 * @author      ALIENTEK
 * @brief       RTC example
 * @license     Copyright (C) 2020-2030, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK DNESP32S3 board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  :
 * version      data         notes
 * V1.0         20240430     the first version
 *
 ****************************************************************************************************
"""

from machine import Pin,RTC,SPI,I2C
import st7735
import time


"""
 * @brief       The application entry point
 * @param       None
 * @retval      None
"""
if __name__ == '__main__':
    
    # Set pin high on creation
    led = Pin(1,Pin.OUT,value = 1)
    spi = SPI(2, baudrate=80000000, sck=Pin(12), mosi=Pin(11), miso=Pin(13))
    tft = st7735.atk_tft(spi,40,38,39,41,rotate = 1) #DC, Reset, CS, BL, rotate
    time.sleep_ms(100)
    # Display experiment information on LCD
    tft.p_string(0,0,"ESP32-S3")
    tft.p_string(0,12,"ATOM@ALIENTEK")
    tft.p_string(0,24, "Specific:")
    tft.p_string(0,36, "Time:")
    tft.p_string(0,48, "Date:")
    # Initialize Real-Time Clock (RTC)
    rtc = RTC()
    
    if rtc.datetime()[0] != 2023:
        rtc.datetime((2023, 8, 15, 2, 0, 0, 0, 0))
    
    while True:

        data_time = rtc.datetime()
        tft.p_string(60, 24, str(data_time[0:3]))
        tft.p_string(30, 36, str(data_time[4:7]))
        tft.p_string(30, 48, str(int(data_time[3]) + 1))
