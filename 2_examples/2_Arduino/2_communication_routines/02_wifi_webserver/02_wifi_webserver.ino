/**
 ****************************************************************************************************
 * @file        02_wifi_webserver.ino
 * @author      ALIENTEK
 * @brief       WIFI_WEBSERVER example
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


TFT_eSPI myGLCD = TFT_eSPI();     /* define the TFT eSPI object myGLCD */
char* ssid     = "ALIENTEK-YF";   
char* password = "15902020353";   
WiFiServer server(80);            
WiFiClient client;                
void webpage_display(void);       

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
    myGLCD.drawString("WIFI WEBSERVER TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    WiFi.begin(ssid, password);   /* linking network */
    myGLCD.drawString("WIFI_NAME:", 0, 48, 2);
    myGLCD.drawString(ssid, 80, 48, 2);

    while (WiFi.status() != WL_CONNECTED)   /* waitting for connecting network */
    {
        delay(500);
        myGLCD.drawChar('.', 10 + 8 * i, 64, 2);
        i++;
    }
    myGLCD.fillRect(0, 48, 160, 16, TFT_WHITE);
    myGLCD.drawString("WiFi connected.", 0, 48, 2);
    myGLCD.drawString("IP address:", 0, 64, 2);

    sprintf(ip_buf, "%s", WiFi.localIP().toString().c_str());   /* start the Internet of Things web server built by ESP32S3 */
    myGLCD.drawString(ip_buf, 68, 64, 2);

    server.begin();   /* wait for the client to connect to the IP address through the web */
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    client = server.available();   /* detects whether there are active client connections on the server side */

    if (client)     /* client connect? */
    {                             
        String currentLine = "";              /* create a string to hold incoming data from the client */
        while (client.connected())            /* check whether the device is successfully connected to the server */
        { 
            // Serial.println("Client is connected");
            if (client.available())           /* check whether the network client receives messages from the server */
            {             
                char c = client.read();       /* read the message from the server */
                //Serial.write(c);   
                if (c == '\n')                /* used to determine whether the received web page access data is complete */
                {   
                    /* If the current action is empty, there are two newlines in a line. 
                     * This is the end of the client's HTTP request, so send a response: */            
                    if (currentLine.length() == 0) 
                    {
                        webpage_display();
                        break;
                    } 
                    else                /* if there is a newline, then clear currentLine */ 
                    {    
                        currentLine = ""; 
                    }
                } 
                else if (c != '\r')     /* if there is no carriage return */
                {  
                    currentLine += c;   /* add it to the end of currentLine */  
                }

                /* check that the client request is "GET /H" or "GET /L" */
                if (currentLine.endsWith("GET /LED_ON")) 
                {
                    LED(0);   /* GET /H turn on light */
                    Serial.println("LED ON");
                }

                if (currentLine.endsWith("GET /LED_OFF")) 
                {
                    LED(1);   /* GET /L turn on light */
                    Serial.println("LED OFF");
                }
           }
       }
       client.stop();  /* disconnect */
    }
}

/**
 * @brief    web data content
 * @param    None
 * @retval   None
 */
void webpage_display(void)
{
    /* HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and 
     * a content type so that the client knows what comes next, followed by a blank line: */
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");   /* browser response data type is text data */
    client.println();

    client.print("<!DOCTYPE html>");
    client.print("<html>");
    client.print("<head>");
    client.print("<title>ATK ESP32-S3 Web Server</title>");
    client.print("</head>");

    client.print("<body>");
    client.print("<h1> ESP32-S3 Web Server </h1>");
    client.print("<p> LED State </p>");    
    client.print("<p> <a href=\"/LED_ON\"> <button> ON </button> </p>");   
    client.print("<p> <a href=\"/LED_OFF\"> <button> OFF </button> </p>");   
    client.print("</body>");
    client.print("</html>");

    /* the HTTP response ends with another blank line */
    client.println();
}