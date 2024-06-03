/**
 ****************************************************************************************************
 * @file        04_ble_scan.ino
 * @author      ALIENTEK
 * @brief       BLE_SCAN example
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
#include <BLEDevice.h>            
#include <BLEUtils.h>
#include <BLEScan.h>              
#include <BLEAdvertisedDevice.h>  


TFT_eSPI myGLCD = TFT_eSPI();     /* define the TFT eSPI object myGLCD */
int scanTime = 5;                 
BLEScan* pBLEScan;                

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks  /* BLE broadcast callback function (called every time a scan is made to a broadcast device) */
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
        Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());  
    }
};

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
    myGLCD.drawString("BLE SCAN TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    myGLCD.drawString("Scanning...", 10, 48, 2);
    BLEDevice::init("ESP BLEDevice");   /* create a BLE device */
    pBLEScan = BLEDevice::getScan();    /* create a new scan */
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());  /* initializes the callback function */
    pBLEScan->setActiveScan(true);      /* active scanning consumes more energy but gets results faster */
    pBLEScan->setInterval(100);         /* set scan interval */
    pBLEScan->setWindow(99);            /* set window size */
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);   /* Start the BLE scan and call the callback function when the scan reaches the broadcast device */
    myGLCD.drawString("Devices found:", 10, 64, 2);
    myGLCD.drawNumber(foundDevices.getCount(), 112, 64, 2);
    myGLCD.drawString("Scan done!    ", 10, 48, 2);
    pBLEScan->clearResults();   /* Removes the result from the BLEScan buffer to free memory */   
    delay(2000);
}
