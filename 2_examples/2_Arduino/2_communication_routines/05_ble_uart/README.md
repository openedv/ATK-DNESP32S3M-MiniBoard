## ble_scan example

### 1 Brief

The main function of this code is to learn how to use ble.

### 2 Hardware Hookup

The hardware resources used in this experiment are:

- LED - IO1
- UART0
	- TXD0 - IO43
	- RXD0 - IO44
- SPI_LCD
	- CS - IO39
	- SCK - IO12
	- SDA - IO11
	- DC - IO40
	- BL - IO41
	- RST - IO38

The BLE is an internal peripheral, so there is no corresponding connection schematic diagram.

### 3 Running

#### 3.1 Compile & Download

**1 Compilation process**

- Connect the UART interface in the upper left corner of the image to the computer using a USB cable
- 05_ble_client routine opened using Arduino IDE2
- Select Target Board (① Arduino IDE: Tools --> Board --> esp32 --> ESP32S3 Dev Module)
- Select UART port number (② Arduino IDE: Tools --> port --> COMx)
- Set ESP32S3 parameter (③ Arduino IDE: Tools --> esp32s3 parameter(please see led example))
- Compile arduino project (④ Arduino IDE: "Sketch --> Verify/Compile" or "Ctrl+R" or "Verify button")
- Wait for compilation success (⑤ Arduino IDE: Output Window --> show "Done compiling")

**2 Download process**

- Download (⑥ Arduino IDE: "Sketch --> Upload" or "Ctrl+U" or "Upload button")

#### 3.2 Phenomenon

After normal operation, create a BLE server, once we use the Bluetooth assistant to connect, the connection is successful, will receive periodic notifications. The lcd displays the information sent by the Bluetooth assistant.

wait for the client to connect

<img src="../../../../1_docs/3_figures/examples/wifi_ble/ble_uart.png" style="zoom:50%;" />

Bluetooth Assistant connects to ble and receives and sends data

![](../../../../1_docs/3_figures/examples/wifi_ble/bluetooth_assistant.png)

lcd displays the received data

<img src="../../../../1_docs/3_figures/examples/wifi_ble/ble_uart1.png" style="zoom:50%;" />