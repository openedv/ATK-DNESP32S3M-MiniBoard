/**
 ****************************************************************************************************
 * @file        08_rtc.ino
 * @author      ALIENTEK
 * @brief       RTC example
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
#include "uart.h"
#include <SPI.h>
#include "TFT_eSPI.h"
#include <ESP32Time.h>    /* The ESP32Time library needs to be installed */


TFT_eSPI myGLCD = TFT_eSPI();       /* define the TFT eSPI object myGLCD */
ESP32Time rtc;
uint8_t tbuf[100];        /* Store RTC information */

/**
 * @brief    Initialize function
 * @param    None
 * @retval   None
 */
void setup() 
{
    led_init();                             /* led initialization */
    uart_init(0, 115200);                   /* uart0 initialization */
    myGLCD.init();                          /* lcd initialization */
    myGLCD.setRotation(1);                  /* set the orientation of the screen (landscape) */
    myGLCD.fillScreen(TFT_WHITE);           /* lcd clear */
    rtc.setTime(00, 51, 17, 1, 12, 2023);   /* December 1, 2023, 17:52:00 */

    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("RTC TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    struct tm timeinfo = rtc.getTimeStruct();

    sprintf((char *)tbuf, "Time:%02d:%02d:%02d Week:%d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_wday);     
    myGLCD.drawString((char *)tbuf, 0, 48, 2);
    sprintf((char *)tbuf, "Date:%04d-%02d-%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
    myGLCD.drawString((char *)tbuf, 0, 64, 2);

    LED_TOGGLE();  
    delay(1000);

    /* ESP32Time other function interfaces, you can use the Serial.println function to print
    getTime()           //  (String) 15:24:38
    getDate()           //  (String) Sun, Jan 17 2021
    getDate(true)       //  (String) Sunday, January 17 2021
    getDateTime()       //  (String) Sun, Jan 17 2021 15:24:38
    getDateTime(true)   //  (String) Sunday, January 17 2021 15:24:38
    getTimeDate()       //  (String) 15:24:38 Sun, Jan 17 2021
    getTimeDate(true)   //  (String) 15:24:38 Sunday, January 17 2021

    getMicros()         //  (unsigned long) 723546
    getMillis()         //  (unsigned long) 723
    getEpoch()          //  (unsigned long) 1609459200
    getLocalEpoch()     //  (unsigned long) 1609459200 // local epoch without offset
    getSecond()         //  (int)     38    (0-59)
    getMinute()         //  (int)     24    (0-59)
    getHour()           //  (int)     3     (0-12)
    getHour(true)       //  (int)     15    (0-23)
    getAmPm()           //  (String)  pm
    getAmPm(false)      //  (String)  PM
    getDay()            //  (int)     17    (1-31)
    getDayofWeek()      //  (int)     0     (0-6)
    getDayofYear()      //  (int)     16    (0-365)
    getMonth()          //  (int)     0     (0-11)
    getYear()           //  (int)     2021
    */
}
