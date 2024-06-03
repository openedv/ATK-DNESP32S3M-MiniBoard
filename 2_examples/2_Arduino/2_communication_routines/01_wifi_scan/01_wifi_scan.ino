/**
 ****************************************************************************************************
 * @file        01_wifi_scan.ino
 * @author      ALIENTEK
 * @brief       WIFI_SCAN example
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
#include "WiFi.h"


TFT_eSPI myGLCD = TFT_eSPI();       /* define the TFT eSPI object myGLCD */

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
    myGLCD.fillScreen(TFT_WHITE);      

    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("WIFI SCAN TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    WiFi.mode(WIFI_STA);    /* wifi mode is STA mode */
    WiFi.disconnect();      /* disconnect any previous WIFI connections */
    delay(100);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    myGLCD.setTextColor(TFT_BLUE, TFT_WHITE);
    myGLCD.drawString("Scan start", 10, 48, 2);
    int network_cnt = WiFi.scanNetworks();  /* start scanning for available networks */
    myGLCD.drawString("Scan done ", 10, 48, 2);

    if (network_cnt == 0) 
    {
        myGLCD.drawString("no wifi networks found ", 10, 64, 2);
    }
    else 
    {
        myGLCD.drawNumber(network_cnt, 5, 64, 2);
        myGLCD.drawString("networks found", 30, 64, 2);

        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        
        for (int i = 0; i < network_cnt; ++i) 
        {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");

            switch (WiFi.encryptionType(i))
            {
                case WIFI_AUTH_OPEN:
                    Serial.print("open");
                    break;
                case WIFI_AUTH_WEP:
                    Serial.print("WEP");
                    break;
                case WIFI_AUTH_WPA_PSK:
                    Serial.print("WPA");
                    break;
                case WIFI_AUTH_WPA2_PSK:
                    Serial.print("WPA2");
                    break;
                case WIFI_AUTH_WPA_WPA2_PSK:
                    Serial.print("WPA+WPA2");
                    break;
                case WIFI_AUTH_WPA2_ENTERPRISE:
                    Serial.print("WPA2-EAP");
                    break;
                case WIFI_AUTH_WPA3_PSK:
                    Serial.print("WPA3");
                    break;
                case WIFI_AUTH_WPA2_WPA3_PSK:
                    Serial.print("WPA2+WPA3");
                    break;
                case WIFI_AUTH_WAPI_PSK:
                    Serial.print("WAPI");
                    break;
                default:
                    Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    WiFi.scanDelete();
    delay(5000);
    myGLCD.fillRect(0, 64, 160, 16, TFT_WHITE);
}
