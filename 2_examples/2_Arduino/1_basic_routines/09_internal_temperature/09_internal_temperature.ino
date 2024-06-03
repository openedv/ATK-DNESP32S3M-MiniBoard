/**
 ****************************************************************************************************
 * @file        09_internal_temperature.ino
 * @author      ALIENTEK
 * @brief       INTERNAL_TEMPERATURE example
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
#include "driver/temp_sensor.h"


TFT_eSPI myGLCD = TFT_eSPI();   /* define the TFT eSPI object myGLCD */

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

    temp_sensor_config_t temp_sensor = {    
        .dac_offset = TSENS_DAC_L2,         /* Measuring range: -10 ~ 80, error <1 */
        .clk_div = 6,
    };
    temp_sensor_set_config(temp_sensor);    /* temperature sensor configuration */
    temp_sensor_start();                    /* begin temperature measurement */

    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("TEMP TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    myGLCD.drawString("temperature:         C", 10, 48, 2);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    float tsens_out;
    temp_sensor_read_celsius(&tsens_out);   
    // Serial.printf("%.2f\r\n", tsens_out);

    myGLCD.setTextColor(TFT_BLUE, TFT_WHITE);
    myGLCD.drawFloat(tsens_out, 2, 96, 48, 2);

    LED_TOGGLE(); 
    delay(500);
}
