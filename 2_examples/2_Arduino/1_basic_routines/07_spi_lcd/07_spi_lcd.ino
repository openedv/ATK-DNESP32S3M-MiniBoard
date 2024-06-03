/**
 ****************************************************************************************************
 * @file        07_spi_lcd.ino
 * @author      ALIENTEK
 * @brief       SPILCD example
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

TFT_eSPI myGLCD = TFT_eSPI();       /* define the TFT eSPI object myGLCD */

uint8_t x = 0;  

/**
 * @brief    Initialize function
 * @param    None
 * @retval   None
 */
void setup() 
{
    led_init();               /* led initialization */
    uart_init(0, 115200);     /* uart0 initialization */
    myGLCD.init();            /* lcd initialization */
    myGLCD.setRotation(1);    /* set the orientation of the screen (landscape) */
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    switch (x)
    {
        case 0:
            myGLCD.fillScreen(TFT_WHITE);
            break;

        case 1:
            myGLCD.fillScreen(TFT_BLACK);
            break;

        case 2:
            myGLCD.fillScreen(TFT_BLUE);
            break;

        case 3:
            myGLCD.fillScreen(TFT_RED);
            break;

        case 4:
            myGLCD.fillScreen(TFT_GREEN);
            break;

        case 5:
            myGLCD.fillScreen(TFT_YELLOW);
            break;

        case 6:
            myGLCD.fillScreen(TFT_CYAN);
            break;

        case 7:
            myGLCD.fillScreen(TFT_LIGHTGREY);
            break;
    }
    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("LCD TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);
    x++;

    if (x == 8)
    {
        x = 0;
    }

    LED_TOGGLE();  
    delay(1000);
}
