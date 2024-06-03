/**
 ****************************************************************************************************
 * @file        03_wifi_client.ino
 * @author      ALIENTEK
 * @brief       WIFI_CLIENT example
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
#include <WiFiMulti.h>      


TFT_eSPI myGLCD = TFT_eSPI();     /* define the TFT eSPI object myGLCD */
WiFiMulti WiFiMulti;

char* ssid     = "ALIENTEK-YF";   
char* password = "15902020353";   

/**
 * @brief    Initialize function
 * @param    None
 * @retval   None
 */
void setup() 
{
    char ip_buf[20];
    uint8_t i = 0;
	
    led_init();               /* led initialization */
    uart_init(0, 115200);     /* uart0 initialization */
    myGLCD.init();            /* lcd initialization */
    myGLCD.setRotation(1);    /* set the orientation of the screen (landscape) */
    myGLCD.fillScreen(TFT_WHITE);      

    myGLCD.setTextColor(TFT_RED, TFT_WHITE);
    myGLCD.drawString("ESP32-S3", 10, 0, 2);
    myGLCD.drawString("WIFI CLIENT TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    WiFiMulti.addAP(ssid, password);  /* start a network connection */
    myGLCD.drawString("WIFI_NAME:", 0, 48, 2);
    myGLCD.drawString(ssid, 80, 48, 2);

    while(WiFiMulti.run() != WL_CONNECTED)  /* waitting for connecting network */
    {
        delay(200);
        myGLCD.drawChar('.', 10 + 8 * i, 64, 2);
        i++;
    }

    myGLCD.fillRect(0, 48, 160, 16, TFT_WHITE);
    myGLCD.drawString("WiFi connected.", 0, 48, 2);
    delay(1000);
    myGLCD.fillRect(0, 48, 160, 16, TFT_WHITE);
    myGLCD.drawString("IP address:", 0, 48, 2);

    sprintf(ip_buf, "%s", WiFi.localIP().toString().c_str());       
    myGLCD.drawString(ip_buf, 68, 48, 2);
    // Serial.println(ip_buf);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    const uint16_t port = 80;       
    char *host = "192.168.1.247";    
    WiFiClient client;

    myGLCD.fillRect(0, 64, 160, 16, TFT_WHITE);
    myGLCD.drawString("Connect to", 0, 64, 2);
    myGLCD.drawString(host, 76, 64, 2);

    if (!client.connect(host, port))  /* connect to a network server */
    {
        myGLCD.fillRect(0, 64, 160, 16, TFT_WHITE);
        myGLCD.drawString("Connection failed.", 0, 64, 2);
        Serial.println("Waiting 5 seconds before retrying...");
        delay(3000);
        return;
    }

    // client.print("hello server!This is ESP32-S3\n\n");    /* send data to the server */
    client.print("GET /index.html HTTP/1.1\n\n");         /* send a request to the server */

    int maxloops = 0;
    
    while (!client.available() && maxloops < 30000)  /* waitting for a reply from the server */
    {
        maxloops++;
        delay(1); 
    }

    if (client.available() > 0)                     /* whether the server has data */
    {
        String line = client.readStringUntil('\r'); /* read back a line from the server */
        Serial.print("Read: ");
        Serial.println((char *)line.c_str());
    }
    else
    {
        Serial.println("client.available() timed out ");
    }

    Serial.println("Closing connection.");
    client.stop();    /* disconnect */

    Serial.println("Waiting 3 seconds before restarting.");
    delay(3000);
}
