/**
 ****************************************************************************************************
 * @file        10_spi_sdcard.ino
 * @author      ALIENTEK
 * @brief       SPI_SDCARD example
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
#include "key.h"
#include "uart.h"
#include <SPI.h>
#include "TFT_eSPI.h"
#include "spi_sdcard.h"
#include <SD.h>

TFT_eSPI myGLCD = TFT_eSPI();   /* define the TFT eSPI object myGLCD */

/**
 * @brief    Initialize function
 * @param    None
 * @retval   None
 */
void setup() 
{
    led_init();                             /* led initialization */
    key_init();                             /* key initialization */
    uart_init(0, 115200);                   /* uart0 initialization */

    myGLCD.init();                          /* lcd initialization */
    myGLCD.setRotation(1);                  /* set the orientation of the screen (landscape) */
    myGLCD.fillScreen(TFT_WHITE);           /* lcd clear */
    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("SDCARD TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    while (sdcard_init())    
    {
        myGLCD.drawString("SD Card Error!", 10, 48, 2);
        delay(500);
        myGLCD.drawString("Please Check! ", 10, 48, 2);
        delay(500);
        LED_TOGGLE();       
    }

    myGLCD.drawString("SD Card OK    ", 10, 48, 2);
    myGLCD.drawString("SD Card Size:       MB", 10, 64, 2);
    myGLCD.setTextColor(TFT_BLUE, TFT_WHITE);
    myGLCD.drawNumber(SD.cardSize() / (1024 * 1024), 13 * 8, 64, 2);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    if (KEY == 0)
    {
        sd_test();
    }

    LED_TOGGLE();    
    delay(500);
}
