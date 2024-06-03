/**
 ****************************************************************************************************
 * @file        05_ble_uart.ino
 * @author      ALIENTEK
 * @brief       BLE_UART example
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
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


TFT_eSPI myGLCD = TFT_eSPI();           /* define the TFT eSPI object myGLCD */
BLEServer *pServer = NULL;              
BLECharacteristic * pTxCharacteristic; 
BLECharacteristic * pRxCharacteristic;  
bool deviceConnected = false;           
bool oldDeviceConnected = false;        
uint8_t txValue = 0;
char rec_buf[20];

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"   
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   

class MyServerCallbacks: public BLEServerCallbacks   
{
    void onConnect(BLEServer* pServer) 
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) 
    {
        deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0)
        {
            myGLCD.drawString("rec value:", 10, 64, 2);
            myGLCD.fillRect(80, 64, 80, 16, TFT_WHITE);
            for (int i = 0; i < rxValue.length(); i++)
            {
                rec_buf[i] = rxValue[i];
            }
            myGLCD.drawString(rec_buf, 80, 64, 2);
            // Serial.println(rec_buf);
            memset(rec_buf, 0, 20);
        }
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
    myGLCD.drawString("BLE UART TEST", 10, 16, 2);
    myGLCD.drawString("ATOM@ALIENTEK", 10, 32, 2);

    BLEDevice::init("ESP32-S3 BLE Service");          /* create a BLE device */
    pServer = BLEDevice::createServer();              /* create a BLE service */
    pServer->setCallbacks(new MyServerCallbacks());   /* set callback */

    BLEService *pService = pServer->createService(SERVICE_UUID); /* creating a Bluetooth Server */

    /* create send feature, add descriptor, set notification permissions/Create Receive feature, set callback function, set writable permissions */
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks()); 

    pService->start();  /* start service */

    pServer->getAdvertising()->start();   /* start broadcast */
    myGLCD.drawString("Waiting client connect", 10, 48, 2);

    delay(500);
}

/**
 * @brief    Loop function
 * @param    None
 * @retval   None
 */
void loop() 
{
    if (deviceConnected)   /* connect to device */
    {
        // myGLCD.drawString("client connected      ", 10, 48, 2);
        pTxCharacteristic->setValue(&txValue, 1);   /* Set the value to be sent to 1 */
        myGLCD.drawString("notify value:            ", 10, 48, 2);
        pTxCharacteristic->notify();                /* broadcast txValue++ */
        myGLCD.drawNumber(txValue, 104, 48, 2);
        txValue++;
        delay(100);                                 /* if too many packets are sent, the Bluetooth stack will become congested */ 
    }

    if (!deviceConnected && oldDeviceConnected)     /* disconnect */
    {
        delay(500);                                 /* the Bluetooth stack has a chance to be ready */
        pServer->startAdvertising();                /* restart broadcast */
        myGLCD.drawString("start advertising...", 10, 48, 2);
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected && !oldDeviceConnected)     /* connecting */
    {
        oldDeviceConnected = deviceConnected;
    }
}
